/* Calibration parameters */



// System that is at CEA Cadarache
#if defined(_CEA)
/* Mirror TFT screen (determines what is the front of the controller) */
const bool mirror_tft = true;
/* Do you want Wifi :). Make sure password.h is present, see under FTP
   server for details */
const bool wifi = true;


/* RA8875 Touchscreen calibration. Run adapted ts_calibration.ino (Testfunctions/MagOD2/TestTFT50/)*/
# define TOUCH_DIVIDER -339148
# define TOUCH_An      182400
# define TOUCH_Bn      1152
# define TOUCH_Cn      -179366976
# define TOUCH_Dn      109
# define TOUCH_En      116739
# define TOUCH_Fn      -103929226

/* PWM to current calibration */
/* B>0:PWM_value=abs(Ax_pos_TPWM * Val_Bmag_x + Bx_pos_TPWM)
   B<0 PWM_value=abs(Ax_neg_TPWM * Val_Bmag_x + Bx_neg_TPWM)
   same for Ay, Az etc. */
/* In first order 3.1V gives 1A gives 1.5 mT. 12/3.1 V is PWM of
   25%.  So 0.25/1.5*2^resolution PWM/mT = 42 bits/mT (resolution 8
   bit). 42 is always a good number :) */
/* You can calibrate the coils using TestCurrentCalibration.ino  */
/* In CurrentCalibration.ods you will find the current as
   function of PWM, in terms of A/bit. To get Ax_neg_TPMW in bits/mT
   take 1/(slope*mtPerAmp). 0.01356 A/bit and 1.5 mT/A gives
   Ax_neg_TPWM=-49.1584 bit/mT
*/  
const double Ax_neg_TPWM = 49.16; 
const double Bx_neg_TPWM = 0; 
const double Ax_pos_TPWM = 49.21; 
const double Bx_pos_TPWM = 0; 

const double Ay_neg_TPWM = 49.19; 
const double By_neg_TPWM = 0; 
const double Ay_pos_TPWM = 49.34; 
const double By_pos_TPWM = 0; 

const double Az_neg_TPWM = 49.93; 
const double Bz_neg_TPWM = 0; 
const double Az_pos_TPWM = 49.73; 
const double Bz_pos_TPWM = 0;

/* Current calibration */
/* We assume calibration is done at GAIN_ONE. Use the testprogram
   TestCurrentCalibration.ino and a multimeter to get the relation
   between ADC bits and currents. See CurrentCalibration.ods*/
/* ADC 4 measures current in X-direction */
const int    adc4Offset   = 13006;
const double adc4SlopePos = 0.00018374;
const double adc4SlopeNeg = 0.00024586;
/* ADC 5 measures current in Z-direction !!!*/
const int    adc5Offset   = 13043;
const double adc5SlopePos = 0.00019299;
const double adc5SlopeNeg = 0.00024766;
  /* ADC 6 measures current in Y-direction !!!*/
const int    adc6Offset   = 12960;
const double adc6SlopePos = 0.00027076;
const double adc6SlopeNeg = 0.00024817;

/* Temperature calibration */
/* See NTCAGLUG02 datasheet */
const double NTCT     = 25.0; // NTC central temperature
const double NTCC     = -4.39e-2; //NTC temperature coeff at NTCT
const double NTCR     = 1.0; /* Ratio between R26=10k and 10 kOhm of
				NTC at 25oC */
const double NTCV     = 5.0;  //Drive voltage

/* Offset photodetector */
const double PDOFFSET = 4.096; // Photodetector offset

#endif

// System that is at KIST Europe
#if defined(_KIST)
/* Mirror TFT screen (determines what is the front of the controller) */
const bool mirror_tft = true;
/* Do you want Wifi:). Make sure password.h is present, see under FTP
   server for details */
const bool wifi = true;


/* RA8875 Touchscreen calibration. Run adapted ts_calibration.ino (Testfunctions/MagOD2/TestTFT50/)*/
# define TOUCH_DIVIDER -342554
# define TOUCH_An      187200
# define TOUCH_Bn      -768
# define TOUCH_Cn      -182232864
# define TOUCH_Dn      327
# define TOUCH_En      114886
# define TOUCH_Fn      -103861067

/* PWM to current calibration */
/* B>0:PWM_value=abs(Ax_pos_TPWM * Val_Bmag_x + Bx_pos_TPWM)
   B<0 PWM_value=abs(Ax_neg_TPWM * Val_Bmag_x + Bx_neg_TPWM)
   same for Ay, Az etc.
*/
/* In first order 3.1V gives 1A gives 1.5 mT. 12/3.1 V is PWM of
   25%.  So 0.25/1.5*2^resolution PWM/mT = 42 bits/mT (resolution 8
   bit). 42 is always a good number :) */
