#!/bin/bash

for graph_file in ../data/complete_set/ssmc*.graph
do
    basename_file=$(basename "$graph_file")
    echo "${basename_file}:"
    txt_file="${graph_file%.graph}.txt"
    td_file="${graph_file%.graph}.td"
    if [[ -s "${txt_file}" ]]
    then
        start_vertex=$(awk 'NR == 1 { if (NF == 6) print $6 }' "${txt_file}")
        if [[ -n "${start_vertex}" ]]
        then
            echo "Run with ${start_vertex}"
            ../build/elimination "${graph_file}" "${start_vertex}" "--print_td=true" > "${td_file}" 
        fi
    fi
done