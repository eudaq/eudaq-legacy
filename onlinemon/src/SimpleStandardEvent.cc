#include "SimpleStandardEvent.hh"
#include <set>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <string.h>



void SimpleEvent::addPlane(SimpleStandardPlane &plane) {
	// Checks if plane with same name and id is registered already
	bool found = false;
	for (unsigned int  i = 0; i < _planes.size(); ++i) {
		if (_planes.at(i) == plane) found = true;
	}
	if (found) plane.addSuffix("-2");
	_planes.push_back(plane);
}
void SimpleEvent::doClustering() {
	for (int plane = 0 ; plane < getNPlanes(); plane++) {
		_planes.at(plane).doClustering();
		//std::cout << "Found " << _planes.at(plane).getNClusters() << " on Plane " << _planes.at(plane).getName() << " " << _planes.at(plane).getID() << std::endl;
	}
}
