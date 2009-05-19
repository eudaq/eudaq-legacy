#include "eudaq/TimepixConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"

#if USE_LCIO
#  include <lcio.h>
#  include <IMPL/LCEventImpl.h>
#  include <IMPL/TrackerRawDataImpl.h>
#  include <IMPL/LCCollectionVec.h>
#endif

#include <iostream>

namespace eudaq
{

  TimepixConverterPlugin const TimepixConverterPlugin::m_timepixconverterplugininstance;

#if USE_LCIO
  bool TimepixConverterPlugin::GetLCIOSubEvent(lcio::LCEvent & le, const eudaq::Event ee) const
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

    //lcio::LCEventImpl * le = new lcio::LCEventImpl; // no longer needed, it is apssed in as a parameter

    // These should probably be set from the surrounding DetectorEvent, not from each subevent
    //le.setEventNumber( ee->GetEventNumber () );
    //le.setRunNumber(    ee->GetRunNumber () );

    // This should probably be set from the TLU event
    //le.setTimeStamp( ee->GetTimestamp() );

    // There may be multiple different detectors in one data file, so this is a property of the added collection
    //le.setDetectorName( "Timepix");

    // the vector for the timepix data, only needed one, so it's created before the loop
    std::vector<short> timepixdata;
    timepixdata.resize(65536);

    // loop all data blocks
    for (size_t block = 0 ; block < re->NumBlocks(); block++)
      {
        std::vector<unsigned char> bytedata = re->GetBlockUChar(block);

        // convert the byte sequence to lcio data
        for (unsigned int i=0; i < 65536 ; i++)
          {
            // the byte sequence is little endian
            timepixdata[i]= ( (bytedata[2*i]& 0xFF) << 8) |  (bytedata[2*i+1]& 0xFF);
          }

        lcio::TrackerRawDataImpl *timepixlciodata=new lcio::TrackerRawDataImpl;
        timepixlciodata->setCellID0(0);
        timepixlciodata->setCellID1(block);
        timepixlciodata->setADCValues(timepixdata);

        lcio::LCCollectionVec * timepixCollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERRAWDATA);
        timepixCollection->addElement(timepixlciodata);

        le.addCollection(timepixCollection,"TimePixRawData");
      }// for (block)

    return true;
  }
#else
  bool TimepixConverterPlugin::GetLCIOSubEvent(lcio::LCEvent &, const eudaq::Event &) const
  { return false; }
#endif

  bool TimepixConverterPlugin::GetStandardSubEvent(StandardEvent &, const eudaq::Event &) const
  {
    //StandardEvent * se = new StandardEvent;
    return false;
  }

} //namespace eudaq
