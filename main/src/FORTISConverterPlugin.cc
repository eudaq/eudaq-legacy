#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Configuration.hh"
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

  /********************************************/

  class FORTISConverterPlugin : public DataConverterPlugin {
  public:
    //virtual lcio::LCEvent * GetLCIOEvent( eudaq::Event const * ee ) const;

    virtual bool GetStandardSubEvent(StandardEvent &, const eudaq::Event &) const;

  private:
    StandardPlane ConvertPlane(const std::vector<unsigned char> & data, unsigned id) const {
      size_t expected = ((m_NumColumns + 2) * m_NumRows) * sizeof (short);
      if (data.size() < expected)
        EUDAQ_THROW("Bad Data Size (" + to_string(data.size()) + " < " + to_string(expected) + ")");
      StandardPlane plane(id, "FORTIS", "FORTIS");
      unsigned npixels = m_NumRows * m_NumColumns;
      plane.SetSizeZS(512, 512, npixels);
      size_t i = 0;
      for (size_t Row = 0; Row < m_NumRows; ++Row) {
        for (size_t Column = 0; Column < m_NumColumns; ++Column) {
          unsigned offset = (Column + 2 + (m_NumColumns + 2) * Row) * sizeof (short);
          unsigned short d = getlittleendian<unsigned short>(&data[offset]);
          plane.m_x[i] = Column + m_InitialColumn ;
          plane.m_y[i] = Row + m_InitialRow;
          // FORTIS data has a pedestal near 0xFFFF with excursions below this when charge is deposited.
          // The initial [0] refers to the frame number.
          plane.m_pix[0][i] = 0xffff - d ;
          i++;
        }
      }
      return plane;
    }
    FORTISConverterPlugin() : DataConverterPlugin("FORTIS"),
                              m_NumRows(512), m_NumColumns(512),
                              m_InitialRow(0), m_InitialColumn(0) {}
    mutable unsigned m_NumRows, m_NumColumns, m_InitialRow , m_InitialColumn;

    static FORTISConverterPlugin const m_instance;
  };

  FORTISConverterPlugin const FORTISConverterPlugin::m_instance;

  bool FORTISConverterPlugin::GetStandardSubEvent(StandardEvent & result, const Event & source) const {
    if (source.IsBORE()) {
      m_NumRows = from_string(source.GetTag("NumRows"), 512);
      m_NumColumns = from_string(source.GetTag("NumColumns"), 512);
      m_InitialRow = from_string(source.GetTag("InitialRow"), 0);
      m_InitialColumn = from_string(source.GetTag("InitialColumn"), 0);

      std::cout << " Nrows , NColumns = " << m_NumRows << "  ,  " <<  m_NumColumns << std::endl;
      std::cout << " Initial row , column = " << m_InitialRow << "  ,  " <<  m_InitialColumn << std::endl;

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


} //namespace eudaq
