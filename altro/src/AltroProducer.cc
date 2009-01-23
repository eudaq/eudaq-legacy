#include "AltroProducer.hh"
#include "eudaq/Producer.hh"
#include "eudaq/Logger.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/OptionParser.hh"
#include <iostream>
#include <ostream>
#include <sstream>
#include <cctype>

// the ilcaltro stuff
#include "pca16.h"
#include "readout.h"
#include "runhandler.h"


namespace gear{

namespace altroproducer{

Power::Power( int powercommand ) : _powercommand(powercommand) {}

void Power::Execute(AltroProducer *producer, RUNSTATUS *rs)
{
    // if daq is running only return the status
    if (rs->daq) _powercommand = 0;

    FECPOWER *fecpower;

    switch(_powercommand) {
	case 1: 
	    fecpower = ilcFecPowerOn(); 
	    producer->SetStatus(eudaq::Status::LVL_OK, "Power on");
	    break;
	case 2: 
	    fecpower = ilcFecPowerOff(); 
	    producer->SetStatus(eudaq::Status::LVL_OK, "Power off");
	    break;
	case 0:    
	default: fecpower =  ilcFecPowerStatus(); break;
    }
    if (fecpower == NULL) fecpower = ilcFecPowerStatus();

    // send status information to logger
    for (unsigned int i = 0; i <= RCU_MAX_ID; i++) {
	std::stringstream s;
	s << "Power Status S" << i <<" "<< fecpower[i].status
	  << " RCU"<<i<<" "<<fecpower[i].power << " P"<<fecpower[i].poweredon;
	EUDAQ_INFO(s.str());
    }
}    
    
StartDAQ::StartDAQ( int control, int mode, int type )
    : _control(control), _mode(mode), _type(type) {}

void StartDAQ::Execute(AltroProducer *producer, RUNSTATUS *rs)
{
    if (rs->daq == 0) 
    {
	rs->control = _control;
	rs->runmode = _mode;
	r->type     = _type;
	rs->daq = 1;
	rs->action = RUNACTION_START;
	producer->SetStatus(eudaq::Status::LVL_OK, "DAQ on");
    }
    else // daq already was on
	EUDAQ_WARN("Cannot turn on DAQ, is already running");	    
}

void StopDAQ::Execute(AltroProducer *producer, RUNSTATUS *rs)
{
    producer->SetStatus(eudaq::Status::LVL_OK, "DAQ off");
    if (rs->daq == 1)
    {
	if (rs->run == 0)
	{
	    rs->daq = 0;
	    rs->run = 0;
	    rs->action = RUNACTION_STOP;
	    producer->SetStatus(eudaq::Status::LVL_OK, "DAQ off");
	}
	else // run is still active
	{
	    EUDAQ_WARN("Cannot turn off DAQ, run is still active");
	}
    }
    else
    {
	EUDAQ_WARN("Cannot turn off DAQ, is already off");
    }
}

StartRun::StartRun(unsigned int monevents, int logging, int type, 
		       int maxevents, int maxmonevents)
    :  _monevents(monevents), _logging(logging), _type(type)
       _maxevents(maxevents), _maxmonevents(maxmonevents) {}

void StartRun::Execute(AltroProducer *producer, RUNSTATUS *rs)
{
    if (rs->daq == 1)
    {
	if (rs->run == 0)
	{
	    rs->monevents = _monevents;
	    rs->logging   = _logging;
	    rs->type      = _type;
	    /* number of events to read this run - run stopped when reached */
	    if (_maxevents)
	    {
		rs->events = _maxevents;
		rs->evtflag = 1;
	    }
	    /* number of events to monitor this run - run stopped when reached */
	    if (_maxmonevents)
	    {
		rs->mevents = iarg;
		rs->evtflag = 2;
	    }
    
	    rs->run = 1;
	    rs->action = RUNACTION_SOR;
	    producer->SetStatus(eudaq::Status::LVL_OK, "DAQ off");
	}
	else // run is still active
	    EUDAQ_WARN("Cannot start run, run is already active");
    }
    else // daq if off
	EUDAQ_WARN("Cannot start run, DAQ is off");
}

void PauseRun::Execute(AltroProducer *producer, RUNSTATUS *rs)
{
    if((rs->daq == 1) && (rs->run == 1))
    {
      rs->run = 2;
      rs->action = RUNACTION_PAUSE;
      producer->SetStatus(eudaq::Status::LVL_OK, "Run paused");
    }
    else
	EUDAQ_WARN("Cannot pause run, run is not active");	
}

void ContinueRun::Execute(AltroProducer *producer, RUNSTATUS *rs)
{
    if((rs->daq == 1) && (rs->run == 2))
    {
      rs->run = 1;
      rs->action = RUNACTION_CONT;
      producer->SetStatus(eudaq::Status::LVL_OK, "Run continued");
    }
    else
	EUDAQ_WARN("Cannot continue run, run is not paused");	
}

void EndRun::Execute(AltroProducer *producer, RUNSTATUS *rs)
{
    if ((rs->daq == 1) && (rs->run > 0)) 
    {
      rs->run = 0;
      rs->action = RUNACTION_EOR;
      producer->SetStatus(eudaq::Status::LVL_OK, "Run stopped");
    }
    else
	EUDAQ_WARN("Cannot stop run, run is already stopped");    
}

PCA::PCA( int shiftRegister, int dac )
    : _shiftRegister(shiftRegister) , _dac(dac) {}

void void PCA::Execute(AltroProducer *producer, RUNSTATUS *rs)
{
    int retstat = 0;
    /* decode what to do - but if daq is active we can only return the current status */ 
    if (rs->daq == 0) 
    {
	PCA16 pca;
	pca->shiftreg = _shiftRegister;
	pca->dac = _dac;
	// calculate all other pararam of the pca from the given shiftregister and dac
	decodePca16Parameters(&pca);

	/* Load into FEC, make new pca16.cfg file, remember current setting  */
	retstat = ilcPcaLoadSettings(&pca);
      }
    }
    else
	EUDAQ_WARN("Setting PCA requested, but daq is active");    
	
