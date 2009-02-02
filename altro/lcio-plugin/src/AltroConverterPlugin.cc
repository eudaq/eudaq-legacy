#include "eudaq/AltroConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"

#include <IMPL/LCEventImpl.h>
#include <IMPL/TrackerRawDataImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/LCIO.h>

#include <iostream>

namespace eudaq
{

AltroConverterPlugin const AltroConverterPlugin::m_altroconverterplugininstance;

lcio::LCEvent * AltroConverterPlugin::GetLCIOEvent( eudaq::Event const * eudaqevent ) const
{
    //try to cast the eudaq event to RawDataEvent
    eudaq::RawDataEvent const *rawdataevent = dynamic_cast<eudaq::RawDataEvent const *>(eudaqevent);
    if (rawdataevent == 0) //cast failed, throw an exception
	EUDAQ_THROW(std::string("AltroConverterPlugin::GetLCIOEvent: Error") + 
		    " given event is not an eudaq::RawDataEvent");
    
    //check type of RawDataEvent
    if (rawdataevent->GetType() != "AltroEvent")
	EUDAQ_THROW(std::string("AltroConverterPlugin::GetLCIOEvent: Error") + 
		    " given event is not an AltroEvent");
 
    lcio::LCEventImpl * lcioevent = new lcio::LCEventImpl;
    lcioevent->setEventNumber( ee->GetEventNumber () );
    lcioevent->setRunNumber(    ee->GetRunNumber () );
    lcioevent->setDetectorName( "TPC with Pads and Altro");
    lcioevent->setTimeStamp( ee->GetTimestamp() );
    
    // loop all data blocks
    for (size_t block = 0 ; block < re->NumBlocks(); block++)
    {
	bytedata = rawdataevent->GetBlockUChar(block);

	// interpret the byte sequence

	// test integrity
	unsigned int eventlength = get32bitWord(0);
	if (eventlength != 4*bytedata.size())
	{
	    EUDAQ_WARN("AltroConverterPlugin::GetLCIOEvent: Wrong event length given in raw data");
	}

	unsigned int headerlength = get32bitWord(1);
	if ( (eventlength == 4) )
	{
	    EUDAQ_WARN("AltroConverterPlugin::GetLCIOEvent: Wrong event length given in raw data");
	}

	unsigned int eventlength = get32bitWord(0);
	if (eventlength != 4*bytedata.size())
	{
	    EUDAQ_WARN("AltroConverterPlugin::GetLCIOEvent: Wrong event length given in raw data");
	}

	lcio::TrackerRawDataImpl *altrolciodata=new lcio::TrackerRawDataImpl;
	altrolciodata->setCellID0(0);
	altrolciodata->setCellID1(block);
	altrolciodata->setADCValues(altrodata);

	lcio::LCCollectionVec * altroCollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERRAWDATA);
	altroCollection->addElement(altrolciodata);
    
	lcioevent->addCollection(altroCollection,"AltroRawData");
    }// for (block)

    return le;
}

StandardEvent * AltroConverterPlugin::GetStandardEvent( eudaq::Event const * ee ) const
{
    StandardEvent * se = new StandardEvent;
    se->b = ee->GetEventNumber ();

    return se;
}

} //namespace eudaq
