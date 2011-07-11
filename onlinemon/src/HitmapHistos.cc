/*
 * HitmapHistos.cc
 *
 *  Created on: Jun 16, 2011
 *      Author: stanitz
 */

#include "HitmapHistos.hh"
#include <cstdlib>

HitmapHistos::HitmapHistos(SimpleStandardPlane p): _sensor(p.getName()), _id(p.getID()), _maxX(p.getMaxX()), _maxY(p.getMaxY()), _wait(false),
_hitmap(NULL),_clusterMap(NULL),_lvl1Distr(NULL), _lvl1Width(NULL),_lvl1Cluster(NULL),_totSingle(NULL),_totCluster(NULL),
_hitOcc(NULL), _nClusters(NULL), _nHits(NULL), _clusterXWidth(NULL), _clusterYWidth(NULL)
{
	char out[64], out2[64];

	std::cout << "Sensorname: " << _sensor << " "<< _id<< std::endl;

	if (_maxX != -1 && _maxY != -1) {
		sprintf(out,"%s %i Raw Hitmap",_sensor.c_str(),_id);
		sprintf(out2,"h_hitmap_%s_%i",_sensor.c_str(), _id);
		_hitmap = new TH2I(out2, out, _maxX,0,_maxX, _maxY,0,_maxY);
		//std::cout << "Created Histogram " << out2 << std::endl;

		sprintf(out,"%s %i Cluster Hitmap",_sensor.c_str(),_id);
		sprintf(out2,"h_clustermap_%s_%i",_sensor.c_str(), _id);
		_clusterMap = new TH2I(out2, out, _maxX,0,_maxX, _maxY,0,_maxY);
		//std::cout << "Created Histogram " << out2 << std::endl;

		sprintf(out,"%s %i LVL1 Pixel Distribution",_sensor.c_str(), _id);
		sprintf(out2,"h_lvl1_%s_%i",_sensor.c_str(), _id);
		_lvl1Distr = new TH1I(out2, out,16,0,16);

		sprintf(out,"%s %i LVL1 Cluster Distribution",_sensor.c_str(), _id);
		sprintf(out2,"h_lvl1cluster_%s_%i",_sensor.c_str(), _id);
		_lvl1Cluster = new TH1I(out2, out,16,0,16);

		sprintf(out,"%s %i LVL1 Clusterwidth",_sensor.c_str(), _id);
		sprintf(out2,"h_lvl1width_%s_%i",_sensor.c_str(), _id);
		_lvl1Width = new TH1I(out2, out,16,0,16);

		sprintf(out,"%s %i TOT Single Pixels",_sensor.c_str(), _id);
		sprintf(out2,"h_totsingle_%s_%i",_sensor.c_str(), _id);
		if (_sensor == "USBPIXI4")
			_totSingle = new TH1I(out2, out,16,0,15);
		else
			_totSingle = new TH1I(out2, out,256,0,255);


		std::cout << "Sensorname: " << _sensor << std::endl;
		sprintf(out,"%s %i TOT Clusters",_sensor.c_str(), _id);
		sprintf(out2,"h_totcluster_%s_%i",_sensor.c_str(), _id);
		if (_sensor == "USBPIXI4")
			_totCluster= new TH1I(out2, out,80,0,79);
		else
			_totCluster= new TH1I(out2, out,256,0,255);

		sprintf(out,"%s %i Hitoccupancy",_sensor.c_str(), _id);
		sprintf(out2,"h_hitocc%s_%i",_sensor.c_str(), _id);
		_hitOcc= new TH1F(out2, out,250,0.01, 1);

		sprintf(out,"%s %i Clustersize",_sensor.c_str(), _id);
		sprintf(out2,"h_clustersize_%s_%i",_sensor.c_str(), _id);
		_clusterSize= new TH1I(out2, out,10,0,9);

		sprintf(out,"%s %i Number of Hits",_sensor.c_str(), _id);
		sprintf(out2,"h_nHits_%s_%i",_sensor.c_str(), _id);
		_nHits = new TH1I(out2, out,500,0,99);

		sprintf(out,"%s %i Number of Invalid Hits",_sensor.c_str(), _id);
		sprintf(out2,"h_nbadHits_%s_%i",_sensor.c_str(), _id);
		_nbadHits = new TH1I(out2, out,50,0,50);

		sprintf(out,"%s %i Number of Hot Pixels",_sensor.c_str(), _id);
		sprintf(out2,"h_nhotpixels_%s_%i",_sensor.c_str(), _id);
		_nHotPixels = new TH1I(out2, out,50,0,50);


		sprintf(out,"%s %i Number of Clusters",_sensor.c_str(), _id);
		sprintf(out2,"h_nClusters_%s_%i",_sensor.c_str(), _id);
		_nClusters = new TH1I(out2, out,500,0,99);

		sprintf(out,"%s %i Clustersize in X",_sensor.c_str(), _id);
		sprintf(out2,"h_clustersizeX_%s_%i",_sensor.c_str(), _id);
		_clusterXWidth = new TH1I(out2, out,20,0,19);

		sprintf(out,"%s %i Clustersize in Y",_sensor.c_str(), _id);
		sprintf(out2,"h_clustersizeY_%s_%i",_sensor.c_str(), _id);
		_clusterYWidth = new TH1I(out2, out,20,0,19);


		for (unsigned int section=0; section<mimosa26_max_section; section++)
		{
			sprintf(out,"%s %i Number of Hits in Section %i ",_sensor.c_str(), _id,section);
			sprintf(out2,"h_hits_section%i_%s_%i",section,_sensor.c_str(), _id);
			_nHits_section[section]= new TH1I(out2, out,50,0,50);
			if (_nHits_section[section]==NULL)
			{
				cout<< "Error allocating Histogram" << out<< endl;
				exit(-1);
			}
			else
			{
				_nHits_section[section]->GetXaxis()->SetTitle("Hits");
			}

			sprintf(out,"%s %i Number of Clusters in Section %i ",_sensor.c_str(), _id,section);
			sprintf(out2,"h_clusters_section%i_%s_%i",section,_sensor.c_str(), _id);
			_nClusters_section[section]= new TH1I(out2, out,50,0,50);
			if (_nClusters_section[section]==NULL)
			{
				cout<< "Error allocating Histogram" << out<< endl;
				exit(-1);
			}
			else
			{
				_nClusters_section[section]->GetXaxis()->SetTitle("Clusters");
			}

			sprintf(out,"%s %i Cluster size in Section %i ",_sensor.c_str(), _id,section);
			sprintf(out2,"h_clustersize_section%i_%s_%i",section,_sensor.c_str(), _id);
			_nClustersize_section[section]= new TH1I(out2, out,10,0,10);
			if (_nClustersize_section[section]==NULL)
			{
					cout<< "Error allocating Histogram" << out<< endl;
					exit(-1);
			}
			else
			{
				_nClustersize_section[section]->GetXaxis()->SetTitle("Cluster Size");
			}

		}
// make a plane array for calculating e..g hotpixels and occupancy

		plane_map_array=new int*[_maxX];
		if (plane_map_array!=NULL)
		{
			for (int j=0; j< _maxX; j++)
			{
				plane_map_array[j]=new int[_maxY];
				if (plane_map_array[j]==NULL)
				{
					cout << "HitmapHistos :Error in memory allocation"<<endl;
					exit(-1);
				}
			}
			zero_plane_array();
		}






	} else {
		std::cout << "No max sensorsize known!" << std::endl;
	}

}

