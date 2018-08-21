//********************************************************************************
//文件：PMEdit.h
//作者：feiren
//时间：2011-8-20
//用途：封装表格的文本框实现
//版本：1.0.0.0
//联系：feirench@gmail.com
//********************************************************************************
#ifndef __PMEDITOR_H__
#define __PMEDITOR_H__

#include "PMScrollBar.h"
#include "PMBaseAction.h"

#pragma once

#define RESERVEOFF 3
#define RESERVECONTENT 16
#define PICTURE_SECTION "\nPictures"
#define EDCONTENT_SECTION "Editor"

#define GETPOSITIONJSON(LD) &mJnData->At(LD.mJnIndex) 
//mBoundary是会变的，因为它会随着表格内容一起滚动，而边框却是不会变
class PMEdit:public IInput,ScreenGDI,PMBaseAction//行号是从1开始
{
	enum TextState      //-3中间选择,-2前面半选择，-1后面半选择，0普通、全部1选择，2编辑
	{
		STATE_MIDDLE = -3,
		STATE_FRONT		 ,
		STATE_BACK		 ,
		STATE_NORMAL	 ,
		STATE_SELECTEDALL,
		STATE_PICTUREEDIT,
	};
	enum SelectionState //-1全选、0没有选择、1范围、2图片编辑
	{
		SELECTED_ALL = -1,
		SELECTED_NONE,
		SELECTED_SCOPE,
		SELECTED_PICTUREEDIT,
	};
	enum ECType//编辑器内容类型
	{
		CONTENT_NONE			= 0x000000,
		CONTENT_TEXT			= 0x000001,
		CONTENT_PICTURE			= 0x000002,
		CONTENT_LINENUMBER		= 0x000004,
		CONTENT_LINEICON		= 0x000008,
		CONTENT_TEXTATTRIBUTE	= 0x000010,
		CONTENT_TEXTALIGN		= 0x000020,
	};
	enum ECPositionType//当前的部分，-2行号位置、-1图标位置、0内容位置
	{
		POSITION_LINENUBER = -2,
		POSITION_LINEICON,
		POSITION_RETE,
	};
	struct ECElement//用来显示的组合原始
	{
		ECElement():mType(CONTENT_NONE),mJnIndex(0),mOffset(0),mCount(0),mStringID(NULL),mIntID(-1),mJnFont(NULL),mFontIndex(-1){}
		PMInt		mType;				//类型,只有两种类型：text、picture
		IntRect		mBoundary;			//边框				
		PMInt		mJnIndex;			//数据的索引
		PMInt		mOffset;			//字符偏移
		PMInt		mCount;				//字符个数
		PMChar*		mStringID;			//图片的MD5、名称、字符串内容
		PMInt		mIntID;				//GIF图片注册的ID、字符串类型
		PMChar		mC;					//用来存储临时变量
		TJSON	   *mJnFont;			//字体
		PMInt	    mFontIndex;			//字体的索引,-1就是系统字体
		PMChar     *GetString()					  {mC=mStringID[mCount]; mStringID[mCount]=0; return mStringID;}
		PMVoid      Restore ()					  {mStringID[mCount]=mC;}
		PMVoid		Assign  (TTexToken *t)		  {mIntID=t->type; mStringID=t->p; mCount=t->len;}//先Assign才能SetJson
		PMVoid		SetJson (TJSON *j)			  {mOffset=mStringID-j->ToCString();}
		PMVoid		Clear   ()					  {mType=CONTENT_NONE; mJnIndex=0; mOffset=0; mCount=0; mStringID=NULL; mIntID=-1; mJnFont=NULL; mFontIndex=-1;}
		PMVoid		SetBound(PMInt l,PMInt t,PMInt r,PMInt b) {mBoundary.left=l; mBoundary.top=t; mBoundary.right=r; mBoundary.bottom=b;}
		PMVoid		SetFont (PMInt index,TJSON *f){mFontIndex=index; mJnFont=f;}
		PMVoid		AddWidth(PMInt deta)		  {mBoundary.left=mBoundary.right;mBoundary.right+=deta;}
		PMVoid		SetPicture(PMInt w,PMInt h,TJSON *j);		
	};
	typedef vector<ECElement> ECElements;
	struct ECLine//用来显示的行数据
	{
		ECLine():mRow(1),mY(0),mHeight(0),mWidth(0),mIconID(NULL),mGifID(-1){}
		PMInt		mRow;				//行号
		PMInt		mY;					//y坐标
		PMInt		mHeight;			//行高
		PMInt		mWidth;				//行宽
		PMChar*		mIconID;			//行图标
		PMInt		mGifID;				//GIF图片注册的ID
		ECElements  mElements;			//显示内容
	};
	typedef vector<ECLine*> ECLines;//用来显示的编辑器内容

