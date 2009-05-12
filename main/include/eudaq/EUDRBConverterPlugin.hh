#ifndef EUDAQ_INCLUDED_EUDRBConverterPlugin
#define EUDAQ_INCLUDED_EUDRBConverterPlugin

#include "eudaq/DataConverterPlugin.hh"

namespace eudaq {

/** Implementation of the DataConverterPlugin to convert an eudaq::Event
 *  to an lcio::event.
 *
 *  The class is implemented as a singleton because it manly
 *  contains the conversion code, which is in the getLcioEvent()
 *  function. There has to be one static instance, which is
 *  registered at the pluginManager. This is automatically done by the
 *  inherited constructor of the DataConverterPlugin.
 */

  class EUDRBConverterPlugin : public DataConverterPlugin {
  public:
    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    //virtual lcio::LCEvent * GetLCIOEvent( eudaq::Event const * ee ) const;

    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    virtual StandardEvent * GetStandardEvent( eudaq::Event const * ee ) const;

    /** The empty destructor. Need to add it to make it virtual.
     */
    virtual ~EUDRBConverterPlugin(){}

  private:
    /** The private constructor. The only time it is called is when the
     *  one single instance is created, which lives within the object.
     *  It calls the DataConverterPlugin constructor with the
     *  accoring event type string. This automatically registers
     *  the plugin to the plugin manager.
     */
    EUDRBConverterPlugin() : DataConverterPlugin("EUDRBEvent"){}

    /** The one single instance of the TimepixConverterPlugin.
     *  It has to be created in the .cc file
     */
    static EUDRBConverterPlugin const m_eudrbconverterplugininstance;
  };

}//namespace eudaq

#endif // EUDAQ_INCLUDED_EUDRBConverterPlugin
