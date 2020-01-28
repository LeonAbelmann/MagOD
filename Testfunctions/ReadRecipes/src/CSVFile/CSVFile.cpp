#include "CSVFile.h"

#define File_END_OF_LINE ' '
#define NULL_CHAR '\0'

#if GOTO_BEGIN_STARTS_WITH

bool CSVFile::CurrentSubstring(const char * substr) 
{
	while(*substr)
	{
		int chVal = read();
		if (*substr++ != chVal)
		{
			return false;
		}
	}
	return true;
}
#endif GOTO_BEGIN_STARTS_WITH


bool CSVFile::EndOfFile() 
{
	return available() == 0;
}

// Always return true.
bool CSVFile::BeginOfFile() 
{
	rewind();
	numField = 0;
	
	numLine = 0;
	return true;
}

// *** Lines ***
bool CSVFile::BeginOfLine()
{
	//Begin of file
	if (!seekSet(-1))
	{
		return true;
	}
	int ch = read();
	return ch == File_END_OF_LINE;
}

bool CSVFile::EndOfLine() 
{
	//End of file
	if (EndOfFile())
	{
		return true;
	}
	bool status_ = read() == File_END_OF_LINE;
	seekSet(-1);
	return status_;
}

//Return number current line. Lines are numbered from 0.
unsigned int CSVFile::NumberOfLine()
{
	return numLine;
}

#if CHECK_EMPTY_LINE
//After set pointer at end of line or at first not empty character
//Require gotoBeginOfLine()
bool CSVFile::EmptyLine()
{
	int chVal = read();
	while (chVal >= 0)
	{
		if (((char) chVal) == File_END_OF_LINE)
		{
			seekSet(-1);
			return true;
		}
		else if (((char)chVal) != NULL_CHAR)
		{
			return false;
		}
		chVal = read();
	}
	return true; //End of line
}
#endif

bool CSVFile::BeginOfLine() 
{
	numField = 0;
	//Begin of file;
	if (!seekSet(-1))
	{
		return true;
	}

	int chVal = read();

	while (chVal != File_END_OF_LINE)
	{
		if (!seekSet(-2))
		{
			seekSet(-1);
			return true;
		}
		chVal = read();
	}

	return true;
}

bool CSVFile::nextLine() 
{
	int chVal = read();
	//End of file
	if (chVal < 0)
	{
		return false;
	}
	//Content of current line
	while (chVal != File_END_OF_LINE)
	{
		chVal = read();
		//End of file
		if (chVal < 0)
		{
			return false;
		}
	}

	numLine += 1;
	return true;
}

bool CSVFile::gotoLine(unsigned int num) 
{
	if (num == numLine)
	{
		return BeginOfLine();
	}
	else if (num < numLine)
	{
		BeginOfFile();
	}

	while (num != numLine)
	{
		if (!nextLine())
		{
			return false;
		}
	}
	return true;
}

#if GOTO_BEGIN_STARTS_WITH
// Find first line starts with argument string
// from CURRENT position.
// Probably you should call gotoBeginOfFile().
bool CSVFile::gotoLine(const char * startsWith_) 
{
	while (!CurrentSubstring(startsWith_))
	{
		if (!nextLine())
		{
			return false;
		}
	}
	BeginOfLine();
	return true;
}
#endif

// *** Fields ***
bool CSVFile::EndOfField() 
{
	//End of line
	if (EndOfLine())
	{
		return true;
	}
	else
	{
		bool status_ = read() == CSV_FILE_DELIMITER;
		seekSet(-1);
		return status_;
	}
}

// Always return TRUE
bool CSVFile::BeginOfField() 
{
	//Begin of file;
	if (! seekSet(-1))
	{
		return true;
	}

	int chVal = read();

	while (chVal != END_OF_LINE && chVal != CSV_FILE_DELIMITER)
	{
		//Begin of file
		if (!seekSet(-2))
		{
			seekSet(-1);
			return true;
		}
		chVal = read();
	}

	return true;
}

bool CSVFile::Field(byte num) {
	if (num == numField)
	{
		BeginOfField();
		return true;
	}
	else if (num < numField)
	{
		BeginOfLine();
	}
	
	while (num != numField)
	{
		if (!nextField())
		{
			return false;
		}
	}
	return true;
}

// Return number of reading bytes
// Return 0 when end reading current field
// Auto-safe. If reading bytes is less then @bufferSize
// then after last reading bytes is put null char.
byte CSVFile::readField(char * buffer_, byte bufferSize) 
{
	byte numReading = read(buffer_, bufferSize);
	char chVal = 0;
	byte correctBytes = 0;
	
	for (correctBytes=0; correctBytes < numReading; ++correctBytes)
	{
		chVal = buffer_[correctBytes];
		if (chVal == File_END_OF_LINE || chVal == CSV_FILE_DELIMITER)
		{
			// If read over end of field or end of line then we should
			// seek before first delimiter or first end of line
			// -numReading is state before reading
			//  correctBytes is number of bytes to first delimiter or first end of line
			//    (without this char)
			// Sum -numReading and correctBytes is position before delimiter
			seekSet(-numReading + correctBytes);
			break;
		}
	}
	
	// If read file to end or if not read full single field
	// then position in file is correctBytes.
	
	// Special case: last reading bytes is first half of non-ASCII character
	if (correctBytes == numReading && chVal < 0 && buffer_[correctBytes - 2] > 0) //correctBytes - 1 is index of chVal
	{
		correctBytes -= 1;
		seekSet(-1);
	}
	
	if (correctBytes < bufferSize)
		buffer_[correctBytes] = NULL_CHAR;
	
	return correctBytes;
}

//Buffer size should be equals or greater then decimal place
//reading value.
//Return FALSE if not read field or field is empty.
//Number can have a sign.
//Number can be preceded by whitespaces.
//Converting to number is end when is read first non-number char
//Warning! If reading buffer don't contain any number char
//then function return 0!
bool CSVFile::readField(int& value, char * buffer_, byte bufferSize) 
{
	int reading = readField(buffer_, bufferSize);
	if (reading > 0)
	{
		value = atoi(buffer_);
		return true;
	}
	else
	{
		return false;
	}
}

// If this field is end field in line (file)
// NOT go to next line and return FALSE
// When return true set pointer to position
// first char of file (after delimiter last field).
bool CSVFile::nextField() 
{
	int chVal = read();
	//End of file
	if (chVal == -1)
	{
		return false;
	}
	//Any content of field
	//Not end of file, not end of field, not end of line
	while (chVal >= 0 && chVal != CSV_FILE_DELIMITER && chVal != File_END_OF_LINE)
	{
		chVal = read();
	}
	//End of line
	if (chVal == File_END_OF_LINE)
	{
		seekSet(-1);
		return false;
	}
	
	numField += 1;
	
	return true;
}

#undef CSV_FILE_DELIMITER
#undef END_OF_LINE
#undef NULL_CHAR
#undef GOTO_BEGIN_STARTS_WITH
#undef CHECK_EMPTY_LINE