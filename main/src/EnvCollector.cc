#include "eudaq/EnvCollector.hh"
#include "eudaq/EnvMessage.hh"
#include "eudaq/Logger.hh"
#include "eudaq/TransportFactory.hh"
#include "eudaq/BufferSerializer.hh"
#include <iostream>
#include <ostream>
#include <sstream>

namespace eudaq {

  namespace {

    void * EnvCollector_thread(void * arg) {
      EnvCollector * dc = static_cast<EnvCollector *>(arg);
      dc->EnvThread();
      return 0;
    }

  } // anonymous namespace

  EnvCollector::EnvCollector(const std::string & runcontrol,
                               const std::string & listenaddress)
    : CommandReceiver("EnvCollector", "", runcontrol, false),
      m_done(false),
      m_listening(true),
      m_envserver(TransportFactory::CreateServer(listenaddress)),
      m_thread(),
      m_filename("../envs/" + Time::Current().Formatted("%Y-%m-%d.env")),
      m_file(m_filename.c_str(), std::ios_base::app)
  {
    if (!m_file.is_open()) EUDAQ_THROWX(FileWriteException, "Unable to open log file (" + m_filename + ") is there a envs directory?");
    m_envserver->SetCallback(TransportCallback(this, &EnvCollector::EnvHandler));
    pthread_attr_init(&m_threadattr);
    pthread_create(&m_thread, &m_threadattr, EnvCollector_thread, this);
    std::cout << "###### listenaddress=" << m_envserver->ConnectionString() << std::endl
              << "       Environment-logfile=" << m_filename << std::endl;
    std::ostringstream os;
    os << "\n*** EnvCollector started at " << Time::Current().Formatted() << " ***" << std::endl;
    m_file.write(os.str().c_str(), os.str().length());
    m_file.flush();
	  //m_file << os.str() << std::endl;
    CommandReceiver::StartThread();
  }

  EnvCollector::~EnvCollector() {
    m_file << "*** EnvCollector stopped at " << Time::Current().Formatted() << " ***" << std::endl;
    m_done = true;
    /*if (m_thread)*/ pthread_join(m_thread, 0);
    delete m_envserver;
  }

  void EnvCollector::OnServer() {
    if (!m_envserver) EUDAQ_ERROR("Oops");
    //std::cout << "OnServer: " << m_logserver->ConnectionString() << std::endl;
    m_status.SetTag("_SERVER", m_envserver->ConnectionString());
  }

  void EnvCollector::DoReceive(const EnvMessage & ev) {
    std::ostringstream buf;
    ev.Write(buf);
    //m_file.write(buf.str().c_str(), buf.str().length());
   m_file << buf.str() << std::endl; 
	  
   OnReceive(ev);
  }

  void EnvCollector::EnvHandler(TransportEvent & ev) {
    //std::cout << "LogHandler()" << std::endl;
    switch (ev.etype) {
    case (TransportEvent::CONNECT):
      //std::cout << "Connect:    " << ev.id << std::endl;
      if (m_listening) {
        m_envserver->SendPacket("OK EUDAQ ENV EnvCollector", ev.id, true);
      } else {
        m_envserver->SendPacket("ERROR EUDAQ ENV Not accepting new connections", ev.id, true);
        m_envserver->Close(ev.id);
      }
      break;
    case (TransportEvent::DISCONNECT):
      //std::cout << "Disconnect: " << ev.id << std::endl;
      OnDisconnect(ev.id);
      break;
    case (TransportEvent::RECEIVE):
      //std::cout << "EnvCollector Received: " << ev.packet << std::endl;
      if (ev.id.GetState() == 0) { // waiting for identification
        // check packet
        do {
          size_t i0 = 0, i1 = ev.packet.find(' ');
          if (i1 == std::string::npos) break;
          std::string part(ev.packet, i0, i1);
          if (part != "OK") break;
          i0 = i1+1;
          i1 = ev.packet.find(' ', i0);
          if (i1 == std::string::npos) break;
          part = std::string(ev.packet, i0, i1-i0);
          if (part != "EUDAQ") break;
          i0 = i1+1;
          i1 = ev.packet.find(' ', i0);
          if (i1 == std::string::npos) break;
          part = std::string(ev.packet, i0, i1-i0);
          if (part != "ENV") break;
          i0 = i1+1;
          i1 = ev.packet.find(' ', i0);
          part = std::string(ev.packet, i0, i1-i0);
          ev.id.SetType(part);
          i0 = i1+1;
          i1 = ev.packet.find(' ', i0);
          part = std::string(ev.packet, i0, i1-i0);
          ev.id.SetName(part);
	  //std::cout << "Correct string" << std::endl;
        } while(false);
        //std::cout << "client replied, sending OK" << std::endl;
        m_envserver->SendPacket("OK", ev.id, true);
        ev.id.SetState(1); // successfully identified
        OnConnect(ev.id);
      } else {
	
        //std::cout << "Receive:    " << ev.id << " " << ev.packet << std::endl;
        //for (size_t i = 0; i < 8 && i < ev.packet.size(); ++i) {
	//   std::cout << to_hex(ev.packet[i], 2) << ' ';
        //}
        //std::cout << ")" << std::endl;
        BufferSerializer ser(ev.packet.begin(), ev.packet.end());
        std::string src = ev.id.GetType();
        if (ev.id.GetName() != "") src += "." + ev.id.GetName();
        DoReceive(EnvMessage(ser).SetSender(src));
      }
      break;
    default:
      std::cout << "Unknown:    " << ev.id << std::endl;
    }
  }

  void EnvCollector::EnvThread() {
    try {
      //int i = 0;
      while (!m_done) {
        m_envserver->Process(100000);
	//i++;
	/*if (i ==10) { //once per second
		i=0; 
		std::cout << "In while loop of Envthread" << std::endl; 
		//m_envserver->SendPacket("ENVTRIG", ConnectionInfo::ALL);
		
	}*/
	SetStatus(eudaq::Status::LVL_USER,"bla");
      }
    } catch (const std::exception & e) {
      std::cout << "Error: Uncaught exception: " << e.what() << "\n"
                << "EnvThread is dying..." << std::endl;
    } catch (...) {
      std::cout << "Error: Uncaught unrecognised exception: \n"
                << "EnvThread is dying..." << std::endl;
    }
  }
  
  void EnvCollector::OnEnv(const std::string &env) {
	  //std::cout << "On Env: " << env << std::endl;
  }

}
