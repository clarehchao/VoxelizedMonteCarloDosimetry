#!/bin/bash
# TASK: submit multiple jobs at higgs for simulation

geoname=MIBGPT1_segCT
SO=Brain
. runscript/VHDMSDv3MultiRun.sh $geoname $SO 1 10  > joblog4.txt &

:<<'COMMENT'
num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c


SO=Lungs
. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun.sh $geoname $SO 1 10  > joblog2.txt &


num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c


SO=SalivaryGlands
. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun.sh $geoname $SO 1 10 > joblog3.txt &


num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c


SO=Bladder
. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun.sh $geoname $SO 1 10 > joblog4.txt &

num=$RANDOM
#echo $num
a=$(echo "scale=5; $num / 32767" | bc)
#echo $a
b=$(echo "scale=5; $a * 75" | bc)
#echo $b
c=$(echo "scale=5; $b + 280" | bc)
#echo $c


SO=Tumor2
. runscript/sleep.sh $c
. runscript/VHDMSDv3MultiRun.sh $geoname $SO 1 10 > joblog5.txt &
COMMENT


