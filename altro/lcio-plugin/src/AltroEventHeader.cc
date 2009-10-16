/*
 * AltroEventHeader.cc
 *
 *  Created on: Oct 13, 2009
 *      Author: caiazza
 */

#include "AltroConverterHelper.hh"

#include <stdexcept>
#include <iostream>

#include "eudaq/Exception.hh"
#include "eudaq/Logger.hh"
#include "AltroConverterPlugin.hh"

#define NDEBUGEVNHEAD
#define NDEBUG
#include <assert.h>

namespace eudaq
{

// Here I declare the value of some static constant used in the code

const unsigned int AltroEventHeader::RIGHTBLOCKID421 = 0x22222222;
const unsigned int AltroEventHeader::RIGHTHEADERLENGTH421 = 5;

AltroEventHeader::AltroEventHeader(std::vector<unsigned char>::const_iterator & headerstart, int & formatversion)
	: _headerstart(headerstart), _formatversion(formatversion)
{
	//The Constructor tests if the header is meaningful
	try{
		TestIntegrity();
		DecodeHeader();
	} catch(AltroConverterPlugin::BadDataBlockException &e)
	{
		std::stringstream message;
		message << e.what() << std::endl;
		message << "Exception caught in the event header";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}

#ifndef NDEBUGEVNHEAD
	std::cout << "Event number: "<< _eventNumber << std::endl;
	std::cout << "The Event header length is " << _headerlength << std::endl;
	std::cout << "_DBOXEVTN: " << _DBOXEVTN << std::endl;
	std::cout << "_DBOXTIME: " << _DBOXTIME << std::endl;
	std::cout << "_TLUEVTN: " << _TLUEVTN << std::endl;
#endif

}

void AltroEventHeader::TestIntegrity() const
{
	switch (_formatversion)
	{
	case 421:
		if(Get32bitBigEndian(_headerstart) != RIGHTHEADERLENGTH421)
		{
			std::stringstream message;
			message << "Altro Event header length = "<< std::hex <<Get32bitBigEndian(_headerstart) << std::dec << '.' <<std::endl;
			message << "Version " << _formatversion << " event header length should be" << std::hex << RIGHTHEADERLENGTH421 << std::dec << '.'<<  std::endl;
			message << "Possible version mismatch. Trying to continue";
			EUDAQ_WARN(message.str());
		}
		if(Get32bitBigEndian(_headerstart + 4) != RIGHTBLOCKID421)
		{
			std::stringstream message;
			message << "Altro Event header Block Identifier = 0x"<< std::hex << Get32bitBigEndian(_headerstart + 1) << '.' <<std::endl;
			message << "Block ID should be" << RIGHTBLOCKID421 << '.'<< std::dec <<  std::endl;
			EUDAQ_ERROR(message.str());
			throw AltroConverterPlugin::BadDataBlockException(message.str());
		}
		break; //Case 421

	default:
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());

	} // End format version switch
}

void AltroEventHeader::DecodeHeader()
{
	switch(_formatversion)
	{
	case 421:
		_headerlength = Get32bitBigEndian(_headerstart);
		_eventNumber = Get32bitBigEndian(_headerstart + 8);
		_DBOXEVTN = Get32bitBigEndian(_headerstart + 12);
		_DBOXTIME = Get32bitBigEndian(_headerstart + 16);
		_TLUEVTN = Get32bitBigEndian(_headerstart + 20);
		_headerend = (_headerstart + 24);
		break;

	default:
		std::stringstream message;
		message << "Data Format Version "<< _formatversion << "not recognized.";
		throw AltroConverterPlugin::BadDataBlockException(message.str());
	}
}

void AltroEventHeader::FillLCIOEvent(lcio::LCEvent & lcioevent) const
{

}

const std::vector<unsigned char>::const_iterator & AltroEventHeader::End() const
{
	const std::vector<unsigned char>::const_iterator & retval = _headerend;
	return retval;
	//return _headerend;
}

//-------------End of AltroEventHeader Member functions---------------
}
