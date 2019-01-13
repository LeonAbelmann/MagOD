/* MagOD2.h
 MagOD2 libary 
 Oct 2018
 Struct definitions
 Leon Abelmann 
*/
#ifndef MagOD2_h
#define MagOD2_h

/* Reference diode voltages */
struct references {
  double Vref;   // Current value
  double Vred;   // Voltage for red LED
  double Vgreen; // Voltage for green LED
  double Vblue;  // Voltage for blue LED
};

#endif
