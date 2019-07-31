#!/usr/bin/env bash
set -e

__USAGE="Usage: $(basename $0) [OPTIONS]

Options:
   -h, --help        Shows this text.
   -t, --test        Run the tests.
   -c, --compiler    Specify the compiler to use [clang, gcc/g++] DEFAULT=clang.
"

RUN_TESTS=false
C_COMPILER=clang
CXX_COMPILER=clang

function set-compiler {
   case "${1,,}" in
      gcc|g++)
         C_COMPILER=gcc
         CXX_COMPILER=g++
         ;;
      clang)
         C_COMPILER=clang
         CXX_COMPILER=clang
         ;;
      *)
         echo -e "Unknown compiler $1"
         exit 1
         ;;
   esac
}

while [[ $# -gt 0 ]]; do
   case "$1" in
      -h|--help)
         echo "$__USAGE"
         exit 0
         ;;
      -t|--test)
         shift
         RUN_TESTS=true
         ;;
      -c|--compiler)
         shift
         set-compiler $1
         shift
         ;;
      *)
         echo -e "Unknown argument $1\n"
         echo "$__USAGE"
         exit 1
         ;;
   esac
done

cd /opt/src
rm -rf ./build
mkdir -p build
pushd build
cmake .. \
   -D CMAKE_C_COMPILER="$C_COMPILER" \
   -D CMAKE_CXX_COMPILER="$CXX_COMPILER"
make -j 6
if [ "$RUN_TESTS" = true ]; then
   pushd tests
   ./test --success
   popd
fi
popd
