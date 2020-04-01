module P = ReludeParse.Parser;
open P.Infix;

type t =
  | Port(int);

let make = i => Port(i);

let parser = P.anyUnsignedInt <#> make;

let show = (Port(i)) => Relude.Int.show(i);

module Show: BsBastet.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};
