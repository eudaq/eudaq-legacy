#include "eudaq/Producer.hh"
#include "eudaq/altro/AltroProducer.hh"
#include "eudaq/Logger.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/OptionParser.hh"
#include <iostream>
#include <ostream>
#include <cctype>



AltroProducer::AltroProducer(const std::string & name,
					   const std::string & runcontrol)
    : eudaq::Producer(name, runcontrol), m_done(false), m_run(0) , m_ev(0)
{
//    // First initialise the mutex attributes
//    pthread_mutexattr_init(&m_mutexattr);

    // Inititalise the mutexes
    pthread_mutex_init( &m_commandqueue_mutex, 0 );
    pthread_mutex_init( &m_runactive_mutex, 0 );
    pthread_mutex_init( &m_run_mutex, 0 );
    pthread_mutex_init( &m_ev_mutex, 0 );
}

AltroProducer::~AltroProducer()
{
    pthread_mutex_destroy( &m_commandqueue_mutex );
    pthread_mutex_destroy( &m_runactive_mutex );
    pthread_mutex_destroy( &m_run_mutex );
    pthread_mutex_destroy( &m_ev_mutex );
}

int 

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

unsigned int AltroProducer::GetIncreaseEventNumber()
{
    unsigned int retval;
    pthread_mutex_lock( &m_ev_mutex );
      retval = m_ev++;
    pthread_mutex_unlock( &m_ev_mutex );
    return retval;
}

void AltroProducer::SetDone(bool done)
{
    pthread_mutex_lock( &m_done_mutex );
       m_done = done;
    pthread_mutex_unlock( &m_done_mutex );    
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

void AltroProducer::Event(unsigned short *timepixdata)
{
    eudaq::RawDataEvent ev("TimepixEvent",GetRunNumber(), GetIncreaseEventNumber() );

    // a 128 kB data block, in this the data is stored in little endian
    unsigned char serialdatablock[131072];
    
    for (unsigned i=0; i < 65536 ; i ++)
    {
	// send little endian, i. e. the most significant first
	serialdatablock[2*i] = (timepixdata[i] & 0xFF00) >> 8 ;
	serialdatablock[2*i + 1] = timepixdata[i] & 0xFF ;
    }

    ev.AddBlock(serialdatablock , 131072);

    SendEvent(ev);
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
    }
    else
    {
	//call the altro configuration
	// note that this is happening in the communication thread
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
    CommandPush( Commands::START_RUN );    
}

void AltroProducer::OnStopRun()
{
//    SendEvent(eudaq::RawDataEvent::EORE("TimepixEvent", GetRunNumber(), GetEventNumber()));

    // Tell the main loop to stop the run
    CommandPush( Commands::STOP_RUN );
}
 
void AltroProducer::OnTerminate()
{
    // Tell the main loop to terminate
    CommandPush( Commands::TERMINATE );
}
 
void AltroProducer::OnReset()
{
    std::cout << "Reset" << std::endl;
    // Tell the main loop to terminate
    CommandPush( Commands::RESET );
    SetStatus(eudaq::Status::LVL_OK);
}

void AltroProducer::OnStatus()
{
    // Tell the main loop send the status
    CommandPush( Commands::STATUS );
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
	   retval = Commands::NONE;
       }
       else
       {
	   retval = m_commandQueue.first();
	   m_commandQueue.pop();
       }
    pthread_mutex_unlock( &m_commandqueue_mutex );

    return retval();
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
