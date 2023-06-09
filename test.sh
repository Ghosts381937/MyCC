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
try 30 "2*3+4*6"
try 1 "3 - 2/1 "
try 1 "1*3-4/2"
try 4 "2+(3-1 )"
try 5 "+10/(5+-3)"
try 1 "1 == 1"
try 1 "1 + 2 == 3 + 0"
try 1 "1 < 2 == 3 > 0"
try 0 "1 < 2 != 3 > 0"
try 0 "2 <= 2 != 3 > 0"
try 1 "2 <= 2 == 3 >= 0"
try 1 "(1 + 2) * 6 != 4 / 2 - 1 < 5"
echo OK
