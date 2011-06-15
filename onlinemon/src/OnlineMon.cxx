// ROOT includes
#include "TROOT.h"
#include "TNamed.h"
#include "TApplication.h"
#include "TGClient.h"
#include "TGMenu.h"
#include "TGTab.h"
#include "TGButton.h"
#include "TGComboBox.h"
#include "TGLabel.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TGComboBox.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TRootEmbeddedCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TPaletteAxis.h"
#include "TThread.h"
#include "TFile.h"
#include "TColor.h"
#include "TString.h"
#include "TF1.h"
//#include "TSystem.h" // for TProcessEventTimer
// C++ INCLUDES
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cstring>
#include <stdio.h>
#include <string.h>


//ONLINE MONITOR Includes
#include "OnlineMon.hh"

using namespace std;

RootMonitor::RootMonitor(const std::string & runcontrol, const std::string & datafile, int x, int y, int w, int h, int argc, int offline)
	: eudaq::Holder<int>(argc), eudaq::Monitor("OnlineMon", runcontrol, datafile), _offline(offline){

	if (_offline <= 0) onlinemon = new OnlineMonWindow(gClient->GetRoot(),800,600);
	//onlinemon->setRootMonitor(this);
	HitmapCollection *hmCollection = new HitmapCollection();
	
	

	CorrelationCollection *corrCollection = new CorrelationCollection();
	

	_colls.push_back(hmCollection);
	_colls.push_back(corrCollection);
	if (_offline <= 0) {
		hmCollection->setRootMonitor(this);
		corrCollection->setRootMonitor(this);
		onlinemon->setCollections(_colls);
	}
	cout << "Datafile: " << datafile << endl;
	if (datafile != "") {
		cout << "Calling program from file" << endl;
		size_t first = datafile.find_last_of("/")+1;
		size_t last = datafile.find_last_of(".raw") - first - 3;
		char out[64] = "";
		char num[64] = "";
		datafile.copy(out,last,first);
		datafile.copy(num,last-3,first+3);
		int n;
		sscanf(num,"%d",&n);
		if (_offline <= 0) onlinemon->setRunNumber(n);
		strcat(out,".root");
		cout << "Name is: " << out << endl;
		if (_offline <= 0) onlinemon->setRootFileName(out);
		rootfilename = out;
		if (_offline > 0) {
			_checkEOF.setCollections(_colls);
			_checkEOF.setRootFileName(out);
			_checkEOF.startChecking(10000);
		}
	}
	
	cout << "End of Constructor" << endl;
	//hmCollection->Connect("PlaneRegistered(char*,int)","OnlineMonWindow", onlinemon,"registerPlane(char*,int)");

	//set a few defaults
	snapshotdir="../snapshots/";

	onlinemon->SetOnlineMon(this);
}



void RootMonitor::setReduce(const unsigned int red) {
	if (_offline <= 0) onlinemon->setReduce(red);
	for (unsigned int i = 0 ; i < _colls.size(); ++i) {
			_colls.at(i)->setReduce(red);
		}
}

void RootMonitor::OnEvent(const eudaq::StandardEvent & ev) {
	  //cout << "called onEvent " << ev.GetEventNumber()<< endl;
	_checkEOF.EventReceived();
	bool reduce;
	if (_offline > 0) {
		if (_offline < ev.GetEventNumber()) { //FIXME
			TFile *f = new TFile(rootfilename.c_str(),"RECREATE");
			for (unsigned int i = 0 ; i < _colls.size(); ++i) {
				_colls.at(i)->Write(f);
			}
			f->Close();    
			exit(0); // Kill the program
		}
		reduce = true;
		
	} else {
		reduce = (ev.GetEventNumber() % onlinemon->getReduce() == 0);
	}
	if (reduce) {
		int num = ev.NumPlanes();
		  SimpleEvent simpEv;
		if (ev.GetEventNumber() == 1) onlinemon->UpdateStatus("Getting data..");
		//cout << "GetNum: " << ev.NumPlanes() << endl;		
		  for (int i = 0; i < num;i++) {
			const eudaq::StandardPlane & plane = ev.GetPlane(i);
			if (strcmp(plane.Sensor().c_str(), "FORTIS") != 0 ) {
				 //int tmpId = plane.ID();
				//if ( plane.Sensor() == "MIMOSA26" && i > 2) tmpId += 3;
				 SimpleStandardPlane simpPlane(plane.Sensor(),plane.ID(),plane.XSize(),plane.YSize());
				 for (unsigned int lvl1 = 0; lvl1 < plane.NumFrames(); lvl1++) {
					 // if (lvl1 > 2 && plane.HitPixels(lvl1) > 0) std::cout << "LVLHits: " << lvl1 << ": " << plane.HitPixels(lvl1) << std::endl;
					  for (unsigned int index = 0; index < plane.HitPixels(lvl1);index++) {
						  SimpleStandardHit hit((int)plane.GetX(index,lvl1),(int)plane.GetY(index,lvl1));
						  hit.setTOT((int)plane.GetPixel(index,lvl1));
						  hit.setLVL1(lvl1);
						  simpPlane.addHit(hit);
					  
					  }
				 }
				 simpEv.addPlane(simpPlane);
				//cout << "Type: " << plane.Type() << endl;
				//cout << "StandardPlane: "<< plane.Sensor() <<  " " << plane.ID() << " " << plane.XSize() << " " << plane.YSize() << endl;
				//cout << "PlaneAddress: " << &plane << endl;
			}
		  }
		 
		simpEv.doClustering();
		if (ev.GetEventNumber() < 1) {
			cout << "Waiting for booking of Histograms..." << endl;
			sleep(1);
			cout << "...long enough"<< endl;
		}
		
		for (unsigned int i = 0 ; i < _colls.size(); ++i) {
			_colls.at(i)->Fill(simpEv);
			_colls.at(i)->Calculate(ev.GetEventNumber());
		}
		
		if (_offline <= 0) {
			onlinemon->setEventNumber(ev.GetEventNumber());
			onlinemon->increaseAnalysedEventsCounter();
		}
	}
	  if (ev.IsBORE()) {
		std::cout << "This is a BORE" << std::endl;

	//std::cout << "end of OnEvent" << std::endl;		  
		 
   
    } 
    
    }
    
