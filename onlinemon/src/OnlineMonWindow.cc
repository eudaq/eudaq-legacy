#include "OnlineMonWindow.hh"
#include <iostream>
#include <TH2F.h>
#include <TCanvas.h>
#include <TGPicture.h>
#include <TApplication.h>



using namespace std;



OnlineMonWindow::OnlineMonWindow(const TGWindow* p, UInt_t w, UInt_t h) 
: TGMainFrame(p,w,h,kVerticalFrame), _eventnum(0), _runnum(0) {
	cout << "Opening Window" << endl;
	
	Hfrm_windows = new TGHorizontalFrame(this);
		Hfrm_left = new TGVerticalFrame(Hfrm_windows);
			int i=1;
			tb = new TGToolBar(Hfrm_left,180,80);
			ToolBarData_t icondata;
		
			/*------*/
			icondata.fPixmap = "../onlinemon/icons/save_btn.xpm";
			icondata.fTipText = "Save histograms to rootfile";
			icondata.fStayDown = kFALSE;
			icondata.fId = i;
			i++;
			save = tb->AddButton(Hfrm_left, &icondata,0);
			save->Connect("Clicked()","OnlineMonWindow",this, "Write()");
	
			/*------*/
			icondata.fPixmap = "../onlinemon/icons/delete_btn.xpm";
			icondata.fTipText = "Reset all Histograms";
			icondata.fStayDown = kFALSE;
			icondata.fId = i;
			i++;
			reset = tb->AddButton(Hfrm_left, &icondata,5);
			reset->Connect("Clicked()","OnlineMonWindow",this, "Reset()");
			
			/*------*/
			icondata.fPixmap = "../onlinemon/icons/autodel_btn.xpm";
			icondata.fTipText = "Autoreset Histograms on StopRun";
			icondata.fStayDown = kTRUE;
			icondata.fId = i;
			i++;
			autoreset = tb->AddButton(Hfrm_left, &icondata,0);
			autoreset->Connect("Clicked()","OnlineMonWindow",this, "AutoReset()");
			
			/*------*/
			nen_reduce = new TGNumberEntry(Hfrm_left,1,3,-1,TGNumberFormat::kNESInteger, TGNumberFormat::kNEAPositive);
			tb->AddFrame(nen_reduce,new TGLayoutHints(kLHintsTop| kLHintsLeft,17,0,7,2));
			nen_reduce->Connect("ValueSet(Long_t)","OnlineMonWindow",this,"ChangeReduce(Long_t)");
			
			
			/*------*/
			icondata.fPixmap = "../onlinemon/icons/quit_btn.xpm";
			icondata.fTipText = "Quit";
			icondata.fStayDown = kFALSE;
			icondata.fId = i;
			i++;
			quit = tb->AddButton(Hfrm_left, &icondata,10);
			quit->Connect("Clicked()","OnlineMonWindow",this, "Quit()");
		
			Cvs_left = new TGCanvas(Hfrm_left,200,h);
		TGViewPort *vp= Cvs_left->GetViewPort();
		
		LTr_left = new TGListTree(Cvs_left, kHorizontalFrame);
			LTr_left->Connect("Clicked(TGListTreeItem*, Int_t)","OnlineMonWindow", this, "actor(TGListTreeItem*, Int_t)");
			LTr_left->Connect("Clicked(TGListTreeItem*, Int_t, Int_t, Int_t)","OnlineMonWindow", this, "actorMenu(TGListTreeItem*, Int_t, Int_t, Int_t)");
		

			
			vp->AddFrame(LTr_left,new TGLayoutHints(kLHintsExpandY| kLHintsExpandY,5,5,5,5));
			//LTr_left->MapSubwindows();
			Hfrm_left->AddFrame(tb,new TGLayoutHints(kLHintsExpandX,5,5,5,5));
			Hfrm_left->AddFrame(Cvs_left,new TGLayoutHints(kLHintsExpandX |  kLHintsExpandY,5,5,5,5));
			//Hfrm_left->AddFrame(LTr_left,new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,5,5));
		Hfrm_right = new TGVerticalFrame(Hfrm_windows);
			ECvs_right = new TRootEmbeddedCanvas("Canvas", Hfrm_right);
			Hfrm_right->AddFrame(ECvs_right, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,5,5,5,5));
		
		Hfrm_windows->AddFrame(Hfrm_left, new TGLayoutHints( kLHintsExpandY,2,2,2,2));
		Hfrm_windows->AddFrame(Hfrm_right, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY,2,2,2,2));
	AddFrame(Hfrm_windows, new TGLayoutHints(kLHintsExpandY | kLHintsExpandX | kLHintsLeft,0,0,0,0));	
		fStatusBar = new TGStatusBar(this,510,10,kHorizontalFrame);
		int parts[] = {33,33,34};
		fStatusBar->SetParts(parts,3);
		fStatusBar->SetText("IDLE",0);
		fStatusBar->SetText("run: 0",1);
		fStatusBar->SetText("event: ",2);
		
	AddFrame(fStatusBar, new TGLayoutHints(kLHintsBottom | kLHintsExpandX, 0,0,2,0));
	
			
	timer = new TTimer();
	timer->Connect("Timeout()","OnlineMonWindow",this,"autoUpdate()");
	timer->Start(1000,kFALSE);
	_reduceUpdate = 0;
	
	
	h1 = new TH2F("h1","hu h1",11,0,10,11,0,10);
	h2 = new TH2F("h2","hu h2",11,0,10,11,0,10);
	//a->Draw();
	SetWindowName("ATLAS Pixels Eudet-Online-Monitor");
	MapSubwindows();
	//Resize(GetDefaultSize());
	Resize(w,h);
        Layout();
        MapSubwindows();
	MapWindow();
	
	CtxMenu = new TContextMenu("","");
	
	
}

