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

  bool EUDRBConverterPlugin::GetStandardSubEvent(StandardEvent & result, const eudaq::Event & source) const {
    StandardPlane plane;
    result.AddPlane(plane);
    return true;
  }

} //namespace eudaq
