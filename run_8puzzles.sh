#!/bin/bash

PROGRAM="./main"
INPUT_FILE="instances/8puzzle_instances.txt"
ALGORITHMS=("bfs" "idfs" "astar" "idastar" "gbfs")
overall_total_time=0
overall_max_memory=0

# Reads all lines from the input file and stores it in a list
mapfile -t lines < "$INPUT_FILE"

for ALGORITHM_TYPE in "${ALGORITHMS[@]}"
do
    total_time=0
    max_memory=0

    echo "=============================== $ALGORITHM_TYPE ==============================="
    for line in "${lines[@]}"
    do
        # Redirects the program's standard output to the screen (stdout) and captures the time output
        output=$(/usr/bin/time -v $PROGRAM -$ALGORITHM_TYPE "$line" 2>&1 >/dev/tty)

        time=$(echo "$output" | grep 'Elapsed (wall clock) time' | awk '{print $8}')
        memory=$(echo "$output" | grep 'Maximum resident set size' | awk '{print $6}')

        # Converts execution time to seconds (assuming mm:ss.xx format)
        IFS=: read min sec <<< "$time"
        time_seconds=$(echo "$min*60 + $sec" | bc)

        total_time=$(echo "$total_time + $time_seconds" | bc)
        if (( memory > max_memory )); then
            max_memory=$memory
        fi
    done

    echo "----------------------------------------"
    echo "Total running time for $ALGORITHM_TYPE: $total_time seconds"
    echo "Maximum memory usage for $ALGORITHM_TYPE: $max_memory kbytes"
    
    overall_total_time=$(echo "$overall_total_time + $total_time" | bc)
    if (( max_memory > overall_max_memory )); then
        overall_max_memory=$max_memory
    fi
done

echo "=============================================================="
echo "Overall total execution time: $overall_total_time seconds"
echo "Maximum overall memory usage: $overall_max_memory kbytes"
echo "=============================================================="
