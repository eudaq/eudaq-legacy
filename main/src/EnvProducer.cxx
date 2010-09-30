#include "eudaq/CommandReceiver.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/Logger.hh"
#include "eudaq/Utils.hh"
#include "eudaq/EnvTranscoder.hh"

#include <iostream>
#include <ostream>
#include <time.h>

class EnvProducer : public eudaq::CommandReceiver {
public:
  EnvProducer(const std::string & name, const std::string & runcontrol) :
    CommandReceiver("EnvProducer", name, runcontrol),
    done(false) {
	
  }
  void Run() {
	   EUDAQ_ENV_CONNECT("EnvProducer","","tcp://localhost:44008");
    while (!done) {
      eudaq::mSleep(1000);
	srand( time(NULL));
	    
     EUDAQ_ENV(_transcoder.encode(ENV_TEMP,"Chip1",rand() % 10 +1 ,"DEGC"));
     EUDAQ_ENV(_transcoder.encode(ENV_TEMP,"Chip2",(rand() % 10 +1) +5  ,"DEGC"));
    }
  }
private:
  EnvTranscoder _transcoder;
  virtual void OnConfigure(const eudaq::Configuration & config) {
    // This gets called whenever the DAQ is configured
    // You can get values from the configuration file with config.Get(name, default);
    unsigned someparam = config.Get("Parameter", 0);
    std::cout << "Configuring: " << config.Name() << std::endl
              << "Some Parameter = " << someparam << std::endl;
    SetStatus(eudaq::Status::LVL_OK, "Configured (" + config.Name() + ")");
    EUDAQ_WARN("This is a test error message");
  }
  virtual void OnStartRun(unsigned param) {
    // This gets called whenever a new run is started
    // It receives the new run number as a parameter
    std::cout << "Start Run: " << param << std::endl;
    SetStatus(eudaq::Status::LVL_OK, "Running");
  }
  virtual void OnStopRun() {
    // This gets called whenever a run is stopped
    std::cout << "Stopping Run" << std::endl;
    SetStatus(eudaq::Status::LVL_OK, "Stopped");
  }
  virtual void OnTerminate() {
    // This gets called when we are asked by Run Control to terminate
    done = true; // So that the Run Loop terminates
  }
  bool done;
};

int main(int /*argc*/, const char ** argv) {
  eudaq::OptionParser op("EUDAQ EnvProducer", "1.0", "Sends Env-Variables to Run Control");
  eudaq::Option<std::string> rctrl(op, "r", "runcontrol", "tcp://localhost:44000", "address",
                                   "The address of the RunControl application");
  eudaq::Option<std::string> name (op, "n", "name", "TestEnv", "string",
                                   "The name of this EnvProducer");

  try {
    op.Parse(argv);
    EnvProducer listener(name.Value(), rctrl.Value());
    listener.Run();
  } catch (...) {
    return op.HandleMainException();
  }
  return 0;
}
