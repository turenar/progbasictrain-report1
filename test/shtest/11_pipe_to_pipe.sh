_pass _call --from pbm --to pbm <"${pbmdir}/04_lf.pbm" >"${outdir}/11_1.pbm"
_pass _call -f pbm -t pbm -i- -o- <"${pbmdir}/04_lf.pbm" >"${outdir}/11_2.pbm"
_pass diff -q \
	<(tr -d '\r' <"${outdir}/11_1.pbm") \
	<(tr -d '\r' <"${outdir}/11_2.pbm")
