/*
 * SimpleStandardPlane.hh
 *
 *  Created on: Jun 9, 2011
 *      Author: stanitz
 */

#ifndef SIMPLESTANDARDPLANE_HH_
#define SIMPLESTANDARDPLANE_HH_

#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>

#include <stdio.h>
#include <string.h>

#include "include/SimpleStandardHit.hh"
#include "include/SimpleStandardCluster.hh"



class SimpleStandardPlane {
protected:
	std::string _name;
	int _id;
	int _maxX;
	int _maxY;
	int _binsX;
	int _binsY;
	std::vector<SimpleStandardHit> _hits;
	std::vector<SimpleStandardHit> _badhits; //stores hits which appear to be corrupted
	std::vector<SimpleStandardCluster> _clusters;
	std::vector<SimpleStandardHit> _section_hits[4];//FIXME hard-coded
	std::vector<SimpleStandardCluster> _section_clusters[4];//FIXME hard-coded
public:


	SimpleStandardPlane(const std::string name, const int id, const int maxX, const int maxY) : _name(name), _id(id), _maxX(maxX), _maxY(maxY),_binsX(maxX), _binsY(maxY) {
	_hits.reserve(400);
	_badhits.reserve(400); //
	_clusters.reserve(40);
	}

	SimpleStandardPlane(const std::string name, const int id) : _name(name), _id(id), _maxX(-1), _maxY(-1) {}
	void addHit(SimpleStandardHit oneHit);
	void doClustering();
	std::vector<SimpleStandardHit> getHits() { return _hits; }
	int getNHits() const { return _hits.size(); }
	int getNBadHits() const { return _badhits.size(); }
	int getNSectionHits(unsigned int section) const { return _section_hits[section].size(); }
	int getNClusters() const { return _clusters.size(); }
	int getNSectionClusters(unsigned int section) const { return _section_clusters[section].size(); }
	SimpleStandardCluster getCluster(const int i ) const { return _clusters.at(i); }
	SimpleStandardHit getHit(const int i) const { return _hits.at(i); }
	std::string getName() const {return _name; }
	int getID() const {return _id;}
	int getMaxX() { return _maxX; }
	int getMaxY() { return _maxY; }
	int getBinsX() {return _binsX;}
	int getBinsY() {return _binsY;}
	void addSuffix( const std::string suf ) { _name = _name + suf; }
	void reducePixels(const int reduceX, const int reduceY);
};


#endif /* SIMPLESTANDARDPLANE_HH_ */
