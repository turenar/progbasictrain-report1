a=$(_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2 diff2)
b=$(_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2:b=1 diff2)
c=$(_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2:g=2 diff2)
d=$(_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2:r=1 diff2)
e=$(_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2:t=flat diff2)
_pass test $a = $b
_pass test $a = $c
_pass test $a = $d
_pass test $a = $e
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2:b=2:g=11:r=2:t=dia diff2
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2:b=2:g=5:r=1:t=circle diff2
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2:b=2:g=16:r=2:t=square diff2
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount diff1 icm2:b=1:g=2:r=1:t=flat diff2
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:b=invalid
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:b=
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:b=-1
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:b=0
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:g=invalid
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:g=
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:g=-1
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:g=0
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:r=invalid
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:r=
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:r=-1
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:r=0
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:t=flat
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:t=square
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:t=dia
_pass _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:t=circle
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:t=invalid
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:t=
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:b
xfail _call -i"${pbmdir}/99_image2.pbm" -tcount icm2:Z=
