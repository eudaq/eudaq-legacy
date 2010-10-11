#ifndef _DUTGlobal
#define _DUTGlobal

#include<rude/config.h>


using namespace rude;
using namespace std;

#include<TROOT.h>

class Global{

  Bool_t GlobStatus;

 public:

  Int_t RunNumber;

  Config config;
  Char_t* ConfigPath; 

  //Run
  Double_t Energy;

  //Telescope

  Int_t TELNumberOfPlanes;
  Int_t* TELNumberOfStripsX;
  Int_t* TELNumberOfStripsY;
  
  Double_t* TELStripSizeX;
  Double_t* TELStripSizeY;

  Double_t* TELSigmaXCogX;
  Double_t* TELSigmaXCogY;

  Double_t* TELOffsetZ;

  Double_t* TELThickness;

  Double_t* TELSeedCutX;
  Double_t* TELSeedCutY;

  Int_t TELMaxHit;

  const Char_t* TELRawPath;

  //TelescopeCorrelation
  Double_t* TELCorrealationSlopeX;
  Double_t* TELCorrealationConstX;
  Double_t* TELCorrIntervalX;

  Double_t* TELCorrealationSlopeY;
  Double_t* TELCorrealationConstY;
  Double_t* TELCorrIntervalY;

  //DUT
  const Char_t* DUTName;

  Int_t DUTNumberOfPixelsX;
  Int_t DUTNumberOfPixelsY;

  Double_t DUTPixSizeX;
  Double_t DUTPixSizeY;
  
  Double_t DUTPhiX;
  Double_t DUTPhiY;
  Double_t DUTPhiZ;

  Double_t DUTOffsetX;
  Double_t DUTOffsetY;
  Double_t DUTOffsetZ;

  Double_t DUTThickness;

  Float_t DUTPedChiCut;
  Float_t DUTGain;

  Float_t DUTSeedCut;
  Float_t DUTNeighCut;

  Float_t DUTCorrealationSlopeX;
  Float_t DUTCorrealationConstX;
  Float_t DUTCorrIntervalX;

  Float_t DUTCorrealationSlopeY;
  Float_t DUTCorrealationConstY;
  Float_t DUTCorrIntervalY;

  Int_t DUTMaxHit;
  const Char_t* DUTRawPath;
  
  Global(Int_t);
  Int_t PrintParameters();
  ~Global();

};//Global

#endif
