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
      StandardPlane plane(id, "FORTIS", "?");
      plane.m_tluevent = getlittleendian<unsigned>(&data[4]);
      unsigned npixels = m_NumRows * m_NumColumns;
      plane.SetSizeZS(512,512,npixels); // create an array that 
      size_t i = 0;
      for (size_t Row = 0; Row < m_NumRows; ++Row) {
	for (size_t Column = 0; Column < m_NumColumns; ++Column) {

	  unsigned offset = (Column + 2 + Row*(m_NumColumns+2))*sizeof (short);
	  unsigned short d = getlittleendian<unsigned short>(&data[offset]);
	  plane.m_x[i] = Column + m_InitialColumn ;
	  plane.m_y[i] = Row + m_InitialRow;
	  plane.m_pix[0][i] = 0xffff - d ; // FORTIS data has a pedestal near 0xFFFF with excursions below this when charge is deposited. The initial [0] refers to the frame number.
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
      std::string configstr = source.GetTag("CONFIG");
      eudaq::Configuration config(configstr, "Producer.FORTIS");
      
      std::cout << "Config = " << configstr << std::endl;

      m_NumRows = config.Get("NumRows", 512);
      m_NumColumns = config.Get("NumColumns", 512);
      m_InitialRow = config.Get("InitialRow", 0);
      m_InitialColumn = config.Get("InitialColumn", 0);

      std::cout << " Nrows , NColumns = " << m_NumRows << "  ,  " <<  m_NumColumns << std::endl;
      std::cout << " Initial row , column = " << m_InitialRow << "  ,  " <<  m_InitialColumn << std::endl;

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


} //namespace eudaq
