#include "eudaq/Producer.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Utils.hh"
#include "eudaq/Logger.hh"
#include "eudaq/OptionParser.hh"
#include "fortis/FORTIS.hh"

#include <iostream>
#include <ostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <cassert>
#include <pthread.h>

// Declare their use so we don't have to type namespace each time ....
using eudaq::RawDataEvent;
using eudaq::to_string;
using std::vector;

using namespace std;




class FORTISProducer : public eudaq::Producer {

public:
  
  FORTISProducer(const std::string & name,   // Hard code to FORTIS (used to select which portion of cfg file to use)
		 const std::string & runcontrol // Points to run control process
		 )
    : eudaq::Producer(name, runcontrol),
      done(false),
      started(false),
      juststopped(false),
      configured(false),
      m_rawData(),
      m_frameBuffer(2),
      m_triggers_pending(false),
      m_buffer_number(0),
      m_executableThreadId()
  {} // empty constructor


  void Process() {

    // we always want to be sensitive to data from the FORTIS.... which can arrive any time after configuration....
    if (!configured) { // If we aren't configured just sleep for a while and return
      eudaq::mSleep(1);
      return;
    }

    if (juststopped) started = false; // this risks loosing the last event or two in the buffers. cf. EUDRBProducer for ideas how to fix this.

    // wait for data here....
    try {

      m_buffer_number = ( ++m_buffer_number ) % 2; // alternate between buffers...

      m_rawData_pointer = reinterpret_cast<char *>(&m_frameBuffer[m_buffer_number][0]); // read into buffer number m_buffer_number
      if (  m_FORTIS_Data.good() ) {
	m_FORTIS_Data.read( m_rawData_pointer , sizeof(short)*m_num_pixels_per_frame );
      } else {
	EUDAQ_THROW("Problem reading FORTIS data from input pipe");
      }

      std::cout << "Read block of data ..."<< std::endl;

    }  catch (const std::exception & e) {
      printf("Caught exception: %s\n", e.what());
      SetStatus(eudaq::Status::LVL_ERROR, "Read Error");
    }

    if (started) { // OK, the run has started. We want to do something with the frames we are reading....

      std::cout << "Processing data"<<std::endl;

      ProcessFrame();

    }
  }


  // Called from Process, in order to 
  void ProcessFrame() {

    const int evtModID = 0; // FORTIS will have only one module....

    unsigned int words_per_row =  m_NumColumns +  WORDS_IN_ROW_HEADER;
    unsigned int row_counter ;
    unsigned int word_counter ;

    // If DEBUG flag is set then print out the frame...
    // #define DEBUG 1
#if DEBUG

    for (row_counter=0; row_counter < m_NumRows ; row_counter++) {

      std::cout << "Row = " << row_counter << std::endl ;

      
      for (word_counter =0; word_counter < words_per_row ; word_counter++) {

	std::cout << hex << m_frameBuffer[m_buffer_number][word_counter + (words_per_row)*row_counter ] << "\t" ;
      }

    std::cout << dec << std::endl;      
    }

#endif

    m_currentFrame = m_frameBuffer[m_buffer_number][0];

    if (  m_triggers_pending > 0 ) { // We have triggers pending from a previous frame. 
                                     // Append this frame to the previous one and send out event....

      // look the other way while I do something really inefficient....
      int frame;

      for ( frame = 0; frame<2 ; frame++ ) {

	int buffer_number = (m_buffer_number + frame + 1 )%2; // point to the previous frame first.
	for (word_counter = 0 ; word_counter < m_num_pixels_per_frame ; word_counter++) {

	  m_rawData[frame*m_num_pixels_per_frame + word_counter] =  m_frameBuffer[buffer_number][word_counter] ;
	}
      }
    
      std::cout << "Sending Event number " << m_ev << " , frame number " << m_currentFrame << std::endl;
 
       RawDataEvent ev(FORTIS_DATATYPE_NAME, m_run, m_ev); // create an instance of the RawDataEvent with FORTIS-ID
       ev.AddBlock(evtModID , m_rawData); // add the raw data block to the event
       SendEvent( ev ); // send the 2-frame data to the data-collector

       ++m_ev; // increment the internal event counter.
       --m_triggers_pending; // decrement the number of triggers to send out...

       while ( m_triggers_pending > 0 ) {

	 std::cout << "Sending EMPTY Event number " << m_ev << std::endl;

	 RawDataEvent ev(FORTIS_DATATYPE_NAME, m_run, m_ev);
	 SendEvent( ev );
	 ++m_ev; // increment the internal event counter.
	 --m_triggers_pending; // decrement the number of triggers to send out...
       }

    }

    m_triggers_pending = 0; // this shouldn't be necessary....

    // Loop through looking for triggers ...
    for ( row_counter=0; row_counter < m_NumRows ; row_counter++) {
      m_triggers_pending = m_triggers_pending + m_frameBuffer[m_buffer_number ][1 + row_counter*words_per_row ];
    }

    std::cout << "Found " << m_triggers_pending << " triggers in frame " << m_currentFrame << std::endl;

    if ( m_ev < 10 ) { // print out a debug message for each of first ten events, then every 10th 
      std::cout << "Processed frame number = " << m_currentFrame << std::endl;
    } else if ( m_ev % 10 == 0 ) {
      std::cout << "Processed frame number = " << m_currentFrame << std::endl;
    }



  }

