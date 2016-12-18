#!/bin/bash

if [ ! -x "src/general_filter" ]; then
	echo "src/general_filter: No such file or directory" >&2
	exit 1
fi

: ${probability:=0.1}
min_option=none
min_diff=10000000
for t in flat square circle dia; do
	if [ $t = flat ]; then
		b=1
	else
		b=2
	fi
	for radius in {1..3}; do
		prev_diff=10000000
		same_count=0
		for gamma in {1..100}; do
			option=b=${b}:g=${gamma}:r=${radius}:t=${t}
			printf "\r\e[K%s..." ${option} >&2
			diff=$(src/general_filter -i "$1" -t count diff1 noise:${probability} icm2:${option} diff2)
			if [ x$prev_diff = x$diff ]; then
				same_count=$((same_count + 1))
				if [ $same_count -ge 5 ]; then
					break
				fi
			else
				same_count=0
			fi
			if [ $min_diff -gt $diff ]; then
				min_diff=$diff
			fi
			printf "\r\e[K%d %s\n" ${diff} ${option}
			prev_diff=${diff}
		done
	done
done
