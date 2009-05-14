#include "eudaq/PluginManager.hh"
#include "eudaq/Exception.hh"

//#include <iostream>
#include <string>
using namespace std;

namespace eudaq
{

PluginManager &  PluginManager::GetInstance()
{
    // the only one static instance of the plugin manager is in the getInstance function
    // like this it is ensured that the instance is created before it is used
    static PluginManager manager;
    return manager;
}

void PluginManager::RegisterPlugin(DataConverterPlugin const * plugin)
{
    m_pluginmap[plugin->GetEventType()]= plugin;
}

DataConverterPlugin const * PluginManager::GetPlugin(std::string eventtype)
{
    std::map<std::string, DataConverterPlugin const *>::iterator pluginiter
      = m_pluginmap.find(eventtype);

    if (pluginiter != m_pluginmap.end())
    {
      return pluginiter->second;
    }
    else
    {
      EUDAQ_THROW("PluginManager::GetPlugin(): Unkown event type "+eventtype);
      return 0;
    }
    
}

}//namespace eudaq
