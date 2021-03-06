///////////////////////////////////////////////////////////////////////////////////////////////////
// Copyright 2015 Advanced Software Engineering Limited
//
// You may use and modify the code in this file in your application, provided the code and
// its modifications are used only in conjunction with ChartDirector. Usage of this software
// is subjected to the terms and condition of the ChartDirector license.
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "chartdir.h"
#include <tchar.h>

//
// Constants
//
#ifdef CD_NAMESPACE
namespace CD_NAMESPACE
{
#endif

	namespace Chart
	{
		//
		// Mouse usage mode constants
		//
		enum
		{
			MouseUsageDefault = 0,
			MouseUsageScroll = 1,
			MouseUsageZoomIn = 3,
			MouseUsageZoomOut = 4,
		};
	}

#ifdef CD_NAMESPACE
}
#endif

//
// Forward declarations
//
class CViewPortControl;

class UTF8toTCHAR
{
public:
	UTF8toTCHAR(const char* utf8_string) : t_string(0), needFree(false)
	{
		if (0 == utf8_string)
			t_string = 0;
		else if ((sizeof(TCHAR) == sizeof(char)) && isPureAscii(utf8_string))
			// No conversion needed for pure ASCII text
			t_string = (TCHAR*)utf8_string;
		else
		{
			// Either TCHAR = Unicode (2 bytes), or utf8_string contains non-ASCII characters.
			// Needs conversion
			needFree = true;

			// Convert to Unicode (2 bytes)
			int string_len = (int)strlen(utf8_string);
			wchar_t* buffer = new wchar_t[string_len + 1];
			MultiByteToWideChar(CP_UTF8, 0, utf8_string, -1, buffer, string_len + 1);
			buffer[string_len] = 0;

#ifdef _UNICODE
			t_string = buffer;
#else
			// TCHAR is MBCS - need to convert back to MBCS
			t_string = new char[string_len * 2 + 2];
			WideCharToMultiByte(CP_ACP, 0, buffer, -1, t_string, string_len * 2 + 1, 0, 0);
			t_string[string_len * 2 + 1] = 0;
			delete[] buffer;
#endif
		}

	}

	operator const TCHAR* ()
	{
		return t_string;
	}

	~UTF8toTCHAR()
	{
		if (needFree)
			delete[] t_string;
	}

private:
	TCHAR* t_string;
	bool needFree;

	//
	// helper utility to test if a string contains only ASCII characters
	//
	bool isPureAscii(const char* s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}

	//disable assignment
	UTF8toTCHAR(const UTF8toTCHAR& rhs);
	UTF8toTCHAR& operator=(const UTF8toTCHAR& rhs);
};

//
// Utility to convert from MFC TCHAR string to UTF8 string
//
class TCHARtoUTF8
{
public:
	TCHARtoUTF8(const TCHAR* t_string) : utf8_string(0), needFree(false)
	{
		if (0 == t_string)
			utf8_string = 0;
		else if (0 == *t_string)
			utf8_string == "";
		else if ((sizeof(TCHAR) == sizeof(char)) && isPureAscii((char*)t_string))
			// No conversion needed for pure ASCII text
			utf8_string = (char*)t_string;
		else
		{
			// TCHAR is non-ASCII. Needs conversion.

			needFree = true;
			int string_len = (int)_tcslen(t_string);

#ifndef _UNICODE
			// Convert to Unicode if not already in unicode.
			wchar_t* w_string = new wchar_t[string_len + 1];
			MultiByteToWideChar(CP_ACP, 0, t_string, -1, w_string, string_len + 1);
			w_string[string_len] = 0;
#else
			wchar_t* w_string = (wchar_t*)t_string;
#endif

			// Convert from Unicode (2 bytes) to UTF8
			utf8_string = new char[string_len * 3 + 1];
			WideCharToMultiByte(CP_UTF8, 0, w_string, -1, utf8_string, string_len * 3 + 1, 0, 0);
			utf8_string[string_len * 3] = 0;

			if (w_string != (wchar_t*)t_string)
				delete[] w_string;
		}

	}

	operator const char* ()
	{
		return utf8_string;
	}

	~TCHARtoUTF8()
	{
		if (needFree)
			delete[] utf8_string;
	}

private:
	char* utf8_string;
	bool needFree;

	//
	// helper utility to test if a string contains only ASCII characters
	//
	bool isPureAscii(const char* s)
	{
		while (*s != 0) { if (*(s++) & 0x80) return false; }
		return true;
	}

	//disable assignment
	TCHARtoUTF8(const TCHARtoUTF8& rhs);
	TCHARtoUTF8& operator=(const TCHARtoUTF8& rhs);
};

/////////////////////////////////////////////////////////////////////////////
// CRectCtrl window

//
// A rectangle with a background color. Use as thick edges for the selection
// rectangle.
//

/////////////////////////////////////////////////////////////////////////////
// CChartViewer window

//
// Event message ID
//
#define CVN_ViewPortChanged	1000			// View port has changed
#define CVN_MouseMoveChart 1001				// Mouse moves over the chart
#define CVN_MouseLeaveChart 1002			// Mouse leaves the chart
#define CVN_MouseMovePlotArea 1003			// Mouse moves over the extended plot area
#define CVN_MouseLeavePlotArea 1004			// Mouse leaves the extended plot area