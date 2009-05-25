#include "stdafx.h"
#include "eudaq/Producer.hh"
#include "TimepixProducer.h"
#include "eudaq/Logger.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/OptionParser.hh"
#include <iostream>
#include <ostream>
#include <cctype> 

#ifndef WINVER
#define WINVER = 0x0501
#endif


TimepixProducer::TimepixProducer(const std::string & name,
					   const std::string & runcontrol, CPixelmanProducerMFCDlg* pixelmanCtrl)
    : eudaq::Producer(name, runcontrol), m_done(false), m_run(0) , m_ev(0)
{
    // First initialise the mutex attributes
    pthread_mutexattr_init(&m_mutexattr);

    // Inititalise the mutexes
    pthread_mutex_init( &m_done_mutex, 0 );
    pthread_mutex_init( &m_run_mutex, 0 );
    pthread_mutex_init( &m_ev_mutex, 0 );
	pthread_mutex_init( &m_stopRun_mutex, 0 );
	pthread_mutex_init( &m_runFinished_mutex, 0);

	this->pixelmanCtrl = pixelmanCtrl;
}

TimepixProducer::~TimepixProducer()
{
    pthread_mutex_destroy( &m_done_mutex );
    pthread_mutex_destroy( &m_run_mutex );
    pthread_mutex_destroy( &m_ev_mutex );
	pthread_mutex_destroy( &m_stopRun_mutex );
	pthread_mutex_destroy( &m_runFinished_mutex );
}

bool TimepixProducer::GetDone()
{
    bool retval;
    pthread_mutex_lock( &m_done_mutex );
      retval = m_done;
    pthread_mutex_unlock( &m_done_mutex );    
    return retval;
}

unsigned int TimepixProducer::GetRunNumber()
{
    unsigned int retval;
    pthread_mutex_lock( &m_run_mutex );
      retval = m_run;
    pthread_mutex_unlock( &m_run_mutex );    
    return retval;
}

unsigned int TimepixProducer::GetEventNumber()
{
    unsigned int retval;
    pthread_mutex_lock( &m_ev_mutex );
      retval = m_ev;
    pthread_mutex_unlock( &m_ev_mutex );    
    return retval;
}

unsigned int TimepixProducer::GetIncreaseEventNumber()
{
    unsigned int retval;
    pthread_mutex_lock( &m_ev_mutex );
      retval = m_ev++;
    pthread_mutex_unlock( &m_ev_mutex );
    return retval;
}

void TimepixProducer::SetDone(bool done)
{
    pthread_mutex_lock( &m_done_mutex );
       m_done = done;
    pthread_mutex_unlock( &m_done_mutex );    
}

void TimepixProducer::SetEventNumber(unsigned int eventnumber)
{
    pthread_mutex_lock( &m_ev_mutex );
       m_ev = eventnumber;
    pthread_mutex_unlock( &m_ev_mutex );    
}

void TimepixProducer::SetRunNumber(unsigned int runnumber)
{
    pthread_mutex_lock( &m_run_mutex );
       m_run = runnumber;
    pthread_mutex_unlock( &m_run_mutex );    
}

void TimepixProducer::Event(i16 *timepixdata, u32 size)
{
    static unsigned char *serialdatablock;

	eudaq::RawDataEvent ev("TimepixEvent",GetRunNumber(), GetIncreaseEventNumber() );
	
    serialdatablock = new unsigned char[size];
        
    for (unsigned i=0; i < size ; i ++)
    {
		serialdatablock[2*i] = (timepixdata[i] & 0xFF00) >> 8 ;
		serialdatablock[2*i + 1] = timepixdata[i] & 0xFF ;
	}

    ev.AddBlock(serialdatablock , size*sizeof(char));

    SendEvent(ev);
}

void TimepixProducer::SimpleEvent()
{
    // a 64 kWord data block in the computer whatever endian format
    unsigned short rawdatablock[65536];

    for (unsigned i=0; i < 65536 ; i ++)
    {
	rawdatablock[i] = i;
    }

    //ev.SetTag("Debug", "foo");
  //  Event(rawdatablock);
}

void TimepixProducer::BlobEvent()
{
//    TestEvent ev(m_run, ++m_ev, str);
//    //ev.SetTag("Debug", "foo");
//    SendEvent(ev);
}


