# COLORREF和COLOR和RGB和CString的转化总结 - L_Andy的专栏 - CSDN博客

2013年08月27日 17:27:17[卡哥](https://me.csdn.net/L_Andy)阅读数：14305


一、COLORREF与RGB的相互转化

RGB(r,g,b)是一个宏

实际上它做得事是((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

 rgb(r,g,b)   =   一个整型值   =   r   +   g   *   256   + b*255*256   

COLORREF 是 一 个 32-bit 整 型 数 值，它 代 表 了 一 种 颜 色。你 可以 使 用 RGB 函 数 来 初 始 化 COLORREF

它的定义

typedef DWORD   COLORREF;

COLORREF变量有两种赋值方法

第一种

COLORREF cf = RGB(,,);

第二种

 CColorDialog colorDialog;

 COLORREF color;

    if( colorDialog.DoModal() == IDOK )

    {

        color = colorDialog.GetColor();

    }

这 段 代 码 使 用 了 MFC 中 的 颜 色 对 话 框

如何从 COLORREF中取出RGB分量值?

可以使用宏GetRValue

 GetGValue

 GetBValue

他们的定义如下

#define GetRValue(rgb)      ((BYTE)(rgb))

#define GetGValue(rgb)      ((BYTE)(((WORD)(rgb)) >> 8))

#define GetBValue(rgb)      ((BYTE)((rgb)>>16))

二 、Color与ColorRef(int类型表示的颜色)的相互转换

实际的软件开发过程中,常需要用到非.net平台的代码。这时候就可能碰到ColorRef(也就是以int类型代表的颜色值

或是以DWORD值表示的颜色)。这跟.net平台下的颜色的相互转换MS并没有直接实现。那么就需要我们自己处理

了。这里给出两个函数。                   

uint GetCustomColor(Color color)

{            

     int nColor = color.ToArgb();            

     int blue = nColor & 255;            

     int green = nColor >> 8 & 255;            

     int red = nColor >> 16 & 255;            

     return Convert.ToUInt32(blue << 16 | green << 8 | red);        

}

Color GetArgbColor(int color)        

 {            

     int blue = color & 255;            

     int green = color >> 8 & 255;            

     int red = color >> 16 & 255 ;            

     return Color.FromArgb(blue, green, red);        

 } 

注：COLORREF中颜色的排列是BGR，而通过Color.ToArgb()得到的数值中颜色排列是AARRGGBB

三、 注意

CColorDialog dlg;
if(dlg.DoModal() == IDOK)
{
//m_FillColor = dlg.GetColor();
COLORREF ctmpColor = dlg.GetColor();
Color tmpColor;
tmpColor.SetFromCOLORREF(ctmpColor);
ARGB argb = Color::MakeARGB(m_nFillTran, tmpColor.GetR(),tmpColor.GetG(),tmpColor.GetB());
//Graphics graphics(pDC->m_hDC);
//HatchBrush hBrush(HatchStyleDashedHorizontal,Color(100,255,0,0), Color(argb));
tmpColor = argb;
m_FillColor = tmpColor.ToCOLORREF();
//m_FillColor = RGB(Color(argb));

}

这是用api函数写的Colorref和color直间的转换。

//////////////////////////////////////////////////////////////////////////////////////////////////////

COLORREF 和字符之间的转换

一、格式化字符串的功能很强大

int iRed = GetRValue(pMarkInfo->lfColor);

int iGreen = GetGValue(pMarkInfo->lfColor);

int iBlue = GetBValue(pMarkInfo->lfColor);

CString szColor;

szColor.Format(_T("#%02X%02X%02X"), iRed, iGreen, iBlue);

/////////////////////////////////////////////////////////

CString m_BackColor.m_frame_color = "#0290D8";

DWORD r,g,b;

sscanf(m_BackColor.m_frame_color,"#%2X%2X%2X",&r,&g,&b);

COLORREF rgb = RGB(r,g,b);

brush.CreateSolidBrush (rgb)

//////////////////////////////////////////////////
//COLORREF转换为字符串

BOOL CDataManager::GetRGBText(std::string &strRGBText , COLORREF color)

{

 //COLORREF col = RGB( 255 , 12 , 4);

 BYTE Red = GetRValue(color); ///得到红颜色

 BYTE Green = GetGValue(color); ///得到绿颜色

 BYTE Blue = GetBValue(color); ///得到兰颜色

 char chR[4];

 itoa(Red ,chR , 10 );

 char chG[4];

 itoa(Green , chG , 10);

 char chB[4];

 itoa(Blue , chB , 10);

 std::string strR , strG, strB;

 strR = chR ;

 strG = chG;

 strB = chB;

 strRGBText = strR + "," + strG + "," + strB;

 return TRUE;

}

//字符串转换为COLORREF，如（"32","34","21"）

BOOL CDataManager::GetColorRGB(CString strColorText , COLORREF& color)

{

    char chR[4] = "", chG[4] = "", chB[4] = ""; 

    sscanf( strColorText, "%[^,],%[^,],%[^,]", chR, chG, chB);

 color = RGB(atoi(chR), atoi(chG), atoi(chB));

 return TRUE;

}

//////////////////////////////////////////////////////////////////////

