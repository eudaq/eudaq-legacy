#include "eudaq/StandardEvent.hh"

namespace eudaq {

  StandardEvent::StandardEvent(size_t pixels, size_t frames) 
    : m_x(pixels), m_y(pixels), m_pix(frames, std::vector<pixel_t>(pixels)), m_pivot(pixels)
  {
    //
  }

}
