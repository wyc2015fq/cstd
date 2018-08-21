//********************************************************************************
//文件：PMScreenGDI.h
//作者：feiren
//时间：2011-8-20
//用途：封装基本的GDI显示方法、lafer算法、全局枚举对象
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMSCEENGDI_H__
#define __PMSCEENGDI_H__

#include "PMSpaceAlgorithm.h"
#include "PMGridConst.h"
#include "ximage.h"


#pragma comment (lib,"CXImage.lib")

#pragma once

#define TABLENGHT 4

enum PMType
{
//1、*******************************实体的类型***********************************
	PMENTITY_POINT=1,	//点
	PMENTITY_LINE,		//线
	PMENTITY_POLYLINE,	//折线
	PMENTITY_TRIANGLE,	//三角形
	PMENTITY_RECTANGLE,	//矩形	
	PMENTITY_POLYGON,	//多边形
	PMENTITY_ELLIPSE,	//椭圆
	PMENTITY_PIE,		//扇形
	PMENTITY_SMOOTINGRECT,	//渐变矩形
//2、*******************************单元格类型***********************************
	PMCELL_SYSTEM=0,	//系统表格,就是具有数字的头和行号
	PMCELL_USER,		//用户类型
	PMCELL_FIX,			//不可以编辑的表格
	PMCELL_TEXT,		//文本表格
	PMCELL_PICTURE,		//图片表格（图片作为背景，前景显示文字）
	PMCELL_FUNCTION,	//功能表格，如隐藏和收缩
	//==========================等等再实现=============================
	PMCELL_TREE=0,		//表格树
	PMCELL_LIST,		//列表
	PMCELL_COMBOLBOX,	//组合框
	PMCELL_BUTTON,		//按钮
	PMCELL_TIME,		//时间控件
	PMCELL_DATE,		//日期控件
	PMCELL_CKECKBOX,	//选择框
	PMCELL_TABBOX,		//页面框
	PMCELL_PROGRESS,	//进度条
	PMCELL_CHART,		//图表
	PMCELL_MIND,		//思维导图
	PMCELL_GRAPHIC,		//画图板
	PMCELL_MEDIA,		//声音
	PMCELL_VIDIO,		//视频
	PMCELL_ORGE3D,		//OGRE 3D动画
//3、******************************单元格状态************************************
	PMCELL_DEFAULT			=0x000000,
	PMCELL_FOCUSED			=0x000001,
	PMCELL_SELECTED			=0x000010,
	PMCELL_ENTER			=0x000100,
	PMCELL_MODIFIED			=0x001000,
//4、*******************************单元格线框类型***********************************
	PMCELL_LINE_All			=0x000000,
	PMCELL_LINE_UP			=0x000001,
	PMCELL_LINE_DOWN		=0x000010,
	PMCELL_LINE_LEFT		=0x000100,
	PMCELL_LINE_RIGHT		=0x001000,
	PMCELL_LINE_UP_CROSS	=0x010000,
	PMCELL_LINE_DOWN_CROSS	=0x100000,
	PMCELL_GROUND			=0x000002,
//5、*******************************表格鼠标的当前状态***********************************
	PMCELL_MOUSE_DEFAULT	=0x000000,
	PMCELL_MOUSE_SELECTED	,
	PMCELL_MOUSE_RESIZENS	,
	PMCELL_MOUSE_RESIZEWE	,
	PMCELL_MOUSE_DRAG		,
	PMCELL_MOUSE_COPY		,

//6、********************************编辑框鼠标的状态**********************************
	PMEDITOR_MOUSE_DEFAULT	=0x000000,
	PMEDITOR_MOUSE_SELECTED ,
	PMEDITOR_MOUSE_LINESELECT,
	PMEDITOR_MOUSE_BEGINDRAG,
	PMEDITOR_MOUSE_DRAG		,	
	PMEDITOR_MOUSE_PICTUREEDIT,
	PMEDITOR_MOUSE_PICTURERESIZE,

//7、********************************编辑框命令的状态**********************************
	PMEDITOR_COMMAND_DEFAULT	= 0x000000,
	PMEDITOR_COMMAND_CHARACTERMERGE	,
	PMEDITOR_COMMAND_DELETE			,
	PMEDITOR_COMMAND_EDIT			,