	struct  Position//位置信息
	{
		Position():mSection(POSITION_RETE),mJnIndex(0),mOriOffset(0),mOffset(0),mJnFont(NULL),mFontIndex(-1),mFontHeight(0),mPositionX(0),mPositionY(0),mRow(0),mCol(0),mType(CONTENT_NONE){}
		PMInt		mSection;			//当前的部分，-2行号位置、-1图标位置、0内容位置
		PMInt		mJnIndex;			//对应的json数据位置
		PMInt		mOriOffset;			//对应的json数据原始位置偏移
		PMInt		mOffset;			//对应的json数据位置偏移
		TJSON	   *mJnFont;			//对应的json数据字体
		PMInt		mFontIndex;			//对应的json数据字体的索引
		PMInt		mFontHeight;		//对应的json数据字体高度
		PMInt		mPositionX;			//对应的json数据X坐标
		PMInt		mPositionY;			//对应的json数据Y坐标
		PMInt		mRow;				//对应的json数据所在的行
		PMInt		mCol;				//对应的json数据所在的列
		PMInt		mType;				//对象的类型
		
		PMVoid	ToJson(TJSON &jnFather)		  ;
		PMVoid	FromJson(TJSON &jnFather)	  ;
		PMVoid	Assign(ECElement *e)		  ;
		PMBool  operator !=(const TJSON &d)   ;
		PMBool  operator ==(const TJSON &d)	  ;
		PMBool	operator > (const Position &d){return mPositionY>d.mPositionY||(mPositionY==d.mPositionY&&mPositionX>d.mPositionX);}
		PMBool  operator !=(const Position &d){return !(mPositionX==d.mPositionX && mPositionY==d.mPositionY);}
		PMBool  operator ==(const Position &d){return mPositionX==d.mPositionX && mPositionY==d.mPositionY;}		
		PMVoid	Clear()						  {mSection=POSITION_RETE;mJnIndex=0;mOriOffset=0;mOffset=0;mJnFont=NULL;mFontIndex=-1;mFontHeight=0;mPositionX=0;mPositionY=0;mRow=0;mCol=0;}
		PMBool	Empty()						  {return mOffset==0 && mJnIndex==0 && mOriOffset==0;}
		IntPoint GetPosition()				  {return IntPoint(mPositionX,mPositionY);}
	};
	typedef vector<Position*> Positions;
	struct Selection //选择集合 
	{
		Selection():mType(SELECTED_NONE){}
		PMInt		mType;				//-1全选、0没有选择、1范围、2图片编辑
		Position	mFrom;				//选择集的开始位置
		Position	mTo;				//选择集的结束位置
		IntRect		mTopBound;			//选择集的开始范围
		IntRect		mMidBound;			//选择集的中间范围
		IntRect		mBotBound;			//选择集的结束范围
		PMVoid		Clear(){mType=SELECTED_NONE;mTopBound.clear();mMidBound.clear();mBotBound.clear();}
	};
	struct Caret    //光标
	{
		Caret():mVisible(true),mbState(false),mLastX(0){}
		PMBool		mVisible;			//光标是否可视，就是不断闪烁
		PMBool		mbState;			//光标当前的显示状态，1显示或者0不显示
		PMInt		mLastX;				//存放上一次的X坐标用来上移、下移、翻页
		Position    mLocation;			//位置
	};
	struct PictureResizeArea
	{
		PMInt	mIndex;					//存储8个点的位置方向,循序是左右，上下
		IntRect mNewBoundary;			//新的图片范围
		IntRect mBoundary;				//原始的图片范围
		IntRect mTop1;					//上1点的捕捉范围
		IntRect mTop2;					//上2点的捕捉范围
		IntRect mTop3;					//上3点的捕捉范围
		IntRect mMiddle1;				//中间1点的捕捉范围
		IntRect mMiddle2;				//中间2点的捕捉范围
		IntRect mBottom1;				//下1点的捕捉范围
		IntRect mBottom2;				//下2点的捕捉范围
		IntRect mBottom3;				//下3点的捕捉范围
	};
public://公共方法
	PMEdit(HWND hWnd,const RECT &ClientRect);
	virtual ~PMEdit(void);
	PMVoid  Initialize			(TJSON *jnFile);										//初始化文本框特性
	PMVoid  Show				(PMBool bDraw = false,PMBool bDrawScrollBar=true);		//显示文本框及其内容
	//单纯的显示
	PMVoid  ShowEx				(TJSON *jnData ,TJSON *jnSystemFont);					//直接显示文本内容，用于表格内容显示
	PMVoid	Clear				();														//清理内存
	string  GetText				(TJSON *jnData);										//获取文本框的文字内容
	string  GetSelectedText		();														//获取选择文字内容
	PMVoid  GetPack				(TBuffer &pack);										//获取包含字体，图片的所有内容
	PMVoid  GetSelectedScopePack		(TBuffer &pack);								//获取包含字体，图片的选择内容
protected://控件的事件处理	
	PMBool  OnSetFocus			(TInputData *EventData) ;								//控件得到焦点
	PMBool  OnLostFocus			(TInputData *EventData) ;								//控件失去焦点
	PMBool  OnMouseDrag			(TInputData *EventData) ;								//控件开始拖拽
	PMBool  OnDragOver			(TInputData *EventData) ;								//控件拖拽经过
	PMBool  OnMouseDrop			(TInputData *EventData) ;								//控件拖拽放下
	PMBool  OnMouseMove			(TInputData *EventData) ;								//控件鼠标移动
	PMVoid  OnMouseMoveOutside	(TInputData *EventData) ;								//鼠标在外部移动
	PMBool  OnMouseWheel		(TInputData *EventData) ;								//控件鼠标滚轮滚动	
	PMBool  OnLButtonDBLCLK		(TInputData *EventData) ;								//鼠标左键双击
	PMBool  OnRButtonDown		(TInputData *EventData) ;								//鼠标右键按下	
	PMBool  OnLButtonUp			(TInputData *EventData) ;								//鼠标左键松开
	PMBool  OnRButtonUp			(TInputData *EventData) ;								//鼠标右键松开
	PMBool  OnKeyDown			(TInputData *EventData) ;								//键盘非系统键按下
	PMBool  OnKeyUp				(TInputData *EventData) ;								//键盘非系统键松开
	PMBool  OnChar				(TInputData *EventData) ;								//键盘普通输入
	PMBool  OnIMEChar			(TInputData *EventData) ;								//键盘输入法输入	
	PMBool  OnDropFiles			(TInputData *EventData) ;								//从explorer拖拽文件到控件	
	PMBool  OnCaretTimmer		(TInputData *EventData) ;								//插入符号定时器消息	
	PMBool  OnEnter				(TInputData *EventData) ;   							//鼠标进入
	PMBool  OnLeave				(TInputData *EventData) ;   							//鼠标出来
	PMBool  OnSize				(TInputData *EventData) ;								//大小改变
	PMBool  OnPaint				(TInputData *EventData) ;								//重新绘制
	PMBool  OnLButtonDown		(TInputData *EventData) ;								//鼠标左键按下
	PMBool  OnGifTimmer			(TInputData *EventData) ;								//定时器消息
private://控件的操作方法
	PMVoid  UnExcute			(TJSON &pAction);									    //反向执行命令
	PMVoid  Excute				(TJSON &pAction);									    //执行命令	
	PMVoid  ClearCommandList	(TJSONS &cmdList);									    //删除命令的内存对象	

