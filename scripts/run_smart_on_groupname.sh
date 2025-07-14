#!/bin/bash

ulimit -s unlimited
ulimit -v $((100 * 1024 * 1024))

GROUP=$1

#> ../tex/report/"${GROUP}".tex

#echo "\\\\[-10pt]" >> ../tex/report/"${GROUP}".tex

for graphfile in ../data/complete_set/"${GROUP}"*.graph
do
        filename=$(basename "$graphfile")
        echo "${filename}"

        filename=$(basename "$graphfile" .graph)
        safe_filename=$(echo "$filename" | sed 's/_/\\_/g')
        #reduced_graphfile=../data/complete_set_reduced/"${filename}.graph"
        NUMB_OF_NODES="-"
        NUMB_OF_EDGES="-"
        WD="-"
        NUMB_OF_BAGS="-"
        MWIS="-"
        TIME="-"
        
        PLINE=$(head -n 1 "${graphfile}")
        NUMB_OF_NODES=$(echo "$PLINE" | awk '{print $1}')
        NUMB_OF_EDGES=$(echo "$PLINE" | awk '{print $2}')

        #Look at the .gr file
        #Look at the corresponding .td files
        #If s line of td exists and corresponding width value is smaller than 30
        #Create .ind file an store weight, time and set
        tdfile="${graphfile%graph}td"
        if [[ -s "${tdfile}" ]]
        then
                SLINE=$(grep -m 1 '^s td' "${tdfile}")
                #echo "${SLINE}"
                if [[ -n "$SLINE" ]]
                then

                        WD=$(echo "$SLINE" | awk '{print $4}')
                        NUMB_OF_BAGS=$(echo "$SLINE" | awk '{print $3}')
                        if [ "${WD}" -lt 128 ]
                        then
                                if [ "${WD}" -lt 127 ]
                                then
                                        ../../mwis_tree_decomposition/build/mwis_smart "${tdfile}" "${graphfile}" > "${tdfile%td}ind"
                                        MWIS=$(sed -n '1p' "${tdfile%td}ind" | tr -d '\n')
                                        TIME=$(sed -n '2p' "${tdfile%td}ind")
                                fi
                                #echo "${safe_filename} & ${NUMB_OF_NODES} & ${NUMB_OF_EDGES} & ${WD} & ${NUMB_OF_BAGS} & ${MWIS} & ${TIME} \\\\" >> ../tex/report/data_rows.tex
                        fi
                else
                        echo "${safe_filename} contains no sline"
                fi
        else
                echo "${safe_filename} is an empty file or doesn't exist"
        fi
        cat "${tdfile%td}ind"
        #if [[ "${MWIS}" == "-" ]]
        #then
        #        MWIS_LATEX="-"
        #else
        #        MWIS_LATEX="\\numprint{${MWIS}}"
        #fi
        #if [[ "${TIME}" == "-" ]]
        #then
        #        TIME_LATEX="-"
        #else
        #        TIME_LATEX=$(awk -v t="${TIME}" 'BEGIN {
        #        if (t < 0.0001) print "\\textless{}0.0001";
        #        else printf("%.4f", t);
        #        }')
        #fi
        #echo "${filename} & ${NUMB_OF_NODES} & ${NUMB_OF_EDGES} & ${WD} & ${NUMB_OF_BAGS} & ${MWIS} & ${TIME}"
        #echo "${safe_filename} & ${NUMB_OF_NODES} & ${NUMB_OF_EDGES} & ${WD} & ${NUMB_OF_BAGS} & ${MWIS_LATEX} & ${TIME_LATEX} \\\\" >> ../tex/report/osm.tex
        #if [[ -f "${reduced_graphfile}" ]]; then
        #        echo "${safe_filename} & \\numprint{${NUMB_OF_NODES}} & \\numprint{${NUMB_OF_EDGES}} & \\numprint{${WD}} & \\numprint{${NUMB_OF_BAGS}} & ${MWIS_LATEX} & ${TIME_LATEX} \\\\" >> ../tex/report/"${GROUP}".tex
        #else
        #
        #        echo "\\rowcolor{gray!20} ${safe_filename} & \\numprint{${NUMB_OF_NODES}} & \\numprint{${NUMB_OF_EDGES}} & \\numprint{${WD}} & \\numprint{${NUMB_OF_BAGS}} & ${MWIS_LATEX} & ${TIME_LATEX} \\\\" >> ../tex/report/"${GROUP}".tex
        #fi
done