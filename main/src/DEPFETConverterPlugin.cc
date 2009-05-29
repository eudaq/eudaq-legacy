#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/DEPFETEvent.hh"
#include "eudaq/Logger.hh"
#include "eudaq/Utils.hh"

#if USE_LCIO
#  include "IMPL/LCEventImpl.h"
#  include "IMPL/TrackerRawDataImpl.h"
#  include "IMPL/LCCollectionVec.h"
#  include "lcio.h"
#endif

#include <iostream>
#include <string>
#include <vector>

namespace eudaq {

  class DEPFETConverterBase {
  public:
    StandardPlane ConvertPlane(const std::vector<unsigned char> & data, unsigned id) const {
      StandardPlane plane(id, "DEPFET", "?");
      plane.m_tluevent = getlittleendian<unsigned>(&data[4]);
      plane.SetSizeRaw(64, 128);
      unsigned npixels = plane.m_xsize * plane.m_ysize;
      for (size_t i = 0; i < npixels; ++i) {
        unsigned d = getlittleendian<unsigned>(&data[3 + 4*i]);
        plane.m_x[i] = (d >> 16) & 0x3f;
        plane.m_y[i] = (d >> 22) & 0x7f;
        plane.m_pix[0][i] = d & 0xffff;
      }
      return plane;
    }
  };

  /********************************************/

  class DEPFETConverterPlugin : public DataConverterPlugin, public DEPFETConverterBase {
  public:
    //virtual lcio::LCEvent * GetLCIOEvent( eudaq::Event const * ee ) const;

    virtual bool GetStandardSubEvent(StandardEvent &, const eudaq::Event &) const;

  private:
    DEPFETConverterPlugin() : DataConverterPlugin("DEPFET") {}

    static DEPFETConverterPlugin const m_instance;
  };

  DEPFETConverterPlugin const DEPFETConverterPlugin::m_instance;

  bool DEPFETConverterPlugin::GetStandardSubEvent(StandardEvent & result, const Event & source) const {
    if (source.IsBORE()) {
      //FillInfo(source);
      return true;
    } else if (source.IsEORE()) {
      // nothing to do
      return true;
    }
    // If we get here it must be a data event
    const RawDataEvent & ev = dynamic_cast<const RawDataEvent &>(source);
    for (size_t i = 0; i < ev.NumBlocks(); ++i) {
      result.AddPlane(ConvertPlane(ev.GetBlock(i),
                                   ev.GetID(i)));
    }
    return true;
  }

  /********************************************/

  class LegacyDEPFETConverterPlugin : public DataConverterPlugin, public DEPFETConverterBase {
    virtual bool GetStandardSubEvent(StandardEvent &, const Event & source) const;
  private:
    LegacyDEPFETConverterPlugin() : DataConverterPlugin(Event::str2id("_DEP")){}
    static LegacyDEPFETConverterPlugin const m_instance;
  };

  bool LegacyDEPFETConverterPlugin::GetStandardSubEvent(StandardEvent & result, const eudaq::Event & source) const {
    std::cout << "GetStandardSubEvent " << source.GetRunNumber() << ", " << source.GetEventNumber() << std::endl;
    if (source.IsBORE()) {
      //FillInfo(source);
      return true;
    } else if (source.IsEORE()) {
      // nothing to do
      return true;
    }
    // If we get here it must be a data event
    const DEPFETEvent & ev = dynamic_cast<const DEPFETEvent &>(source);
    for (size_t i = 0; i < ev.NumBoards(); ++i) {
      result.AddPlane(ConvertPlane(ev.GetBoard(i).GetDataVector(),
                                   ev.GetBoard(i).GetID()));
    }
    return true;
  }

  LegacyDEPFETConverterPlugin const LegacyDEPFETConverterPlugin::m_instance;

} //namespace eudaq
