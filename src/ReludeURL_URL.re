module P = ReludeParse.Parser;
open P.Infix;

module Scheme = ReludeURL_Scheme;
module Authority = ReludeURL_Authority;
module Path = ReludeURL_Path;
module Query = ReludeURL_Query;
module Fragment = ReludeURL_Fragment;

type t = {
  scheme: Scheme.t,
  authority: option(Authority.t),
  path: Path.t,
  query: option(Query.t),
  fragment: option(Fragment.t),
};

let make = (scheme, authority, path, query, fragment) => {
  scheme,
  authority,
  path,
  query,
  fragment,
};

// URI = scheme:[//authority]path[?query][#fragment]
// authority = [userinfo@]host[:port]

let parser: P.t(t) =
  make
  <$> Scheme.parser
  <* P.str(":")
  <*> P.opt(P.str("//") *> Authority.parser)
  <*> Path.parser
  <*> P.opt(P.str("?") *> Query.parser)
  <*> P.opt(P.str("#") *> Fragment.parser)
  <* P.eof;