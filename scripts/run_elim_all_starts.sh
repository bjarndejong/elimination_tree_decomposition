#!/bin/bash

# Start time
start_time_script=$(date +"%Y-%m-%d %H:%M:%S")
echo "Start time script: $start_time_script"

set -e

total_limit_sec=$((30 * 60))

graph_basename=$1
graph_file="../data/complete_set/${graph_basename}.graph"
num_vertices=$(head -n 1 "$graph_file" | awk '{print $1}')
num_edges=$(head -n 1 "$graph_file" | awk '{print $2}')

echo "${graph_basename}:"
head -n 1 "${graph_file}"

random_vertex_order=$(shuf -i 1-"${num_vertices}")
count_done=0

temp_file=$(mktemp)                      #Special file
cleanup()
{
    rm -f "$temp_file"
}
trap cleanup EXIT

SECONDS=0                               #Special paramater/shell variable
remaining_time=$total_limit_sec
for i in $random_vertex_order
do
    remaining_time=$((total_limit_sec - SECONDS))
    if ((remaining_time <= 0))
    then
        break
    fi
    if output=$(timeout "$remaining_time" ../build/elimination "$graph_file" "$i" 2>/dev/null)
    then
        if [[ -n "$output" ]]
        then
            echo "$output $i"
            count_done=$((count_done+1))
        fi
    fi
done > "${temp_file}"

echo "Completed $count_done out of $num_vertices"

# Sort and print after loop completes successfully


if [[ $count_done -eq 0 ]]
then
    echo "s td ${num_vertices} ${num_vertices} ${num_vertices}" > "${graph_file%.graph}.txt"
else
    sort -k4,4n "${temp_file}" > "${graph_file%.graph}.txt"
fi

#rm "${temp_file}"

head -n 1 "${graph_file%.graph}.txt"

# Print end time
end_time_script=$(date +"%Y-%m-%d %H:%M:%S")
echo "End time script: $end_time_script"
echo ""