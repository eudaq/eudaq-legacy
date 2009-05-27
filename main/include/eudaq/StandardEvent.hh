#ifndef EUDAQ_INCLUDED_StandardEvent
#define EUDAQ_INCLUDED_StandardEvent

#include "eudaq/Event.hh"
#include <vector>
#include <string>

namespace eudaq {

  class StandardPlane : public Serializable {
  public:
    StandardPlane(unsigned id, const std::string & type, const std::string & sensor);
    StandardPlane(Deserializer &);
    void Serialize(Serializer &) const;
    typedef double pixel_t;
    typedef double coord_t;
    StandardPlane(size_t pixels = 0, size_t frames = 1);
    pixel_t GetPixel(size_t i) const;

    std::string m_type, m_sensor;
    unsigned m_id, m_tluevent;
    unsigned m_xsize, m_ysize;
    std::vector<coord_t> m_x, m_y;
    std::vector<std::vector<pixel_t> > m_pix;
    std::vector<bool> m_pivot;
  };

  class StandardEvent : public Event {
    EUDAQ_DECLARE_EVENT(StandardEvent);
  public:
    StandardEvent(unsigned run = 0, unsigned evnum = 0, unsigned long long timestamp = NOTIMESTAMP);
    StandardEvent(Deserializer &);
    void SetTimestamp(unsigned long long);
    void AddPlane(const StandardPlane &);
    size_t NumPlanes() const;
    const StandardPlane & GetPlane(size_t i) const;
    StandardPlane & GetPlane(size_t i);
    virtual void Serialize(Serializer &) const;
    virtual void Print(std::ostream &) const;
  private:
    std::vector<StandardPlane> m_planes;
  };

}

#endif // EUDAQ_INCLUDED_StandardEvent
