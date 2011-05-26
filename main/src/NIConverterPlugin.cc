#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Configuration.hh"
#include "eudaq/Logger.hh"

#if USE_LCIO
#  include "IMPL/LCEventImpl.h"
#  include "IMPL/TrackerRawDataImpl.h"
#  include "IMPL/TrackerDataImpl.h"
#  include "IMPL/LCCollectionVec.h"
#  include "IMPL/LCGenericObjectImpl.h"
#  include "UTIL/CellIDEncoder.h"
#  include "lcio.h"
#endif

#if USE_EUTELESCOPE
#  include "EUTELESCOPE.h"
#  include "EUTelMimoTelDetector.h"
#  include "EUTelMimosa18Detector.h"
#  include "EUTelMimosa26Detector.h"
#  include "EUTelSetupDescription.h"
#  include "EUTelEventImpl.h"
#  include "EUTelSparseDataImpl.h"
#  include "EUTelSimpleSparsePixel.h"
#  include "EUTelRunHeaderImpl.h"
using eutelescope::EUTELESCOPE;
#endif

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

namespace eudaq {

  class NIConverterPlugin : public DataConverterPlugin {
  public:
    virtual void Initialize(const Event & /*e*/, const Configuration & /*c*/) {
      //FillInfo(e, c);
    }

    virtual unsigned GetTriggerID(Event const & ev) const {
      const RawDataEvent & rawev = dynamic_cast<const RawDataEvent &>(ev);
      if (rawev.NumBlocks() < 1) return (unsigned)-1;
      //const std::vector<unsigned char> & data = rawev.GetBlock(rawev.NumBlocks() - 1);
      return (unsigned)-1; //GetTLUEvent(data);
    }

    virtual bool GetStandardSubEvent(StandardEvent & /*result*/, const Event & /*source*/) const {
      return false;
      //return ConvertStandard(result, source);
    }

#if USE_LCIO && USE_EUTELESCOPE
    virtual void GetLCIORunHeader(lcio::LCRunHeader & /*header*/, eudaq::Event const & /*bore*/, eudaq::Configuration const & /*conf*/) const {
      return false;
      //return ConvertLCIOHeader(header, bore, conf);
    }

    virtual bool GetLCIOSubEvent(lcio::LCEvent & /*lcioEvent*/, const Event & /*eudaqEvent*/) const {
      return false;
      //return ConvertLCIO(lcioEvent, eudaqEvent);
    }
#endif

  protected:
    static size_t GetID(const Event & event, size_t i) {
      if (const RawDataEvent * ev = dynamic_cast<const RawDataEvent *>(&event)) {
        return ev->GetID(i);
      }
      return 0;
    }
  private:
    NIConverterPlugin() : DataConverterPlugin("NI") {}
    static NIConverterPlugin const m_instance;
  };

  NIConverterPlugin const NIConverterPlugin::m_instance;


} //namespace eudaq
