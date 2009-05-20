#ifndef EUDAQ_INCLUDED_PluginManager
#define EUDAQ_INCLUDED_PluginManager

#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/DetectorEvent.hh"
#include <string>
#include <map>

namespace eudaq{

/** The plugin manager has a map of all available plugins.
 *  On creating time every plugin automatically registeres at
 *  the plugin manager, wich adds the event type string and 
 *  a pointer to the according plugin to a map.
 *  A generic event being received contains an identification string,
 *  and the plugin manager can deliver the correct plugin to 
 *  convert it to lcio.
 */
  class PluginManager {

  public:
    /** Register a new plugin to the plugin manager.
     */
    void RegisterPlugin(DataConverterPlugin const * plugin);

    /** Get the instance of the plugin manager. As this is a songleton class with
     *  private constructor and copy constructor, this is the only way to acces it.
     */
    static PluginManager & GetInstance();

    static eudaq::StandardEvent ConvertToStandard(const DetectorEvent &);
    static lcio::LCEvent * ConvertToLCIO(const DetectorEvent &);

    /** Get the correct plugin implementation according to the event type.
     */
    DataConverterPlugin const * GetPlugin(std::string eventtype);

  private:
    /** The map that correlates the event type with its converter plugin.
     */
    std::map<std::string, DataConverterPlugin const *> m_pluginmap;

  private:
    PluginManager(){};
    PluginManager( PluginManager const & ){};
  };

}//namespace eudaq

#endif //EUDAQ_INCLUDED_PluginManager
