#ifndef ONLINE_HISTOGRAMS_H
#define ONLINE_HISTOGRAMS_H

#include <string>
#include <TH2I.h>
#include <map>
#include <TFile.h>
#include <vector>
#include <algorithm>



#include <iostream>
#include <RQ_OBJECT.h>

#include "SimpleStandardEvent.hh"
#ifndef __CINT__
//#include "OnlineMon.hh"
#endif

class RootMonitor;

static const bool correlateAllPlanes = true;



struct Plane {
	std::string sensor;
	int id;
	
};

inline bool operator==(Plane const &a, Plane const &b) {
		if (a.sensor==b.sensor && a.id == b.id) {
			 return true;
		} else {
			return false;
		}
}

inline bool operator<(Plane const &a, Plane const &b) { // Needed to use struct in a map
	return a.sensor<b.sensor || ( a.sensor==b.sensor && a.id < b.id); 
}



class CorrelationHistos {
protected:
	std::string _sensor1;
	std::string _sensor2;
	int _id1;
	int _id2;
	int _maxX1;
	int _maxX2;
	int _maxY1;
	int _maxY2;
	TH2I * _2dcorrX;
	TH2I * _2dcorrY;
	int _fills; 
public:
	CorrelationHistos(SimpleStandardPlane p1, SimpleStandardPlane p2) 
		: _sensor1(p1.getName()), _sensor2(p2.getName()), _id1(p1.getID()), _id2(p2.getID()),
		_maxX1(p1.getMaxX()), _maxX2(p2.getMaxX()), _maxY1(p1.getMaxY()), _maxY2(p2.getMaxY()), _fills(0),
		_2dcorrX(NULL),_2dcorrY(NULL){
		char out[64], out2[64];
		if (_maxX1 != -1 && _maxX2 != -1) {
			sprintf(out,"X Correlation of %s %i and %s %i",_sensor1.c_str(),_id1, _sensor2.c_str(), _id2);
			sprintf(out2,"h_corr_X_%s_%i_vs_%s_%i",_sensor1.c_str(),_id1, _sensor2.c_str(), _id2);
			_2dcorrX = new TH2I(out2, out, _maxX1,0,_maxX1, _maxX2,0,_maxX2);
		}
		
		if (_maxY1 != -1 && _maxY2 != -1) {
			sprintf(out,"Y Correlation of %s %i and %s %i",_sensor1.c_str(),_id1, _sensor2.c_str(), _id2);
			sprintf(out2,"h_corr_Y_%s_%i_vs_%s_%i",_sensor1.c_str(),_id1, _sensor2.c_str(), _id2);
			_2dcorrY = new TH2I(out2, out, _maxY1,0,_maxY1, _maxY2,0,_maxY2);
		}
	}
	
	void Fill(const SimpleStandardCluster &cluster1, const SimpleStandardCluster &cluster2) {
		//std::cout << "Filling Histogram: " << _2dcorrX->GetName() << " (" << cluster1.getX() << ", " << cluster2.getX() << ")" << std::endl;
		if (_2dcorrX !=NULL) _2dcorrX->Fill(cluster1.getX(),cluster2.getX());
		if (_2dcorrY !=NULL) _2dcorrY->Fill(cluster1.getY(),cluster2.getY());
		_fills++;
	}
	
	void Reset() {
		_2dcorrX->Reset();
		_2dcorrY->Reset();
	}
	
	TH2I * getCorrXHisto() { return _2dcorrX; }
	TH2I * getCorrYHisto() { return _2dcorrY; }
	int getFills() const { return _fills; }
	void resetFills() { _fills = 0; }
	
	void Write() {
		_2dcorrX->Write();
		_2dcorrY->Write();
	}
	
};

