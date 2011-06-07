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
#include <ostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <algorithm>

#define GET(d, i) getlittleendian<unsigned>(&(d)[(i)*4])

namespace eudaq {
  static const int dbg = 0; // 0=off, 1=structure, 2=structure+data

  class NIConverterPlugin : public DataConverterPlugin {
    typedef std::vector<unsigned char> datavect;
    typedef std::vector<unsigned char>::const_iterator datait;
  public:
    virtual void Initialize(const Event & bore, const Configuration & /*c*/) {
      m_boards = from_string(bore.GetTag("BOARDS"), 0);
      m_ids.clear();
      for (unsigned i = 0; i < m_boards; ++i) {
        unsigned id = from_string(bore.GetTag("ID" + to_string(i)), i);
        m_ids.push_back(id);
      }
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
        EUDAQ_WARN("Ignoring bad event " + to_string(source.GetEventNumber()));
        return false;
      }
      const datavect & data0 = rawev.GetBlock(0);
      const datavect & data1 = rawev.GetBlock(1);
      unsigned header0 = GET(data0, 0);
      unsigned header1 = GET(data1, 0);
      unsigned tluid = GetTriggerID(source);
      if (dbg) std::cout << "TLU id = " << hexdec(tluid, 4) << std::endl;
      unsigned pivot = GET(data0, 1) >> 16;
      if (dbg) std::cout << "Pivot = " << hexdec(pivot, 4) << std::endl;
      datait it0 = data0.begin() + 8;
      datait it1 = data1.begin() + 8;
      unsigned board = 0;
      while (it0 < data0.end() && it1 < data1.end()) {
        unsigned id = board;
        if (id < m_ids.size()) id = m_ids[id];
        if (dbg) std::cout << "Sensor " << board << ", id = " << id << std::endl;
        if (dbg) std::cout << "Mimosa_header0 = " << hexdec(header0) << std::endl;
        if (dbg) std::cout << "Mimosa_header1 = " << hexdec(header1) << std::endl;
        if (it0 + 2 >= data0.end()) {
          EUDAQ_WARN("Trailing rubbish in first frame");
          break;
        }
        if (it1 + 2 >= data1.end()) {
          EUDAQ_WARN("Trailing rubbish in second frame");
          break;
        }
        if (dbg) std::cout << "Mimosa_framecount0 = " << hexdec(GET(it0, 0)) << std::endl;
        if (dbg) std::cout << "Mimosa_framecount1 = " << hexdec(GET(it1, 0)) << std::endl;
        unsigned len0 = GET(it0, 1);
        if (dbg) std::cout << "Mimosa_wordcount0 = " << hexdec(len0 & 0xffff, 4)
                           << ", " << hexdec(len0 >> 16, 4) << std::endl;
        if ((len0 & 0xffff) != (len0 >> 16)) {
          EUDAQ_WARN("Mismatched lengths decoding first frame (" +
                     to_string(len0 & 0xffff) + ", " + to_string(len0 >> 16) + ")");
          len0 = std::max(len0 & 0xffff, len0 >> 16);
        }
        len0 &= 0xffff;
        unsigned len1 = GET(it1, 1);
        if (dbg) std::cout << "Mimosa_wordcount1 = " << hexdec(len1 & 0xffff, 4)
                           << ", " << hexdec(len1 >> 16, 4) << std::endl;
        if ((len1 & 0xffff) != (len1 >> 16)) {
          EUDAQ_WARN("Mismatched lengths decoding second frame (" +
                     to_string(len1 & 0xffff) + ", " + to_string(len1 >> 16) + ")");
          len1 = std::max(len1 & 0xffff, len1 >> 16);
        }
        len1 &= 0xffff;
        if (it0 + len0*4 + 12 > data0.end()) {
          EUDAQ_WARN("Bad length in first frame");
          break;
        }
        if (it1 + len1*4 + 12 > data1.end()) {
          EUDAQ_WARN("Bad length in second frame");
          break;
        }
        StandardPlane plane(id, "NI", "MIMOSA26");
        plane.SetSizeZS(1152, 576, 0, 2, StandardPlane::FLAG_WITHPIVOT | StandardPlane::FLAG_DIFFCOORDS);
        plane.SetTLUEvent(tluid);
        plane.SetPivotPixel(pivot);
        DecodeFrame(plane, len0, it0+8, 0);
        DecodeFrame(plane, len1, it1+8, 1);
        result.AddPlane(plane);
        if (dbg) std::cout << "Mimosa_trailer0 = " << hexdec(GET(it0, len0+2)) << std::endl;
        if (dbg) std::cout << "Mimosa_trailer1 = " << hexdec(GET(it1, len1+2)) << std::endl;
        it0 += len0*4 + 16;
        it1 += len1*4 + 16;
        if (it0 <= data0.end()) header0 = GET(it0, -1);
        if (it1 <= data1.end()) header1 = GET(it1, -1);
        ++board;
      }
      return true;
    }
    void DecodeFrame(StandardPlane & plane, size_t len, datait it, int frame) const {
      std::vector<unsigned short> vec;
      for (size_t i = 0; i < len; ++i) {
        unsigned v = GET(it, i);
        vec.push_back(v & 0xffff);
        vec.push_back(v >> 16);
      }

        unsigned npixels = 0;
        for (size_t i = 0; i < vec.size(); ++i) {
          //  std::cout << "  " << i << " : " << hexdec(vec[i]) << std::endl;
          if (i == vec.size() - 1) break;
          unsigned numstates = vec[i] & 0xf;
          unsigned row = vec[i]>>4 & 0x7ff;
          if (numstates+1 > vec.size()-i) {
            // Ignoring bad line
            //std::cout << "Ignoring bad line " << row << " (too many states)" << std::endl;
            break;
          }
          if (dbg>1) std::cout << "Hit line " << (vec[i] & 0x8000 ? "* " : ". ") << row
                             << ", states " << numstates << ":";
          bool pivot = row >= (plane.PivotPixel() / 16);
          for (unsigned s = 0; s < numstates; ++s) {
            unsigned v = vec.at(++i);
            unsigned column = v>>2 & 0x7ff;
            unsigned num = v & 3;
            if (dbg>1) std::cout << (s ? "," : " ") << column;
            if (dbg>1) if ((v&3) > 0) std::cout << "-" << (column + num);
            for (unsigned j = 0; j < num+1; ++j) {
              plane.PushPixel(column+j, row, 1, pivot, frame);
            }
            npixels += num + 1;
          }
          if (dbg>1) std::cout << std::endl;
        }
        if (dbg) std::cout << "Total pixels " << frame << " = " << npixels << std::endl;
        //++offset;


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
    NIConverterPlugin() : DataConverterPlugin("NI"), m_boards(0) {}
    unsigned m_boards;
    std::vector<int> m_ids;
    static NIConverterPlugin const m_instance;
  };

  NIConverterPlugin const NIConverterPlugin::m_instance;


} //namespace eudaq
