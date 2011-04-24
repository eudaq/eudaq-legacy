#ifndef SIMPLE_STANDARD_PLANE_H
#define SIMPLE_STANDARD_PLANE_H
#include <string>
#include <vector>
#include <iostream>

class SimpleStandardHit {
protected:
	int _x;
	int _y;
	int _tot;
	int _lvl1;
	int _reduceX;
	int _reduceY;

public:
	SimpleStandardHit(const int x, const int y) : _x(x), _y(y), _tot(-1), _lvl1(-1), _reduceX(-1), _reduceY(-1) {}
	SimpleStandardHit(const int x, const int y, const int tot, const int lvl1) : _x(x), _y(y), _tot(tot), _lvl1(lvl1), _reduceX(-1), _reduceY(-1) {}
	SimpleStandardHit(const int x, const int y, const int tot, const int lvl1, const int reduceX, const int reduceY) : _x(x), _y(y), _tot(tot), _lvl1(lvl1), _reduceX(reduceX), _reduceY(-reduceY){}
	int getX() const { if (_reduceX == -1) return _x; else {
			return ((int)(_x/_reduceX)*_reduceX); // when reduce is 2 e.g. 7 becomes 6 and 2 becomes 2, but 3 becomes 2 as well
		}
	}
	int getY() const { if (_reduceY == -1) return _y; else {
			return ((int)(_y/_reduceY)*_reduceY);
		}
	}
	int getTOT() const {return _tot; }
	int getLVL1() const {return _lvl1; }
	void setLVL1(const int lvl1) {_lvl1 = lvl1; }
	void setTOT(const int tot) {_tot = tot; }
	void reduce(const int reduceX, const int reduceY) {_reduceX = reduceX; _reduceY = reduceY;}
};

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
		for (int i = 0; i < _hits.size(); ++i) {
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
		for (int i = 0; i < _hits.size(); ++i) {
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
		for (int i = 0; i < _hits.size(); i++) {
			back+= _hits.at(i).getX();
		}
		return (int)back/_hits.size();
	}
	int getY()  const  {
		int back = 0;
		for (int i = 0; i < _hits.size(); i++) {
			back+= _hits.at(i).getY();
		}
		return (int)back/_hits.size();
	}
	int getTOT()  const {
		int back = 0;
		for (int i = 0; i < _hits.size(); i++) {
			back+= _hits.at(i).getTOT();
		}
		return (int)back;
	}
	int getFirstLVL1() const {
		int back = -1;
		for (int i = 0; i < _hits.size(); i++) {
			if (back == -1 || _hits.at(i).getLVL1() < back ) back = _hits.at(i).getLVL1();			
		}
		return back;
	}
	int getLVL1Width() const {
		int first = -1;
		int last = -1;
		for (int i = 0; i < _hits.size(); i++) {
			int currentLVL1 = _hits.at(i).getLVL1();
			if (first == -1 && last == -1 ) first = currentLVL1; last = currentLVL1;
			if (first > currentLVL1) first = currentLVL1;
			if (last < currentLVL1) last = currentLVL1;
		}
		return last -first;
	}
};


class SimpleStandardPlane {
protected:
	std::string _name;
	int _id;
	int _maxX;
	int _maxY;
	int _binsX;
	int _binsY;
	std::vector<SimpleStandardHit> _hits;
	std::vector<SimpleStandardCluster> _clusters;
	
public:
		

	SimpleStandardPlane(const std::string name, const int id, const int maxX, const int maxY) : _name(name), _id(id), _maxX(maxX), _maxY(maxY),_binsX(maxX), _binsY(maxY) {
	_hits.reserve(200);
	_clusters.reserve(20);
	}
		
	SimpleStandardPlane(const std::string name, const int id) : _name(name), _id(id), _maxX(-1), _maxY(-1) {}
	void addHit(SimpleStandardHit oneHit) {//oneHit.reduce(_binsX, _binsY); 
		_hits.push_back(oneHit); }
	std::vector<SimpleStandardHit> getHits() { return _hits; }
	int getNHits() const { return _hits.size(); }
	int getNClusters() const { return _clusters.size(); }
	SimpleStandardCluster getCluster(const int i ) const { return _clusters.at(i); }
	SimpleStandardHit getHit(const int i) const { return _hits.at(i); }
	std::string getName() const {return _name; }
	int getID() const {return _id;}
	int getMaxX() { return _maxX; }
	int getMaxY() { return _maxY; }
	int getBinsX() {return _binsX;}
	int getBinsY() {return _binsY;}
	void doClustering();
	void addSuffix( const std::string suf ) { _name = _name + suf; }
	void reducePixels(const int reduceX, const int reduceY) {
		
		_binsX = reduceX;
		_binsY = reduceY;
		//std::cout << "Reducing " << reduce << " -> " << _reduce <<" " << _maxX << " -> " << _binsX << " " << _maxY << " -> " << _binsY << std::endl; 
		
	}
};

inline bool operator==(SimpleStandardPlane const &a, SimpleStandardPlane const &b) {
	return (a.getName()==b.getName() && a.getID() == b.getID());
}



inline bool operator<(SimpleStandardPlane const &a, SimpleStandardPlane const &b) { // Needed to use struct in a map
	return a.getName()<b.getName() || ( a.getName()==b.getName() && a.getID() < b.getID()); 
}

class SimpleEvent {
protected:
	//int _nr;
	std::vector<SimpleStandardPlane> _planes;

public:
	SimpleEvent() {_planes.reserve(20);}
	void addPlane(SimpleStandardPlane &plane) {
		// Check si plane with same name and id is registered already
		bool found = false;
		for (int  i = 0; i < _planes.size(); ++i) {
			if (_planes.at(i) == plane) found = true;
		}
		if (found) plane.addSuffix("-2");
		_planes.push_back(plane); 
	}
	SimpleStandardPlane getPlane (const int i) const {return _planes.at(i);}
	int getNPlanes() const {return _planes.size(); }
	void doClustering() {
		for (int plane = 0 ; plane < getNPlanes(); plane++) {
				_planes.at(plane).doClustering();
				//std::cout << "Found " << _planes.at(plane).getNClusters() << " on Plane " << _planes.at(plane).getName() << " " << _planes.at(plane).getID() << std::endl;
		}
	}
		
	
};



#ifdef __CINT__

#pragma link C++ class SimpleStandardHit-;

#endif
#endif
