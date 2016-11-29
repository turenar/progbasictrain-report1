#!/bin/sh

function _die() {
	local exitcode msg
	exitcode=$1
	msg=$2
	echo ${msg} >&1
	exit ${exitcode}
}

function assert_count() {
	local expected exitcode
	expected=$1
	shift
	c=$(../src/general_filter${EXEEXT:-} "$@")
	exitcode=$?
	test ${exitcode} -eq 0 || _die ${exitcode} "command failed; cmd='$*'"
	test ${c} -eq ${expected} || _die 1 "assertion failed; expected=${expected}, actual=${c}; cmd='$*'"
}

assert_count 4 -i ${srcdir:-.}/data/pbm/04_lf.pbm -t count

cat <<EOD | assert_count 2 -f pbm -t count
P1
2 2
1 0
0 1
EOD
