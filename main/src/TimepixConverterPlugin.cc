#include "eudaq/TimepixConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"

#include <IMPL/LCEventImpl.h>
#include <IMPL/TrackerRawDataImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/LCIO.h>

#include <iostream>

namespace eudaq
{

TimepixConverterPlugin const TimepixConverterPlugin::m_timepixconverterplugininstance;

lcio::LCEvent * TimepixConverterPlugin::GetLCIOEvent( eudaq::Event const * ee ) const
{
    //try to cast the eudaq event to RawDataEvent
    eudaq::RawDataEvent const *re = dynamic_cast<eudaq::RawDataEvent const *>(ee);
    if (re == 0) //cast failed, throw an exception
	EUDAQ_THROW(std::string("TimepixConverterPlugin::GetLCIOEvent: Error") + 
		    " given event is not a eudaq::RawDataEvent");
    
    //check type of RawDataEvent
    if (re->GetType() != "TimepixEvent")
	EUDAQ_THROW(std::string("TimepixConverterPlugin::GetLCIOEvent: Error") + 
		    " given event is not a TimepixEvent");
 
    // currently there is only one data block which contains the timepix data
    std::vector<unsigned char> bytedata = re->GetBlockUChar(0);

    // convert the byte sequence to lcio data
    std::vector<short> timepixdata;
    timepixdata.resize(65536);
    for (unsigned int i=0; i < 65536 ; i++)
    {
	// the byte sequence is little endian
	timepixdata[i]= ( (bytedata[2*i]& 0xFF) << 8) |  (bytedata[2*i+1]& 0xFF);
    }

    lcio::TrackerRawDataImpl *timepixlciodata=new lcio::TrackerRawDataImpl;
    timepixlciodata->setCellID0(0);
    timepixlciodata->setCellID1(0);
    timepixlciodata->setADCValues(timepixdata);

    lcio::LCCollectionVec * timepixCollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERRAWDATA);
    timepixCollection->addElement(timepixlciodata);
    
    lcio::LCEventImpl * le = new lcio::LCEventImpl;
    le->setEventNumber( ee->GetEventNumber () );
    le->setRunNumber(    ee->GetRunNumber () );
    le->setDetectorName( "Timepix");
    le->setTimeStamp( ee->GetTimestamp() );
    le->addCollection(timepixCollection,"TimePixRawData");

    return le;
}

StandardEvent * TimepixConverterPlugin::GetStandardEvent( eudaq::Event const * ee ) const
{
    StandardEvent * se = new StandardEvent;
    se->b = ee->GetEventNumber ();

    return se;
}

} //namespace eudaq