void OnlineMonWindow::Write() {
	cout << "Pressed Write!" << endl;
	TFile *f = new TFile(_rootfilename.c_str(),"RECREATE");
	for (int i = 0 ; i < _colls.size(); ++i) {
		_colls.at(i)->Write(f);
	}
	f->Close();
}

void OnlineMonWindow::Reset() {
	for (int i = 0 ; i < _colls.size(); ++i) {
		_colls.at(i)->Reset();
	}
}

void OnlineMonWindow::AutoReset() {
	_autoreset = autoreset->IsOn();
	//cout << "AutoReset " << status << endl;
}

void OnlineMonWindow::Quit() {
	gApplication->Terminate(0);
}

void OnlineMonWindow::registerTreeItem(std::string item) {
	//rootitem?
	int f = item.find("/");
	if (f == string::npos ) { //Yes
		_treeMap[item] = LTr_left->AddItem(NULL,item.c_str());
		_treeBackMap[_treeMap[item] ] = item;
		//cout << "Item (Root): " << item << endl;
	} else { //No
		char post[64] ="";
		char pre[64] ="";
		item.copy(pre,item.find_last_of("/"),0);
		item.copy(post,item.length()-item.find_last_of("/"),item.find_last_of("/")+1);
		if (_treeMap[string(pre)] == NULL) registerTreeItem(string(pre));
		_treeMap[item] = LTr_left->AddItem(_treeMap[string(pre)],post);
		_treeBackMap[_treeMap[item] ] = item;
		//cout << "Item: PRE: " << pre << " POST: " << post << endl;
	}
        Cvs_left->MapSubwindows();
        Cvs_left->MapWindow();
}

void OnlineMonWindow::makeTreeItemSummary(std::string item) {
		
	std::map<std::string, TH1*>::iterator it;
	std::vector<std::string> v;
	for (it = _hitmapMap.begin(); it != _hitmapMap.end(); it++) {
		std::string c = std::string(it->first,0,item.length());
		if (c == item) {
			//cout << "c is: " << c << " compared to "<< item <<endl;
			v.push_back(it->first);
		}
	}
	//cout << "Setting up summary map for item " << item << endl;
		_summaryMap[item] = v;
}

void OnlineMonWindow::addTreeItemSummary(std::string item, std::string histoitem) {
		
	std::vector<std::string> v;
	std::map<std::string,std::string>::iterator it;
	if (_summaryMap.find(item) != _summaryMap.end()) {
		v = _summaryMap[item];
	} 
	v.push_back(histoitem);
	_summaryMap[item] = v;
}

void OnlineMonWindow::registerHisto(std::string tree,TH1* h, std::string op, const unsigned int l) {
	_hitmapMap[tree] = h;
	_hitmapOptions[tree]= op;
	_logScaleMap[tree] = l;
	if (op == "") {
		const TGPicture *thp = gClient->GetPicture("h2_t.xpm");
		_treeMap[tree]->SetPictures(thp,thp);
	} else {
		const TGPicture *thp = gClient->GetPicture("h3_t.xpm");
		_treeMap[tree]->SetPictures(thp,thp);
	}

}

