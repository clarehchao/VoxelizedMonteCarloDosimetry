#!/bin/bash
# TASK: execute VHDMSDv3 with appropriate input parameters to sumbit single/multiple simulations to a given linux machine
# Input arguments:
# $1: geometry name
# $2: source organ name
# $3: first run #, run1
# $4: last run #, run2

rootdir=/data4/G4data_Clare
myg4dir=$rootdir/G4.9.6.p02work
runname="./VHDMSDv3"
rdirname=VoxelizedHumanDoseMultiSDv3-build
g4inputdir=$rootdir/G4INPUT
GEOdir=$g4inputdir/GeometryIM/G4IM
SRCMPdir=$g4inputdir/SourceMap
RUNdir=$myg4dir/$rdirname
GEOtype=NestedParam
isSRCMPsparse=1

GEOname=$1
ORGANname=$2
SRCMPname=$GEOname/$ORGANname
run1=$3
run2=$4
PARname=I131
macfile=$RUNdir/macro/I131_Emphysics2.mac
isElectron=1
isPhoton=1
RUNACTType=Root  #Root or Reg
ebin=1   #ebin == 0, use 25 energy bins; ebin == 1, use 28 energy bins
OOItype=all # all or skel

#make the appropriate directories for the simulation
dirtag="pCellFlux"

for (( n=$run1; n<=$run2; n++ ))
do
	datadirname=$RUNdir/data/GEO_$GEOname/SRCMP_$SRCMPname/$PARname/Run$n
	if [ "$RUNACTType" == "Root" ]
	then
		echo "Save root files!"
		rootoutdir=$datadirname/BustOutRoot
		rootdatdir=$datadirname/RootData
		#echo $rootoutdir
		#echo $rootdatdir
		mkdir -p $rootoutdir
		mkdir -p $rootdatdir
	else
		for p in Edep 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28
		do
			if [ "$p" == "Edep" ]
			then
				dirname=$datadirname/Edep_MultiSD
			elif [ "$p" -lt "10" ]
			then
				dirname=$datadirname/$dirtag"0"$p
				echo $dirname
			else
				dirname=$datadirname/$dirtag$p
				echo $dirname
			fi
			mkdir -p $dirname
		done
	fi
	$runname $GEOtype $GEOdir $GEOname $SRCMPdir $SRCMPname $isSRCMPsparse $datadirname $isElectron $isPhoton $RUNACTType $ebin $OOItype $macfile > $datadirname/log.txt
 	#echo "$runname $GEOtype $GEOdir $GEOname $SRCMPdir $SRCMPname $isSRCMPsparse $datadirname $isElectron $isPhoton $RUNACTType $ebin $OOItype $macfile > $datadirname/log.txt"
	echo "Finish $runname run #$n [$GEOname][$ORGANname]! good job bucko!"

	if [ "$n" -ne "$run2" ]
	then 
		echo "~~Give me a break, break me a piece of that kit-kat bar~~ 6-mins!"
    	 	sleep 3m  #sleep/delay for 6 minutes
	fi
done
