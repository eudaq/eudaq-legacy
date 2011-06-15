#include "SimpleStandardEvent.hh"
#include <set>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>

void SimpleStandardPlane::doClustering() {
	int nClusters = 0;
	std::vector<int> clusterNumber;
	const int NOCLUSTER = -1000;
	const int minXDistance = 1;
	const int minYDistance = 1;
	clusterNumber.assign(_hits.size(), NOCLUSTER);
	//std::cout << "Before big loop" << std::endl;
	//std::cout << "Name: " << _name << std::endl;
	if (_hits.size() > 1 && _hits.size() < 1000 && strcmp(_name.c_str(), "FORTIS") != 0) {
		for (unsigned int aPixel = 0; aPixel < _hits.size(); aPixel++) {
			for (unsigned int bPixel = aPixel+1; bPixel < _hits.size(); bPixel++) {
				//std::cout << aPixel << " " << bPixel << " " << _hits.size() << std::endl;
				SimpleStandardHit aPix = _hits.at(aPixel);
				SimpleStandardHit bPix = _hits.at(bPixel);
				int xDist = abs(aPix.getX() - bPix.getX());
				int yDist = abs(aPix.getY() - bPix.getY());
				if ( ( xDist <= minXDistance) && (yDist <= minYDistance)  ) { //this means they are neighbours in x-direction && / this means they are neighbours in y-direction
					if ( (clusterNumber.at(aPixel) == NOCLUSTER) && clusterNumber.at(bPixel) == NOCLUSTER) { // none of these pixels have been assigned to a cluster
						++nClusters;
						clusterNumber.at(aPixel) = nClusters;
						clusterNumber.at(bPixel) = nClusters;
					}else if ( (clusterNumber.at(aPixel) == NOCLUSTER) && clusterNumber.at(bPixel) != NOCLUSTER) { // b was assigned already, a not
						clusterNumber.at(aPixel) = clusterNumber.at(bPixel);
					}else if ( (clusterNumber.at(aPixel) != NOCLUSTER) && clusterNumber.at(bPixel) == NOCLUSTER) { // a was assigned already, b not
						clusterNumber.at(bPixel) = clusterNumber.at(aPixel);
					}else { //both pixels have a clusternumber already
						int min = std::min(clusterNumber.at(aPixel), clusterNumber.at(bPixel));
						clusterNumber.at(aPixel) = min;
						clusterNumber.at(bPixel) = min;
					}
				} else { // these pixels are not neighboured
					if ( clusterNumber.at(aPixel) == NOCLUSTER ) {
						++nClusters;
						clusterNumber.at(aPixel) = nClusters;
					}
					if ( clusterNumber.at(bPixel) == NOCLUSTER ) {
						++nClusters;
						clusterNumber.at(bPixel) = nClusters;
					}
				}
				
				
			}
			
		}
	} else { //You can't use the clustering algorithym with only one pixel
		if (_hits.size() == 1) clusterNumber.at(0) = 1;
	}
	
	//std::cout << "After big loop" << std::endl;
	std::set<int> clusterSet;
	for (unsigned int i=0; i<_hits.size();++i) {
		if ( clusterNumber.at(i) == NOCLUSTER) {
			//std::cerr << "Cluster with ID -1 in " << i << std::endl;
		} else {
			clusterSet.insert(clusterNumber.at(i));
		}
	}
	nClusters = clusterSet.size();
	
	std::set<int>::iterator it;
	for (it=clusterSet.begin();it!=clusterSet.end();it++) {
		SimpleStandardCluster cluster;
		for (unsigned int i=0;i< _hits.size();i++) {
			if(clusterNumber.at(i)== *it) { //Put only these pixels in that ClusterCollection that belong to that cluster
				cluster.addPixel(_hits.at(i));	
			}
		}
		_clusters.push_back(cluster);
		//std::cout << "Cluster at: " << cluster.getX() << std::endl;
		
	}
}
