#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <iostream>

#include <fstream>
#include <string>
#include <vector>

#include <TStyle.h>
#include <TApplication.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TGClient.h>
#include <TCanvas.h>
#include <TF1.h>
#include <TRandom.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TStyle.h>
#include <TGLayout.h>
#include <TGWindow.h>
#include <TGWidget.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>

#include<Global.hh>

using namespace std;

#define NumOfPlanes 3
#define EventLength 1280* NumOfPlanes + 20 + 256 + 10

TGMainFrame *fMain;
TRootEmbeddedCanvas *fEcanvas;
TCanvas *fCanv;
TCanvas *fCanvas;

FILE* SiFile;
Global* glob;

Int_t NumOpenFile;
Int_t ADCXSize;
Int_t ADCYSize;
Int_t* XADC;
Int_t* YADC;
Int_t* ShiftX;
Int_t* ShiftY;

pthread_t thread;

void ReadNumFile() {
	const Char_t *Path;
	FILE* SiNumberFile;
	Int_t StringLength = 200;

	Path = new Char_t[StringLength];
	Path = "./../data/runnumber.dat";

	SiNumberFile = fopen(Path, "r");
	if (!SiNumberFile) {
		cout << "!!! ERROR !!!" << endl;
		cout << "File " << Path << " does not exist" << endl;
	}
	if (SiNumberFile) {
		printf("----===== Number File is open =====----- \n");
		Char_t Header[80];
		fgets(Header, 80, SiNumberFile); //printf("%s",Header);
		NumOpenFile = atoi(Header);
		printf("file=%d \n", NumOpenFile);
	}
}
void OpenFile() {
	Char_t Path[200];

	sprintf(Path, "./../data/run%d.raw", NumOpenFile);
	SiFile = fopen(Path, "r");

	if (!SiFile) {
		cout << "!!! ERROR !!!" << endl;
		cout << "File " << Path << " does not exist" << endl;
	}
	if (SiFile) {
		printf("----===== File is open =====----- \n");
	}
}
void CloseFile() {
	fclose(SiFile);
}
Int_t swap4(Int_t w4) {
	unsigned int rw4;
	unsigned char b[4];

	for (Int_t i = 0; i < 4; i++)
		b[i] = (w4 >> i * 8) & 0xff;
	rw4 = 0;
	for (Int_t i = 0; i < 4; i++)
		rw4 += b[i] << (3 - i) * 8;
	return (Int_t) rw4;
}
void Variable() {
	glob = new Global(1001);

	ShiftX = new Int_t[glob->TELNumberOfPlanes];
	ShiftY = new Int_t[glob->TELNumberOfPlanes];
	for (Int_t i = 0; i < glob->TELNumberOfPlanes; i++) {
		ShiftX[i] = 0;
		ShiftY[i] = 0;
	}

	ADCXSize = 0;
	ADCYSize = 0;
	for (Int_t i = 0; i < glob->TELNumberOfPlanes; i++) {
		ADCXSize += glob->TELNumberOfStripsX[i];
		ADCYSize += glob->TELNumberOfStripsY[i];
		if (i > 0) {
			ShiftX[i] = ShiftX[i - 1] + glob->TELNumberOfStripsX[i - 1];
			ShiftY[i] = ShiftY[i - 1] + glob->TELNumberOfStripsY[i - 1];
		}

	}
	XADC = new Int_t[ADCXSize];
	YADC = new Int_t[ADCYSize];
	for (Int_t i = 0; i < ADCYSize; i++) {
		XADC[i] = 0;
		YADC[i] = 0;
	}
}
Int_t FindEvent(Int_t EndEvent) {
	int events = 0;
	int k;
	unsigned char tmp_bit;
	unsigned char temp_bit[5000];

	while (1) {
		k = 0;
		while (1) {
			if (feof(SiFile) || ferror(SiFile))
				break;
			fread(&tmp_bit, 1, 1, SiFile);
			temp_bit[k] = tmp_bit;
			if (k > 4) {
				if ((temp_bit[k - 3] == 0x5F) & (temp_bit[k - 2] == 0x44) & (temp_bit[k - 1] == 0x45) & (temp_bit[k] == 0x54)) {
					break;
				}//if
			}//if
			k++;
		}//while
		events++;
		if (EndEvent != 0) {
			if (events == EndEvent)
				break;
		}
		if (feof(SiFile) || ferror(SiFile))
			break;
	}
	printf("all events=%d \n", events);
	return events;
}
/*
void Read2() {
	unsigned char temp_bit[5000];
	unsigned char tmp_bit;
	//Int_t events;
	Int_t tmp;
	Int_t ADC[2];
	Int_t TelPointer = 0;
	Int_t start_event;
	Int_t all_event;
	Int_t k;

	Int_t ClustSizeX;
	Int_t ClustSizeY;
	ClustSizeX = 3;
	ClustSizeY = 3;

	Bool_t* SeedX = new Bool_t[ADCXSize];
	Bool_t* SeedY = new Bool_t[ADCYSize];

	TH2F* hlego1 = new TH2F("hlego1", "Option1", 640, 0, 640, 640, 0, 640);
	TH2F* hlego2 = new TH2F("hlego2", "Option2", 640, 0, 640, 640, 0, 640);
	TH2F* hlego3 = new TH2F("hlego3", "Option3", 640, 0, 640, 640, 0, 640);

	fCanv = fEcanvas->GetCanvas();
	fCanv->Divide(3, 1);

	Variable();
	ReadNumFile();

	//
	start_event = 1000;
	while (1) {
		OpenFile();
		all_event = FindEvent(0);
		CloseFile();
		start_event = 1000;
		//start_event = +1000;
		//if(start_event > all_event) break;
		//for(Int_t i = 1; i<1000; i++ ){
		//	if (all_event > i * 1000) start_event =i * 1000;
		//	if (all_event + 1000 < all_event ) break;
		//}

		OpenFile();
		//Int_t k = FindEvent(start_event);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		while (1) {
			k = 0;
			while (1) {
				if (feof(SiFile) || ferror(SiFile))
					break;
				fread(&tmp_bit, 1, 1, SiFile);
				temp_bit[k] = tmp_bit;
				//printf("i=%d, tmp= 0x%x \n", k, temp_bit[k]);
				if (k > 4) {
					if ((temp_bit[k - 3] == 0xAA) & (temp_bit[k - 2] == 0xBB) & (temp_bit[k - 1] == 0xCC) & (temp_bit[k] == 0xD0))
						break;
				}//if
				k++;
			}//while
			TelPointer = 0;
			//if (tmp == 0xAABBCCD0){
			//printf("events = %d ", events);
			fread(&tmp, 4, 1, SiFile);
			tmp = swap4(tmp);
			//printf("Trigger time: %d s ", tmp);
			fread(&tmp, 4, 1, SiFile);
			tmp = swap4(tmp);
			//printf(" %d us\n", tmp);

			//}
			for (Int_t i = 0; i < ADCXSize; i++)
				XADC[i] = 0;
			for (Int_t i = 0; i < ADCYSize; i++)
				YADC[i] = 0;

			for (Int_t i = 0; i < EventLength; i++) {
				if (feof(SiFile) || ferror(SiFile))
					break;
				fread(&tmp, 4, 1, SiFile);
				tmp = swap4(tmp);

				if (tmp == 0xAABBCCDD)
					break;
				if (tmp == 0xAABBCC01) {
					TelPointer = 1;
					continue;
				}
				//end of second telescope
				if (tmp == 0xAABBCC03) {
					TelPointer = 2;
					continue;
				}
				ADC[0] = (tmp >> 12) & 0xfff;//chanel number
				ADC[1] = (tmp & 0xfff); //chanel data
				if (ADC[0] > 1277)
					continue;
				if (ADC[0] < glob->TELNumberOfStripsX[TelPointer]) {
					XADC[ADC[0] + ShiftX[TelPointer]] = ADC[1];
				}
				if (ADC[0] >= glob->TELNumberOfStripsX[TelPointer]) {
					YADC[ADC[0] - glob->TELNumberOfStripsX[TelPointer] + ShiftY[TelPointer]] = ADC[1];
				}
			}//for
			//FIXME
			for (Int_t k = 0; k < ADCXSize; k++)
				SeedX[k] = false;
			for (Int_t k = 0; k < ADCXSize; k++)
				SeedY[k] = false;
			for (Int_t n = 0; n < glob->TELNumberOfPlanes; n++) {
				for (Int_t x = ClustSizeX / 2; x <= glob->TELNumberOfStripsX[n] - ClustSizeX / 2; x++) {
					if (XADC[x + ShiftX[n]] > glob->TELSeedCutX[n]) {
						SeedX[x + ShiftX[n]] = true;
						for (Int_t tmp_pointer = -ClustSizeX / 2; tmp_pointer <= ClustSizeX / 2; tmp_pointer++) {
							if (XADC[x + ShiftX[n]] < XADC[x + tmp_pointer + ShiftX[n]]) {
								SeedX[x + ShiftX[n]] = false;
								continue;
							}
						}//tmp_pointerLenru
					}//if
				}//x
				for (Int_t y = ClustSizeY / 2; y <= glob->TELNumberOfStripsY[n] - ClustSizeY / 2; y++) {
					if (YADC[y + ShiftY[n]] > glob->TELSeedCutY[n]) {
						SeedY[y + ShiftY[n]] = true;
						for (Int_t tmp_pointer = -ClustSizeY / 2; tmp_pointer <= ClustSizeY / 2; tmp_pointer++) {
							if (YADC[y + ShiftY[n]] < YADC[y + tmp_pointer + ShiftY[n]]) {
								SeedY[y + ShiftY[n]] = false;
								continue;
							}
						}//tmp_pointer
					}//if
				}//y
				for (Int_t x = 0; x < glob->TELNumberOfStripsX[n]; x++) {
					if (SeedX[x + ShiftX[n]]) {
						for (Int_t y = 0; y < glob->TELNumberOfStripsY[n]; y++) {
							if (SeedY[y + ShiftY[n]]) {
								//if (i == 150) {
								//printf("stripX=%d, x=%d, striY=%d, y=%d,  \n",x + ShiftX[n], XADC[x + ShiftX[n]], y + ShiftY[n], YADC[y + ShiftX[n]]);
								if (n == 0)
									hlego1->Fill(x, y);
								if (n == 1)
									hlego2->Fill(x, y);
								if (n == 2)
									hlego3->Fill(x, y);
								//hlego2->Fill(x + ShiftX[n], y + ShiftY[n]);
								//}

							}//SeedY
						}//y
					}//SeedX
				}//x
			}

			//FIXME
			// set number of event
			//if (events == 1052) Lego();
			if (feof(SiFile) || ferror(SiFile))
				break;
		}
		//printf("All events = %d \n", events - 1);
		//sleep(3);
		//fclose(SiFile);

		fCanv->cd(1);
		gStyle->SetPalette(1);
		hlego1->Draw("COLZs");
		//sleep(3);

		fCanv->cd(2);
		gStyle->SetPalette(1);
		hlego3->Draw("COLZs");
		//sleep(3);

		fCanv->cd(3);
		gStyle->SetPalette(1);
		hlego2->Draw("COLZs");
		//fCanvas->Modified(1);
		fCanv->Update();
		hlego1->Reset();
		hlego2->Reset();
		hlego3->Reset();
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


		//ReadEvents_();
		CloseFile();
		sleep(3);

	}
}
*/
void ReadEvents() {

	unsigned char temp_bit[5000];
	unsigned char tmp_bit;
	Int_t events;
	Int_t start_event;
	Int_t tmp;
	Int_t ADC[2];
	Int_t TelPointer = 0;
	Int_t k;
	Int_t all_event;

	Bool_t* SeedX = new Bool_t[ADCXSize];
	Bool_t* SeedY = new Bool_t[ADCYSize];

	TH2F *hlego1 = new TH2F("hlego1", "Option1", 640, 0, 640, 640, 0, 640);
	TH2F *hlego2 = new TH2F("hlego2", "Option2", 640, 0, 640, 640, 0, 640);
	TH2F *hlego3 = new TH2F("hlego3", "Option3", 640, 0, 640, 640, 0, 640);

	fCanv = fEcanvas->GetCanvas();
	fCanv->Divide(3, 1);

	Int_t ClustSizeX;
	Int_t ClustSizeY;
	ClustSizeX = 3;
	ClustSizeY = 3;

	while (1) {
		ReadNumFile();
		OpenFile();
		all_event = FindEvent(0);
		fclose(SiFile);
		for (Int_t i = 1; i < 2000; i++) {
			if (all_event > i * 2000)
				start_event = i * 2000;
			if (all_event + 2000 < all_event)
				break;
		}

		OpenFile();
		FindEvent(start_event);

		events = 0;
		while (1) {
			k = 0;
			while (1) {
				if (feof(SiFile) || ferror(SiFile))
					break;
				fread(&tmp_bit, 1, 1, SiFile);
				temp_bit[k] = tmp_bit;
				if (k > 4) {
					if ((temp_bit[k - 3] == 0xAA) & (temp_bit[k - 2] == 0xBB) & (temp_bit[k - 1] == 0xCC) & (temp_bit[k] == 0xD0)) {
						break;
					}//if
				}//if
				k++;
			}//while
			TelPointer = 0;
			fread(&tmp, 4, 1, SiFile);
			tmp = swap4(tmp);
			fread(&tmp, 4, 1, SiFile);
			tmp = swap4(tmp);

			for (Int_t i = 0; i < ADCXSize; i++)
				XADC[i] = 0;
			for (Int_t i = 0; i < ADCYSize; i++)
				YADC[i] = 0;

			for (Int_t i = 0; i < EventLength; i++) {
				if (feof(SiFile) || ferror(SiFile))
					break;
				fread(&tmp, 4, 1, SiFile);
				tmp = swap4(tmp);

				if (tmp == 0xAABBCCDD)
					break;
				if (tmp == 0xAABBCC01) {
					TelPointer = 1;
					continue;
				}
				//end of second telescope
				if (tmp == 0xAABBCC03) {
					TelPointer = 2;
					continue;
				}
				ADC[0] = (tmp >> 12) & 0xfff;//chanel number
				ADC[1] = (tmp & 0xfff); //chanel data
				if (ADC[0] > 1277)
					continue;
				if (ADC[0] < glob->TELNumberOfStripsX[TelPointer]) {
					XADC[ADC[0] + ShiftX[TelPointer]] = ADC[1];
				}
				if (ADC[0] >= glob->TELNumberOfStripsX[TelPointer]) {
					YADC[ADC[0] - glob->TELNumberOfStripsX[TelPointer] + ShiftY[TelPointer]] = ADC[1];
				}
			}//for
			events++;

			//FIXME
			for (Int_t k = 0; k < ADCXSize; k++)
				SeedX[k] = false;
			for (Int_t k = 0; k < ADCXSize; k++)
				SeedY[k] = false;
			for (Int_t n = 0; n < glob->TELNumberOfPlanes; n++) {
				for (Int_t x = ClustSizeX / 2; x <= glob->TELNumberOfStripsX[n] - ClustSizeX / 2; x++) {
					if (XADC[x + ShiftX[n]] > glob->TELSeedCutX[n]) {
						SeedX[x + ShiftX[n]] = true;
						for (Int_t tmp_pointer = -ClustSizeX / 2; tmp_pointer <= ClustSizeX / 2; tmp_pointer++) {
							if (XADC[x + ShiftX[n]] < XADC[x + tmp_pointer + ShiftX[n]]) {
								SeedX[x + ShiftX[n]] = false;
								continue;
							}
						}//tmp_pointerLenru
					}//if
				}//x
				for (Int_t y = ClustSizeY / 2; y <= glob->TELNumberOfStripsY[n] - ClustSizeY / 2; y++) {
					if (YADC[y + ShiftY[n]] > glob->TELSeedCutY[n]) {
						SeedY[y + ShiftY[n]] = true;
						for (Int_t tmp_pointer = -ClustSizeY / 2; tmp_pointer <= ClustSizeY / 2; tmp_pointer++) {
							if (YADC[y + ShiftY[n]] < YADC[y + tmp_pointer + ShiftY[n]]) {
								SeedY[y + ShiftY[n]] = false;
								continue;
							}
						}//tmp_pointer
					}//if
				}//y
				for (Int_t x = 0; x < glob->TELNumberOfStripsX[n]; x++) {
					if (SeedX[x + ShiftX[n]]) {
						for (Int_t y = 0; y < glob->TELNumberOfStripsY[n]; y++) {
							if (SeedY[y + ShiftY[n]]) {
								if (n == 0)				hlego1->Fill(x, y);
								if (n == 1)				hlego2->Fill(x, y);
								if (n == 2)				hlego3->Fill(x, y);
							}//SeedY
						}//y
					}//SeedX
				}//x
			}
			if (feof(SiFile) || ferror(SiFile))
				break;
		}
		printf("All events = %d \n", events - 1);
		//sleep(3);
		fclose(SiFile);

		fCanv->cd(1);
		gStyle->SetPalette(1);
		hlego1->Draw("COLZs");
		//sleep(3);

		fCanv->cd(2);
		gStyle->SetPalette(1);
		hlego3->Draw("COLZs");
		//sleep(3);

		fCanv->cd(3);
		gStyle->SetPalette(1);
		hlego2->Draw("COLZs");
		//fCanvas->Modified(1);
		fCanv->Update();
		hlego1->Reset();
		hlego2->Reset();
		hlego3->Reset();
		//sleep(3);
	}
}
static void *thread_func(void *vptr_args) {
	Variable();
	ReadEvents();
	return NULL;
}
int main(int argc, char** argv) {
	TApplication* app = new TApplication("name", &argc, argv);
	pthread_t thread;

	fMain = new TGMainFrame(gClient->GetRoot(), 200, 20);
	// Create canvas widget
	fEcanvas = new TRootEmbeddedCanvas("Ecanvas", fMain, 1000, 300);
	fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10, 1));
	// Create a horizontal frame widget with buttons
	TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain, 200, 40);
	TGTextButton *draw = new TGTextButton(hframe, "&Draw");
	TGTextButton *exit = new TGTextButton(hframe, "&Exit", "gApplication->Terminate(0)");
	//draw->Connect("Clicked()", "Density_gui", this, "DoDraw()");
	hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
	hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX, 5, 5, 3, 4));
	fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX, 2, 2, 2, 2));
	// Set a name to the main frame
	fMain->SetWindowName("MVD Monitor");
	// Map all subwindows of main frame
	fMain->MapSubwindows();
	// Initialize the layout algorithm
	fMain->Resize(fMain->GetDefaultSize());
	// Map main frame
	fMain->MapWindow();

	if (pthread_create(&thread, NULL, thread_func, NULL) != 0) {
		return EXIT_FAILURE;
	}
	//fMain->Cleanup();
	app -> Run();
	if (pthread_join(thread, NULL) != 0) {
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
