#include <iostream>
#include <ostream>
#include <cctype>
//#include <csignal>
#include "TestEnvMon.hh"
//#include "eudaq/Utils.hh"

#include "eudaq/Logger.hh"
#include "eudaq/OptionParser.hh"

#include <unistd.h>

#include "GraphWindow.hh"
#include <TApplication.h>
//#include "TGraphSet.hh"
#include "eudaq/EnvTranscoder.hh"
#include <sstream>

class TestEnvMon : public eudaq::EnvCollector { //, public TApplication {
public:
  TestEnvMon(const std::string & runcontrol,
                   const std::string & listenaddress,
		GraphWindow *window)
    : eudaq::EnvCollector(runcontrol, listenaddress),
      done(false), _window(window) {
	_window->SetTGraphSet(&set);
	/*  
	set.AddGraph("georg1","degC");
	set.AddPoint("georg1",1,1);
	set.AddPoint("georg1",2,2);
	set.AddPoint("georg1",3,5);
	
	set.AddGraph("georg2","degC");
	set.AddPoint("georg2",2,1);
	set.AddPoint("georg2",3,2);
	set.SetLimits(0,10,0,10);
	TCanvas *fCanvas = _window->GetCanvas();
	set.Update(fCanvas);
	
	double a,b;
	set.GetXLimits(a,b);*/ 
   }
  void OnConnect(const eudaq::ConnectionInfo & id) {
    std::cout << "Connect:    " << id << std::endl;
  }
  void OnDisconnect(const eudaq::ConnectionInfo & id) {
    std::cout << "Disconnect: " << id << std::endl;
  }
  virtual void OnReceive(const eudaq::EnvMessage & ev) {
    //std::cout << "On Receive in TestEnvCollector: " <<ev << std::endl;
	  EnvSet s = Decode(ev);
	  //std::cout << "HERE: " << s.name << " / " << s.value << " / " << s.date  << " / " << s.unit<< std::endl;
	  _window->Update();
	  parseToGui(s);
	 
	  
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
    bool done;
  protected:
	  
  TGraphSet set;
  int m_loglevel;
  GraphWindow *_window;
  
  EnvSet Decode(const eudaq::EnvMessage & ev) {
	  EnvTranscoder t;
	  std::stringstream ss;
	  
	  ss << ev;
	  std::cout << "To decode: " << ss.str() << std::endl;
	  return t.decode(ss.str());
	  
  }
  void parseToGui(EnvSet &e) {
	set.AddGraph(e.name,e.unit);
	//_window->getLegend()->AddEntry(set.GetGraph(e.name), (e.name + std::string("[")+e.unit+std::string("]")).c_str());
	  
	set.AddPoint(e.name,e.GetSeconds(),e.value);
	  TCanvas *fCanvas = _window->GetCanvas();
	set.Redraw(fCanvas);
	//_window->getLegend()->Draw();
  }

};






int main(int argc, char ** argv) {
  eudaq::OptionParser op("EUDAQ Test Env Mon", "1.0", "A comand-line version of the Env Monitor");
  eudaq::Option<std::string> rctrl(op, "r", "runcontrol", "tcp://localhost:44000", "address",
                                   "The address of the RunControl application");
  eudaq::Option<std::string> addr (op, "a", "listen-address", "tcp://44008", "address",
                                   "The address on which to listen for Env connections");

  try {
    op.Parse(argv);
    EUDAQ_LOG_LEVEL("NONE");
   TApplication theApp("App",&argc, argv);
    GraphWindow *g =  new GraphWindow(gClient->GetRoot(),600,400);
    TestEnvMon fw(rctrl.Value(), addr.Value(),g);

	
	  
	theApp.Run();
	  //g_ptr = &fw;
    //std::signal(SIGINT, &ctrlc_handler);
   //do {
   //   eudaq::mSleep(10);
   // } while (!fw.done);
    std::cout << "Quitting" << std::endl;
  } catch (...) {
    return op.HandleMainException();
  }
  return 0;
}

#ifdef __CINT__
#pragma link C++ class TestEnvMon-;
#endif
