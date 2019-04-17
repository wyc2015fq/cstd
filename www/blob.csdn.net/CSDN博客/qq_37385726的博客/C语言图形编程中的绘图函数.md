# C语言图形编程中的绘图函数~ - qq_37385726的博客 - CSDN博客





2017年02月01日 16:19:44[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：1942







一、屏幕和视口设置与清除函数

    36. setviewport() 设置图形视口函数

    37. clearviewport()清除图形视口函数

    38. cleardevice() 清除屏幕函数

    39. getviewsettings() 获取图形视口设置函数

二、调色板和颜色函数

    40. setpalette()设置调色板函数

    41. setallpalette()设置整个调色板函

    42. setbackcolor() 设置背景颜色函数

    43. setcolor() 设置颜色函数

    44. getbkcolor() 返回背景色函数

    45. getcolor() 返回当前绘图颜色

    46. getmaxcolor()返回最大颜色值函数

    47. getpalette() 获取调色板函数

    48. getpalettesize() 返回调色板大小函数

    49. getdefaultpalette() 获取缺省调色板函数

三、屏幕位置函数

    50. getmaxx()返回最大x坐标函数

    51. getmaxy() 返回最大y坐标函数

    52. getx() 返回x坐标函数

    53. gety() 返回y坐标函数

    54. moveto()函数

    55. moverel() 相对移动函数

----------------------------------------------------





   Borland C++提供了一整套综合性的图形函数，上节介绍了其中有关图形显示的函数，本节介绍其中的绘图函数，下节将介绍它的图形文本函数。

   一般绘图时，首先要清除屏幕，设置图形视口，设置绘图颜色，然后在屏幕上某个位置画点或直线或曲线等。下面给出具有这些功能的函数。



一、屏幕和视口设置与清除函数



36. setviewport() 设置图形视口函数

功能： 函数setviewport() 设置当前图形视口。

用法： 函数调用方式为void setviewport(int left,int top,int right,int bottom,int clip);

说明： 参数left,top是左上角坐标,right,bottom是右下角坐标，它们都是绝对屏幕坐标。

      传给setviewport()的第五个参数是clip，如果clip为1，则超出视口的输出图形自动被裁剪掉，即所有作图限制于当前图形视口之内，如果clip为0，则不做裁剪，即作图将无限制地扩展于视口周界之外，直到屏幕边界。若无效参数传入setviewport(),则graphresult()函数返回值-11，先前视口设置仍有效。

      setviewport()函数的对应头文件为 graphics.h

返回值： 无

例： 建立对角为(6,6)和(88,88)的视口，并带有裁剪功能：

   setviewport(6,6,88,88,1);



37. clearviewport()清除图形视口函数

功能： 函数clearviewport()清除当前图形视口，并把光标从当前位置移到原点(0,0)。

用法： 该函数调用方式为void clearviewport(void);

说明： 此函数调用执行后，当前图形视口将不复存在。

      这个函数对应的头文件是graphics.h

返回值： 无

例： 清除当前图形视口，但并非整个屏幕：

   clearviewport();



38. cleardevice() 清除屏幕函数

功能： 函数cleardevice() 清除整个屏幕，并且将当前位置移到屏幕原点。

用法： 此函数调用方式为void cleardevice(void);

说明： 该函数类似于文本模式下的函数clrscr()，但clrscr()函数不能在图形方式下工作；同样cleardevice()函数不能在文本模式下工作。

      这个函数相应的头文件为graphics.h

返回值： 无

例： 清除当前整个屏幕：

   cleardevice();



39. getviewsettings() 获取图形视口设置函数

功能： 函数getviewsettings()返回当前图形视口坐标和裁剪标志，坐标是绝对屏幕坐标，裁剪标志 1或 0。

用法： 此函数调用方式为void getviewsettings(struct viewporttype *info);

说明： 该函数执行后，把有关当前视口的信息装入info指向的viewporttype型结构中。

      viewporttype结构定义如下：

      struct viewporttype {

         int left,top,right,bottom;

         int clipflag;

      }

   这里left,top中存放视口左上角坐标，right,bottom中存放视口右下角坐标,clipflag中存放裁剪标志，若clipflag为1 ，执行裁剪以防止超出视口边界，如果clipflag为0，则不对超出边界输出作裁剪。

      getviewsettings()函数对应的头文件是graphics.h

返回值： 无

例： 显示当前视口角点坐标：

   struct viewporttype info;

   getviewsettings(&info);

   printf("left:%d top:%d\n",info.left,info.top);

   printf("right:%d bottom:%d\n",info.right,info.bottom);





二、调色板和颜色函数

进行图形显示或绘图，除了要知道用什么显示卡以及相应的图形驱动程序外，还必须了解它们支持什么样的调色板颜色。



40. setpalette()设置调色板函数

功能： 函数setpalette()可对调色板上的颜色的色彩值重新设定。

用法： setpalette()函数调用方式为void setpalette(int value,int color);

说明： 参数value为色彩值，color为色彩值相应的颜色名。当需要将一颜色指定为一色彩值地，即可调用此函数来设置。



表1-7  预先定义的调色板与色彩

-----------------------------------------------------------------

调色板         色彩0         色彩1         色彩2         色彩3

-----------------------------------------------------------------

  C0            黑色         淡绿           红浅          黄色

  C1            黑色         淡青           粉红          白色

  C2            黑色         绿色           红色          棕色

  C3            黑色         青色          品粉红        淡灰色

-----------------------------------------------------------------

这个函数相对应的头文件为graphics.h

返回值： 无



41. setallpalette()设置整个调色板函数

功能： 函数setallpalette()对某个调色板上的所有颜色重新设定彩色值。

用法： 函数调用方式为void setallpalette(struct palettetype *p);

说明： 在低分辨率图形显示模式下，该函数无效。若函数调用成功，则把p指向的palettetype结构中的色彩值设置为当前调色板所有颜色的新色彩值。

   palettetype结构定义如下：

   struct palettetype {

      unsigned char size;

      signed char color[16];

   };

这里必须把size设置为调色板颜色的数目，并把每个色彩值都装入color数组相应元素中，然后才可用于调用函数。

      此函数对应的头文件是graphics.h

返回值： 无

例：把16色的EGA/VGA显示卡调色板改为其前16种颜色：

   struct palettetype;

   int i;

   for(i=0;i<16;i++) p.color[i]=i;

   p.size=16;

   setallpalette(&p);



42. setbackcolor() 设置背景颜色函数

功能： 函数setbackcolor()把当前调色板颜色的第一个值改成指定的颜色值，以设置背景颜色。

用法： 此函数调用方式为void setbackcolor(int color);

说明： 参数color为指定的背景色，其有效值可以是颜色符号名，也可以是色彩值。表1-8列出了背景颜色供选取。



表1-8 背景颜色值

-----------------------------------------------------------------

   颜色符号名         色彩值      颜色符号名         色彩值

-----------------------------------------------------------------

   BLACK(黑色)      








四、图形和图像函数


(一) 像素函数


56. putpiel() 画像素点函数


57. getpixel()返回像素色函数


(二) 直线和线型函数


58. line() 画线函数


59. lineto() 画线函数


60. linerel() 相对画线函数


61. setlinestyle() 设置线型函数


62. getlinesettings() 获取线型设置函数


63. setwritemode() 设置画线模式函数


(三)、多边形函数


64. rectangle() 画矩形函数


65. bar() 画条函数


66. bar3d() 画条块函数


67. drawpoly() 画多边形函数


(四)、 圆、弧和曲线函数


68. getaspectratio()获取纵横比函数


69. circle()画圆函数


70. arc() 画圆弧函数


71. ellipse()画椭圆弧函数


72. fillellipse() 画椭圆区函数


73. pieslice() 画扇区函数


74. sector() 画椭圆扇区函数


75. getarccoords()获取圆弧坐标函数


(五)、 填充函数


76. setfillstyle() 设置填充图样和颜色函数


77. setfillpattern() 设置用户图样函数


78. floodfill() 填充闭域函数


79. fillpoly() 填充多边形函数


80. getfillsettings() 获取填充设置函数


81. getfillpattern() 获取用户图样设置函数


(六)、图像函数


82. imagesize() 图像存储大小函数


83. getimage() 保存图像函数


84. putimage() 输出图像函数








四、图形和图像函数


对许多图形应用程序，直线和曲线是非常有用的。但对有些图形只能靠操作单个像素才能画出。当然如果没有画像素的功能，就无法操作直线和曲线的函数。而且通过大规模使用像素功能，整个图形就可以保存、写、擦除和与屏幕上的原有图形进行叠加。


(一) 像素函数




56. putpixel() 画像素点函数


功能： 函数putpixel() 在图形模式下屏幕上画一个像素点。


用法： 函数调用方式为void putpixel(int x,int y,int color);


说明： 参数x,y为像素点的坐标，color是该像素点的颜色，它可以是颜色符号名，也可以是整型色彩值。


此函数相应的头文件是graphics.h


返回值： 无


例： 在屏幕上(6,8)处画一个红色像素点：


putpixel(6,8,RED);




57. getpixel()返回像素色函数


功能： 函数getpixel()返回像素点颜色值。


用法： 该函数调用方式为int getpixel(int x,int y);


说明： 参数x,y为像素点坐标。


函数的返回值可以不反映实际彩色值，这取决于调色板的设置情况(参见setpalette()函数)。


这个函数相应的头文件为graphics.h


返回值： 返回一个像素点色彩值。


例： 把屏幕上(8,6)点的像素颜色值赋给变量color。


color=getpixel(8,6);




(二) 直线和线型函数


有三个画直线的函数，即line(),lineto(),linerel()。这些直线使用整型坐标，并相对于当前图形视口，但不一定受视口限制，如果视口裁剪标志clip为真，那么直线将受






到视口边缘截断；如果clip为假，即使终点坐标或新的当前位置在图形视口或屏幕极限之外，直线截断到屏幕极限。


有两种线宽及几种线型可供选择，也可以自己定义线图样。下面分别介绍直线和线型函数。




58. line() 画线函数


功能： 函数line()使用当前绘图色、线型及线宽，在给定的两点间画一直线。


用法： 该函数调用方式为void line(int startx,int starty,int endx,int endy);


说明： 参数startx,starty为起点坐标,endx,endy为终点坐标，函数调用前后，图形状态下屏幕光标(一般不可见)当前位置不改变。


此函数相应的头文件为graphics.h


返回值： 无


例： 见函数60.linerel()中的实例。




59. lineto() 画线函数


功能： 函数lineto()使用当前绘图色、线型及线宽，从当前位置画一直线到指定位置。


用法： 此函数调用方式为void lineto(int x,int y);


说明： 参数x,y为指定点的坐标，函数调用后，当前位置改变到指定点(x,y)。


该函数对应的头文件为graphics.h


返回值： 无


例： 见函数60.linerel()中的实例。




60.linerel() 相对画线函数


功能： 函数linerel() 使用当前绘图色、线型及线宽，从当前位置开始，按指定的水平和垂直偏移距离画一直线。


用法： 这个函数调用方式为void linerel(int dx,int dy);


说明： 参数dx,dy分别是水平偏移距离和垂直偏移距离。


函数调用后，当前位置变为增加偏移距离后的位置，例如，原来的位置是(8,6)，调用函数linerel(10,18)后，当前位置为(18,24)。


返回值：无


例： 下面的程序为画线函数调用实例：


#include<graphics.h>


void main()


{


int driver,mode;


driver=DETECT;


mode=0;


initgraph(&driver,&mode,"");


setcolor(15);


line(66,66,88,88);


lineto(100,100);


linerel(36,64);


getch();


restorecrtmode();


}




61. setlinestyle() 设置线型函数


功能： setlinestyle() 为画线函数设置当前线型，包括线型、线图样和线宽。


用法： setlinestyle() 函数调用方式为void setlinestyle(int stly,unsigned pattern,int width);


说明： 参数style为线型取值，也可以用相应名称表示，如表1-10中所示。


参数pattern用于自定义线图样，它是16位(bit)字，只有当style=USERBIT_LINE(值为1)时，pattern的值才有意义，使用用户自定义线图样，与图样中“1”位对应的像素显示，因此，pattern=0xFFFF，则画实线；pattern=0x9999，则画每隔两个像素交替显示的虚线，如果要画长虚线，那么pattern的值可为0xFF00和0xF00F，当style不为USERBIT_LINE值时，虽然pattern的值不起作用，但扔须为它提供一个值，一般取为0。


参数wigth用来设定线宽，其取值见表1-11，表中给出了两个值，即1和3，实际上，线宽取值为2也是可以接受的。


若






用非法参数调用setlinestyle()函数，那么graphresult()会返回错误代码，并且当前线型继续有效。


Turbo C提供的线型与线宽定义在头文件graphics.h中，表1-10和1-11分别列出了参数的取值与含义。




表1-10 线型


-----------------------------------------------------


名 称 取 值 含 义


-----------------------------------------------------


SOLID_LINE 0 实线


DOTTED_LINE 1 点线


CNTER_LINE 2 中心线


DASHED_LINE 3 虚线


USERBIT_LINE 4 用户自定义线型


-----------------------------------------------------




表1-11 线宽


-----------------------------------------------------------


名 称 取 值 说 明


-----------------------------------------------------------


NORM_WIDTH(常宽) 1 一个像素宽(缺省值)


THICK_WIDTH(加宽) 3 三个像素宽


-----------------------------------------------------------





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




***graphics.h里的定义***




/******************************************************

 * EasyX Library for C++ (Ver:20100825)

 * http://www.easyx.cn

 *

 * graphics.h

 * 在 VC 下模拟 Borland BGI 绘图库，实现简单的绘图

 ******************************************************/



#pragma once



#ifndef WINVER

#define WINVER 0x0400// Specifies that the minimum required platform is Windows 95/NT4.

#endif



#ifndef _WIN32_WINNT

#define _WIN32_WINNT 0x0400// Specifies that the minimum required platform is Windows 95/NT4.

#endif



#ifndef _WIN32_WINDOWS

#define _WIN32_WINDOWS 0x0410// Specifies that the minimum required platform is Windows 98.

#endif



#if defined(_UNICODE) && (_MSC_VER > 1200)
#pragma comment(lib,"graphicswu.lib")

#elif !defined(_UNICODE) && (_MSC_VER > 1200)
#pragma comment(lib,"graphicsw.lib")

#elif defined(_UNICODE)
#pragma comment(lib,"graphicsu.lib")

#elif !defined(_UNICODE)
#pragma comment(lib,"graphics.lib")

#endif





#include "windows.h"

#include "tchar.h"



#define PI 3.1415926535



// 为了兼容 Borland C++ 3.1 而做的无意义定义

#define DETECT 0

#define VGA0

#define VGAHI 0



// 绘图环境初始化参数

#define SHOWCONSOLE1// 进入图形模式时，保留控制台的显示



// 颜色

#defineBLACK
0

#defineBLUE
0xA80000

#defineGREEN
0x00A800

#defineCYAN
0xA8A800

#defineRED
0x0000A8

#defineMAGENTA0xA800A8

#defineBROWN
0x0054A8

#defineLIGHTGRAY0xA8A8A8

#defineDARKGRAY0x545454

#defineLIGHTBLUE0xFC5454

#defineLIGHTGREEN0x54FC54

#defineLIGHTCYAN0xFCFC54

#defineLIGHTRED0x5454FC

#defineLIGHTMAGENTA0xFC54FC

#defineYELLOW0x54FCFC

#defineWHITE
0xFCFCFC



// 填充模式

#defineNULL_FILL1// 不填充

#defineSOLID_FILL2// 用指定颜色完全填充

#defineBDIAGONAL_FILL3// /// 填充 (普通一组)

#defineCROSS_FILL4// +++ 填充

#defineDIAGCROSS_FILL5// xxx 填充

#defineDOT_FILL6// ... 填充

#defineFDIAGONAL_FILL7// \\\ 填充

#defineHORIZONTAL_FILL8// --- 填充

#defineVERTICAL_FILL9// ||| 填充

#defineBDIAGONAL2_FILL10// /// 填充 (密集一组)

#defineCROSS2_FILL11// +++ 填充

#defineDIAGCROSS2_FILL12// xxx 填充

#defineDOT2_FILL13// ... 填充

#defineFDIAGONAL2_FILL14// \\\ 填充

#defineHORIZONTAL2_FILL15// --- 填充

#defineVERTICAL2_FILL16// ||| 填充

#defineBDIAGONAL3_FILL17// /// 填充 (粗线一组)

#defineCROSS3_FILL18// +++ 填充

#defineDIAGCROSS3_FILL19// xxx 填充

#defineDOT3_FILL20// ... 填充

#defineFDIAGONAL3_FILL21// \\\ 填充

#defineHORIZONTAL3_FILL22// --- 填充

#defineVERTICAL3_FILL23// ||| 填充

#defineINTERLEAVE_FILL24// xxx 填充 (十分密)

#definePATTERN_FILL25// 用指定图案填充







// 绘图模式相关函数



void initgraph(int Width, int Height, int Flag = NULL);// 初始化图形环境

void initgraph(int* gdriver, int* gmode, char* path);// 兼容 Borland C++ 3.1 的重载，默认 640x480@24bit

void closegraph();// 关闭图形环境







// 绘图环境设置



void cleardevice();// 清屏



void getviewport(int *pleft, int *ptop, int *pright, int *pbottom, bool *pclip = NULL);// 获取视图信息

void setviewport(int left, int top, int right, int bottom, bool clip = true);// 设置视图

void clearviewport();// 清空视图



void getlinestyle(int *plinestyle, WORD *pupattern = NULL, int *pthickness = NULL);// 获取当前线形

void setlinestyle(int linestyle, WORD upattern = NULL, int thickness = 1);// 设置当前线形

void getfillstyle(COLORREF *pcolor, int *ppattern = NULL, char *pupattern = NULL);// 获取填充类型

void setfillstyle(COLORREF color, int pattern = SOLID_FILL, const char *pupattern = NULL);// 设置填充类型



void getaspectratio(float *pxasp, float *pyasp);
// 获取当前缩放因子

void setaspectratio(float xasp = 1, float yasp = 1);// 设置当前缩放因子



void setwritemode(int mode);// 设置绘图位操作模式



void graphdefaults();// 重置所有绘图设置为默认值



COLORREF getcolor();// 获取当前绘图前景色

void setcolor(COLORREF color);// 设置当前绘图前景色

COLORREF getbkcolor();// 获取当前绘图背景色

void setbkcolor(COLORREF color);// 设置当前绘图背景色

void setbkmode(int iBkMode);// 设置背景混合模式



// 颜色模型转换函数

COLORREF RGBtoGRAY(COLORREF rgb);

void RGBtoHSL(COLORREF rgb, float *H, float *S, float *L);

void RGBtoHSV(COLORREF rgb, float *H, float *S, float *V);

COLORREF HSLtoRGB(float H, float S, float L);

COLORREF HSVtoRGB(float H, float S, float V);





// 绘图函数



COLORREF getpixel(int x, int y);// 获取点的颜色

void putpixel(int x, int y, COLORREF color);
// 画点



void moveto(int x, int y);// 移动当前点(绝对坐标)

void moverel(int dx, int dy);// 移动当前点(相对坐标)



void line(int x1, int y1, int x2, int y2);// 画线

void linerel(int dx, int dy);// 画线(至相对坐标)

void lineto(int x, int y);// 画线(至绝对坐标)



void rectangle(int left, int top, int right, int bottom);// 画矩形



void getarccoords(int *px, int *py, int *pxstart, int *pystart, int *pxend, int *pyend);// 获取圆弧坐标信息

void arc(int x, int y, int stangle, int endangle, int radius);// 画圆弧

void circle(int x, int y, int radius);// 画圆

void pieslice(int x, int y, int stangle, int endangle, int radius);// 画填充圆扇形

void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);// 画椭圆弧线

