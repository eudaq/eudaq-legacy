#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/EUDRBEvent.hh"
#include "eudaq/Logger.hh"

#if USE_LCIO
#  include "IMPL/LCEventImpl.h"
#  include "IMPL/TrackerRawDataImpl.h"
#  include "IMPL/LCCollectionVec.h"
#  include "lcio.h"
#endif

#include <iostream>
#include <string>
#include <map>

namespace eudaq {

  //struct SensorInfo {
  //  unsigned x, y, m;
  //};

  struct BoardInfo {
    enum E_DET  { DET_MIMOSTAR2, DET_MIMOTEL, DET_MIMOTEL_NEWORDER, DET_MIMOSA18, DET_MIMOSA5 };
    enum E_MODE { MODE_RAW3, MODE_RAW2, MODE_ZS };
    BoardInfo() : m_version(0), m_det(DET_MIMOTEL), m_mode(MODE_RAW3) {}
    BoardInfo(const Event & ev, int brd)
      : m_version(0), m_det(DET_MIMOTEL), m_mode(MODE_RAW3)
    {
      std::string det = ev.GetTag("DET" + to_string(brd));
      if (det == "") det = ev.GetTag("DET", "MIMOTEL");

      if (det == "MIMOTEL") m_det = DET_MIMOTEL;
      else if (det == "MIMOSTAR2") m_det = DET_MIMOSTAR2;
      else if (det == "MIMOSA18") m_det = DET_MIMOSA18;
      else if (det == "MIMOSA1") m_det = DET_MIMOSA18; // DEBUGGING (can be removed)
      else EUDAQ_THROW("Unknown detector in EUDRBConverterPlugin: " + det);

      std::string mode = ev.GetTag("MODE" + to_string(brd));
      if (mode == "") mode = ev.GetTag("MODE", "RAW3");

      if (mode == "ZS") m_mode = MODE_ZS;
      else if (mode == "RAW2") m_mode = MODE_RAW2;
      else if (mode == "RAW3") m_mode = MODE_RAW3;
      else EUDAQ_THROW("Unknown mode in EUDRBConverterPlugin: " + mode);

      m_version = ev.GetTag("VERSION", 0);
      if (m_version == 0) { // No VERSION tag, try to guess it
        if (det == "MIMOTEL" && mode != "RAW2") {
          m_version = 1;
        } else {
          m_version = 2;
        }
        EUDAQ_WARN("No EUDRB Version tag, guessing VERSION=" + to_string(m_version));
      }
    }
    int m_version;
    E_DET m_det;
    E_MODE m_mode;
  };

  class EUDRBConverterBase {
  public:
    void FillMap(const Event & bore) const {
      unsigned nboards = from_string(bore.GetTag("BOARDS"), 0);
      for (unsigned i = 0; i < nboards; ++i) {
        unsigned id = from_string(bore.GetTag("ID" + to_string(i)), i);
        m_map[id] = BoardInfo(bore, i);
      }
    }
  protected:
    mutable std::map<unsigned, BoardInfo> m_map;
  };

  /********************************************/

  class EUDRBConverterPlugin : public DataConverterPlugin, public EUDRBConverterBase {
  public:
    //virtual lcio::LCEvent * GetLCIOEvent( eudaq::Event const * ee ) const;

    virtual bool GetStandardSubEvent(StandardEvent &, const eudaq::Event &) const;

  private:
    EUDRBConverterPlugin() : DataConverterPlugin("EUDRB") {}

    static EUDRBConverterPlugin const m_instance;
  };

  EUDRBConverterPlugin const EUDRBConverterPlugin::m_instance;

  bool EUDRBConverterPlugin::GetStandardSubEvent(StandardEvent & result, const eudaq::Event & source) const {
    if (source.IsBORE()) {
      FillMap(source);
      // TODO: copy some info into StandardEvent?
      return true;
    } else if (source.IsEORE()) {
      // nothing to do
      return true;
    }
    // If we get here it must be a data event
    const RawDataEvent & ev = dynamic_cast<const RawDataEvent &>(source);
    for (size_t i = 0; i < ev.NumBlocks(); ++i) {
      StandardPlane plane;
      // TODO: fill pixels (and other info) into plane...
      result.AddPlane(plane);
    }
    return true;
  }

  /********************************************/

  class LegacyEUDRBConverterPlugin : public DataConverterPlugin, public EUDRBConverterBase {
    virtual bool GetStandardSubEvent(StandardEvent &, const eudaq::Event & source) const;
  private:
    LegacyEUDRBConverterPlugin() : DataConverterPlugin(Event::str2id("_DRB")){}
    static LegacyEUDRBConverterPlugin const m_instance;
  };

  bool LegacyEUDRBConverterPlugin::GetStandardSubEvent(StandardEvent & result, const eudaq::Event & source) const {
    if (source.IsBORE()) {
      FillMap(source);
      // TODO: copy some info into StandardEvent?
      return true;
    } else if (source.IsEORE()) {
      // nothing to do
      return true;
    }
    // If we get here it must be a data event
    const EUDRBEvent & ev = dynamic_cast<const EUDRBEvent &>(source);
    for (size_t i = 0; i < ev.NumBoards(); ++i) {
      StandardPlane plane;
      // TODO: fill pixels (and other info) into plane...
      result.AddPlane(plane);
    }
    return true;
  }

  LegacyEUDRBConverterPlugin const LegacyEUDRBConverterPlugin::m_instance;

} //namespace eudaq
