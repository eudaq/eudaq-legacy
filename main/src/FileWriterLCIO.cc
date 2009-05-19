#include "eudaq/FileWriterLCIO.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"
#include <iostream>

#if USE_LCIO
#  include <IO/ILCFactory.h>
#  include <IMPL/LCEventImpl.h>
#  include <IMPL/LCCollectionVec.h>
#  include <EVENT/LCIO.h>
#  include <Exceptions.h>
#  include <IMPL/LCTOOLS.h>
#  include <IO/LCWriter.h>
#  include <lcio.h>

namespace eudaq {

  namespace {
    static RegisterFileWriter<FileWriterLCIO> reg("lcio");
  }

  FileWriterLCIO::FileWriterLCIO(const std::string & param)
    : m_lcwriter(lcio::LCFactory::getInstance()->createLCWriter()), // get an LCWriter from the factory
      m_fileopened(false)
  {
    std::cout << "EUDAQ_DEBUG: FileWriterLCIO::FileWriterLCIO(" << param << ")" << std::endl;
  }

  void FileWriterLCIO::StartRun(unsigned runnumber) {
    // close an open file
    if (m_fileopened) {
      m_lcwriter->close();
      m_fileopened = false;
    }

    // open a new file
    try {
      m_lcwriter->open(FileNamer(m_filepattern).Set('X', ".slcio").Set('R', runnumber),
                       lcio::LCIO::WRITE_NEW) ;
      m_fileopened=true;
    } catch(const lcio::IOException & e) {
      std::cout << e.what() << std::endl ;
      ///FIXME Error message to run control and logger
    }
  }

  void FileWriterLCIO::WriteEvent(const DetectorEvent & devent) {
    std::cout << "EUDAQ_DEBUG: FileWriterLCIO::WriteEvent() processing event "
              <<  devent.GetRunNumber () <<"." << devent.GetEventNumber () << std::endl;

    lcio::LCEventImpl lcevent;

    lcevent.setEventNumber(devent.GetEventNumber());
    lcevent.setRunNumber(  devent.GetRunNumber());
    lcevent.setTimeStamp(  devent.GetTimestamp());

    //disentangle the detector event
    for (size_t i = 0; i < devent.NumEvents(); i++) {
      Event const * subevent = devent.GetEvent(i);

      try {
        DataConverterPlugin const * converterplugin = PluginManager::GetInstance().GetPlugin(subevent->GetType());
        converterplugin->GetLCIOSubEvent(lcevent, *subevent);
      } catch(eudaq::Exception & e) {
        //std::cout <<  e.what() << std::endl;
        std::cout <<  "FileWriterLCIO::WriteEvent(): Ignoring event type "
                  <<  subevent->GetType() << std::endl;
      }
    }

    // only write non-empty events
    if (!lcevent.getCollectionNames()->empty()) {
      // std::cout << " FileWriterLCIO::WriteEvent() : doing the actual writing : " <<std::flush;
      m_lcwriter->writeEvent(&lcevent);
      // std::cout << " done" <<std::endl;
    }
  }

  FileWriterLCIO::~FileWriterLCIO() {
    // close an open file
    if (m_fileopened) {
      m_lcwriter->close();
    }
  }

  unsigned long long FileWriterLCIO::FileBytes() const { return 0; }

}

#endif // USE_LCIO
