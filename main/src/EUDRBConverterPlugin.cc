#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/Exception.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/EUDRBEvent.hh"
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
#  include "EUTelSetupDescription.h"
#  include "EUTelEventImpl.h"
#  include "EUTelSparseDataImpl.h"
#  include "EUTelSimpleSparsePixel.h"
#endif

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>

namespace eudaq {

  void map_1x1(unsigned & x, unsigned & y, unsigned c, unsigned r, unsigned, unsigned, unsigned) {
    x = c;
    y = r;
  }

  void map_4x1(unsigned & x, unsigned & y, unsigned c, unsigned r, unsigned m, unsigned nc, unsigned) {
    unsigned mat = (m == 0 || m == 3) ? 3-m : m;
    x = c + mat * nc;
    y = r;
  }

  void map_2x2(unsigned & x, unsigned & y, unsigned c, unsigned r, unsigned m, unsigned nc, unsigned nr) {
    x = (m < 2) ? c : 2*nc - 1 - c;
    y = (m == 0 || m == 3) ? r : 2*nr - 1 - r;
  }

  struct SensorInfo {
    typedef void (*mapfunc_t)(unsigned & x, unsigned & y, unsigned c, unsigned r, unsigned m, unsigned nc, unsigned nr);
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
    void FillInfo(const Event & bore, const Configuration &) {
      unsigned nboards = from_string(bore.GetTag("BOARDS"), 0);
      //std::cout << "FillInfo " << nboards << std::endl;
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
    bool ConvertStandard(StandardEvent & stdEvent, const Event & eudaqEvent) const;
    StandardPlane ConvertPlane(const std::vector<unsigned char> & data, unsigned id) const {
      const BoardInfo & info = GetInfo(id);
      StandardPlane plane(id, "EUDRB", info.Sensor().name);
      plane.m_tluevent = (data[data.size()-7] << 8) | data[data.size()-6];
      plane.m_xsize = info.Sensor().width;
      plane.m_ysize = info.Sensor().height;
      plane.m_pivotpixel = ((data[5] & 0x3) << 16) | (data[6] << 8) | data[7];
      if (info.m_mode == BoardInfo::MODE_ZS) {
        ConvertZS(plane, data, info);
      } else {
        ConvertRaw(plane, data, info);
      }
      plane.m_flags |= StandardPlane::FLAG_NEGATIVE;
      return plane;
    }
    static void ConvertZS(StandardPlane & plane, const std::vector<unsigned char> & alldata, const BoardInfo & info);
    static void ConvertRaw(StandardPlane & plane, const std::vector<unsigned char> & data, const BoardInfo & info);
    bool ConvertLCIO(lcio::LCEvent & lcioEvent, const Event & eudaqEvent) const;
  protected:
    static size_t NumPlanes(const Event & event) {
      if (const RawDataEvent * ev = dynamic_cast<const RawDataEvent *>(&event)) {
        return ev->NumBlocks();
      } else if (const EUDRBEvent * ev = dynamic_cast<const EUDRBEvent *>(&event)) {
        return ev->NumBoards();
      }
      return 0;
    }
    static std::vector<unsigned char> GetPlane(const Event & event, size_t i) {
      if (const RawDataEvent * ev = dynamic_cast<const RawDataEvent *>(&event)) {
        return ev->GetBlock(i);
      } else if (const EUDRBEvent * ev = dynamic_cast<const EUDRBEvent *>(&event)) {
        return ev->GetBoard(i).GetDataVector();
      }
      return std::vector<unsigned char>();
    }
    static size_t GetID(const Event & event, size_t i) {
      if (const RawDataEvent * ev = dynamic_cast<const RawDataEvent *>(&event)) {
        return ev->GetID(i);
      } else if (const EUDRBEvent * ev = dynamic_cast<const EUDRBEvent *>(&event)) {
        return ev->GetBoard(i).GetID();
      }
      return 0;
    }
    std::vector<BoardInfo> m_info;
  };

  /********************************************/

  class EUDRBConverterPlugin : public DataConverterPlugin, public EUDRBConverterBase {
  public:
    virtual void Initialize(const Event & e, const Configuration & c) {
      FillInfo(e, c);
    }

    virtual bool GetStandardSubEvent(StandardEvent & result, const Event & source) const {
      return ConvertStandard(result, source);
    }

#if USE_LCIO
    virtual bool GetLCIOSubEvent(lcio::LCEvent & lcioEvent, const Event & eudaqEvent) const {
      return ConvertLCIO(lcioEvent, eudaqEvent);
    }
#endif