/* You can calibrate the coils using TestCurrentCalibration.ino  */
/* In CurrentCalibration.ods you will find the current as
   function of PWM, in terms of A/bit. To get Ax_neg_TPMW in bits/mT
   take 1/(slope*mtPerAmp). 0.01356 A/bit and 1.5 mT/A gives
   Ax_neg_TPWM=-49.1584 bit/mT
*/  
const double Ax_neg_TPWM = 63.84; 
const double Bx_neg_TPWM = 0; 
const double Ax_pos_TPWM = 62.62; 
const double Bx_pos_TPWM = 0; 

const double Ay_neg_TPWM = 61.95; 
const double By_neg_TPWM = 0; 
const double Ay_pos_TPWM = 63.50; 
const double By_pos_TPWM = 0; 

const double Az_neg_TPWM = 60.17; 
const double Bz_neg_TPWM = 0; 
const double Az_pos_TPWM = 61.22; 
const double Bz_pos_TPWM = 0;

/* Current calibration */
/* We assume calibration is done at GAIN_ONE. Use the testprogram
   TestCurrentCalibration.ino and a multimeter to get the relation
   between ADC bits and currents. See CurrentCalibration.ods*/
/* ADC 4 measures current in X-direction */
const int    adc4Offset   = 13006;
const double adc4SlopePos = 0.000261;
const double adc4SlopeNeg = 0.000261;

/* ADC 5 measures current in Z-direction !!!*/
const int    adc5Offset   = 13038;
const double adc5SlopePos = 0.000260;
const double adc5SlopeNeg = 0.000260;

  /* ADC 6 measures current in Y-direction !!!*/
const int    adc6Offset   = 13041;
const double adc6SlopePos = 0.000261;
const double adc6SlopeNeg = 0.000261;

/* Temperature calibration */
// Between 5V_a and A7 At 23 degC, 9.9 kOhm, 1.30 V, 0 degC 32 kOhm, 0.45 V
// That is 9.7e-2 /K (correct!)
const double NTCT     = 23.0; // NTC central temperature
const double NTCC     = -9.7e-2; //NTC temperature coeff at NTCT
const double NTCR     = 0.323; /* Ratio between R26=3.2k and 10 kOhm of
				NTC at 23oC */
const double NTCV     = 5.0;  //Drive voltage

/* Offset photodetector */
const double PDOFFSET = 4.096; // Photodetector offset

#endif

#if defined(_BAYREUTH)
/* Mirror TFT screen (determines what is the front of the controller) */
const bool mirror_tft = true;
/* Do you want Wifi :). Make sure password.h is present, see under FTP
   server for details */
const bool wifi = false;

/* RA8875 Touchscreen calibration. Run adapted ts_calibration.ino (Testfunctions/MagOD2/TestTFT50/ts_calibration/)*/

# define TOUCH_DIVIDER -360072
# define TOUCH_An      188736
# define TOUCH_Bn      6144
# define TOUCH_Cn      -190275456
# define TOUCH_Dn      -9701
# define TOUCH_En      119464
# define TOUCH_Fn      -105996848

/* PWM to current calibration */
/* B>0:PWM_value=abs(Ax_pos_TPWM * Val_Bmag_x + Bx_pos_TPWM) */
/* B<0 PWM_value=abs(Ax_neg_TPWM * Val_Bmag_x + Bx_neg_TPWM) same for
   Ay, Az etc.*/
/* In first order at full range, 255 PWM is 12V, we measured 2.5A and
   5.6 mT. 255 bit/5.6mT = 45 bit/mT */
/* You can calibrate the coils using TestCurrentCalibration.ino  */
/* In CurrentCalibration.ods you will find the current as
   function of PWM, in terms of A/bit. To get Ax_neg_TPMW in bits/mT
   take 1/(slope*mtPerAmp). 0.01356 A/bit and 1.5 mT/A gives
   Ax_neg_TPWM=-49.1584 bit/mT
*/  
  const double Ax_neg_TPWM = 48.1; 
  const double Bx_neg_TPWM = 0; 
  const double Ax_pos_TPWM = 48.0;
  const double Bx_pos_TPWM = 0; 

  const double Ay_neg_TPWM = 48.6; 
  const double By_neg_TPWM = 0; 
  const double Ay_pos_TPWM = 48.7; 
  const double By_pos_TPWM = 0; 

  const double Az_neg_TPWM = 51.9; 
  const double Bz_neg_TPWM = 0; 
  const double Az_pos_TPWM = 52.9; 
  const double Bz_pos_TPWM = 0;