	//生成Action的真实操作
	PMVoid	CommandBack		();														    //删除
	PMVoid	CommandDelete	();														    //删除
	PMVoid	CommandCut		();														    //剪贴
	PMVoid	CommandPaste	();														    //粘贴
	//具有回退的操作	
	PMVoid  DoEdit				(TJSON &pAction);										//往前删除操作
	PMVoid  DoPictureResize		(TJSON &pAction);										//图片改变大小操作
	//回退操作
			
	PMVoid  UnDoEdit			(TJSON &pAction);										//回退往前删除操作
	PMVoid  UnDoPictureResize	(TJSON &pAction);										//回退图片改变大小操作

	//下面是没有回退的操作
	PMVoid  DoCopy();																	//复制操作
	PMVoid  DoSelectAll();																//全选操作
	PMVoid  DoEscape();																	//取消操作
	PMVoid  DoDown();																	//光标下移操作
	PMVoid  DoUp();																		//光标上移操作
	PMVoid  DoRight();																	//光标右移操作
	PMVoid  DoLeft();																	//光标左移操作
	PMVoid  DoPageDown();																//向下翻页操作
	PMVoid  DoPageUp();																	//向上翻页操作
	PMVoid  DoHome();																	//回到行首操作
	PMVoid  DoEnd();																	//回到行尾操作
	PMVoid  DoInsert();																	//删除插入
	PMVoid	DoSave();																	//保存数据

