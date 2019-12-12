# relude-url

[![CircleCI](https://img.shields.io/circleci/project/github/reazen/relude-url/master.svg)](https://circleci.com/gh/reazen/relude-url)
[![npm](https://img.shields.io/npm/v/relude-url.svg)](https://www.npmjs.com/package/relude-url)

URL utilities with Relude

# Documentation

`ReludeURL` contains data structures and functions for dealing with URIs, URLs, etc.  This library
is not currently fully conformant to the [RFC 3986 URI spec](https://tools.ietf.org/html/rfc3986) - it only
parses a subset of common absolute URI formats, i.e. formats that are most commonly found in the wild.  If you
encounter a `URI` that `ReludeURL` can't parse correctly, please open an issue with the `URI` for consideration.

## Component types

A URI is broken up into several sub-component types, including `Scheme`, `Authority`, `UserInfo`, `Path`, etc.

## Parsing

[![GitHub CI](https://img.shields.io/github/workflow/status/reazen/relude-url/CI/master)](https://github.com/reazen/relude-url/actions)
[![npm](https://img.shields.io/npm/v/relude-url//www.npmjs.com/package/relude-url)

`ReludeURL` uses [ReludeParse](https://github.com/reazen/relude-parse) to parse URIs.  To run a URI parser, you use
the standard convention for running a `ReludeParse` parser, like below:

```reasonml
ReludeURL.URI.parser |> ReludeParse.runParser("https://example.com:80/my/path") // Belt.Result.Ok(URI(...))
```

## Rendering

Once a `URI` is parsed or constructed, it's often useful to write it back out to a string:

```reasonml
ReludeURL.URI.show(myURI) // http
```

## Manipulation

Parts of the `URI` can be manipulated using the functions from the target part of the `URI`.  A common operation is
to manipulate the query params - this can be done using functions in the `URI` module, or in the `Query`/`QueryParam`/etc. modules.

# Developer info

## Scripts

```sh
> npm run clean
> npm run build
> npm run cleanbuild
> npm run test
> npm run cleantest
> npm run coverage
> npm run cleancoverage
> npm run watch
```

## Publish to npm

```sh
> npm version major|minor|patch
> git push origin --follow-tags
> git push upstream --follow-tags
> npm publish
# Create release on GitHub
```

## NixOS

A simple default.nix shell config is provided to avoid issues with OCaml-based binaries
and bs-platform.

```sh
> nix-shell
%nix%> npm install
```