open Jest;
open Expect;

open ReludeURL;

module Scheme = ReludeURL.Scheme;
module Authority = ReludeURL.Authority;
module UserInfo = ReludeURL.UserInfo;
module Username = ReludeURL.Username;
module Password = ReludeURL.Password;
module Hostname = ReludeURL.Hostname;
module Port = ReludeURL.Port;
module Path = ReludeURL.Path;
module QueryKey = ReludeURL.QueryKey;
module QueryValue = ReludeURL.QueryValue;
module QueryParam = ReludeURL.QueryParam;
module Query = ReludeURL.Query;
module Fragment = ReludeURL.Fragment;
module URI = ReludeURL.URI;

let testFull1 = (
  "http://user:pass@example.com:99/my/dir?key1=val1&key2=val2&key1=val3#frag",
  ReludeURL.URI.makeNamed(
    ~scheme=Scheme.make("http"),
    ~authority=
      Authority.makeNamed(
        ~userInfo=
          UserInfo.makeNamed(
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
        QueryParam.make(QueryKey.make("key2"), [QueryValue.make("val2")]),
      ]),
    ~fragment=Fragment.make("frag"),
    (),
  ),
);

describe("URI", () =>
  testAll("URL parsing", [testFull1], ((url, expected)) =>
    expect(URI.parser |> ReludeParse.Parser.runParser(url))
    |> toEqual(Belt.Result.Ok(expected))
  )
);