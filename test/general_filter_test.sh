#!/bin/bash

function _die() {
	local mess max i line src

	mess=${1:-assertion failed}
	let max=${#BASH_LINENO[@]}-1

	line=${BASH_LINENO[0]}
	src=${BASH_SOURCE[1]}
	echo "$mess at $src line $line" >&2

	i=1
	while [ $i -lt ${max} ]; do
		line=${BASH_LINENO[$i]}
		src=${BASH_SOURCE[`expr $i + 1`]}
		echo "	${FUNCNAME[$i]}() called at ${src} line ${line}" >&2
		let i=$i+1
	done
	exit 1
}

function _call() {
	../src/general_filter${EXEEXT} "$@"
}

function xfail() {
	printf "\e[35m# xfail: %s\e[0m\n" "$*">&2
	"$@" && _die
}

function _pass() {
	printf "\e[36m# _pass:  %s\e[0m\n" "$*" >&2
	"$@" || _die
}


pbmdir="${srcdir:-.}/data/pbm"
outdir=general_filter.test.tmp
test -d "${outdir}" && rm -rf "${outdir}"
mkdir "${outdir}"

# 00: no argument
xfail _call
# 01: show help
_pass _call --help
# 02: invalid short opt
xfail _call -?
xfail _call -Q
# 03: invalid long opt
xfail _call --invalid-long-opt

# 10: pbm->pbm (file to file)
_pass _call --in-file "${pbmdir}/04_lf.pbm" --out-file="${outdir}/10_1_pbm_to.pbm"
_pass _call -i "${pbmdir}/04_lf.pbm" -o "${outdir}/10_2_pbm_to.pbm"
_pass diff -q "${outdir}/10_1_pbm_to.pbm" "${outdir}/10_2_pbm_to.pbm"
# 11: pbm->pbm (pipe to pipe)
_pass _call --from pbm --to pbm <"${pbmdir}/04_lf.pbm" >"${outdir}/11_1.pbm"
_pass _call -f pbm -t pbm <"${pbmdir}/04_lf.pbm" >"${outdir}/11_2.pbm"
_pass diff -q "${outdir}/11_1.pbm" "${outdir}/11_2.pbm"
# 12: pbm ->pbm (f2p/p2f)
_pass _call -i "${pbmdir}/04_lf.pbm" -t pbm >"${outdir}/12_1.pbm"
_pass _call -o "${outdir}/12_2.pbm" -f pbm <"${pbmdir}/04_lf.pbm"
_pass diff -q "${outdir}/12_1.pbm" "${outdir}/12_2.pbm"
# 13: invalid source
xfail _call -fpbm -i/dev/null -tpbm
echo | xfail _call -fpbm -tpbm
# 14: not readable
_pass echo >"${outdir}/14.pbm"
_pass chmod u-r "${outdir}/14.pbm"
xfail _call -tpbm -i "${outdir}/14.pbm"
# 15: not writable
_pass echo >"${outdir}/15.pbm"
_pass chmod u-w "${outdir}/15.pbm"
xfail _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/15.pbm"
# 16: invalid codec type
xfail _call -finvalid -tpbm -i"${pbmdir}/04_lf.pbm" -o"${outdir}/16_1.pbm"
xfail _call -tinvalid -fpbm -i"${pbmdir}/04_lf.pbm" -o"${outdir}/16_1.pbm"

exit 0