class HitmapHistos {
protected:
	std::string _sensor;
	int _id;
	TH2I * _hitmap;
	TH2I * _clusterMap;
	TH1I * _lvl1Distr;
	TH1I * _lvl1Width;
	TH1I * _lvl1Cluster;
	TH1I * _totSingle;
	TH1I * _totCluster;
	TH1F * _hitOcc;
	TH1I * _clusterSize;
	TH1I * _nHits;
	TH1I * _nClusters;
	TH1I * _clusterXWidth;
	TH1I * _clusterYWidth;
	int _maxX;
	int _maxY;
	bool _wait;
public:
		HitmapHistos(SimpleStandardPlane p) : _sensor(p.getName()), _id(p.getID()), _maxX(p.getMaxX()), _maxY(p.getMaxY()), _wait(false),
		_hitmap(NULL),_clusterMap(NULL),_lvl1Distr(NULL), _lvl1Width(NULL),_lvl1Cluster(NULL),_totSingle(NULL),_totCluster(NULL),
		_hitOcc(NULL), _nClusters(NULL), _nHits(NULL), _clusterXWidth(NULL), _clusterYWidth(NULL){
		char out[64], out2[64];
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
			_totSingle = new TH1I(out2, out,256,0,255);
			
			sprintf(out,"%s %i TOT Clusters",_sensor.c_str(), _id);
			sprintf(out2,"h_totcluster_%s_%i",_sensor.c_str(), _id);
			_totCluster= new TH1I(out2, out,256,0,255);
			
			sprintf(out,"%s %i Hitoccupancy",_sensor.c_str(), _id);
			sprintf(out2,"h_hitocc%s_%i",_sensor.c_str(), _id);
			_hitOcc= new TH1F(out2, out,100,0.01, 1);

			sprintf(out,"%s %i Clustersize",_sensor.c_str(), _id);
			sprintf(out2,"h_clustersize_%s_%i",_sensor.c_str(), _id);
			_clusterSize= new TH1I(out2, out,10,0,9);
			
			sprintf(out,"%s %i Number of Hits",_sensor.c_str(), _id);
			sprintf(out2,"h_nHits_%s_%i",_sensor.c_str(), _id);
			_nHits = new TH1I(out2, out,100,0,99);
			
			sprintf(out,"%s %i Number of Clusters",_sensor.c_str(), _id);
			sprintf(out2,"h_nClusters_%s_%i",_sensor.c_str(), _id);
			_nClusters = new TH1I(out2, out,100,0,99);
			
			sprintf(out,"%s %i Clustersize in X",_sensor.c_str(), _id);
			sprintf(out2,"h_clustersizeX_%s_%i",_sensor.c_str(), _id);
			_clusterXWidth = new TH1I(out2, out,20,0,19);
			
			sprintf(out,"%s %i Clustersize in Y",_sensor.c_str(), _id);
			sprintf(out2,"h_clustersizeY_%s_%i",_sensor.c_str(), _id);
			_clusterYWidth = new TH1I(out2, out,20,0,19);
			

		} else {
			std::cout << "No max sensorsize known!" << std::endl;
		}
		
	}
	/*
	void Fill(const int a, const int b) {
		_hitOcc->Fill(a,b);
		//std::cout << "Filling Histogram " << _hitOcc->GetName() << std::endl;
	}
	*/
	void Fill(const SimpleStandardHit hit) {
		if (_hitmap != NULL) _hitmap->Fill(hit.getX(), hit.getY());
		if (_sensor == std::string("APIX")) {
			if (_totSingle != NULL) _totSingle->Fill(hit.getTOT());
			if (_lvl1Distr != NULL) _lvl1Distr->Fill(hit.getLVL1());
		}
	}

	void Fill(const SimpleStandardPlane plane) {
		if (_nHits != NULL) _nHits->Fill(plane.getNHits());
		if (_nClusters != NULL) _nClusters->Fill(plane.getNClusters());
		

	}

	void Fill(const SimpleStandardCluster cluster) {
		if (_clusterMap != NULL) _clusterMap->Fill(cluster.getX(), cluster.getY());
		if (_clusterSize != NULL) _clusterSize->Fill(cluster.getNPixel());
		if (_sensor == std::string("APIX")) {
			if (_lvl1Width != NULL)   _lvl1Width->Fill(cluster.getLVL1Width());
			if (_totCluster != NULL ) _totCluster->Fill(cluster.getTOT());
			if (_lvl1Cluster != NULL) _lvl1Cluster->Fill(cluster.getFirstLVL1());
			if (_clusterXWidth != NULL) _clusterXWidth->Fill(cluster.getWidthX());
			if (_clusterYWidth != NULL) _clusterYWidth->Fill(cluster.getWidthY());
		}

	}
	
	void Reset() {
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
		_clusterYWidth->Reset();
		_clusterXWidth->Reset();
	}
	
	void Calculate(const int currentEventNum) {
		char out[64], out2[64];
		
		_wait = true;
		//In this way we are sure that _hitOcc never points to somewhere in the darkness!
		/*TH1F *waste = _hitOcc;
		_hitOcc = NULL;
		delete waste;
		*/
		sprintf(out,"%s %i Hitoccupancy Propablity",_sensor.c_str(), _id);
		sprintf(out2,"h_hitoccprop%s_%i",_sensor.c_str(), _id);
		/*if (currentEventNum < 1000) {
			_hitOcc= new TH1F(out2, out,100,0, 1);
		} else {
			_hitOcc= new TH1F(out2, out,currentEventNum/10,0, 1);
		}*/
		_hitOcc->SetBins(currentEventNum/10,0,1);
		_hitOcc->Reset();
		//std::cout << "Filling" << std::endl;
		for (int x = 0; x < _maxX; ++x) {
			for (int y = 0; y < _maxY; ++y) {
				unsigned int bin = _hitmap->GetBinContent(x,y);
				if (bin != 0) _hitOcc->Fill(bin/(double)currentEventNum);
			}
		}
		_wait = false;
		
		
	}
	
	void Write() {
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
		_clusterXWidth->Write();
		_clusterYWidth->Write();
	}
	
	TH2I * getHitmapHisto() { return _hitmap; }
	TH2I * getClusterMapHisto() { return _clusterMap; }
	TH1I * getLVL1Histo() { return _lvl1Distr; }
	TH1I * getLVL1WidthHisto() { return _lvl1Width; }
	TH1I * getLVL1ClusterHisto() { return _lvl1Cluster; }
	TH1I * getTOTSingleHisto() { return _totSingle; }
	TH1I * getTOTClusterHisto() { return _totCluster; }
	TH1F * getHitOccHisto() { if (_wait) return NULL; else return _hitOcc; }
	TH1I * getClusterSizeHisto() { return _clusterSize; }	
	TH1I * getNHitsHisto() { return _nHits; }
	TH1I * getNClustersHisto() { return _nClusters; }	
	TH1I * getClusterWidthXHisto() { return _clusterXWidth; }
	TH1I * getClusterWidthYHisto() { return _clusterYWidth; }
};
class RootMonitor;

