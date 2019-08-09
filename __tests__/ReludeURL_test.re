open Jest;
open Expect;

open ReludeURL;

/*
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
 module URL = ReludeURL.URL;
 */

describe("ReludeURL", () => {
  test("hi", () =>
    expect(true) |> toEqual(true)
  );

  testAll(
    "URL parsing",
    [
      (
        "http://user:pass@example.com:99/my/dir?key1=val1&key2=val2&key1=val3#frag",
        ReludeURL.URL.make(
          Scheme.make("http"),
          Some(
            Authority.make(
              Some(
                UserInfo.make(
                  Username.make("user"),
                  Some(Password.make("pass")),
                ),
              ),
              Hostname.make("example.com"),
              Some(Port.make(99)),
            ),
          ),
          Path.make([PathSegment.make("my"), PathSegment.make("dir")]),
          Some(
            Query.make([
              QueryParam.make(
                QueryKey.make("key1"),
                [QueryValue.make("val1"), QueryValue.make("val3")],
              ),
              QueryParam.make(
                QueryKey.make("key2"),
                [QueryValue.make("val2")],
              ),
            ]),
          ),
          Some(Fragment.make("frag")),
        ),
      ),
    ],
    ((url, expected)) =>
    expect(URL.parser |> ReludeParse.Parser.runParser(url))
    |> toEqual(Belt.Result.Ok(expected))
  );
});