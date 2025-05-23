#!/bin/bash

cat out_misses.txt | tr '\n' ' ' | sed -E 's/us /us\n/g' | sed -E 's/us//g' | awk 'BEGIN{FS=" "; print "\"size(kB)\",\"speed(us)\"" } { print $2 "," $4 }' >file.csv
cat out_cont_mem.txt | tr '\n' ' ' | sed -E 's/us /us\n/g' | sed -E 's/us//g' | awk 'BEGIN{FS=" "; print "\"size(kB)\",\"speed(us)\"" } { print $1 "," $2 }' >file_cont.csv
cat out_non_cont_mem.txt | tr '\n' ' ' | sed -E 's/us /us\n/g' | sed -E 's/us//g' | awk 'BEGIN{FS=" "; print "\"size(kB)\",\"speed(us)\"" } { print $1 "," $2 }' >file_n_cont.csv

