/* screen_8875.h
 MagOD2 libary 
 Mar 2019
 Definition of screen layout and screen update functions 
 for TFT050 screen (MAGOD2 system has that one) 
 Tijmen Hageman, Jordi Hendrix, Hans Keizer, Leon Abelmann 
*/
//Avoid that Arduino IDE compiles this file when not MagOD1
#include "../../MagOD.h" //MagOD version is defined in MagOD.h
#if defined(_MAGOD2)

#ifndef Screen_RA8875_h
#define Screen_RA8875_h

#include <Adafruit_GFX.h>
#include "Adafruit_RA8875.h"
#include "../recipes/recipes.h"

#define TFT_INT 16
#define TFT_CS 5
#define TFT_RST 17
#define SCRN_VERT 272
#define SCRN_HOR 480

// Color definitions
//https://wiibrew.org/wiki/U16_colors
//https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html
#define	TFTCOLOR_BLACK     RA8875_BLACK
#define	TFTCOLOR_BLUE      RA8875_BLUE
#define	TFTCOLOR_RED       RA8875_RED
#define	TFTCOLOR_GRAY      0x7B4D
#define	TFTCOLOR_LIGHTGRAY 0xBDF7
#define	TFTCOLOR_DARKGRAY  0x7BEF
#define	TFTCOLOR_GREEN     RA8875_GREEN
#define	TFTCOLOR_YELLOW    RA8875_YELLOW
#define	TFTCOLOR_WHITE     RA8875_WHITE
#define TFTCOLOR_CYAN      RA8875_CYAN
#define	TFTCOLOR_ORANGE    0xFBE0
#define	TFTCOLOR_BROWN     0x79E0
#define	TFTCOLOR_PING      0xF81F

class screen{
 public:
  screen(); //Constructor
  Adafruit_RA8875 tft = Adafruit_RA8875(TFT_CS, TFT_RST);
  void setupScreen();
  // https://forum.arduino.cc/index.php?topic=203124.0, answer #1:
  void showRecipes(struct recipe recipe_arr[], int N, int cnt);
  void updateInfo(unsigned int Looppar_1, unsigned int Looppar_2,
		  int16_t program_cnt, int led, const char *filename);
  void updateGraph(double value, int led);
  //void setScreenLimit(uint16_t val1, uint16_t val2);
  void updateV(diodes Vdiodes, references Vref, double OD,
	       feedbacks currents);
  void updateFILE(const char *str);
  void setRecButton(bool active);

  int16_t column_space; //Distance between columns on top
  int16_t locText_vSpace,locText_hSpace;//Distance between text fields
  int16_t screenSiz_x,screenSiz_h;//Size of screen
  int16_t locText_x,locText_y;//Location of top left corner text fields
  int16_t g_x,g_y,g_w,g_h;//Size of graph area
  
 private:
  int16_t g_xCursor, g_xCursor_prev;
  int16_t V1,V2,Vref,OD;
  double g_minVal,g_maxVal;
  double g_value_prev;
  double value_min;
  double value_max;
};

#endif // Screen_RA8875_h
#endif // _MAGOD2
