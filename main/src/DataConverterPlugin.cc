#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/PluginManager.hh"
#include <iostream>

namespace eudaq
{

DataConverterPlugin::DataConverterPlugin(std::string eventtype) : m_eventtype( eventtype )
{
    std::cout << "EUDAQ_DEBUG: This is DataConverterPlugin::DataConverterPlugin() @ " 
	      << this << std::endl
	      << "EUDAQ_DEBUG:  Registering event type "<< m_eventtype << std::endl;
    PluginManager::GetInstance().RegisterPlugin( this );    
}

}//namespace eudaq
