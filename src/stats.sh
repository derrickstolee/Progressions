#!/bin/bash
tail -n 9999999 ../data/out*.txt | grep -f stataccept.txt 

