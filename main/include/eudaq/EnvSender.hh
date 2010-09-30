#ifndef EUDAQ_INCLUDED_EnvSender
#define EUDAQ_INCLUDED_EnvSender

#include "eudaq/TransportClient.hh"
#include "eudaq/Serializer.hh"
#include "eudaq/Status.hh"
#include <string>

namespace eudaq {

  class EnvMessage;

	
  class EnvSender {
  public:
    EnvSender();
    ~EnvSender();
    void Connect(const std::string & type, const std::string & name, const std::string & server);
    void SendEnvMessage(const EnvMessage &, bool show = true);
    void SendEnv(const std::string type, const std::string dType, const std::string name, const std::string value);
    //void SetLevel(int level) { m_level = level; }
    //void SetLevel(const std::string & level) { SetLevel(Status::String2Level(level)); }
  private:
    std::string m_name;
    TransportClient * m_envclient;
    //int m_level;
    bool m_shownotconnected;
  };

}

#endif // EUDAQ_INCLUDED_EnvSender
