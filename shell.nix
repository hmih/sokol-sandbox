let
  pkgs = import (builtins.fetchGit {
    name = "nixpkgs";
    url = "https://github.com/nixos/nixpkgs/";
    ref = "refs/heads/master";
    # pinned on 01.04.2024
    rev = "07512215ac3313f69817029a857a036dc0a5e3a3";
  }) {};
in pkgs.mkShellNoCC {
  buildInputs = with pkgs; [
    pkg-config
    ninja
    meson
    raylib
    clang
    lldb

    # misc
    git
    less
    vim
  ];
}
