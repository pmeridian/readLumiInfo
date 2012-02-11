#!/bin/tcsh

cd /afs/cern.ch/cms/CAF/CMSPHYS/PHYS_EGAMMA/electrons/meridian/CMSSW_4_2_8/src
cmsenv
cd -
foreach run (`cat goodRuns.txt`)
    echo $run
    cat <<EOF >! test.C
{
std::cout <<"Hello" << std::endl;
gROOT->ProcessLine(".L readLumiInfo/readLumiCsv.cc+");
readLumiCsv("${run}-perbunch.csv",1);
}
EOF
    root -l -b -q test.C
    mv lumiByLS.root lumiByLS_${run}.root
end    
