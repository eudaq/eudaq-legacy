#include "TLUSynchroniser.h"
#include <stdexcept>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/ppdev.h>
#include <linux/parport.h>
#include <stropts.h>
#include <iostream>
#include <unistd.h>

TLUSynchroniser::TLUSynchroniser( std::string const device )
{
    // Inititalise the mutex
    pthread_mutex_init( &_parportMutex, 0 );
    // as long as the constructor is not finished we don't need to protect the hardware

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

void TLUSynchroniser::reset()
{
  ppdev_frob_struct frobWord;

  // lower the init line;
  frobWord.mask = PARPORT_CONTROL_INIT;
  frobWord.val  = 0;

  pthread_mutex_lock( &_parportMutex );
     ioctl(_parport, PPFCONTROL, &frobWord );
  
     /* Delay for a bit */
     timespec oneMicrosecond;
     oneMicrosecond.tv_sec = 0;
     oneMicrosecond.tv_nsec = 1000;
     nanosleep (&oneMicrosecond, NULL);
 
     // raise the init line again
     frobWord.val  = PARPORT_CONTROL_INIT;
     ioctl(_parport, PPFCONTROL, &frobWord );  
  pthread_mutex_unlock( &_parportMutex );  

  std::cout << "wait for reset..." << std::flush;
  /* Give the U2F time to recover from the reset */
  timespec halfASecond;
  oneMicrosecond.tv_sec = 5;
  oneMicrosecond.tv_nsec = 500000000;
  sleep(5);
  nanosleep (&halfASecond, NULL);

  std::cout << " Reset done" << std::endl;
     
}

bool TLUSynchroniser::readTrigger()
{
  unsigned char statusWord;
  pthread_mutex_lock( &_parportMutex );
    ioctl(_parport, PPRSTATUS, &statusWord);
  pthread_mutex_unlock( &_parportMutex );
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
  unsigned char controlWord = PARPORT_CONTROL_INIT;
  if  ( !busyState ) controlWord |= PARPORT_CONTROL_STROBE ;
  //  std::cout << "Setting control word 0x" << std::hex << static_cast<unsigned short>(controlWord) 
  //	    << std::dec << std::endl;

  pthread_mutex_lock( &_parportMutex );
    ioctl(_parport, PPWCONTROL, &controlWord );
  pthread_mutex_unlock( &_parportMutex );
}

TLUSynchroniser::~TLUSynchroniser()
{
  // Release the busy line so TLU will not be blocked
  setBusy(false);

  //release control of the parallel port and close the device
  ioctl (_parport, PPRELEASE);
  close( _parport );

  // Destroy the mutex
  pthread_mutex_destroy( &_parportMutex );
  
}
