#include "MVDController.hh"
#include "eudaq/Utils.hh"
#include<sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <vector>

using eudaq::to_string;
using namespace std;

MVDController::MVDController() : m_modv551(0) {
}
void MVDController::Clear() {
  delete m_modv551;
  m_modv551 = 0;
  for (unsigned i = 0; i < m_modv550.size(); ++i) {
    delete m_modv550[i];
    m_modv550[i] = 0;
  }
  m_modv550.resize(0);
  m_enabled.resize(0);
}
MVDController::~MVDController() {
  Clear();
}
void MVDController::Configure(const eudaq::Configuration & param) {
  Clear();
  NumOfChan = 	param.Get("NumOfChan", 1280);
  NumOfADC = 	param.Get("NumOfADC", 2);
  m_modv551 = 	new ModV551(param.Get("Base551", 0));
  m_modv551->Init(NumOfChan);

  for (unsigned i = 0; i < NumOfADC; ++i) {
    unsigned addr = param.Get("Base_" + to_string(i), 0);
    m_modv550.push_back(new ModV550(addr));
    m_modv550[i]->Init(NumOfChan);
    m_modv550[i]->SetPedThrZero();

    unsigned status = param.Get("Disable_" + to_string(i), 0);
    for (unsigned j = 0; j < MODV550_CHANNELS; ++j) {
      unsigned mask = 1 << j;
      m_enabled.push_back(!(status & mask));
      if (m_enabled.back()) NumOfSil++;
    }
  }
  //Read and sett calibration from file ./conf/Calibration.dat
  ReadCalFile();
}
bool MVDController::Enabled(unsigned adc, unsigned subadc) {
  return adc < NumOfADC && subadc < MODV550_CHANNELS && m_enabled[adc*MODV550_CHANNELS + subadc];
}
std::vector<unsigned int> MVDController::Time(unsigned adc){
	std::vector<unsigned int> result(4);
	result[0]=0xAABBCCD0 + adc;
    gettimeofday(&tv,NULL);
    sec = tv.tv_sec;
    microsec = tv.tv_usec;
    result[1]= sec;
    result[2] = microsec;
    result[3] = 0;
	return result;
}
std::vector<unsigned int> MVDController::Read(unsigned adc, unsigned subadc) {
  if (adc >= NumOfADC) EUDAQ_THROW("Invalid ADC (" + to_string(adc) + " >= " + to_string(NumOfADC) + ")");
  if (subadc >= MODV550_CHANNELS) EUDAQ_THROW("Invalid SubADC (" + to_string(subadc) + " >= " + to_string(MODV550_CHANNELS) + ")");
  unsigned hits = m_modv550[adc]->GetWordCounter(subadc);
  std::vector<unsigned int> result(hits+2);
  result[0] = ((0xAABBCC00 | adc << 1) | subadc);//end plane
  for (unsigned i = 1; i < hits+1; ++i) {
    result[i] = m_modv550[adc]->GetEvent(subadc);
    if ( (adc == 1 ) & (subadc == 1) & (i == hits) ) { result[hits+1]=0xAABBCCDD;}//end events
  }
  if ( (adc == 1 ) & (subadc == 1) & ( hits < 1 ) ) { result[1]=0xAABBCCDD; }//end events
  return result;
}
bool MVDController::DataBusy(){
	return m_modv551->GetBusy();
}
bool MVDController:: ActiveSequence(){
	return m_modv551->GetActiveSequence();
}
bool MVDController::DataReady(){
  return m_modv551->GetDataRedy();
}
bool MVDController::GetStatTrigger(){
	unsigned int p = m_modv551->GetTrigger();
	return p;
}
int MVDController::swap4(int w4){
  unsigned int rw4;
  unsigned char b[4];
  for(int i = 0; i < 4; i++) 	b[i] = (w4>>i*8)&0xff;
  rw4 = 0;
  for(int i = 0; i < 4; i++) 	rw4+=b[i]<<(3-i)*8;
  return (int)rw4;
}
void MVDController::ReadCalFile() {
	//This function read calibration from file what have name "Calibration.dat" in folder ./conf
	//and set this calibration to the crate

    int ped_tmp, thr_tmp;
    unsigned int tmp;
    int NumOfCrate;
    int ADC_int;
    FILE* SiFile;
    int string = 200;
    bool ADC_Status[4];

    //only 3 sillicon detectors
    ADC_Status[0] = true; //telescope module 1
    ADC_Status[1] = true; //telescope module 2
    ADC_Status[2] = false; //not active ADC in v550
    ADC_Status[3] = true; //telescope module 3

    printf("DAQ: Set the calibration number: ");
    pather = new char[string];
    pather ="./conf/Calibration.dat";
	SiFile = fopen(pather,"r");

	if(!SiFile){
	  cout << "!!! ERROR !!!" << endl;
	  cout << "File does not exist" << endl;
	  return ;
	}
    //Run Header
    char Header[80];
    for(int i = 0; i < 4; i++){
      fgets(Header,80,SiFile); printf("%s",Header);
    }
    int chan = 0;
    NumOfCrate = 0;
    ADC_int=0;
	for(int i = 0; i < EventLength; i++){//For all channel of strip detector  //EventLength
		if(feof(SiFile) || ferror(SiFile)) return ;

	    ped_tmp = 0;
	    thr_tmp = 0;
	    fread(&tmp,4,1,SiFile);
	    if(tmp == 0xAABBCCDD)  break;
	    //end of the first telescope
	    if(tmp == 0xAABBCC00){
	    	NumOfCrate = 0;
	    	ADC_int = 0;
	    	chan = 0;
	    	printf("ADC=1\n");
	    	continue;
	    }
	    //end of second telescope
	    if(tmp == 0xAABBCC01){
	    	NumOfCrate = 0;
	    	ADC_int = 1;
	    	chan = 0;
	    	printf("ADC=2\n");
	    	continue;
	    }
	    //end of second telescope
	    if(tmp == 0xAABBCC03){
	    	NumOfCrate = 1;
	    	ADC_int = 0;
	    	chan = 0;
	    	printf("ADC=3\n");
	    	continue;
	    }
	    ped_tmp = (tmp >> 12) & 0xFFF;
	    thr_tmp = tmp & 0xFFF;
	    m_modv550[NumOfCrate]->ModV550::SetPedestalThresholdFull(ADC_int,chan, tmp);
	    if (NumOfCrate == 1)	    m_modv550[NumOfCrate]->SetPedestalThreshold(1, chan, ped_tmp, thr_tmp);
	    chan++;
	  }//for
	  if(feof(SiFile) || ferror(SiFile)) { return ; }
	  return ;
}
void MVDController::CalibAutoTrig(){
	int Ped[3840];
	int Ped2[3840];
	int Noi [3840];
	int Norm[3840];

  for(int tmp = 0; tmp < 3840; tmp++) {
    Ped[tmp] = 0;
    Ped2[tmp] = 0;
    Norm[tmp] = 0;
    Noi[tmp] = 0;
  }

  for(int i_trig = 0; i_trig < 5; i_trig++) {
	  printf("Calibration: Event - %d \n\r",i_trig);
	  m_modv551->SoftwareTrigger();
	  while (! DataBusy())	;
	  while (! DataReady())	;
	  SilcPointer = 0;
	  for(unsigned int i = 0; i < NumOfADC; i++) {
		  printf("NumOfADC=%d ", i  );
		  for(int ADC_int = 0; ADC_int < 2; ADC_int++) {
    		if(ADC_int == 0){
    			unsigned int hits = m_modv550[i]->GetWordCounter(ADC_int);
    			for(unsigned int chan = 0; chan < hits; chan++){
    				ChTmp = m_modv550[i]->GetEvent(ADC_int);
    				ChNum = (ChTmp >> 12) & 0xfff;
    				ChData = (ChTmp & 0xfff);
    				Ped[  ChNum + (SilcPointer*NumOfCha) ] += ChData;
    				Ped2[ ChNum + (SilcPointer*NumOfCha) ] += ChData*ChData;
    				Norm[ ChNum + (SilcPointer*NumOfCha) ] += 1;
    			}//chan
    			SilcPointer++;
    		}//ADC0
    		if(ADC_int == 1){
    			unsigned hits = m_modv550[i]->GetWordCounter(ADC_int);
    			for(unsigned int chan = 0; chan < hits; chan++){
    				ChTmp = m_modv550[i]->GetEvent(ADC_int);
    				ChNum = (ChTmp >> 12) & 0xfff;
    				ChData = (ChTmp & 0xfff);
    				Ped[  ChNum + (SilcPointer*NumOfCha) ] += ChData;
    				Ped2[ ChNum + (SilcPointer*NumOfCha) ] += ChData*ChData;
    				Norm[ ChNum + (SilcPointer*NumOfCha) ] += 1;
    			}//chan
    			SilcPointer++;
    		}//ADC1
    	}//ADC_int
    }//i
  }//i_trig
  printf("!!!!!=====Calibration=====!!!!! ");
}//CalibAutoTrig
