#ifndef TLU_SYNCHRONISER_H
#define TLU_SYNCHRONISER_H

#include <pthread.h>
#include <string>

/** The TLUSynchroniser is a helper class 
 *   which reads the parport busy 
 *    (connected to the TLU Trigger)
 *   and asserts the  strobe line 
 *    (connected to TLU busy).
 * 
 *  In addition there is a reset line connected.
 *  It is high by default and goes to ground to perform the reset.
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

  /// lowers the busy line for a moment to perform a reset
  void reset();

 private:
  //  std::string _device; ///< the device name
  int _parport; ///< the file descriptor for the parallel port

  pthread_mutex_t _parportMutex;

};

#endif // TLU_SYNCHRONISER_H
