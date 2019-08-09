module P = ReludeParse.Parser;
open P.Infix;

type t =
  | Fragment(string);

let make = str => Fragment(str);

let parser: P.t(t) = P.many(P.anyChar) <#> Relude.List.String.join <#> make;