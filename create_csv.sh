#!/bin/bash

PROGRAM="./main"
TIME_LIMIT=30
timestamp=$(date +"%Y%m%d_%H%M%S")

if [[ $1 == "-8puzzle" ]]; then
    INPUT_FILE="instances/8puzzle_instances.txt"
    ALGORITHMS=("bfs" "idfs" "astar" "idastar" "gbfs")
    output_dir="results/8puzzle_$timestamp"
elif [[ $1 == "-15puzzle" ]]; then
    INPUT_FILE="instances/15puzzle_instances.txt"
    ALGORITHMS=("astar")
    output_dir="results/15puzzle_$timestamp"
else
    echo "Invalid parameter. Use '-8puzzle' or '-15puzzle'."
    exit 1
fi

mkdir -p "$output_dir"

log_file="$output_dir/summary.log"
exec &> >(tee -a "$log_file")

mapfile -t lines < "$INPUT_FILE"

overall_total_time=0
overall_max_memory=0

for ALGORITHM_TYPE in "${ALGORITHMS[@]}"
do
    output_file="${output_dir}/${ALGORITHM_TYPE}.csv"

    total_time=0
    max_memory=0
    executed_lines=0

    for line in "${lines[@]}"
    do
        output=$( ( /usr/bin/time -v timeout $TIME_LIMIT $PROGRAM -$ALGORITHM_TYPE "$line" ) 2>&1 )

        if [ $? -eq 124 ]; then
            total_time=$(echo "$total_time + $TIME_LIMIT" | bc)
            echo "$output" | head -n 1 >> "$output_file"
            continue
        fi

        executed_lines=$((executed_lines + 1))

        echo "$output" | head -n 1 >> "$output_file"

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
    echo "Algoritmo $ALGORITHM_TYPE"
    echo "Linhas executadas: $executed_lines"
    echo "Tempo total de execução: $total_time segundos"
    echo "Uso máximo de memória: $max_memory kbytes"

    overall_total_time=$(echo "$overall_total_time + $total_time" | bc)
    if (( max_memory > overall_max_memory )); then
        overall_max_memory=$max_memory
    fi
done

echo "========================================"
echo "Tempo total de execução para todos os algoritmos: $overall_total_time segundos"
echo "Uso máximo de memória para todos os algoritmos: $overall_max_memory kbytes"