    /* get current values from readout */
    PCA16 *pcasettings = ilcPcaSettingsStatus();

    /* send the PCA status - retstat contains status/error from loading */

    stringtream s;
    s << "PCA Status ShiftRegister " << pca->shiftreg 
      <<" DAC "<< pca->dac,retstat 
      << " Error "<<retstat;
	EUDAQ_INFO(s.str());
    
    if (retstat < 0) // there was an error 
	producer->SetStatus(eudaq::Status::LVL_ERROR, "Error setting PCA");
    else if (retstat > 0) // there was a warning
	producer->SetStatus(eudaq::Status::LVL_ERROR, "Warning setting PCA");
    else // retstat == 0 , everything OK
	producer->SetStatus(eudaq::Status::LVL_OK, "Setting PCA OK");
}

AltroProducer::AltroProducer(const std::string & name,
					   const std::string & runcontrol)
    : eudaq::Producer(name, runcontrol), m_run(0) , m_ev(0)
{
    // Inititalise the mutexes
    pthread_mutex_init( &m_commandqueue_mutex, 0 );
    pthread_mutex_init( &m_runactive_mutex, 0 );
    pthread_mutex_init( &m_run_mutex, 0 );
    pthread_mutex_init( &m_ev_mutex, 0 );
}

AltroProducer::~AltroProducer()
{
    // Destroy all mutexes
    pthread_mutex_destroy( &m_commandqueue_mutex );
    pthread_mutex_destroy( &m_runactive_mutex );
    pthread_mutex_destroy( &m_run_mutex );
    pthread_mutex_destroy( &m_ev_mutex );
}

unsigned int AltroProducer::GetRunNumber()
{
    unsigned int retval;
    pthread_mutex_lock( &m_run_mutex );
      retval = m_run;
    pthread_mutex_unlock( &m_run_mutex );    
    return retval;
}

unsigned int AltroProducer::GetEventNumber()
{
    unsigned int retval;
    pthread_mutex_lock( &m_ev_mutex );
      retval = m_ev;
    pthread_mutex_unlock( &m_ev_mutex );    
    return retval;
}

bool AltroProducer::GetRunActive()
{
    bool retval;
    pthread_mutex_lock( &m_runactive_mutex );
      retval = m_runactive;
    pthread_mutex_unlock( &m_runactive_mutex );    
    return retval;
}

unsigned int AltroProducer::GetIncreaseEventNumber()
{
    unsigned int retval;
    pthread_mutex_lock( &m_ev_mutex );
      retval = m_ev++;
    pthread_mutex_unlock( &m_ev_mutex );
    return retval;
}

void AltroProducer::SetEventNumber(unsigned int eventnumber)
{
    pthread_mutex_lock( &m_ev_mutex );
       m_ev = eventnumber;
    pthread_mutex_unlock( &m_ev_mutex );    
}

void AltroProducer::SetRunNumber(unsigned int runnumber)
{
    pthread_mutex_lock( &m_run_mutex );
       m_run = runnumber;
    pthread_mutex_unlock( &m_run_mutex );    
}

void AltroProducer::SetRunActive(bool activestatus)
{
    pthread_mutex_lock( &m_runactive_mutex );
       m_runactive = activestatus;
    pthread_mutex_unlock( &m_runactive_mutex );    
}

void AltroProducer::Event(volatile unsigned long *altrodata, int length)
{
    eudaq::RawDataEvent ev("TimepixEvent",GetRunNumber(), GetIncreaseEventNumber() );

    // a data block of unsigned char, in this the data is stored in little endian
    unsigned char *serialdatablock =  new unsigned char[length*sizeof(unsigned long)];
    
    for (int i=0; i < length ; i ++)
    {
	for (unsigned int j = 0; j < sizeof(unsigned long); j++)
	{
	    // send little endian, i. e. the most significant first
	    serialdatablock[sizeof(unsigned long)*i+j] 
		= (altrodata[i] & (0xFF << 8*j)) >> (sizeof(unsigned long)-j-1)*8 ;
	}
    }

    ev.AddBlock(serialdatablock , length*sizeof(unsigned long));

    SendEvent(ev);
    delete[] serialdatablock;
}

void AltroProducer::OnConfigure(const eudaq::Configuration & param) 
{
    std::cout << "Configuring." << std::endl;
    SetStatus(eudaq::Status::LVL_OK, "Wait (" + param.Name() + ")");


    // if the run is active throw an exception?
    // give a warning? an error to the run control/ logger
    if( GetRunActive() )
    {
	// do the warning stuff
	SetStatus(eudaq::Status::LVL_WARN, "Cannot reconfigure, run is active!");
	return;
    }

    // power up the hardware and set PCA
    CommandPush( POWER );
    CommandPush( PCA );
    
    EUDAQ_INFO("Configured (" + param.Name() + ")");
    SetStatus(eudaq::Status::LVL_OK, "Configured (" + param.Name() + ")");
}
void AltroProducer::OnPrepareRun(unsigned /*param*/) 
{
    // if the run is active throw an exception?
    // give a warning? an error to the run control/ logger
    if( GetRunActive() )
    {
	// do the warning stuff
	SetStatus(eudaq::Status::LVL_WARN, "Cannot prepare run, run is active!");
	return;
    }
    // send start daq command 
    CommandPush( START_DAQ );    
    
}



void AltroProducer::OnStartRun(unsigned param) 
{
    SetRunNumber( param );
    SetEventNumber( 1 ); // has to be 1 because BORE is event 0 :-(
    SendEvent(eudaq::RawDataEvent::BORE( "AltroEvent", param )); // send param instead of GetRunNumber
    std::cout << "Start Run: " << param << std::endl;

    SetRunActive(true);
    // Tell the main loop to start the run
    CommandPush( START_RUN );    
}

void AltroProducer::OnStopRun()
{
//    SendEvent(eudaq::RawDataEvent::EORE("TimepixEvent", GetRunNumber(), GetEventNumber()));

    // Tell the main loop to stop the run
    CommandPush( STOP_RUN );

    // Wait for DAQ to turn off the runactive flag
    while (GetRunActive())
	eudaq::mSleep(1);

    // turn off the daq? Or do it in the destructor?
    CommandPush( STOP_DAQ );

}
 
void AltroProducer::OnTerminate()
{
    // Tell the main loop to terminate
    CommandPush( TERMINATE );
}
 
void AltroProducer::OnReset()
{
    std::cout << "Reset" << std::endl;
    // Tell the main loop to terminate
    CommandPush( RESET );
    SetStatus(eudaq::Status::LVL_OK);
}

void AltroProducer::OnStatus()
{
    // Tell the main loop send the status
    CommandPush( STATUS );
    //std::cout << "Status - " << m_status << std::endl;
    //SetStatus(eudaq::Status::WARNING, "Only joking");
}

void AltroProducer::OnUnrecognised(const std::string & cmd, const std::string & param) 
{
    std::cout << "Unrecognised: (" << cmd.length() << ") " << cmd;
    if (param.length() > 0) std::cout << " (" << param << ")";
    std::cout << std::endl;
    SetStatus(eudaq::Status::LVL_WARN, "Received unkown command " + cmd);
}

AltroProducer::Commands AltroProducer::CommandPop()
{
    Commands retval;

    pthread_mutex_lock( &m_commandqueue_mutex );
       if (m_commandQueue.empty())
       {
	   retval = NONE;
       }
       else
       {
	   retval = m_commandQueue.front();
	   m_commandQueue.pop();
       }
    pthread_mutex_unlock( &m_commandqueue_mutex );

    return retval;
}

AltroProducer::Commands AltroProducer::CommandFront()
{
    Commands retval;

    pthread_mutex_lock( &m_commandqueue_mutex );
       if (m_commandQueue.empty())
       {
	   retval = NONE;
       }
       else
       {
	   retval = m_commandQueue.front();
       }
    pthread_mutex_unlock( &m_commandqueue_mutex );

    return retval;
}

void  AltroProducer::CommandPush(Commands c)
{
    pthread_mutex_lock( &m_commandqueue_mutex );
       m_commandQueue.push(c);
    pthread_mutex_unlock( &m_commandqueue_mutex );    
}

}// namespace gear

}// namespace altroproducer
