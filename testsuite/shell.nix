{ nixpkgs ? <nixpkgs>, system ? builtins.currentSystem }:

with import nixpkgs { inherit system; };

stdenv.mkDerivation {
  name = "unit_test";
  buildInputs = [ 
    cunit	
  ];
}
