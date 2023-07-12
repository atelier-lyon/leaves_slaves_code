{ nixpkgs ? <nixpkgs>, system ? builtins.currentSystem }:

with import nixpkgs { inherit system; };

stdenv.mkDerivation {
  name = "criterion-test-env";
  buildInputs = [ 
    gnustep.base
    criterion
    pkg-config
    cmake
    meson
    ninja
    libffi
    libgit2
  ];
}
