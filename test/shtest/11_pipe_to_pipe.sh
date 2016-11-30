_pass _call --from pbm --to pbm <"${pbmdir}/04_lf.pbm" >"${outdir}/11_1.pbm"
_pass _call -f pbm -t pbm <"${pbmdir}/04_lf.pbm" >"${outdir}/11_2.pbm"
_pass diff -q "${outdir}/11_1.pbm" "${outdir}/11_2.pbm"
