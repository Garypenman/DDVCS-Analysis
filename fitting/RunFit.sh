#!/bin/bash

tag=""

# If a tag argument was supplied, prepend an underscore
if [ -n "$1" ]; then
    tag="_$1"
fi

root -q /home/gp140f/brufit/macros/LoadBru.C FitThetaPhi.C

outpdf="TCSFitResults_BHONLY${tag}.pdf"

root -q "MakeFitResultPDF.C(\"${outpdf}\")"

evince "${outpdf}"
