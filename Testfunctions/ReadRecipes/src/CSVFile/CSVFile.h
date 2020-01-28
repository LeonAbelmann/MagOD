/**
 used  existing library file
 *  @author: Slawomir Figiel
 *  @version: 1.0.1
 *  @license: MIT
 */
#ifndef CSVFile_h
#define CSVFile_h

#include <SdFat.h>
#include <Arduino.h>

#define GOTO_BEGIN_STARTS_WITH 0
#define CHECK_EMPTY_LINE 	   0


#define CSV_FILE_DELIMITER ','


class CSVFile{ //: public File{
	
	private:
	unsigned int numLine = 0;
	byte numField = 0;
	
	#if GOTO_BEGIN_STARTS_WITH
	
	//Change pointer position
	//Don't stop when end of field/line.
	
	bool isCurrentSubstring(const char * substr);
	#endif // GOTO_BEGIN_STARTS_WITH
	
	
	public:
	// CSVFile(): File() {}
	// virtual ~CSVFile() {}
	
	
	bool EndOfFile();
	// Always return true. 
	bool BeginOfFile();
	
	// *** Lines ***
	bool BeginOfLine();
	bool EndOfLine();
	
	//Return number current line. Lines are numbered from 0.
	unsigned int NumberOfLine();
	
	#if CHECK_EMPTY_LINE
	//After set pointer at end of line or at first not empty character
	//Require gotoBeginOfLine()
	bool EmptyLine();
	#endif	CHECK_EMPTY_LINE
	
	//bool BeginOfLine();
	
	// Return false when next line not exist. When return false
	// then positon in file is set at end.
	bool nextLine();
	bool gotoLine(unsigned int number);
	
	#if GOTO_BEGIN_STARTS_WITH
	// Find first line starts with argument string
	// from CURRENT position.
	// Probably you should call gotoBeginOfFile().
	bool gotoLine(const char * startsWith_);
	#endif
	
	
	// *** Fields ***
	bool EndOfField();
	
	// Always return TRUE
	bool BeginOfField();
	bool Field(byte num);
	
	// Return number of reading bytes
	// Return 0 when end reading current field
	// Auto-safe. If reading bytes is less then @bufferSize
	// then after last reading bytes is put null char.
	byte readField(char * buffer_, byte bufferSize);
	
	//Buffer size should be equals or greater then decimal place
	//reading value.
	//Return FALSE if not read field or field is empty.
	//Number can have a sign.
	//Number can be preceded by whitespaces.
	//Converting to number is end when is read first non-number char
	//Warning! If reading buffer don't contain any number char
	//then function return 0!
	bool readField(int& value, char * buffer_, byte bufferSize);
	
	// If this field is end field in line (file)
	// NOT go to next line and return FALSE
	// When return true set pointer to position
	// first char of file (after delimiter last field).
	bool nextField();
};
	
#endif //CSVFile_h