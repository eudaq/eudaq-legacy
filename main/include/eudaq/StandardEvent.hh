#ifndef EUDAQ_INCLUDED_StandardEvent
#define EUDAQ_INCLUDED_StandardEvent

#include <vector>

namespace eudaq {

  class StandardEvent {
  public:
    typedef double pixel_t;
    StandardEvent(size_t pixels = 0, size_t frames = 1);

    size_t m_xsize, m_ysize;
    std::vector<unsigned> m_x, m_y;
    std::vector<std::vector<pixel_t> > m_pix;
    std::vector<bool> m_pivot;
  };

}

#endif // EUDAQ_INCLUDED_StandardEvent