void fillellipse(int x, int y, int xradius, int yradius);// 画填充椭圆

void sector(int x, int y, int stangle, int endangle, int xradius, int yradius);// 画填充椭圆扇形



void bar(int left, int top, int right, int bottom);// 画无边框填充矩形

void bar3d(int left, int top, int right, int bottom, int depth, bool topflag);// 画有边框三维填充矩形



void drawpoly(int numpoints, const int *polypoints);// 画多边形

void fillpoly(int numpoints, const int *polypoints);// 画填充的多边形

void floodfill(int x, int y, int border);// 填充区域







// 文字相关函数



void outtext(LPCTSTR textstring);// 在当前位置输出文字

void outtext(TCHAR c);// 在当前位置输出字符

void outtextxy(int x, int y, LPCTSTR textstring);
// 在指定位置输出文字

void outtextxy(int x, int y, TCHAR c);// 在当前位置输出字符

int textwidth(LPCTSTR textstring);// 获取字符串占用的像素宽

int textheight(LPCTSTR textstring);// 获取字符串占用的像素高



// 设置当前字体样式(详见帮助)

//nHeight: 字符的平均高度；

//nWidth: 字符的平均宽度(0 表示自适应)；

//lpszFace: 字体名称；

//nEscapement: 字符串的书写角度(单位 0.1 度)；

