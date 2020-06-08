#!/bin/bash
./progressions.exe run -m 1000 -k 6 --maxsols 10 --mode quasi -n 38 -N 50 -K 8 -I 3 < in.0 > ../data/out-k08i3.txt
./progressions.exe run -m 1000 -k 6 --maxsols 10 --mode quasi -n 44 -N 50 -K 9 -I 3 < in.0 > ../data/out-k09i3.txt
./progressions.exe run -m 1000 -k 6 --maxsols 10 --mode quasi -n 50 -N 60 -K 8 -I 4 < in.0 > ../data/out-k08i4.txt
./progressions.exe run -m 1000 -k 6 --maxsols 10 --mode quasi -n 64 -N 70 -K 9 -I 4 < in.0 > ../data/out-k09i4.txt
./progressions.exe run -m 1000 -k 6 --maxsols 10 --mode quasi -n 72 -N 100 -K 7 -I 5 < in.0 > ../data/out-k07i5.txt
