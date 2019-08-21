open Jest;
open Expect;

module Scheme = ReludeURL.Scheme;
module Authority = ReludeURL.Authority;
module UserInfo = ReludeURL.UserInfo;
module Username = ReludeURL.Username;
module Password = ReludeURL.Password;
module Hostname = ReludeURL.Hostname;
module Port = ReludeURL.Port;
module Path = ReludeURL.Path;
module PathSegment = ReludeURL.PathSegment;
module QueryKey = ReludeURL.QueryKey;
module QueryValue = ReludeURL.QueryValue;
module QueryParam = ReludeURL.QueryParam;
module Query = ReludeURL.Query;
module Fragment = ReludeURL.Fragment;
module URI = ReludeURL.URI;

let tests = [
  (
    "https://example.com",
    "https://example.com",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=Authority.fromHostname(Hostname.make("example.com")),
      ~path=Path.empty,
      (),
    ),
  ),
  (
    "https://example.com/",
    "https://example.com",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=Authority.fromHostname(Hostname.make("example.com")),
      ~path=Path.empty,
      (),
    ),
  ),
  (
    "https://user@example.com",
    "https://user@example.com",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromUsernameAndHostname(
          Username.make("user"),
          Hostname.make("example.com"),
        ),
      ~path=Path.empty,
      (),
    ),
  ),
  (
    "https://user@example.com/",
    "https://user@example.com",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromUsernameAndHostname(
          Username.make("user"),
          Hostname.make("example.com"),
        ),
      ~path=Path.empty,
      (),
    ),
  ),
  (
    "https://user:pass@example.com",
    "https://user:pass@example.com",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromCredentialsAndHostname(
          Username.make("user"),
          Password.make("pass"),
          Hostname.make("example.com"),
        ),
      ~path=Path.empty,
      (),
    ),
  ),
  (
    "https://user:pass@example.com/",
    "https://user:pass@example.com",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromCredentialsAndHostname(
          Username.make("user"),
          Password.make("pass"),
          Hostname.make("example.com"),
        ),
      ~path=Path.empty,
      (),
    ),
  ),
  (
    "https://example.com:80",
    "https://example.com:80",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromHostnameAndPort(
          Hostname.make("example.com"),
          Port.make(80),
        ),
      ~path=Path.empty,
      (),
    ),
  ),
  (
    "https://example.com:80/",
    "https://example.com:80",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromHostnameAndPort(
          Hostname.make("example.com"),
          Port.make(80),
        ),
      ~path=Path.empty,
      (),
    ),
  ),
  (
    "https://example.com:80/my/path",
    "https://example.com:80/my/path",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromHostnameAndPort(
          Hostname.make("example.com"),
          Port.make(80),
        ),
      ~path=Path.make([PathSegment.make("my"), PathSegment.make("path")]),
      (),
    ),
  ),
  (
    "https://example.com:80/my/path/",
    "https://example.com:80/my/path",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromHostnameAndPort(
          Hostname.make("example.com"),
          Port.make(80),
        ),
      ~path=Path.make([PathSegment.make("my"), PathSegment.make("path")]),
      (),
    ),
  ),
  (
    "https://example.com:80/my/path?key1=val1",
    "https://example.com:80/my/path?key1=val1",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromHostnameAndPort(
          Hostname.make("example.com"),
          Port.make(80),
        ),
      ~path=Path.make([PathSegment.make("my"), PathSegment.make("path")]),
      ~query=
        Query.fromList([
          QueryParam.make1(QueryKey.make("key1"), QueryValue.make("val1")),
        ]),
      (),
    ),
  ),
  (
    "https://example.com:80/my/path/?key1=val1",
    "https://example.com:80/my/path?key1=val1",
    URI.makeWithLabels(
      ~scheme=Scheme("https"),
      ~authority=
        Authority.fromHostnameAndPort(
          Hostname.make("example.com"),
          Port.make(80),
        ),
      ~path=Path.make([PathSegment.make("my"), PathSegment.make("path")]),
      ~query=
        Query.fromList([
          QueryParam.make1(QueryKey.make("key1"), QueryValue.make("val1")),
        ]),
      (),
    ),
  ),
  (
    "http://user:pass@example.com:99/my/dir?key1=val1&key2=val2&key1=val3#frag",
    "http://user:pass@example.com:99/my/dir?key1=val1&key1=val3&key2=val2#frag",
    URI.makeWithLabels(
      ~scheme=Scheme.make("http"),
      ~authority=
        Authority.makeWithLabels(
          ~userInfo=
            UserInfo.makeWithLabels(
              ~username=Username.make("user"),
              ~password=Password.make("pass"),
              (),
            ),
          ~hostname=Hostname.make("example.com"),
          ~port=Port.make(99),
          (),
        ),
      ~path=Path.make([PathSegment.make("my"), PathSegment.make("dir")]),
      ~query=
        Query.fromList([
          QueryParam.make(
            QueryKey.make("key1"),
            [QueryValue.make("val1"), QueryValue.make("val3")],
          ),
          QueryParam.make(
            QueryKey.make("key2"),
            [QueryValue.make("val2")],
          ),
        ]),
      ~fragment=Fragment.make("frag"),
      (),
    ),
  ),
  (
    "http://user:pass@example.com:99/my/dir/?key1=val1&key2=val2&key1=val3#frag",
    "http://user:pass@example.com:99/my/dir?key1=val1&key1=val3&key2=val2#frag",
    URI.makeWithLabels(
      ~scheme=Scheme.make("http"),
      ~authority=
        Authority.makeWithLabels(
          ~userInfo=
            UserInfo.makeWithLabels(
              ~username=Username.make("user"),
              ~password=Password.make("pass"),
              (),
            ),
          ~hostname=Hostname.make("example.com"),
          ~port=Port.make(99),
          (),
        ),
      ~path=Path.make([PathSegment.make("my"), PathSegment.make("dir")]),
      ~query=
        Query.fromList([
          QueryParam.make(
            QueryKey.make("key1"),
            [QueryValue.make("val1"), QueryValue.make("val3")],
          ),
          QueryParam.make(
            QueryKey.make("key2"),
            [QueryValue.make("val2")],
          ),
        ]),
      ~fragment=Fragment.make("frag"),
      (),
    ),
  ),
  (
    "http://user:pass@example.com:99/my/dir?key1=this%20is%20a%20test%21#frag",
    "http://user:pass@example.com:99/my/dir?key1=this%20is%20a%20test%21#frag",
    URI.makeWithLabels(
      ~scheme=Scheme.make("http"),
      ~authority=
        Authority.makeWithLabels(
          ~userInfo=
            UserInfo.makeWithLabels(
              ~username=Username.make("user"),
              ~password=Password.make("pass"),
              (),
            ),
          ~hostname=Hostname.make("example.com"),
          ~port=Port.make(99),
          (),
        ),
      ~path=Path.make([PathSegment.make("my"), PathSegment.make("dir")]),
      ~query=
        Query.fromList([
          QueryParam.make1(
            QueryKey.make("key1"),
            QueryValue.make("this%20is%20a%20test%21"),
          ),
        ]),
      ~fragment=Fragment.make("frag"),
      (),
    ),
  ),
];

describe("URI", () =>
  testAll(
    "URL parsing",
    tests,
    ((uriString, expectedURIString, expectedURI)) => {
      let actualURIResult =
        URI.parser |> ReludeParse.Parser.runParser(uriString);
      let actualURIStringResult =
        actualURIResult |> Relude.Result.map(uri => uri |> URI.show);
      expect((actualURIResult, actualURIStringResult))
      |> toEqual((
           Belt.Result.Ok(expectedURI),
           Belt.Result.Ok(expectedURIString),
         ));
    },
  )
);