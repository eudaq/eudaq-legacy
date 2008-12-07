#ifndef EUDAQ_INCLUDED_DataConverterPlugin
#define EUDAQ_INCLUDED_DataConverterPlugin

#include "eudaq/debugtools.hh"
#include "eudaq/Event.hh"

#include <EVENT/LCEvent.h>
#include <lcio.h>

#include <string>

namespace eudaq{

/**
 *  The DataConverterPlugin is the abstract base for all plugins. The
 *  actual impementation provides the GetLCIOEvent() and GetStandardEvent()
 *  functions which
 *  get the eudaq::Event as input parameter.
 *
 *  The implementations should be sinlgeton classes which only can be
 *  accessed via the plugin manager. (See TimepixConverterPlugin as example).
 *  The plugin implementations have to register with the plugin manager.
 */

class DataConverterPlugin
{
    
public:
    /** Returns the LCIO version of the event.
     */
    virtual lcio::LCEvent * GetLCIOEvent( eudaq::Event const * ee ) const = 0;

    /** Returns the StandardEvent version of the event.
     */
    virtual StandardEvent * GetStandardEvent( eudaq::Event const * ee ) const = 0;

    /** Returns the type of event this plugin can convert to lcio as a string.
     */
    virtual std::string const & GetEventType() const {return m_eventtype;}

protected:
    /** The string storing the event type this plugin can convert to lcio.
     *  This string has to be set in the constructor of the actual implementations 
     *  of the plugin.
     */
    std::string m_eventtype;

    /** The protected constructor which automatically registeres the plugin
     *  at the pluginManager.
     */
    DataConverterPlugin(std::string eventtype);

private:
    /** The private copy constructor. It is not used anywhere, so there is not 
     *  even an implementation. Even if the childs default copy constructor is public
     *  the code will not compile if it is called, since it cannot acces this cc, which the
     *  the default cc does.
     */
    DataConverterPlugin(DataConverterPlugin &);
};

}//namespace eudaq

#endif // EUDAQ_INCLUDED_DataConverterPlugin
