module P = ReludeParse.Parser;
open P.Infix;

module PathSegment = ReludeURL_PathSegment;

type t =
  | Path(list(PathSegment.t));

let make: list(PathSegment.t) => t = segments => Path(segments);

let empty = Path([]);

let parser: P.t(t) =
  P.opt(P.str("/"))
  *> P.opt(P.tries(PathSegment.parser |> P.sepByOptEnd(P.str("/"))))
  <#> Relude.Option.getOrElse([])
  <#> make;

let show = (Path(segments)) =>
  if (segments |> Relude.List.isNotEmpty) {
    "/"
    ++ (
      segments
      |> Relude.List.map(PathSegment.show)
      |> Relude.List.String.joinWith("/")
    );
  } else {
    "";
  };

module Show: BsAbstract.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};