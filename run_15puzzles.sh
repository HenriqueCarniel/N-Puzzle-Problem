#!/bin/bash

PROGRAM="./main"
INPUT_FILE="instances/15puzzle_instances.txt"
ALGORITHM="astar"
TIME_LIMIT=30

total_time=0
max_memory=0
executed_lines=0
mapfile -t lines < "$INPUT_FILE"

echo "============================ $ALGORITHM ============================"
for line in "${lines[@]}"
do
    output=$(timeout $TIME_LIMIT /usr/bin/time -v $PROGRAM -$ALGORITHM "$line" 2>&1 >/dev/tty)

    if [ $? -eq 124 ]; then
        total_time=$(echo "$total_time + $TIME_LIMIT" | bc)
        continue
    fi

    executed_lines=$((executed_lines + 1))

    time=$(echo "$output" | grep 'Elapsed (wall clock) time' | awk '{print $8}')
    memory=$(echo "$output" | grep 'Maximum resident set size' | awk '{print $6}')

    IFS=: read min sec <<< "$time"
    time_seconds=$(echo "$min*60 + $sec" | bc)

    total_time=$(echo "$total_time + $time_seconds" | bc)
    if (( memory > max_memory )); then
        max_memory=$memory
    fi
done

echo "----------------------------------------"
echo "Successfully executed lines: $executed_lines"
echo "Total execution time for $ALGORITHM: $total_time seconds"
echo "Total memory usage for $ALGORITHM: $max_memory kbytes"