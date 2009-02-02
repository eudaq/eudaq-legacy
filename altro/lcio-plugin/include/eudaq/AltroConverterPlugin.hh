#ifndef EUDAQ_INCLUDED_AltroConverterPlugin
#define EUDAQ_INCLUDED_AltroConverterPlugin

#include "eudaq/DataConverterPlugin.hh"
#include "EVENT/LCEvent.h"
#include "lcio.h"
#include <string>

namespace eudaq{

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
    virtual const AltroConverterPlugin * GetInstance() const;

protected:
    /** The private constructor. The only time it is called is when the
     *  one single instance is created, which lives within the object.
     *  It calls the DataConverterPlugin constructor with the
     *  accoring event type string. This automatically registers
     *  the plugin to the plugin manager.
     */
    AltroConverterPlugin() : DataConverterPlugin("AltroEvent"){}

    /** Helper function to return the correct index for accessing the big endian 32 bit data.
     *  In this order the index sequence is 3 2 1 0 7 6 5 4 11 10 9 8 etc.
     */
    inline unsigned int endian(unsigned int i)
    {
	// unsigned int j = i % 4; // we'll need that remainder twice
	// return (i - j) + (3 - j); // i - j: this is the correct 4-byte block, 3 - j: revert the endianness inside the 4-byte block
	
	return i - 2 * (i % 4) + 3;
    }

    unsigned short get10bitWord(unsigned int tenBitIndex, unsigned int offset32bit); 

    /** Helper function to get a 32 bit word with correct eninanness out of the byte vector.
     */
    unsigned int get32bitWord(unsigned int index32bit);
    
    std::vector<unsigned char> bytedata;

private:
    /** The one single instance of the AltroConverterPlugin.
     *  It has to be created in the .cc file
     */
    static AltroConverterPlugin const m_altroconverterplugininstance;
    
};

}//namespace eudaq

#endif // EUDAQ_INCLUDED_AltroConverterPlugin
