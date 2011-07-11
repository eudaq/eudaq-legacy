#ifndef SIMPLE_STANDARD_EVENT_H
#define SIMPLE_STANDARD_EVENT_H
#include <string>
#include <vector>
#include <iostream>

#include "include/SimpleStandardPlane.hh"








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
	void addPlane(SimpleStandardPlane &plane);
	SimpleStandardPlane getPlane (const int i) const {return _planes.at(i);}
	int getNPlanes() const {return _planes.size(); }
	void doClustering();
		
};



#ifdef __CINT__

#pragma link C++ class SimpleStandardPlane-;

#endif
#endif
