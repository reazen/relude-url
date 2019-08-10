module P = ReludeParse.Parser;
open P.Infix;

module QueryKey = ReludeURL_QueryKey;
module QueryValue = ReludeURL_QueryValue;

type t = {
  key: QueryKey.t,
  values: list(QueryValue.t),
};

let make = (key, values) => {key, values};

let make1 = (key, value) => {key, values: [value]};

let fromTuple = ((key, values)) => {key, values};

let fromTuple1 = ((key, value)) => {key, values: [value]};

let parser: P.t(t) =
  make1 <$> QueryKey.parser <* P.str("=") <*> QueryValue.parser;

let show = ({key, values}) => {
  values
  |> Relude.List.map(value =>
       QueryKey.show(key) ++ "=" ++ QueryValue.show(value)
     )
  |> Relude.List.String.joinWith("&");
};

module Show: BsAbstract.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};