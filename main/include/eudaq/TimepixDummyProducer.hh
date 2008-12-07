#include "eudaq/Producer.hh"
//#include "eudaq/Logger.hh"
//#include "eudaq/RawDataEvent.hh"
//#include "eudaq/Utils.hh"
//#include "eudaq/OptionParser.hh"
//#include <iostream>
//#include <ostream>
//#include <cctype>

//typedef eudaq::StringEvent TestEvent;

class TimepixDummyProducer : public eudaq::Producer
{
  public:
    TimepixDummyProducer(const std::string & name, const std::string & runcontrol);

    void Event(unsigned short *timepixdata);
    void SimpleEvent();
    void BlobEvent();
//	TestEvent ev(m_run, ++m_ev, str);
//	//ev.SetTag("Debug", "foo");
//	SendEvent(ev);
//  }

    virtual void OnConfigure(const eudaq::Configuration & param);
    virtual void OnStartRun(unsigned param);
    virtual void OnStopRun();
    virtual void OnTerminate();
    virtual void OnReset();
    virtual void OnStatus();
    virtual void OnUnrecognised(const std::string & cmd, const std::string & param);

    bool done;
    unsigned m_run, m_ev;
};
