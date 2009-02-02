#include "eudaq/AltroConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Logger.hh"

#include <IMPL/LCEventImpl.h>
#include <IMPL/TrackerRawDataImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <EVENT/LCIO.h>

#include <iostream>

namespace eudaq
{

AltroConverterPlugin const AltroConverterPlugin::m_altroconverterplugininstance;

UCharBigEndianVec::UCharBigEndianVec(std::vector<unsigned char> const & datavec) :
    bytedata(datavec) {}

unsigned long long int UCharBigEndianVec::Get40bitWord(unsigned int index40bit,
							  unsigned int offset32bit) const
{
    // check position in data stream
    if ( Get32bitWord(offset32bit - 8) != 0xFFFFFFFF )
	    EUDAQ_WARN("AltroConverterPlugin::Get40bitWord: Suspicious position in data stream, altro sequence does not start 8 words after rcu header");	

    unsigned int index8bit = offset32bit*4 + index40bit*5;
    unsigned long long int retval = 
	static_cast<unsigned long long int>( bytedata[ endian(index8bit)     ] )       |
	static_cast<unsigned long long int>( bytedata[ endian(index8bit + 1) ] ) <<  8 |
	static_cast<unsigned long long int>( bytedata[ endian(index8bit + 2) ] ) << 16 |
	static_cast<unsigned long long int>( bytedata[ endian(index8bit + 3) ] ) << 24 |
	static_cast<unsigned long long int>( bytedata[ endian(index8bit + 4) ] ) << 32 ;
    return retval;
}

unsigned int UCharBigEndianVec::Get32bitWord(unsigned int index32bit) const
{
    unsigned int index8bit = index32bit*4;

    unsigned int retval = 
	static_cast<unsigned int>( bytedata[ endian(index8bit)     ] )       |
	static_cast<unsigned int>( bytedata[ endian(index8bit + 1) ] ) <<  8 |
	static_cast<unsigned int>( bytedata[ endian(index8bit + 2) ] ) << 16 |
	static_cast<unsigned int>( bytedata[ endian(index8bit + 3) ] ) << 24 ;

    return retval;
}

unsigned short UCharBigEndianVec::Get10bitWord(unsigned int index10bit,
					     unsigned int offset32bit) const
{
    unsigned short y = (index10bit % 4) * 2; // how far to shift the window, i. e. relative alignment of 10-bit and 8-bit blocks
    unsigned short M = 0x03FF << y; // 10-bit mask, shifted

    unsigned int   i = index10bit * 10 / 8 + offset32bit*4; // keep external 10-bit index and internal 8-bit index in sync
    unsigned short N = (bytedata[endian(i + 1)] << 8) | bytedata[endian(i)]; // compose the 2-byte word onto which the mask will be applied
    return (N & M) >> y; // apply the mask and shift the result back to the LSB
    
}

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
//    lcioevent->setEventNumber( eudaqevent->GetEventNumber () ); done later
    lcioevent->setRunNumber(    eudaqevent->GetRunNumber () );
    lcioevent->setDetectorName( "TPC with Pads and Altro");
    lcioevent->setTimeStamp( eudaqevent->GetTimestamp() );


