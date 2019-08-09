module P = ReludeParse.Parser;
open P.Infix;

type t =
  | QueryValue(string);

let make = str => QueryValue(str);

let parser: P.t(t) =
  P.many1(P.anyCharNotIn(["#", "&"]))
  <#> Relude.Nel.toList
  <#> Relude.List.String.join
  <#> make;