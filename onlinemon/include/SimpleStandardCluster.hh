/*
 * SimpleStandardCluster.hh
 *
 *  Created on: Jun 9, 2011
 *      Author: stanitz
 */

#ifndef SIMPLESTANDARDCLUSTER_HH_
#define SIMPLESTANDARDCLUSTER_HH_

#include "include/SimpleStandardHit.hh"

class SimpleStandardCluster {
protected:
	std::vector<SimpleStandardHit> _hits;

public:
	SimpleStandardCluster()  {_hits.reserve(20);}
	void addPixel(SimpleStandardHit hit) {_hits.push_back(hit); }
	int getNPixel() const { return _hits.size(); }
	int getWidthX() const {
		int min = -1;
		int max = -1;
		for (unsigned int i = 0; i < _hits.size(); ++i) {
			if (_hits.at(i).getX() < min || min == -1) {
				min = _hits.at(i).getX();
			}
			if (_hits.at(i).getX() > max || max == -1) {
				max = _hits.at(i).getX();
			}
		}
		return max - min;
	}
	int getWidthY() const {
		int min = -1;
		int max = -1;
		for (unsigned int i = 0; i < _hits.size(); ++i) {
			if (_hits.at(i).getY() < min || min == -1) {
				min = _hits.at(i).getY();
			}
			if (_hits.at(i).getY() > max || max == -1) {
				max = _hits.at(i).getY();
			}
		}
		//std::cout << "YWidth: " << max-min << std::endl;
		return max - min;
	}
	int getX() const {
		int back = 0;
		for (unsigned int i = 0; i < _hits.size(); i++) {
			back+= _hits.at(i).getX();
		}
		return (int)back/_hits.size();
	}
	int getY()  const  {
		int back = 0;
		for (unsigned int i = 0; i < _hits.size(); i++) {
			back+= _hits.at(i).getY();
		}
		return (int)back/_hits.size();
	}
	int getTOT()  const {
		int back = 0;
		for (unsigned int i = 0; i < _hits.size(); i++) {
			back+= _hits.at(i).getTOT();
		}
		return (int)back;
	}
	int getFirstLVL1() const {
		int back = -1;
		for (unsigned int i = 0; i < _hits.size(); i++) {
			if (back == -1 || _hits.at(i).getLVL1() < back ) back = _hits.at(i).getLVL1();
		}
		return back;
	}
	int getLVL1Width() const {
		int first = -1;
		int last = -1;
		for (unsigned int i = 0; i < _hits.size(); i++) {
			int currentLVL1 = _hits.at(i).getLVL1();
			if (first == -1 && last == -1 ) first = currentLVL1; last = currentLVL1;
			if (first > currentLVL1) first = currentLVL1;
			if (last < currentLVL1) last = currentLVL1;
		}
		return last -first;
	}
};
#endif /* SIMPLESTANDARDCLUSTER_HH_ */
