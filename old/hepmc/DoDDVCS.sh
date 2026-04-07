#!/bin/bash

source "/home/garyp/eic/setup_rad.sh"
#cd "/home/garyp/eic/rad/examples/DDVCS/"

if [ -z "$1" ]
then
    echo "No directory given, exiting."
    exit 1
fi
 
#ls $1

for f in $1/*_hplus.root
do
    if [ -f $f ]; then
	plusfile=$f
    fi
    done

for f in $1/*_hminus.root
do
    if [ -f $f ]; then
	minusfile=$f
    fi
done

#echo $plusfile
#echo $minusfile
root -l -b -q "DDVCS_GenHeli.C(\"$plusfile\")"
root -l -b -q "DDVCS_GenHeli.C(\"$minusfile\")"
