/*
 * AltroConverterHelper.hh
 *
 *  Created on: Oct 1, 2009
 *      Author: caiazza
 */

#ifndef ALTROCONVERTERHELPER_HH_
#define ALTROCONVERTERHELPER_HH_

#include <stdexcept>
#include <vector>

#include "EVENT/LCCollection.h"
#include <EVENT/LCEvent.h>
#include <IMPL/LCEventImpl.h>
#include <IMPL/TrackerRawDataImpl.h>
#include <EVENT/LCIO.h>
#include <lcio.h>

namespace eudaq
{
/**
 * All the classes contained in this file are thought to be generic objects to decode
 * the Altro data format, converting it into the LCIO format. Depending on the version
 * of the raw data this classes can use different decoding algorithm. In this way it will
 * be possible to update the converter plug-in whenever the data format is modified again
 * just modifying the single block that changed.
 */

/**
 * This class contains the decoding algorithms for the AltroEvent header. It only assumes that the
 * header length is contained in the first field of the block. The beginning of the block is pointed
 * to with a constant iterator
 */

class AltroEventHeader
{

public:
	/**
	 * This class contains the data of the ALTRO Event header.
	 * The constructor performs a test to verify the integrity of the data block and will decode
	 * the header itself filling the class member variables
	 */
	AltroEventHeader(std::vector<unsigned char>::const_iterator & headerstart, int & formatversion);
	~AltroEventHeader() {};

	/**
	 * This method will fill the LCIO Event with the data contained in the header
	 */
	void FillLCIOEvent(lcio::LCEvent & lcioevent) const;

	/**
	 * Returns an iterator to the first word after the end of the header, a behaviour similar to the
	 * vector.end method
	 */
	const std::vector<unsigned char>::const_iterator & End() const;


private:
	void TestIntegrity() const;

	void DecodeHeader();

	///This variable identifies the first word of the event header
	const std::vector<unsigned char>::const_iterator _headerstart;
	///This variable will in fact identify the first word of the next block (like vector.end does)
	std::vector<unsigned char>::const_iterator _headerend;
	const int _formatversion;
	unsigned int _headerlength;
	unsigned int _eventNumber;
	unsigned int _DBOXEVTN;
	unsigned int _DBOXTIME;
	unsigned int _TLUEVTN;

	//Static constants
	static const unsigned int RIGHTBLOCKID421;
	static const unsigned int RIGHTHEADERLENGTH421;

}; // class AltroEventHeader

class AltroRCUHeader
{
	friend class AltroRCUBlock;
public:
	AltroRCUHeader(std::vector<unsigned char>::const_iterator & blockStart, int & formatversion);
	~AltroRCUHeader(){};
	const std::vector<unsigned char>::const_iterator & End();
	void FillLCIOEvent(lcio::LCEvent & lcioevent) const;

private:
	void Decode();
	void TestIntegrity() const;

	std::vector<unsigned char>::const_iterator _blockStart;
	std::vector<unsigned char>::const_iterator _blockEnd;
	int _formatversion;

	static const unsigned int RCUHEADLENGTH421;
	static const unsigned int RCUHEADID421;

};
//End AltroRCUHeader
//Begin of AltroRCUTrailer
class AltroRCUTrailer
{
public:
	friend class AltroRCUBlock;
	AltroRCUTrailer(std::vector<unsigned char>::const_iterator & blockStart, int & formatversion);
	~AltroRCUTrailer();

	const std::vector<unsigned char>::const_iterator & End() const;
	unsigned int RCUPayloadLength() const;
	void FillLCIOEvent(lcio::LCEvent & lcioevent) const;

private:
	void TestIntegrity() const;
	void Decode();

	std::vector<unsigned char>::const_iterator _blockStart;
	std::vector<unsigned char>::const_iterator _blockEnd;
	int _formatversion;
	unsigned int _payloadLength;
	unsigned int _RCUFWVer;

