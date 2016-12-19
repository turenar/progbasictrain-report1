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

function _calln() {
	../src/noiser${EXEEXT} "$@"
}

function _callg() {
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
outdir=noiser.test.tmp
test -d "${outdir}" && rm -rf "${outdir}"
mkdir "${outdir}"

_pass _calln "${pbmdir}/04_lf.pbm" "${outdir}/1.pbm"
_pass _callg -i "${pbmdir}/04_lf.pbm" -o "${outdir}/2.pbm" noise:0.1
_pass diff -q "${outdir}/1.pbm" "${outdir}/2.pbm"

_pass _calln "${pbmdir}/04_lf.pbm" "${outdir}/3.pbm" 0.1
_pass _callg -i "${pbmdir}/04_lf.pbm" -o "${outdir}/4.pbm" noise
_pass diff -q "${outdir}/3.pbm" "${outdir}/4.pbm"

_pass _calln "${pbmdir}/04_lf.pbm" "${outdir}/5.pbm" 0.2
_pass _callg -i "${pbmdir}/04_lf.pbm" -o "${outdir}/6.pbm" noise:0.2
_pass diff -q "${outdir}/5.pbm" "${outdir}/6.pbm"

# noiser doesn't support - -
xfail _calln - - <"${pbmdir}/04_lf.pbm" >"${outdir}/11.pbm"
# but /dev/stdin or /dev/stdout passes
# mingw on linux fails...
#_pass _calln /dev/stdin /dev/stdout <"${pbmdir}/04_lf.pbm" >"${outdir}/12.pbm"

xfail _calln "${outdir}/notfound.pbm" "${outdir}/21.pbm"
xfail _calln "${pbmdir}/04_lf.pbm" "${outdir}/notfounddir/22.pbm"
xfail _calln "${pbmdir}/01_empty.pbm" "${outdir}/23.pbm"
xfail _calln "${pbmdir}/04_lf.pbm" "${outdir}/24.pbm" invalid_probability

exit 0
