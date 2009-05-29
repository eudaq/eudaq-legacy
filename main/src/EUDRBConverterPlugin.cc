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

  void map_1x1(double & x, double & y, unsigned c, unsigned r, unsigned, unsigned, unsigned) {
    x = c;
    y = r;
  }

  void map_4x1(double & x, double & y, unsigned c, unsigned r, unsigned m, unsigned nc, unsigned) {
    unsigned mat = (m == 0 || m == 3) ? 3-m : m;
    x = c + mat * nc;
    y = r;
  }

  void map_2x2(double & x, double & y, unsigned c, unsigned r, unsigned m, unsigned nc, unsigned nr) {
    x = (m < 2) ? c : 2*nc - 1 - c;
    y = (m == 0 || m == 3) ? r : 2*nr - 1 - r;
  }

  struct SensorInfo {
    typedef void (*mapfunc_t)(double & x, double & y, unsigned c, unsigned r, unsigned m, unsigned nc, unsigned nr);
    SensorInfo(const std::string name, unsigned c, unsigned r, unsigned m, unsigned w, unsigned h, mapfunc_t mfunc = 0)
      : name(name), cols(c), rows(r), mats(m), width(w), height(h), mapfunc(mfunc)
      {}
    std::string name;
    unsigned cols, rows, mats, width, height;
    mapfunc_t mapfunc;
  };

  static const SensorInfo g_sensors[] = {
    SensorInfo("MIMOSTAR2",   0,   0, 0,  132,  128),
    SensorInfo("MIMOTEL",    66, 256, 4,  264,  256, map_4x1),
    SensorInfo("MIMOTEL",    66, 256, 4,  264,  256, map_4x1),
    SensorInfo("MIMOSA18",  256, 256, 4,  512,  512, map_2x2),
    SensorInfo("MIMOSA5",     0,   0, 0, 1024, 1024),
    SensorInfo("MIMOSA26", 1152, 576, 1, 1152,  576, map_1x1)
  };

  struct BoardInfo {
    enum E_DET  { DET_NONE = -1, DET_MIMOSTAR2, DET_MIMOTEL, DET_MIMOTEL_NEWORDER, DET_MIMOSA18, DET_MIMOSA5, DET_MIMOSA26 };
    enum E_MODE { MODE_NONE = -1, MODE_ZS, MODE_RAW1, MODE_RAW2, MODE_RAW3 };
    BoardInfo() : m_version(0), m_det(DET_MIMOTEL), m_mode(MODE_RAW3) {}
    BoardInfo(const Event & ev, int brd)
      : m_version(0), m_det(DET_NONE), m_mode(MODE_NONE)
    {
      std::string det = ev.GetTag("DET" + to_string(brd));
      if (det == "") det = ev.GetTag("DET", "MIMOTEL");

      for (size_t i = 0; i < sizeof g_sensors / sizeof *g_sensors; ++i) {
        if (det == g_sensors[i].name) {
          m_det = (E_DET)i;
          break;
        }
      }
      if (m_det == DET_NONE) EUDAQ_THROW("Unknown detector in EUDRBConverterPlugin: " + det);

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
    const SensorInfo & Sensor() const {
      return g_sensors[m_det];
    }
    unsigned Frames() const {
      return m_mode > 0 ? m_mode : 1;
    }
    int m_version;
    E_DET m_det;
    E_MODE m_mode;
  };

  class EUDRBConverterBase {
  public:
    void FillInfo(const Event & bore) const {
      unsigned nboards = from_string(bore.GetTag("BOARDS"), 0);
      std::cout << "FillInfo " << nboards << std::endl;
      for (unsigned i = 0; i < nboards; ++i) {
        unsigned id = from_string(bore.GetTag("ID" + to_string(i)), i);
        if (m_info.size() <= id) m_info.resize(id+1);
        m_info[id] = BoardInfo(bore, i);
      }
    }
    const BoardInfo & GetInfo(unsigned id) const {
      if (id >= m_info.size() || m_info[id].m_version < 1) EUDAQ_THROW("Unrecognised ID ("+to_string(id)+", num="+to_string(m_info.size())+") converting EUDRB event");
      return m_info[id];
    }
    StandardPlane ConvertPlane(const std::vector<unsigned char> & data, unsigned id) const {
      const BoardInfo & info = GetInfo(id);
      StandardPlane plane(id, "EUDRB", info.Sensor().name);
      plane.m_tluevent = (data[data.size()-7] << 8) | data[data.size()-6];
      plane.m_xsize = info.Sensor().width;
      plane.m_ysize = info.Sensor().height;
      if (info.m_mode == BoardInfo::MODE_ZS) {
        ConvertZS(plane, data, info);
      } else {
        ConvertRaw(plane, data, info);
      }
      return plane;
    }
    static void ConvertZS(StandardPlane & plane, const std::vector<unsigned char> & data, const BoardInfo & info) {
      unsigned headersize = 8, trailersize = 8;
      if (info.m_version > 2) {
        headersize += 8;
        EUDAQ_THROW("EUDRB V3 decoding not yet implemented");
      }
      bool padding = (data[data.size()-trailersize-4] == 0);
      unsigned npixels = (data.size() - headersize - trailersize - 4*padding) / 4;
      plane.SetSizeZS(info.Sensor().width, info.Sensor().height, npixels);
      for (unsigned i = 0; i < npixels; ++i) {
        int mat = (data[4*i] >> 6), col = 0, row = 0;
        if (info.m_version < 2) {
          row = ((data[4*i] & 0x7) << 5) | (data[4*i+1] >> 3);
          col = ((data[4*i+1] & 0x7) << 4) | (data[4*i+2] >> 4);
        } else {
          row = ((data[4*i] & 0x3F) << 3) |  (data[4*i+1] >> 5);
          col = ((data[4*i+1] & 0x1F) << 4) | (data[4*i+2] >> 4);
        }
        info.Sensor().mapfunc(plane.m_x[i], plane.m_y[i], col, row, mat, info.Sensor().cols, info.Sensor().rows);
        plane.m_pix[0][i] = ((data[4*i+2] & 0x0F) << 8) | (data[4*i+3]);
      }
    }
    static void ConvertRaw(StandardPlane & plane, const std::vector<unsigned char> & data, const BoardInfo & info) {
      unsigned headersize = 8, trailersize = 8;
      if (info.m_version > 2) {
        headersize += 8;
        EUDAQ_THROW("EUDRB V3 decoding not yet implemented");
      }
      unsigned possible1 = 2 *  info.Sensor().cols * info.Sensor().rows      * info.Sensor().mats * info.Frames();
      unsigned possible2 = 2 * (info.Sensor().cols * info.Sensor().rows - 1) * info.Sensor().mats * info.Frames();
      bool missingpixel = false;
      if (data.size() - headersize - trailersize == possible1) {
        // OK
      } else if (data.size() - headersize - trailersize == possible2) {
        missingpixel = true;
      } else {
        EUDAQ_THROW("Bad raw data size (" + to_string(data.size() - headersize - trailersize)+") expecting "
                    + to_string(possible1) + " or " + to_string(possible2));
      }
      //unsigned npixels = info.Sensor().cols * info.Sensor().rows * info.Sensor().mats;
      plane.SetSizeRaw(info.Sensor().width, info.Sensor().height, info.Frames(), true);
      unsigned pivot = ((data[5] & 0x3) << 16) | (data[6] << 8) | data[7];
      const unsigned char * ptr = &data[headersize];
      for (unsigned row = 0; row < info.Sensor().rows; ++row) {
        for (unsigned col = 0; col < info.Sensor().cols; ++col) {
          if (missingpixel && row == info.Sensor().rows-1 && col == info.Sensor().cols-1) break; // last pixel is not transferred
          for (size_t frame = 0; frame < info.Frames(); ++frame) {
            for (size_t mat = 0; mat < info.Sensor().mats; ++mat) {
              double x = 0, y = 0;
              info.Sensor().mapfunc(x, y, col, row, mat, info.Sensor().cols, info.Sensor().rows);
              size_t i = x + y*info.Sensor().width;
              if (frame == 0) {
                plane.m_x[i] = x;
                plane.m_y[i] = y;
                if (info.m_version < 2) {
                  plane.m_pivot[i] = (row << 7 | col) >= pivot;
                } else {
                  plane.m_pivot[i] = (row << 9 | col) >= pivot;
                }
              }
              short pix = *ptr++ << 8;
              pix |= *ptr++;
              pix &= 0xfff;
              plane.m_pix[frame][i] = pix;
            }
          }
        }
      }
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
    std::cout << "GetStandardSubEvent " << source.GetRunNumber() << ", " << source.GetEventNumber() << std::endl;
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
