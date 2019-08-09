module P = ReludeParse.Parser;
open P.Infix;

module Username = ReludeURL_Username;
module Password = ReludeURL_Password;

type t = {
  username: Username.t,
  password: option(Password.t),
};

let make = (username, password) => {username, password};

let parser: P.t(t) =
  make <$> Username.parser <*> P.opt(P.str(":") *> Password.parser);