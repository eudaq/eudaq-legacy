#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Exception.hh"
#include "eudaq/Logger.hh"
#include "UCharBigEndianVec.hh"

#if USE_LCIO
#  include <IMPL/LCEventImpl.h>
#  include <IMPL/TrackerRawDataImpl.h>
#  include <IMPL/LCCollectionVec.h>
#  include <EVENT/LCIO.h>
#  include <EVENT/LCEvent.h>
#  include <IMPL/LCFlagImpl.h>
#  include <Exceptions.h>
#  include <lcio.h>
#endif

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

#include "AltroConverterPlugin.hh"
#include "AltroConverterHelper.hh"

#define NDEBUG

namespace eudaq
{
AltroConverterPlugin const AltroConverterPlugin::m_instance;
AltroConverterPlugin::~AltroConverterPlugin()
{
	delete _eventheader; _eventheader = 0;
}

#if USE_LCIO
bool AltroConverterPlugin::GetLCIOSubEvent( lcio::LCEvent & lcioevent , eudaq::Event const & eudaqevent) const
{
    //try to cast the eudaq event to RawDataEvent
    eudaq::RawDataEvent const & rawdataevent = dynamic_cast<eudaq::RawDataEvent const &>(eudaqevent);

    // set the flag whether altro words are in reversed order
    // this depends on the format version
    // odd formatversions > 410 are reversed
    int formatversion = atoi( eudaqevent.GetTag("Data format version","0" ).c_str() );

    // note: atoi will return 0 if it fails, so formatversion is 0 if the version was not
    // set or was not an integer. Throw an exception in this case
    if ( formatversion == 0 )
    	EUDAQ_THROW(std::string("AltroConverterPlugin::GetLCIOEvent: Error") +
    			" data format version not set correctly");

    bool altrowordsreversed;

    if ( (formatversion > 410) && ((formatversion % 2) == 0) )
    {
    	//      std::cout << "reading event in reversed order"<< std::endl;
    	// if formatversions from 4.1.0 have the last digit set to 1
    	// the order of the altro words is reversed in each rvu block
    	altrowordsreversed =true;
    }
    else
    {
    	// std::cout << "reading event in normal order"<< std::endl;
    	altrowordsreversed =false;
    }
    
    lcio::LCCollectionVec * altrocollection = new lcio::LCCollectionVec(lcio::LCIO::TRACKERRAWDATA);

    // set the flags to allow cellID1 to be stored
    lcio::LCFlagImpl trkFlag(0) ;
    trkFlag.setBit( lcio::LCIO::TRAWBIT_ID1 ) ;
    altrocollection->setFlag( trkFlag.getFlag() );

    try 
    {
    	// loop all data blocks
    	for (size_t block = 0 ; block < rawdataevent.NumBlocks(); block++)
    	{
    		std::vector<unsigned char> bytedata = rawdataevent.GetBlock(block);
    		UCharBigEndianVec altrodatavec(bytedata, altrowordsreversed);

    		//Contains the total size of the event expressed in number of 32 bit words excluding the field containing the header itself
    		unsigned int eventlength;
    		//Contains the total size of the event header expressed in number of 32 bit words excluding the field containing the header itself
    		unsigned int headerlength;

    		//Here we begin the conversion of the event itself
    		//The switch clause is used to choose the different decoding units to be used depending on the different data format
    		//At the moment there are two version:
    		//Version 4.2.0 and lower with the ALTRO 40 bit datawords directly streamed in the 32 bit RCU output
    		//Version 4.2.0 with dataformat 1, which is recognized as formatversion 421 were 3 10 bit ALTRO datawords
    		//are contained in a 32 bit RCU word with 2 bits used as padding

    		switch(formatversion)
    		{

    		case(420):
    		case(410):
    		case(400):
    		case(200):
    		{
    			// test integrity
    			eventlength = altrodatavec.Get32bitWord(0);
				if ( eventlength+1 != altrodatavec.Size())
				{
					std::cout << "event length = "<<( eventlength+1)*4<< " data size " << altrodatavec.Size() << std::endl;
					EUDAQ_WARN("AltroConverterPlugin::GetLCIOEvent: Wrong event length given in raw data");
					}

				headerlength = altrodatavec.Get32bitWord(1);
				//	if ( (headerlength == 4) )
				//	{
				//	    EUDAQ_WARN("AltroConverterPlugin::GetLCIOEvent: Suspicious header length given in raw data");
				//	    headerlength = 5; // this should be the correct size
				//	}

				// the third word is the block ID, it has to be 0x22222222
				unsigned int blockID = altrodatavec.Get32bitWord(2);
				if (blockID != 0x22222222)
				{
					EUDAQ_THROW("AltroConverterPlugin::GetLCIOEvent: Error reading event header");
					}

				// read the TLU event number (7th word)
				unsigned int tlu_eventnumber = 0;

				// the 7th bit in the header only exists from data format version 4.1 on,
				// which has header length > 4)
				if (headerlength > 4)
					tlu_eventnumber = altrodatavec.Get32bitWord(6);
				if (tlu_eventnumber != 0)
				{
					// check consistency
					// if ( lcioevent.getEventNumber() != tlu_eventnumber  )
					// what to do ?
					}
	  
				// the start position of the rcu block in 32bit bords
				// the two extra words are the event length and the header length, which are not counted
				// in the length word
				unsigned int rcublockstart = headerlength + 2 ;
				while ( rcublockstart < altrodatavec.Size() )
				{
					// the first word in the rcu block is its length
					unsigned int rcublocklength = altrodatavec.Get32bitWord(rcublockstart);
					unsigned int rcuID =  altrodatavec.Get32bitWord(rcublockstart + 1);
					//	    std::cout << "DEBUG: rcublocklength = "<<rcublocklength<< " at rcublockstart"
					//		      << rcublockstart << std::endl;

					// check that we have the correct position of the rcu block, third word has to be 0xFFFFFFFF
					if (  altrodatavec.Get32bitWord(rcublockstart +2 ) != 0xFFFFFFFF )
					{
						std::cerr << "invalid rcu identifier 0x"<<std::hex << altrodatavec.Get32bitWord(rcublockstart +2 )
						<<std::dec << std::endl;
						EUDAQ_THROW("Invalid RCU identifier");
						}

					// the rcu block ends with the trailer,
					// the last trailer word contains the trailer length (bits [6:0] )
					unsigned int rcutrailerlength =  altrodatavec.Get32bitWord(rcublockstart + rcublocklength )
							& 0x7F ;
					//	    std::cout << "DEBUG: rcutrailerlength "<<rcutrailerlength << std::endl;
	      
					// the number of 40 bit words is the first entry in the rcu trailer
					unsigned int n40bitwords = altrodatavec.Get32bitWord(rcublockstart
							+ rcublocklength
							- rcutrailerlength + 1);
					//	    std::cout << "DEBUG: rcu block contains "<<n40bitwords<<" 40bit words"<< std::endl;
	      
					// read the sequence of 40 bit altro words backward. It is made up of blocks
					// ending with a trailer word

					// the position of the next trailer word to read
					// use a signed int, it will become negative if there are no more words to read
					int altrotrailerposition = n40bitwords-1;
					//	    std::cout << "DEBUG: altrotrailerposition" << altrotrailerposition << std::endl;

					//	    // dump the 10 bit hex data
					//	    for (unsigned int i=0; i < n40bitwords ; i++)
					//	    {
					//		std::cout << std::hex << std::setfill('0')
					//			  << std::setw(10)
					//			  << altrodatavec.Get40bitWord(i, n40bitwords,  rcublockstart + 10 ) << "\t"
					//			  << std::setw(3)
					//			  << altrodatavec.Get10bitWord(4*i+3, n40bitwords, rcublockstart + 10 )<<" "
					//			  << std::setw(3)
					//			  << altrodatavec.Get10bitWord(4*i+2, n40bitwords, rcublockstart + 10 )<<" "
					//			  << std::setw(3)
					//			  << altrodatavec.Get10bitWord(4*i+1, n40bitwords, rcublockstart + 10 )<<" "
					//			  << std::setw(3)
					//			  << altrodatavec.Get10bitWord(4*i  , n40bitwords, rcublockstart + 10 )
					//			  <<std::dec<<std:: endl;
					//	    }

					//	    // dump the 40 bit hex data
					//	    std::cout <<"blip"<<std::endl;
					//	    for (unsigned int i=0; i <= n40bitwords ; i++)
					//	    {
					//		std::cout << std::hex << std::setfill('0')
					//			  << std::setw(10)<< altrodatavec.Get40bitWord(i, rcublockstart + 10 )
					//			  <<std::dec<<std:: endl;
					//	    }

					//	    std::cout <<"blub"<<std::endl;
					//	    // dump the 32 bit hex data
					//	    for (unsigned int i= rcublockstart ; i <= rcublockstart + rcublocklength ; i++)
					//	    {
					//		std::cout << std::hex << std::setfill('0')
					//			  << std::setw(8)<< altrodatavec.Get32bitWord(i)
					//			  <<std::dec<<std:: endl;
					//	    }

					// helper variable to detect endless loops
					int previous_altrotrailer = -1;
					while (altrotrailerposition > 0)
					{
						// check for endless loop
						if (altrotrailerposition == previous_altrotrailer)
						{
							throw BadDataBlockException("Endless loop, reading same altro trailer position twice.");
							}
						else
						{
							previous_altrotrailer = altrotrailerposition;
							}

						unsigned long long int altrotrailer
						= altrodatavec.Get40bitWord(altrotrailerposition, n40bitwords, rcublockstart +10 );

						// bool channel_is_broken = false;
						// test if we realy have the altro trailer word
						if ( (altrotrailer & 0xFFFC00F000ULL) != 0xAAA800A000ULL )
						{
							throw BadDataBlockException("Invalid Altro trailer word");
							//    if ( (altrotrailer & 0xFFFC00F000ULL) == 0xABB800A000ULL )
							//    {
							//	  channel_is_broken = true;
							//    }
							//    else // now something really went wrong, and we cannot recover
							//    {
							//	  std::cerr << "error wrong altro trailer word 0x"<< std::hex << altrotrailer << std::dec << std::endl;
							//	  EUDAQ_THROW("Invalid Altro trailer word");
							//    }
							}

						// even if the channel is broken we have to continue and read the number of 10bit words contine the event
						unsigned int n10bitwords = (altrotrailer & 0x3FF0000) >> 16;
						unsigned int channelnumber = altrotrailer & 0xFFF;
		  
						//		std::cout <<"DEBUG: altro block on channel "<< channelnumber <<" contains "
						//			  <<n10bitwords << " 10bit words"<< std::endl;
						// loop all pulse blocks, starting with the last 10 bit word

						int nfillwords =  (4 - n10bitwords%4)%4; // the number of fill words to complete the 40 bit words
						int index10bit = (altrotrailerposition*4) - nfillwords -1;

						//if (channel_is_broken)
						//  {
						//    std::cout << "skipping broken altro block on channel " << channelnumber << " in event " << lcioevent.getEventNumber() << std::endl;
						//  }
						//else // alto block is ok process it
						{
							int previous_index10bit = -1;

							while (index10bit > (altrotrailerposition*4) - static_cast<int>(n10bitwords + nfillwords) )
							{
								if ( (altrotrailerposition*4) - static_cast<int>(n10bitwords + nfillwords) < 0 )
								{
									throw BadDataBlockException("Trying to read backward to negative 10bit index.");
									}

								// check for endless loop
								if (index10bit == previous_index10bit)
								{
									std::cout << "trying to read 10bit index " << index10bit << " twice"<<std::endl;
									std::cout << std::hex << "altrotrailer " << altrotrailer << std::dec << std::endl;
									throw BadDataBlockException("Endless loop, reading same 10bit index twice.");
									}
								else
								{
									previous_index10bit = index10bit;
									}

								// length of this data record, it also counts this length word
								// and the timestamp word, so the number of data samples is length - 2
								unsigned short length    = altrodatavec.Get10bitWord( index10bit, n40bitwords,
										rcublockstart +10);
								// the length word also coints itself and the timestamp
								// so number of data samples is two less
								unsigned short ndatasamples = length - 2;
								// timestamp is the next to last word
								unsigned short timestamp = altrodatavec.Get10bitWord( index10bit - 1, n40bitwords,
										rcublockstart +10);

								//		    std::cout <<"DEBUG: found pulse with "<< ndatasamples
								//			      <<" ndatasamples at time index " << timestamp
								//			      <<" on channel " << channelnumber << std::endl;

								// the time stamp in the data stream corresponds to the last sample
								// in lcio it has to be the first sample
								timestamp -= (ndatasamples - 1);

								// create the lcio data record
								lcio::TrackerRawDataImpl *altrolciodata=new lcio::TrackerRawDataImpl;
								altrolciodata->setCellID0(channelnumber);
								//		    std::cout << "rcu ID is " << rcuID<< std::endl;
								altrolciodata->setCellID1(rcuID);
								altrolciodata->setTime(timestamp);

								//		    std::cout <<"DEBUG: found pulse with "<< ndatasamples
								//			      <<" ndatasamples at time index " << timestamp
								//			      <<" on channel " << channelnumber << std::endl;

								// fill the data samples into a vactor and add it to the lcio raw data
								lcio::ShortVec datasamples( ndatasamples );
								for ( unsigned int sample = index10bit - length + 1 , i = 0 ;
										sample < static_cast<unsigned int>(index10bit) - 1 ;
										sample ++ , i++  )
								{
				     					datasamples[ i ] = altrodatavec.Get10bitWord( sample, n40bitwords,
											rcublockstart +10 );
									}
								altrolciodata->setADCValues(datasamples);

								// add the TrackerRawData to the lcio collection
								altrocollection->addElement(altrolciodata);

								// set index to the next 10 bit word to read
								index10bit -= length;
								} // while (index10bit > 0)
							}// if channel_is_broken

						// calculate the number of 40bit words.
						// It is n10bitwords/4, rounded up (so we have to perform a floating point division)
						// plus the altro trailer word.
						unsigned int n40bitwords_in_altroblock = static_cast<unsigned int>(
								std::ceil(static_cast<double>(n10bitwords)/4.)+1);

						// calculate the position of the next altro block trailer word
						altrotrailerposition -= n40bitwords_in_altroblock;
						} // while (altrotrailerposition > 0)

					// calculate the position of the next rcu block
					rcublockstart += rcublocklength +1;
					} // while ( rcublockstart < (bytedata.size() / 4 ))

				break;
				}// End of switch case 420 410 400 200

    		case 421:
    		{
    			// Testing the correctness of the event header
    			eventlength = altrodatavec.Get32bitWord(0);
    			if ( eventlength+1 != altrodatavec.Size())
    			{
    				std::stringstream message;
    				message << "AltroConverterPlugin::GetLCIOEvent: Wrong event length given in raw data" << std::endl;
    				message << "Event length = "<<( eventlength+1)*4<< ". Data size " << altrodatavec.Size();
    				EUDAQ_ERROR(message.str());
    				throw BadDataBlockException(message.str());
    			}

    			// Filling the Event Header class and the LCIO Event with the header data
#ifndef NDEBUG
    			std::cout<< "Beginning to decode the Event Header" << std::endl;
#endif
    			_eventheaderstart = altrodatavec.PointDataWord(1);
    			_eventheader = new AltroEventHeader(_eventheaderstart, formatversion);
    			_eventheader->FillLCIOEvent(lcioevent);

#ifndef NDEBUG
    			std::cout<< "Event Header decoding complete." << std::endl;
    			std::cout << "Beginning RCUBlock Decoding" << std::endl;
#endif

    			std::vector<unsigned char>::const_iterator RCUBlockstart = _eventheader->End();
    			do
    			{
    				if(_RCUBlock != 0)
    				{
    					delete _RCUBlock;
    				}
    				try{
    					_RCUBlock = new AltroRCUBlock(RCUBlockstart, formatversion);
    				}catch(AltroConverterPlugin::BadDataBlockException &e){
    					std::stringstream message;
    					message << e.what() << std::endl;
    					message << "Exception while decoding the RCU Block. Skipping this event";
    					delete altrocollection;
    					return false;
    				}catch(std::exception &e){
    					std::cout << e.what();
    				}

        			//If the RCU Block goes out of bound, that could indicate an improper end of the file
        			if(_RCUBlock->End() > altrodatavec.end())
        			{
        				std::stringstream message;
        				message << "AltroConverterPlugin::GetLCIOEvent: RCUBlock going out of the event bounds" << std::endl;
        				EUDAQ_ERROR(message.str());
        				throw BadDataBlockException(message.str());
        			}
        			RCUBlockstart = _RCUBlock->End();
        			//Now the construction is ended and the input data has been elaborated
        			//now I can fill the altrocollection
        			try{
        				_RCUBlock->FillLCIODataCollection(altrocollection);
        			}catch(AltroConverterPlugin::BadDataBlockException &e){
    					std::stringstream message;
    					message << e.what() << std::endl;
    					message << "Exception while filling the AltroCollection. Skipping this event";
    					delete altrocollection;
    					return false;
    				}catch(std::exception &e){
    					std::cout << e.what();
    				}
    			    lcioevent.addCollection(altrocollection,"AltroRawData");
    			#ifndef NDEBUG
    						std::cout << "DEBUG: LCIO Collection successfully added." << std::endl << std::endl;
    			#endif
        			delete _RCUBlock;_RCUBlock=0;
    			    return true;
    			}
    			while(RCUBlockstart < altrodatavec.end());


    			break;

    		}//case 421


    		default:
    		{
    			std::stringstream message;
    			message << "Dataformat " << formatversion << " non recognized, data conversion impossible";
    			throw BadDataBlockException(message.str());
    		}

    		}//End of switch clause

    	}// for (eudaq data block)
#ifndef NDEBUG
			std::cout << "DEBUG: Event " << eudaqevent.GetEventNumber() << " conversion complete"<< std::endl;
#endif
    }// try
    catch (BadDataBlockException &e)
    {
      std::cout << "Event " <<  lcioevent.getEventNumber() << " contains bad data block, skipping it " 
		<< e.what()
		<< std::endl;
	delete altrocollection;
	return false;
    }

    // If the collection is empty, delete the empty collection and event
    // and return 0
    if ( altrocollection->getNumberOfElements() == 0 )
    {
#ifndef NDEBUG
			std::cout << "DEBUG: LCIO Collection empty. No Collection added" << std::endl << std::endl;
#endif
	delete altrocollection;
	return false;
    }
    
    // If the collection is not empty, add the collection to the event and return the event
    lcioevent.addCollection(altrocollection,"AltroRawData");
#ifndef NDEBUG
			std::cout << "DEBUG: LCIO Collection successfully added." << std::endl << std::endl;
#endif
    return true;
}
#else // if USE_LCIO
bool AltroConverterPlugin::GetLCIOSubEvent( lcio::LCEvent & , eudaq::Event const & ) const
{
    return false;
}
#endif // IF USE_LCIO

bool AltroConverterPlugin::GetStandardSubEvent( StandardEvent &, eudaq::Event const & ) const
{
//    StandardEvent * se = new StandardEvent;
//    se->b = eudaqevent->GetEventNumber ();
//
//    return se;
    return false;
}

} //namespace eudaq
