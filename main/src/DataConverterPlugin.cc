#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/PluginManager.hh"

#include <iostream>

namespace eudaq {

  DataConverterPlugin::DataConverterPlugin(std::string subtype)
    : m_eventtype(make_pair(Event::str2id("_RAW"), subtype))
  {
    std::cout << "EUDAQ_DEBUG: DataConverterPlugin registering event type " << Event::id2str(m_eventtype.first) << ":" << m_eventtype.second << std::endl;
    PluginManager::GetInstance().RegisterPlugin(this);
  }

  DataConverterPlugin::DataConverterPlugin(unsigned type, std::string subtype)
    : m_eventtype(make_pair(type, subtype))
  {
    std::cout << "EUDAQ_DEBUG: DataConverterPlugin registering event type " << Event::id2str(m_eventtype.first) << ":" << m_eventtype.second << std::endl;
    PluginManager::GetInstance().RegisterPlugin(this);
  }

}//namespace eudaq
