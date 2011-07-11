/*
 * CorrelationCollection.hh
 *
 *  Created on: Jun 17, 2011
 *      Author: stanitz
 */

#ifndef CORRELATIONCOLLECTION_HH_
#define CORRELATIONCOLLECTION_HH_
//ROOT Includes
#include <RQ_OBJECT.h>
#include <TH2I.h>
#include <TFile.h>

#include <map>
#include <vector>
#include <string>

#include "SimpleStandardPlaneDouble.hh"
#include "CorrelationHistos.hh"
#include "BaseCollection.hh"


static const bool correlateAllPlanes = true;

using namespace std;
class RootMonitor;

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




class CorrelationCollection : public BaseCollection {
protected:

	std::map<SimpleStandardPlaneDouble, CorrelationHistos*> _map;
	std::vector<SimpleStandardPlane> _planes;
	bool isPlaneRegistered(SimpleStandardPlane p);
	void fillHistograms(const SimpleStandardPlaneDouble &simpPlaneDouble);
public:
	CorrelationCollection() : BaseCollection() {}
	void Fill(SimpleEvent &simpev);
	virtual void Reset();
	void setRootMonitor(RootMonitor *mon);
	CorrelationHistos * getCorrelationHistos(SimpleStandardPlaneDouble pd);


	void registerPlaneCorrelations(const SimpleStandardPlane &p1, const SimpleStandardPlane &p2);

	virtual void Write(TFile *file);
	virtual void Calculate(const unsigned int currentRunNumber) {};

};

#ifdef __CINT__
#pragma link C++ class CorrelationCollection-;
#endif

#endif /* CORRELATIONCOLLECTION_HH_ */
