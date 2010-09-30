#ifndef EUDAQ_INCLUDED_EnvCollector
#define EUDAQ_INCLUDED_EnvCollector

#include <pthread.h>
#include <string>
#include <fstream>

#include "eudaq/TransportServer.hh"
#include "eudaq/CommandReceiver.hh"

namespace eudaq {

  class EnvMessage;

  /** Implements the functionality of the File Writer application.
   *
   */
  class EnvCollector : public CommandReceiver {
  public:
    EnvCollector(const std::string & runcontrol,
                  const std::string & listenaddress);

    virtual void OnConnect(const ConnectionInfo & /*id*/) {}
    virtual void OnDisconnect(const ConnectionInfo & /*id*/) {}
    virtual void OnServer();
    virtual void OnReceive(const EnvMessage & msg) = 0;
    virtual void OnEnv(const std::string & env);
    virtual ~EnvCollector();

    void EnvThread();
  private:
    void EnvHandler(TransportEvent & ev);
    void DoReceive(const EnvMessage & msg);
    bool m_done, m_listening;
    TransportServer * m_envserver; ///< Transport for receiving log messages
    pthread_t m_thread;
    pthread_attr_t m_threadattr;
    std::string m_filename;
    std::ofstream m_file;
  };

}

#endif // EUDAQ_INCLUDED_EnvCollector
