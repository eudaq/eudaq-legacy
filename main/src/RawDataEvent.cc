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
    return m_data.at(i);
  }

  RawDataEvent::byte_t RawDataEvent::GetByte(size_t block, size_t index) const {
    return GetBlock(block).at(index);
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