	static const unsigned int TRAILERWORDIDMASK421;
	static const unsigned int TRAILERLENGTH421;
	static const unsigned int PARIDMASK421;
	static const unsigned int PARVALUEMASK421;

};
//------------End of AltroRCUTrailer----------
//--------------Begin of AltroEventData-------
class AltroEventData
{
public:
	/**
	 * This enum lists the version recognized by the class
	 * The RCU3X32 version handles the Altro Data Format where three sample words are contained in
	 * a single 32 bit dataword
	 */
	enum DataVer_t
	{
		UNRECOGNIZED,
		RCU3X32
	};
	AltroEventData(std::vector<unsigned char>::const_iterator & blockStart, unsigned int payload, DataVer_t version);
	~AltroEventData();
	void FillLCIODataCollection(lcio::LCCollection *altrocollection, unsigned int RCUID);

private:

	void TestIntegrity() const;
	void Decode();

	void CreateSamplesVec(std::vector<short>::const_iterator & SampleBegin, std::vector<short> & datasamples);

	const std::vector<unsigned char>::const_iterator _blockStart;
	const unsigned int _payLoad;
	const DataVer_t _version;

	std::vector<unsigned char>::const_iterator _blockEnd;
	std::list<lcio::TrackerRawDataImpl*> _dataList;

	static const unsigned int DATAWORDIDMASK421;
	static const unsigned int SAMPLEWORDMASKRCU3X32;

	//Nested class
	//------------Begin AltroChHeader-------------
	class AltroChHeader
	{
	public:
		/**
		 * This enum lists the different headers this version of the class can handle.
		 * The 32 bit header is used since version 4.2.0 with dataformat 1.
		 * Before that version and with version 4.2.0 with dataformat 0, the header was contained
		 * in the first (less significant) 32 bits o the first 40 bit Altro Word.
		 * This first version is not yet handled by this class
		 */
		AltroChHeader(unsigned int word = 0xC0000000, DataVer_t version = UNRECOGNIZED);
		~AltroChHeader(){};

		void AssignWord(unsigned int word, DataVer_t version);
		unsigned int ChAddress() {return _chAddress;}
		unsigned int BlockLength() {return _blockLength;}
		bool ChError() {return _error;}

	private:
		unsigned int _chAddress;
		unsigned int _blockLength;
		bool _error;

		static const unsigned int ERRORBITMASKRCU3X32;
		static const unsigned int CHADDRESSMASKRCU3X32;
		static const unsigned int NRSAMPLEMASKRCU3X32;

	};
	//--------------End AltroChHeader-------------
};
//----------------End AltroEventData-------------
/**
 * The Class AltroRCUBlock contains the data received from a single RCU of an Altro DAQ system
 * The class splits those data in a header a trailer and a datablock.
 * Each of those part are separately decoded by specialized classes.
 * Finally this class fills the tracker raw data collection with these data and send a pointer to this collection
 * to the other classes so that they can put the data they contain in it.
 */
class AltroRCUBlock
{
public:
	AltroRCUBlock(std::vector<unsigned char>::const_iterator & blockStart, int & formatversion);
	~AltroRCUBlock();
	const std::vector<unsigned char>::const_iterator End();
	void FillLCIODataCollection(lcio::LCCollection *altrocollection);

private:
	std::vector<unsigned char>::const_iterator _blockStart;
	///This iterator is set just after the end of the block
	std::vector<unsigned char>::const_iterator _blockEnd;

	void TestIntegrity() const;
	void Decode();

	unsigned int _RCUID;

	std::vector<unsigned char>::const_iterator _headerStart;
	AltroRCUHeader* _RCUHeader;
	std::vector<unsigned char>::const_iterator _trailerStart;
	AltroRCUTrailer* _RCUTrailer;
	std::vector<unsigned char>::const_iterator _dataStart;
	AltroEventData* _EventData;

	int _formatversion;

	static const int MINRCULENGTH421;
};

/**
 * Common Helper Functions
 * Helper Functions used by more than one helper classes
 */

/**
 * This function take the iterator on a vector of 8 bit words which are meant to be part o a 32 bit
 * words with big endian byte ordering and gives back the value of the word actually converting
 * the word from Big Endian to Little Endian. The function doesn't check whether the data word is actually
 * inside of the data vector bouds. This check must be done by the caller
 */
unsigned int Get32bitBigEndian(std::vector<unsigned char>::const_iterator itDataIndex);

} //namespace eudaq
#endif /* ALTROV421CONVERTER_HH_ */
