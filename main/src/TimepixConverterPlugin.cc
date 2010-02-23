#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Logger.hh"
#include "eudaq/TimepixBore.hh"

#if USE_LCIO
#  include <lcio.h>
#  include <IMPL/LCEventImpl.h>
#  include <IMPL/TrackerRawDataImpl.h>
#  include <IMPL/LCCollectionVec.h>
#  include <IMPL/LCFlagImpl.h>
#endif

#if USE_TPCCONDDATA
#  include <TimePixParameters.h>
#endif

#include <iostream>
#include <string>

namespace eudaq {

/** Implementation of the DataConverterPlugin to convert an eudaq::Event
 *  to an lcio::event.
 *
 *  The class is implemented as a singleton because it manly
 *  contains the conversion code, which is in the getLcioEvent()
 *  function. There has to be one static instance, which is 
 *  registered at the pluginManager. This is automatically done by the
 *  inherited constructor of the DataConverterPlugin.
 */

  class TimepixConverterPlugin : public DataConverterPlugin {

  public:
    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    bool GetStandardSubEvent(StandardEvent &, const eudaq::Event &) const;

    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    bool GetLCIOSubEvent(lcio::LCEvent &, const eudaq::Event &) const;

  private:
    /** The private constructor. The only time it is called is when the
     *  one single instance is created, which lives within the object.
     *  It calls the DataConverterPlugin constructor with the
     *  accoring event type string. This automatically registers
     *  the plugin to the plugin manager.
     */
    TimepixConverterPlugin() : DataConverterPlugin("Timepix") {}

    /** The one single instance of the TimepixConverterPlugin.
     *  It has to be created in the .cc file
     */
    static TimepixConverterPlugin const m_instance;
  };

  TimepixConverterPlugin const TimepixConverterPlugin::m_instance;

  bool TimepixConverterPlugin::GetStandardSubEvent(StandardEvent &, const eudaq::Event &) const {
    //StandardEvent * se = new StandardEvent;
    // TODO: implement...
    return false;
  }

#if USE_LCIO
  bool TimepixConverterPlugin::GetLCIOSubEvent(lcio::LCEvent & le, const eudaq::Event & ee) const {
    //try to cast the eudaq event to RawDataEvent
    eudaq::RawDataEvent const & re = dynamic_cast<eudaq::RawDataEvent const &>(ee);

    // the vector for the timepix data, only needed one, so it's created before the loop
    //    static const size_t NUMPIX = 65536;
    //    std::vector<short> timepixdata(NUMPIX);

    // loop all data blocks
    for (size_t block = 0 ; block < re.NumBlocks(); block++) {
	RawDataEvent::data_t bytedata = re.GetBlock(block);
    
	std::vector<short> timepixdata(bytedata.size()/2);

	// convert the byte sequence to lcio data
	for (unsigned int i = 0; i < (bytedata.size()/2); i++) {
	    // the byte sequence is little endian
	    timepixdata[i] = (bytedata[2*i] << 8) | bytedata[2*i+1];
	}
	
	lcio::TrackerRawDataImpl * timepixlciodata = new lcio::TrackerRawDataImpl;
	timepixlciodata->setCellID0(0);
	timepixlciodata->setCellID1(re.GetID(block));
	timepixlciodata->setADCValues(timepixdata);
	
	try{
	    // if the collection is already existing add the current data
	    lcio::LCCollectionVec * timepixCollection = dynamic_cast<lcio::LCCollectionVec *>(le.getCollection("TimePixRawData"));
	    timepixCollection->addElement(timepixlciodata);
	}
	catch(lcio::DataNotAvailableException &)
	{
	    // collection does not exist, create it and add it to the event
	    lcio::LCCollectionVec * timepixCollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERRAWDATA);

	    // set the flags that cellID1 should be stored
	    lcio::LCFlagImpl trkFlag(0) ;
	    trkFlag.setBit( lcio::LCIO::TRAWBIT_ID1 ) ;
	    timepixCollection->setFlag( trkFlag.getFlag() );

	    timepixCollection->addElement(timepixlciodata);
	    
	    le.addCollection(timepixCollection,"TimePixRawData");
	}
    }// for (block)

    return true;
  }
#else
  bool TimepixConverterPlugin::GetLCIOSubEvent(lcio::LCEvent &, const eudaq::Event &) const {
    return false;
  }
