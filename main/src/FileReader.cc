#include "eudaq/FileReader.hh"
#include "eudaq/FileNamer.hh"
#include "eudaq/PluginManager.hh"

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

//   const StandardEvent & FileReader::GetStandardEvent() const {
//     if (!m_sev) {
//       counted_ptr<StandardEvent> sevent(new StandardEvent);
//       const DetectorEvent & dev = GetDetectorEvent();
//       for (size_t i = 0; i < dev.NumEvents(); ++i) {
//         const eudaq::Event * subevent = dev.GetEvent(i);

//         try {
//           const DataConverterPlugin * converterplugin = PluginManager::GetInstance().GetPlugin(subevent->GetType());
//           converterplugin->GetStandardSubEvent(*sevent, *subevent);
//           //std::fprintf(m_file, "Event %d %d\n", devent.GetEventNumber(), standardevent->m_x.size());
//         } catch(eudaq::Exception & e) {
//           //std::cout <<  e.what() << std::endl;
//           std::cout <<  "FileWriterText::WriteEvent(): Ignoring event type "
//                     <<  subevent->GetType() << std::endl;
//           continue;
//         }
//       }
//       m_sev = sevent;
//     }
//     return *m_sev;
//   }

}
