module P = ReludeParse.Parser;
open P.Infix;

module PathSegment = ReludeURL_PathSegment;

type t =
  | Path(list(PathSegment.t));

let make = segments => Path(segments);

let parser: P.t(t) =
  P.opt(P.str("/")) *> PathSegment.parser |> P.sepBy(P.str("/")) <#> make;