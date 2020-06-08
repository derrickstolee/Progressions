#!/bin/bash
./progressions.exe run -m 1000 -k 6 --maxsols 10 --mode quasi -n 66 -N 70 -K 10 -I 4 < in.0 > ../data/out-k10i4.txt
./progressions.exe run -m 1000 -k 6 --maxsols 10 --mode quasi -n 74 -N 80 -K 11 -I 4 < in.0 > ../data/out-k11i4.txt
./progressions.exe run -m 1000 -k 6 --maxsols 10 --mode quasi -n 82 -N 90 -K 12 -I 4 < in.0 > ../data/out-k12i4.txt
./progressions.exe run -m 1000 -k 9 --maxsols 10 --mode quasi -n 96 -N 100 -K 13 -I 4 < in.0 > ../data/out-k13i4.txt
./progressions.exe run -m 1000 -k 12 --maxsols 10 --mode quasi -n 98 -N 100 -K 14 -I 4 < in.0 > ../data/out-k14i4.txt
