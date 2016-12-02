_pass _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/34_noise1.pbm" noise
_pass _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/34_noise2.pbm" noise:0.1
_pass _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/34_noise3.pbm" noise:1
_pass _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/34_noise4.pbm" noise:0
xfail _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/34_noise5.pbm" noise:invalid
xfail _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/34_noise6.pbm" noise:
xfail _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/34_noise7.pbm" noise:2
xfail _call -i"${pbmdir}/04_lf.pbm" -o"${outdir}/34_noise8.pbm" noise:-3
