#ifndef EUDAQ_INCLUDED_AltroConverterPlugin
#define EUDAQ_INCLUDED_AltroConverterPlugin

#include "eudaq/DataConverterPlugin.hh"
#include <EVENT/LCEvent.h>
#include <Exceptions.h>
#include <lcio.h>
#include <string>

namespace eudaq{

/** A helper class to read the byte sequence of big endian 32 bit data as 10 bit, 32 bit or
 *  40 bit data.
 *  This class only contains a reference to the data, not a copy, to avoid copying of the data.
 */

class UCharBigEndianVec
{
private:

    /** A reference to the data vector.
     *  Can only be accessed through the GetNNbitWord() functions.
     */
    std::vector<unsigned char> const & _bytedata;

protected:
    bool _altrowordsreversed;

public:
    /** The constructor. 
     *  It reqires a reference of the actual data vector.
     */
  UCharBigEndianVec(std::vector<unsigned char> const & datavec, bool altrowordsreversed);

    /// size in 32 bit words
    size_t Size(){ return _bytedata.size() / 4 ; }
    

    /** Helper function to get a 40 bit word with correct endinanness out of the byte vector.
     *  The offset32bit is the position of the first 10 bit word within the 32bit stream
     */
    unsigned short Get10bitWord(unsigned int index10bit, unsigned int n40bitwords,
				unsigned int offset32bit) const; 

    /** Helper function to get a 32 bit word with correct endinanness out of the byte vector.
     */
    unsigned int Get32bitWord(unsigned int index32bit) const;

    /** Helper function to get a 40 bit word with correct endinanness out of the byte vector.
     *  The offset32bit is the position of the first 40 bit word within the 32bit stream
     */
    unsigned long long int Get40bitWord(unsigned int index40bit, unsigned int n40bitwords, 
					unsigned int offset32bit) const;
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
    /** Returns the event converted to lcio. This is the working horse and the 
     *  main part of this plugin.
     */
    virtual lcio::LCEvent * GetLCIOEvent( eudaq::Event const * ee ) const;

    /** Returns the event converted to eudaq::StandardEvent.
     *  Only contains the primitive implementation for the dummy StandardEvent.
     */
    virtual StandardEvent * GetStandardEvent( eudaq::Event const * ee ) const;


    /** Nested helper class: Exception which is throws in case of bad data block
     */
    class BadDataBlockException : public lcio::Exception 
    {
      public:
        BadDataBlockException(std::string const & text) : lcio::Exception(text) {}
    };

    /** The empty destructor. Need to add it to make it virtual.
     */
    virtual ~AltroConverterPlugin(){}

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
