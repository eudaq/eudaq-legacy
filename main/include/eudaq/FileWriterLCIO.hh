#ifndef EUDAQ_INCLUDED_FileWriterLCIO
#define EUDAQ_INCLUDED_FileWriterLCIO

#include "eudaq/FileWriter.hh"
#include "eudaq/debugtools.hh"
//#include "eudaq/FileSerializer.hh"

namespace IO { class LCWriter; }
namespace lcio { using namespace IO; }

namespace eudaq {

  class FileWriterLCIO : public FileWriter {
  public:
    FileWriterLCIO(const std::string &);
    virtual void StartRun(unsigned);
    virtual void WriteEvent(const DetectorEvent &);
    virtual unsigned long long FileBytes() const;
    virtual ~FileWriterLCIO();
  private:

      lcio::LCWriter *m_lcwriter; /// The lcio writer
      bool m_fileopened; /// We have to keep track whether a file is open ourselves
  };

}

#endif // #ifndef EUDAQ_INCLUDED_FileWriterLCIO
