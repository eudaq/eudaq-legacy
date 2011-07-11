/*
 * BaseCollection.hh
 *
 *  Created on: Jun 16, 2011
 *      Author: stanitz
 */

#ifndef BASECOLLECTION_HH_
#define BASECOLLECTION_HH_
//ROOT includes
#include <TH2I.h>
#include <TFile.h>
//Project includes
#include "SimpleStandardEvent.hh"

class RootMonitor;

class BaseCollection
{
protected:
	unsigned int _reduce;
	RootMonitor *_mon;
public:
		BaseCollection() : _reduce(1), _mon(NULL) {};
		virtual void Write(TFile *file) = 0;
		virtual void Calculate(const unsigned int currentRunNumber) = 0;
		virtual void Fill(SimpleEvent &simpev) = 0;
		virtual void Reset() = 0;
		void setReduce(const unsigned int red);
};

#ifdef __CINT__
#pragma link C++ class BaseCollection-;
#endif
#endif /* BASECOLLECTION_HH_ */