	PMEDITOR_COMMAND_PICTURERESIZE	,
	PMEDITOR_COMMAND_DRAG			,
};
class ScreenGDI;
class PMPicture
{
public:
	PMPicture();
	~PMPicture();
	void ReleaseBuffer();
public:
	HBITMAP mhBitmap;
	IntSize mSize;	
	IntRect mRect;
	//TJSON   *mJson;
};

class PMPictures
{
public:
	PMPictures();
	~PMPictures();
	PMPicture* operator[](PMInt index);
	void ClearPictures();
public:
	vector<PMPicture*> mPictures;	
};

class PMLaferLexer
{
public:
	TJSON *CreateLaferJson		(const string &laferdata);							//从lafer代码转化成JSON内存数据
	string CreateLaferCode		(TJSON &laferJson);									//将内存数据转化成字符串
private:
	PMVoid ParseEntity		(TJSON &laferJson,TWordLex &lex);
	PMVoid ParseRect		(TJSON &laferJson,TWordLex &lex);
	PMVoid ParseFrame		(TJSON &laferJson,TWordLex &lex);
	PMVoid ParseBody		(TJSON &laferJson,TWordLex &lex);
	PMVoid ParsePoint		(TJSON &laferJson,TWordLex &lex);
	PMVoid ParseLine		(TJSON &laferJson,TWordLex &lex);
	PMVoid ParsePolyLine	(TJSON &laferJson,TWordLex &lex);
	PMVoid ParseTriangle	(TJSON &laferJson,TWordLex &lex);
	PMVoid ParseRectangle	(TJSON &laferJson,TWordLex &lex);
	PMVoid ParseSmootingRect(TJSON &laferJson,TWordLex &lex);
	PMVoid ParseEclipse		(TJSON &laferJson,TWordLex &lex);
	PMVoid ParsePolygon		(TJSON &laferJson,TWordLex &lex);
	PMVoid ParsePie			(TJSON &laferJson,TWordLex &lex);

	string EntityToCode		(TJSON &laferJson);
	string FrameToCode		(TJSON &laferJson);
	string BodyToCode		(TJSON &laferJson);
	string PointToCode		(TJSON &laferJson);
	string LineToCode		(TJSON &laferJson);
	string PolyLineToCode	(TJSON &laferJson);
	string TriangleToCode	(TJSON &laferJson);
	string RectangleToCode	(TJSON &laferJson);
	string SmootingRectToCode(TJSON &laferJson);
	string EllipseToCode	(TJSON &laferJson);
	string PolygonToCode	(TJSON &laferJson);
	string PieToCode		(TJSON &laferJson);
	string ColorToCode		(TJSON &laferJson);


	PMVoid SetJsonValue(TJSON &laferJson,const string &strValue);
	PMVoid SetJsonValue(TJSON &laferJson,TWordLex &lex);
	PMVoid SetJsonColor(TJSON &laferJson,TWordLex &lex);
	string GetJsonValue(TJSON &laferJson);
	string GetJsonColor(TJSON &laferJson);
};

struct PenData
{
	PenData():mHDC(NULL),mColor(0),mWidth(1),mStyle(0){}
	PMBool Equal(HDC hdc,PMColor iColor, PMInt iWidth, PMInt iStyle);
	PMVoid Set(HDC hdc,PMColor iColor, PMInt iWidth, PMInt iStyle);
	HDC		mHDC;
	PMColor mColor;
	PMInt	mWidth;
	PMInt	mStyle;
};

