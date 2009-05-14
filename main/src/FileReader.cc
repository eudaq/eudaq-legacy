#include "eudaq/FileReader.hh"
#include "eudaq/FileNamer.hh"

namespace eudaq {

  FileReader::FileReader(const std::string & file, const std::string & filepattern)
    : m_filename(FileNamer(filepattern).Set('X', ".raw").SetReplace('R', file)),
      m_des(m_filename),
      m_ev(EventFactory::Create(m_des)) {
  }

  bool FileReader::NextEvent(size_t skip) {
    if (!m_des.HasData()) {
      return false;
    }
    for (size_t i = 0; i <= skip; ++i) {
      if (!m_des.HasData()) break;
      m_ev = eudaq::EventFactory::Create(m_des);
    }
    return true;
  }

  unsigned FileReader::RunNumber() const {
    return m_ev->GetRunNumber();
  }

  const DetectorEvent & FileReader::Event() const {
    return dynamic_cast<const DetectorEvent &>(*m_ev);
  }

}
