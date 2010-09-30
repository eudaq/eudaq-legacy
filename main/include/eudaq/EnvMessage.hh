#ifndef EUDAQ_INCLUDED_EnvMessage
#define EUDAQ_INCLUDED_EnvMessage

#include <string>
#include <vector>

#include "eudaq/Serializable.hh"
#include "eudaq/Serializer.hh"
#include "eudaq/Status.hh"
#include "eudaq/Time.hh"

namespace eudaq {

  class EnvMessage : public Status {
  public:
    EnvMessage(const std::string & msg = "",
               Level level = LVL_EXTRA,
               const Time & t = Time::Current());
    EnvMessage(Deserializer &);
    virtual void Serialize(Serializer &) const;
    virtual void Print(std::ostream &) const;
    void Write(std::ostream &) const;
    static EnvMessage Read(std::istream &);
    EnvMessage & SetLocation(const std::string & file, unsigned line, const std::string & func = "");
    EnvMessage & SetSender(const std::string & name);
    std::string GetSender() const { return m_sendertype + (m_sendername == "" ? std::string("") : "." + m_sendername); }
    std::string GetSenderType() const { return m_sendertype; }
    std::string GetSenderName() const { return m_sendername; }
  protected:
    std::string m_file, m_func, m_sendertype, m_sendername;
    unsigned m_line;
    Time m_time, m_createtime;
  };

  inline std::ostream & operator << (std::ostream &os, const EnvMessage &ev) {
    ev.Print(os);
    return os;
  }


}

#endif // EUDAQ_INCLUDED_EnvMessage
