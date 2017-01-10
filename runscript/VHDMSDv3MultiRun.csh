#!/bin/csh
# Input arguments:
# $1: source organ name
# $2: first run #, run1
# $3: last run #, run2

set nwdir=/data/petct1
set myg4dir=$nwdir/G4.10.0.p02work
set runname=./VHDMSDv3
set rdirname=VoxelizedHumanDoseMultiSDv3-build
set GEOtype=NestedParam
set GEOdir=$nwdir/GeometryIM/G4IM
set SRCMPdir=$nwdir/SourceMap
set isSRCMPsparse=1
set RUNdir=$myg4dir/$rdirname
#echo $RUNdir

set GEOname=ufh00f_1
set ORGANname=$1
set SRCMPname=$GEOname/$ORGANname
set run1=$2
set run2=$3
set PARname=I131
set macfile=$RUNdir/macro/I131_Emphysics2.mac
set isElectron=1
set isPhoton=1
set RUNACTType=Root  #Root or Reg
set ebin=1   #ebin == 0, use 25 energy bins; ebin == 1, use 28 energy bins
set OOItype=skel # all or skel


#make the appropriate directories for the simulation
set dirtag = "pCellFlux"

foreach i (`seq $run1 $run2`)
	set datadirname=$RUNdir/data/GEO_$GEOname/SRCMP_$SRCMPname/$PARname/Run$i
	if($RUNACTType == 'Root')	then
		set rootoutdir=$datadirname/BustOutRoot
		set rootdatdir=$datadirname/RootData
		mkdir -p $rootoutdir
		mkdir -p $rootdatdir
		unset rootoutdir
		unset rootdata
	else
		foreach p (Edep 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28)
			if($p == "Edep")  then
				set dirname = $datadirname/Edep_MultiSD
			else if($p < 10) then
				set dirname = $datadirname/$dirtag"0"$p
				#echo $dirname
			else
				set dirname = $datadirname/$dirtag$p
				#echo $dirname
			endif
			mkdir -p $dirname
		end
	endif
	#echo "$runname $GEOtype $GEOdir $GEOname $SRCMPdir $SRCMPname $isSRCMPsparse $datadirname $isElectron $isPhoton $RUNACTType $ebin $OOItype $macfile > $datadirname/log.txt"
	$runname $GEOtype $GEOdir $GEOname $SRCMPdir $SRCMPname $isSRCMPsparse $datadirname $isElectron $isPhoton $RUNACTType $ebin $OOItype $macfile > $datadirname/log.txt
	#valgrind --leak-check=full --show-reachable=yes --log-file=LeakCheck.log $G4WORKDIR/bin/$G4SYSTEM/$runname $isReg $GEOdir $GEOname $SRCMPdir $SRCMPname $datadirname $isElectron $isPhoton $isroot $macfile > $datadirname/log.txt
	#valgrind --leak-check=full --log-file=LeakCheck.log $G4WORKDIR/bin/$G4SYSTEM/$runname $isReg $GEOdir $GEOname $SRCMPdir $SRCMPname $datadirname $isElectron $isPhoton $isroot $macfile > $datadirname/log.txt
	echo "Finish VHDMSDv3 run #$i [$ORGANname]! good job bucko!"
	#sleep 600
end


unset nwdir
unset myg4dir
unset runname
unset rdirnam
unset GEOdir
unset SRCMPdir
unset RUNdir
unset GEOname
unset SRCMPname
unset runN
unset PARname
unset macfile
unset datadirname
unset dirname
unset dirtag
unset isElectron
unset isPhoton
unset ORGANname
unset run1
unset run2
unset isSRCMPsparse
unset GEOtype
unset isSRCMPsparse
unset RUNACTType
unset ebin
unset OOItype