class SimpleStandardPlaneDouble {
protected:
	SimpleStandardPlane _p1;
	SimpleStandardPlane _p2;
public:
	SimpleStandardPlaneDouble(const SimpleStandardPlane &p1, const SimpleStandardPlane &p2) : _p1(p1), _p2(p2) {}
	std::string getName() const {
		char out[64];
		sprintf(out, "%s%i_vs_%s%i",_p1.getName(),_p1.getID(),_p2.getName(),_p2.getID());
		return std::string(out);
	}
	SimpleStandardPlane getPlane1() const { return _p1; }
	SimpleStandardPlane getPlane2() const { return _p2; }
};

inline bool operator==(SimpleStandardPlaneDouble const &a, SimpleStandardPlaneDouble const &b) {
		if ( a.getPlane1() ==  b.getPlane1() && a.getPlane2() ==  b.getPlane2()) {
			 return true;
		} else {
			if ( a.getPlane1() ==  b.getPlane2() && a.getPlane2() ==  b.getPlane1()) {
				return true;
			} else { return false;}
		}
}

inline bool operator<(SimpleStandardPlaneDouble const &a, SimpleStandardPlaneDouble const &b) { // Needed to use struct in a map
	return a.getPlane1()<b.getPlane1() || ( a.getPlane1()==b.getPlane1() && a.getPlane2() < b.getPlane2()); 
}


