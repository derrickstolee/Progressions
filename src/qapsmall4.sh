#!/bin/bash
./progressions.exe run -m 1000 -k 600 --maxsols 10000 --mode quasi -n 72 -N 80 -K 7 -I 5 < in.0 > out-k07i5.txt
./progressions.exe run -m 1000 -k 900 --maxsols 10000 --mode quasi -n 92 -N 95 -K 8 -I 5 < in.0 > out-k08i5.txt
./progressions.exe run -m 1000 -k 1200 --maxsols 10000 --mode quasi -n 114 -N 118 -K 9 -I 5 < in.0 > out-k09i5.txt
