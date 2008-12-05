#include "eudaq/TimepixConverterPlugin.hh"
#include "eudaq/Exception.hh"

#include <iostream>

namespace eudaq
{

TimepixConverterPlugin const TimepixConverterPlugin::m_timepixconverterplugininstance;

lcioevent * TimepixConverterPlugin::GetLCIOEvent( eudaq::Event const * ee ) const
{
    lcioevent * le = new lcioevent;
    le->a = ee->GetEventNumber ();

    return le;
}

StandardEvent * TimepixConverterPlugin::GetStandardEvent( eudaq::Event const * ee ) const
{
    StandardEvent * se = new StandardEvent;
    se->b = ee->GetEventNumber ();

    return se;
}

} //namespace eudaq
