module P = ReludeParse.Parser;
open P.Infix;

type t =
  | Port(int);

let make = i => Port(i);

let parser = P.anyUnsignedInt <#> make;