struct  TColorFormula
{
	static TColorFormula *GetSingleton();
	TColorFormula():RBase(68),RNumerator(89),GBase(71),GNumerator(90),BBase(80),BNumerator(90){};
	PMVoid  Initialize(TJSON *json);
	PMColor GetColor(PMColor sourceColor);	
	BYTE RBase;
	BYTE RNumerator;
	BYTE GBase;
	BYTE GNumerator;
	BYTE BBase;
	BYTE BNumerator;
};
//本类封装GDI画图，每次都直接画在内存的bitmap上，
//直到调用DrawFrame才将数据绘制到屏幕上
enum TTEXTFORMAT
{
	TF_VTOP		=0x0001,
	TF_VCENTER	=0x0002,
	TF_VBOTTOM	=0x0004,
	TF_HLEFT	=0x0008,
	TF_HCENTER	=0x0010,
	TF_HRIGHT	=0x0020
};
class ScreenGDI
{
public:
	ScreenGDI();
	virtual ~ScreenGDI(void);
	static PMVoid SetInstance	(HINSTANCE hInstance);
	PMVoid Initialize			(HWND hWnd,const RECT &ClientRect);
	PMVoid SetCurrentDC			(HDC currentDC);									//设置当前DC
	PMVoid DrawFrame			(const IntRect *rect=NULL);							//将ScreenGDI创建的内容绘制到屏幕上
	PMVoid WindowResize			(const RECT &clientRect);							//重新设置窗体的大小
	PMVoid WindowResize			(const IntRect &clientRect );						//重新设置窗体的大小
	HDC    GetDC				();													//获取当前的DC
	PMVoid ClearBackGround		(PMInt iColor=-1);									//擦除背景
	PMVoid SetNullPen			();													//设置空画笔
	PMVoid SetPen				(PMColor color = RGB(0,0,0),PMInt width = 1,PMInt style = 0);//设置画笔
	PMVoid SetHatchPen			(PMColor color,PMInt width ,PMInt style );			//设置样式画笔
	PMVoid SetBrush				(PMColor color = RGB(255,255,255));					//设置颜色画刷
	PMVoid SetHatchBrush		(PMInt fnStyle,PMColor color = RGB(255,255,255));	//设置样式画刷
	PMVoid SetPictureBrush		(PMBitmap hbmp);									//设置图片画刷
	PMVoid SetSystemFont		();													//设置系统行列的字体
	PMVoid SetFont				(TJSON *jnFont);									//设置系统行列的字体
	PMVoid CalcFontWidth		();													//计算当前字体的宽度
 	PMVoid SetTextAxtra			(PMInt VAxtra,PMInt HAxtra);						//设置文字显示的距离
	PMInt  GetStringWidth		(const PMChar *data,PMInt len);

	PMVoid LineExactly			(PMInt x1,PMInt y1,PMInt x2,PMInt y2,PMInt width);	//精确画线
 	PMVoid DrawPicture			(const PMPicture *picture,const IntRect &rect);		//绘制图片
 	PMVoid DrawLine				(const IntPoint &start,const IntPoint &end);		//绘制线条
	PMVoid DrawLine				(const IntPoints &points);							//绘制连接线条
 	PMVoid DrawDoubleLine		(const IntPoint &start,const IntPoint &end);		//绘制双线
 	PMVoid FillRetangle			(const IntRect &rect);								//绘制内容，将忽略边框的颜色
	PMVoid DrawRetangle			(const IntRect &rect);								//绘制边框，忽略内容填充
	PMVoid Retangle				(const IntRect &rect);								//绘制边框、和内容
	PMVoid ShowNumber			(PMInt number,PMInt group=0);
	PMVoid TextOutFormat		(const PMChar *data,PMInt len,const IntRect &rect,PMInt format=TF_VCENTER|TF_HCENTER);

	PMVoid PushPicture			(const IntSize &pictureSize);						//创建位图并设置为当前DC
	PMPicture* PopPicture();														//获取位图并将当前DC恢复原样

