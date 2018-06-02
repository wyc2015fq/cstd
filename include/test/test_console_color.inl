#include <windows.h>
#include <stdio.h>
void ConPrint(char* CharBuffer, int len);
void ConPrintAt(int x, int y, char* CharBuffer, int len);
void gotoXY(int x, int y);
void ClearConsole(void);
void ClearConsoleToColors(int ForgC, int BackC);
void SetColorAndBackground(int ForgC, int BackC);
void SetColor(int ForgC);
void HideTheCursor(void);
void ShowTheCursor(void);
int main(int argc, char* argv[])
{
  HideTheCursor();
  ClearConsoleToColors(15, 1);
  ClearConsole();
  gotoXY(1, 1);
  SetColor(14);
  printf("This is a test...\n");
  Sleep(5000);
  ShowTheCursor();
  SetColorAndBackground(15, 12);
  ConPrint("This is also a test...\n", 23);
  SetColorAndBackground(1, 7);
  ConPrintAt(22, 15, "This is also a test...\n", 23);
  gotoXY(0, 24);
  SetColorAndBackground(7, 1);
  return 0;
}
//This will clear the console while setting the forground and
//background colors.
void ClearConsoleToColors(int ForgC, int BackC)
{
  WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
  //Get the handle to the current output buffer...
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  //This is used to reset the carat/cursor to the top left.
  COORD coord = {0, 0};
  //A return value... indicating how many chars were written
  //not used but we need to capture this since it will be
  //written anyway (passing NULL causes an access violation).
  DWORD count;
  //This is a structure containing all of the console info
  // it is used here to find the size of the console.
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  //Here we will set the current color
  SetConsoleTextAttribute(hStdOut, wColor);
  if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
    //This fills the buffer with a given character (in this case 32=space).
    FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    //This will set our cursor position for the next print statement.
    SetConsoleCursorPosition(hStdOut, coord);
  }
}
//This will clear the console.
void ClearConsole()
{
  //Get the handle to the current output buffer...
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  //This is used to reset the carat/cursor to the top left.
  COORD coord = {0, 0};
  //A return value... indicating how many chars were written
  //   not used but we need to capture this since it will be
  //   written anyway (passing NULL causes an access violation).
  DWORD count;
  //This is a structure containing all of the console info
  // it is used here to find the size of the console.
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  //Here we will set the current color
  if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
    //This fills the buffer with a given character (in this case 32=space).
    FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    //This will set our cursor position for the next print statement.
    SetConsoleCursorPosition(hStdOut, coord);
  }
}
//This will set the position of the cursor
void gotoXY(int x, int y)
{
  //Initialize the coordinates
  COORD coord = {x, y};
  //Set the position
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//This will set the forground color for printing in a console window.
void SetColor(int ForgC)
{
  WORD wColor;
  //We will need this handle to get the current background attribute
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  //We use csbi for the wAttributes word.
  if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
    //Mask out all but the background attribute, and add in the forgournd color
    wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
    SetConsoleTextAttribute(hStdOut, wColor);
  }
}
//This will set the forground and background color for printing in a console window.
void SetColorAndBackground(int ForgC, int BackC)
{
  WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}
//Direct console output
void ConPrint(char* CharBuffer, int len)
{
  DWORD count;
  WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), CharBuffer, len, &count, NULL);
}
//Direct Console output at a particular coordinate.
void ConPrintAt(int x, int y, char* CharBuffer, int len)
{
  DWORD count;
  COORD coord = {x, y};
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleCursorPosition(hStdOut, coord);
  WriteConsole(hStdOut, CharBuffer, len, &count, NULL);
}
//Hides the console cursor
void HideTheCursor()
{
  CONSOLE_CURSOR_INFO cciCursor;
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (GetConsoleCursorInfo(hStdOut, &cciCursor)) {
    cciCursor.bVisible = FALSE;
    SetConsoleCursorInfo(hStdOut, &cciCursor);
  }
}
//Shows the console cursor
void ShowTheCursor()
{
  CONSOLE_CURSOR_INFO cciCursor;
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (GetConsoleCursorInfo(hStdOut, &cciCursor)) {
    cciCursor.bVisible = TRUE;
    SetConsoleCursorInfo(hStdOut, &cciCursor);
  }
}
VC6.0中包含了头文件conio.h 怎么用gotoxy()等函数？(2014 - 05 - 11 15: 03: 55)
转载
▼
标签： 转载   分类： c / cpp
原文地址：VC6.0中包含了头文件conio.h 怎么用gotoxy()等函数？作者：Jerry
很多DOS底下的东西在VC中都没了，所以有的TC可以运行，但vc不行。不过可以自己写一个函数；（很多可以用msdn查到）头文件加上#include<windows.h>
void gotoxy(int x, int y) //cursor an gewuenschte position auf dem bildschirm setzen
{
  CONSOLE_SCREEN_BUFFER_INFO   csbiInfo;                            //variablendklaration
  HANDLE   hConsoleOut;
  hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
  csbiInfo.dwCursorPosition.X = x;                                    //cursorposition X koordinate festlegen
  csbiInfo.dwCursorPosition.Y = y;                                    //cursorposition Y koordinate festlegen
  SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);   //den cursor an die festgelegte koordinate setzen
}
void clrscr(void)  //clearscreen: gesamten Bildschirm leeren
{
  CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;                            //variablendklaration
  HANDLE    hConsoleOut;
  COORD    Home = {0, 0};
  DWORD    dummy;
  hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
  FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy); //bis cursorposition leerzeichen ausgeben
  csbiInfo.dwCursorPosition.X = 0;                                    //cursorposition X koordinate festlegen
  csbiInfo.dwCursorPosition.Y = 0;                                    //cursorposition Y koordinate festlegen
  SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);   //den cursor an die festgelegte koordinate setzen
}
void clreol(void)  //clear end of line: den rest der Zeile nach dem cursor l鰏chen
{
  CONSOLE_SCREEN_BUFFER_INFO        csbiInfo;                            //variablendklaration
  HANDLE    hConsoleOut;
  COORD    Home, pos;
  DWORD    dummy;
  hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
  Home = csbiInfo.dwCursorPosition;
  pos.X = 80 - csbiInfo.dwCursorPosition.X;
  FillConsoleOutputCharacter(hConsoleOut, ' ', pos.X, Home, &dummy);
}
void clreoscr(void)  //clear end of screen: alles nach dem cursor l鰏chen
{
  CONSOLE_SCREEN_BUFFER_INFO        csbiInfo;                            //variablendklaration
  HANDLE    hConsoleOut;
  COORD    Home;
  DWORD    dummy;
  hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
  GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
  Home = csbiInfo.dwCursorPosition;
  FillConsoleOutputCharacter(hConsoleOut, ' ', csbiInfo.dwSize.X * csbiInfo.dwSize.Y, Home, &dummy);
}

