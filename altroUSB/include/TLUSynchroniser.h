#ifndef TLU_SYNCHRONISER_H
#define TLU_SYNCHRONISER_H

#include <string>

/** The TLUSynchroniser is a helper class 
 *   which reads the parport busy 
 *    (connected to the TLU Trigger)
 *   and asserts the  strobe line 
 *    (connected to TLU busy).
 */

class TLUSynchroniser
{
 public:
  TLUSynchroniser( std::string const device = "/dev/parport0" );
  ~TLUSynchroniser();

  /// Checks if the TLU trigger line is high
  bool readTrigger();

  /// (de)asserts the busy line
  void setBusy(bool busyState);

 private:
  //  std::string _device; ///< the device name
  int _parport; ///< the file descriptor for the parallel port

};

#endif // TLU_SYNCHRONISER_H
