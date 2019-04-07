/* pins.h
MagOD2 libary 
Oct 2018
Define pins for communication between screen and the arduino MEGA
Perhaps these definitions should go to the different libraries (screen, led etc) Leon.
Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann */
#ifndef pins_h
#define pins_h


//TODO. LEON. Perhaps we should define the pins with the libaries. So temperatue pin in temparature library. coil pins in field library, led pins in led library etc.

class pins
{
 public:
  //does this really have to be public. Shield it in a function ReadTemp or so. TODO. LEON.
  const int Temp_read = 12;
 private:
};

#endif
