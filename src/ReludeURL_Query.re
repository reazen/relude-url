module P = ReludeParse.Parser;
open P.Infix;

module QueryKey = ReludeURL_QueryKey;
module QueryValue = ReludeURL_QueryValue;
module QueryParam = ReludeURL_QueryParam;

type t =
  | Query(list(QueryParam.t));

let make = params => Query(params);

let rec addOne: (QueryKey.t, QueryValue.t, t) => t =
  (key, value, query) => {
    switch (query) {
    | Query([{key: existingKey, values: existingValues} as head, ...tail]) =>
      if (existingKey == key) {
        Query([
          {
            key: existingKey,
            values: existingValues |> Relude.List.append(value),
          },
          ...tail,
        ]);
      } else {
        let Query(params) = addOne(key, value, Query(tail));
        Query([head, ...params]);
      }
    | Query([]) => Query([{key, values: [value]}])
    };
  };

let rec add: (QueryKey.t, list(QueryValue.t), t) => t =
  (key, values, query) => {
    switch (query) {
    | Query([{key: existingKey, values: existingValues} as head, ...tail]) =>
      if (existingKey == key) {
        Query([
          {
            key: existingKey,
            values: Relude.List.concat(existingValues, values),
          },
          ...tail,
        ]);
      } else {
        let Query(params) = add(key, values, Query(tail));
        Query([head, ...params]);
      }
    | Query([]) => Query([{key, values}])
    };
  };

let normalize: t => t =
  (Query(params)) => {
    params
    |> Relude.List.foldLeft(
         (acc, {QueryParam.key, QueryParam.values}) =>
           add(key, values, acc),
         Query([]),
       );
  };

let parser: P.t(t) =
  QueryParam.parser |> P.sepBy(P.str("&")) <#> make <#> normalize;