void OnlineMonWindow::autoUpdate() {
	//cout << "Updateing window " << _reduceUpdate << endl;
	_reduceUpdate++;
	if (_reduceUpdate > _activeHistos.size()) {
		
		if (_activeHistos.size() !=0) { //&&_hitmapMap[_activeHisto]!=NULL) {
			TCanvas *fCanvas = ECvs_right->GetCanvas();
			if (_activeHistos.size() == 1)fCanvas->cd();
			for (int i = 0; i< _activeHistos.size();++i) {
				if (_activeHistos.size() > 1) fCanvas->cd(i+1);
				if (_hitmapMap[_activeHistos.at(i)]!=NULL) {
					_hitmapMap[_activeHistos.at(i)]->Draw(_hitmapOptions[_activeHistos.at(i)].c_str());
					
				}
				if (_activeHistos.size() > 1) {
					
					fCanvas->GetPad(i+1)->Modified();
					//fCanvas->GetPad(i+1)->Update();
				}
			}
			
			//fCanvas->Modified();
			fCanvas->Update();
		}
		UpdateEventNumber(_eventnum);
		UpdateRunNumber(_runnum);
		
		MapSubwindows();
		MapWindow();
		
		_reduceUpdate = 0;
	}
	
	//cout << "...updated" << endl;
	
	
}

void OnlineMonWindow::ChangeReduce(Long_t num) {
	
	_reduce = nen_reduce->GetNumber();
	for (int i = 0 ; i < _colls.size(); ++i) {
		_colls.at(i)->setReduce(_reduce);
	}
}

OnlineMonWindow::~OnlineMonWindow() {gApplication->Terminate(0); }
/*
void registerSensor(std::string name, int id) {

	

}
*/

void OnlineMonWindow::actorMenu(TGListTreeItem* item, Int_t btn, Int_t x, Int_t y) {
cout << "actining menu" << btn << " " << x << " " << y << endl;	
	if (btn == 3) {
		cout << "hit right mouse button" << endl;
		CtxMenu->Popup(x,y,this);
	}
}



void OnlineMonWindow::actor(TGListTreeItem* item, Int_t btn) {
	
	//cout << "Here we are acting " << btn << endl;
	
	
	TCanvas *fCanvas = ECvs_right->GetCanvas();
	//fCanvas->cd();
	//fCanvas->Divide(1,1);
	fCanvas->Clear();
	std::string tree = _treeBackMap[item];
	
	 fCanvas->SetLogx(bool(_logScaleMap[tree] & kLogX)); 
	 fCanvas->SetLogy(bool(_logScaleMap[tree] & kLogY)); 
	 fCanvas->SetLogz(bool(_logScaleMap[tree] & kLogZ)); 
	
	
	
	_activeHistos.clear();
	cout << "String is: " << tree<< endl;
	
	if (_hitmapMap[tree] != NULL) {
		
		_hitmapMap[tree]->Draw(_hitmapOptions[tree].c_str());
		
		_activeHistos.push_back(tree);
		
	}
	if (_summaryMap.find(tree)!= _summaryMap.end()) {
		cout <<"Here we are" << endl;
		std::vector<std::string> v = _summaryMap[tree];
		size_t s = v.size();
		int d1=1;
		int d2=1;
		if (s ==2) d2=2;
		if(s > 2) {d1 = 2; d2 = 2;}
		if(s>4) {d1=3;d2=2;}
		if(s>6) {d1=3;d2=3;}
		if(s>9) {d1=4;d2=3;}
		if(s>12) {d1=4;d2=4;}
		if(s>16) {d1=5;d2=4;}
		if(s>20) {d1=5;d2=5;}
		if(s>25){d1=6;d2=5;}
		fCanvas->Divide(d2,d1);
		
		for (int i = 0; i < s; ++i) {
			fCanvas->cd(i+1);
			_activeHistos.push_back(v.at(i));
			_hitmapMap[v.at(i)]->Draw(_hitmapOptions[v.at(i)].c_str());
		}
		
	}
	
	fCanvas->Update();
	
}

void OnlineMonWindow::registerPlane(char * sensor, int id) {
	cout << "Plane " << sensor << " " << id << "should be registered" << endl;
	
}

void OnlineMonWindow::UpdateRunNumber(const int num) {
	char out[64];
	sprintf(out,"run: %u", num);
	fStatusBar->SetText(out,1);
	
}

void OnlineMonWindow::UpdateEventNumber(const int event) {
	char out[64]="";
	
	sprintf(out,"event: %u", event);
	fStatusBar->SetText(out,2);
	
	//cout << out << endl;
}

void OnlineMonWindow::setReduce(const unsigned int red) { 
	_reduce = red; 
	nen_reduce->SetNumber(red);
}

void OnlineMonWindow::UpdateStatus(const std::string status) {
	fStatusBar->SetText(status.c_str(),0);
}

void OnlineMonWindow::setAutoReset(bool reset) {
	_autoreset = reset; 
	autoreset->SetOn(_autoreset);
}

void OnlineMonWindow::setUpdate(const unsigned int up) {
	
	timer->Stop();
	timer->Start(up,kFALSE);
}



/*
virtual OnlineMonWindow::~OnlineMonWindow() {
	
}*/
