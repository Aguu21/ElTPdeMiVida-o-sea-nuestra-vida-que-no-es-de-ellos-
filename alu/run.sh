#!/bin/bash

for i in $(seq $1); do
    PORT=$(expr 4000 + $RANDOM % 1024)
    ./client $PORT &
    pids[${i}]=$!
done

for pid in ${pids[*]}; do
    wait $pid
done