void TimepixProducer::OnConfigure(const eudaq::Configuration & param) 
{
	DEVID devId = pixelmanCtrl->mpxDevId[pixelmanCtrl->mpxCurrSel].deviceId;
	pixelmanCtrl->mpxCtrlInitMpxDevice(devId);
    EUDAQ_INFO("Configured (" + param.Name() + ")");
    SetStatus(eudaq::Status::LVL_OK, "Configured (" + param.Name() + ")");

}

void TimepixProducer::OnStartRun(unsigned param) 
{	
	pixelmanCtrl->m_commHistRunCtrl.AddString(_T("Start Of Run."));
	pthread_mutex_lock( &m_stopRun_mutex );
       m_stopRun = false;
	   static bool stopRun = m_stopRun;
    pthread_mutex_unlock( &m_stopRun_mutex );
	pthread_mutex_lock( &m_runFinished_mutex);
		m_runFinished = false;
	pthread_mutex_unlock( &m_runFinished_mutex);
    
	SetRunNumber( param );
    SetEventNumber( 1 ); // has to be 1 because BORE is event 0 :-(
    SendEvent(eudaq::RawDataEvent::BORE(_T("TimepixEvent"), param )); // send param instead of GetRunNumber
    //std::cout << "Start Run: " << param << std::endl;
	MessageBox(NULL, "Start of Run", "EudaqMessage", NULL);
	while(stopRun==false)
	{	
		pixelmanCtrl->mpxCtrlPerformTriggeredFrameAcqTimePixProd();
		Event(pixelmanCtrl->mpxDevId[pixelmanCtrl->mpxCurrSel].databuffer,
				pixelmanCtrl->mpxDevId[pixelmanCtrl->mpxCurrSel].sizeOfDataBuffer);	

		pthread_mutex_lock( &m_stopRun_mutex );
			stopRun = m_stopRun;
		pthread_mutex_unlock( &m_stopRun_mutex );
	}
	pthread_mutex_lock( &m_runFinished_mutex);
		m_runFinished = true;
	pthread_mutex_unlock( &m_runFinished_mutex);
	
}

void TimepixProducer::OnStopRun()
{
	pixelmanCtrl->m_commHistRunCtrl.AddString(_T("End Of Run."));
	pthread_mutex_lock( &m_stopRun_mutex );
       m_stopRun = false;
	   static bool stopRun = m_stopRun;
	pthread_mutex_unlock( &m_stopRun_mutex );
	pthread_mutex_lock( &m_runFinished_mutex );
		static bool runFinished = m_runFinished;
	pthread_mutex_unlock( &m_runFinished_mutex );
	
	while (runFinished==false)
	{
		pthread_mutex_lock( &m_runFinished_mutex);
				runFinished = m_runFinished;
		pthread_mutex_unlock( &m_runFinished_mutex);
	}

    SendEvent(eudaq::RawDataEvent::EORE(_T("TimepixEvent"),GetRunNumber(), GetEventNumber()));
    //std::cout << "Stop Run" << std::endl;
	MessageBox(NULL, "End Of Run", "Message from Runcontrol",NULL);
}
 
void TimepixProducer::OnTerminate()
{	
	DEVID devId = pixelmanCtrl->mpxDevId[pixelmanCtrl->mpxCurrSel].deviceId;
	pixelmanCtrl->mpxCtrlAbortOperation(devId);
	pixelmanCtrl->m_commHistRunCtrl.AddString(_T("Terminated (I'll be back)"));
	//std::cout << "Terminate (press enter)" << std::endl;
    SetDone( true );
	
}
 
void TimepixProducer::OnReset()
{
	pixelmanCtrl->m_commHistRunCtrl.AddString(_T("Reset."));
	//std::cout << "Reset" << std::endl;
    //SetStatus(eudaq::Status::LVL_OK);
}

void TimepixProducer::OnStatus()
{
	pixelmanCtrl->m_commHistRunCtrl.AddString(_T("OnStatus Not Implemented."));
	//std::cout << "Status - " << m_status << std::endl;
    //SetStatus(eudaq::Status::WARNING, "Only joking");
}

void TimepixProducer::OnUnrecognised(const std::string & cmd, const std::string & param) 
{
	pixelmanCtrl->m_commHistRunCtrl.AddString(_T("Unrecognised Command."));
    std::cout << "Unrecognised: (" << cmd.length() << ") " << cmd;
    if (param.length() > 0) std::cout << " (" << param << ")";
    std::cout << std::endl;
    SetStatus(eudaq::Status::LVL_WARN, "Just testing");
}