/*
 * UCharBigEndianVec.cc
 *
 *  Created on: Sep 30, 2009
 *      Author: caiazza
 */

#include "UCharBigEndianVec.hh"
#include "eudaq/Exception.hh"
#include "eudaq/Logger.hh"

#include <cstdlib>

using namespace eudaq;

UCharBigEndianVec::UCharBigEndianVec(std::vector<unsigned char> const & datavec, bool altrowordsreversed) :
			_bytedata(datavec), _dataindex(datavec.begin()) , _altrowordsreversed(altrowordsreversed)
{
    // check data integrety of the SOR format
    if (Get32bitWord(2) != 0x22222222)
	EUDAQ_THROW("Invalid header format in altro event data");
}

unsigned long long int UCharBigEndianVec::Get40bitWord(unsigned int index40bit, unsigned int n40bitwords,
		unsigned int offset32bit) const
{
	// check position in data stream
	if ( Get32bitWord(offset32bit - 8) != 0xFFFFFFFF )
		EUDAQ_WARN("AltroConverterPlugin::Get40bitWord: Suspicious position in data stream, altro sequence does not start 8 words after rcu header");

	// calculate the 8 bit offset in dependence whether the 40 bits words are in reversed order or not
	unsigned int index8bit = offset32bit*4 +
			(!_altrowordsreversed ? index40bit : n40bitwords - index40bit -1 )*5;

	// The 32  bit words have big endian order. The 40 bit word is always contained
	// in 2 consecutive 32 bit words,
	// the most significant bytes being in the second word (big endian 32 bit sequence).
	// This means the 5 bytes are in a consecutive
	// sequence in the big endian data stream, but in reverse order. We just have to pick them out
	// and place them in the long long word

	// Forunately this also works with the reversed 40 bit order, because 40 bits is always a
	// sequence of 5 bytes, whatever the other words are.

	unsigned long long int retval =
			static_cast<unsigned long long int>( _bytedata[ index8bit ] )  |
			static_cast<unsigned long long int>( _bytedata[ index8bit + 1 ] ) << 8  |
			static_cast<unsigned long long int>( _bytedata[ index8bit + 2 ] ) << 16 |
			static_cast<unsigned long long int>( _bytedata[ index8bit + 3 ] ) << 24 |
			static_cast<unsigned long long int>( _bytedata[ index8bit + 4 ] ) << 32 ;
	return retval;
}

unsigned int UCharBigEndianVec::Get32bitWord(unsigned int index32bit) const
{
    unsigned int index8bit = index32bit*4;

    /// read the 4 bytes, least significant first (big endian byte sequence)
    unsigned int retval =
	static_cast<unsigned int>( _bytedata[ index8bit     ] )       |
	static_cast<unsigned int>( _bytedata[ index8bit + 1 ] ) <<  8 |
	static_cast<unsigned int>( _bytedata[ index8bit + 2 ] ) << 16 |
	static_cast<unsigned int>( _bytedata[ index8bit + 3 ] ) << 24;

    return retval;
}

unsigned short UCharBigEndianVec::Get10bitWord(unsigned int index10bit, unsigned int n40bitwords,
					     unsigned int offset32bit) const
{
    // The way to get the bits out of the data stream is a sliding window algorithm.
    // The sequence is always contained in two consecutive bytes. First the two relevant bytes
    // are copied to a 16 bit word, then the 10 bit mask is placed at the specific position.
    // The position depends on the 10 bit index (the second word is shifted by 2 bits, the third
    // by 4 bits and the fourth by 6 bits. The fifth word is shifted by 8 bits, which means no
    // shift, but index8bit is increased by 1).
    // Afterwords the ten bits are shifted to the end of the 16 bit word.

    unsigned int   index8bit;
    if (!_altrowordsreversed) // normal ordering of altro words, i. e. backward
    	{
    	// Calculate the 8 bit index. Note that the first part is an integer division, which
    	// automatically generates the correct index ( 1*10/8=1 , 2*10/8=2, 3*10/8=3, 4*10/8=5 etc)
    	index8bit = index10bit * 10 / 8 + offset32bit*4;
    	}
    else // reversed ordering of altro words, i. e. forward
    	{
    	// Calculation as before, but first we have to transform the 10 bit index
    	// Reverse the 40-bit part ( index10bit / 4 ) (integer division)
    	// and add the 10-bit part ( index10bit % 4 )
    	unsigned int reversedindex10bit = (n40bitwords - ( index10bit / 4 ) - 1)*4
    			+ ( index10bit % 4 );
    	//      std::cout << "i10 "<< index10bit<< " ri10 "<< reversedindex10bit //		<< " n40 "<< n40bitwords<<std::endl;

    	index8bit = reversedindex10bit * 10 / 8 + offset32bit*4;
    	}

    // The ordering within the 40 bit altro words is the same for both cases

    // Compose the 2-byte word onto which the mask will be applied.
    // The least significant is the first byte in the _bytedata vector.
    unsigned short data16bit = _bytedata[index8bit] | (_bytedata[index8bit + 1] << 8) ;

    // Calculate how far to shift the window, i. e. relative alignment of 10-bit and 8-bit blocks
    unsigned short bitshift = (index10bit % 4) * 2;
    unsigned short mask = 0x3FF << bitshift; // 10-bit mask, shifted

    return (data16bit & mask) >> bitshift; // apply the mask and shift the result back to the LSB
}

std::vector<unsigned char>::const_iterator UCharBigEndianVec::PointDataWord(unsigned int index32bit)
{
	_dataindex = _bytedata.begin()+(index32bit*4);
	return _dataindex;
}

std::vector<unsigned char>::const_iterator UCharBigEndianVec::end()
{
	_dataindex = _bytedata.end();
	return _dataindex;
}
