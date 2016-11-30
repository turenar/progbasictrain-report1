_pass _call --in-file "${pbmdir}/04_lf.pbm" --out-file="${outdir}/10_1_pbm_to.pbm"
_pass _call -i "${pbmdir}/04_lf.pbm" -o "${outdir}/10_2_pbm_to.pbm"
_pass diff -q "${outdir}/10_1_pbm_to.pbm" "${outdir}/10_2_pbm_to.pbm"
