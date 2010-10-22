#include "MVDController.hh"
#include "eudaq/Utils.hh"
#include<sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <vector>

#include<signal.h>
#include<math.h>
#include <iostream>


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
  ReadCalFromFileToHw();
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
void MVDController::ReadCalFromFileToHw() {
	/*
	 * This function read calibration from file what have name "Calibration.dat" in folder ./conf
	 * and set this calibration to the crate
	 */

    int ped_tmp, thr_tmp;
    unsigned int tmp;
    int NumOfCrate;
    int ADC_int;
    FILE* SiFile;
    int string = 200;
    bool read_from_file;

    printf("DAQ: Set the calibration number: ");
    pather = new char[string];
    pather ="../conf/Calibration.dat";
	SiFile = fopen(pather,"r");
	read_from_file = true;
	if(!SiFile){
	  cout << "!!! ERROR !!!" << endl;
	  cout << "Calibration file does not exist" << endl;
	  printf("Calibration will be done \n");
	  CalibAutoTrig();
	  ReadCalFromHwToFile();
	  read_from_file = false;
	}
	if (read_from_file){
		//Run Header
		char Header[80];
		for(int i = 0; i < 4; i++){
			fgets(Header,80,SiFile);
			printf("%s",Header);
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
}
void MVDController::CalibAutoTrig(){
	int Ped[3840];
	int Ped2[3840];
	int Noi [3840];
	int Norm[3840];

	int SilcPointer;

	for(int tmp = 0; tmp < 3840; tmp++) {
		Ped[tmp] = 0;
		Ped2[tmp] = 0;
		Norm[tmp] = 0;
		Noi[tmp] = 0;
	}
	m_modv551->SetAutoTrigger(0);

 	for(int i_trig = 0; i_trig < 1000; i_trig++) {
 		if((i_trig%100) == 0) printf("Calibration: Event - %d \n",i_trig); //\r
 		m_modv551->SoftwareTrigger();
 		while (! DataBusy()) ;//printf("data  busy \n")	;
 		while (! DataReady());//  printf("data  ready \n")	;
 		SilcPointer = 0;
 		for (unsigned int adc = 0; adc < NumOfADC; adc++) {
 			for(int sub_adc = 0; sub_adc < 2; sub_adc++) {
 				if (!Enabled(adc, sub_adc))	continue;
 				//printf("adc=%d sub=%d \n", adc, sub_adc );
 				unsigned int hits = m_modv550[adc]->GetWordCounter(sub_adc);
 				//printf("hits=%d \n", hits);
 				for(unsigned int chan = 0; chan < hits; chan++){
 					ChTmp = m_modv550[adc]->GetEvent(sub_adc);
 					ChNum = (ChTmp >> 12) & 0xfff;
 					ChData = (ChTmp & 0xfff);
 					Ped[  ChNum + (SilcPointer*NumOfCha) ] += ChData;
 					Ped2[ ChNum + (SilcPointer*NumOfCha) ] += ChData*ChData;
 					Norm[ ChNum + (SilcPointer*NumOfCha) ] += 1;
 				}//chan
 				SilcPointer++;
 			}//sub_adc
 		}//adc
 	}//i_trig

 	for(int tmp = 0; tmp < 3840; tmp++){
 		if(Norm[tmp] > 1){
 			Ped[tmp] = Ped[tmp]/Norm[tmp];
 			Ped2[tmp] = Ped2[tmp]/Norm[tmp];
 			Noi[tmp] = (int)( sqrt( (double)(Norm[tmp]/(Norm[tmp] - 1) )*( Ped2[tmp] - Ped[tmp]*Ped[tmp]))  );
 		}
 		else{
 			Ped[tmp] = 0;
 			Noi[tmp] = 0;
 		}
 	}//tmp
 	int ped_tmp, thr_tmp;
 	SilcPointer = 0;
 	for(int adc = 0; adc < NumOfADC; adc++){
 		for(int sub_adc = 0; sub_adc < 2; sub_adc++){
 			if (!Enabled(adc, sub_adc))	continue;
 			for(int chan = 0; chan < NumOfChan; chan++){
 				ped_tmp = Ped[chan + SilcPointer*NumOfCha];
 				thr_tmp = ped_tmp + 3*Noi[chan + SilcPointer*NumOfCha];
 				printf("adc=%d sub=%d ped=%d thr=%d\n", adc, sub_adc, ped_tmp, thr_tmp);
 				m_modv550[adc]->SetPedestalThreshold(sub_adc, chan, ped_tmp, thr_tmp);
 			}//chan
 			SilcPointer++;
 		}//ADC_int
 	}//i
 	printf("!!!!!=====Calibration done=====!!!!! \n");
}//CalibAutoTrig
void MVDController::ReadCalFromHwToFile(){
	/*
	 * This function read calibration data from file hardware and save to file
	 * This file will have location in folder ./conf
	 */
	int string=200;
	int iev;
	unsigned int CalVal[BuffSize + 8];

	FILE* fp;

	FileName = new char[string];
	FileName = "../conf/Calibration.dat";
	fp = fopen(FileName, "w");

	//event time
	time_t timer;
	struct tm *date;

	time(&timer);
	date = localtime(&timer);
	fprintf(fp, "*Calibration file:  \n");
	fprintf(fp, "*time %d:%d, date %d/%d/%d \n", date->tm_hour, date->tm_min, date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
	fprintf(fp, "*init_SEQ: t1=%d, t2=%d, t3=%d, t4=%d, t5=%d \n", m_modv551->time1, m_modv551->time1, m_modv551->time3, m_modv551->time4, m_modv551->time5);
	fprintf(fp, "*REM = %s \n", FileName);

	int ped_tmp, thr_tmp, PedThr;

	iev = 0;
	CalVal[iev] = 0xAABBCC00;
	for (int adc = 0; adc < NumOfADC; adc++) {//number of crate
		for (int sub_adc = 0; sub_adc < 2; sub_adc++) {//number of ADC
			if (!Enabled(adc, sub_adc))	continue;
			printf("adc=%d sub=%d \n", adc, sub_adc );
			CalVal[iev++] = ((0xAABBCC00 | adc << 1) | sub_adc);
			for (int chan = 0; chan < NumOfChan; chan++) {
				m_modv550[adc]->GetPedestalThresholdFull(sub_adc, chan, &PedThr);
				CalVal[iev++] = PedThr;
			}//chan
		}//ADC_int
	}//i
	CalVal[iev++] = 0xAABBCCDD;
	fwrite(CalVal, 4, iev + 1, fp);
	fclose(fp);
	printf("!!!!!=====Calibration saved to file=====!!!!! \n");
}//ReadCalFromHwToFile

