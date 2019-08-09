module P = ReludeParse.Parser;
open P.Infix;

module QueryKey = ReludeURL_QueryKey;
module QueryValue = ReludeURL_QueryValue;

type t = {
  key: QueryKey.t,
  values: list(QueryValue.t),
};

let make = (key, values) => {key, values};

let makeOne = (key, value) => {key, values: [value]};

let parser: P.t(t) =
  makeOne <$> QueryKey.parser <* P.str("=") <*> QueryValue.parser;