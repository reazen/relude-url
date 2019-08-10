module P = ReludeParse.Parser;
open P.Infix;

module PathSegment = ReludeURL_PathSegment;

type t =
  | Path(list(PathSegment.t));

let make = segments => Path(segments);

let parser: P.t(t) =
  P.opt(P.str("/")) *> PathSegment.parser |> P.sepBy(P.str("/")) <#> make;

let show = (Path(segments)) =>
  segments
  |> Relude.List.map(PathSegment.show)
  |> Relude.List.String.joinWith("/");

module Show: BsAbstract.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};