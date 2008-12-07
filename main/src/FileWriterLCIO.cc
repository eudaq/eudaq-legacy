#include "eudaq/FileWriterLCIO.hh"
#include "eudaq/FileNamer.hh"
#include <iostream>

namespace eudaq {

  namespace {
    static RegisterFileWriter<FileWriterLCIO> reg("lcio");
  }

  FileWriterLCIO::FileWriterLCIO(const std::string & whatIsThisFor) {
      // get an LCWriter from the factory
      std::cout << "EUDAQ_DEBUG: This is FileWriterLCIO::FileWriterLCIO("<<
	  whatIsThisFor <<")"<< std::endl;
  }

  void FileWriterLCIO::StartRun(unsigned runnumber) {
      // close an open file

    // open a new file
      
  }

  void FileWriterLCIO::WriteEvent(const DetectorEvent & ev) {
//    if (!m_ser) EUDAQ_THROW("FileWriterNative: Attempt to write unopened file");
//    m_ser->write(ev);
      
      //disentangle the event

  }

  FileWriterLCIO::~FileWriterLCIO() {
      // close file
      // delete the LCWriter?
  }

  unsigned long long FileWriterLCIO::FileBytes() const { return 0; }

}