	PMVoid  ControlDelete();															//行尾删除
	PMVoid	ControlBack();																//行头删除
	PMVoid  ControlHome();																//回到文章开头
	PMVoid  ControlEnd();																//回到文章末尾

	PMVoid  ShiftDown();																//选择下
	PMVoid  ShiftUp();																	//选择上
	PMVoid  ShiftRight();																//选择右
	PMVoid  ShiftLeft();																//选择左
	PMVoid  ShiftPageDown();															//选择下一页
	PMVoid  ShiftPageUp();																//选择上一页
	PMVoid  ShiftHome();																//选择行首
	PMVoid  ShiftEnd();																	//选择尾

	PMVoid  AltRight();																	//下焦点
	PMVoid  AltLeft();																	//上焦点

	PMVoid	Refresh	(PMBool bShow);														//刷新：重新计算文本显示内容，再显示
private://控件事件及操作方法实现	
	PMVoid		OnKeyChar					(TInputData *EventData);					//键盘字符敲入事件
	PMVoid		OnMoveOutside				(TInputData *EventData);					//鼠标在文本框外部移动事件
	PMVoid		OnHScrollView				(PMInt iChange);							//水平滚动
	PMVoid		OnVScrollView				(PMInt iChange);							//竖直滚动
	PMVoid      OnIconAreaClick				(Position &LD);								//行图标区点击
	PMVoid      OnReteAreaClick				(Position &LD,TInputData *EventData);		//内容区点击
	PMVoid      OnLNAreaClick				(Position &LD,TInputData *EventData);		//行号区点击
	PMVoid		OnPictureClick				(Position &LD);								//图片被点击
	PMVoid		OnSelectionAdd				(Position &LD);								//选择集添加
	PMVoid		OnSelectionChange			(Position &LD);								//选择集变更
	PMVoid		OnContentDrag				(Position &LD);								//内容的拖拽
	PMVoid		OnPictureResize				(TInputData *EventData);					//图片大小改变事件
	PMVoid		OnPictureResizeCompleted	(TInputData *EventData);					//图片大小改变完成
	PMVoid		OnMouseSelectOutside		(TInputData *EventData);					//鼠标在文本框外文本选择事件
	PMVoid		OnSelectedIdentical			();											//选择相同类型的内容
	PMVoid		OnLineSelected				(TInputData *EventData);					//行选择
	//滚动条和浏览窗口事件
	PMVoid		OnHScroll					(PMInt iRange);								//水平滚动条滚动
	PMVoid		OnVScroll					(PMInt iRange);								//垂直滚动条滚动
	PMVoid		OnNavigate					(IntRect &Navigator);						//浏览窗口改变
private://辅助实现功能函数
	IntRect		GetRetePosition				(IntRect  &Bound);							//从真实坐标转换成Rete坐标
	IntPoint	GetRetePosition				(IntPoint &Position);						//从真实坐标转换成Rete坐标	
	IntPoint	GetRetePosition				(PMInt x,PMInt y);							//从真实坐标转换成Rete坐标	
	IntRect		GetActualPosition			(IntRect  &Bound);							//从Rete坐标转换成真实坐标
	IntPoint	GetActualPosition			(IntPoint &Position);						//从Rete坐标转换成真实坐标	
	IntPoint	GetActualPosition			(PMInt x,PMInt y);							//从Rete坐标转换成真实坐标	