//nOrientation: 每个字符的书写角度(单位 0.1 度)；

//nWeight: 字符的笔画粗细(0 表示默认粗细)；

//bItalic: 是否斜体；

//bUnderline: 是否下划线；

//bStrikeOut: 是否删除线；

//fbCharSet: 指定字符集；

//fbOutPrecision: 指定文字的输出精度；

//fbClipPrecision: 指定文字的剪辑精度；

//fbQuality: 指定文字的输出质量；

//fbPitchAndFamily: 指定以常规方式描述字体的字体系列。

void setfont(int nHeight, int nWidth, LPCTSTR lpszFace);

void setfont(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut);

void setfont(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut, BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality, BYTE fbPitchAndFamily);

void setfont(const LOGFONT *font);// 设置当前字体样式

void getfont(LOGFONT *font);// 获取当前字体样式







// 图像处理函数



// 定义图像对象

class IMAGE

{

private:
HBITMAP m_hBmp;
HDC m_hDC;
int m_width, m_height;

public:
IMAGE();
IMAGE(IMAGE &img);// 拷贝构造函数
IMAGE& operator = (IMAGE &img);// 赋值运算符重载函数
~IMAGE();
void getimage(int srcX, int srcY, int srcWidth, int srcHeight);
void getimage(LPCTSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);
void getimage(LPCTSTR pResType, LPCTSTR pResName, int zoomWidth = 0, int zoomHeight = 0);
void getimage(const IMAGE *pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);
void putimage(int dstX, int dstY, DWORD dwRop = SRCCOPY) const;
void putimage(int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;
void putimage(IMAGE *pDstImg, int dstX, int dstY, DWORD dwRop = SRCCOPY) const;
void putimage(IMAGE *pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, int srcX, int srcY, DWORD dwRop = SRCCOPY) const;

};



void getimage(IMAGE *pDstImg, int srcX, int srcY, int srcWidth, int srcHeight);// 从屏幕获取图像

void getimage(IMAGE *pDstImg, LPCTSTR pImgFile, int zoomWidth = 0, int zoomHeight = 0);// 从图片文件获取图像(bmp/jpg/gif/emf/wmf)

void getimage(IMAGE *pDstImg, LPCTSTR pResType, LPCTSTR pResName, int zoomWidth = 0, int zoomHeight = 0);// 从资源文件获取图像(bmp/jpg/gif/emf/wmf)

void getimage(IMAGE *pDstImg, const IMAGE *pSrcImg, int srcX, int srcY, int srcWidth, int srcHeight);// 从另一个 IMAGE 对象中获取图像

void putimage(int dstX, int dstY, const IMAGE *pSrcImg, DWORD dwRop = SRCCOPY);// 绘制图像到屏幕

void putimage(int dstX, int dstY, int dstWidth, int dstHeight, const IMAGE *pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);// 绘制图像到屏幕(指定宽高)

void putimage(IMAGE *pDstImg, int dstX, int dstY, const IMAGE *pSrcImg, DWORD dwRop = SRCCOPY);// 绘制图像到另一图像中

void putimage(IMAGE *pDstImg, int dstX, int dstY, int dstWidth, int dstHeight, const IMAGE *pSrcImg, int srcX, int srcY, DWORD dwRop = SRCCOPY);// 绘制图像到另一图像中(指定宽高)







// 其它函数



intgetwidth();// 获取绘图区宽度

intgetheight();// 获取绘图区高度

intgetx();
// 获取当前 x 坐标

intgety();
// 获取当前 y 坐标



void BeginBatchDraw();// 开始批量绘制

void FlushBatchDraw();// 执行未完成的绘制任务

void FlushBatchDraw(int left, int top, int right, int bottom);// 执行指定区域内未完成的绘制任务

void EndBatchDraw();// 结束批量绘制，并执行未完成的绘制任务

void EndBatchDraw(int left, int top, int right, int bottom);// 结束批量绘制，并执行指定区域内未完成的绘制任务



HWND GetHWnd();// 获取绘图窗口句柄

int GetGraphicsVer();// 获取当前版本







// 鼠标消息

// 支持如下消息：

//WM_MOUSEMOVE鼠标移动

//WM_MOUSEWHEEL鼠标滚轮拨动

//WM_LBUTTONDOWN左键按下

//WM_LBUTTONUP左键弹起

//WM_LBUTTONDBLCLK左键双击

//WM_MBUTTONDOWN中键按下

//WM_MBUTTONUP中键弹起

//WM_MBUTTONDBLCLK中键双击

//WM_RBUTTONDOWN右键按下

//WM_RBUTTONUP右键弹起

//WM_RBUTTONDBLCLK右键双击

struct MOUSEMSG

{
UINT uMsg;
// 当前鼠标消息
bool mkCtrl;
// Ctrl 键是否按下
bool mkShift;
// Shift 键是否按下
bool mkLButton;// 鼠标左键是否按下
bool mkMButton;// 鼠标中键是否按下
bool mkRButton;// 鼠标右键是否按下
short x;
// 当前鼠标 x 坐标
short y;
// 当前鼠标 y 坐标
short wheel;
// 鼠标滚轮滚动值(120的倍数)

};



bool MouseHit();// 检查是否存在鼠标消息

MOUSEMSG GetMouseMsg();// 获取一个鼠标消息。如果没有，就等待

void FlushMouseMsgBuffer();// 清空鼠标消息缓冲区








