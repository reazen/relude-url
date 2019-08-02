module P = ReludeParse.Parser;
open P.Infix;

module Scheme = {
  type t =
    | Scheme(string);

  let parser: P.t(t) =
    P.many(P.anyCharNotIn([":"]))
    <* P.str(":")
    <#> Relude.List.String.join
    <#> (str => Scheme(str))
    <?> "Expected URL scheme";
};

module Authority = {
  module Username = {
    type t =
      | Username(string);

    let parser: P.t(t) =
      P.many1(P.anyCharNotIn([":"]))
      <#> Relude.Nel.toList
      <#> Relude.List.String.join
      <#> (str => Username(str));
  };

  module Password = {
    type t =
      | Password(string);
  };

  module Host = {
    type t =
      | Host(string);
  };

  module Port = {
    type t =
      | Port(int);
  };

  module UserInfo = {
    type t = {
      username: Username.t,
      password: option(Password.t),
    };
  };

  type t = {
    userInfo: option(UserInfo.t),
    host: option(Host.t),
    port: option(Port.t),
  };
  //let parser: P.t(t) =
};

module Path = {
  module Segment = {
    type t =
      | Segment(string);
  };

  type t = {segments: list(Segment.t)};
};

module Query = {
  module Key = {
    type t =
      | Key(string);
  };

  module Value = {
    type t =
      | Value(string);
  };

  module Param = {
    type t = {
      key: Key.t,
      values: list(Value.t),
    };
  };

  type t = {params: list(Param.t)};
};

module Fragment = {
  type t =
    | Fragment(string);
};

type t = {
  scheme: option(Scheme.t),
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