  private:
    EUDRBConverterPlugin() : DataConverterPlugin("EUDRB") {}
    static EUDRBConverterPlugin const m_instance;
  };

  EUDRBConverterPlugin const EUDRBConverterPlugin::m_instance;

  /********************************************/

  class LegacyEUDRBConverterPlugin : public DataConverterPlugin, public EUDRBConverterBase {
    virtual void Initialize(const eudaq::Event & e, const eudaq::Configuration & c) {
      FillInfo(e, c);
    }

    virtual bool GetStandardSubEvent(StandardEvent & result, const Event & source) const {
      return ConvertStandard(result, source);
    }

#if USE_LCIO
    virtual bool GetLCIOSubEvent(lcio::LCEvent & lcioEvent, const Event & eudaqEvent) const {
      return ConvertLCIO(lcioEvent, eudaqEvent);
    }
#endif

  private:
    LegacyEUDRBConverterPlugin() : DataConverterPlugin(Event::str2id("_DRB")){}
    static LegacyEUDRBConverterPlugin const m_instance;
  };

  LegacyEUDRBConverterPlugin const LegacyEUDRBConverterPlugin::m_instance;

  /********************************************/

  bool EUDRBConverterBase::ConvertStandard(StandardEvent & result, const Event & source) const {
    if (source.IsBORE()) {
      // shouldn't happen
      return true;
    } else if (source.IsEORE()) {
      // nothing to do
      return true;
    }
    // If we get here it must be a data event
    size_t numplanes = NumPlanes(source);
    for (size_t i = 0; i < numplanes; ++i) {
      result.AddPlane(ConvertPlane(GetPlane(source, i), GetID(source, i)));
    }
    return true;
  }

  void EUDRBConverterBase::ConvertZS(StandardPlane & plane, const std::vector<unsigned char> & alldata, const BoardInfo & info) {
    unsigned headersize = 8, trailersize = 8;
    if (info.m_version > 2) {
      headersize += 8;
      EUDAQ_THROW("EUDRB V3 decoding not yet implemented");
    }
    bool padding = (alldata[alldata.size()-trailersize-4] == 0);
    unsigned npixels = (alldata.size() - headersize - trailersize) / 4 - padding;
    plane.SetSizeZS(info.Sensor().width, info.Sensor().height, npixels);
    plane.m_mat.resize(plane.m_pix[0].size());
    const unsigned char * data = &alldata[headersize];
    for (unsigned i = 0; i < npixels; ++i) {
      int mat = (data[4*i] >> 6), col = 0, row = 0;
      if (info.m_version < 2) {
        row = ((data[4*i] & 0x7) << 5) | (data[4*i+1] >> 3);
        col = ((data[4*i+1] & 0x7) << 4) | (data[4*i+2] >> 4);
      } else {
        row = ((data[4*i] & 0x3F) << 3) |  (data[4*i+1] >> 5);
        col = ((data[4*i+1] & 0x1F) << 4) | (data[4*i+2] >> 4);
      }
      unsigned x, y;
      info.Sensor().mapfunc(x, y, col, row, mat, info.Sensor().cols, info.Sensor().rows);
      plane.m_x[i] = x;
      plane.m_y[i] = y;
      plane.m_mat[i] = mat;
      plane.m_pix[0][i] = ((data[4*i+2] & 0x0F) << 8) | (data[4*i+3]);
    }
  }

