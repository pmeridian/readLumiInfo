#include <deque>
#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TNtuple.h>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

typedef std::deque <std::string> record_t;
typedef std::deque <record_t> table_t;

void readLumiCsv(char* csvfile, int format)
{
  //*****************************************************************************************************
  //  PM 2010.12.01
  //  Use as input file csv file as produced by the lumiCalc or the lumiSumPlot
  //  format1 (luminosity by xing): lumiCalc.py -i goodrunlist_json.txt -o goodrun_lumistatus.csv  --nowarning lumibylsXing
  //  format2 (luminosity by ls with lsstarttime): lumiSumPlot.py -c oracle://cms_orcoff_prod/cms_lumi_prod -P /afs/cern.ch/cms/DB/lumi -begin "05/19/10 22:00:00.000000" 
  //  -end "11/04/10 00:00:00.000000" -batch lumiByLS-time.png timels -o /tmp/meridian/lumiByLS.csv
  //*****************************************************************************************************

  // int format=1;//format by lumiCalc
  // int format=2;//format by lumiSumPlot
  
  ifstream in;
  in.open(csvfile);

  std::string s;

  table_t table;
  table.clear();

  int nlines=0;

  while (std::getline( in, s ))
    {
      ++nlines;
      int nfields=0;
      std::istringstream ss( s );
      record_t record;
      std::string field;
      bool final = true;
      while (std::getline( ss, field, ',' ))
	{
	  record.push_back( field );
	  final = ss.eof();
	  ++nfields;
	}
      if (!final)
	record.push_back( std::string() );
      //      std::cout << "Line " << nlines << ": found " << nfields << " field(s)" << std::endl;
      table.push_back( record );
    }

  std::cout << "Found " << nlines << " LS" << std::endl;

  TFile *f = new TFile("lumiByLS.root","RECREATE");
  TNtuple *ntuple = new TNtuple("lumiByLS","lumiByLS","run:LS:delLumi:recLumi:nBX:avgInstLumi:lsstarttimesec:lsstarttimeusec");
  if ( format == 1)
    {
      for (int irecord=0;irecord<table.size();++irecord)
	{
	  int run=atoi(table[irecord][0].c_str());
	  int LS=atoi(table[irecord][1].c_str());
	  float delLumi=atof(table[irecord][2].c_str());
	  float recLumi=atof(table[irecord][3].c_str());
	  int nBX=0;
	  int iField=4;
	  float avgInstLumi=0.;
	  if (LS==0)
	    continue;
	  while (iField<table[irecord].size())
	    {
	      float bxLumi=atof(table[irecord][++iField].c_str());
	      //Apply a minimal cut on bxLumi to avoid counting parasitic non-colliding bunches... tuned on 2010 intensities
	      if (bxLumi>0.005)
		{
		  ++nBX;
		  avgInstLumi+=bxLumi;
		}
	      ++iField;
	    }
	  avgInstLumi=avgInstLumi/nBX;
	  if (irecord%100==0)
	    std::cout << "run " << run << " LS " << LS << " delLumi " << delLumi << " nBX " << nBX << " avgInstLumi " << avgInstLumi << std::endl;     
	  
	  ntuple->Fill(float(run),float(LS),float(delLumi),float(recLumi),float(nBX),float(avgInstLumi),float(-1),float(-1));
	}
    }
  else
    if ( format == 2)
      {
	for (int irecord=0;irecord<table.size();++irecord)
	  {
	    int run=atoi(table[irecord][0].c_str());
	    string runstarttime=table[irecord][1];
	    int LS=atoi(table[irecord][2].c_str());
	    string lsstarttime=table[irecord][3];
	    tm lstime;
	    strptime(lsstarttime.c_str(),"%Y-%m-%d %H:%M:%S",&lstime);
	    int lstimesec=mktime(&lstime);
	    int lstimeusec=atoi(lsstarttime.c_str()+20);  
	    float delLumi=atof(table[irecord][4].c_str());
	    float recLumi=atof(table[irecord][5].c_str());
	    time_t lstime_t=lstimesec;
	    if (LS==0)	  
	      continue;
	    
	    if (irecord%100==0)
	      {
		char lstime_str[80];
		strftime (lstime_str,80,"%a %x %X",localtime(&lstime_t));
		lstime_str[strcspn ( lstime_str, "\n" )] = '\0';
		std::cout << "run " << run << " LS " << LS << " delLumi " << delLumi << " lsstarttime " << lstime_str << "." << lstimeusec <<std::endl;     
	      }
	    
	    ntuple->Fill(float(run),float(LS),float(delLumi),float(recLumi),float(-1),float(-1),float(lstimesec),float(lstimeusec));
	  }
    } 
  f->Write();
}
