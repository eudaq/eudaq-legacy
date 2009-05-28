#include "eudaq/Monitor.hh"
#include "eudaq/Logger.hh"
#include "eudaq/PluginManager.hh"

#define EUDAQ_MAX_EVENTS_PER_IDLE 32

namespace eudaq {

  Monitor::Monitor(const std::string & name, const std::string & runcontrol, const std::string & datafile) :
    CommandReceiver("Monitor", name, runcontrol, false),
    m_run(0),
    m_callstart(false),
    m_reader(0)
  {
    if (datafile != "") {
      // set offline
      m_reader = counted_ptr<FileReader>(new FileReader(datafile));
      PluginManager::ConvertToStandard(m_reader->Event()); // process BORE
      //m_callstart = true;
      std::cout << "DEBUG: Reading file " << datafile << " -> " << m_reader->FileName() << std::endl;
      //OnStartRun(m_run);
    }
    StartThread();
  }

  bool Monitor::ProcessEvent() {
    //std::cout << "processevent" << std::endl;
    if (!m_reader.get()) return false;
    if (!m_reader->NextEvent()) return false;
    try {
      const DetectorEvent & dev = m_reader->Event();
      if (dev.IsBORE()) m_lastbore = counted_ptr<DetectorEvent>(new DetectorEvent(dev));
      OnEvent(PluginManager::ConvertToStandard(dev));
    } catch (const InterruptedException &) {
      return false;
    }
    return true;
  }

  void Monitor::OnIdle() {
    if (m_callstart) {
      m_callstart = false;
      OnStartRun(m_run);
    }
    bool processed = false;
    for (int i = 0; i < EUDAQ_MAX_EVENTS_PER_IDLE; ++i) {
      if (ProcessEvent()) {
        processed = true;
      } else {
        //if (offline) OnTerminate();
        break;
      }
    }
    if (!processed) mSleep(1);
  }

  void Monitor::OnStartRun(unsigned param) {
    m_run = param;
    m_reader = counted_ptr<FileReader>(new FileReader(to_string(m_run)));
    EUDAQ_INFO("Starting run " + to_string(m_run));
  }

  void Monitor::OnStopRun() {
    m_reader->Interrupt();
  }

}
