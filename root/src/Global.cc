#include<Global.hh>

#include<iostream>

Global::Global(Int_t RN):RunNumber(RN){

  Int_t StringSize = 200;

  ConfigPath = new Char_t[StringSize];
  sprintf(ConfigPath,"../conf/mvd-config.ini",RunNumber);

  Char_t TELPlaneName[20];

  if(config.load(ConfigPath)){

    config.setSection("Run");
    Energy = config.getDoubleValue("Energy");

    config.setSection("Telescope");
    TELNumberOfPlanes = config.getIntValue("TELNumberOfPlanes");
    TELRawPath = config.getStringValue("TELRawPath");
    TELMaxHit = config.getIntValue("TELMaxHit");

    TELNumberOfStripsX = new Int_t[TELNumberOfPlanes];
    TELNumberOfStripsY = new Int_t[TELNumberOfPlanes];

    TELStripSizeX = new Double_t[TELNumberOfPlanes];
    TELStripSizeY = new Double_t[TELNumberOfPlanes];

    TELSigmaXCogX = new Double_t[TELNumberOfPlanes];
    TELSigmaXCogY = new Double_t[TELNumberOfPlanes];

    TELOffsetZ = new Double_t[TELNumberOfPlanes];

    TELThickness = new Double_t[TELNumberOfPlanes];

    TELSeedCutX = new Double_t[TELNumberOfPlanes];
    TELSeedCutY = new Double_t[TELNumberOfPlanes];

    TELCorrealationSlopeX = new Double_t[TELNumberOfPlanes - 1];
    TELCorrealationConstX = new Double_t[TELNumberOfPlanes - 1];
    TELCorrIntervalX = new Double_t[TELNumberOfPlanes - 1];

    TELCorrealationSlopeY = new Double_t[TELNumberOfPlanes - 1];
    TELCorrealationConstY = new Double_t[TELNumberOfPlanes - 1];
    TELCorrIntervalY = new Double_t[TELNumberOfPlanes - 1];

    for(Int_t i = 0; i < TELNumberOfPlanes; i++){

      sprintf(TELPlaneName,"Telescope%u",i+1);
      config.setSection(TELPlaneName);

      TELNumberOfStripsX[i] = config.getIntValue("TELNumberOfStripsX");
      TELNumberOfStripsY[i] = config.getIntValue("TELNumberOfStripsY");

      TELSigmaXCogX[i] = config.getDoubleValue("TELSigmaXCogX");
      TELSigmaXCogY[i] = config.getDoubleValue("TELSigmaXCogY");

      TELStripSizeX[i] = config.getDoubleValue("TELStripSizeX");
      TELStripSizeY[i] = config.getDoubleValue("TELStripSizeY");

      TELOffsetZ[i] = config.getDoubleValue("TELOffsetZ");

      TELThickness[i] = config.getDoubleValue("TELThickness");

      TELSeedCutX[i] = config.getDoubleValue("TELSeedCutX");
      TELSeedCutY[i] = config.getDoubleValue("TELSeedCutY");

    }//i

    Double_t tmpCorrelation[4];

    for(Int_t i = 0; i < TELNumberOfPlanes - 1; i++){

      config.setSection("TelescopeCorrelationX");
      
      sprintf(ConfigPath,"TELPointA%d%dX",i+1,i+2);
      tmpCorrelation[0] = config.getDoubleValue(ConfigPath);

      sprintf(ConfigPath,"TELPointA%d%dY",i+1,i+2);
      tmpCorrelation[1] = config.getDoubleValue(ConfigPath);

      sprintf(ConfigPath,"TELPointB%d%dX",i+1,i+2);
      tmpCorrelation[2] = config.getDoubleValue(ConfigPath);

      sprintf(ConfigPath,"TELPointB%d%dY",i+1,i+2);
      tmpCorrelation[3] = config.getDoubleValue(ConfigPath);
     
      TELCorrealationSlopeX[i] = (tmpCorrelation[1] - tmpCorrelation[3])/(tmpCorrelation[0] - tmpCorrelation[2]);
      TELCorrealationConstX[i] = tmpCorrelation[1] - TELCorrealationSlopeX[i]*tmpCorrelation[0];

      sprintf(ConfigPath,"TELCorrIntervalX%d%d",i+1,i+2);
      TELCorrIntervalX[i] = config.getDoubleValue(ConfigPath);

      config.setSection("TelescopeCorrelationY");

      sprintf(ConfigPath,"TELPointA%d%dX",i+1,i+2);
      tmpCorrelation[0] = config.getDoubleValue(ConfigPath);

      sprintf(ConfigPath,"TELPointA%d%dY",i+1,i+2);
      tmpCorrelation[1] = config.getDoubleValue(ConfigPath);

      sprintf(ConfigPath,"TELPointB%d%dX",i+1,i+2);
      tmpCorrelation[2] = config.getDoubleValue(ConfigPath);

      sprintf(ConfigPath,"TELPointB%d%dY",i+1,i+2);
      tmpCorrelation[3] = config.getDoubleValue(ConfigPath);

      TELCorrealationSlopeY[i] = (tmpCorrelation[1] - tmpCorrelation[3])/(tmpCorrelation[0] - tmpCorrelation[2]);
      TELCorrealationConstY[i] = tmpCorrelation[1] - TELCorrealationSlopeY[i]*tmpCorrelation[0];

      sprintf(ConfigPath,"TELCorrIntervalY%d%d",i+1,i+2);
      TELCorrIntervalY[i] = config.getDoubleValue(ConfigPath);

    }

    DUTName = new Char_t[StringSize];
    DUTRawPath = new Char_t[StringSize];

    config.setSection("DUT");
    DUTName = config.getStringValue("DUTName");
    DUTRawPath = config.getStringValue("DUTRawPath");
    DUTMaxHit = config.getIntValue("DUTMaxHit");

    DUTNumberOfPixelsX = config.getIntValue("DUTNumberOfPixelsX");
    DUTNumberOfPixelsY = config.getIntValue("DUTNumberOfPixelsY");
    DUTPixSizeX = config.getDoubleValue("DUTPixSizeX");
    DUTPixSizeY = config.getDoubleValue("DUTPixSizeY");
    
    DUTPhiX = config.getDoubleValue("DUTPhiX");
    DUTPhiY = config.getDoubleValue("DUTPhiY");
    DUTPhiZ = config.getDoubleValue("DUTPhiZ");
    
    DUTOffsetX = config.getDoubleValue("DUTOffsetX");
    DUTOffsetY = config.getDoubleValue("DUTOffsetY");
    DUTOffsetZ = config.getDoubleValue("DUTOffsetZ");

    DUTThickness = config.getDoubleValue("DUTThickness");

    DUTPedChiCut = config.getDoubleValue("DUTPedChiCut");
    DUTGain = config.getDoubleValue("DUTGain");

    DUTSeedCut = config.getDoubleValue("DUTSeedCut");
    DUTNeighCut = config.getDoubleValue("DUTNeighCut");

    tmpCorrelation[0] = config.getDoubleValue("DUTPointAXX");
    tmpCorrelation[1] = config.getDoubleValue("DUTPointAXY");
    tmpCorrelation[2] = config.getDoubleValue("DUTPointBXX");
    tmpCorrelation[3] = config.getDoubleValue("DUTPointBXY");

    DUTCorrealationSlopeX = (tmpCorrelation[1] - tmpCorrelation[3])/(tmpCorrelation[0] - tmpCorrelation[2]);
    DUTCorrealationConstX = tmpCorrelation[1] - DUTCorrealationSlopeX*tmpCorrelation[0];

    tmpCorrelation[0] = config.getDoubleValue("DUTPointAYX");
    tmpCorrelation[1] = config.getDoubleValue("DUTPointAYY");
    tmpCorrelation[2] = config.getDoubleValue("DUTPointBYX");
    tmpCorrelation[3] = config.getDoubleValue("DUTPointBYY");

    DUTCorrealationSlopeY = (tmpCorrelation[1] - tmpCorrelation[3])/(tmpCorrelation[0] - tmpCorrelation[2]);
    DUTCorrealationConstY = tmpCorrelation[1] - DUTCorrealationSlopeY*tmpCorrelation[0];

    DUTCorrIntervalX = config.getDoubleValue("DUTCorrIntervalX");
    DUTCorrIntervalY = config.getDoubleValue("DUTCorrIntervalY");

    GlobStatus = true;

  }//reading config file
 
  else{

    cout << "!!! WARNING !!!" << endl;
    cout << "I can't find config file: " << ConfigPath << endl;

    GlobStatus = false;

  }

}//Global

