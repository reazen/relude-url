module P = ReludeParse.Parser;
open P.Infix;

type t =
  | Password(string);

let make = str => Password(str);

let parser: P.t(t) =
  P.many1(P.anyCharNotIn(["@"]))
  <#> Relude.Nel.toList
  <#> Relude.List.String.join
  <#> make;

let show = (Password(str)) => str;

module Show: BsBastet.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};
