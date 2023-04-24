#!/bin/sh
try() {
    expected="$1"
    input="$2"

    ./MyCC "$input" > tmp.s
    gcc -o tmp tmp.s
    ./tmp
    actual="$?"
    if [ "$actual" = "$expected" ]; then
        echo "$input => $actual"
    else
        echo "$input => $expected expected, but got $actual"
        exit 1
    fi
}

try 0 0
try 34 34
try 101 "50+110-59"
try 10 " 3 + 10 - 3 "

echo OK
