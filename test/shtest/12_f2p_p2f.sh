_pass _call -i "${pbmdir}/04_lf.pbm" -t pbm >"${outdir}/12_1.pbm"
_pass _call -o "${outdir}/12_2.pbm" -f pbm <"${pbmdir}/04_lf.pbm"
_pass diff -q \
	<(tr -d '\r' <"${outdir}/12_1.pbm") \
	<(tr -d '\r' <"${outdir}/12_2.pbm")
