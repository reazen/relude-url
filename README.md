# relude-url

URL utilities with Relude

# Documentation

`ReludeURL` contains data structures and functions for dealing with URLs.

## Accessors

TODO

## Parsing

TODO

## Rendering

TODO

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