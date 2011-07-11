/*
 * HitmapHistos.hh
 *
 *  Created on: Jun 16, 2011
 *      Author: stanitz
 */

#ifndef HITMAPHISTOS_HH_
#define HITMAPHISTOS_HH_

#include <TH2I.h>
#include <TFile.h>

#include <map>

#include "SimpleStandardEvent.hh"

const unsigned int mimosa26_max_section=4; //number of sections in MIMOSA26 //FIXME
using namespace std;
class HitmapHistos {
protected:
	std::string _sensor;
	int _id;
	int _maxX;
	int _maxY;
	bool _wait;
	TH2I * _hitmap;
	TH2I * _clusterMap;
	TH1I * _lvl1Distr;
	TH1I * _lvl1Width;
	TH1I * _lvl1Cluster;
	TH1I * _totSingle;
	TH1I * _totCluster;
	TH1F * _hitOcc;
	TH1I * _clusterSize;
	TH1I * _nClusters;
	TH1I * _nHits;
	TH1I * _clusterXWidth;
	TH1I * _clusterYWidth;
	TH1I * _nHits_section[mimosa26_max_section];
	TH1I * _nClusters_section[mimosa26_max_section];
	TH1I * _nClustersize_section[mimosa26_max_section];
	TH1I * _nbadHits;
	TH1I * _nHotPixels;
public:
	HitmapHistos(SimpleStandardPlane p);


	void Fill(const SimpleStandardHit hit);
	void Fill(const SimpleStandardPlane plane);
	void Fill(const SimpleStandardCluster cluster);
	void Reset();

	void Calculate(const int currentEventNum);
	void Write();


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
	TH1I * getNbadHitsHisto() { return _nbadHits; }
	TH1I * getSectionsNHitsHisto(unsigned int section) { return _nHits_section[section]; }
	TH1I * getSectionsNClusterHisto(unsigned int section) { return _nClusters_section[section]; }
	TH1I * getSectionsNClusterSizeHisto(unsigned int section) { return _nClustersize_section[section]; }
	TH1I * getNHotPixelsHisto() { return _nHotPixels; }

private:
	int ** plane_map_array; //store an array representing the map
	int zero_plane_array(); // fill array with zeros;

};

#ifdef __CINT__
#pragma link C++ class HitmapHistos-;
#endif

#endif /* HITMAPHISTOS_HH_ */
