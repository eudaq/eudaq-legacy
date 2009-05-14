#ifndef EUDAQ_INCLUDED_FileReader
#define EUDAQ_INCLUDED_FileReader

#include "eudaq/FileSerializer.hh"
#include "eudaq/DetectorEvent.hh"
#include "eudaq/StandardEvent.hh"
#include "eudaq/counted_ptr.hh"
#include <string>

namespace eudaq {

  class FileReader {
  public:
    FileReader(const std::string & filename, const std::string & filepattern = "");
    bool NextEvent(size_t skip = 0);
    std::string FileName() const { return m_filename; }
    unsigned RunNumber() const;
    const DetectorEvent & Event() const;
  private:
    std::string m_filename;
    FileDeserializer m_des;
    counted_ptr<eudaq::Event> m_ev;
  };

}

#endif // EUDAQ_INCLUDED_FileReader
