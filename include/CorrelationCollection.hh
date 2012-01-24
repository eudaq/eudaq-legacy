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
#include <set>
#include <vector>
#include <string>
#include <utility>

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

	std::map<SimpleStandardPlaneDouble, CorrelationHistos*> _mapOld;
	std::map<std::pair<SimpleStandardPlane,SimpleStandardPlane>, CorrelationHistos*> _map;
	std::vector<SimpleStandardPlane> _planes;
	bool isPlaneRegistered(SimpleStandardPlane p);
	void fillHistograms(const SimpleStandardPlaneDouble &simpPlaneDouble);
	void fillHistograms(const SimpleStandardPlane& p1, const SimpleStandardPlane& p2);
public:
	CorrelationCollection() : BaseCollection()
	{
		std::cout << " Initializing Correlation Collection"<<std::endl;
		CollectionType = CORRELATION_COLLECTION_TYPE;
	}
	void Fill(const SimpleStandardEvent &simpev);
	virtual void Reset();
	void setRootMonitor(RootMonitor *mon);
	CorrelationHistos * getCorrelationHistos(SimpleStandardPlaneDouble pd);
	CorrelationHistos * getCorrelationHistos(const SimpleStandardPlane &p1, const SimpleStandardPlane &p2);

	void registerPlaneCorrelations(const SimpleStandardPlane &p1, const SimpleStandardPlane &p2);

	virtual void Write(TFile *file);
	virtual void Calculate(const unsigned int currentEventNumber)
	{

	}

};

#ifdef __CINT__
#pragma link C++ class CorrelationCollection-;
#endif

#endif /* CORRELATIONCOLLECTION_HH_ */
