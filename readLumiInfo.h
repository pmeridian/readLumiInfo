#include "TFile.h"
#include "TNtuple.h"
#include "TGraph.h"
#include "sys/time.h"

class readLumiInfo
{
 public:
  struct runLS
  {
    int run;
    int LS;
    //operator for comparison of runLS
    bool operator<( const runLS & n ) const
    {
      return ( (this->run < n.run) || ( (this->run == n.run) && (this->LS < n.LS) ) );
     }
    runLS() : run(-1),LS(-1) {};
    runLS(int arun , int aLS) : run(arun),LS(aLS) {};
   };

  struct lumiInfo
  {
    float deliveredInstLumi;
    float recordedInstLumi;
    int nCollidingBunches;
    float avgInstLumiPerBX;
    timeval lsStartTime;
    lumiInfo() : deliveredInstLumi(-1),recordedInstLumi(-1),nCollidingBunches(-1),avgInstLumiPerBX(-1) {}
  };

   typedef std::map< runLS, lumiInfo > lumiTable;
  
  //Constructors
  readLumiInfo() {};
  readLumiInfo(const char* file);

  //Destructor
  ~readLumiInfo() {};
  
  //Total Lumi informations
  float getTotalDeliveredLumi();
  float getTotalRecordedLumi();

  //Total delivered up to run,ls
  float getTotalDeliveredLumi(int run,int ls);
  float getTotalRecordedLumi(int run,int ls);

  //Get start time of a lumisection
  timeval getLuminosityStartTime(int run,int ls);

  //Lumi Information per run
  float getDeliveredLumi(int run);
  float getRecordedLumi(int run);

  //Lumi information per run and LS
  lumiInfo getLumiInfo(int run,int LS);
  float getDeliveredInstLumi(int run,int LS);
  int getNumberOfCollidingBunches(int run,int LS);
  float getAvgInstLumiPerBX(int run,int LS);

  //Instantaneous luminosity graph per run as a function of LS
  TGraph* getHistoDeliveredInstLumi(int run);
  TGraph* getHistoAvgInstLumiPerBX(int run);

  //Integrated luminosity graphs per run as a function of LS
  TGraph* getHistoDeliveredLumi(int run);
  TGraph* getHistoRecordedLumi(int run);

  //Integrated luminosity graphs up to run as a function of LS
  TGraph* getHistoTotalDeliveredLumiVsTime(const time_t* start,const time_t* stop);
  TGraph* getHistoTotalRecordedLumiVsTime(const time_t* start,const time_t* stop);

 private:
  //Holders of lumiTable informations
  TNtuple* lumiTree;
  lumiTable lumiMap;
  
};
