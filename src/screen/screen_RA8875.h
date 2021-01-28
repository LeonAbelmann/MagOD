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
#define	TFTCOLOR_DARKGRAY  0x39C2
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

  void setupScreen(double t_minval, double t_maxval,
		   double g_minval, double g_maxval);

  
  // https://forum.arduino.cc/index.php?topic=203124.0, answer #1:
  void showRecipes(struct recipe recipe_arr[], int N, int cnt);

  /* data fields: */
  void updateInfo(unsigned int Looppar_1, unsigned int Looppar_2,
		  int16_t program_cnt, int led, const char *filename);
  void updateV(diodes Vdiodes, references Vref, double OD,
	       feedbacks currents);
  void updateFILE(char *str);
  void setRecButton(bool active);

  /* graph */
  void updateGraph(dataPlot *graphArray,
		   int graphCount, int graphLength,
		   recipe recArray[], int program);
  void clearGraph(recipe recArray[], int program);
  void graphUpdateScale(double g_minVal, double g_maxVal);
  void graphAutoScale(dataPlot *graphArray,
		      int graphCount, int graphLength,
		      recipe recArray[], int program);
  void graphRecipeLines(recipe recArray[], int program);

  int16_t screenSiz_x = SCRN_HOR; //Size of screen
  int16_t screenSiz_h = SCRN_VERT;

  //Text area
  int16_t column_space = 85; //Space between the two columns of text data
  int16_t locText_vSpace = 12;//Vertical distance between text fields
  int16_t locText_hSpace = 40;//Horizontal distance between text fields
  //Graph area
  int16_t g_x = 40;//Graph area (top left x,y, width, height
  int16_t g_y = 88;
  int16_t g_w = SCRN_HOR-g_x-1;
  int16_t g_h = SCRN_VERT-g_y-12;
  
 private:
  // For text fields
  int16_t locText_x =0;//Location of top left corner text fields
  int16_t locText_y= 0;
  int16_t V1,V2,Vref,OD;//Obsolete? LEON
  
  // For graph:
  int lastCount = 0; // Which point was updated last?
  double g_minVal,g_maxVal; // Min, max value of y range in graph (V)
  double graphMinScale = 0.010; //Min vertical scale (V)
};

#endif // Screen_RA8875_h
#endif // _MAGOD2
