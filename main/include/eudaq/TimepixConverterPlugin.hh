#ifndef EUDAQ_INCLUDED_TimepixConverterPlugin
#define EUDAQ_INCLUDED_TimepixConverterPlugin

#include "eudaq/DataConverterPlugin.hh"
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

class TimepixConverterPlugin : public DataConverterPlugin
{
    
public:
    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    virtual lcioevent * GetLCIOEvent( eudaq::Event const * ee ) const;

    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    virtual StandardEvent * GetStandardEvent( eudaq::Event const * ee ) const;

private:
    /** The private constructor. The only time it is called is when the
     *  one single instance is created, which lives within the object.
     *  It calls the DataConverterPlugin constructor with the
     *  accoring event type string. This automatically registers
     *  the plugin to the plugin manager.
     */
    TimepixConverterPlugin() : DataConverterPlugin("timepixEvent"){}

    /** The one single instance of the TimepixConverterPlugin.
     *  It has to be created in the .cc file
     */
    static TimepixConverterPlugin const m_timepixconverterplugininstance;
    
};

}//namespace eudaq

#endif // EUDAQ_INCLUDED_TimepixConverterPlugin