#endif

  /**
   * Converter for the special Timepix Begin Of Run Event
   */

 class TimepixBoreConverterPlugin : public DataConverterPlugin {

  public:
   virtual bool GetLCIOSubEvent(lcio::LCEvent & lcioBore, const eudaq::Event & eudaqBore) const;

  private:
    /** The private constructor. The only time it is called is when the
     *  one single instance is created, which lives within the object.
     *  It calls the DataConverterPlugin constructor with the
     *  accoring event type string. This automatically registers
     *  the plugin to the plugin manager.
     */
   TimepixBoreConverterPlugin() : DataConverterPlugin(Event::str2id("_TXB")) {}

    /** The one single instance of the TimepixConverterPlugin.
     *  It has to be created in the .cc file
     */
    static TimepixBoreConverterPlugin const m_instance;
  };

  TimepixBoreConverterPlugin const TimepixBoreConverterPlugin::m_instance;


#if USE_LCIO &&  USE_TPCCONDDATA
  bool TimepixBoreConverterPlugin::GetLCIOSubEvent(lcio::LCEvent & lcioBore, 
						   const eudaq::Event & eudaqBore) const 
  {
    //try to cast the eudaq event to TimepixBore
    eudaq::TimepixBore const & tpx_bore = dynamic_cast<eudaq::TimepixBore const &>(eudaqBore);
    //    if (tpx_bore==0)
    //  EUDAQ_THROW("could not cast eduaq::Event to TimepixBore");

    tpcconddata::TimePixParameters * timepixParams = 
        new tpcconddata::TimePixParameters( tpx_bore.GetNChips() );
    
    timepixParams->setChipID (tpx_bore.GetModuleID() ); // In Gear/ LCIO: The chip is the module,
                                                        // the pixels are the pads
    timepixParams->setType ( tpx_bore.GetMPXType() ); 
    timepixParams->setClock ( tpx_bore.GetClock() );
    timepixParams->setShutterEndTime ( tpx_bore.GetTimeToEndOfShutter() );

    static const size_t maxDacsPerChip = 16; // the size hardcoded in the TimePixParameters class
    size_t nDacsPerChip = tpx_bore.GetDACValues().size() / tpx_bore.GetNChips();

    if ( nDacsPerChip > maxDacsPerChip )
    {
      EUDAQ_WARN(std::string("Number of DACs in TimepixBore is > 16. Truncating when converting to LCIO"));
    }

    // copy the dac values to an array with 16 integers per chip
    // for each chip the index is chipIndex*maxDacsPerChip + dacIndex
    int * dacValues = new int[maxDacsPerChip * tpx_bore.GetNChips() ];

    // fill the array 
    for ( size_t chipIndex = 0; chipIndex < static_cast<size_t>(tpx_bore.GetNChips()) ; ++chipIndex)
    {
        for ( size_t dacIndex = 0; dacIndex < maxDacsPerChip ; ++dacIndex)
	{
	  if ( dacIndex < nDacsPerChip )
	  {
	    // fill in the dac value. Count 16 for the target index and nDacsPerChip for the source index
	    dacValues[chipIndex*maxDacsPerChip + dacIndex] = 
	      tpx_bore.GetDACValues()[chipIndex*nDacsPerChip + dacIndex];
	  }
	  else //fill additional target values with 0
	  {
	    dacValues[chipIndex*maxDacsPerChip + dacIndex] = 0 ;
	  }
	}
    }

    // set the dac values for each chip
    for ( size_t chipIndex = 0; chipIndex < static_cast<size_t>( tpx_bore.GetNChips()) ; ++chipIndex)
    {
      timepixParams->setDACs(chipIndex, &dacValues[chipIndex*maxDacsPerChip] );
    }

    try
    {
      // if the collection is already existing add the current data
      lcio::LCCollectionVec * timepixParametersCollection = 
	dynamic_cast<lcio::LCCollectionVec *>(lcioBore.getCollection(tpcconddata::TimePixParameters::getDefaultColName()));
      timepixParametersCollection->addElement(timepixParams);
    }
    catch(lcio::DataNotAvailableException &)
    {
	  // collection does not exist, create it and add it to the event
      lcio::LCCollectionVec * timepixParametersCollection = 
	new lcio::LCCollectionVec(lcio::LCIO::LCGENERICOBJECT);
      
      timepixParametersCollection->addElement(timepixParams);
      
      lcioBore.addCollection(timepixParametersCollection,tpcconddata::TimePixParameters::getDefaultColName());
      
  }
	    
    return true;
  }
#else // no LCIO / TPCCondData
  bool TimepixBoreConverterPlugin::GetLCIOSubEvent(lcio::LCEvent & , 
						   const eudaq::Event & ) const 
  {
    EUDAQ_WARN("TimepixBoreConverterPlugin compiled without TPCConddata. You will not have the Timepix conditions data in your LCIO file!");
    return false;
  }
#endif

} //namespace eudaq