  virtual void OnConfigure(const eudaq::Configuration & param) {
    SetStatus(eudaq::Status::LVL_OK, "Wait");
    
    m_param = param;


    try {
      std::cout << "Configuring (" << param.Name() << ")..." << std::endl;

      // put our configuration stuff in here...
      m_NumRows = m_param.Get("NumRows", 512) ;
      m_NumColumns = m_param.Get("NumColumns", 512) ;
      m_num_pixels_per_frame = ( m_NumColumns + WORDS_IN_ROW_HEADER) *   m_NumRows ;

      std::cout << "Number of rows: " <<  m_NumRows << std::endl;
      std::cout << "Number of columns: " <<  m_NumColumns  << std::endl;
      std::cout << "Number of pixels in each frame (including row-headers) = " << m_num_pixels_per_frame << std::endl;

      std::cout << "Starting Command line programme to stream FORTIS data" << std::endl;
      startExecutable();

      usleep(1000000);

      // Open input file ( actually a named pipe... )
      std::string filename = m_param.Get("NamedPipe","./fortis_named_pipe") ;
      std::cout << "Opening named pipe for raw data input. Filename = " << filename << std::endl;

      m_FORTIS_Data.open( filename.c_str() , ios::in | ios::binary );
      if ( ! m_FORTIS_Data.is_open() ) { EUDAQ_THROW("Unable to open named pipe"); }

      std::cout << "Opened name pipe " << std::endl;

      m_frameBuffer[0].resize( sizeof(short) * m_num_pixels_per_frame); // set the size of our frame buffer.
      m_frameBuffer[1].resize( sizeof(short) * m_num_pixels_per_frame); 
      m_rawData.resize( 2* sizeof(short) * m_num_pixels_per_frame); // set the size of our event(big enough for two frames)...

      configured = true;

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

    if (started) {
      std::cout << "Already started. Refusing to do anything." << std::endl;
      return;
    } 

    try {
      m_run = param;
      m_ev = 0;

      // At the start of run point to buffer_number=0 and declare that we don't have any triggers in the  
      // previous frame.
      m_triggers_pending = 0;
      m_buffer_number = 0;
 
      std::cout << "Start Run: " << param << std::endl;
      
      RawDataEvent ev( RawDataEvent::BORE( FORTIS_DATATYPE_NAME , m_run ) );

      ev.SetTag("InitialRow", to_string( m_param.Get("InitialRow", 0x0)  )) ; // put run parameters into BORE
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
  
    m_FORTIS_Data.close();

    // Kill the thread with the command-line-programme here ....
    std::string killcommand =  "killall " + m_exeArgs.filename; 
    system(  killcommand.c_str() );
    done = true;
  }


      // Declare members of class FORTISProducer.
  unsigned m_run, m_ev;
  bool done, started, juststopped , configured;
  eudaq::Configuration  m_param;

  //  std::vector<unsigned short> m_rawData; // buffer for raw data frames. 
  //  std::vector<char> m_rawData; // buffer for raw data frames. 
  std::vector<unsigned short> m_rawData; // buffer for raw data frames. 


private:

  // PRIVATE TYPEDEFS
  typedef std::vector<unsigned short> FrameBuffer;
  typedef std::vector<FrameBuffer> DoubleFrame;

  
  // PRIVATE MEMBER VARIABLES
  ifstream m_FORTIS_Data;  ///< Named pipe for receiving FORTIS data
  DoubleFrame m_frameBuffer; // buffer for two frames

  unsigned int m_currentFrame;

  unsigned int m_triggers_pending;
  unsigned int m_buffer_number  ;

  char * m_rawData_pointer;
  unsigned int m_NumRows ;
  unsigned int m_NumColumns;
  unsigned int m_num_pixels_per_frame ;

  pthread_t m_executableThreadId;
  
  ExecutableArgs m_exeArgs;
  
  // PRIVATE METHODS
  
  /// Entry point for thread that starts the command-line-program that streams data from FORTIS
  void startExecutable() {
    // ExecutableArgs exeArgs;
    m_exeArgs.dir = m_param.Get("ExecutableDirectory","./");
    m_exeArgs.filename  = m_param.Get("ExecutableFilename","stream_exe") ;
    m_exeArgs.args = m_param.Get("ExecutableArgs","");

    unsigned threadCreateResult = pthread_create(&m_executableThreadId, NULL, &startExecutableThread, (void*)&m_exeArgs);
  
    // If the thread wasn't made successfully, bail out
    if(threadCreateResult) { EUDAQ_THROW("Error creating thread (result=" + to_string(threadCreateResult) + ")!" ); }    
  }
  
};

