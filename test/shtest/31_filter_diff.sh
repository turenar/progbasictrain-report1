_pass _call -i"${pbmdir}/04_lf.pbm" -tcount diff1
_pass _call -i"${pbmdir}/04_lf.pbm" -tcount diff1 diff2
xfail _call -i"${pbmdir}/04_lf.pbm" -tcount diff1 diff2 diff2
_pass _call -i"${pbmdir}/04_lf.pbm" -tcount diff1 diff2 diff1 diff2
xfail _call -i"${pbmdir}/04_lf.pbm" -tcount diff1 diff1
xfail _call -i"${pbmdir}/04_lf.pbm" -tcount diff2
