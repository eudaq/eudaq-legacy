#ifndef EUDAQ_INCLUDED_RawDataEvent
#define EUDAQ_INCLUDED_RawDataEvent

#include <sstream>

#include <vector>
#include "eudaq/Event.hh"

namespace eudaq {

  /** An Event type consisting of just a vector of bytes.
   *
   */
  class RawDataEvent : public Event {
    EUDAQ_DECLARE_EVENT(RawDataEvent);
  public:
    typedef std::vector<unsigned char> data_t;
    RawDataEvent(std::string type, unsigned run, unsigned event);
    RawDataEvent(Deserializer &);

    /// Add a data block as std::vector
    template <typename T>
    void AddBlock(const std::vector<T> & data) {
      m_data.push_back(make_vector(data));
    }

    /// Add a data block as array with given size
    template <typename T>
    void AddBlock(const T * data, size_t bytes) {
      m_data.push_back(make_vector(data, bytes));
    }

    /** Get the data block number i as vector of \c{unsigned char}, which is the byte sequence which
     *  which has been serialised. This is the recommended way to retrieve your
     *  data from the RawDataEvent since the other GetBlock functions might
     *  give different results depending on the endiannes of your mashine.
     */
    const data_t & GetBlock(size_t i) const;

    /// Return the number of data blocks in the RawDataEvent
    size_t NumBlocks() const { return m_data.size(); }

    virtual void Print(std::ostream &) const;
    static RawDataEvent BORE(std::string type, unsigned run) {
      return RawDataEvent(type, run, 0, Event::FLAG_BORE);
    }
    static RawDataEvent EORE(std::string type, unsigned run, unsigned event) {
      return RawDataEvent(type, run, event, Event::FLAG_EORE);
    }
    virtual void Serialize(Serializer &) const;

    /// Return the type string.
    virtual std::string GetSubType() const { return m_type; }

  private:
    // private constructor to create BORE and EORE
    // make sure that event number is 0 for BORE
    RawDataEvent(std::string type, unsigned run, unsigned event, Event::Flags flag)
      : Event(run, event, NOTIMESTAMP, flag) ,  m_type(type)
      {}

    template <typename T>
    static data_t make_vector(const T * data, size_t bytes) {
      const unsigned char * ptr = reinterpret_cast<const unsigned char *>(data);
      return data_t(ptr, ptr + bytes);
    }

    template <typename T>
    static data_t make_vector(const std::vector<T> & data) {
      const unsigned char * ptr = reinterpret_cast<const unsigned char *>(&data[0]);
      return data_t(ptr, ptr + data.size() * sizeof(T));
    }

    std::string m_type;
    std::vector<data_t> m_data;
  };

}

#endif // EUDAQ_INCLUDED_RawDataEvent
