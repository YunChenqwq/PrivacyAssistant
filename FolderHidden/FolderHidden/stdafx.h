#include <iostream>   
#include <windows.h>  
#include <fstream>    
#include <stdlib.h> 
#include <sys/stat.h>
#include <direct.h>   
#include <conio.h>   
#include <cwchar>   
#include <sstream>
#ifndef _WIN32_WINNT  
#define _WIN32_WINNT 0x0601
#include<wincon.h>
#include<string>
#include<Windows.h>
#include<iostream>
using namespace std::basic_istream;
typedef struct _CONSOLE_FONT_INFOEX
{
    ULONG cbSize;
    DWORD nFont;
    COORD dwFontSize;
    UINT FontFamily;
    UINT FontWeight;
    WCHAR FaceName[LF_FACESIZE];
}CONSOLE_FONT_INFOEX, * PCONSOLE_FONT_INFOEX;

#ifdef __cplusplus
extern "C"
{
#endif
    BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput, BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX  lpConsoleCurrentFontEx);
#ifdef __cplusplus
}
#endif
#endif
using namespace std; 
