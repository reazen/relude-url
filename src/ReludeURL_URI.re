open Relude.Globals;

module P = ReludeParse.Parser;
open P.Infix;

module Scheme = ReludeURL_Scheme;
module Authority = ReludeURL_Authority;
module UserInfo = ReludeURL_UserInfo;
module Username = ReludeURL_Username;
module Password = ReludeURL_Password;
module Hostname = ReludeURL_Hostname;
module Port = ReludeURL_Port;
module Path = ReludeURL_Path;
module PathSegment = ReludeURL_PathSegment;
module Query = ReludeURL_Query;
module QueryKey = ReludeURL_QueryKey;
module QueryParam = ReludeURL_QueryParam;
module QueryValue = ReludeURL_QueryValue;
module Fragment = ReludeURL_Fragment;
module Component = ReludeURL_URI_Component;

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

let makeWithLabels =
    (
      ~scheme: Scheme.t,
      ~authority: option(Authority.t)=?,
      ~path: Path.t,
      ~query: option(Query.t)=?,
      ~fragment: option(Fragment.t)=?,
      (),
    ) =>
  make(scheme, authority, path, query, fragment);

let scheme: t => Scheme.t = ({scheme}) => scheme;

let authority: t => option(Authority.t) = ({authority}) => authority;

let userInfo: t => option(UserInfo.t) =
  ({authority}) =>
    authority
    |> Option.flatMap((authority: Authority.t) => authority.userInfo);

let username: t => option(Username.t) =
  url =>
    url |> userInfo |> Option.map((userInfo: UserInfo.t) => userInfo.username);

let password: t => option(Password.t) =
  url =>
    url
    |> userInfo
    |> Option.flatMap((userInfo: UserInfo.t) => userInfo.password);

let hostname: t => option(Hostname.t) =
  ({authority}) =>
    authority |> Option.map((authority: Authority.t) => authority.hostname);

let port: t => option(Port.t) =
  ({authority}) =>
    authority |> Option.flatMap((authority: Authority.t) => authority.port);

let path: t => Path.t = ({path}) => path;

let pathSegments: t => list(PathSegment.t) =
  ({path: Path(segments)}) => segments;

let query: t => option(Query.t) = ({query}) => query;

let fragment: t => option(Fragment.t) = ({fragment}) => fragment;

let withScheme: (Scheme.t, t) => t = (scheme, uri) => {...uri, scheme};

let withAuthority: (Authority.t, t) => t =
  (authority, uri) => {...uri, authority: Some(authority)};

let withAuthorityOpt: (option(Authority.t), t) => t =
  (authority, uri) => {...uri, authority};

let withPath: (Path.t, t) => t = (path, uri) => {...uri, path};

let withQuery: (Query.t, t) => t =
  (query, uri) => {...uri, query: Some(query)};

let withQueryOpt: (option(Query.t), t) => t =
  (query, uri) => {...uri, query};

let getQueryParam: (QueryKey.t, t) => option(list(QueryValue.t)) =
  (key, uri) => {
    uri |> query |> Option.flatMap(query => query |> Query.get(key));
  };

let setQueryParam: (QueryParam.t, t) => t =
  (param, uri) => {
    let newQuery =
      uri |> query |> Option.getOrElse(Query.empty()) |> Query.set(param);
    uri |> withQuery(newQuery);
  };

let appendQueryParam: (QueryParam.t, t) => t =
  (param, uri) => {
    let newQuery =
      uri |> query |> Option.getOrElse(Query.empty()) |> Query.append(param);
    uri |> withQuery(newQuery);
  };

let removeQueryParam: (QueryKey.t, t) => t =
  (key, uri) => {
    let newQuery =
      uri |> query |> Option.map(query => query |> Query.remove(key));
    uri |> withQueryOpt(newQuery);
  };

let withFragment: (Fragment.t, t) => t =
  (fragment, uri) => {...uri, fragment: Some(fragment)};

let withFragmentOpt: (option(Fragment.t), t) => t =
  (fragment, uri) => {...uri, fragment};

/**
 * A parser for a URL
 *
 * Here is the general parsing strategy:
 *
 * URI = scheme:[//authority]path[?query][#fragment]
 * authority = [userinfo@]host[:port]
 */
let parser: P.t(t) =
  make
  <$> Scheme.parser
  <* P.str(":")
  <*> P.opt(P.str("//") *> Authority.parser)
  <*> Path.parser
  <*> P.opt(P.str("?") *> Query.parser)
  <*> P.opt(P.str("#") *> Fragment.parser)
  <* P.eof;

let show: t => string =
  ({scheme, authority, path, query, fragment}) =>
    Scheme.show(scheme)
    ++ ":"
    ++ (
      authority
      |> Option.fold("", authority => "//" ++ Authority.show(authority))
    )
    ++ Path.show(path)
    ++ (query |> Option.fold("", query => "?" ++ Query.show(query)))
    ++ (
      fragment |> Option.fold("", fragment => "#" ++ Fragment.show(fragment))
    );

module Show: BsBastet.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};
