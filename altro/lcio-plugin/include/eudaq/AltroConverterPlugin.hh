#ifndef EUDAQ_INCLUDED_AltroConverterPlugin
#define EUDAQ_INCLUDED_AltroConverterPlugin

#include "eudaq/DataConverterPlugin.hh"
#include "EVENT/LCEvent.h"
#include "lcio.h"
#include <string>

namespace eudaq{

/** A helper class to read the byte sequence of big endian 32 bit data as 10 bit, 32 bit or
 *  40 bit data.
 *  This class only contains a reference to the data, not a copy, to avoid copying of the data.
 */

class UCharBigEndianVec
{
private:

    /** Helper function to return the correct index for accessing the big endian 32 bit data.
     *  In this order the index sequence is 3 2 1 0 7 6 5 4 11 10 9 8 etc.
     */
    inline unsigned int endian(unsigned int i) const
    {
	// unsigned int j = i % 4; // we'll need that remainder twice
	// return (i - j) + (3 - j); // i - j: this is the correct 4-byte block, 3 - j: revert the endianness inside the 4-byte block
	
	return i - 2 * (i % 4) + 3;
    }

    std::vector<unsigned char> const & bytedata;

public:
    UCharBigEndianVec(std::vector<unsigned char> const & datavec);

    // size in 32 bit words
    size_t Size(){ return bytedata.size() / 4 ; }
    

    unsigned short Get10bitWord(unsigned int tenBitIndex, unsigned int offset32bit) const; 

    /** Helper function to get a 32 bit word with correct endinanness out of the byte vector.
     */
    unsigned int Get32bitWord(unsigned int index32bit) const;

    /** Helper function to get a 40 bit word with correct endinanness out of the byte vector.
     *  The offset32bit is the position of the first 40 bit within the 32bit stream
     */
    unsigned long long int Get40bitWord(unsigned int index40bit, unsigned int offset32bit) const;
};

/** Implementation of the DataConverterPlugin to convert an eudaq::Event
 *  to an lcio::event.
 *
 *  The class is implemented as a singleton because it manly
 *  contains the conversion code, which is in the getLcioEvent()
 *  function. There has to be one static instance, which is 
 *  registered at the pluginManager. This is automatically done by the
 *  inherited constructor of the DataConverterPlugin.
 */

class AltroConverterPlugin : public DataConverterPlugin
{
    
public:
    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    virtual lcio::LCEvent * GetLCIOEvent( eudaq::Event const * ee ) const;

    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    virtual StandardEvent * GetStandardEvent( eudaq::Event const * ee ) const;

    /** Returns the on instance for access to the plugin. Or do this with the handler? */
    // virtual const AltroConverterPlugin * GetInstance() const;

protected:
    /** The private constructor. The only time it is called is when the
     *  one single instance is created, which lives within the object.
     *  It calls the DataConverterPlugin constructor with the
     *  accoring event type string. This automatically registers
     *  the plugin to the plugin manager.
     */
    AltroConverterPlugin() : DataConverterPlugin("AltroEvent"){}


private:
    /** The one single instance of the AltroConverterPlugin.
     *  It has to be created in the .cc file
     */
    static AltroConverterPlugin const m_altroconverterplugininstance;
    
};

}//namespace eudaq

#endif // EUDAQ_INCLUDED_AltroConverterPlugin
