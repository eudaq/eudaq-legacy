#include "eudaq/EnvSender.hh"
#include "eudaq/EnvMessage.hh"
#include "eudaq/TransportFactory.hh"
#include "eudaq/Exception.hh"
#include "eudaq/BufferSerializer.hh"

#include <sstream>

namespace eudaq {

  EnvSender::EnvSender() :
    m_envclient(0), m_shownotconnected(false) {}

  void EnvSender::Connect(const std::string & type, const std::string & name, const std::string & server) {
    //std::cout << "Called connect" << std::endl;
    m_shownotconnected = true;
    delete m_envclient;
    m_name = type + " " + name;
    m_envclient = TransportFactory::CreateClient(server);
    //std::cout << "Created EnvClient at " << server << std::endl;

    std::string packet;
    if (!m_envclient->ReceivePacket(&packet, 1000000)) EUDAQ_THROW("No response from EnvCollector server");
    //std::cout << "Receiving from EnvCollector: " << packet << std::endl;
    size_t i0 = 0, i1 = packet.find(' ');
    if (i1 == std::string::npos) EUDAQ_THROW("Invalid response from EnvCollector server");
    std::string part(packet, i0, i1);
    if (part != "OK") EUDAQ_THROW("Invalid response from EnvCollector server: " + packet);
    i0 = i1+1;
    i1 = packet.find(' ', i0);
    if (i1 == std::string::npos) EUDAQ_THROW("Invalid response from EnvCollector server");
    part = std::string(packet, i0, i1-i0);
    if (part != "EUDAQ") EUDAQ_THROW("Invalid response from EnvCollector server, part=" + part);
    i0 = i1+1;
    i1 = packet.find(' ', i0);
    if (i1 == std::string::npos) EUDAQ_THROW("Invalid response from EnvCollector server");
    part = std::string(packet, i0, i1-i0);
    if (part != "ENV") EUDAQ_THROW("Invalid response from EnvCollector server, part=" + part);
    i0 = i1+1;
    i1 = packet.find(' ', i0);
    part = std::string(packet, i0, i1-i0);
    if (part != "EnvCollector" ) EUDAQ_THROW("Invalid response from EnvCollector server, part=" + part);

    m_envclient->SendPacket("OK EUDAQ ENV " + m_name);
    packet = "";
    if (!m_envclient->ReceivePacket(&packet, 1000000)) EUDAQ_THROW("No response from EnvCollector server");
    i1 = packet.find(' ');
    if (std::string(packet, 0, i1) != "OK") EUDAQ_THROW("Connection refused by EnvCollector server: " + packet);
  }

  void EnvSender::SendEnvMessage(const EnvMessage & msg, bool show) {
    //std::cout << "Sending EnvMessage: " << msg << std::endl;
    if (!m_envclient) {
      if (m_shownotconnected) std::cerr << "### Logger not connected ###\n";
    } else {
      BufferSerializer ser;
      msg.Serialize(ser);
      //std::cout << "Calling SendPacket" << std::endl;
      m_envclient->SendPacket(ser);
    }
      if (m_name != "") std::cerr << "[" << m_name << "] ";
      std::cerr << msg << std::endl;
  }
  
  void SendEnv(const std::string type, const std::string dType, const std::string name, const std::string value) {
	if (name=="" || value=="") { EUDAQ_THROW("Setting of name and value is required"); } else {
		std::ostringstream out;
		out << type << " " << dType << " " << name << " " << value << std::endl;
		//SendEnvMessage(EnvMessage(out, eudaq::EnvMessage::LVL_ENV));
		
	}
  }

  EnvSender::~EnvSender() {
    delete m_envclient;
  }

}