  void EUDRBConverterBase::ConvertRaw(StandardPlane & plane, const std::vector<unsigned char> & data, const BoardInfo & info) {
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
    plane.m_flags |= StandardPlane::FLAG_NEEDCDS;
    plane.m_mat.resize(plane.m_pix[0].size());
    const unsigned char * ptr = &data[headersize];
    for (unsigned row = 0; row < info.Sensor().rows; ++row) {
      for (unsigned col = 0; col < info.Sensor().cols; ++col) {
        if (missingpixel && row == info.Sensor().rows-1 && col == info.Sensor().cols-1) break; // last pixel is not transferred
        for (size_t frame = 0; frame < info.Frames(); ++frame) {
          for (size_t mat = 0; mat < info.Sensor().mats; ++mat) {
            unsigned x = 0, y = 0;
            info.Sensor().mapfunc(x, y, col, row, mat, info.Sensor().cols, info.Sensor().rows);
            size_t i = x + y*info.Sensor().width;
            if (frame == 0) {
              plane.m_x[i] = x;
              plane.m_y[i] = y;
              plane.m_mat[i] = mat;
              if (info.m_version < 2) {
                plane.m_pivot[i] = (row << 7 | col) >= plane.m_pivotpixel;
              } else {
                plane.m_pivot[i] = (row << 9 | col) >= plane.m_pivotpixel;
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

#if USE_LCIO
  bool EUDRBConverterBase::ConvertLCIO(lcio::LCEvent & result, const Event & source) const {

    if (source.IsBORE()) {
      // shouldn't happen
      return true;
    } else if (source.IsEORE()) {
      // nothing to do
      return true;
    }
    // If we get here it must be a data event

#if USE_EUTELESCOPE

    result.parameters().setValue( eutelescope::EUTELESCOPE::EVENTTYPE, eutelescope::kDE );

    // prepare the collections for the rawdata and the zs ones
    std::auto_ptr< lcio::LCCollectionVec > rawDataCollection ( new lcio::LCCollectionVec (lcio::LCIO::TRACKERRAWDATA) ) ;
    std::auto_ptr< lcio::LCCollectionVec > zsDataCollection  ( new lcio::LCCollectionVec (lcio::LCIO::TRACKERDATA) ) ;

    // set the proper cell encoder
    CellIDEncoder< TrackerRawDataImpl > rawDataEncoder ( eutelescope::EUTELESCOPE::MATRIXDEFAULTENCODING, rawDataCollection.get() );
    CellIDEncoder< TrackerDataImpl    > zsDataEncoder  ( eutelescope::EUTELESCOPE::ZSDATADEFAULTENCODING, zsDataCollection.get()  );

    // a description of the setup
    std::vector< eutelescope::EUTelSetupDescription * >  setupDescription;

    // to understand if we have problem with de-syncronisation, let
    // me prepare a Boolean switch and a vector of size_t to contain the
    // pivot pixel position
    bool outOfSyncFlag = false;
    std::vector<size_t > pivotPixelPosVec;

    //const RawDataEvent & rawDataEvent = dynamic_cast< const RawDataEvent & > ( source ) ;

    size_t numplanes = NumPlanes(source);
    for (size_t iPlane = 0; iPlane < numplanes; ++iPlane) {

      StandardPlane plane = ConvertPlane(GetPlane(source, iPlane), GetID(source, iPlane));

      // The current detector is ...
      eutelescope::EUTelPixelDetector * currentDetector = 0x0;
      if ( plane.m_sensor == "MIMOTEL" ) {

        currentDetector = new eutelescope::EUTelMimoTelDetector;
        std::string mode;
        plane.IsZS() ? mode = "ZS" : mode = "RAW2";
        currentDetector->setMode( mode );
        if ( result.getEventNumber() == 0 ) {
          setupDescription.push_back( new eutelescope::EUTelSetupDescription( currentDetector )) ;
        }
      } else if ( plane.m_sensor == "MIMOSA18" ) {

        currentDetector = new eutelescope::EUTelMimosa18Detector;
        std::string mode;
        plane.IsZS() ? mode = "ZS" : mode = "RAW2";
        currentDetector->setMode( mode );
        if ( result.getEventNumber() == 0 ) {
          setupDescription.push_back( new eutelescope::EUTelSetupDescription( currentDetector ));
        }
      } else {

        EUDAQ_ERROR("Unrecognised sensor type in LCIO converter: " + plane.m_sensor);
        return true;

      }
      std::vector<size_t > markerVec = currentDetector->getMarkerPosition();

      if ( plane.IsZS() ) {
        // storage of ZS data is done here
        zsDataEncoder["sensorID"] = iPlane;
        zsDataEncoder["sparsePixelType"] = eutelescope::kEUTelSimpleSparsePixel;

        // get the total number of pixel. This is written in the
        // eudrbBoard and to get it in a easy way pass through the eudrbDecoder
        size_t nPixel = plane.m_x.size();

        // prepare a new TrackerData for the ZS data
        std::auto_ptr<lcio::TrackerDataImpl > zsFrame( new lcio::TrackerDataImpl );
        zsDataEncoder.setCellID( zsFrame.get() );

        // this is the structure that will host the sparse pixel
        std::auto_ptr< eutelescope::EUTelSparseDataImpl< eutelescope::EUTelSimpleSparsePixel > >
          sparseFrame( new eutelescope::EUTelSparseDataImpl< eutelescope::EUTelSimpleSparsePixel > ( zsFrame.get() ) );

        // prepare a sparse pixel to be added to the sparse data
        std::auto_ptr< eutelescope::EUTelSimpleSparsePixel > sparsePixel( new eutelescope::EUTelSimpleSparsePixel );
        for ( size_t iPixel = 0; iPixel < nPixel; ++iPixel ) {

          // the data contain also the markers, so we have to strip
          // them out. First I need to have the original position
          // (with markers in) and then calculate how many pixels I
          // have to remove
          size_t originalX = (size_t)plane.m_x[ iPixel ] ;

          if ( find( markerVec.begin(), markerVec.end(), originalX ) == markerVec.end() ) {
            // the original X is not on a marker column, so I need
            // to remove a certain number of pixels depending on the
            // position

            // this counts the number of markers found on the left
            // of the original X
            short  diff = ( short ) count_if ( markerVec.begin(),markerVec.end(), std::bind2nd( std::less<short> (), originalX ) );
            sparsePixel->setXCoord( originalX - diff );

            // no problem instead with the Y coordinate
            sparsePixel->setYCoord( (size_t)plane.m_y[ iPixel ] );

            // last the pixel charge. The CDS is automatically
            // calculated by the EUDRB
            sparsePixel->setSignal( (size_t)plane.m_pix[0][ iPixel ] );

            // in case of DEBUG
            // streamlog_out ( DEBUG0 ) << ( *(sparsePixel.get() ) ) << endl;

            // now add this pixel to the sparse frame
            sparseFrame->addSparsePixel( sparsePixel.get() );
          } else {
            // the original X was on a marker column, so we don't
            // need to process this pixel any further and of course
            // we don't have to add it to the sparse frame.

            /*
              streamlog_out ( DEBUG0 ) << "Found a sparse pixel ("<< iPixel
                                       <<")  on a marker column. Not adding it to the frame" << endl
                                       << (* (sparsePixel.get() ) ) << endl;
            */

          }

        }

        // perfect! Now add the TrackerData to the collection
        zsDataCollection->push_back( zsFrame.release() );

        // for the debug of the synchronization
        pivotPixelPosVec.push_back( plane.m_pivotpixel );

      } else {

        // storage of RAW data is done here according to the mode
        rawDataEncoder["xMin"]     = currentDetector->getXMin();
        rawDataEncoder["xMax"]     = currentDetector->getXMax() - markerVec.size();
        rawDataEncoder["yMin"]     = currentDetector->getYMin();
        rawDataEncoder["yMax"]     = currentDetector->getYMax();
        rawDataEncoder["sensorID"] = iPlane;

        // get the full vector of CDS
        std::vector<short> cdsVec = plane.GetPixels<short>();

        // now we have to strip out the marker cols from the CDS
        // value. To do this I need a vector of short large enough
        // to accommodate the full matrix without the markers
        std::vector<short > cdsStrippedVec( currentDetector->getYNoOfPixel() * ( currentDetector->getXNoOfPixel() - markerVec.size() ) );

        // I need also two iterators, one for the stripped vec and
        // one for the original one.
        std::vector<short >::iterator currentCDSPos = cdsStrippedVec.begin();
        std::vector<short >::iterator cdsBegin      = cdsVec.begin();

        // now loop over all the pixels
        for ( size_t y = 0; y < currentDetector->getYNoOfPixel(); ++y ) {
          size_t offset = y * currentDetector->getXNoOfPixel();
          std::vector<size_t >::iterator marker = markerVec.begin();

          // first copy from the beginning of the row to the first
          // marker column
          currentCDSPos = copy( cdsBegin + offset, cdsBegin + ( *(marker) + offset ), currentCDSPos );

            // now copy from the next column to the next marker into a
            // while loop
            while ( marker != markerVec.end() ) {
              if ( marker < markerVec.end() - 1 ) {
                currentCDSPos = copy( cdsBegin + ( *(marker) + 1 + offset ), cdsBegin + ( *(marker + 1) + offset ), currentCDSPos );
              } else {
                // now from the last marker column to the end of the
                // row
                currentCDSPos = copy( cdsBegin + ( *(marker) + 1 + offset ), cdsBegin + offset + currentDetector->getXNoOfPixel(), currentCDSPos );
              }
              ++marker;
            }
        }

        // this is the right place to prepare the TrackerRawData
        // object
        std::auto_ptr< lcio::TrackerRawDataImpl > cdsFrame( new lcio::TrackerRawDataImpl );
        rawDataEncoder.setCellID( cdsFrame.get() );

        // add the cds stripped values to the current TrackerRawData
        cdsFrame->setADCValues( cdsStrippedVec ) ;

        // put the pivot pixel in the timestamp field of the
        // TrackerRawData. I know that is not correct, but this is
        // the only place where I can put this info
        cdsFrame->setTime( plane.m_pivotpixel );

        // this is also the right place to add the pivot pixel to
        // the pivot pixel vector for synchronization checks
        pivotPixelPosVec.push_back( plane.m_pivotpixel );

        // now append the TrackerRawData object to the corresponding
        // collection releasing the auto pointer
        rawDataCollection->push_back( cdsFrame.release() );
      }

      delete currentDetector;

    }

    if ( result.getEventNumber() == 0 ) {

      // do this only in the first event
      std::auto_ptr< lcio::LCCollectionVec > eudrbSetupCollection( new LCCollectionVec ( lcio::LCIO::LCGENERICOBJECT) );

      for ( size_t iPlane = 0 ; iPlane < setupDescription.size() ; ++iPlane ) {
        eudrbSetupCollection->push_back( setupDescription.at( iPlane ) );
      }

      result.addCollection( eudrbSetupCollection.release(), "eudrbSetup" );
    }

    // check if all the boards where running in synchronous mode or
    // not. Remember that the last pivot pixel entry is the one of the
    // master board.
    std::vector<size_t >::iterator masterBoardPivotAddress = pivotPixelPosVec.end() - 1;
    std::vector<size_t >::iterator slaveBoardPivotAddress  = pivotPixelPosVec.begin();
    while ( slaveBoardPivotAddress < masterBoardPivotAddress ) {
      if ( *slaveBoardPivotAddress - *masterBoardPivotAddress >= 2 ) {
        outOfSyncFlag = true;

        // we don't need to continue looping over all boards if one of
        // them is already out of sync
        break;
      }
      ++slaveBoardPivotAddress;
    }
    if ( outOfSyncFlag ) {

      if ( result.getEventNumber()  < 20 ) {
        // in this case we have the responsibility to tell the user that
        // the event was out of sync
        std::cout << "Event number " << result.getEventNumber() << " seems to be out of sync" << std::endl;
        std::vector<size_t >::iterator masterBoardPivotAddress = pivotPixelPosVec.end() - 1;
        std::vector<size_t >::iterator slaveBoardPivotAddress  = pivotPixelPosVec.begin();
        while ( slaveBoardPivotAddress < masterBoardPivotAddress ) {
          // print out all the slave boards first
          std::cout << " --> Board (S) " <<  std::setw(3) << setiosflags( std::ios::right )
                    << slaveBoardPivotAddress - pivotPixelPosVec.begin() << resetiosflags( std::ios::right )
                    << " = " << std::setw(15) << setiosflags( std::ios::right )
                    << (*slaveBoardPivotAddress) << resetiosflags( std::ios::right )
                    << " (" << std::setw(15) << setiosflags( std::ios::right )
                    << (signed) (*masterBoardPivotAddress) - (signed) (*slaveBoardPivotAddress) << resetiosflags( std::ios::right)
                    << ")" << std::endl;
          ++slaveBoardPivotAddress;
        }
        // print out also the master. It is impossible that the master
        // is out of sync with respect to itself, but for completeness...
        std::cout  << " --> Board (M) "  <<  std::setw(3) << setiosflags( std::ios::right )
                   << slaveBoardPivotAddress - pivotPixelPosVec.begin() << resetiosflags( std::ios::right )
                   << " = " << std::setw(15) << setiosflags( std::ios::right )
                   << (*slaveBoardPivotAddress) << resetiosflags( std::ios::right )
                   << " (" << std::setw(15)  << setiosflags( std::ios::right )
                   << (signed) (*masterBoardPivotAddress) - (signed) (*slaveBoardPivotAddress) << resetiosflags( std::ios::right)
                   << ")" << std::endl;

      } else if ( result.getEventNumber()  == 20 ) {
        // if the number of consecutive warnings is equal to the maximum
        // allowed, don't bother the user anymore with this message,
        // because it's very likely the run was taken unsynchronized on
        // purpose
        std::cout << "The maximum number of unsychronized events has been reached." << std::endl
                  << "Assuming the run was taken in asynchronous mode" << std::endl;
      }
    }

    // add the collections to the event only if not empty!
    if ( rawDataCollection->size() != 0 ) {
      result.addCollection( rawDataCollection.release(), "rawdata" );
    }

    if ( zsDataCollection->size() != 0 ) {
      result.addCollection( zsDataCollection.release(), "zsdata" );
    }

    return true;
#else
    (void)result;
    EUDAQ_ERROR("EUDAQ was not compiled with EUTelescope support: cannot convert EUDRB event to LCIO format");
    return false;
#endif
  }
#endif

} //namespace eudaq
