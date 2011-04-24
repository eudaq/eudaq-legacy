#include "OnlineHistograms.hh"
#include <TDirectory.h>
#include "OnlineMon.hh"

using namespace std;

void CorrelationCollection::registerPlaneCorrelations(const SimpleStandardPlane &p1, const SimpleStandardPlane &p2) {
	std::cout << "Correlating: " << p1.getName() << " " <<p1.getID() << " with " <<  p2.getName() << " " <<p2.getID() << std::endl;
	CorrelationHistos *tmphisto = new CorrelationHistos(p1,p2);
	SimpleStandardPlaneDouble planeDouble(p1,p2);
	_map[planeDouble] = tmphisto;
	if (_mon != NULL) {
		char tree[64];
		sprintf(tree,"Correlation/%s %i/%s %i in X",p1.getName().c_str(),p1.getID(),p2.getName().c_str(),p2.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getCorrelationHistos(planeDouble)->getCorrXHisto(), "COLZ",kLogZ);
		
		sprintf(tree,"Correlation/%s %i/%s %i in Y",p1.getName().c_str(),p1.getID(),p2.getName().c_str(),p2.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getCorrelationHistos(planeDouble)->getCorrYHisto(), "COLZ",kLogZ);
		
		sprintf(tree,"Correlation/%s %i",p1.getName().c_str(),p1.getID());
		_mon->getOnlineMon()->makeTreeItemSummary(tree);
	}
}

void CorrelationCollection::Write(TFile *file) {
	gDirectory->mkdir("Correlations");
	gDirectory->cd("Correlations");
	std::map<SimpleStandardPlaneDouble, CorrelationHistos*>::iterator it;
	
	for (it = _map.begin(); it != _map.end(); it++) {
		//char sensorfolder[255]="";
		//sprintf(sensorfolder,"%s_%d:%s_%d",it->first.getPlane1().getName().c_str(),it->first.getPlane1().getID(), it->first.getPlane2().getName().c_str(),it->first.getPlane2().getID());
		//gDirectory->mkdir(sensorfolder);
		//gDirectory->cd(sensorfolder);
		it->second->Write();
		
		
		//gDirectory->cd("..");
	}
	gDirectory->cd("..");
}

void HitmapCollection::Write(TFile *file) {
	gDirectory->mkdir("Hitmaps");
	gDirectory->cd("Hitmaps");
	
	
	std::map<SimpleStandardPlane,HitmapHistos*>::iterator it;
	for (it = _map.begin(); it != _map.end(); it++) {
		
		char sensorfolder[255] = "";
		sprintf(sensorfolder,"%s_%d",it->first.getName().c_str(), it->first.getID());
		cout << "Making new subfolder " << sensorfolder << endl;
		gDirectory->mkdir(sensorfolder);
		gDirectory->cd(sensorfolder);
		it->second->Write();
		
		//gDirectory->ls();
		gDirectory->cd("..");
	}
	gDirectory->cd("..");
}

void HitmapCollection::registerPlane(const SimpleStandardPlane &p) {
	HitmapHistos *tmphisto = new HitmapHistos(p);
	_map[p] = tmphisto;
	//std::cout << "Registered Plane: " << p.getName() << " " << p.getID() << std::endl;
	//PlaneRegistered(p.getName(),p.getID());
	if (_mon != NULL) {
		cout << "running in online-mode" << endl;
		char tree[64], folder[64];
		sprintf(tree,"%s/%i/RawHitmap",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getHitmapHisto(), "COLZ",kLogZ);
	
	
		sprintf(folder,"%s",p.getName().c_str());
		_mon->getOnlineMon()->addTreeItemSummary(folder,tree);
	
	
		sprintf(tree,"%s/%i/Clustermap",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getClusterMapHisto(), "COLZ",kLogZ);
		
		if (p.getName() == std::string("APIX") || p.getName() == std::string("USBPIX") || p.getName() == std::string("USBPIXI4")) {
			sprintf(tree,"%s/%i/LVL1Distr",p.getName().c_str(),p.getID());
			_mon->getOnlineMon()->registerTreeItem(tree);
			_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getLVL1Histo());
	
			sprintf(tree,"%s/%i/LVL1Cluster",p.getName().c_str(),p.getID());
			_mon->getOnlineMon()->registerTreeItem(tree);
			_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getLVL1ClusterHisto());
	
			sprintf(tree,"%s/%i/LVL1Width",p.getName().c_str(),p.getID());
			_mon->getOnlineMon()->registerTreeItem(tree);
			_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getLVL1WidthHisto());
		
			sprintf(tree,"%s/%i/SingleTOT",p.getName().c_str(),p.getID());
			_mon->getOnlineMon()->registerTreeItem(tree);
			_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getTOTSingleHisto());

			sprintf(tree,"%s/%i/ClusterTOT",p.getName().c_str(),p.getID());
			_mon->getOnlineMon()->registerTreeItem(tree);
			_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getTOTClusterHisto());
			
			sprintf(tree,"%s/%i/ClusterWidthX",p.getName().c_str(),p.getID());
			_mon->getOnlineMon()->registerTreeItem(tree);
			_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getClusterWidthXHisto());
			
			sprintf(tree,"%s/%i/ClusterWidthY",p.getName().c_str(),p.getID());
			_mon->getOnlineMon()->registerTreeItem(tree);
			_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getClusterWidthYHisto());
		

		}

		sprintf(tree,"%s/%i/Clustersize",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getClusterSizeHisto());

		sprintf(tree,"%s/%i/NumHits",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getNHitsHisto());
		
		sprintf(tree,"%s/%i/NumClusters",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getNClustersHisto());

		sprintf(tree,"%s/%i/HitOcc",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getHitOccHisto(), "",kLogX | kLogY);
		
		sprintf(tree,"%s/%i",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->makeTreeItemSummary(tree);
	
	}
}
