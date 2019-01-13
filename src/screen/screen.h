/* screen.h
 MagOD2 libary 
 Oct 2018
 Definition of screen layout and screen update functions 
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/

#ifndef Screen_h
#define Screen_h

#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

//Pins for the screen
#define TFT_CS     10 
#define TFT_RST    9 
#define TFT_DC     8 

// Color definitions
#define	ST7735_BLACK   0x0000
#define	ST7735_BLUE    0x001F
#define	ST7735_RED     0xF800
#define	ST7735_GRAY    0xEEEE
#define	ST7735_GREEN   0x07E0


class screen{
 public:
  //screen(int16_t w, int16_t h, Adafruit_GFX *tft_p); //Constructor
  screen(); //Constructor
  // Screen resolution
  static const int scrn_vert = 160;
  static const int scrn_hor = 128;
  
  void setupScreen();
  void updateInfo(unsigned int Looppar_1, unsigned int Looppar_2, int16_t program_cnt);
  void updateGraph(double value, int led);
  //void setScreenLimit(uint16_t val1, uint16_t val2);
  void updateV(double Vav, double Vled, double Vref, double OD);
  void updateFILE(const char *str);
  void setRecButton(bool active);

 private:
  //initialization of communication with screen
  /*  Adafruit_GFX *tft; */
  
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
