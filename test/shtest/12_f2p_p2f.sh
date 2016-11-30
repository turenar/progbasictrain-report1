_pass _call -i "${pbmdir}/04_lf.pbm" -t pbm >"${outdir}/12_1.pbm"
_pass _call -o "${outdir}/12_2.pbm" -f pbm <"${pbmdir}/04_lf.pbm"
_pass diff -q "${outdir}/12_1.pbm" "${outdir}/12_2.pbm"
