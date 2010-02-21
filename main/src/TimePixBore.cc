//#include "stdafx.h"
#include "eudaq/TimePixBore.hh"



namespace eudaq 
{

#ifdef PIXELMAN_FOR_EUDAQ
  TimePixBore::TimePixBore( unsigned int run, DevInfo *param, double timeToEndOfShutter,
			    double shutterLength, int moduleID,
			    DACTYPE *dacVals, size_t sizeOfDacVals)
    : Event(run, 0,     NOTIMESTAMP, Event::FLAG_BORE), // this is always a BORE, so event 0, no timestamp
      //           run, event, timestamp,   flags
      m_revision( SVN_REVISION ),  m_rowLen(param->rowLen),  m_numberOfRows( param->numberOfRows),
      m_mpxType( param->mpxType ),  m_chipboardIDstring( param->chipboardID, MPX_MAX_CHBID),
      m_ifaceName( param->ifaceName ), m_clockTimepix( param->clockTimepix),
      m_timeToEndOfShutter( timeToEndOfShutter ), m_shutterLength( shutterLength ),
      m_moduleID( moduleID ), m_dacVals( sizeOfDacVals )
  {
    for ( size_t index = 0 ; index < sizeOfDacVals ; ++index )
    {
      m_dacVals[index] = dacVals[index];
    }
  }
#endif

   TimePixBore::TimePixBore(Deserializer & ds )
     : Event( ds )
   {
     ds.read( m_revision );
     ds.read( m_rowLen );
     ds.read( m_numberOfRows );
     ds.read( m_mpxType );
     ds.read( m_chipboardIDstring );
     ds.read( m_ifaceName );
     ds.read( m_clockTimepix );
     ds.read( m_timeToEndOfShutter );
     ds.read( m_shutterLength );
     ds.read( m_moduleID );
     ds.read( m_dacVals );
   }

   void TimePixBore::Serialize (Serializer &s) const
   {
     // First call the serialiser of the event
     // In the deserialiser constructor the construcor of the mother class (Event) 
     // is called before the constructor of bore is called
     Event::Serialize(s);

     // now serialise all member variables
    s.write( m_revision );
    s.write( m_rowLen );
    s.write( m_numberOfRows );
    s.write( m_mpxType );
    s.write( m_chipboardIDstring );
    s.write( m_ifaceName );
    s.write( m_clockTimepix );
    s.write( m_timeToEndOfShutter );
    s.write( m_shutterLength );
    s.write( m_moduleID );
    s.write( m_dacVals );
   }

 // namespace eudaq
}