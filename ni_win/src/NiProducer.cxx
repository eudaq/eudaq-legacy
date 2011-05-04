// vs_data.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include "eudaq/Producer.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"
#include "eudaq/OptionParser.hh"
#include "eudaq/counted_ptr.hh"

using eudaq::RawDataEvent;


class QACProducer: public eudaq::Producer {
public:
	QACProducer(const std::string & runcontrol) :
		eudaq::Producer("QAC", runcontrol),
		m_run(0),
		m_ev(0),
		done(false),
		m_running(false),
		m_stoppedrun(false)
		{
	}
	void MainLoop() {
		do {
			/*
			 * waiting when button was press
			 * if (!m_running) 			continue;
			 */
			if (!m_running) 	eudaq::mSleep(1000);		continue;
			eudaq::RawDataEvent ev("MVD", m_run, m_ev);
			SendEvent(ev);
			m_ev++;
		} while (!done);
	}
	virtual void OnConfigure(const eudaq::Configuration & param) {
		try {
			std::cout << "Configuring (" << param.Name() << ")..." << std::endl;
			std::cout << "...Configured (" << param.Name() << ")" << std::endl;
			EUDAQ_INFO("Configured (" + param.Name() + ")");
			SetStatus(eudaq::Status::LVL_OK, "Configured (" + param.Name()
					+ ")");
		} catch (const std::exception & e) {
			printf("Caught exception: %s\n", e.what());
			SetStatus(eudaq::Status::LVL_ERROR, "Configuration Error");
		} catch (...) {
			printf("Unknown exception\n");
			SetStatus(eudaq::Status::LVL_ERROR, "Configuration Error");
		}
	}
	virtual void OnStartRun(unsigned param) {
		try {
			m_run = param;
			m_ev = 0;
			std::cout << "Start Run: " << param << std::endl;
			RawDataEvent ev(RawDataEvent::BORE("QAC", m_run));
			SendEvent(ev);
			eudaq::mSleep(500);
			m_running = true;
			SetStatus(eudaq::Status::LVL_OK, "Started");
		} catch (const std::exception & e) {
			printf("Caught exception: %s\n", e.what());
			SetStatus(eudaq::Status::LVL_ERROR, "Start Error");
		} catch (...) {
			printf("Unknown exception\n");
			SetStatus(eudaq::Status::LVL_ERROR, "Start Error");
		}
	}
	virtual void OnStopRun() {
		try {
			std::cout << "Stop Run" << std::endl;
			m_running = false;
			SetStatus(eudaq::Status::LVL_OK, "Stopped");
		} catch (const std::exception & e) {
			printf("Caught exception: %s\n", e.what());
			SetStatus(eudaq::Status::LVL_ERROR, "Stop Error");
		} catch (...) {
			printf("Unknown exception\n");
			SetStatus(eudaq::Status::LVL_ERROR, "Stop Error");
		}
	}
	virtual void OnTerminate() {
		std::cout << "Terminate (press enter)" << std::endl;
		done = true;
		eudaq::mSleep(1000);
	}
private:
	unsigned m_run, m_ev;
	bool done, m_running, m_stoppedrun;
	struct timeval tv;
	unsigned int sec;
	unsigned int microsec;
	time_t timer;
	struct tm *date;

};
// ----------------------------------------------------------------------
int main(int /*argc*/, const char ** argv)
{
    std::cout << "Start Producer \n" << std::endl;

    eudaq::OptionParser op(
    			"EUDAQ QAC Producer",
    			"0.1",
    			"The Producer task for the Charge to Amplitude Converter"
    			);
	eudaq::Option < std::string > rctrl(
			op,
			"r",
			"runcontrol",
			"tcp://localhost:44000",
			"address",
			"The address of the RunControl application"
			);
	eudaq::Option < std::string > level(
			op,
			"l",
			"log-level",
			"NONE",
			"level",
			"The minimum level for displaying log messages locally"
			);
	try {
		op.Parse(argv);
		EUDAQ_LOG_LEVEL(level.Value());
		QACProducer producer(rctrl.Value());
		producer.MainLoop();
		eudaq::mSleep(500);
	} catch (...) {
		return op.HandleMainException();
	}
	return 0;
}
