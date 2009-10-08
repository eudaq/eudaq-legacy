#include "TLUSynchroniser.h"
#include <stdexcept>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>
#include <stropts.h>
#include <iostream>

TLUSynchroniser::TLUSynchroniser( std::string const device )
{
  // open the device. This does not block/open  the port itself yet
  _parport = open (device.c_str(), O_RDWR);
  if (_parport == -1) 
  {
    perror ("open");
    throw std::runtime_error("TLUSynchroniser: Could not open parallel device.");
  }

  // block the port and get access to it
  if (ioctl (_parport, PPCLAIM))
  {
    perror ("PPCLAIM");
    close (_parport);
    throw std::runtime_error("TLUSynchroniser: Claiming access to parallel port failed.");
  }

  // set the busy high to precent triggers from the TLU
  setBusy(true);

  // now check that the trigger line is low to avoid race condition
  /* Delay for a bit */
  timespec oneMicrosecond;
  oneMicrosecond.tv_sec = 0;
  oneMicrosecond.tv_nsec = 1000;
  nanosleep (&oneMicrosecond, NULL);

  // check the trigger line
  if ( readTrigger() )
  {
    // there is a race condition. Lower the busy line and raise it again. 
    // After this the trigger has to be low, otherwise there is something very wrong
    setBusy(false);
    nanosleep (&oneMicrosecond, NULL);
    setBusy(true);
    
    // now check the trigger line again
    nanosleep (&oneMicrosecond, NULL);
    if ( readTrigger() )
    {
      throw std::runtime_error("TLUSynchroniser: TLU does not react to bysy signal and always has trigger high.");
    }
  }
}

bool TLUSynchroniser::readTrigger()
{
  unsigned char statusWord;
  ioctl(_parport, PPRSTATUS, &statusWord);
  //  std::cout << "Read status " << std::hex << static_cast<unsigned short>(statusWord) 
  //	    << std::dec << std::endl;

  // the parport busy input is conneted to TLU trigger
  // As the hardware inverts the logic, we have to revert it back here again
  return ( ! (statusWord & PARPORT_STATUS_BUSY) );

}

void TLUSynchroniser::setBusy(bool busyState)
{
  // if busy state is true assert the stobe line, otherwise deassert it
  // As the hardware inverts the signal, we have to apply !busyState
  unsigned char controlWord = ( !busyState ? PARPORT_CONTROL_STROBE : 0 );
  //  std::cout << "Setting control word 0x" << std::hex << static_cast<unsigned short>(controlWord) 
  //	    << std::dec << std::endl;
  ioctl(_parport, PPWCONTROL, &controlWord );
}

TLUSynchroniser::~TLUSynchroniser()
{
  // Release the busy line so TLU will not be blocked
  setBusy(false);

  //release control of the parallel port and close the device
  ioctl (_parport, PPRELEASE);
  close( _parport );
}
