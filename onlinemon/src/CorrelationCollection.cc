/*
 * CorrelationCollection.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: stanitz
 */

#include "CorrelationCollection.hh"
#include "OnlineMon.hh"

bool CorrelationCollection::isPlaneRegistered(SimpleStandardPlane p)
{
	std::vector<SimpleStandardPlane>::iterator it = std::find(_planes.begin(), _planes.end(), p);
	if (it == _planes.end()) { return false; } else { return true; }
}
void CorrelationCollection::fillHistograms(const SimpleStandardPlaneDouble &simpPlaneDouble)
{
	CorrelationHistos *corrmap = _map[simpPlaneDouble];
	if (corrmap == NULL)
	{
		//std::cout << "Histogram not registered" << std::endl;
	} else
	{
		SimpleStandardPlane planeA = simpPlaneDouble.getPlane1();
		SimpleStandardPlane planeB = simpPlaneDouble.getPlane2();

		for (int acluster = 0; acluster < planeA.getNClusters();acluster++)
		{
			SimpleStandardCluster oneAcluster = planeA.getCluster(acluster);
			if (oneAcluster.getNPixel() < 2) // we are only interested in clusters with several pixels
			{
				continue;
			}
			for (int bcluster = 0; bcluster < planeB.getNClusters();bcluster++)
			{
				SimpleStandardCluster oneBcluster = planeB.getCluster(bcluster);
				//
				if (oneBcluster.getNPixel() > 1)
				{
					corrmap->Fill(oneAcluster, oneBcluster);
				}
			}
		}
	}
}


void CorrelationCollection::setRootMonitor(RootMonitor *mon)  {_mon = mon; }

CorrelationHistos * CorrelationCollection::getCorrelationHistos(SimpleStandardPlaneDouble pd)
{

	return _map[pd];
}

void CorrelationCollection::Reset()
{
	std::map<SimpleStandardPlaneDouble, CorrelationHistos*>::iterator it;
	for (it = _map.begin(); it != _map.end(); it++)
	{
		(*it).second->Reset();
	}
}

void CorrelationCollection::Fill(SimpleEvent &simpev)
{
	int totalFills = 0;
	int nPlanes = simpev.getNPlanes();
	for (int planeA = 0; planeA < nPlanes; planeA++)
	{
		SimpleStandardPlane simpPlane = simpev.getPlane(planeA);
		//std::cout << "Checking Plane " << simpPlane.getName() << " " <<simpPlane.getID() << "..." <<  std::endl;
		if (!isPlaneRegistered(simpPlane))
		{
			//std::cout << "Plane " << simpPlane.getName() << " " <<simpPlane.getID() << "is not registered" << std::endl;
			if (correlateAllPlanes)
			{
				for (unsigned int oldPlanes = 0 ; oldPlanes < _planes.size(); oldPlanes++) {
					registerPlaneCorrelations(_planes.at(oldPlanes), simpPlane); // Correlating this plane with all the other ones
				}
			}
			if (_planes.size() > 0 && !correlateAllPlanes) registerPlaneCorrelations(_planes.at(_planes.size()-1),simpPlane);
			_planes.push_back(simpPlane);
		}
		for (int planeB = planeA +1; planeB < nPlanes; planeB++)
		{

			SimpleStandardPlaneDouble db(simpev.getPlane(planeA), simpev.getPlane(planeB));
			fillHistograms(db);
			/*
			CorrelationHistos *corrmap = _map[db];
			if (corrmap != NULL) {
				totalFills += corrmap->getFills();
				corrmap->resetFills();
			}*/
		}
	}
	//std::cout << "Fills: " << totalFills << std::endl;
	totalFills = 0;
}


void CorrelationCollection::registerPlaneCorrelations(const SimpleStandardPlane &p1, const SimpleStandardPlane &p2)
{
	std::cout << "Correlating: " << p1.getName() << " " <<p1.getID() << " with " <<  p2.getName() << " " <<p2.getID() << std::endl;
	CorrelationHistos *tmphisto = new CorrelationHistos(p1,p2);
	SimpleStandardPlaneDouble planeDouble(p1,p2);
	_map[planeDouble] = tmphisto;
	if (_mon != NULL)
	{
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

	if (file==NULL) // if the file pointer is null jump back
	{
		cout << "Can't Write Correllation Collections " <<endl;
		return;
	}
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