	PMVoid		CreateSWInfos				(TJSON *jnData,TJSON *jnSystemFont);		//构造显示结构	
	PMVoid		CreateAutoLineSWInfos		(TJSON *jnData,TJSON *jnSystemFont);		//构造显示结构	
	PMVoid		CreateSystemSWInfos			(TJSON *jnData,TJSON *jnSystemFont);		//构造显示结构		
	PMVoid		CreateCaretData				();											//重新创建光标位置，一般放在文本开头
	PMVoid		ScrollVertical				(PMInt Range);								//竖直滚动
	PMVoid		ScrollHorizon				(PMInt Range);								//水平滚动	
	PMBool		ScrollToPosition			(Position &LD);								//滚动到指定位置显示	
	PMBool		ScrollToLocation			(PMInt x,PMInt y);							//滚动到指定位置显示	
	PMVoid		ShowText					(ECLines::iterator &Line,ECElements::iterator &Element,PMInt State);//显示文字
	PMVoid		ShowPicture					(ECElement &SWInfo,PMInt State);			//显示图片
	PMVoid		ShowLineIcon				(PMInt nLine);								//显示行开头的图标
	PMVoid		ShowLineNumber				(PMInt nLine);								//显示行号	
	PMVoid		DrawCaret					();											//绘制光标
	PMVoid		DisplayCaret				();											//显示或隐藏光标
	PMVoid		HideCaret					(HDC pdc);									//隐藏光标
	PMInt		GetType						(TJSON *jnItem);							//获取当前json的类型：文本、图片、图标、文字属性
	PMInt		GetCurrentRowHeight			();											//获取当前行高度，影响因素：字体高度、图标高度
	ECLine*     CreateLine					();											//创建行
	PMVoid		DeleteLine					(PMInt  iLine);								//删除行
	PMInt		GetLineNumberWidth			(PMInt	iLine);								//获取行号宽度
	PMInt		GetTotalWidth				(PMInt	iLine);								//获取总共的内容宽度
	PMInt		GetTotalHeight				();											//获取总共的内容高度
	PMInt		GetHeaderWidth				(PMInt	iLine);								//获取文本框头的宽度
	PMVoid		CardingHeight				(ECLine *Line,PMInt h,PMInt w);				//重新计算行的高度
	PMVoid		GetInitData					(TJSON *jnInitData);						//获取文本框初始化数据
	PMVoid		DrawImageFrame				(HDC pdc,IntRect &ImageBound);				//绘制编辑图片的外边框
	PMVoid		GetIterStartLine			();											//计算开始行数据	
	PMBool		IsInSelection				(Position &LD);								//位置是否在选择集中
	PMVoid		SetSelection				(Position &LD);								//设置选择集
	Position    GetPosition					(TInputData *EventData);					//从鼠标坐标获取位置数据
	Position    GetPosition					(PMInt x,PMInt y,PMInt MouseX);				//从Rete坐标获取位置数据
	Position    GetOutsidePosition			(PMInt x);									//获取第一行外部的选择鼠标
	PMVoid		AssignPosition				(Position &LD,ECLine *Line,ECElement *content,PMInt iCol,PMBool bLeft);//位置赋值
	PMVoid		GetPositionData				(Position &LD,ECElement *content,PMInt x);  //获取内容的位置数据
	PMBool		IsMouseInSelection			(TInputData *EventData);					//判断鼠标是否在选择区域内
	PMInt		GetMouseArea				(TInputData *EventData);					//获取鼠标所在区域，0普通文章区域，1行号区，2行图标区域，3选择区域	
	PMInt		GetContentState				(ECElement *Emlement);						//-1半选择，0普通、全部1选择，2编辑	
	PMInt		GetPositionX				(Position &LD);								//获取位置的X坐标
	PMVoid		RecalcPosition				(Position &LD);								//重新计算位置数据
	PMVoid		SetBeginPosition			(Position &LD);								//设置为开始的数据
	PMVoid		SetEndPosition				(Position &LD);								//设置为结束的数据
	PMVoid		GetSystemFont				(Position &LD);								//当文本框没有数据但是却设置了字体时候使用
	PMVoid		SetLastFont					(Position &LD,PMInt Index);					//专门处理文本末尾的字体设置
	PMVoid		CalcSelectionBound			();											//计算选择框	
	PMVoid		SetCaret					(Position &LD,PMBool bSetLast = true,PMBool bSetX = true);//设置光标
	Position    GetPreLocation				(Position &LD);								//获取当前位置的前一个位置
	Position    GetNextLocation				(Position &LD);								//获取当前位置的后一个位置
	Position    GetUpLocation				(Position &LD);								//获取当前位置的上一行的位置
	Position    GetDownLocation				(Position &LD);								//获取当前位置的下一行的位置
	Position    GetPageUpLocation			(Position &LD,PMInt DeltaY);				//获取上一页的位置
	Position    GetPageDownLocation			(Position &LD,PMInt DeltaY);				//获取下一页的位置
	Position    GetLineBeginLocation		(Position &LD);								//获取行开头的位置
	Position    GetLineEndLocation			(Position &LD);								//获取行结束的位置
	PMVoid		ScrollToPageDownLocaltion	(PMInt DeltaY,PMBool bShow);				//滚动到下一页的位置
	PMVoid		ScrollToPageUpLocaltion		(PMInt DeltaY,PMBool bShow);				//滚动到上一页的位置
	PMInt		GetPageDownDetaY			();
	PMInt		GetPageUpDetaY				();
	//文本选择
	PMBool		VerticalSelected			(TInputData *EventData,PMInt iChange);		//竖直选择，会上下方向滚动
	PMBool		HorizonSelected				(TInputData *EventData,PMInt iChange);		//水平选择，会水平方向滚动
	PMBool		HScrollView					(PMInt iChange);							//水平滚动
	PMBool		VScrollView					(PMInt iChange);							//竖直滚动
	//图片编辑
	PMVoid		GetPictureResizeArea		(IntRect &ImageBound);						//获取图片大小改变的范围数据
	PMBool		InPictureResizeArea			(PMInt x,PMInt y);							//判断捕捉是否在图片大小改变的位置
	PMVoid		DrawPictureEditFrame		();											//绘制图片大小范围框
	PMVoid		ConvertPosition2Json		(TJSON &jnFather,Position &LD);				//将Position转化为Json
	PMVoid		ConvertJson2Position		(TJSON &jnFather,Position &LD);				//将Json转化为Position	
	IntRect		GetPositionRect				();											//获取保存显示区域位置的矩形	
	PMVoid		MoveView					(const IntRect &RT);						//移动视图
private://数据操作
	ECElement   *GetElement					(Position &LD);								//获取当前位置的对象
	ECElement   *GetPreElement				(Position &LD);								//获取当前位置的前一个对象
	ECElement   *GetNextElement				(Position &LD);								//获取当前位置的后一个对象
	ECElement   *GetLastElement				();											//获取最后一个属性
	PMVoid		CharacterMergeCommand		(PMChar *data);								//数据合并命令
	PMVoid		SymbolAddCommand			(PMChar *data);								//数据添加命令	
	TJSON*		GetLastCommand				();											//获取最后一个执行的可合并的命令	
	PMVoid		EditDelete					(PMInt iDirection);							//删除数据
	PMVoid		EditContent					(Position &LD,TJSON *jnNew,TJSON *jnOld,PMInt iFrom);//数据替换
	PMVoid		EditContent					(TJSON &jnContent,PMInt iCommand);			//当前位置添加，选择替换，选择删除
	PMVoid		AddFont						(TJSON &jnNewData,ECElement *ElementTo,TJSON *jnOldData = NULL);//添加字体	
	PMVoid		ReCalcPosition				();											//重新计算位置
	PMVoid		PushOldPosition				(TJSON &jnFather);							//保存位置数据
	PMVoid		PopOldPosition				(TJSON &jnFather);							//获取位置数据
	PMInt		GetContentJsonIndex			(TJSON &jnContent);
	PMInt		GetContentJsonIndex			(PMInt iIndex);
	PMVoid		SetLDData					(Position &LD,TJSON &jnContent,ECElement *ElementFrom,PMInt iOffset,PMInt iIndex);
	PMBool		HasPosition					(Positions &LDS,PMInt jnIndex);
	PMVoid		ReCalcLinePosition			(Positions &LDS,ECLine *Line);
	PMBool		IsLastPosition				(Position &LD);
	PMBool		IsBeginPosition				(Position &LD);
	PMVoid		DealWithFontData			(Position LD,PMInt &iFrom);
	PMVoid		DealWithFontData			(Position LD,PMInt &iFrom,ECElement *ElementTo);
	Position    GetNextIdenticalPosition	(Position LD,PMInt &Type);
	Position    GetPreIdenticalPosition		(Position LD,PMInt &Type);
	PMBool		IsFont						(PMInt iIndex);
	PMBool		HasFont						(TJSON &jnContent);

