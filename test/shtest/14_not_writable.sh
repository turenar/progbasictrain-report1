_pass echo >"${outdir}/14.pbm"
_pass chmod u-w "${outdir}/14.pbm"
xfail _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/14.pbm"
