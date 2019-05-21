#!/bin/sh
# Script name: build-mingw.sh
# Description: builds examples with mingw32 cross-compiler

FLAGS="-Wall -Wextra -g --static"
CROSSPREFIX="i686-w64-mingw32-"
#CROSSPREFIX="i686-pc-mingw32-"

${CROSSPREFIX}g++ $FLAGS tests/cpp-test.cpp -o cpp-test.exe || exit 1

${CROSSPREFIX}gcc $FLAGS tests/c-test.c -o c-test.exe || exit 1

${CROSSPREFIX}gcc $FLAGS examples/example.c -o c-example.exe || exit 1

