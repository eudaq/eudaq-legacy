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
#include <vector>

namespace eudaq {

  struct SensorInfo {
    SensorInfo(const std::string name, unsigned w, unsigned h)
      : name(name), width(w), height(h)
      {}
    std::string name;
    unsigned width, height;
  };

  static const SensorInfo g_sensors[] = {
    SensorInfo("MIMOSTAR2", 0, 0),
    SensorInfo("MIMOTEL", 264, 256),
    SensorInfo("MIMOTEL", 264, 256),
    SensorInfo("MIMOSA18", 512, 512),
    SensorInfo("MIMOSA5", 0, 0),
    SensorInfo("MIMOSA26", 0, 0)
  };

  struct BoardInfo {
    enum E_DET  { DET_MIMOSTAR2, DET_MIMOTEL, DET_MIMOTEL_NEWORDER, DET_MIMOSA18, DET_MIMOSA5, DET_MIMOSA26 };
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
      else if (det == "MIMOSA26") m_det = DET_MIMOSA26;
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
    void FillInfo(const Event & bore) const {
      unsigned nboards = from_string(bore.GetTag("BOARDS"), 0);
      for (unsigned i = 0; i < nboards; ++i) {
        unsigned id = from_string(bore.GetTag("ID" + to_string(i)), i);
        if (m_info.size() <= id) m_info.resize(id+1);
        m_info[id] = BoardInfo(bore, i);
      }
    }
    const BoardInfo & GetInfo(unsigned id) const {
      if (id >= m_info.size() || m_info[id].m_version < 1) EUDAQ_THROW("Unrecognised ID converting EUDRB event");
      return m_info[id];
    }
    StandardPlane ConvertPlane(const std::vector<unsigned char> & data, unsigned id) const {
      const BoardInfo & info = GetInfo(id);
      StandardPlane plane(id, "EUDRB", g_sensors[info.m_det].name);
      plane.m_tluevent = (data[data.size()-7] << 8) | data[data.size()-6];
      plane.m_xsize = g_sensors[info.m_det].width;
      plane.m_ysize = g_sensors[info.m_det].height;
      return plane;
    }
  protected:
    mutable std::vector<BoardInfo> m_info;
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

  bool EUDRBConverterPlugin::GetStandardSubEvent(StandardEvent & result, const Event & source) const {
    if (source.IsBORE()) {
      FillInfo(source);
      // TODO: copy some info into StandardEvent?
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

  class LegacyEUDRBConverterPlugin : public DataConverterPlugin, public EUDRBConverterBase {
    virtual bool GetStandardSubEvent(StandardEvent &, const Event & source) const;
  private:
    LegacyEUDRBConverterPlugin() : DataConverterPlugin(Event::str2id("_DRB")){}
    static LegacyEUDRBConverterPlugin const m_instance;
  };

  bool LegacyEUDRBConverterPlugin::GetStandardSubEvent(StandardEvent & result, const eudaq::Event & source) const {
    if (source.IsBORE()) {
      FillInfo(source);
      // TODO: copy some info into StandardEvent?
      return true;
    } else if (source.IsEORE()) {
      // nothing to do
      return true;
    }
    // If we get here it must be a data event
    const EUDRBEvent & ev = dynamic_cast<const EUDRBEvent &>(source);
    for (size_t i = 0; i < ev.NumBoards(); ++i) {
      result.AddPlane(ConvertPlane(ev.GetBoard(i).GetDataVector(),
                                   ev.GetBoard(i).GetID()));
    }
    return true;
  }

  LegacyEUDRBConverterPlugin const LegacyEUDRBConverterPlugin::m_instance;

} //namespace eudaq