/* Current calibration */
/* We assume calibration is done at GAIN_ONE. Use the testprogram
   TestCurrentCalibration.ino and a multimeter to get the relation
   between ADC bits and currents. See CurrentCalibration.ods*/
/* ADC 4 measures current in X-direction */
const int    adc4Offset   = 12955;
const double adc4SlopePos = -0.000238;
const double adc4SlopeNeg = -0.000238; /* not working properly! */
  /* ADC 6 measures current in Y-direction !!!*/
const int    adc6Offset   = 13199;
const double adc6SlopePos = -0.000240;
const double adc6SlopeNeg = -0.000223; /* not working properly */
/* ADC 5 measures current in Z-direction !!!*/
const int    adc5Offset   = 13050;
const double adc5SlopePos = -0.000238;
const double adc5SlopeNeg = -0.000238;

/* Temperature calibration */
/* See NTCAGLUG02 datasheet */
const double NTCT     = 25.0; // NTC central temperature
const double NTCC     = -4.39e-2; //NTC temperature coeff at NTCT
const double NTCR     = 1.0; /* Ratio between R26=10k and 10 kOhm of
				NTC at 25oC */
const double NTCV     = 5.0;  //Drive voltage

/* Offset photodetector */
const double PDOFFSET = 4.096; // Photodetector offset

#endif

#if defined(_ASTON)
/* Mirror TFT screen (determines what is the front of the controller) */
const bool mirror_tft = true;
/* Do you want Wifi :). Make sure password.h is present, see under FTP
   server for details */
const bool wifi = false;

/* RA8875 Touchscreen calibration. Run adapted ts_calibration.ino (Testfunctions/MagOD2/TestTFT50/ts_calibration/)*/
# define TOUCH_DIVIDER -353559
# define TOUCH_An      188736
# define TOUCH_Bn      768
# define TOUCH_Cn      -184074960
# define TOUCH_Dn      -545
# define TOUCH_En      117611
# define TOUCH_Fn      -106917755


/* PWM to current calibration */
/* B>0:PWM_value=abs(Ax_pos_TPWM * Val_Bmag_x + Bx_pos_TPWM) */
/* B<0 PWM_value=abs(Ax_neg_TPWM * Val_Bmag_x + Bx_neg_TPWM) same for
   Ay, Az etc.*/
/* In first order at full range, 255 PWM is 12V, we measured 2.5A and
   5.6 mT. 255 bit/5.6mT = 45 bit/mT */
/* You can calibrate the coils using TestCurrentCalibration.ino  */
/* In CurrentCalibration.ods you will find the current as
   function of PWM, in terms of A/bit. To get Ax_neg_TPMW in bits/mT
   take 1/(slope*mtPerAmp). 0.01356 A/bit and 1.5 mT/A gives
   Ax_neg_TPWM=-49.1584 bit/mT
*/  
  const double Ax_neg_TPWM = 48.1; 
  const double Bx_neg_TPWM = 0; 
  const double Ax_pos_TPWM = 48.0;
  const double Bx_pos_TPWM = 0; 

  const double Ay_neg_TPWM = 48.6; 
  const double By_neg_TPWM = 0; 
  const double Ay_pos_TPWM = 48.7; 
  const double By_pos_TPWM = 0; 

  const double Az_neg_TPWM = 51.9; 
  const double Bz_neg_TPWM = 0; 
  const double Az_pos_TPWM = 52.9; 
  const double Bz_pos_TPWM = 0;

/* Current calibration */
/* We assume calibration is done at GAIN_ONE. Use the testprogram
   TestCurrentCalibration.ino and a multimeter to get the relation
   between ADC bits and currents. See CurrentCalibration.ods*/
/* ADC 4 measures current in X-direction */
const int    adc4Offset   = 13111;
const double adc4SlopePos = -0.000238;
const double adc4SlopeNeg = -0.000238; /* not working properly! */
/* ADC 6 measures current in Y-direction !!!*/
const int    adc6Offset   = 12989;
const double adc6SlopePos = -0.000240;
const double adc6SlopeNeg = -0.000223; /* not working properly */
/* ADC 5 measures current in Z-direction !!!*/
const int    adc5Offset   = 13110;
const double adc5SlopePos = -0.000238;
const double adc5SlopeNeg = -0.000238;

/* Temperature calibration */
/* See NTCAGLUG02 datasheet */
const double NTCT = 25.0; // NTC central temperature
const double NTCC = -4.39e-2; //NTC temperature coeff at NTCT
const double NTCR = 0.3204; /* Ratio between R26=3.204k and 10 kOhm of
				NTC at 25oC */
const double NTCV = 5.0;  //Drive voltage

/* Offset photodetector */
const double PDOFFSET = 4.096; // Photodetector offset

#endif