int HitmapHistos::zero_plane_array()
{
	for (int i=0; i<_maxX; i++)
	{
		for (int j=0; j< _maxY; j++)
		{
			plane_map_array[i][j]=0;
		}
	}
	return 0;
}

void HitmapHistos::Fill(const SimpleStandardHit hit)
{
	int pixel_x=hit.getX();
	int pixel_y=hit.getY();

	if (_hitmap != NULL) _hitmap->Fill(pixel_x,pixel_y);
	if ((pixel_x<_maxX) &&  (pixel_y<_maxY))
	{
		plane_map_array[pixel_x][pixel_y]=plane_map_array[pixel_x][pixel_y]+1;
	}
	//if (_sensor == std::string("APIX")) {
	if (_sensor == std::string("APIX") || _sensor == std::string("USBPIX") || _sensor == std::string("USBPIXI4") ) {
		if (_totSingle != NULL) _totSingle->Fill(hit.getTOT());
		if (_lvl1Distr != NULL) _lvl1Distr->Fill(hit.getLVL1());
	}
}

void HitmapHistos::Fill(const SimpleStandardPlane plane)
{
	if (_nHits != NULL) _nHits->Fill(plane.getNHits());
	if ((_nbadHits != NULL) &&(plane.getNBadHits()>0))
	{
		_nbadHits->Fill(plane.getNBadHits());
	}
	if (_nClusters != NULL) _nClusters->Fill(plane.getNClusters());

	// we fill the information for the individula mimosa sections, and do a zero-suppression,in case not all sections have hits/clusters
	if (_sensor==std::string("MIMOSA26"))
	{
		for (unsigned int section=0; section<mimosa26_max_section; section++)
		{
			if (_nHits_section[section]!=NULL)
			{
				if (plane.getNSectionHits(section)> 0)
				{
					_nHits_section[section]->Fill(plane.getNSectionHits(section));
				}
			}
			if (_nClusters_section[section]!=NULL)
			{
				if (plane.getNSectionClusters(section)>0)
				{
						_nClusters_section[section]->Fill(plane.getNSectionClusters(section));
				}
			}
		}
	}

}