class BaseCollection {
protected:
	unsigned int _reduce;
	RootMonitor *_mon;
public:
		BaseCollection() : _reduce(1), _mon(NULL) {};
		virtual void Write(TFile *file) = 0;
		virtual void Calculate(const unsigned int currentRunNumber) = 0;
		virtual void Fill(SimpleEvent &simpev) = 0;
		virtual void Reset() = 0;
		void setReduce(const unsigned int red) { _reduce = red; }; 
};

class CorrelationCollection : public BaseCollection {
protected:
	
	std::map<SimpleStandardPlaneDouble, CorrelationHistos*> _map;
	std::vector<SimpleStandardPlane> _planes;
	bool isPlaneRegistered(SimpleStandardPlane p) {
			std::vector<SimpleStandardPlane>::iterator it = std::find(_planes.begin(), _planes.end(), p);
			if (it == _planes.end()) { return false; } else { return true; }
		}
	void fillHistograms(const SimpleStandardPlaneDouble &simpPlaneDouble) {
		CorrelationHistos *corrmap = _map[simpPlaneDouble];
		if (corrmap == NULL) {
			//std::cout << "Histogram not registered" << std::endl; 
		} else {
			SimpleStandardPlane planeA = simpPlaneDouble.getPlane1();
			SimpleStandardPlane planeB = simpPlaneDouble.getPlane2();
			
			for (int acluster = 0; acluster < planeA.getNClusters();acluster++) {
				SimpleStandardCluster oneAcluster = planeA.getCluster(acluster);
				for (int bcluster = 0; bcluster < planeB.getNClusters();bcluster++) {
					SimpleStandardCluster oneBcluster = planeB.getCluster(bcluster);
					if (oneAcluster.getNPixel() > 1 && oneBcluster.getNPixel() > 1) {
						corrmap->Fill(oneAcluster, oneBcluster);
					}
				}
			}
		}
	}
public:
	CorrelationCollection() : BaseCollection() {}
	void Fill(SimpleEvent &simpev) {
		int totalFills = 0;
		int nPlanes = simpev.getNPlanes();
		for (int planeA = 0; planeA < nPlanes; planeA++) {
			SimpleStandardPlane simpPlane = simpev.getPlane(planeA);
			//std::cout << "Ckecking Plane " << simpPlane.getName() << " " <<simpPlane.getID() << "..." <<  std::endl;
			if (!isPlaneRegistered(simpPlane)) {
				//std::cout << "Plane " << simpPlane.getName() << " " <<simpPlane.getID() << "is not registered" << std::endl;
				if (correlateAllPlanes) {
					for (int oldPlanes = 0 ; oldPlanes < _planes.size(); oldPlanes++) {
						registerPlaneCorrelations(_planes.at(oldPlanes), simpPlane); // Correlating this plane with all the other ones
					}
				}
				if (_planes.size() > 0 && !correlateAllPlanes) registerPlaneCorrelations(_planes.at(_planes.size()-1),simpPlane);
				_planes.push_back(simpPlane);
			}
			for (int planeB = planeA +1; planeB < nPlanes; planeB++) {
			
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
	virtual void Reset() {
		std::map<SimpleStandardPlaneDouble, CorrelationHistos*>::iterator it;
		for (it = _map.begin(); it != _map.end(); it++) {
			(*it).second->Reset();
		}	
	}
	void registerPlaneCorrelations(const SimpleStandardPlane &p1, const SimpleStandardPlane &p2);
	void setRootMonitor(RootMonitor *mon)  {_mon = mon; }
	CorrelationHistos * getCorrelationHistos(SimpleStandardPlaneDouble pd) {
		
		return _map[pd];
	}
	
	virtual void Write(TFile *file);
	virtual void Calculate(const unsigned int currentRunNumber) {};
	
};


class HitmapCollection : public BaseCollection{
	RQ_OBJECT("HitmapCollection")
protected:
	bool isOnePlaneRegistered;
	std::map<SimpleStandardPlane,HitmapHistos*> _map;
	
		
	bool isPlaneRegistered(SimpleStandardPlane p) {
		std::map<SimpleStandardPlane,HitmapHistos*>::iterator it;
		it = _map.find(p);
		return (it != _map.end());
	}
	
	void fillHistograms(const SimpleStandardPlane &simpPlane) {
		
		if (!isPlaneRegistered(simpPlane)) {
			
			registerPlane(simpPlane);
			
			isOnePlaneRegistered = true;
		}
		
		HitmapHistos *hitmap = _map[simpPlane];
		hitmap->Fill(simpPlane);
		
		for (int hitpix = 0; hitpix < simpPlane.getNHits();hitpix++) {
			SimpleStandardHit onehit = simpPlane.getHit(hitpix);
			
			hitmap->Fill(onehit);
		}
		
		for (int cluster = 0; cluster < simpPlane.getNClusters();cluster++) {
			SimpleStandardCluster onecluster = simpPlane.getCluster(cluster);
			
			hitmap->Fill(onecluster);
		}
		
	}
	
public:
	void registerPlane(const SimpleStandardPlane &p);
	
	void bookHistograms(const SimpleEvent &simpev) {
		for (int plane = 0; plane < simpev.getNPlanes(); plane++) {
			SimpleStandardPlane simpPlane = simpev.getPlane(plane);
			if (!isPlaneRegistered(simpPlane)) {
				registerPlane(simpPlane);
				
			}
			
		}
	}
	void setRootMonitor(RootMonitor *mon)  {_mon = mon; }
	
	HitmapCollection() : BaseCollection() {isOnePlaneRegistered = false;}
	void Fill(SimpleEvent &simpev) {

		for (int plane = 0; plane < simpev.getNPlanes(); plane++) {
			SimpleStandardPlane simpPlane = simpev.getPlane(plane);
			fillHistograms(simpPlane);
		}
	}
	HitmapHistos * getHitmapHistos(std::string sensor, int id) {
		SimpleStandardPlane sp(sensor,id);
		return _map[sp];
	}
	
	void Reset() {
		std::map<SimpleStandardPlane,HitmapHistos*>::iterator it;
		for (it = _map.begin(); it != _map.end(); it++) {
			(*it).second->Reset();
		}
		
	}
	
	virtual void Write(TFile *file);
	virtual void Calculate(const unsigned int currentRunNumber) {
		if (currentRunNumber > 10 && currentRunNumber % 500*_reduce == 0) {
			std::map<SimpleStandardPlane,HitmapHistos*>::iterator it;
			for (it = _map.begin(); it != _map.end(); it++) {
				//std::cout << "Calculating ..." << std::endl;
				it->second->Calculate(currentRunNumber/_reduce);
			}
		}
	}
	
};

class OnlineHistograms {
	protected:
		std::vector<std::string> _sensorVec;
		
	
	public:
		OnlineHistograms() {}
		void addSensor(const std::string name, const int id) {}
		void FillHistos() {}
	
};

#ifdef __CINT__

#pragma link C++ class HitmapCollection-;
#pragma link C++ class HitmapHistos-;
#pragma link C++ class OnlineHistograms-;
#pragma link C++ class BaseCollection-;
#pragma link C++ class CorrelationCollection-;
#pragma link C++ class CorrelationHistos-;
#endif

#endif
