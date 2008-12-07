#ifndef EUDAQ_INCLUDED_FileWriterLCIO
#define EUDAQ_INCLUDED_FileWriterLCIO

#include "eudaq/FileWriter.hh"
#include "eudaq/debugtools.hh"
//#include "eudaq/FileSerializer.hh"

namespace eudaq {

  class FileWriterLCIO : public FileWriter {
  public:
    FileWriterLCIO(const std::string &);
    virtual void StartRun(unsigned);
    virtual void WriteEvent(const DetectorEvent &);
    virtual unsigned long long FileBytes() const;
    virtual ~FileWriterLCIO();
  private:
//    FileSerializer * m_ser;
  };

}

#endif // #ifndef EUDAQ_INCLUDED_FileWriterLCIO
