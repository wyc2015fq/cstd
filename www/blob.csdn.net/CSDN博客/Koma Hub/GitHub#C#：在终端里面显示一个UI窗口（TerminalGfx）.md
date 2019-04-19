# GitHub#C#：在终端里面显示一个UI窗口（TerminalGfx） - Koma Hub - CSDN博客
2018年06月11日 13:01:07[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：184

# TerminalGfx
C header file that provides a few functions and colors for creating a basic UI in a terminal.
[GitHub: TerminalGfx][https://github.com/MihaiChirculete/TerminalGfx](https://github.com/MihaiChirculete/TerminalGfx)
## [](https://github.com/rtoax/ComputerNetwork/blob/master/test.md#%E6%96%87%E4%BB%B6%E6%A0%91)文件树
```
$ tree
.
├── README.md
├── termgfx.h
├── termgfx_MANUAL.txt
├── TerminalGfx.md
└── test.c
```
## [](https://github.com/rtoax/ComputerNetwork/blob/master/test.md#termgfxh%E6%96%87%E4%BB%B6%E5%86%85%E5%AE%B9%E5%A6%82%E4%B8%8B)`termgfx.h`文件内容如下：
/*	Header file that allows:		--> drawing certain UI elements within the terminal such as colored boxes		--> moving the cursor within the terminal		--> getting the width and height of the terminal in Lines and Columns		--> clearing the screen	Author: Chirculete Vlad Mihai	Date (created): 12.10.2016	Date (last edit): 19.11.2016*/
#include<stdio.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<string.h>// text colors
#defineNRM"\x1B[0m"
#defineBLK"\x1B[30m"
#defineRED"\x1B[31m"
#defineGRN"\x1B[32m"
#defineYEL"\x1B[33m"
#defineBLU"\x1B[34m"
#defineMAG"\x1B[35m"
#defineCYN"\x1B[36m"
#defineWHT"\x1B[37m"// background colors
#defineBGBLK"\x1B[40m"
#defineBGRED"\x1B[41m"
#defineBGGRN"\x1B[42m"
#defineBGYEL"\x1B[43m"
#defineBGBLU"\x1B[44m"
#defineBGMAG"\x1B[45m"
#defineBGCYN"\x1B[46m"
#defineBGWHT"\x1B[47m"// other text attributes
#defineBOLD_ON"\x1b[1m"// Bold on(enable foreground intensity)
#defineUNDERLINE_ON"\x1b[4m"// Underline on
#defineBLINK_ON"\x1b[5m"// Blink on(enable background intensity)
#defineBOLD_OFF"\x1b[21m"// Bold off(disable foreground intensity)
#defineUNDERLINE_OFF"\x1b[24m"//	Underline off
#defineBLINK_OFF"\x1b[25m"// Blink off(disable background intensity)typedefstruct window window;
struct window
{
	int id;				// the id of the windowchar *title;		// the title of the windowchar *titleColor;	// color of the window's titleint x, y;			// x and y coordinates of the top left corner of the windowint width, height;	// the width and the height of the windowint drawBorder;		// 1 or 0, enables/disables drawing of the border and titlechar *borderColor;	// color of the window's borderint drawBackground;		// 1 or 0, enables/disables drawing of the window background char *backgroundColor;	// background color of the box
};
// sets the window's title and enables border drawingvoidsetWindowTitle(window *w, char *title, char *title_color)
{
	(*w).title = title;
	(*w).titleColor = title_color;
	(*w).drawBorder = 1;
}
// sets the window's border color and enables border drawingvoidsetWindowBorderColor(window *w, char *color)
{
	(*w).borderColor = color;
	(*w).drawBorder = 1;
}
// moves the cursor to X and Y in terminalvoidgotoxy(int x,int y)
{
 	printf("%c[%d;%df",0x1B,y,x);
}
// get the terminal height in linesintgetTermHeight()
{
	// get the terminal height in linesint lines = 0;
	#ifdef TIOCGSIZE
    	struct ttysize ts;
    	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    	lines = ts.ts_lines;
	#elif defined(TIOCGWINSZ)
	    struct winsize ts;
	    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	    lines = ts.ws_row;
	#endif/* TIOCGSIZE */return lines;
}
intgetTermWidth()
{
	// get the terminal width in columnsint cols = 0;
	#ifdef TIOCGSIZE
    	struct ttysize ts;
    	ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
    	cols = ts.ts_cols;
	#elif defined(TIOCGWINSZ)
	    struct winsize ts;
	    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	    cols = ts.ws_col;
	#endif/* TIOCGSIZE */return cols;
}
// draws a filled bar and then moves the cursor to the top of the terminalvoiddrawBar(int x, int y, int width, int height, char *color)
{
	width--;
	height--;
	int i, j=x;
	gotoxy(x, y);
	for(i=y; i<=y+height; i++)
	{
		gotoxy(x, i);
		j=x;
		while(j<=x+width)
		{
			printf("%s", color);	j++;
		}
	}
	printf("%s", NRM);
	gotoxy(0, 0);
}
// draws a box (different from a bar since it has no filling)  then moves the cursor to the top of the terminalvoiddrawBox(int x, int y, int width, int height, char *color)
{
	width--;
	height--;
	int i, j=x;
	gotoxy(x, y);
	for(i=y; i<=y+height; i++)
	{
		gotoxy(x, i);
		j=x;
		while(j<=x+width)
		{
			if(j==x || j==x+width || i==y || i == y+height) printf("%s", color);	
			elseprintf("%s", NRM);
			j++;
		}
		//printf("%s\n", NRM);
	}
	printf("%s", NRM);
	gotoxy(0, 0);
}
// prints text at X and Y then moves the cursor to the top of the terminalvoidtextXY(char* text, int x, int y)
{
	gotoxy(x, y);
	printf("%s", text);
	gotoxy(0, 0);
}
// prints colored text at X and Y then moves the cursor to the top of the terminalvoidtextXYcolor(char* text, int x, int y, char* text_color, char* text_bg)
{
	gotoxy(x, y);
	printf("%s%s%s%s", text_color, text_bg, text, NRM);
	gotoxy(0, 0);
}
// sets the current text color to TEXT_COLORvoidsetTextColor(char* text_color)
{
	printf("%s", text_color);
}
// sets the current text background to TEXT_BGvoidsetTextBackground(char* text_bg)
{
	printf("%s", text_bg);
}
// sets text attributesvoidsetTextAttr(char* text_attr)
{
	printf("%s", text_attr);
}
// clear the screenvoidclearscr()
{
	system("clear");
}
// draw a given windowvoiddrawWindow(window w)
{
	int titlePosition;	// position on X axis where to start drawing the text
	titlePosition = (w.width / 2) - (strlen(w.title) / 2);
	// if border drawing is set to true (1), draw the border and the titleif(w.drawBorder)
	{
		drawBox(w.x, w.y, w.width, w.height, w.borderColor);
		// set the bold and underline attributes to ON for title then switch it back offsetTextAttr(BOLD_ON);	setTextAttr(UNDERLINE_ON);
		textXYcolor(w.title, w.x + titlePosition, w.y, w.titleColor, w.borderColor);
		setTextAttr(BOLD_OFF);	setTextAttr(UNDERLINE_OFF);
	}
	// if background drawing is set to true (1), draw the backgroundif(w.drawBackground)
	{
		drawBar(w.x+1, w.y+1, w.width-1, w.height-1, w.backgroundColor);
	}
}
// draw a given array of windowsvoiddrawWindows(window windows[], int n)
{
	int i;
	for(i=0; i<n; i++)
		drawWindow(windows[i]);
}
## [](https://github.com/rtoax/ComputerNetwork/blob/master/test.md#termgfx_manualtxt%E5%86%85%E5%AE%B9%E5%A6%82%E4%B8%8B)`termgfx_MANUAL.txt`内容如下：
```
Terminal graphics header file 
Author: Chirculete Vlad Mihai
Date: 12.10.2016
	Functions provided by termgfx.h:
		--> clearscr()
			Clears the screen.
		--> gotoxy(int x, int y)
			Moves the cursor inside the terminal at position given by x and y.
		--> getTermHeight()
			Returns an integer which represents the height of the terminal measured in columns.
		--> getTermWidth()
			Returns an integer which represents the width of the terminal measured in lines.
		--> drawBar(int x, int y, int WIDTH, int HEIGHT, char *FILL_COLOR)
			Draws a filled bar at x and y that is WIDTH wide and HEIGHT tall. The filling color is determined by FILL_COLOR.
			For the variable FILL_COLOR you should use one of the background colors defined. (see the list of availible colors below)
		--> drawBox(int x, int y, int WIDTH, int HEIGHT, char *COLOR)
			Draws a box that unlike the bar it isn't filled.
		--> textXY(char *TEXT, int x, int y)
			Prints TEXT at the given x and y position inside the terminal.
		--> textXYcolor(char *TEXT, int x, int y, char *TEXT_COLOR , char *TEXT_BACKGROUND)
			Prints TEXT at the given x and y position inside the terminal.
			The color of the text is given by TEXT_COLOR, and it's background color by TEXT_BACKGROUND.
			You should use the colors defined in the list below.
	Colors provided by termgfx.h:
		Foreground colors (text colors)
		NRM  - normal (default terminal color)
		RED
		GRN  - green
		YEL  - yellow
		BLU  - blue
		MAG  - magenta
		CYN  - cyan
		WHT  - white
		Background colors (for text background and boxes)
		BGRED   - background red
		BGGRN	- background green
		BGYEL	- background yellow
		BGBLU	- background blue
		BGMAG	- background magenta
		BGCYN	- background cyan
		BGWHT	- background white
```
## [](https://github.com/rtoax/ComputerNetwork/blob/master/test.md#%E4%B8%80%E4%B8%AA%E7%AE%80%E5%8D%95%E7%9A%84%E6%B5%8B%E8%AF%95testc)一个简单的测试：`test.c`
#include<stdlib.h>
#include<stdio.h>
#include"termgfx.h"voiddrawBar();
intmain(int argc, char **argv)
{
	system("clear");
	window w[10];
	// window 0
	w[0].title = "Window 0";
	w[0].drawBorder = 1;
	w[0].titleColor = WHT;
	w[0].borderColor = BGBLU;
	w[0].drawBackground = 0;
	w[0].backgroundColor = BGWHT;
	w[0].x = 0;
	w[0].y = 5;
	w[0].width = (getTermWidth()/2)-1;
	w[0].height = getTermHeight()-3;
	// window 1
	w[1].title = "Window 1";
	w[1].drawBorder = 1;
	w[1].titleColor = YEL;
	w[1].borderColor = BGGRN;
	w[1].drawBackground = 1;
	w[1].backgroundColor = BGWHT;
	w[1].x = getTermWidth()/2;
	w[1].y = 5;
	w[1].width = getTermWidth()/3;
	w[1].height = getTermHeight()/3;
	// window 2
	w[2].title = "Window 2";
	w[2].drawBorder = 1;
	w[2].titleColor = GRN;
	w[2].borderColor = BGYEL;
	w[2].drawBackground = 0;
	w[2].backgroundColor = BGWHT;
	w[2].x = getTermWidth()/2;
	w[2].y = getTermHeight()/2;
	w[2].width = getTermWidth()/2;
	w[2].height = getTermHeight()/2;
	drawWindows(w, 3);
	return0;
}
## [](https://github.com/rtoax/ComputerNetwork/blob/master/test.md#%E7%BB%93%E6%9E%9C%E5%9B%BE%E5%A6%82%E4%B8%8B)结果图如下：
![](https://img-blog.csdn.net/20180611130009266)
