#include "eudaq/StandardEvent.hh"

namespace eudaq {

  EUDAQ_DEFINE_EVENT(StandardEvent, str2id("_STD"));

  StandardPlane::StandardPlane(const std::string & type, const std::string & sensor)
    : m_type(type), m_sensor(sensor)
  {}

  StandardPlane::StandardPlane(Deserializer & ds) {
    ds.read(m_type);
    ds.read(m_sensor);
    ds.read(m_xsize);
    ds.read(m_ysize);
    ds.read(m_x);
    ds.read(m_y);
    ds.read(m_pix);
    ds.read(m_pivot);
  }

  void StandardPlane::Serialize(Serializer & ser) const {
    EUDAQ_THROW("StandardPlane serialization not yet implemented (wait until implementation has stabilised)");
    ser.write(m_type);
    ser.write(m_sensor);
    ser.write(m_xsize);
    ser.write(m_ysize);
    ser.write(m_x);
    ser.write(m_y);
    ser.write(m_pix);
    ser.write(m_pivot);
  }

  StandardPlane::StandardPlane(size_t pixels, size_t frames) 
    : m_x(pixels), m_y(pixels), m_pix(frames, std::vector<pixel_t>(pixels)), m_pivot(pixels)
  {
    //
  }

  StandardEvent::StandardEvent(unsigned run, unsigned evnum, unsigned long long timestamp)
    : Event(run, evnum, timestamp)
  {
  }

  StandardEvent::StandardEvent(Deserializer & ds)
    : Event(ds)
  {
    ds.read(m_planes);
  }

  void StandardEvent::Serialize(Serializer & ser) const {
    EUDAQ_THROW("StandardEvent serialization not yet implemented (wait until implementation has stabilised)");
    Event::Serialize(ser);
    ser.write(m_planes);
  }

  void StandardEvent::SetTimestamp(unsigned long long val) {
    m_timestamp = val;
  }

  void StandardEvent::Print(std::ostream & os) const {
    os << "Planes " << m_planes.size();
  }

  void StandardEvent::AddPlane(const StandardPlane & plane) {
    m_planes.push_back(plane);
  }

}