Int_t Global::PrintParameters(){

  cout << "-------------------------------------------------------" << endl << endl;
  cout << "   These are parameters of the " << RunNumber << " run " << endl << endl;
  cout << "-------------------------------------------------------" << endl << endl;

  if(!GlobStatus){

    cout << "!!! WARNING !!!" << endl;
    cout << "I can't find config file: " << ConfigPath << endl;

    return 0;

  }

  cout << "Energy : " << Energy << "GeV" << endl << endl;

  cout << "Telescope :" << endl << endl;

  for(Int_t i = 0; i < TELNumberOfPlanes; i++){

    cout << "PLANE " << i+1 << endl << endl;
    cout << "  Number of strips in the X direction " << TELNumberOfStripsX[i] << endl;
    cout << "  Number of strips in the Y direction " << TELNumberOfStripsY[i] << endl;
    cout << "  Strip size x " << TELStripSizeX[i] << endl;
    cout << "  Strip size y " << TELStripSizeY[i] << endl;
    cout << "  Plane thickness " << TELThickness[i] << endl;

    cout << "  Offset Z " << TELOffsetZ[i] << endl << endl;

    cout << "  Seed cut for plane X " << TELSeedCutX[i] << endl;
    cout << "  Seed cut for plane X " << TELSeedCutY[i] << endl;

  }//i

  cout << "-------------------------------------------------------" << endl << endl; 

  cout << "Detector undet test " << DUTName << " :" << endl << endl;

  cout << "  Number of pixels in the X direction " << DUTNumberOfPixelsX << endl;
  cout << "  Number of pixels in the Y direction " << DUTNumberOfPixelsY << endl;
  cout << "  Pixel size " << DUTPixSizeX << " x " << DUTPixSizeY << endl;
  cout << "  Detector thickness " << DUTThickness << endl;
  cout << "  DUT Chi2 Cut " << DUTPedChiCut << endl;
  cout << "  DUT gain " << DUTGain << endl;
  cout << "  DUT Seed Cut " << DUTSeedCut << endl;
  cout << "  DUT Neighbours Cut " << DUTNeighCut << endl;

  cout << "  Angle PhiX " << DUTPhiX << endl;
  cout << "  Angle PhiY " << DUTPhiY << endl;
  cout << "  Angle PhiZ " << DUTPhiZ << endl;
  
  cout << "  Offset X " << DUTOffsetX << endl;
  cout << "  Offset Y " << DUTOffsetY << endl;
  cout << "  Offset Z " << DUTOffsetZ << endl << endl;

  cout << "-------------------------------------------------------" << endl << endl; 

  return 1;

}//PrintParameters

Global::~Global(){

  delete [] TELNumberOfStripsX;
  delete [] TELNumberOfStripsY;

  delete [] TELStripSizeX;
  delete [] TELStripSizeY;

  delete [] TELSigmaXCogX;
  delete [] TELSigmaXCogY;
  
  delete [] TELOffsetZ;

  delete [] TELThickness;
  
  delete [] ConfigPath;

  delete [] TELCorrealationSlopeX;
  delete [] TELCorrealationConstX;
  delete [] TELCorrealationSlopeY;
  delete [] TELCorrealationConstY;
 
  delete [] TELCorrIntervalX;
  delete [] TELCorrIntervalY;

}
