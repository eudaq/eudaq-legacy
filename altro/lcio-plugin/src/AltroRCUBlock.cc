/*
 * AltroRCUBlock.cc
 *
 *  Created on: Oct 13, 2009
 *      Author: caiazza
 */

#include "AltroConverterHelper.hh"

//STD Include
#include <stdexcept>
#include <iostream>

//EUDAQ Include
#include "eudaq/Exception.hh"
#include "eudaq/Logger.hh"
#include "AltroConverterPlugin.hh"

//Debug Include
#define NDEBUGRCUBLOCK
#define NDEBUG
#include <assert.h>

namespace eudaq
{

//-------------Begin of AltroRCUBlock member functions----------------

const int AltroRCUBlock::MINRCULENGTH421 = 19;

AltroRCUBlock::AltroRCUBlock(std::vector<unsigned char>::const_iterator & blockStart, int & formatversion)
	: _blockStart(blockStart), _formatversion(formatversion)
{
	switch(_formatversion)
	{
	case 421:
	{
		unsigned int blockLength = Get32bitBigEndian(_blockStart);
		_blockEnd = (_blockStart + ((blockLength+1)*4));
		break;
	}
	default:
		std::stringstream message;
		message << "Exception caught in the RCU Block";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}

	//Performing the data decoding
	try{
		TestIntegrity();
		Decode();
	}catch(AltroConverterPlugin::BadDataBlockException &e){
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught in the RCU Block";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}

#ifndef NDEBUGRCUBLOCK
	std::cout << "Decoding data from the RCU " << _RCUID << std::endl;
	std::cout << "RCUBlock length: " << (Get32bitBigEndian(_blockStart)*4) << std::endl;
#endif

	//Creating the RCU Header Object.
	try{
		_RCUHeader = new AltroRCUHeader(_headerStart, _formatversion);
	}catch(AltroConverterPlugin::BadDataBlockException &e){
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught in the RCU Block";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}catch(std::exception &e){
		std::cout << e.what();
	}

	try{
		_RCUTrailer = new AltroRCUTrailer(_trailerStart, _formatversion);
	}catch(AltroConverterPlugin::BadDataBlockException &e){
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught in the RCU Block";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}catch(std::exception &e){
		std::cout << e.what();
	}

	try{
		_EventData = new AltroEventData(_dataStart, _RCUTrailer->RCUPayloadLength(), AltroEventData::RCU3X32);
	}catch(AltroConverterPlugin::BadDataBlockException &e){
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught in the RCU Block";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}catch(std::exception &e){
		std::cout << e.what() << std::endl;
	}

#ifndef NDEBUGRCUBLOCK
	std::cout << "RCU " << _RCUID << " data Decoded."<< std::endl;
#endif
}

AltroRCUBlock::~AltroRCUBlock()
{
	delete _RCUHeader; _RCUHeader=0;
	delete _RCUTrailer; _RCUTrailer=0;
}

void AltroRCUBlock::TestIntegrity() const
{
	switch (_formatversion)
	{
	case 421:
	{
		if(std::distance(_blockStart,_blockEnd) < MINRCULENGTH421)
		{
			std::stringstream message;
			message << "Length of the RCU Block is lower than the minimum for data format: "<< _formatversion << std::endl;
			message << "Minimum Length allowed is " << MINRCULENGTH421 << ". Actual length is " << std::distance(_blockStart,_blockEnd) << '.' << std::endl;
			throw AltroConverterPlugin::BadDataBlockException(message.str());
		}
		break;
	}
	default:
	{
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}
	}
}

void AltroRCUBlock::Decode()
{
	switch (_formatversion)
	{
	case 421:
	{
		_RCUID = Get32bitBigEndian(_blockStart + 4);
		_headerStart = _blockStart + 8;
		_dataStart = _blockStart + 40;
		_trailerStart = _blockEnd - 36;
		break;
	}
	default:
	{
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}
	}
}

const std::vector<unsigned char>::const_iterator AltroRCUBlock::End()
{
	//const std::vector<unsigned char>::const_iterator & retval = _blockEnd;
	return _blockEnd;
}

void AltroRCUBlock::FillLCIODataCollection(lcio::LCCollection *altrocollection)
{
	try{
	_EventData->FillLCIODataCollection(altrocollection, _RCUID);
	}catch(AltroConverterPlugin::BadDataBlockException &e){
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception while filling the LCIO Collection with Event Data";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}catch(std::exception &e){
			std::cout << "Exception while filling the LCIO Collection with Event Data. /n" << e.what() << std::endl;
	}
}

//------------------ End of AltroRCUBlock ------------------
}
