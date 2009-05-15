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

  FileWriterLCIO::FileWriterLCIO(const std::string & whatIsThisFor) : m_fileopened(false)
  {
    // get an LCWriter from the factory
    m_lcwriter = lcio::LCFactory::getInstance()->createLCWriter() ;

    std::cout << "EUDAQ_DEBUG: This is FileWriterLCIO::FileWriterLCIO("<<
      whatIsThisFor <<")"<< std::endl;


  }

  void FileWriterLCIO::StartRun(unsigned runnumber)
  {
    // close an open file
    if (m_fileopened)
      {
        m_lcwriter->close();
        m_fileopened=false;
      }

    // open a new file
    try{  m_lcwriter->open( FileNamer(m_filepattern).Set('X', ".slcio").Set('R', runnumber),
                            lcio::LCIO::WRITE_NEW ) ;
      m_fileopened=true;
    }
    catch( lcio::IOException& e ){
      std::cout << e.what() << std::endl ;
      ///FIXME Error message to rin control and logger
    }

  }

  void FileWriterLCIO::WriteEvent(const DetectorEvent & devent)
  {
    std::cout << "EUDAQ_DEBUG: FileWriterLCIO::WriteEvent() processing event "
              <<  devent.GetRunNumber () <<"." << devent.GetEventNumber () << std::endl;

    lcio::LCEventImpl * lcevent =  new lcio::LCEventImpl;

    lcevent->setEventNumber( devent.GetEventNumber () );
    lcevent->setRunNumber(   devent.GetRunNumber () );
    lcevent->setTimeStamp(   devent.GetTimestamp() );

    //disentangle the detector event
    for (size_t i = 0; i < devent.NumEvents(); i++)
      {
        Event const * subevent = devent.GetEvent(i);

        DataConverterPlugin const * converterplugin;
        try
          {
            converterplugin = PluginManager::GetInstance().GetPlugin( subevent->GetType() );
          }
        catch(eudaq::Exception & e)
          {
            //std::cout <<  e.what() << std::endl;
            std::cout <<  "FileWriterLCIO::WriteEvent(): Ignoring event type "
                      <<  subevent->GetType() << std::endl;

            continue;
          }

        lcio::LCEvent * lcsubevent = converterplugin->GetLCIOEvent( subevent );

        // take the collections from the subevent and add them to the event
        std::vector<std::string> const * colletionsnames = lcsubevent->getCollectionNames();

        for (std::vector<std::string>::const_iterator nameiter = colletionsnames->begin() ;
             nameiter < colletionsnames->end() ; nameiter ++)
          {
            lcio::LCCollectionVec * col = dynamic_cast<lcio::LCCollectionVec *>(lcsubevent->takeCollection(*nameiter));
            // have to restore non-transientness of the colleciton so it's written to disc
            col->setTransient(false);
            lcevent->addCollection( col, *nameiter );
            // FIXME: what to do if collection already exists?
          }
        // FIXME: what to do with the parameters?

        //now that the subevent is empty we have to delete it
        delete lcsubevent;
      }

    // only write non-empty events
    if( !lcevent->getCollectionNames()->empty() )
      {
        // std::cout << " FileWriterLCIO::WriteEvent() : doing the actual writing : " <<std::flush;
        m_lcwriter->writeEvent( lcevent );
        // std::cout << " done" <<std::endl;
      }


    delete lcevent;
  }

  FileWriterLCIO::~FileWriterLCIO()
  {
    // close an open file
    if (m_fileopened)
      m_lcwriter->close();
  }

  unsigned long long FileWriterLCIO::FileBytes() const { return 0; }

}

#endif // USE_LCIO
