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
	../src/denoiser${EXEEXT} "$@"
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
outdir=denoiser.test.tmp
test -d "${outdir}" && rm -rf "${outdir}"
mkdir "${outdir}"

_pass _calln "${pbmdir}/04_lf.pbm" "${outdir}/1.pbm"
_pass _callg -i "${pbmdir}/04_lf.pbm" -o "${outdir}/2.pbm" icm
_pass diff -q "${outdir}/1.pbm" "${outdir}/2.pbm"

# denoiser doesn't support - -
xfail _calln - - <"${pbmdir}/04_lf.pbm" >"${outdir}/11.pbm"
# but /dev/stdin or /dev/stdout passes
# mingw on linux fails...
#_pass _calln /dev/stdin /dev/stdout <"${pbmdir}/04_lf.pbm" >"${outdir}/12.pbm"

xfail _calln "${outdir}/notfound.pbm" "${outdir}/21.pbm"
xfail _calln "${pbmdir}/04_lf.pbm" "${outdir}/notfounddir/22.pbm"

exit 0
