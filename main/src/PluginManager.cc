#include "eudaq/PluginManager.hh"
#include "eudaq/Exception.hh"

#if USE_LCIO
#  include "lcio.h"
#  include "IMPL/LCEventImpl.h"
#endif

//#include <iostream>
#include <string>
using namespace std;

namespace eudaq {

  PluginManager & PluginManager::GetInstance() {
    // the only one static instance of the plugin manager is in the getInstance function
    // like this it is ensured that the instance is created before it is used
    static PluginManager manager;
    return manager;
  }

  void PluginManager::RegisterPlugin(DataConverterPlugin const * plugin) {
    m_pluginmap[plugin->GetEventType()] = plugin;
  }

  DataConverterPlugin const & PluginManager::GetPlugin(const Event & event) {
    return GetPlugin(std::make_pair(event.get_id(), event.GetSubType()));
  }

  DataConverterPlugin const & PluginManager::GetPlugin(PluginManager::t_eventid eventtype) {
    std::map<t_eventid, DataConverterPlugin const *>::iterator pluginiter
      = m_pluginmap.find(eventtype);

    if (pluginiter == m_pluginmap.end()) {
      EUDAQ_THROW("PluginManager::GetPlugin(): Unkown event type "+Event::id2str(eventtype.first)+":"+eventtype.second);
    }

    return *pluginiter->second;
  }

  StandardEvent PluginManager::ConvertToStandard(const DetectorEvent & dev) {
    StandardEvent event(dev.GetRunNumber(), dev.GetEventNumber(), dev.GetTimestamp());
    for (size_t i = 0; i < dev.NumEvents(); ++i) {
      ConvertStandardSubEvent(event, *dev.GetEvent(i));
    }
    return event;
  }

#if USE_LCIO
  lcio::LCEvent * PluginManager::ConvertToLCIO(const DetectorEvent & dev) {
    lcio::LCEvent * event = new lcio::LCEventImpl;
    for (size_t i = 0; i < dev.NumEvents(); ++i) {
      ConvertLCIOSubEvent(*event, *dev.GetEvent(i));
    }
    return event;
  }
#else
  lcio::LCEvent * PluginManager::ConvertToLCIO(const DetectorEvent &) {
    return 0;
  }
#endif

  void PluginManager::ConvertStandardSubEvent(StandardEvent & dest, const Event & source) {
    GetInstance().GetPlugin(source).GetStandardSubEvent(dest, source);
  }

  void PluginManager::ConvertLCIOSubEvent(lcio::LCEvent & dest, const Event & source) {
    GetInstance().GetPlugin(source).GetLCIOSubEvent(dest, source);
  }

}//namespace eudaq
