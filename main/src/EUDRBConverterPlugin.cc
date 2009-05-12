#include "eudaq/EUDRBConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"

#if USE_LCIO
#  include <IMPL/LCEventImpl.h>
#  include <IMPL/TrackerRawDataImpl.h>
#  include <IMPL/LCCollectionVec.h>
#endif

#include <iostream>
#include <string>

namespace eudaq
{

  EUDRBConverterPlugin const EUDRBConverterPlugin::m_eudrbconverterplugininstance;

  StandardEvent * EUDRBConverterPlugin::GetStandardEvent(eudaq::Event const * ee) const {
    StandardEvent * se = new StandardEvent;
    se->b = ee->GetEventNumber();
    return se;
  }

} //namespace eudaq
