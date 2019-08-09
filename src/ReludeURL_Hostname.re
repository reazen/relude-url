module P = ReludeParse.Parser;
open P.Infix;

type t =
  | Hostname(string);

let make = str => Hostname(str);

let parser: P.t(t) =
  P.many1(P.anyCharNotIn([":", "/"]))
  <#> Relude.Nel.toList
  <#> Relude.List.String.join
  <#> make;
