#!/bin/bash

geoname=segCT_MIBGPT3

SO=TotalBody
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 1 2  > joblog1.txt &

#"Adrenal","Lung","Heart","Liver","SalivaryGlands","Spleen","Urinarybladder","Stomach","Thyroid","Kidney","Tumor1","Tumor2","Tumor3","Tumor4","Tumor5","Tumor6"
num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c

. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 3 4  > joblog2.txt &


#SO=Tumor3
num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c

. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 5 6  > joblog3.txt &

num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c

. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 7 8  > joblog4.txt &

#SO=Tumor4
num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c

. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 9 10  > joblog5.txt &

<<'COMMENT'
num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c
. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 6 10  > joblog6.txt &

SO=Tumor5
num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c

. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 1 5  > joblog7.txt &

num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c
. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 6 10  > joblog8.txt &


SO=Tumor6

num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c

. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 1 5  > joblog9.txt &

num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c

. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun3.sh $geoname $SO 6 10  > joblog10.txt &
COMMENT
