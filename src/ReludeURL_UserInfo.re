module P = ReludeParse.Parser;
open P.Infix;

module Username = ReludeURL_Username;
module Password = ReludeURL_Password;

type t = {
  username: Username.t,
  password: option(Password.t),
};

let make = (username: Username.t, password: option(Password.t)) => {
  username,
  password,
};

let makeNamed = (~username: Username.t, ~password: option(Password.t)=?, ()) =>
  make(username, password);

let fromUsername = (username: Username.t): t => {username, password: None};

let fromCredentials = (username, password) => {
  username,
  password: Some(password),
};

let username: t => Username.t = ({username}) => username;

let password: t => option(Password.t) = ({password}) => password;

let withUsername: (Username.t, t) => t =
  (username, userInfo) => {...userInfo, username};

let withPassword: (Password.t, t) => t =
  (password, userInfo) => {...userInfo, password: Some(password)};

let withPasswordOpt: (option(Password.t), t) => t =
  (password, userInfo) => {...userInfo, password};

let parser: P.t(t) =
  make <$> Username.parser <*> P.opt(P.str(":") *> Password.parser);

let show = ({username, password}) =>
  Username.show(username)
  ++ (password |> Relude.Option.fold("", pw => ":" ++ Password.show(pw)));

module Show: BsAbstract.Interface.SHOW with type t = t = {
  type nonrec t = t;
  let show = show;
};