#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/TLUEvent.hh"
#include <string>

#if USE_LCIO
#  include <lcio.h>
#  include <IMPL/LCEventImpl.h>
#  include <IMPL/LCCollectionVec.h>
#endif

namespace eudaq{

  class TLUConverterPlugin : public DataConverterPlugin {
  public:
    TLUConverterPlugin() : DataConverterPlugin("TLUEvent") {}

    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    virtual bool GetLCIOSubEvent(lcio::LCEvent &, const eudaq::Event & source) const;

    /** Returns the event converted to. This is the working horse and the 
     *  main part of this plugin.
     */
    virtual bool GetStandardSubEvent(eudaq::StandardEvent & result, const eudaq::Event & source) const;

  private:
    /** The one single instance of the TLUConverterPlugin.
     *  It has to be created in the .cc file
     */
    static TLUConverterPlugin const m_tluconverterplugininstance;
  };

  TLUConverterPlugin const TLUConverterPlugin::m_tluconverterplugininstance;

#if USE_LCIO
  bool TLUConverterPlugin::GetLCIOSubEvent(lcio::LCEvent & result, const eudaq::Event & source) const {
    lcio::LCEventImpl & le = dynamic_cast<lcio::LCEventImpl &>(result);
    le.setTimeStamp(source.GetTimestamp());
    return true;
  }
#else
  bool TLUConverterPlugin::GetLCIOSubEvent(lcio::LCEvent &, const eudaq::Event &) const
  { return false; }
#endif

  bool TLUConverterPlugin::GetStandardSubEvent(StandardEvent & result, const eudaq::Event & source) const
  {
    const TLUEvent * tluev  = dynamic_cast<const TLUEvent *>(&source);
    if (!tluev) return false;
    result.SetTimestamp(tluev->GetTimestamp());
    return true;
  }

}//namespace eudaq
