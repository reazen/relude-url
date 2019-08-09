module P = ReludeParse.Parser;
open P.Infix;

type t =
  | QueryKey(string);

let make = str => QueryKey(str);

let show = (QueryKey(str)) => str;

let parser: P.t(t) =
  P.many1(P.anyCharNotIn(["="]))
  <#> Relude.Nel.toList
  <#> Relude.List.String.join
  <#> make;