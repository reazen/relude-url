module P = ReludeParse.Parser;
open P.Infix;

module UserInfo = ReludeURL_UserInfo;
module Hostname = ReludeURL_Hostname;
module Port = ReludeURL_Port;

type t = {
  userInfo: option(UserInfo.t),
  hostname: Hostname.t,
  port: option(Port.t),
};

let make = (userInfo, hostname, port) => {userInfo, hostname, port};

let parser: P.t(t) =
  make
  <$> P.opt(UserInfo.parser <* P.str("@"))
  <*> Hostname.parser
  <*> P.opt(P.str(":") *> Port.parser);