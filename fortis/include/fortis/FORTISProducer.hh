#include "eudaq/Producer.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"
#include "eudaq/OptionParser.hh"

#include <iostream>
#include <ostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <cassert>

// Declare their use so we don't have to type namespace each time ....
using eudaq::RawDataEvent;
using eudaq::to_string;
using std::vector;

using namespace std;

// put this next definition somewhere more sensible....
#define FORTIS_DATATYPE_NAME "FORTIS"
#define WORDS_IN_ROW_HEADER 2

class FORTISProducer : public eudaq::Producer {

public:
  
  FORTISProducer(const std::string & name,   // Hard code to FORTIS (used to select which portion of cfg file to use)
		 const std::string & runcontrol // Points to run control process
		 )
    : eudaq::Producer(name, runcontrol),
      done(false),
      started(false),
      juststopped(false),
      m_rawData()
  {} // empty constructor


  // Poll for new data
  // N.B. This will have to be changed when we don't veto triggers during FORTIS readout and we end up with several
  // telescope events

  void Process() {

    if (!started) { // If the run isn't started just sleep for a while and return
      eudaq::mSleep(1);
      return;
    }

    // counted_ptr<eudaq::RawDataEvent> ev; // declare a pointer to a RawDataEvent object.

    if (juststopped) started = false; // this risks loosing the last event or two in the buffers. cf. EUDRBProducer for ideas how to fix this.

    // wait for data here....
    try {

      //     FORTISData.read( data[buffer_number] ,  (sizeof short)*m_num_pixels_per_frame );
      m_rawData_pointer = (char *)&m_rawData[0];

      FORTISData.read( m_rawData_pointer , 2*m_num_pixels_per_frame );


    }  catch (const std::exception & e) {
      printf("Caught exception: %s\n", e.what());
      SetStatus(eudaq::Status::LVL_ERROR, "Configuration Error");
    }

    // OK, we have a frame from the FORTIS. Scan through looking for triggers....

    //FORTISPackData ( m_rawData , number_of_triggers_this_frame , ..... ); // this is pseudo code....


    // loop round sending empty events to data collector for each telecope trigger that we haven't read out...

    // end of loop

    RawDataEvent ev(FORTIS_DATATYPE_NAME, m_run, m_ev); // create an instance of the RawDataEvent with FORTIS-ID

    int evtModID = 0; // FORTIS will have only one module....
    ev.AddBlock(evtModID , m_rawData); // add the raw data block to the event

    SendEvent( ev ); // send the data to the data-collector
    

    if ( m_ev < 10 ) { // print out a debug message for each of first ten events, then every 10th 
      std::cout << "Sent event " << m_ev << std::endl;
    } else if ( m_ev % 10 == 0 ) {
      std::cout << "Sent event " << m_ev << std::endl;
    }

    ++m_ev; // increment the internal event counter.
  }



  virtual void OnConfigure(const eudaq::Configuration & param) {
    SetStatus(eudaq::Status::LVL_OK, "Wait");
    
    m_param = param;


    try {
      std::cout << "Configuring (" << param.Name() << ")..." << std::endl;

      // put our configuration stuff in here...
      m_num_pixels_per_frame = ( m_param.Get("NumRows", 512) + WORDS_IN_ROW_HEADER) *  m_param.Get("NumColumns", 512) ;
      std::cout << "Number of rows: " <<   m_param.Get("NumRows", 512) << std::endl;
      std::cout << "Number of columns: " <<   m_param.Get("NumColumns", 512) << std::endl;
      std::cout <<"Number of pixels in each frame (including row-headers =" << m_num_pixels_per_frame << std::endl;
      
      // Open input file ( actually a named pipe... )
      std::cout << "Opening named pipe for raw data input. Filename = " << m_param.Get("NamedPipe","./fortis_named_pipe") << std::endl;
      FORTISData.open( "./fortis_named_pipe" , ios::in | ios::binary );
      if ( ! FORTISData.is_open() ) { throw "Unable to open named pipe"; }

      // FORTISData.open( m_param.Get("NamedPipe","./fortis_named_pipe") , ios::in | ios::binary );

      m_rawData.resize(2*m_num_pixels_per_frame); // set the size of our event.

      std::cout << "...Configured (" << param.Name() << ")" << std::endl;
      EUDAQ_INFO("Configured (" + param.Name() + ")");
      SetStatus(eudaq::Status::LVL_OK, "Configured (" + param.Name() + ")");
    } catch (const std::exception & e) {
      printf("Caught exception: %s\n", e.what());
      SetStatus(eudaq::Status::LVL_ERROR, "Configuration Error");
    } catch (char * str) {
      printf("Exception: %s\n" , str);
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
      
      RawDataEvent ev( RawDataEvent::BORE( FORTIS_DATATYPE_NAME , m_run ) );
      std::string paramname = "InitialRow";
      std::cout << "pname " << paramname << std::endl
		<< "m_param " << m_param << std::endl;
      int InitialRow = m_param.Get(paramname, 0x0) ;
      ev.SetTag("InitialRow", to_string( InitialRow )) ; // put run parameters into BORE
      ev.SetTag("InitialColumn", to_string( m_param.Get("InitialColumn", 0x0) )) ; // put run parameters into BORE  
      ev.SetTag("NumRows", to_string( m_param.Get("NumRows", 512) )) ; // put run parameters into BORE
      ev.SetTag("NumColumns", to_string( m_param.Get("NumRows", 512) )) ; // put run parameters into BORE

      ev.SetTag("Vectors", m_param.Get("Vectors", "./MyVectors.bin" ) ) ; // put run parameters into BORE

      SendEvent( ev );

      eudaq::mSleep(100);
      started=true;
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
      std::cout << "Stopping Run" << std::endl;
      juststopped = true;
      while (started) {
        eudaq::mSleep(100);
      }
      juststopped = false;
      SendEvent(RawDataEvent::EORE( FORTIS_DATATYPE_NAME, m_run, ++m_ev));
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
    std::cout << "Terminating..." << std::endl;
  
    FORTISData.close();

    // Kill the thread with the command-line-programme here ....

    done = true;
  }


      // Declare members of class FORTISProducer.
  unsigned m_run, m_ev;
  bool done, started, juststopped;
  eudaq::Configuration  m_param;

  //  std::vector<unsigned short> m_rawData; // buffer for raw data frames. 
  //  std::vector<char> m_rawData; // buffer for raw data frames. 
  std::vector<unsigned short> m_rawData; // buffer for raw data frames. 
  char * m_rawData_pointer;

  int m_num_pixels_per_frame ;

private:
  ifstream FORTISData;

};
