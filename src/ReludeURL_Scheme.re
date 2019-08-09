module P = ReludeParse.Parser;
open P.Infix;

type t =
  | Scheme(string);

let make = str => Scheme(str);

let parser: P.t(t) =
  P.many(P.anyOf([P.anyAlphaOrDigit, P.str("-"), P.str("+"), P.str(".")]))
  <#> Relude.List.String.join
  <#> make
  <?> "Expected URL scheme";