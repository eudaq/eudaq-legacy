#include "AltroProducer.hh"
#include "eudaq/Producer.hh"
#include "eudaq/Logger.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/OptionParser.hh"
#include <iostream>
#include <ostream>
#include <cctype>



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

void AltroProducer::Event(unsigned long *altrodata, int length)
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
	// do the warning stuff, but how to do it in eudq??
    }
    else
    {
	// send start daq command 
	CommandPush( START_DAQ );    
    }

    EUDAQ_INFO("Configured (" + param.Name() + ")");
    SetStatus(eudaq::Status::LVL_OK, "Configured (" + param.Name() + ")");
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
    // CommandPush( STOP_DAQ );

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


//int main(int /*argc*/, const char ** argv) {
//  eudaq::OptionParser op("EUDAQ Producer", "0.0", "A comand-line version of a timepix dummy Producer");
//  eudaq::Option<std::string> rctrl(op, "r", "runcontrol", "tcp://localhost:44000", "address",
//                                   "The address of the RunControl application");
//  eudaq::Option<std::string> level(op, "l", "log-level", "NONE", "level",
//                                   "The minimum level for displaying log messages locally");
//  eudaq::Option<std::string> name (op, "n", "name", "Altro", "string",
//                                   "The name of this Producer");
//  try {
//    op.Parse(argv);
//    EUDAQ_LOG_LEVEL(level.Value());
//    AltroProducer producer(name.Value(), rctrl.Value());
//    bool help = true;
//    do {
//      if (help) {
//        help = false;
//        std::cout << "--- Commands ---\n"
//                  << "s      Send simple TimepixEvent\n"
//                  << "b      Send blob TimepixEvent\n"
//                  << "l msg  Send log message\n"
//                  << "o msg  Set status=OK\n"
//                  << "w msg  Set status=WARN\n"
//                  << "e msg  Set status=ERROR\n"
//                  << "q      Quit\n"
//                  << "?      \n"
//                  << "----------------" << std::endl;
//      }
//      std::string line;
//      std::getline(std::cin, line);
//      //std::cout << "Line=\'" << line << "\'" << std::endl;
//      char cmd = '\0';
//      if (line.length() > 0) {
//        cmd = std::tolower(line[0]);
//        line = eudaq::trim(std::string(line, 1));
//      } else {
//        line = "";
//      }
//      switch (cmd) {
//      case '\0': // ignore
//        break;
//      case 's':
//        producer.SimpleEvent();
//        break;
//      case 'b':
//        producer.BlobEvent();
//        break;
//      case 'l':
//        EUDAQ_USER(line);
//        break;
//      case 'o':
//        producer.SetStatus(eudaq::Status::LVL_OK, line);
//        break;
//      case 'w':
//        producer.SetStatus(eudaq::Status::LVL_WARN, line);
//        break;
//      case 'e':
//        producer.SetStatus(eudaq::Status::LVL_ERROR, line);
//        break;
//      case 'q':
//	producer.SetDone(true);
//        break;
//      case '?':
//        help = true;
//        break;
//      default:
//        std::cout << "Unrecognised command, type ? for help" << std::endl;
//     }
//    } while (!producer.GetDone());
//    std::cout << "Quitting" << std::endl;
//  } catch (...) {
//    return op.HandleMainException();
//  }
//  return 0;
//}
