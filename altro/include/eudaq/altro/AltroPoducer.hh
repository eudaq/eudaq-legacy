#include "eudaq/Producer.hh"
#include <pthread.h>

class AltroProducer : public eudaq::Producer
{
  public:
    AltroProducer(const std::string & name, const std::string & runcontrol);
    virtual ~AltroProducer();

    /** The main loop of the producer
     */
    int Exec();

protected:

    /** Send an recorded event from the physmem(??)
     */
    void Event(unsigned short *timepixdata);

    /** Threadsave version to get (a copy of) the m_done variable
     */
    bool GetDone();

    /** Threadsave version to set the m_run variable
     */
    void SetRunNumber(unsigned int runnumber);

    /** Threadsave version to get (a copy of) the m_run variable
     */
    unsigned int GetRunNumber();

    /** Threadsave version to set the m_event variable
     */
    void SetEventNumber(unsigned int eventnumber);

    /** Threadsave version to get (a copy of) the m_event variable
     */
    unsigned int GetEventNumber();

    /** Threadsave version to get (a copy of) the m_event variable.
     *  After creating the copy m_event is increased before releasing the 
     *  mutex. The non-increased version is returned (like in the post-increment operator).
     */
    unsigned int GetIncreaseEventNumber();

    /** The protected member functions live in the communication thread.
     *  Make sure they do not perfom computing intensive tasks and return as
     *  soon as possible.
     */
    virtual void OnConfigure(const eudaq::Configuration & param);
    virtual void OnStartRun(unsigned param);
    virtual void OnStopRun();
    virtual void OnTerminate();
    virtual void OnReset();
    virtual void OnStatus();
    virtual void OnUnrecognised(const std::string & cmd, const std::string & param);

    /** The available commands that the command receiver can pass to the main loop
     *  The configure is not a valid command for the loop since it is executed in
     *  the communication thread.
     */
    enum Commands( NONE = 0; START_RUN = 1 , STOP_RUN =2 , STATUS =3 , TERMINATE =4 , RESET=5 );

    /** As the command recaiver runs in a separate thread we need a queue to ensure all 
     *  commands are executed. Do not access directly, only use the thread safe CommandPush() and
     *  CommandPop()!
     */
    std::queue<Commands> m_commandQueue;

    /// A mutex to protext the CommandQueue
    pthread_mutex_t m_commandqueue_mutex;

    /// The thread safe way to push a command to the command queue
    void CommandPush(Commands c);

    /** The thread safe way to pop a command fom the command queue. If the queue is emty
     *  it returns Commands::NONE.
     */
    Commands CommandPop();

    /** The thread safe way to get a reference to the first command fom the command queue
     *  without removing it from the queue
     */
//    Commands & CommandFront();
    
    // all data members have to be protected by mutex since they can be accessed by multiple 
    // threads
    /// status wether a data run is active							
    bool m_runactive; pthread_mutex_t m_runactive_mutex;
    /// The run number
    unsigned m_run;  pthread_mutex_t m_run_mutex;
    /// The event number
    unsigned m_ev;   pthread_mutex_t m_ev_mutex;
    
    pthread_mutexattr_t m_mutexattr;
};
