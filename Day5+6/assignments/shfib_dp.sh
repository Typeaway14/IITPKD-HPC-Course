#!/bin/bash
VALUE=$1
gcc fib_dp.c && time -p ./a.out $VALUE && rm a.out