void HitmapHistos::Fill(const SimpleStandardCluster cluster)
{
	if (_clusterMap != NULL) _clusterMap->Fill(cluster.getX(), cluster.getY());
	if (_clusterSize != NULL) _clusterSize->Fill(cluster.getNPixel());
	if (_sensor==std::string("MIMOSA26"))
	{
		unsigned int nsection=cluster.getX()/288; //get to which section in Mimosa the cluster belongs
		if ((nsection <mimosa26_max_section) && (_nClustersize_section[nsection]!=NULL)) //check if valid address
		{
			if (cluster.getNPixel()>0)
			{
				_nClustersize_section[nsection]->Fill(cluster.getNPixel());
			}
		}
	}
	//if (_sensor == std::string("APIX")) {
	if (_sensor == std::string("APIX") || _sensor == std::string("USBPIX") || _sensor == std::string("USBPIXI4") ) {
		if (_lvl1Width != NULL)   _lvl1Width->Fill(cluster.getLVL1Width());
		if (_totCluster != NULL ) _totCluster->Fill(cluster.getTOT());
		if (_lvl1Cluster != NULL) _lvl1Cluster->Fill(cluster.getFirstLVL1());
		if (_clusterXWidth != NULL) _clusterXWidth->Fill(cluster.getWidthX());
		if (_clusterYWidth != NULL) _clusterYWidth->Fill(cluster.getWidthY());
	}
}


void HitmapHistos::Reset() {
	_hitmap->Reset();
	_totSingle->Reset();
	_lvl1Distr->Reset();
	_clusterMap->Reset();
	_totCluster->Reset();
	_lvl1Cluster->Reset();
	_lvl1Width->Reset();
	_hitOcc->Reset();
	_clusterSize->Reset();
	_nClusters->Reset();
	_nHits->Reset();
	_nbadHits->Reset();
	_nHotPixels->Reset();
	_clusterYWidth->Reset();
	_clusterXWidth->Reset();
	for (unsigned int  section=0; section<mimosa26_max_section; section++)
	{
		_nClusters_section[section]->Reset();
		_nHits_section[section]->Reset();
		_nClustersize_section[section]->Reset();
	}
	// we have to reset the aux array as well
	zero_plane_array();
}





void HitmapHistos::Calculate(const int currentEventNum)
{
//	char out[64], out2[64];

	_wait = true;

//	sprintf(out,"%s %i Hitoccupancy Propablity",_sensor.c_str(), _id);
//	sprintf(out2,"h_hitoccprop%s_%i",_sensor.c_str(), _id);
	/*if (currentEventNum < 1000) {
			_hitOcc= new TH1F(out2, out,100,0, 1);
		} else {
			_hitOcc= new TH1F(out2, out,currentEventNum/10,0, 1);
		}*/
	_hitOcc->SetBins(currentEventNum/10,0,1);
	_hitOcc->Reset();
	//std::cout << "Filling" << std::endl;

	int nHotpixels=0;
	double Hotpixelcut=0.05; //FIXME should be configurable
	double bin=0;
	double occupancy=0;
	for (int x = 0; x < _maxX; ++x) {
		for (int y = 0; y < _maxY; ++y) {

			bin=plane_map_array[x][y];

			if (bin != 0)
			{
				occupancy=bin/(double)currentEventNum;
				_hitOcc->Fill(occupancy);
				if (occupancy>Hotpixelcut)
				{
					nHotpixels++;
				}
			}
		}
	}
	if (nHotpixels>0)
	{
		_nHotPixels->Fill(nHotpixels);
	}
	_wait = false;


}


void HitmapHistos::Write()
{
	_hitmap->Write();
	_totSingle->Write();
	_lvl1Distr->Write();
	_clusterMap->Write();
	_totCluster->Write();
	_lvl1Cluster->Write();
	_lvl1Width->Write();
	_hitOcc->Write();
	_clusterSize->Write();
	_nClusters->Write();
	_nHits->Write();
	_nbadHits->Write();
	_nHotPixels->Write();
	_clusterXWidth->Write();
	_clusterYWidth->Write();
	for (unsigned int  section=0; section<mimosa26_max_section; section++)
	{
		_nClusters_section[section]->Write();
		_nHits_section[section]->Write();
		_nClustersize_section[section]->Write();
	}
}
