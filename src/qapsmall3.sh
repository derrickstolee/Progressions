#!/bin/bash
./progressions.exe run -m 1000 -k 600 --maxsols 10000 --mode quasi -n 66 -N 70 -K 10 -I 4 < in.0 > out-k10i4.txt
./progressions.exe run -m 1000 -k 600 --maxsols 10000 --mode quasi -n 74 -N 80 -K 11 -I 4 < in.0 > out-k11i4.txt
./progressions.exe run -m 1000 -k 600 --maxsols 10000 --mode quasi -n 82 -N 90 -K 12 -I 4 < in.0 > out-k12i4.txt
./progressions.exe run -m 1000 -k 900 --maxsols 10000 --mode quasi -n 96 -N 100 -K 13 -I 4 < in.0 > out-k13i4.txt
./progressions.exe run -m 1000 -k 1200 --maxsols 10000 --mode quasi -n 98 -N 100 -K 14 -I 4 < in.0 > out-k14i4.txt