    lcio::LCCollectionVec * altrocollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERRAWDATA);

    // loop all data blocks
    for (size_t block = 0 ; block < rawdataevent->NumBlocks(); block++)
    {
	std::vector<unsigned char> bytedata_ = rawdataevent->GetBlockUChar(block);
	UCharBigEndianVec altrodatavec(bytedata_);

	// interpret the byte sequence

	// test integrity
	unsigned int eventlength = altrodatavec.Get32bitWord(0);
	if (eventlength != altrodatavec.Size())
	{
	    EUDAQ_WARN("AltroConverterPlugin::GetLCIOEvent: Wrong event length given in raw data");
	}

	unsigned int headerlength = altrodatavec.Get32bitWord(1);
	if ( (headerlength == 4) )
	{
	    EUDAQ_WARN("AltroConverterPlugin::GetLCIOEvent: Wrong event length given in raw data");
	    headerlength = 5; // this should be the correct size
	}

	// the third word is the block ID, it has to be 0x22222222
	unsigned int blockID = altrodatavec.Get32bitWord(2);
	if (blockID != 0x22222222)
	{
	    EUDAQ_THROW("AltroConverterPlugin::GetLCIOEvent: Error reading event header");
	}

	// read the TLU event number (7th word)
	unsigned int tlu_eventnumber = altrodatavec.Get32bitWord(6);
	if (tlu_eventnumber != 0)
	{
	       lcioevent->setEventNumber( tlu_eventnumber  );
	}
	else // tlu event number is not set, use the software number
	{
	       lcioevent->setEventNumber( eudaqevent->GetEventNumber () );
	} 

	// the start position of the rcu block in 32bit bords
	unsigned int rcublockstart = 7;
	while ( rcublockstart < altrodatavec.Size() )
	{
	    // the first word in the rcu block is its length
	    unsigned int rcublocklength = altrodatavec.Get32bitWord(rcublockstart);

	    // the rcu block ends with the trailer, 
	    // the last trailer word contains the trailer length (bits [6:0] )
	    unsigned int rcutrailerlength =  altrodatavec.Get32bitWord(rcublockstart + rcublocklength -1 ) 
                                             & 0x7F ;

	    // the number of 40 bit words is the first entry in the rcu trailer
	    unsigned int n40bitwords = altrodatavec.Get32bitWord(rcublockstart 
						    + rcublocklength 
						    - rcutrailerlength);

	    // read the sequence of 40 bit altro words backward. It is made up of blocks
	    // ending with a trailer word

	    
            // the position of the next trailer word to read
            // use a signed int, it will become negative if there are no more words to read
	    int altrotrailerposition = n40bitwords-1;

	    while (altrotrailerposition > 0)
	    {
		unsigned long long int altrotrailer 
		    = altrodatavec.Get40bitWord(altrotrailerposition, rcublockstart +10 );

		unsigned int n10bitwords = (altrotrailer & 0x3FF0000) >> 16;
		unsigned int channelnumber = altrotrailer & 0xFFF;

		int index10bit = n10bitwords - 1;
		while (index10bit > 0)
		{
		    // length of this data record, it also counts this length word
		    // and the timestamp word, so the number of data samples is length - 2
		    unsigned short length    = altrodatavec.Get10bitWord( index10bit ,
									  rcublockstart +10);
		    unsigned short ndatasamples = length - 2;
		    unsigned short timestamp = altrodatavec.Get10bitWord( index10bit - 1 ,
									  rcublockstart +10);

		    // the time stamp in the data stream corresponds to the last sample
		    // in lcio it has to be the first sample
		    timestamp -= (ndatasamples - 1);

		    // create the lcio data record
		    lcio::TrackerRawDataImpl *altrolciodata=new lcio::TrackerRawDataImpl;

		    altrolciodata->setCellID0(channelnumber);
		    altrolciodata->setCellID1(block);

		    lcio::ShortVec datasamples( ndatasamples );
		    for ( unsigned int sample = index10bit - length + 1 , i = 0 ;
			  sample < static_cast<unsigned int>(index10bit) - 1 ;
			  sample ++ , i++  )
		    {
			datasamples[ i ] = altrodatavec.Get10bitWord( sample, rcublockstart +10 );
		    }
		    altrolciodata->setADCValues(datasamples);
		    altrocollection->addElement(altrolciodata);
		} // while (index10bit > 0)

	    } // while (altrotrailerposition > 0)
	    
	} // while ( rcublockstart < (bytedata.size() / 4 ))

    }// for (block)
	
    lcioevent->addCollection(altrocollection,"AltroRawData");

    return lcioevent;
}

StandardEvent * AltroConverterPlugin::GetStandardEvent( eudaq::Event const * eudaqevent ) const
{
    StandardEvent * se = new StandardEvent;
    se->b = eudaqevent->GetEventNumber ();

    return se;
}

} //namespace eudaq
