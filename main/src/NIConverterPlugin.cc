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

#define GET(d, i) getlittleendian<unsigned>(&(d).at((i)*4))

namespace eudaq {

  class NIConverterPlugin : public DataConverterPlugin {
  public:
    virtual void Initialize(const Event & /*e*/, const Configuration & /*c*/) {
      //FillInfo(e, c);
    }

    virtual unsigned GetTriggerID(Event const & ev) const {
      const RawDataEvent & rawev = dynamic_cast<const RawDataEvent &>(ev);
      if (rawev.NumBlocks() < 1 || rawev.GetBlock(0).size() < 8) return (unsigned)-1;
      return GET(rawev.GetBlock(0), 1) & 0xffff;
    }

    virtual bool GetStandardSubEvent(StandardEvent & result, const Event & source) const {
      if (source.IsBORE()) {
        // shouldn't happen
        return true;
      } else if (source.IsEORE()) {
        // nothing to do
        return true;
      }
      // If we get here it must be a data event
      const RawDataEvent & rawev = dynamic_cast<const RawDataEvent &>(source);
      if (rawev.NumBlocks() != 2 || rawev.GetBlock(0).size() < 20 ||
          rawev.GetBlock(1).size() < 20) {
        return false;
      }
      const std::vector<unsigned char> & data = rawev.GetBlock(0);
      unsigned tluid = GetTriggerID(source);
      unsigned pivot = GET(data, 1) >> 16;
      size_t offset = 2;
      while (data.size()/4 >= offset + 3) {
        result.AddPlane(ConvertPlane(data, offset, tluid, pivot));
        unsigned len = GET(data, offset + 1) & 0xffff;
        offset += len + 4;
      }
      return true;
    }
    StandardPlane ConvertPlane(const std::vector<unsigned char> & data, size_t offset, unsigned tluid, unsigned pivot) const {
      unsigned id = 0;
      StandardPlane plane(id, "NI", "MIMOSA26");
      plane.SetSizeZS(1152, 576, 0, 2, StandardPlane::FLAG_WITHPIVOT | StandardPlane::FLAG_DIFFCOORDS);
      plane.SetTLUEvent(tluid);
      plane.SetPivotPixel(pivot);
      return plane;
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
    //static size_t GetID(const Event & event, size_t i) {
    //  if (const RawDataEvent * ev = dynamic_cast<const RawDataEvent *>(&event)) {
    //    return ev->GetID(i);
    //  }
    //  return 0;
    //}
    //static size_t NumPlanes(const Event & event) {
    //  const RawDataEvent & rawev = dynamic_cast<const RawDataEvent &>(ev);
    //  if (rawev.NumBlocks() < 1) return 0;
    //  const std::vector<unsigned char> & data = rawev.GetBlock(0);
    //  size_t offset = 2, result = 0;
    //  while (data.size()/4 >= offset + 3) {
    //    unsigned len = GET(data, offset + 1) & 0xffff;
    //    offset += len + 4;
    //  }
    //}
  private:
    NIConverterPlugin() : DataConverterPlugin("NI") {}
    static NIConverterPlugin const m_instance;
  };

  NIConverterPlugin const NIConverterPlugin::m_instance;


} //namespace eudaq