	PMVoid		SetScrollParameter			();
	PMVoid		ShowScroll					();
private:
	PMInt		mStyle;						//编辑框式样类型
	PMInt		mBKColor;					//文本框的背景颜色
	PMInt		mTextFormat;				//文本对齐方式
	PMBool		mbEdit;						//是否可编辑
	PMBool		mbAutoLine;					//是否自动换行
	PMBool		mbVScrollBar;				//是否具有垂直滚动条
	PMBool		mbHScrollBar;				//是否具有水平滚动条
	PMInt		mLNBackColor;				//行号背景颜色
	PMInt		mTextSelectBKColor;			//文字选择的背景颜色
	PMInt		mHTextAxtra;				//文字水平间距
	PMInt		mVTextAxtra;				//文字垂直间距
	PMInt		mVTextAxtraOffset;			//文字垂直间距偏移量
	TJSON*		mLNTextAttribute;			//行号字体
	PMInt		mVisibleX;					//当前可见x逻辑坐标
	PMInt		mVisibleY;					//当前可见y逻辑坐标		
	PMInt		mVisibleLine;				//当前可见行号	
	PMInt		mHWheelSpeed;				//水平滚动速度
	PMInt		mVWheelSpeed;				//垂直滚动速度
	IntRect		mRectActual;				//真实可视的相对窗口，mboundary是真实窗口
	IntRect		mReteRect;					//真实可视的相对窗口，mboundary是真实窗口
	IntRect		mTextMargin;				//文字上下左右的距离
	IntSize		mSize;						//实际的长宽		
	ECLines	    mEDLines;					//按行保存可以直接显示的数据
	PMInt		mImageGroup;				//本编辑器在图片管理器中的Gif组的索引
	PMInt		mIconDefaultWidth;			//比例系数
	PMInt		mIconWidth;					//实际图标大小
private:
	PMScrollManager   mScrollManager;		//滚动条对象
	PMInt		mMouseState;				//鼠标状态
	PMCliboardManager*mCliboard;			//剪贴板对象
	PMPictureManager *mPictureManager;		//图片管理器
	TColorFormula    *mSelectedFormula;		//选择的颜色变化公式	
	PMInt		mHeaderWidth;				//编辑器头的宽度,包括行号、行图标、mMargin.left的宽度
	TJSON	   *mjnFile;					//文本框数据文件，边框二进制图片和mJnData
	TJSON	   *mProperty;					//文本框属性
	TJSON	   *mJnData;					//用来初始化编辑器内容数据的json,保存编辑的数据
	TJSON	   *mJnInitData;				//编辑器属性数据的json
	TJSON	   *mJnSystemFont;				//编辑器系统文字的json
	PMInt		mXOffset;					//当前显示的X偏移量
	PMInt		mYOffset;					//当前显示的Y偏移量
	PMInt		mContentWidth;				//当前显示的内容宽度,包括行号、行图标和文本内容
	PMInt		mContentHeight;				//当前显示的内容高度
	PMInt		mReteWidth;					//当前显示的有效文字宽度
	Caret		mCaret;						//光标
	Position    mLastLD;					//最后一次的定位位置
	Selection   mSelection;					//选择集
	HRGN		mhRgnReteRect;				//内容区裁剪框
	PictureResizeArea mPictureResizeArea;	//编辑图片的外表形状改变框
};

#endif