#include "MVDController.hh"
#include "eudaq/Utils.hh"
#include<sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <vector>
#include<stdio.h>
#include<signal.h>
#include<math.h>
#include <iostream>

using eudaq::to_string;
using namespace std;

MVDController::MVDController() :
		m_modv551(0) {
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
	NumOfChan = param.Get("NumOfChan", 1280);
	NumOfADC = param.Get("NumOfADC", 2);
	m_modv551 = new ModV551(param.Get("Base551", 0));
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
			if (m_enabled.back())
				NumOfSil++;
		}
	}
	//Read and sett calibration from file ./mvd/conf/Calibration.dat
	ReadCalFromFileToHw();
}
bool MVDController::Enabled(unsigned adc, unsigned subadc) {
	return adc < NumOfADC && subadc < MODV550_CHANNELS && m_enabled[adc * MODV550_CHANNELS + subadc];
}
std::vector<unsigned int> MVDController::Time(unsigned adc) {
	std::vector<unsigned int> result(4);
	result[0] = 0xAABBCCD0 + adc;
	gettimeofday(&tv, NULL);
	sec = tv.tv_sec;
	microsec = tv.tv_usec;
	result[1] = sec;
	result[2] = microsec;
	result[3] = 0;
	return result;
}
std::vector<unsigned int> MVDController::Read(unsigned adc, unsigned subadc) {
	if (adc >= NumOfADC)
		EUDAQ_THROW("Invalid ADC (" + to_string(adc) + " >= " + to_string(NumOfADC) + ")");
	if (subadc >= MODV550_CHANNELS)
		EUDAQ_THROW("Invalid SubADC (" + to_string(subadc) + " >= " + to_string(MODV550_CHANNELS) + ")");
	unsigned hits = m_modv550[adc]->GetWordCounter(subadc);
	std::vector<unsigned int> result(hits + 2);
	result[0] = ((0xAABBCC00 | adc << 1) | subadc); //end plane
	for (unsigned i = 1; i < hits + 1; ++i) {
		result[i] = m_modv550[adc]->GetEvent(subadc);
		if ((adc == 1) & (subadc == 1) & (i == hits)) {
			result[hits + 1] = 0xAABBCCDD;
		} //end events
	}
	if ((adc == 1) & (subadc == 1) & (hits < 1)) {
		result[1] = 0xAABBCCDD;
	} //end events
	return result;
}
bool MVDController::DataBusy() {
	return m_modv551->GetBusy();
}
bool MVDController::ActiveSequence() {
	return m_modv551->GetActiveSequence();
}
bool MVDController::DataReady() {
	return m_modv551->GetDataRedy();
}
bool MVDController::GetStatTrigger() {
	unsigned int p = m_modv551->GetTrigger();
	return p;
}
int MVDController::swap4(int w4) {
	unsigned int rw4;
	unsigned char b[4];
	for (int i = 0; i < 4; i++)
		b[i] = (w4 >> i * 8) & 0xff;
	rw4 = 0;
	for (int i = 0; i < 4; i++)
		rw4 += b[i] << (3 - i) * 8;
	return (int) rw4;
}
void MVDController::ReadCalFromFileToHw() {
	/*
	 * This function read calibration from file what have name "Calibration.dat" and placed in the folder  ./mvd/conf
	 * and set this calibration to the crate
	 */
	unsigned int tmp;
	int NumOfCrate;
	int ADC_int;
	FILE* SiFile;
	int string=200;
	const char* OFName;
	//char OFName[200];

	bool ADC_Status[4];
	//only 3 sillicon detectors
	ADC_Status[0] = true; //telescope module 1
	ADC_Status[1] = true; //telescope module 2
	ADC_Status[2] = false; //not active ADC in v550
	ADC_Status[3] = true; //telescope module 3

	m_modv551->Init(1280);
	for (unsigned int i = 0; i < NumOfADC; i++) {
		m_modv550[i]->Init(NumOfChan);
		m_modv550[i]->SetPedThrZero();
	} //i

	OFName = new char[string];
	OFName = "../mvd/conf/Calibration.dat";
	SiFile = fopen(OFName, "r");

	if (!SiFile) {
		printf("!!! ERROR !!! \n");
		printf("calibration file does not exist \n");
		//return;
		CalibAutoTrig();
		ReadCalFromHwToFile();
	}
	SiFile = fopen(OFName, "r");
	//Run Header
	char Header[80];
	for (int i = 0; i < 4; i++) {
		fgets(Header, 80, SiFile);
		printf("%s", Header);
	}
	int chan = 0;
	NumOfCrate = 0;
	ADC_int = 0;

	for (int i = 0; i < EventLength; i++) { //For all channel of strip detector  //EventLength
		if (feof(SiFile) || ferror(SiFile))
			return;
		fread(&tmp, 4, 1, SiFile);
		chan++;
		//printf("TMP0=%x \n", tmp);
		if (tmp == 0xAABBCCDD)
			break; //end of the calibration data
		if (tmp == 0xAABBCC00) {
			NumOfCrate = 0;
			ADC_int = 0;
			chan = 0;
			continue;
		} //start for first plane telescope
		if (tmp == 0xAABBCC01) {
			NumOfCrate = 0;
			ADC_int = 1;
			chan = 0;
			continue;
		} //start for second plane telescope
		if (tmp == 0xAABBCC03) {
			NumOfCrate = 1;
			ADC_int = 1;
			chan = 0;
			continue;
		} //start for third plane telescope
		if (!ADC_Status[ADC_int + NumOfCrate * NumOfADC])		continue;
		m_modv550[NumOfCrate]->SetPedestalThresholdFull(ADC_int, chan, tmp);
	}
	if (feof(SiFile) || ferror(SiFile)) {
		return;
	}
	return;
}
void MVDController::CalibAutoTrig() {

	unsigned int HitNumberADC0;
	unsigned int HitNumberADC1;
	unsigned int OR;
	unsigned int V;
	int ChNum;
	int ChData;
	int SilcPointer;
	bool ADC_Status[4];
	int string=200;
	bool noise;
	unsigned int CalVal[BuffSize + 8];
	int iev;

	FILE* Noise_file;

	FileName = new char[string];
	FileName = "../mvd/conf/Noise.dat";
	Noise_file = fopen(FileName, "w");
	noise = true;
	if(!Noise_file){
		  printf("Please delete noise data file \n");
		  noise = false;
	}
	//event time
	time_t timer;
	struct tm *date;

	time(&timer);
	date = localtime(&timer);
	fprintf(Noise_file, "*Calibration file:  \n");
	fprintf(Noise_file, "*time %d:%d, date %d/%d/%d \n", date->tm_hour, date->tm_min, date->tm_mday, date->tm_mon + 1, date->tm_year + 1900);
	fprintf(Noise_file, "*init_SEQ: t1=%d, t2=%d, t3=%d, t4=%d, t5=%d \n", m_modv551->time1, m_modv551->time1, m_modv551->time3, m_modv551->time4, m_modv551->time5);
	fprintf(Noise_file, "*REM = %s \n", FileName);


	//only 3 sillicon detectors
	ADC_Status[0] = true; //telescope module 1
	ADC_Status[1] = true; //telescope module 2
	ADC_Status[2] = false; //not active ADC in v550
	ADC_Status[3] = true; //telescope module 3

	m_modv551->Init(NumOfChan);

	for (unsigned int i = 0; i < NumOfADC; i++) {
		m_modv550[i]->Init(NumOfChan);
		m_modv550[i]->SetPedThrZero();
	} //i

	int* Ped = new int[BuffSize];
	int* Ped2 = new int[BuffSize];
	int* Noi = new int[BuffSize];
	int* Norm = new int[BuffSize];

	for (int tmp = 0; tmp < BuffSize; tmp++) {
		Ped[tmp] = 0;
		Ped2[tmp] = 0;
		Noi[tmp] = 0;
		Norm[tmp] = 0;
	} //tmp

	for (int i_trig = 0; i_trig < 1000; i_trig++) {
		m_modv551->SoftwareTrigger();
		if ((i_trig % 100) == 0)
			printf("Calibration: Event - %d \n\r", i_trig);
		SilcPointer = 0;
		for (unsigned int i = 0; i < NumOfADC; i++) {
			while (!m_modv551->GetDataRedy()) ;
			m_modv550[i]->GetWordCounter(HitNumberADC0, HitNumberADC1);
			for (int ADC_int = 0; ADC_int < 2; ADC_int++) {
				if (!ADC_Status[ADC_int + i * NumOfADC])	continue;
				if (ADC_int == 0) {
					for (unsigned int chan = 0; chan < HitNumberADC0; chan++) {
						m_modv550[i]->GetFifo0(OR, V, &ChNum, &ChData);
						Ped[ChNum + SilcPointer * NumOfChan] += ChData;
						Ped2[ChNum + SilcPointer * NumOfChan] += ChData * ChData;
						Norm[ChNum + SilcPointer * NumOfChan] += 1;
					} //chan
					SilcPointer++;
				} //ADC0
				if (ADC_int == 1) {
					for (unsigned int chan = 0; chan < HitNumberADC1; chan++) {
						m_modv550[i]->GetFifo1(OR, V, &ChNum, &ChData);
						Ped[ChNum + SilcPointer * NumOfChan] += ChData;
						Ped2[ChNum + SilcPointer * NumOfChan] += ChData * ChData;
						Norm[ChNum + SilcPointer * NumOfChan] += 1;
					} //chan
					SilcPointer++;
				} //ADC1
			} //ADC_int
		} //i
	} //i_trig
	for (int tmp = 0; tmp < BuffSize; tmp++) {
		if (Norm[tmp] > 1) {
			Ped[tmp] = Ped[tmp] / Norm[tmp];
			Ped2[tmp] = Ped2[tmp] / Norm[tmp];
			Noi[tmp] = (int) (sqrt( (double)( (Norm[tmp] / (Norm[tmp] - 1)) * (Ped2[tmp] - Ped[tmp] * Ped[tmp]) ) ) );
		} else {
			Ped[tmp] = 0;
			Noi[tmp] = 0;
		}
	} //tmp

	int ped_tmp, thr_tmp;
	SilcPointer = 0;
	iev = 0;
	CalVal[iev] = 0xAABBCC00;
	for (unsigned int i = 0; i < NumOfADC; i++) {
		for (int ADC_int = 0; ADC_int < 2; ADC_int++) {
			if (!ADC_Status[ADC_int + i * NumOfADC])	continue;
			 CalVal[iev++] = ( (0xAABBCC00 | i << 1) | ADC_int);
			for (unsigned int chan = 0; chan < NumOfChan; chan++) {
				ped_tmp = Ped[chan + SilcPointer * NumOfChan];
				thr_tmp = ped_tmp + 1 * Noi[chan + SilcPointer * NumOfChan];
				CalVal[iev++] = Noi[chan + SilcPointer*NumOfChan];
				m_modv550[i]->SetPedestalThreshold(ADC_int, chan, ped_tmp, thr_tmp);
			} //chan
			SilcPointer++;
		} //ADC_int
	} //i
	CalVal[iev++] = 0xAABBCCDD;
	if (noise){
		fwrite(CalVal, 4, iev + 1, Noise_file);
		fclose(Noise_file);
	 	printf("!!!!!===Noise data file is saved===!!!!! \n");
	}
	delete[] Ped;
	delete[] Ped2;
	delete[] Noi;
	delete[] Norm;
} //CalibAutoTrig
void MVDController::ReadCalFromHwToFile() {
	/*
	 * This function read calibration data from file hardware and save to file
	 * This file will have location in folder ./conf
	 */
	int string=200;
	char* OFName;
	int iev;
	unsigned int CalVal[BuffSize + 8];

	bool ADC_Status[4];
	//only 3 sillicon detectors
	ADC_Status[0] = true; //telescope module 1
	ADC_Status[1] = true; //telescope module 2
	ADC_Status[2] = false; //not active ADC in the v550
	ADC_Status[3] = true; //telescope module 3

	FILE* fp ;

	OFName = new char[string];
	OFName = "../mvd/conf/Calibration.dat";
	fp = fopen(OFName,"w");


	 //event time
	 time_t    timer;
	 struct tm *date;
	 /*---  write header  ---*/
	 time(&timer);
	 date = localtime(&timer);
	 fprintf(fp,"*Calibration file: %s \n", OFName);
	 fprintf(fp,"*time %d:%d, date %d/%d/%d \n",date->tm_hour, date->tm_min, date->tm_mday, date->tm_mon+1, date->tm_year+1900);
	 fprintf(fp,"*init_SEQ: t1=%d, t2=%d, t3=%d, t4=%d, t5=%d \n",m_modv551->time1,m_modv551->time1,m_modv551->time3,m_modv551->time4,m_modv551->time5);
	 fprintf(fp,"*REM = \n");

	 int ped_tmp, thr_tmp, PedThr;

	 ped_tmp = 0;
	 thr_tmp = 0;

     iev = 0;
     CalVal[iev] = 0xAABBCC00;
	 for(int i = 0; i < NumOfADC; i++){//number of crate
		 for(int ADC_int = 0; ADC_int < 2; ADC_int++){//number of ADC
			 if(!ADC_Status[ADC_int + i*NumOfADC]) continue;
			 CalVal[iev++] = ( (0xAABBCC00 | i << 1) | ADC_int); /*-- end of ADC --*/
			 for(int chan = 0; chan < NumOfChan; chan++){
				 m_modv550[i]->GetPedestalThresholdFull(ADC_int, chan, &PedThr);
				 CalVal[iev++] = PedThr;
			 }//chan
		 }//ADC_int
	 }//i
	 CalVal[iev++] = 0xAABBCCDD; /*-- end of Pedistal Threshold --*/
	 fwrite(CalVal,4,iev + 1,fp);
	 fclose(fp);
} //ReadCalFromHwToFile

