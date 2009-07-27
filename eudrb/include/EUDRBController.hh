#ifndef EUDAQ_INCLUDED_EUDRBController_hh
#define EUDAQ_INCLUDED_EUDRBController_hh

#include "eudaq/Utils.hh"
#include "eudaq/Configuration.hh"
#include "VMEInterface.hh"

#include <vector>
#include <string>

namespace eudaq {

  class EUDRBController {
  public:
    typedef std::vector<unsigned> pedestal_t;
    enum MODE_T { M_NONE, M_RAW3, M_RAW2, M_ZS, M_ZS2 };
    enum DET_T  { D_NONE, D_MIMOSA5, D_MIMOSTAR2, D_MIMOTEL, D_MIMOSA18, D_MIMOSA26 };
    EUDRBController(int id, int slot, int version);
    int Version() const { return m_version; }
    std::string Mode() const;
    std::string Det() const;

    void Configure(const eudaq::Configuration & param, int master);
    std::string PostConfigure(const eudaq::Configuration & param, int master); // Download pedestals, send M26 start pulse, returns ped file name if any
    void ResetTriggerProc();
    void ResetBoard();
    bool WaitForReady(double timeout = 20.0);
    void LoadPedestals(const pedestal_t & peds);
    int EventDataReady_size(double timeout = 1.0); /// Returns 0 on timeout, else event size, or -1 if size unknown
    void ReadEvent(std::vector<unsigned long> &);
    void ResetTriggerBusy();
    static int ModeNum(const std::string & mode);
    static int DetNum(const std::string & det);
  private:
    static pedestal_t GeneratePedestals(int thresh = 10, int ped = 0);
    static pedestal_t ReadPedestals(const std::string & filename, float sigma, int det);
    int m_id, m_version, m_mode, m_det, m_ctrlstat;
    VMEptr m_vmes, m_vmed;
  };

}

#endif // EUDAQ_INCLUDED_EUDRBController_hh
