# C语言图形函数代码~持续更新中 - qq_37385726的博客 - CSDN博客





2017年02月01日 23:30:56[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：528








    下面总结的是一些C语言图形函数代码~持续更新中






**画三类圆**






```
#include
#include
#include
#include
#include
int main(void)
{
	initgraph(640,480);
	setfillcolor(YELLOW);
	solidcircle(100,100,50);
	fillcircle(200,200,50);
	circle(300,300,50);//circle(&x,&y,&r) 以（300，300）为圆心，以50为半径
	Sleep(1000);
	cleardevice();//清屏
	Sleep(1000);
	closegraph();//关闭图形界面
	return 0;
}
```





**画长方体**






```
#include
#include
#include
#include
#include
int main(void)
{
	int gdriver = DETECT;//DETECE是定义在graphic.h里的宏，值为0  //gdiver代表的是图形驱动器（graphics driver）
	int gmode;//gmode代表的是图形模式（graphics mode）
	int midx, midy;//midx=x轴的中值,midy=y轴的中值
	initgraph(&gdriver, &gmode, " ");//initgraph(&形驱，&形模，" ")函数用于初始化图形系统,或一般为initgraph(640,480)
	midx = getwidth() / 2;
	midy = getheight() / 2;
	setfillstyle(DIAGCROSS_FILL);
	setfillcolor(YELLOW);//填充颜色
	setcolor(YELLOW);//外框颜色
	bar3d(midx - 50, midy - 50, midx + 50, midy + 50, 50, 50);
	_getch();
	closegraph();
	return 0;
}
```



**关于三类画线函数**






```
#include
#include
#include
#include
#include

int main(void)
{
	initgraph(640, 480);
	linerel(175, 200);//画线函数，从当前点到相对坐标
	line(175, 200, 450, 450);//划线函数，从左~上点到右~下点
	                         //它不会改变光标的位置，也就是说光标现在还在（175，200）
	moveto(450, 450);//用于将光标移动到x,y
	lineto(0, 0);//划线函数，从当前点到绝对坐标
	_getch();
	return 0;
}
```

**画长方形**









```
#include
#include
#include
#include
#include

int main(void)
{
	initgraph(640, 480);
	setfillcolor(YELLOW);
	solidrectangle(100, 100, 300, 200);//是无边框可填充矩形
	fillrectangle(300, 200, 450, 300);//是有边框可填充矩形
	rectangle(450, 300, 550, 400);//是有边框不可填充矩形
	_getch();
	return 0;
}
```

***画椭圆***




```
#include
#include
#include
#include
#include

int main(void)
{
	initgraph(640, 480);
	setfillcolor(YELLOW);
	solidellipse(50,50,150,100);//左 上  右  椭圆就形成在这个矩形里
	fillellipse(150, 100, 250, 150);
	ellipse(250, 150, 350, 200);
	_getch();
	return 0;
}
```





**画圆角矩形**






```
#include
#include
#include
#include
#include

int main(void)
{
	initgraph(640, 480);
	setfillcolor(YELLOW);
	solidroundrect(50,50,150,100,10,10);
	fillroundrect(170, 120, 270, 170,10,10);
	roundrect(290, 190, 370, 220,10,10);// (int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight);	
	_getch();
	return 0;
}
```




**画扇形**






```
#include
#include
#include
#include
#include
#define N 3.14159266
int main(void)
{
	initgraph(640, 480);
	setfillcolor(YELLOW);
//	solidpie(90, 190, 370, 270,0,N/2);
//	fillpie(90, 190, 370, 270,0,N/2);
	pie(290, 190, 370, 270,0,N/2);//(int left, int top, int right, int bottom, double stangle, double endangle)
	_getch();
	return 0;
}
```




***画多边形***






```
#include
#include
#include
#include
#include
int main(void)
{
	initgraph(640, 480);
	POINT  a[3] = {100, 100, 300, 100, 300, 450 };
	polygon(a, 3);//void polygon  (const POINT *points, int num);
	              /*typedef struct tagPOINT
                    {
                        LONG  x;
                        LONG  y;
                    } POINT, *PPOINT, NEAR *NPPOINT, FAR *LPPOINT;
                  */
                  //polygon函数的第一个参数就是POINT结构类型的指针结构变量points，后一个参数是顶点个数
                  //画的多边形是不可填充的
	_getch(); 
	return 0;
}
```




**画折线**





```
#include
#include
#include
#include
#include
int main(void)
{
	initgraph(640, 480);
	POINT  a[3] = {100, 100, 300, 100, 300, 450 };
	polyline(a, 3);//与polygon函数的区别在于它最后不会自动封口。
	               //最后一个参数是点的个数。
	_getch();
	return 0;
}
```





***关于文本函数***






```
#include
#include
#include
#include
#include
#include
#define N 3.14159266
int main(void)
{
	initgraph(640, 480);
	int height, width;
	settextstyle(100, 100, L"仿宋",-900,900,FW_THIN,false,false,false);
	//(int nHeight, int nWidth, LPCTSTR lpszFace, int nEscapement, int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut);
	//（单字高，单字宽，字体，字体的倾斜度[0.1°为单位]，字符的倾斜度[0.1°为单位]，字的粗细<0,100,200···1000>，是否斜体[true&false],是否加下划线，是否加删除线
	//关于字体的倾斜度   eg.-900即竖向输出文本
	//关于字符的倾斜度   eg.900即将字符逆时针旋转90°
	setbkcolor(YELLOW);//用于给字符加底色
	setcolor(RED);//设置字符颜色
	outtextxy(50,0,L"hello");//在设定位置输出字符串
	height = textheight(L"hello");//返回字符串所用像素的高度
	width = textwidth(L"hello");//返回字符串所用像素的宽度
	//高宽会比设置的大一些，因为还有默认的边框，和设置的底色
	_getch();
	printf("height=%d,width=%d\n", height, width);
	return 0;
}
```




***在图形框显示图片***





```
#include
#include
#include
#include
#include
#include
#define N 3.14159266
int main(void)
{
	initgraph(640, 480);
	IMAGE img;//IMAGE是定义的结构体，参数为getwidth(),getheight()
	int width, height;
	loadimage(&img, L"D:\\try.jpg.jpg",200,200,false);
	//(IMAGE *pDstImg, LPCTSTR pImgFile, int nWidth = 0, int nHeight = 0, bool bResize = false);
	//参数2是图片储存的地址，参数3，4是设置图片的宽和高（大小）
//	Resize(&img, 100, 100);//是设置图形设备的尺寸，100，100，屏幕上就只能显示出100，100那个矩形的图片
	width=img.getwidth();//图形框内图片的宽度
	height = img.getheight();//图形框内图片的高度
	putimage(0, 0, &img, SRCCOPY);//在图形框内显示图片(int dstX, int dstY, const IMAGE *pSrcImg, DWORD dwRop = SRCCOPY);
	//前两个参数，是图片显示的左上角的点的坐标
	_getch();
	printf("height=%d\nwidth=%d\n", height, width);
	return 0;
}
```






***setlinestyle和HSLtoRGB函数***









```
#include
#include
#include
#include
#include
#include
#define N 3.14159266
int main(void)
{
	initgraph(640, 640);
	float H = 0;
	float S = 1;
	float L = 0.5f;
	setlinestyle(PS_SOLID, 1);		// 设置线宽为 2
	                                //线型（PS_SOLID实线型），线粗（一般是1，3代表细粗）
	setcolor(HSLtoRGB(0,0, L));     //HSLtoRGB(a,b,c);参数都是float型的，代表色相（0~360，可百度色相图），饱和度（0~1，0为灰色，1为最纯的色光），亮度（0黑~0.5最鲜艳~1白）
	circle(320, 640, 400);
	_getch();
	return 0;
}
```





**国际象棋盘**





`**#include#include#include#include#include#include#define N 3.14159266char ch[9] = "ABCDEFGH";int num[8] = { 12345678 };void draw_even_square(int i);void draw_odd_square(int i);void printf_letter(void);int main(void){	initgraph(500, 500);	int i;		setcolor(HSLtoRGB(0, 0, 0.5));	setfillcolor(HSLtoRGB(0, 0, 0.5));	fillrectangle(50, 50, 450, 450);	setfillcolor(HSLtoRGB(15,0.4,0.4));	fillrectangle(60, 60, 440, 440);	setfillcolor(HSLtoRGB(0, 0, 0.5));	fillrectangle(80, 80, 420, 420);	setfillcolor(HSLtoRGB(35, 0.2, 0.5));	fillrectangle(90, 90, 410, 410);	setlinecolor(BLACK);	for (i = 1; i <= 8; i++)	{		if (i%2!=0)			draw_odd_square(i);		else			draw_even_square(i);	}	printf_letter();	_getch();	return 0;}void draw_odd_square(int i){	int k;	setfillcolor(BLACK);	setcolor(BLACK);	for (k = 1; k <= 4; k++)	{		fillrectangle(90 + (i / 2) * 40 * 2, 90 + 40 + (k - 1) * 40 * 2, 90 + 40 + (i / 2) * 40 * 2 , 90 + 40 + (k - 1) * 40 * 2+40);	}}void draw_even_square(int i){	int k;	setfillcolor(BLACK);	setcolor(BLACK);	for (k = 1; k <= 4; k++)	{		fillrectangle(90 +40+ (i/2-1) * 40*2, 90 + (k - 1) * 40*2, 90 +40+ (i/2-1) * 40*2+40 , 90 + (k-1) * 40*2+40);	}}void printf_letter(void){	int j,k;	for (j = 0; j < 8; j++)	{		settextcolor(WHITE);		setbkcolor(HSLtoRGB(15, 0.4, 0.4));		settextstyle(10,10, L"宋体", 0, 1800, 1, false, false, false);		outtextxy(107 + 40 * j, 75,ch[j]);		settextstyle(10,10, L"宋体", 0, 0, 1, false, false, false);		outtextxy(108 + 40 * j, 425,ch[j]);	}	for (k = 0; k < 8; k++)	{		settextcolor(WHITE);		setbkcolor(HSLtoRGB(15, 0.4, 0.4));		settextstyle(10, 10, L"宋体", 0, 1800, 1, false, false, false);		outtextxy(425, 115+40*k, ch[k]);		settextstyle(10, 10, L"宋体", 0, 0, 1, false, false, false);		outtextxy(65,105 + 40 * k, ch[k]);	}}**`






**星空和RGB函数**






```
#include
#include
#include
#include
#include
#define N 200  //N为星星总数
struct STAR
{
	double x;  //作为点的坐标应该是int型的，因为putpixel函数的参数就是int x,int y
	double y;  //但是为了更真实就用了double
	double step;
	double colortemp;
	int color;//RGB(红,绿,蓝)函数的返回值为int型
};
STAR star[N];
void initstar(int i)
{
	star[i].x = 0;
	star[i].y = rand() % 480;
	star[i].step = rand() % 5000 / 1000.0 + 1;
	star[i].colortemp = star[i].step * 255 / 6;
	star[i].color = (RGB(star[i].colortemp, star[i].colortemp, star[i].colortemp));
	//RGB(a,b,c),当a=b=c时，为灰色，值越大就越亮
}
void movestar(int i)
{
	putpixel(star[i].x, star[i].y,BLACK);//这里要设成黑色，才可以擦掉原来的点~
	star[i].x += star[i].step;
	putpixel(star[i].x, star[i].y, star[i].color);
	if (star[i].x > 640)
	{
		initstar(i);
		putpixel(star[i].x, star[i].y, star[i].color);
	}
}
int main(void)
{
	initgraph(640, 480);
	srand(time(NULL));
	int i;
	unsigned long num=0;
	for (i = 1; i <= N; i++)
	{
		initstar(i);
		star[i].x = rand() % 640;//这个可以让开始的时候不出现右边是空的的情况
	}
	while (!_kbhit()) //_kbkit() 若没有击键，则返回值为0
	{
		for (i = 1; i < N; i++)
		{
			movestar(i);
		}
		Sleep(100);//发现星星移动的巨快，就先让这个程序睡0.1s~
				   //以毫秒为单位0.001s
	}
	_getch();
	return 0;
}
```





上面的星空的代码，就真的很好很好，说明了两个很重要的道理，

**其一：所谓的动态不过是静态说了一个谎····**

**其二：以后做动态代码，就按照这种思路。 建立主体，初始化主体函数，移动主体函数，主函数。**




***会动的圆***




```
#include
#include
#include
#include
#include
void set_move_circle(void)
{
	int i,k;
	setfillcolor(GREEN);
	fillcircle(40, 240, 40);
	for (i = 1; i <= 560; i++)
	{
		setfillcolor(BLACK);
		setcolor(BLACK);
		fillcircle(40 + i - 1, 240, 40);
		setfillcolor(GREEN);
		setcolor(WHITE);
		fillcircle(40 + i, 240, 40);
		Sleep(10);
	}
	for (k = 1; k <= 560; k++)
	{
		setfillcolor(BLACK);
		setcolor(BLACK);
		fillcircle(40 + i-(k - 1), 240, 40);
		setfillcolor(GREEN);
		setcolor(WHITE);
		fillcircle(40 + i-k, 240, 40);
		Sleep(10);
	}
}
int main(void)
{
	initgraph(640, 480);
	set_move_circle();
	_getch();
	return 0;
}
```





**移动的车**







```
#include
#include
#include
#include
#include
#include
#include
#include
#include
#include
void set_move_car(void)
{
	int i;
	setfillcolor(HSLtoRGB(180,1,0.5));
	fillrectangle(10, 200, 50, 220);
	setfillcolor(HSLtoRGB(30, 0.5, 0.5));
	fillcircle(20, 225, 5);
	fillcircle(40, 225, 5);
	for (i = 1; i <= 560; i++)
	{
		setcolor(BLACK);
		setfillcolor(BLACK);
		fillrectangle(10+(i-1), 200, 50+(i-1), 220);
		fillcircle(20 + (i - 1), 225, 5);
		fillcircle(40 + (i - 1), 225, 5);
		setcolor(BLACK);
		setfillcolor(HSLtoRGB(180, 1, 0.5));
		fillrectangle(10+i, 200, 50+i, 220);
		setfillcolor(HSLtoRGB(30, 0.5, 0.5));
		fillcircle(20+i, 225, 5);
		fillcircle(40+i, 225, 5);
		Sleep(10);
	}
}
int main(void)
{
	initgraph(640, 480);
	set_move_car();
	_getch();
	return 0;
}
```








**烟花**



```
#include
#include
#include
#include
#include
#include
#define Pi  3.14159266
#define N 200  //Littlestar的数目
void draw_Bigstar(void)//画空心五角星的代码//
{
	int num_x[10] = { 50, 110, 170, 230, 290, 350, 410, 470, 530, 590 };
	int num_y[10] = { 50, 75, 35, 42, 60, 32, 45, 65, 48, 40 };
	int bian = 5;//定义星星的边长为5
	int i, k, t;
	POINT STAR[10];//为使用polygon函数为星星的十个点的xy坐标提供储存的位置
	for (i = 0; i < 9; i++)
	{
		STAR[0].x = num_x[i];
		STAR[0].y = num_y[i];
		t = -4;
		for (k = 1; k < 10; k++)
		{
			if (k % 2)
			{
				t += 4;
				STAR[k].x =STAR[k-1].x+ (int)(5 * cos(t * 36 * Pi / 180));
				STAR[k].y = STAR[k - 1].y + (int)(5 * sin(t * 36 * Pi / 180));
			}
			else
			{
				t -= 2;
				STAR[k].x = STAR[k - 1].x+(int)(5 * cos(t * 36 * Pi / 180));
				STAR[k].y = STAR[k - 1].y+(int)(5 * sin(t * 36 * Pi / 180));
			}
			setlinecolor(WHITE);
			line(STAR[k - 1].x, STAR[k - 1].y, STAR[k].x, STAR[k].y);
			if (k == 9)
			{
				line(STAR[9].x, STAR[9].y, STAR[0].x, STAR[0].y);
			}
		}
	}
}
void draw_Littlestar(void)
{
	srand(time(NULL));
	int i;
	int x, y, color;
	for (i = 0; i < N; i++)
	{
		color = rand() % 100 + 150;
		x = rand() % 640;
		y = rand() % 640;
		putpixel(x, y, RGB(color, color, color));
	}
}
/*
void init_Firework(void)
{
	int point_x, point_y;
	int size;
	int color;
	srand(time(NULL));
	point_x = rand() % 450;
	point_y = rand() % 260 + 170;
	size = rand() % 50 + 50;
	color = rand() % 225 + 90;
}
由于初始化的好多变量都在其他两个函数里面有很大的作用，所以就干脆在创造烟花函数里面做好了*/
void cover_Firework(int point_x,int point_y,int size)
{
	int cover = 0;
	setcolor(BLACK);
	setfillcolor(BLACK);
	for (; cover <= size; cover+=3)
	{
		if (cover + 3 - size)
		{
			cover = size;
		}
		fillcircle(point_x, point_y, cover);
		Sleep(30);
	}
	Sleep(10);
}
void create_Firework(void)
{
	while (!_kbhit())
	{
		srand(time(NULL));
		//初始化烟花变量
		int point_x, point_y;
		int size;
		int color;
		srand(time(NULL));
		point_x = rand() % 300+100;
		point_y = rand() % 200 + 150;
		size = rand() % 10 + 50;
		color = rand() % 225 + 90;
		//初始化完了~
		//正式的创建烟花函数开始
		int i, j;
		int x, y;
		for (i = 0; i < size; i+=5)
		{
			for (j = 0; j < 6; j++)
			{
				color--;
				x = point_x + (int)i*cos(j * 60 * Pi / 180);
				y = point_y + (int)i*sin(j * 60 * Pi / 180);
				putpixel(x, y, HSLtoRGB(color, 1, 0.5));
				putpixel(x - 1, y, HSLtoRGB(color, 1, 0.5));
				putpixel(x + 1, y, HSLtoRGB(color, 1, 0.5));
				putpixel(x, y - 1, HSLtoRGB(color, 1, 0.5));
				putpixel(x, y + 1, HSLtoRGB(color, 1, 0.5));
				putpixel(x - 1, y - 1, HSLtoRGB(color, 1, 0.5));
				putpixel(x + 1, y - 1, HSLtoRGB(color, 1, 0.5));
				putpixel(x - 1, y + 1, HSLtoRGB(color, 1, 0.5));
				putpixel(x + 1, y + 1, HSLtoRGB(color, 1, 0.5));
			}
			Sleep(100);
		}
		//这个烟花创建完了，开始cover
		cover_Firework(point_x,point_y,size);
		//这里之所以要用清屏函数就是因为cover总有可能覆盖一些星星，又没有什么更好的办法
		//虽然我想到了星空那样的代码，不过感觉没必要
		cleardevice();
		draw_Bigstar();
		draw_Littlestar();
	}
}
int main(void)
{
	initgraph(640, 640);
	draw_Bigstar();
	draw_Littlestar();
	create_Firework();
	_getch();
	return 0;
}
```





**记得知道C语言图形学的愿望就是因为想做烟花，10天多以后终于实现了**







***方块填色游戏***




```
#include
#include
#include
#include
#include
/*
struct MOUSEMSG
{
UINT uMsg;      // 当前鼠标消息
bool mkCtrl;    // Ctrl 键是否按下
bool mkShift;   // Shift 键是否按下
bool mkLButton; // 鼠标左键是否按下
bool mkMButton; // 鼠标中键是否按下
bool mkRButton; // 鼠标右键是否按下
int x;          // 当前鼠标 x 坐标
int y;          // 当前鼠标 y 坐标
int wheel;      // 鼠标滚轮滚动值
};
*/
/*
WM_MOUSEMOVE     鼠标移动消息
WM_MOUSEWHEEL    鼠标滚轮拨动消息
WM_LBUTTONDOWN   左键按下消息
WM_LBUTTONUP     左键弹起消息
WM_LBUTTONDBLCLK 左键双击消息
WM_MBUTTONDOWN   中键按下消息
WM_MBUTTONUP     中键弹起消息
WM_MBUTTONDBLCLK 中键双击消息
WM_RBUTTONDOWN   右键按下消息
WM_RBUTTONUP     右键弹起消息
WM_RBUTTONDBLCLK 右键双击消息
*/
void setcolorblock(void);
void set_Block(void);
int main(void)
{
	initgraph(640, 480);
	MOUSEMSG m;
	double x, y;
	int i;
	int color;
	setcolorblock();
	set_Block();
	printf("%d\n", BLUE);
	while (true)
	{
		m = GetMouseMsg();
		if (m.uMsg != WM_LBUTTONDOWN&&m.uMsg != WM_RBUTTONDOWN)
		{
			continue;
		}
		else if (m.uMsg == WM_LBUTTONDOWN)
		{
			
			if (m.y >= 400 && m.y <= 450 && m.x <= 400 && m.x >= 200)
			{
				for (i = 0; i < 5; i++)
				{
					if (m.x >= (200 + i * 50) && m.x <= (200 + (i + 1) * 50))
					{

						switch (i)
						{
						case 0:
							color = BLUE;
							break;
						case 1:
							color = YELLOW;
							break;
						case 2:
							color = GREEN;
							break;
						case 3:
							color = RED;
							break;
						}
					}
				}
			}
			else if (m.x >= 100 && m.x <= 500 && m.y >= 100 && m.y <= 300)
			{
				for (i = 0; i < 2; i++)
				{
					if (m.x >= (100 + i * 200) && m.x <= (100 + (i + 1) * 200))
					{
						setcolor(BLACK);
						setfillcolor(BLACK);
						fillrectangle(100 + 200 * i, 100 , 100 + 200 * (i + 1), 300);
						setcolor(WHITE);
						setfillcolor(color);
						fillrectangle(100 + 200 * i, 100, 100 + 200 * (i + 1), 300);
					}
				}
			}
		}
		else if (m.uMsg == WM_RBUTTONDOWN)
		{
			break;
		}
	}
	closegraph();
	return 0;
}
void setcolorblock(void)
{
	setcolor(WHITE);
	setfillcolor(BLUE);
	int num = getfillcolor();
	fillrectangle(200, 400, 250, 450);
	setfillcolor(YELLOW);
	fillrectangle(250, 400, 300, 450);
	setfillcolor(GREEN);
	fillrectangle(300, 400, 350, 450);
	setfillcolor(RED);
	fillrectangle(350, 400, 400, 450);
}
void set_Block(void)
{
	setcolor(WHITE);
	setfillcolor(BLACK);
	fillrectangle(100, 100, 500, 300);
	line(300, 100, 300, 300);
}
```





























