module P = ReludeParse.Parser;
open P.Infix;

// TODO: some way to tag the value as encoded/decoded?
// Maybe a GADT referencing a type-level boolean?

type t =
  | QueryValue(string);

let make = (str: string): t => QueryValue(str);

let parser: P.t(t) =
  P.many1(P.anyCharNotIn(["#", "&"]))
  <#> Relude.Nel.toList
  <#> Relude.List.String.join
  <#> make;

let show = (QueryValue(str)) => str;

module Show: BsBastet.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};