void RootMonitor::autoReset(const bool reset) { 
	//_autoReset = reset; 
	if (_offline <= 0) onlinemon->setAutoReset(reset);
	
}

void RootMonitor::OnStopRun() {
    	if (_writeRoot) {
		TFile *f = new TFile(rootfilename.c_str(),"RECREATE");
		for (unsigned int i = 0 ; i < _colls.size(); ++i) {
		_colls.at(i)->Write(f);
		}
		f->Close();    
	}
	
	if (onlinemon->getAutoReset()) {
		onlinemon->UpdateStatus("Resetting..");
		for (unsigned int i = 0 ; i < _colls.size(); ++i) {
		_colls.at(i)->Reset();
		}
	}
	
	
	/*TFile *file = new TFile(rootfilename.c_str(),"RECREATE");
    	    char run[64];
    	    sprintf(run,"%d",runnumber);
    	    gDirectory->mkdir(run);
    	    hmCollection->Write(file);
    	    corrCollection->Write(file);
    	    gDirectory->cd("..");
    	    */
	onlinemon->UpdateStatus("Run stopped");
}

void RootMonitor::OnStartRun(unsigned param) {
		Monitor::OnStartRun(param);
		std::cout << "Called on start run" << param <<std::endl;
		onlinemon->UpdateStatus("Starting run..");
		char out[255];
		sprintf(out, "run%d.root",param);
		rootfilename = std::string(out);
		runnumber = param;

		if (_offline <= 0) {
			onlinemon->setRunNumber(runnumber);
			onlinemon->setRootFileName(rootfilename);
		}

		SetStatus(eudaq::Status::LVL_OK);
}

void RootMonitor::setUpdate(const unsigned int up) {
	if (_offline <= 0) onlinemon->setUpdate(up);
}


//sets the location for the snapshots
void RootMonitor::SetSnapShotDir(string s)
{
	snapshotdir=s;
}


//gets the location for the snapshots
string RootMonitor::GetSnapShotDir()
{
	return snapshotdir;
}



int main(int argc, const char ** argv) {
	eudaq::OptionParser op("EUDAQ Root Monitor", "1.0", "A Monitor using root for gui and graphics");
	eudaq::Option<std::string> rctrl(op, "r", "runcontrol", "tcp://localhost:44000", "address",
		"The address of the RunControl application");
	eudaq::Option<std::string> level(op, "l", "log-level", "NONE", "level",
		"The minimum level for displaying log messages locally");
	eudaq::Option<std::string> file (op, "f", "data-file", "", "filename",
		"A data file to load - setting this changes the default"
		" run control address to 'null://'");
	eudaq::Option<int>             x(op, "x", "left",    100, "pos");
	eudaq::Option<int>             y(op, "y", "top",       0, "pos");
	eudaq::Option<int>             w(op, "w", "width",  1400, "pos");
	eudaq::Option<int>             h(op, "g", "height",  700, "pos", "The initial position of the window");
	eudaq::Option<int>             reduce(op, "rd", "reduce",  1, "Reduce the number of events");
	eudaq::Option<int>             update(op, "u", "update",  1000, "update every ms");
	eudaq::Option<int>             offline(op, "o", "offline",  0, "running is offlinemode - analyse until event <num>");
	eudaq::OptionFlag do_rootatend (op, "rf","root","Write out root-file after each run");
	eudaq::OptionFlag do_resetatend (op, "rs","reset","Reset Histograms when run stops");

try {
	op.Parse(argv);
	EUDAQ_LOG_LEVEL(level.Value());
	if (file.IsSet() && !rctrl.IsSet()) rctrl.SetValue("null://");
	gROOT->Reset();
	gROOT->SetStyle("Plain");
	gStyle->SetPalette(1);
	gStyle->SetOptStat(0100);
	gStyle->SetStatH(0.03);
	
	
// start the GUI
	TApplication theApp("App", &argc, const_cast<char**>(argv),0,0);
	RootMonitor mon(rctrl.Value(), file.Value(), x.Value(), y.Value(), w.Value(), h.Value(), argc, offline.Value());
	mon.setWriteRoot(do_rootatend.IsSet());
	mon.autoReset(do_resetatend.IsSet());
	mon.setReduce(reduce.Value());
	mon.setUpdate(update.Value());
	//mon.Run();

	theApp.Run(); //execute
} catch (...) {
		return op.HandleMainException();
}
return 0;
}





