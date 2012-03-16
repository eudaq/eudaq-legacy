/*
 * CorrelationCollection.cc
 *
 *  Created on: Jun 17, 2011
 *      Author: stanitz
 */

#include "CorrelationCollection.hh"
#include "OnlineMon.hh"

bool CorrelationCollection::isPlaneRegistered(const SimpleStandardPlane p)
{
	std::vector<SimpleStandardPlane>::iterator it = std::find(_planes.begin(), _planes.end(), p);

	if (it == _planes.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}
void CorrelationCollection::fillHistograms(const SimpleStandardPlaneDouble &simpPlaneDouble)
{
	CorrelationHistos *corrmap = _mapOld[simpPlaneDouble];
	if (corrmap == NULL)// this hasn't been booked yet
	{

	}
	else
	{

		const std::vector<SimpleStandardCluster> aClusters=simpPlaneDouble.getPlane1().getClusters();
		const std::vector<SimpleStandardCluster> bClusters=simpPlaneDouble.getPlane2().getClusters();


		for (unsigned int acluster = 0; acluster < aClusters.size();acluster++)
		{
			const SimpleStandardCluster& oneAcluster = aClusters.at(acluster);
			if (oneAcluster.getNPixel() < _mon->mon_configdata.getCorrel_minclustersize()) // we are only interested in clusters with several pixels
			{
				continue;
			}
			for (unsigned int bcluster = 0; bcluster < bClusters.size();bcluster++)
			{
				const SimpleStandardCluster& oneBcluster = bClusters.at(bcluster);
				//
				if (oneBcluster.getNPixel() <_mon->mon_configdata.getCorrel_minclustersize())
				{
						continue;
				}
				else
				{
					corrmap->Fill(oneAcluster, oneBcluster);
				}
			}
		}
	}
}
void CorrelationCollection::fillHistograms(const SimpleStandardPlane& p1, const SimpleStandardPlane& p2)
{

	std::pair<SimpleStandardPlane ,SimpleStandardPlane> plane (p1,p2);
	CorrelationHistos *corrmap = _map[plane];
	if (corrmap == NULL)
	{
		std::cout << "Histogram not registered ...yet  " << p1.getName()<< " "<<p1.getID() <<" / "<< p2.getName()<<" "<<p2.getID()<<std::endl;
	}
	else
	{
		const std::vector<SimpleStandardCluster> aClusters=p1.getClusters();
		const std::vector<SimpleStandardCluster> bClusters=p2.getClusters();


		for (unsigned int acluster = 0; acluster < aClusters.size();acluster++)
		{
			const SimpleStandardCluster& oneAcluster = aClusters.at(acluster);
			if (oneAcluster.getNPixel() < _mon->mon_configdata.getCorrel_minclustersize()) // we are only interested in clusters with several pixels
			{
				continue;
			}
			for (unsigned int bcluster = 0; bcluster < bClusters.size();bcluster++)
			{
				const SimpleStandardCluster& oneBcluster = bClusters.at(bcluster);
				//
				if (oneBcluster.getNPixel() < _mon->mon_configdata.getCorrel_minclustersize())
				{
					continue;
				}
				else
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

	return _mapOld[pd];
}
CorrelationHistos * CorrelationCollection::getCorrelationHistos(const SimpleStandardPlane& p1, const SimpleStandardPlane& p2)
{
	std::pair<SimpleStandardPlane ,SimpleStandardPlane> plane (p1,p2);
	return _map[plane];
}

void CorrelationCollection::Reset()
{
	std::map<std::pair<SimpleStandardPlane,SimpleStandardPlane>, CorrelationHistos*>::iterator it;
	for (it = _map.begin(); it != _map.end(); ++it)
	{
		(*it).second->Reset();
	}
}

void CorrelationCollection::Fill(const SimpleStandardEvent &simpev)
{
	//int totalFills = 0;
	int nPlanes = simpev.getNPlanes();
	unsigned int plane_vector_size=0;
	for (int planeA = 0; planeA < nPlanes; planeA++)
	{
		const SimpleStandardPlane& simpPlane = simpev.getPlane(planeA);
		//std::cout << "Checking Plane " << simpPlane.getName() << " " <<simpPlane.getID() << "..." <<  std::endl;
		if (!isPlaneRegistered(simpPlane))
		{
			//std::cout << "Plane " << simpPlane.getName() << " " <<simpPlane.getID() << "is not registered" << std::endl;
			plane_vector_size=_planes.size();
			if (correlateAllPlanes)
			{
				for (unsigned int oldPlanes = 0 ; oldPlanes <plane_vector_size ; oldPlanes++) {
					registerPlaneCorrelations(_planes.at(oldPlanes), simpPlane); // Correlating this plane with all the other ones
				}
			}
			if ( plane_vector_size> 0 && !correlateAllPlanes)
			{
					registerPlaneCorrelations(_planes.at(plane_vector_size-1),simpPlane);
			}
			_planes.push_back(simpPlane);

		}
		for (int planeB = planeA +1; planeB < nPlanes; planeB++)
		{

			//SimpleStandardPlaneDouble db(simpev.getPlane(planeA), simpev.getPlane(planeB));
			const SimpleStandardPlane & p1=simpev.getPlane(planeA);
			const SimpleStandardPlane & p2=simpev.getPlane(planeB);
			fillHistograms(p1,p2);
			//fillHistograms(db);

			/*
			CorrelationHistos *corrmap = _map[db];
			if (corrmap != NULL) {
				totalFills += corrmap->getFills();
				corrmap->resetFills();
			}*/
		}
	}
	//std::cout << "Fills: " << totalFills << std::endl;
	//totalFills = 0;
}


void CorrelationCollection::registerPlaneCorrelations(const SimpleStandardPlane& p1, const SimpleStandardPlane& p2)
{
#ifdef DEBUG
	std::cout << "CorrelationCollection:: Correlating: " << p1.getName() << " " <<p1.getID() << " with " <<  p2.getName() << " " <<p2.getID() << std::endl;

#endif
	CorrelationHistos *tmphisto = new CorrelationHistos( p1, p2);
	//SimpleStandardPlaneDouble planeDouble(p1,p2);
	//_map[planeDouble] = tmphisto;
	pair<SimpleStandardPlane,SimpleStandardPlane> pdouble(p1,p2);
	_map[pdouble]=tmphisto;



	if (_mon != NULL)
	{
		cout << "HitmapCollection:: Monitor running in online-mode" << endl;
		char tree[1024];
		sprintf(tree,"Correlation/%s %i/%s %i in X",p1.getName().c_str(),p1.getID(),p2.getName().c_str(),p2.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getCorrelationHistos(p1,p2)->getCorrXHisto(), "COLZ",0);

		sprintf(tree,"Correlation/%s %i/%s %i in Y",p1.getName().c_str(),p1.getID(),p2.getName().c_str(),p2.getID());
		_mon->getOnlineMon()->registerTreeItem(tree);
		_mon->getOnlineMon()->registerHisto(tree,getCorrelationHistos(p1,p2)->getCorrYHisto(), "COLZ",0);

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
	std::map<std::pair<SimpleStandardPlane,SimpleStandardPlane>, CorrelationHistos*>::iterator it;

	for (it = _map.begin(); it != _map.end(); ++it) {
		//char sensorfolder[255]="";
		//sprintf(sensorfolder,"%s_%d:%s_%d",it->first.getPlane1().getName().c_str(),it->first.getPlane1().getID(), it->first.getPlane2().getName().c_str(),it->first.getPlane2().getID());
		//gDirectory->mkdir(sensorfolder);
		//gDirectory->cd(sensorfolder);
		it->second->Write();


		//gDirectory->cd("..");
	}
	gDirectory->cd("..");
}
