let
  pkgs = import (builtins.fetchGit {
    name = "nixpkgs";
    url = "https://github.com/nixos/nixpkgs/";
    ref = "refs/heads/master";
    # pinned on 01.04.2024
    rev = "8a22284f51fcd7771ee65ba124175bf9b90505ad";
  }) {};
in pkgs.mkShellNoCC {
  buildInputs = with pkgs; [
    # build
    pkg-config
    ninja
    meson
    clang

    # deps
    raylib

    # dev
    lldb

    # misc
    git
    less
    vim
  ];
}
