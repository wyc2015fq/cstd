# 用VC实现竖写汉字的方法 - xqhrs232的专栏 - CSDN博客
2010年08月27日 11:19:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：806
原文地址::[http://www.bccn.net/Article/kfyy/vc/jszl/200709/6367.html](http://www.bccn.net/Article/kfyy/vc/jszl/200709/6367.html)
中国人自古就有自右至左、从上到下书写汉字的习惯。而当我们在自己所编写的应用程序中使用输出函数输出的总是自左至右的横排文字。有没有可能在我们的应用程序中实现竖写汉字的效果呢？笔者偶然发现了一种利用VC实现竖写汉字效果的方法，现在就把它介绍给大家。 
Windows系统内置了许多名称以“@”开头的字集，这些字集有一个共同的特点，即其所包含的文字全都是平躺着的(字头朝左，字底朝右)。如果我们能将这些字逆时针旋转270度后再显示出来，就可以达到实现竖写汉字效果的目的了。 
　　要使文字发生旋转，我们只需创建一个逻辑字体并设定该逻辑字体的显示角度，再设定其字体名称为一个以“@”开头的字集的名称即可。我们可以采用两种方式创建逻辑字体。 
　　其一，用CFont类的成员函数CreateFont直接创建逻辑字体。CreateFont的参数定义如下： 
BOOL CreateFont( 
                 int nHeight,           //字体的高度
                 int nWidth,            //字体的宽度
                 int nEscapement,       //字体显示的角度
                 int nOrientation,      //字体的角度
                 int nWeight,           //字体的磅数
                 BYTE bItalic,          //斜体字体
                 BYTE bUnderline,       //带下划线的字体
                 BYTE cStrikeOut,       //带删除线的字体
                 BYTE nCharSet,         //所需的字符集
                 BYTE nOutPrecision,    //输出的精度
                 BYTE nClipPrecision,   //裁减的精度 
                 BYTE nQuality,         //逻辑字体与输出设备的实际
                                        //字体之间的精度
                 BYTE nPitchAndFamily,  //字体间距和字体集
                 LPCTSTR lpszFacename   //字体名称
               );
　　其中nEscapement单位为0.1角度，方向为逆时针，所以为了使平躺的字立起来应设定其值为2700,lpszFacename则赋以“@”开头的字集的名称，如“@system”。 
　　其二,利用LOGFONT结构，将该结构作为参数调用CFont类的成员函数CreateFontIndirect创建逻辑字体。LOGFONT结构通常被用来描述一种逻辑字体，结构内的各变量分别代表逻辑字体的各个属性，这些变量与函数CreateFont的参数相同。其结构如下： 
typedef struct tagLOGFONT { // lf 
　　　　　　　　　　LONG lfHeight;         //字体的高度
　　　　　　　　　　LONG lfWidth;          //字体的宽度
　　　　　　　　　　LONG lfEscapement;     //字体显示的角度 
　　　　　　　　　　LONG lfOrientation;    //字体的角度
　　　　　　　　　　LONG lfWeight;         //字体的磅数
　　　　　　　　　　BYTE lfItalic;         //斜体字体
　　　　　　　　　　BYTE lfUnderline;      //带下划线的字体
　　　　　　　　　　BYTE lfStrikeOut;      //带删除线的字体
　　　　　　　　　　BYTE lfCharSet;        //所需的字符集
　　　　　　　　　　BYTE lfOutPrecision;   //输出的精度
　　　　　　　　　　BYTE lfClipPrecision;  //裁减的精度
　　　　　　　　　　BYTE lfQuality;        //逻辑字体与输出设备的
                                           //实际字体之间的精度
　　　　　　　　　　BYTE lfPitchAndFamily; //字体间距和字体集
　　　　　　　　　　TCHAR lfFaceName[LF_FACESIZE]; //字体名称
} LOGFONT;
　　下面我们就以一个示例来说明竖写汉字的实现方法： 
　　1.建立一个MFC AppWizard(exe)应用工程CHNfont。在MFC AppWizard向导的第一步中选择Single Document，再点击按键Finish->OK完成工程的建立。 
　　2.在CMainFrame::OnCreate函数中添加如下代码，使窗体最大化。AfxGetMainWnd()->ShowWindow(SW_SHOWMAXIMIZED); 
　　3.添加如下代码实现竖写功能。 
void CChildView::OnPaint() 
       {
CPaintDC dc(this);// device context for painting
/////// 利用CFont::CreateFont(...)函数实现竖写汉字////////
CFont myFont;     //创建字体对象
//创建逻辑字体
myFont.CreateFont(
　　　　　　　　　　　　　　56,        //字体高度(旋转后的字体宽度)=56 
　　　　　　　　　　　　　　20,        //字体宽度(旋转后的字体高度)=20
　　　　　　　　　　　　　　2700,       //字体显示角度=270°
　　　　　　　　　　　　　　0,                  //nOrientation=0
　　　　　　　　　　　　　　10,                 //字体磅数=10
　　　　　　　　　　　　　　FALSE,              //非斜体
　　　　　　　　　　　　　　FALSE,              //无下划线
　　　　　　　　　　　　　　FALSE,       //无删除线
　　　　　　　　　　　　　　DEFAULT_CHARSET,    //使用缺省字符集
　　　　　　　　　　　　　　OUT_DEFAULT_PRECIS, //缺省输出精度
　　　　　　　　　　　　　　CLIP_DEFAULT_PRECIS,//缺省裁减精度 
　　　　　　　　　　　　　　DEFAULT_QUALITY,    //nQuality=缺省值
　　　　　　　　　　　　　　DEFAULT_PITCH,      //nPitchAndFamily=缺省值
　　　　　　　　　　　　　　"@system");         //字体名=@system
CFont *pOldFont=dc.SelectObject(&myFont);//选入设备描述表
CRect rtClient;
GetClientRect(rtClient);   //获取客户区尺寸、位置信息
//在客户区适当位置输出文字
dc.TextOut(rtClient.Width()/2+30,rtClient.Height()/8,
               "无边落木萧萧下");
dc.SelectObject(pOldFont); //将myFont从设备环境中分离
myFont.DeleteObject();     //删除myFont对象
/////////// 利用LOGFONT结构实现竖写汉字//////////////
LOGFONT lf;                     //定义字体结构
lf.lfWeight=10;                 //字体磅数=10
lf.lfHeight=56;      //字体高度(旋转后的字体宽度)=56 
lf.lfWidth=20;                  //字体宽度(旋转后的字体高度)=20
lf.lfUnderline=FALSE;           //无下划线
lf.lfStrikeOut=FALSE;           //无删除线
lf.lfItalic=FALSE;              //非斜体
lf.lfEscapement=2700;           //字体显示角度=270°
lf.lfCharSet=DEFAULT_CHARSET;   //使用缺省字符集
        strcpy(lf.lfFaceName,"@system"); //字体名=@system
CFont myLogFont;                   //定义字体对象
myLogFont.CreateFontIndirect(&lf); //创建逻辑字体
pOldFont=dc.SelectObject(&myLogFont);//选入设备描述表
//在客户区适当位置输出文字
dc.TextOut(rtClient.Width()/2-30,rtClient.Height()/8,
                 "不尽长江滚滚来");
dc.SelectObject(pOldFont); //将myFont从设备环境中分离
myLogFont.DeleteObject();  //删除myLogFont对象
       }
4.编译连接，运行。 
以上实例在Windows98下，以VC++6.0编译通过。
