module P = ReludeParse.Parser;
open P.Infix;

module UserInfo = ReludeURL_UserInfo;
module Username = ReludeURL_Username;
module Password = ReludeURL_Password;
module Hostname = ReludeURL_Hostname;
module Port = ReludeURL_Port;

type t = {
  userInfo: option(UserInfo.t),
  hostname: Hostname.t,
  port: option(Port.t),
};

let make =
    (
      userInfo: option(UserInfo.t),
      hostname: Hostname.t,
      port: option(Port.t),
    ) => {
  userInfo,
  hostname,
  port,
};

let makeWithLabels =
    (
      ~userInfo: option(UserInfo.t)=?,
      ~hostname: Hostname.t,
      ~port: option(Port.t)=?,
      (),
    ) => {
  userInfo,
  hostname,
  port,
};

let fromHostname: Hostname.t => t =
  hostname => {userInfo: None, hostname, port: None};

let fromHostnameAndPort: (Hostname.t, Port.t) => t =
  (hostname, port) => {userInfo: None, hostname, port: Some(port)};

let fromUsernameAndHostname: (Username.t, Hostname.t) => t =
  (username, hostname) => {
    userInfo: Some(UserInfo.fromUsername(username)),
    hostname,
    port: None,
  };

let fromCredentialsAndHostname: (Username.t, Password.t, Hostname.t) => t =
  (username, password, hostname) => {
    userInfo: Some(UserInfo.fromCredentials(username, password)),
    hostname,
    port: None,
  };

let fromUsernameHostnameAndPort: (Username.t, Hostname.t, Port.t) => t =
  (username, hostname, port) => {
    userInfo: Some(UserInfo.fromUsername(username)),
    hostname,
    port: Some(port),
  };

let fromCredentialsHostnameAndPort:
  (Username.t, Password.t, Hostname.t, Port.t) => t =
  (username, password, hostname, port) => {
    userInfo: Some(UserInfo.fromCredentials(username, password)),
    hostname,
    port: Some(port),
  };

let userInfo: t => option(UserInfo.t) = ({userInfo}) => userInfo;

let username: t => option(Username.t) =
  ({userInfo}) =>
    userInfo |> Relude.Option.map((userInfo: UserInfo.t) => userInfo.username);

let password: t => option(Password.t) =
  ({userInfo}) =>
    userInfo
    |> Relude.Option.flatMap((userInfo: UserInfo.t) => userInfo.password);

let hostname: t => Hostname.t = ({hostname}) => hostname;

let port: t => option(Port.t) = ({port}) => port;

let withUserInfo: (UserInfo.t, t) => t =
  (userInfo, authority) => {...authority, userInfo: Some(userInfo)};

let withUserInfoOpt: (option(UserInfo.t), t) => t =
  (userInfo, authority) => {...authority, userInfo};

let withHostname: (Hostname.t, t) => t =
  (hostname, authority) => {...authority, hostname};

let withPort: (Port.t, t) => t =
  (port, authority) => {...authority, port: Some(port)};

let withPortOpt: (option(Port.t), t) => t =
  (port, authority) => {...authority, port};

let parser: P.t(t) =
  make
  <$> P.opt(P.tries(UserInfo.parser <* P.str("@")))
  <*> Hostname.parser
  <*> P.opt(P.tries(P.str(":") *> Port.parser));

let show: t => string =
  ({userInfo, hostname, port}) =>
    (
      userInfo
      |> Relude.Option.fold("", userInfo => UserInfo.show(userInfo) ++ "@")
    )
    ++ Hostname.show(hostname)
    ++ (port |> Relude.Option.fold("", port => ":" ++ Port.show(port)));

module Show: BsBastet.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};
