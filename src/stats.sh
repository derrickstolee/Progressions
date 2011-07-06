#!/bin/bash
tail -n 9999999 out*.txt | grep -f stataccept.txt 

