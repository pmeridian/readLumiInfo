#!/bin/tcsh

cd /afs/cern.ch/cms/CAF/CMSPHYS/PHYS_EGAMMA/electrons/meridian/CMSSW_4_2_8/src
cmsenv
cd -
foreach run (`cat goodRuns.txt`)
    echo $run
    lumiCalc2.py -r ${run} -b stable -o ${run}-perbunch.csv -norm pp7TeV -xingMinLum 5.0e-03 lumibylsXing >! ${run}-perbunch.log 
    lumiCalc2.py -r ${run} -b stable -o ${run}-perls.csv -n 0.0429 -norm pp7TeV lumibyls >! ${run}-perls.log 
end    