	//绘制边框，格式内容和线条的式样
	PMPicture* CreatePictur		 ( const IntSize &pictureSize,PMColor color );		//创建一定颜色的图片
	PMPicture* CreateLaferPicture(const IntSize &pictureSize,
								 const string &laferdata,const string &name);		//创建用来绘制lafer内容的图片,只能传Entity的json
	PMPicture* CreateLaferPicture(const IntSize &pictureSize,
								  TJSON &laferJson,const string &name);				//创建用来绘制lafer内容的图片,只能传Entity的json	
	PMPicture* CreateLaferFillPicture(const IntSize &pictureSize,
								 TJSON &laferJson,const string &name);				//创建用来绘制边框的图片,只能传Entity的json
	PMVoid	   DrawLaferFrame	(TJSON &laferJson,const IntRect &laferRect,
								 const string &name);								//按数据名称进行绘制屏幕,只能传Entity的json
	PMVoid	   DrawLaferLineFrame(TJSON &laferJson,const IntRect &laferRect,
								 const string &name);								//按数据名称将线段绘制到屏幕,只能传Entity的json
	TJSON*	   CreateLaferJson	(const string &laferdata);							//从lafer代码转化成JSON内存数据
	string     CreateLaferCode	(TJSON &laferJson);									//将内存数据转化成字符串
	IntRect    GetRect			(TJSON &laferJson,const string &name ="RECT");									//获取边框大小	
	

protected:
	PMVoid InitBuffer();
	PMVoid ReleaseBuffer();
	PMInt  GetValueX			(TJSON &laferJson);									//从lafer json获取x坐标
	PMInt  GetValueY			(TJSON &laferJson);									//从lafer json获取y坐标
	PMInt  GetWidthValue		(TJSON &laferJson);									//从lafer json获取宽度
	PMVoid DrawLaferBody		(TJSON &laferJson);									//绘制lafer帧内容
	PMVoid DrawLaferObjects		(TJSON &laferJson);									//绘制lafer分区实体
	PMVoid DrawLaferPoint		(TJSON &laferJson);									//绘制lafer点
	PMVoid DrawLaferLine		(TJSON &laferJson);									//绘制lafer线
	PMVoid DrawLaferPolyLine	(TJSON &laferJson);									//绘制lafer折线
	PMVoid DrawLaferRectangle	(TJSON &laferJson);									//绘制lafer渐变色矩形
	PMVoid DrawLaferSmootingRect(TJSON &laferJson);									//绘制lafer矩形
	PMVoid DrawLaferTriangle	(TJSON &laferJson);									//绘制lafer三角形
	PMVoid DrawLaferPolygon		(TJSON &laferJson);									//绘制lafer多边形
	PMVoid DrawLaferEllipse		(TJSON &laferJson);									//绘制lafer椭圆
	PMVoid DrawLaferPie			(TJSON &laferJson);									//绘制lafer扇形
	PMInt  SetParameter			(TJSON &wJson,TJSON &cJson,TJSON *bgcJson=NULL);	//设置线宽	
	HDC		mHdcBitmap;
	HDC		mHdcPicture;
	HDC		mCurrentHDC;
	HBITMAP mBitmap;	
	HBRUSH  mhWhiteBrush;
	HPEN	mhWhitePen;
	HBITMAP mOldBitmap;
	HBRUSH  mhOldBrush;
	HPEN	mhOldPen;
	HBRUSH  mhOldPictureBrush;
	HPEN	mhOldPicturePen;
	HFONT   mhOldFont;
	HFONT   mhSystemFont;

	HFONT   mhFont;
	HPEN	mhPen;
	HPEN    mNullPen;
	HBRUSH  mNullBrush;
	HBRUSH	mhBrush;	
	PenData mOldPendata; 

	PMInt	mVAxtra;
	PMInt	mHAxtra;
	//PMColor mOldPenColor;
	PMColor mOldBrushColor;
	PMVoid* mObjects[20];
	TJSON * mJnFont;
	PMInt   mFontTags[260];
	PMInt   mWideFontWidth;
	static const PMColor	mSystemFixLineColor  = 6974058;
	static const PMColor	mSystemGridLineColor = 10921638;
public:
	static  HINSTANCE mHinstance;
	HWND	mHWnd;
	HDC		mHdc;
	IntRect mRect;
	IntRect mLaferRect;
	IntRect mWindowRect;
	PMInt	mFontWidth;
	PMInt	mFontHeight;
};

#endif