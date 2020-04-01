open Relude.Globals;

module P = ReludeParse.Parser;
open P.Infix;

module QueryKey = ReludeURL_QueryKey;
module QueryValue = ReludeURL_QueryValue;
module QueryParam = ReludeURL_QueryParam;

// TODO: it might be cool to make the QueryParams implementation more pluggable, but maybe it's
// not worth it.  It doesn't seem like anyone would likely want to customize this - if there is
// a better implementation (which there probably is), it should probably just be the only option.

/**
 * The point of this is to have a clear abstraction for the internal implementation of the Query
 * map
 */
module type QUERY_MAP = {
  type t;
  let make: (~hintSize: int) => t;
  let get: (QueryKey.t, t) => option(list(QueryValue.t));
  let set: (QueryKey.t, list(QueryValue.t), t) => t;
  let append: (QueryKey.t, list(QueryValue.t), t) => t;
  let remove: (QueryKey.t, t) => t;
  let fromArray: array(QueryParam.t) => t;
  let fromList: list(QueryParam.t) => t;
  let toArray: t => array(QueryParam.t);
  let toList: t => list(QueryParam.t);
};

/**
 * The "internal" implementation of how query params are stored/modified/etc.
 *
 * It's currently using a Belt.HashMap.String that keeps track of the rank for each key,
 * to preserve key ordering.
 */
module QueryMap: QUERY_MAP = {
  type value = {
    rank: int,
    key: QueryKey.t,
    values: list(QueryValue.t),
  };

  type t = Belt.HashMap.String.t(value);

  let make = (~hintSize: int): t => Belt.HashMap.String.make(~hintSize);

  let nextRank = (map: t): int =>
    map
    |> Belt.HashMap.String.valuesToArray
    |> Array.map(({rank}) => rank)
    |> Array.Int.max
    |> Option.getOrElse(-1)
    |> (max => max + 1);

  let getValue = (key: QueryKey.t, map: t): option(value) =>
    Belt.HashMap.String.get(map, QueryKey.show(key));

  let setValue = (key: QueryKey.t, mapValue: value, map: t): t => {
    Belt.HashMap.String.set(map, QueryKey.show(key), mapValue);
    map;
  };

  let get = (key: QueryKey.t, map: t): option(list(QueryValue.t)) =>
    map |> getValue(key) |> Option.map(value => value.values);

  let set = (key: QueryKey.t, values: list(QueryValue.t), map: t): t => {
    map
    |> getValue(key)
    |> Option.map(({rank}) => map |> setValue(key, {rank, key, values}))
    |> Option.getOrElseLazy(_ =>
         map |> setValue(key, {rank: map |> nextRank, key, values})
       );
  };

  let append = (key: QueryKey.t, values: list(QueryValue.t), map: t): t => {
    map
    |> getValue(key)
    |> Option.map(({rank, key, values: existingValues}) =>
         map
         |> setValue(
              key,
              {rank, key, values: List.concat(existingValues, values)},
            )
       )
    |> Option.getOrElseLazy(_ =>
         map |> setValue(key, {rank: map |> nextRank, key, values})
       );
  };

  let remove = (key: QueryKey.t, map: t): t => {
    Belt.HashMap.String.remove(map, QueryKey.show(key));
    map;
  };

  let fromList: list(QueryParam.t) => t =
    params =>
      params
      |> List.foldLeft(
           (acc: t, param: QueryParam.t) =>
             append(param.key, param.values, acc),
           make(~hintSize=List.length(params)),
         );

  let fromArray: array(QueryParam.t) => t =
    params => params |> Array.toList |> fromList;

  let toArray: t => array(QueryParam.t) =
    map =>
      map
      |> Belt.HashMap.String.valuesToArray
      |> Array.sortWithInt((a: value, b: value) => a.rank - b.rank)
      |> Array.map(({key, values}) => ({key, values}: QueryParam.t));

  let toList: t => list(QueryParam.t) = toArray >> Array.toList;
};

/**
 * The type of the collection Query parameters
 *
 * TODO: this could be a module functor of QUERY_MAP, but it doesn't seem likely
 * that anyone would want to swap out the implementation.  We should just have
 * the best implementation here by default.
 */
type t =
  | Query(QueryMap.t);

/**
 * Creates a query from a QueryMap
 */
let make: QueryMap.t => t = map => Query(map);

/**
 * Creates an empty query map
 */
let empty: unit => t = () => Query(QueryMap.make(~hintSize=0));

/**
 * Creates a Query map from a list of QueryParam (key/value pairs)
 */
let fromList: list(QueryParam.t) => t =
  params => Query(QueryMap.fromList(params));

/**
 * Creates a Query map from an array of QueryParam (key/value pairs)
 */
let fromArray: array(QueryParam.t) => t =
  params => Query(QueryMap.fromArray(params));

/**
 * Gets the list of values for the given query key
 */
let get: (QueryKey.t, t) => option(list(QueryValue.t)) =
  (key, Query(map)) => {
    map |> QueryMap.get(key);
  };

/**
 * Gets the first value for the given query key
 */
let getFirst: (QueryKey.t, t) => option(QueryValue.t) =
  (key, map) => get(key, map) |> Relude.Option.flatMap(Relude.List.head);

/**
 * Sets (overwrites) the list of values for the given query key
 */
let set: (QueryParam.t, t) => t =
  ({key, values}, Query(map)) => Query(map |> QueryMap.set(key, values));

/**
 * Appends the value for the given query key to any existing values for the key
 */
let append: (QueryParam.t, t) => t =
  ({key, values}, Query(map)) =>
    Query(map |> QueryMap.append(key, values));

/**
 * Appends a list of values for the given query key to any existing values for the key
 */
let appendKeyValues: (QueryKey.t, list(QueryValue.t), t) => t =
  (key, values, query) => append({key, values}, query);

/**
 * Appends a single value for the given query key to any existing values for the key
 */
let appendKeyValue: (QueryKey.t, QueryValue.t, t) => t =
  (key, value, query) => append({key, values: [value]}, query);

/**
 * Removes a key and associated values from the query map
 */
let remove: (QueryKey.t, t) => t =
  (key, Query(map)) => {
    Query(map |> QueryMap.remove(key));
  };

let parser: P.t(t) = QueryParam.parser |> P.sepBy(P.str("&")) <#> fromList;

let show: t => string =
  (Query(impl)) =>
    impl
    |> QueryMap.toArray
    |> Array.map(QueryParam.show)
    |> Array.String.joinWith("&");

module Show: BsBastet.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};
