#include <iostream>
#include <ostream>
#include <cctype>
//#include <csignal>

#include "eudaq/Utils.hh"
#include "eudaq/EnvCollector.hh"
#include "eudaq/Logger.hh"
#include "eudaq/OptionParser.hh"

#include <unistd.h>

class TestEnvCollector : public eudaq::EnvCollector {
public:
  TestEnvCollector(const std::string & runcontrol,
                   const std::string & listenaddress)
    : eudaq::EnvCollector(runcontrol, listenaddress),
      done(false)
  {}
  void OnConnect(const eudaq::ConnectionInfo & id) {
    std::cout << "Connect:    " << id << std::endl;
  }
  void OnDisconnect(const eudaq::ConnectionInfo & id) {
    std::cout << "Disconnect: " << id << std::endl;
  }
  virtual void OnReceive(const eudaq::EnvMessage & ev) {
    std::cout << "On Receive in TestEnvCollector " <<ev << std::endl;
  }
  virtual void OnConfigure(const std::string & param) {
    std::cout << "Configure: " << param << std::endl;
    SetStatus(eudaq::Status::LVL_OK);
  }
  virtual void OnStartRun(unsigned param) {
    std::cout << "Start Run: " << param << std::endl;
  }
  virtual void OnStopRun() {
    std::cout << "Stop Run" << std::endl;
  }
  virtual void OnTerminate() {
    std::cout << "Terminating" << std::endl;
    done = true;
  }
  virtual void OnReset() {
    std::cout << "Reset" << std::endl;
    SetStatus(eudaq::Status::LVL_OK);
  }
  virtual void OnStatus() {
    std::cout << "Status - " << m_status << std::endl;
    //SetStatus(eudaq::Status::LVL_WARNING, "Only joking");
  }
  virtual void OnUnrecognised(const std::string & cmd, const std::string & param) {
    std::cout << "Unrecognised: (" << cmd.length() << ") " << cmd;
    if (param.length() > 0) std::cout << " (" << param << ")";
    std::cout << std::endl;
    SetStatus(eudaq::Status::LVL_ERROR, "Just testing");
  }
  int m_loglevel;
  bool done;
};

// static TestEnvCollector * g_ptr = 0;
// void ctrlc_handler(int) {
//   if (g_ptr) g_ptr->done = 1;
// }

int main(int /*argc*/, const char ** argv) {
  eudaq::OptionParser op("EUDAQ Env Collector", "1.0", "A comand-line version of the Env Collector");
  eudaq::Option<std::string> rctrl(op, "r", "runcontrol", "tcp://localhost:44000", "address",
                                   "The address of the RunControl application");
  eudaq::Option<std::string> addr (op, "a", "listen-address", "tcp://44008", "address",
                                   "The address on which to listen for Env connections");

  try {
    op.Parse(argv);
    EUDAQ_LOG_LEVEL("NONE");
    TestEnvCollector fw(rctrl.Value(), addr.Value());
    //g_ptr = &fw;
    //std::signal(SIGINT, &ctrlc_handler);
    do {
      eudaq::mSleep(10);
    } while (!fw.done);
    std::cout << "Quitting" << std::endl;
  } catch (...) {
    return op.HandleMainException();
  }
  return 0;
}
