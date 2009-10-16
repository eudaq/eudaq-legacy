/*
 * AltroConverterHelper.cc
 *
 *  Created on: Oct 1, 2009
 *      Author: caiazza
 */

#include "AltroConverterHelper.hh"

//STD Includes
#include <stdexcept>
#include <iostream>

//EUDAQ Includes
#include "eudaq/Exception.hh"
#include "eudaq/Logger.hh"
#include "AltroConverterPlugin.hh"

//LCIO Includes
#include <IMPL/TrackerRawDataImpl.h>
#include <lcio.h>

//The setting of this macro variables disable the debug features of the code

#define NDEBUGRCUHEAD
#define NDEBUGRCUTRAIL
#define NDEBUGALTRODATA
#define NDEBUG
#include <assert.h>

namespace eudaq
{

//------------------ Begin of AltroRCUHeader ---------------

const unsigned int AltroRCUHeader::RCUHEADLENGTH421 = 8;
const unsigned int AltroRCUHeader::RCUHEADID421 = 0xFFFFFFFF;

AltroRCUHeader::AltroRCUHeader(std::vector<unsigned char>::const_iterator & blockStart, int & formatversion)
	: _blockStart(blockStart), _formatversion(formatversion)
{
	//The Constructor tests if the header is meaningful
	try{
		TestIntegrity();
		Decode();
	} catch(AltroConverterPlugin::BadDataBlockException &e){
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught in the RCU Header";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}

#ifndef NDEBUGRCUHEAD
	std::cout << "RCUHeader Decoding Complete" << std::endl;
#endif

}

void AltroRCUHeader::TestIntegrity() const
{
	switch (_formatversion)
	{
	case 421:
	{
		if(Get32bitBigEndian(_blockStart) != RCUHEADID421)
		{
			std::stringstream message;
			message << "BlockID for the RCU Header for data format "<< _formatversion << "is not valid." << std::endl;
			message << "BlockID expected is " << RCUHEADID421 << ". Actual value is " << Get32bitBigEndian(_blockStart) << '.' << std::endl;
			throw AltroConverterPlugin::BadDataBlockException(message.str());
		}
		break;
	}//end 421
	default:
	{
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}//end default
	}//end switch
}

void AltroRCUHeader::Decode()
{
	switch (_formatversion)
	{
	case 421:
	{
		_blockEnd = _blockStart +40;
		break;
	}//end 421
	default:
	{
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}//end default
	}//end switch
}

const std::vector<unsigned char>::const_iterator & AltroRCUHeader::End()
{
	const std::vector<unsigned char>::const_iterator & retval = _blockEnd;
	return retval;
}

void AltroRCUHeader::FillLCIOEvent(lcio::LCEvent & lcioevent) const
{

}
//--------------------End of AltroRCUHeader ----------------------
//--------------------Begin of AltroRCUTrailer -------------------

const unsigned int AltroRCUTrailer::TRAILERWORDIDMASK421 = 0xC << 28;
const unsigned int AltroRCUTrailer::TRAILERLENGTH421 = 9;
const unsigned int AltroRCUTrailer::PARIDMASK421 = 0x3C << 24;
const unsigned int AltroRCUTrailer::PARVALUEMASK421 = 0x03FFFFFF;

AltroRCUTrailer::AltroRCUTrailer(std::vector<unsigned char>::const_iterator & blockStart, int & formatversion)
	: _blockStart(blockStart), _formatversion(formatversion)
{
#ifndef NDEBUGRCUTRAIL
	std::cout<< "DEBUG: Decoding the RCU Trailer" << std::endl;
#endif
	switch(_formatversion)
	{
	case 421:
	{
		_blockEnd = (_blockStart + 36);
		break;
	}
	default:
	{
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}//end default
	}

	//The Constructor tests if the header is meaningful
	try{
		TestIntegrity();
		Decode();
	} catch(AltroConverterPlugin::BadDataBlockException &e)
	{
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught in the RCU Trailer";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}

#ifndef NDEBUGRCUTRAIL
	std::cout<< "DEBUG: RCU Trailer Decoded" << std::endl;
#endif
}

AltroRCUTrailer::~AltroRCUTrailer()
{

}

void AltroRCUTrailer::TestIntegrity() const
{
	switch(_formatversion)
	{
	case 421:
	{
		#ifndef NDEBUGRCUTRAIL
		unsigned int temp = ((Get32bitBigEndian(_blockEnd - 4) & TRAILERWORDIDMASK421) >> 30);
		std::cout<< "Last Trailer Word ID is: 0x" << std::hex << temp << std::dec << std::endl;
		#endif

		//Check the Word ID of the final Trailer Word
		if(((Get32bitBigEndian(_blockEnd - 4) & TRAILERWORDIDMASK421) >> 30) != 3)
		{
			unsigned int temp = ((Get32bitBigEndian(_blockEnd - 4) & TRAILERWORDIDMASK421) >> 30);
			std::stringstream message;
			message << "Unexpected Last Trailer Word ID." << std::endl;
			message << "Expected value was 0x" << std::hex << 3 << ". Actual value is " << temp << std::dec << std::endl;
			throw AltroConverterPlugin::BadDataBlockException(message.str());
		}
		//Check the Parameter ID of the Final Trailer Word
		if(((Get32bitBigEndian(_blockEnd - 4) & PARIDMASK421) >> 26) != 0x8)
		{
			unsigned int temp = ((Get32bitBigEndian(_blockEnd - 4) & PARIDMASK421) >> 26);
			std::stringstream message;
			message << "Unexpected Last Trailer Word Parameter Code." << std::endl;
			message << "Expected value was 0x" << std::hex << 8 << ". Actual value is 0x" << temp << std::dec << std::endl;
			throw AltroConverterPlugin::BadDataBlockException(message.str());
		}
		//Check the Length of the Trailer is as expected
		if((Get32bitBigEndian(_blockEnd - 4) & 0x7F) != TRAILERLENGTH421)
		{
			unsigned int temp = (Get32bitBigEndian(_blockEnd - 4) & 0x7F);
			std::stringstream message;
			message << "Unexpected Trailer Length." << std::endl;
			message << "Expected value was " << std::hex << TRAILERLENGTH421 << ". Actual value is " << temp << std::dec << std::endl;
			EUDAQ_WARN(message.str());
		}
		for(std::vector<unsigned char>::const_iterator itIndex = _blockStart; itIndex != (_blockEnd-4); itIndex+=4)
		{
			unsigned int dataword = Get32bitBigEndian(itIndex);
			unsigned int wordid = (dataword & TRAILERWORDIDMASK421) >> 30;
			if(wordid != 2)
			{
				std::stringstream message;
				message << "Unexpected Word ID in RCU Trailer." << std::endl;
				message << "Expected value was 0x" << std::hex << 2 << ". Actual value is " << wordid << std::dec << std::endl;
				EUDAQ_WARN(message.str());
			}
			int parcode = (dataword & PARIDMASK421) >> 26;
			if ((parcode*4) != std::distance(_blockStart, itIndex))
			{
				std::stringstream message;
				message << "Unexpected Parameter Code in RCU Trailer." << std::endl;
				message << "Expected value was 0x" << std::hex << std::distance(_blockStart, itIndex);
				message << ". Actual value is " << parcode << std::dec << std::endl;
				EUDAQ_WARN(message.str());
			}
		}
		break;
	}
	default:
	{
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}//end default
	}
}

void AltroRCUTrailer::Decode()
{
	switch(_formatversion)
	{
	case 421:
	{
		unsigned int dataword = Get32bitBigEndian(_blockStart);
		dataword = dataword & PARVALUEMASK421;
		_payloadLength = dataword;
#ifndef NDEBUGRCUTRAIL
		std::cout << "DEBUG: Block Payload is: " << _payloadLength << std::endl;
#endif
		dataword = Get32bitBigEndian(_blockEnd - 4);
		dataword = (dataword & 0x03FF0000) >> 16;
		_RCUFWVer = dataword;
#ifndef NDEBUGRCUTRAIL
		std::cout << "DEBUG: Firmware version is: " << _RCUFWVer << std::endl;
#endif
		break;
	}
	default:
	{
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}//end default
	}
}

unsigned int AltroRCUTrailer::RCUPayloadLength() const
{
	return _payloadLength;
}

const std::vector<unsigned char>::const_iterator & AltroRCUTrailer::End() const
{
	const std::vector<unsigned char>::const_iterator & retval = _blockEnd;
	return retval;
}

void AltroRCUTrailer::FillLCIOEvent(lcio::LCEvent & lcioevent) const
{

}
//--------------------End of AltroRCUTrailer -------------------
//--------------------Begin of AltroEventData-------------------

const unsigned int AltroEventData::DATAWORDIDMASK421 = 0xC << 28;
const unsigned int AltroEventData::SAMPLEWORDMASKRCU3X32 = 0x3FF;


AltroEventData::AltroEventData(std::vector<unsigned char>::const_iterator & blockStart, unsigned int payLoad, AltroEventData::DataVer_t version)
	:_blockStart(blockStart), _payLoad(payLoad), _version(version)
{
	_blockEnd = _blockStart + (_payLoad*4);
#ifndef NDEBUGALTRODATA
	std::cout << "Payload is " << _payLoad << std::endl;
	std::cout << "Distance from Start to End is " << std::distance(_blockStart, _blockEnd) << std::endl;
#endif

	if(!(_dataList.empty()))
	{
		_dataList.clear();
	}

	try{
		TestIntegrity();
	} catch(AltroConverterPlugin::BadDataBlockException &e)
	{
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught in the Altro data block";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}

	try{
		Decode();
	} catch(AltroConverterPlugin::BadDataBlockException &e)
	{
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught while decoding the Altro Data";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}
}

AltroEventData::~AltroEventData()
{
	_dataList.clear();
}

void AltroEventData::TestIntegrity() const
{
	switch(_version)
	{
	case RCU3X32:
	{
		if(((Get32bitBigEndian(_blockEnd - 4) & DATAWORDIDMASK421) >> 30) != 0 &&
				((Get32bitBigEndian(_blockEnd - 4) & DATAWORDIDMASK421) >> 30) != 1)
		{
			unsigned int temp = ((Get32bitBigEndian(_blockEnd - 4) & DATAWORDIDMASK421) >> 30);
			std::stringstream message;
			message << "Unexpected Last Data Word ID. Possible data corruption" << std::endl;
			message << "Expected value was 0x" << std::hex << 0 << " or 0x" << 1;
			message << ". Actual value is 0x" << temp << std::dec << std::endl;
			throw AltroConverterPlugin::BadDataBlockException(message.str());
		}
		if(((Get32bitBigEndian(_blockEnd) & DATAWORDIDMASK421) >> 30) != 2)
		{
			unsigned int temp = ((Get32bitBigEndian(_blockEnd) & DATAWORDIDMASK421) >> 30);
			std::stringstream message;
			message << "Unexpected First Trailer Word ID." << std::endl;
			message << "Expected value was 0x" << std::hex << 2 << ". Actual value is 0x" << temp << std::dec << std::endl;
			throw AltroConverterPlugin::BadDataBlockException(message.str());
		}
		break;
	}
	case UNRECOGNIZED:
	{
		std::stringstream message;
		message << "Data Format Version not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}//end default

	}
}

void AltroEventData::Decode()
{
	std::vector<unsigned char>::const_iterator dataindex = _blockStart;
	while(dataindex <_blockEnd)
	{
		AltroChHeader chHeader = AltroChHeader(Get32bitBigEndian(dataindex), _version);

#ifndef NDEBUGALTRODATA
		if(chHeader.BlockLength() > 0)
		{
		std::cout << "Current distance from the end of the block is " << std::distance(dataindex, _blockEnd) << std::endl;
		std::cout << "Channel Address is 0x" << std::hex << chHeader.ChAddress() << std::dec << std::endl;
		std::cout << "Following Blocklength is " << chHeader.BlockLength() << std::endl;
		}
#endif
		dataindex += 4;

		int blockwords = chHeader.BlockLength() / 3;
		if((chHeader.BlockLength() % 3) != 0)
		{
			blockwords ++;
		}

		std::vector<short> sampleList = std::vector<short>();
		sampleList.reserve((blockwords * 3));

		for(int i=0; i < (blockwords - 1); i++ )
		{
			unsigned int word = Get32bitBigEndian(dataindex);
			if(((word & DATAWORDIDMASK421) >> 30) != 0)
			{
				unsigned int temp = ((word & DATAWORDIDMASK421) >> 30);
				std::stringstream message;
				message << "Unexpected Altro Sample Word ID." << std::endl;
				message << "Expected value was 0x" << std::hex << 0 << ". Actual value is 0x" << temp << std::dec << std::endl;
				throw AltroConverterPlugin::BadDataBlockException(message.str());
			}

			short word1 = (word & (SAMPLEWORDMASKRCU3X32 << 20)) >> 20 ;
			sampleList.push_back(word1);
			short word2 = (word & (SAMPLEWORDMASKRCU3X32 << 10)) >> 10 ;
			sampleList.push_back(word2);
			short word3 = word & (SAMPLEWORDMASKRCU3X32) ;
			sampleList.push_back(word3);
			dataindex += 4;
		}//end for

		if(blockwords != 0) //Only activate this cycle if there are datawords
		{
			unsigned int word = Get32bitBigEndian(dataindex);
			switch(chHeader.BlockLength() % 3)
			{
			case 0:
			{
				short word1 = (word & (SAMPLEWORDMASKRCU3X32 << 20)) >> 20 ;
				sampleList.push_back(word1);
				short word2 = (word & (SAMPLEWORDMASKRCU3X32 << 10)) >> 10 ;
				sampleList.push_back(word2);
				short word3 = word & (SAMPLEWORDMASKRCU3X32) ;
				sampleList.push_back(word3);
				break;
			}
			case 2:
			{
				short word1 = (word & (SAMPLEWORDMASKRCU3X32 << 20)) >> 20 ;
				sampleList.push_back(word1);
				short word2 = (word & (SAMPLEWORDMASKRCU3X32 << 10)) >> 10 ;
				sampleList.push_back(word2);
				break;
			}
			case 1:
			{
				short word1 = (word & (SAMPLEWORDMASKRCU3X32 << 20)) >> 20 ;
				sampleList.push_back(word1);
			}
			}//end switch
			dataindex += 4;
		}//end if

		//I choose not to write in the LCIO file the data of the channels which do not contain data
		if(sampleList.size()>0)
		{
			//Because most of the adc channel contains only one set of samples is better, in this case,
			//to avoid further elaboration and directly build the single Tracker Raw Data Object
			if(sampleList.size() == *(sampleList.begin()))
			{
				//The timestamp in the LCIO format is the time of the first sample while in the
				//AltroDataFormat is the time of the last of the samples. Thus we need to perform a conversion
				short timestamp = *(sampleList.begin() + 1) - (*sampleList.begin() - 1);
				std::vector<short> datasamples = std::vector<short>();

				//Temporary variable created to implement a pass by reference function which will be more efficient
				//in the else clause of this if where the corresponding  iterator is already been created
				std::vector<short>::const_iterator itbegin = sampleList.begin();
				CreateSamplesVec(itbegin, datasamples);

				lcio::TrackerRawDataImpl *altrolciodata=new lcio::TrackerRawDataImpl;
				altrolciodata->setCellID0(chHeader.ChAddress());
				altrolciodata->setTime(timestamp);
				altrolciodata->setADCValues(datasamples);

				_dataList.push_back(altrolciodata);
			}
			//If there is more than one set of samples in the datafile I cycle across the datafile vector
			//creating small vectors containing single samplesets
			else
			{
				std::vector<short>::const_iterator index = sampleList.begin();
				while(index < sampleList.end())
				{
					//The first element of the vector is always the length of the sample just following
					//This length also counts the length element itself and the timestamp
					//The timestamp in the LCIO format is the time of the first sample while in the
					//AltroDataFormat is the time of the last of the samples. Thus we need to perform a conversion
					short timestamp = *(index+1) - (*index - 1);

					std::vector<short> datasamples = std::vector<short>();
					CreateSamplesVec(index, datasamples);

					lcio::TrackerRawDataImpl *altrolciodata=new lcio::TrackerRawDataImpl;
					altrolciodata->setCellID0(chHeader.ChAddress());
					altrolciodata->setTime(timestamp);
					altrolciodata->setADCValues(datasamples);

					_dataList.push_back(altrolciodata);

					//going to the next block of data
					index = index + *index;
				}//end of while in the sample list
			}//end of samplelist size if
		}//end of samplelist exist

#ifndef NDEBUGALTRODATA
		if(chHeader.BlockLength() > 0)
		{
			for(std::vector<short>::iterator it = sampleList.begin(); it < sampleList.end() ; it++)
			{
				std::cout << "Sample Value is " << *it << std::endl;
			}
			std::cout << "Number of blocks advanced " << blockwords << std::endl;
			std::cout << "Current size of the Tracker raw data list is: " << _dataList.size() << std::endl;
		}
#endif

		sampleList.clear();
	}// end while
}

void AltroEventData::CreateSamplesVec(std::vector<short>::const_iterator & SampleBegin, std::vector<short> & datasamples)
{
	datasamples.reserve((*SampleBegin)-2);
	for(int i = (*SampleBegin) - 1; i > 1 ; i--)
	{
		datasamples.push_back(*(SampleBegin + i));
	}
}

void AltroEventData::FillLCIODataCollection(lcio::LCCollection *altrocollection, unsigned int RCUID)
{
	try{
		for(std::list<lcio::TrackerRawDataImpl*>::iterator it = _dataList.begin(); it != _dataList.end(); it++)
		{
			(*it)->setCellID1(static_cast<int>(RCUID));
			altrocollection->addElement(*it);
		}
	}catch(AltroConverterPlugin::BadDataBlockException &e){
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception while filling the LCIO Collection with Event Data";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}catch(std::exception &e){
			std::cout << "Exception while filling the LCIO Collection with Event Data. /n" << e.what() << std::endl;
	}
}


//-----------Begin AltroChHeader---------

const unsigned int AltroEventData::AltroChHeader::ERRORBITMASKRCU3X32 = 0x2 << 28;
const unsigned int AltroEventData::AltroChHeader::CHADDRESSMASKRCU3X32 = 0xFFF;
const unsigned int AltroEventData::AltroChHeader::NRSAMPLEMASKRCU3X32 = 0x03FF0000;
AltroEventData::AltroChHeader::AltroChHeader(unsigned int  word, DataVer_t version)
{
	AssignWord(word, version);
}

void AltroEventData::AltroChHeader::AssignWord(unsigned int word, DataVer_t version)
{
	switch(version)
	{
	case RCU3X32:
	{
		if(((word & DATAWORDIDMASK421) >> 30) != 1)
		{
			unsigned int temp = ((word & DATAWORDIDMASK421) >> 30);
			std::stringstream message;
			message << "Unexpected Altro Channel Word ID." << std::endl;
			message << "Expected value was 0x" << std::hex << 1 << ". Actual value is 0x" << temp << std::dec << std::endl;
			throw AltroConverterPlugin::BadDataBlockException(message.str());
		}
		 _chAddress = word & CHADDRESSMASKRCU3X32;
		 _blockLength = (word & NRSAMPLEMASKRCU3X32) >> 16;

		if((word & ERRORBITMASKRCU3X32) == 0)
		{
			_error = false;
		} else {
			std::stringstream message;
			message << "Mismatch for channel address or block length. the RCU corrected the error." << std::endl;
			message << "Channel interested is 0x" << std::hex << _chAddress << ". Block Length is " << _blockLength << std::dec << std::endl;
			EUDAQ_WARN(message.str());
			_error = true;
		}
		break;
	}
	case UNRECOGNIZED:
	{
		std::stringstream message;
		message << "Data Format Version not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}
	}
}

//Get a 32 bit data word coded in Big Endian byte ordering and convert it in a 32 bit word Little Endian
unsigned int Get32bitBigEndian(std::vector<unsigned char>::const_iterator itDataIndex)
{
    unsigned int retval =
	static_cast<unsigned int>( *(itDataIndex) )       |
	static_cast<unsigned int>( *(itDataIndex + 1) ) <<  8 |
	static_cast<unsigned int>( *(itDataIndex + 2) ) << 16 |
	static_cast<unsigned int>( *(itDataIndex + 3) ) << 24;

    return retval;
}

}// end namespace
