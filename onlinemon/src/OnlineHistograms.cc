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
		_mon->getOnlineMon()->registerHisto(tree,getCorrelationHistos(planeDouble)->getCorrXHisto(), "COLZ",0);
		
		sprintf(tree,"Correlation/%s %i/%s %i in Y",p1.getName().c_str(),p1.getID(),p2.getName().c_str(),p2.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getCorrelationHistos(planeDouble)->getCorrYHisto(), "COLZ",0);
		
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
		cout << "Monitor running in online-mode" << endl;
		char tree[64], folder[64];
		sprintf(tree,"%s/Sensor %i/RawHitmap",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getHitmapHisto(), "COLZ",0);
	
	    cout << "DEBUG "<< p.getName().c_str() <<endl;
		sprintf(folder,"%s",p.getName().c_str());
		cout << "DEBUG "<< folder << " "<<tree<<  endl;
		_mon->getOnlineMon()->addTreeItemSummary(folder,tree);
	
	
		sprintf(tree,"%s/Sensor %i/Clustermap",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getClusterMapHisto(), "COLZ",0);
		
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

		sprintf(tree,"%s/Sensor %i/Clustersize",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getClusterSizeHisto());

		sprintf(tree,"%s/Sensor %i/NumHits",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getNHitsHisto());
		
		sprintf(tree,"%s/Sensor %i/NumClusters",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getNClustersHisto());

		sprintf(tree,"%s/Sensor %i/HitOcc",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getHitmapHistos(p.getName(),p.getID())->getHitOccHisto(), "",kLogX | kLogY);
		
		sprintf(tree,"%s/Sensor %i",p.getName().c_str(),p.getID());
		_mon->getOnlineMon()->makeTreeItemSummary(tree);

		if (p.getName() == std::string("MIMOSA26"))
		{
			char mytree[2][64];// holds the number of histogramms for each section, hardcoded, will be moved to vectors
			TH1I * myhistos[2];
//loop over all sections
			for (unsigned int section =0; section<mimosa26_max_section; section ++ )
			{
				sprintf(mytree[0],"%s/Sensor %i/Section %i/NumHits",p.getName().c_str(),p.getID(),section);
				sprintf(mytree[1],"%s/Sensor %i/Section %i/NumClusters",p.getName().c_str(),p.getID(),section);
				myhistos[0]=getHitmapHistos(p.getName(),p.getID())->getSectionsNHitsHisto(section);
				myhistos[1]=getHitmapHistos(p.getName(),p.getID())->getSectionsNClusterHisto(section);
//loop over all histograms
				for (int nhistos=0; nhistos<2; nhistos++)
				{
					if (myhistos[nhistos]==NULL)
					{
						cout << section << " " << "is null" << endl;
					}
					else
					{
						_mon->getOnlineMon()->registerTreeItem(mytree[nhistos]);
						_mon->getOnlineMon()->registerHisto(mytree[nhistos],myhistos[nhistos]);
					}
				}


				sprintf(tree,"%s/Sensor %i/Section %i",p.getName().c_str(),p.getID(),section);
				_mon->getOnlineMon()->makeTreeItemSummary(tree); //make summary page
			}
		}

	}
}
