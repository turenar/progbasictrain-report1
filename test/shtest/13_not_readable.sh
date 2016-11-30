_pass echo >"${outdir}/13.pbm"
_pass chmod u-r "${outdir}/13.pbm"
xfail _call -tpbm -i "${outdir}/13.pbm"
