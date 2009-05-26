#include "eudaq/RawDataEvent.hh"

#include <ostream>

namespace eudaq {

  EUDAQ_DEFINE_EVENT(RawDataEvent, str2id("_RAW"));

  RawDataEvent::RawDataEvent(std::string type, unsigned run, unsigned event) :
    Event(run, event),
    m_type(type)
  {
  }

  RawDataEvent::RawDataEvent(Deserializer & ds) :
    Event(ds)
  {
    ds.read(m_type);
    ds.read(m_data);
  }

  const RawDataEvent::data_t & RawDataEvent::GetBlock(size_t i) const {
    if (i >= m_data.size()) {
      EUDAQ_THROW("RawDataEvent::GetBlock("+to_string(i)+"): Error :"
                  "Only "+to_string(m_data.size())+" data blocks available!");
    }
    return m_data[i];
  }

  void RawDataEvent::Print(std::ostream & os) const {
    Event::Print(os);
    os << ", " << m_data.size() << " blocks";
  }

  void RawDataEvent::Serialize(Serializer & ser) const {
    Event::Serialize(ser);
    ser.write(m_type);
    ser.write(m_data);
  }

}
