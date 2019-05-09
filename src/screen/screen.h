/* screen.h
 MagOD2 libary 
 
 Definition of screen layout and screen update functions
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

//Avoid that Arduino IDE compiles this file when not MagOD1
#include "../../MagOD.h" //MagOD version is defined in MagOD.h
#if defined(_MAGOD1)

#ifndef Screen_h
#define Screen_h

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
//pins
#define TFT_RST    9 
#define TFT_DC     8 
#define TFT_CS     10 
//resolution
#define SCRN_VERT 160
#define SCRN_HOR 128

// Color definitions
//https://wiibrew.org/wiki/U16_colors
#define	TFTCOLOR_BLACK   0x0000
#define	TFTCOLOR_BLUE    0x001F
#define	TFTCOLOR_RED     0xF800
#define	TFTCOLOR_GRAY    0xEEEE
#define	TFTCOLOR_GREEN   0x07E0
#define	TFTCOLOR_YELLOW  0xFFE0
#define	TFTCOLOR_WHITE   0xFFFF


class screen{
 public:
  //screen(int16_t w, int16_t h, Adafruit_GFX *tft_p); //Constructor
  screen(); //Constructor
  void setupScreen();
  void updateInfo(unsigned int Looppar_1, unsigned int Looppar_2, int16_t program_cnt);
  void updateGraph(double value, int led);
  //void setScreenLimit(uint16_t val1, uint16_t val2);
  void updateV(diodes Vdiodes, references Vref, double OD);
  void updateFILE(const char *str);
  void setRecButton(bool active);

 private:

  Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
  
  int16_t locText_x,locText_y,locText_vSpace,locText_hSpace;
  int16_t screenSiz_x,screenSiz_h;
  int16_t g_x,g_y,g_w,g_h;
  int16_t g_xCursor, g_xCursor_prev;
  int16_t V1,V2,Vref,OD;
  double g_minVal,g_maxVal;
  double g_value_prev;
  double value_min;
  double value_max;
};

#endif // Screen_h
#endif // _MAGOD1
