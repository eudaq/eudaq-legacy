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
		std::cout << "CorrelationCollection: Histogram not registered ...yet  " << p1.getName()<< " "<<p1.getID() <<" / "<< p2.getName()<<" "<<p2.getID()<<std::endl;
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
	int nPlanes_disabled=0;

	unsigned int plane_vector_size=0;
	if (skip_this_plane.size()==0) // do this only at the very first event
	{
		selected_planes_to_skip=_mon->mon_configdata.getPlanes_to_be_skipped();
		skip_this_plane.reserve(nPlanes);
		//init vector
		for (int elements=0; elements<nPlanes; elements++)
		{
			skip_this_plane[elements]=false;
		}
		// now get vector of planes to be disabled and set the corresponding entries to true
		for (unsigned int skipplanes=0; skipplanes<selected_planes_to_skip.size(); skipplanes++)
		{
			if ((selected_planes_to_skip[skipplanes]>0) && (selected_planes_to_skip[skipplanes]<nPlanes))
			{
				skip_this_plane[selected_planes_to_skip[skipplanes]]=true;
				std::cout << "CorrelationCollection : Diabling Plane "<< selected_planes_to_skip[skipplanes] <<endl;
				nPlanes_disabled++;
			}
		}
		std::cout << "CorrelationCollection : Diabling "<<  nPlanes_disabled << "Planes" << endl;
	}
    if (nPlanes-nPlanes_disabled<2)
    {
    	std::cout << "CorrelationCollection : Too Many Planes Disabled ..." <<endl;
    }
    else
    {
    	for (int planeA = 0; planeA < nPlanes; planeA++)
    	{
    		const SimpleStandardPlane& simpPlane = simpev.getPlane(planeA);
#ifdef DEBUG
    		std::cout << "CorrelationCollection : Checking Plane " << simpPlane.getName() << " " <<simpPlane.getID() << "..." <<  std::endl;
    		std::cout<<  "CorrelationCollection : " << planeA<<  std::endl;
#endif

    		if (!isPlaneRegistered(simpPlane)  )
    		{
#ifdef DEBUG
    			std::cout << "CorrelationCollection: Plane " << simpPlane.getName() << " " <<simpPlane.getID() << " is not registered" << std::endl;
#endif

    			plane_vector_size=_planes.size(); //how many planes we did look at beforehand
    			if (correlateAllPlanes)
    			{
    				for (unsigned int oldPlanes = 0 ; oldPlanes <plane_vector_size ; oldPlanes++)
    				{
    					registerPlaneCorrelations(_planes.at(oldPlanes), simpPlane); // Correlating this plane with all the other ones
    				}
    			}
    			else // we have deselected a few planes
    			{
    				if  (!skip_this_plane[planeA])
    				{
    					for (unsigned int oldPlanes = 0 ; oldPlanes <plane_vector_size ; oldPlanes++)
    					{
    						if (!skip_this_plane[oldPlanes])
    						{
    							registerPlaneCorrelations(_planes.at(oldPlanes), simpPlane); // Correlating this plane with all the other ones
    						}

    					}
    				}
    			}
    			_planes.push_back(simpPlane); // we have to deal with all planes

    		}
    		for (int planeB = planeA +1; planeB < nPlanes; planeB++)
    		{
    			if ((skip_this_plane[planeA]==false)&& (skip_this_plane[planeB])==false)
    			{
    				const SimpleStandardPlane & p1=simpev.getPlane(planeA);
    				const SimpleStandardPlane & p2=simpev.getPlane(planeB);
    				fillHistograms(p1,p2);
    			}

    		}
    	}
    }
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

bool CorrelationCollection::getCorrelateAllPlanes() const
{
    return correlateAllPlanes;
}

std::vector<int> CorrelationCollection::getSelected_planes_to_skip() const
{
    return selected_planes_to_skip;
}

void CorrelationCollection::setCorrelateAllPlanes(bool correlateAllPlanes)
{
    this->correlateAllPlanes = correlateAllPlanes;
}

void CorrelationCollection::setSelected_planes_to_skip(std::vector<int> selected_planes_to_skip)
{
    this->selected_planes_to_skip = selected_planes_to_skip;
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
