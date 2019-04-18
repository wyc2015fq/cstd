# VC5打印字体的控制 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 15:42:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1904


VC5打印字体的控制

C5.0 为Windows 的程序员提供了一个很好的C++ 开发环境，减少了很多编程负担，但同时也为我们在程序中加入自己的思想增加了难度。本人在一软件开发中，想控制文字打印时的字体，使字体大小缩小一倍，以节省打印纸。经过一段时间的摸索，终于解决了这一问题，下面分几步向大家做一介绍。

---- 一、对VC5 自动生成的程序框架进行改进

---- 这里用VC5 自动创建一个例子程序Test，单文档界面，注意在最后一步修改view 的继承类为CEditView。

---- 在view 类中，VC5 已经自动创建了三个用于支持打印的函数：OnPreparePrinting，OnBeginPrinting，OnEndPrinting。为了实现我们的功能，需要再继承以下几个函数：OnPrepareDC，OnPrint。并将OnPrepareDC 和OnEndPrinting 改为如下实现：

// OnPrepareDC()
void CTestView::OnPrepareDC
(CDC* pDC, CPrintInfo* pInfo)
{
CView::OnPrepareDC(pDC, pInfo);
}

// OnEndPrinting()
void CTestView::OnEndPrinting
(CDC* pDC, CPrintInfo* pInfo)
{
CView::OnEndPrinting(pDC, pInfo);
}
---- 用CView 来替代原来的CEditView，用以避免CEidtView 对打印的控制。控制字体及输出的功能主要在OnBeginPrinting 和OnPrint 两个函数来实现。

---- 二、实现OnBeginPrinting 函数

---- 根据VC5 编程机制，在OnBeginPrinting 函数实现打印前的准备工作，包括设置打印字体，根据打印机当前页面尺寸计算所需页数等。下面的程序是对打印字体的重新设置和计算所需打印纸页数。

---- 程序中首先取得打印机的横向和纵向分辨率，再得到当前打印字体的大小，然后计算出新的字体大小，为默认字体的一半。读者可以根据需要设定自己的打印字体大小。

---- 接着，取得当前打印纸的宽度和高度，再根据新字体的宽度和高度计算出每行的最大字符数和每页的最大行数。

---- 由于打印文件中有些行的宽度可能超过每行的最大字符数，所以程序中调用函数RedealTextData() 对打印文件进行重新整理，函数的实现在下面介绍。

---- 最后，程序中计算并设置所需的打印页数。

OnBeginPrinting()函数实现如下：
//====================================
// OnBeginPrinting
//====================================
void CTestView::OnBeginPrinting(CDC* pDC,
CPrintInfo* pInfo) 
{
//设置新的? 字体////////////////

//取打印机的横方向和纵方向的分辨率
//即每英寸点数
short cxInch = pDC- >GetDeviceCaps(LOGPIXELSX);
short cyInch = pDC- >GetDeviceCaps(LOGPIXELSY);

// 取当前字体大小
CFont *curFont = pDC- >GetCurrentFont();
LOGFONT curLogFont;
LOGFONT newLogFont;

curFont- >GetLogFont( &curLogFont );
long NewFontWidth = curLogFont.lfWidth;
long NewFontHeight = curLogFont.lfHeight;
newLogFont = curLogFont;

//计算新的字体大小--缩小一倍
newLogFont.lfWidth =(long)((float)NewFontWidth/2.0
* ((float)cxInch / 72.0));
newLogFont.lfHeight =(long)((float)NewFontHeight/2.0
* ((float)cyInch / 72.0)); 

//创建并设置新的字体，保留以前的字体
CFont newFont;
CFont *oldFont;

newFont.CreateFontIndirect(&newLogFont);
oldFont = pDC- >SelectObject(&newFont );
/////////////////////////////////
//根据字体宽度、高度计算
//每行最大字数及每页最大行数

//取打印纸张高度和宽度
int nPageHeight, nPageWidth;
nPageHeight = pDC- >GetDeviceCaps(VERTRES);
nPageWidth = pDC- >GetDeviceCaps(HORZRES);

TEXTMETRIC TextM;
pDC- >GetTextMetrics(&TextM);
//字体高度
m_LineHeight = (unsigned short)TextM.tmHeight;
//字体平均宽度
m_CharWidth=(unsigned short)
TextM.tmAveCharWidth;

//每行最大字数
m_MaxLineChar = nPageWidth / m_CharWidth - 8; 
//每页最大行数
m_LinesPerPage = nPageHeight/ m_LineHeight; 
//根据每行最大字数对文字进行重新调整
RedealTextData(); 
//////////////////////////////////////
//计算所需打印纸张数目
int nPrintableLineCount = INT_MAX/m_LineHeight;

// m_lines为文件总行数
if (m_lines < nPrintableLineCount)
nPrintableLineCount = m_lines;
unsigned short MaxPage = (nPrintableLineCount
+ m_LinesPerPage - 1) 
/ m_LinesPerPage;
//设置所需打印纸张数目
pInfo- >SetMaxPage(MaxPage);
pInfo- >m_nCurPage = 1;

//////////////////////////////////////////
//最后不要忘记将字体还原，这一句是必需的
pDC- >SelectObject(oldFont );
}
---- RedealTextData 函数根据每行最大宽度对文件进行重新调整Ｖ饕羌扑阄募忻啃械目矶龋绻畲罂矶仍蚣尤牖恍蟹?x0d,0x0a）。函数实现如下：

//=======================================
// RedealTextData
//注：
//pDoc- >buffer为文件缓冲区
//pDoc- >file_length为文件字节长度
//pDoc- >TextLines为文件原行数
//pDoc- >MaxLineLength为文件原最大行字节宽度
//=======================================
void CTextView::RedealTextData()
{
CDocViewDoc* pDoc = GetDocument();
ASSERT_VALID(pDoc);

short LineLengthMax = m_MaxLineChar;
unsigned short lines=0;
unsigned long i,j;

//申请新的缓冲区保存调整后的文件
long size = pDoc- >file_length + pDoc- >TextLines*
(pDoc- >MaxLineLength/m_MaxLineChar+1);
m_newBuffer = new char [size ];
LPSTR newTempPtr = m_newBuffer;
m_file_length =pDoc- >file_length;
//保存文件新的行数
m_lines = 1;
i = 0;
//记录当前行的宽度
short theLineLength=0; 
//记录当前行中汉字字节数，
//以防止将一半汉字分为两行
unsigned short halfChinese=0;

while(i < pDoc- >file_length)
{
*newTempPtr++ = pDoc- >buffer[i];

j=i+1;
if( (pDoc- >buffer[i] == 0x0d && pDoc- >buffer[j] == 0x0a))
{
m_lines++;
theLineLength = 0;
}
else
{
//如果是TAB字符，宽度加8
if(pDoc- >buffer[i] == VK_TAB)
theLineLength += 8;
else 
{
//大于0xa1的字节为汉字字节
if((unsigned char)pDoc- >buffer[i] >= 0xa1)
halfChinese++;
theLineLength++;
}
//如果行宽大于每行最大宽度，进行特殊处理
if(theLineLength > LineLengthMax)
{
char buff[256];
short m=255;
newTempPtr--;
if((unsigned char )*newTempPtr < 0xa1)
{
//如果当前字符的前一个字符是数字、
//字母或一些特殊的前置符号时，
//指针循环向前取，
//以防止将一个单词分为两行。
while((*newTempPtr >=0 && *newTempPtr< =9)||
(*newTempPtr >=a && *newTempPtr < = z) ||
(*newTempPtr >=A && *newTempPtr < = Z) ||
*newTempPtr == _ || *newTempPtr == * ||
*newTempPtr == ^ || *newTempPtr == ~ )
buff[m--] = *newTempPtr--;
}
else //汉字
{ 
//防止将一个汉字分为两行。
if(halfChinese%2)
buff[m--] = *newTempPtr--;
}
newTempPtr++;
//加入换行符，分为两行
*newTempPtr++ = 0x0d;
*newTempPtr++ = 0x0a;
for(short k=m+1; k< 256; k++)
*newTempPtr++ = buff[k];

m_lines++;
theLineLength = 0;
m_file_length += 2;
}
}
i++;
}
}
---- 三、实现OnPrint 函数

---- 在OnPrint 函数中实现真正的文字输出，主要功能包括设置打印字体大小，计算当前页号文字输出位置，以及文字的输出打印。

---- 程序中首先计算打印区域，文字在这个打印区域内输出。然后设置新的打印字体。

---- 由于OnPrint 函数是每打印一页被调用一次，所以需要根据当前打印页号计算出当前页的文字在整个文字缓冲区的起始偏移量和终止偏移量。这里程序中调用了函数GetOffset()，此函数在下面介绍。

---- 最后调用Windows 的DrawText() 函数实现文字的输出。

OnPrint()函数实现如下：
//====================================
// OnPrint
//========================================
void CTestView::OnPrint(CDC* pDC, 
CPrintInfo* pInfo) 
{
//计算打印区域//////////////////
long yTopOfPage =(pInfo- >m_nCurPage -1) * 
m_LinesPerPage * m_LineHeight;

//左边空出两个字符宽度
pDC- >SetViewportOrg(m_CharWidth * 2, 
-yTopOfPage);

int nPageWidth = pDC- >GetDeviceCaps(HORZRES);
CRect rectClip = CRect(0, 
yTopOfPage, 
nPageWidth,
yTopOfPage + m_LinesPerPage * 
m_LineHeight);

/////设置缩小字体///////////////////
//取打印机的横方向和纵方向的分辨率
//即每英寸点数
short cxInch=pDC- >GetDeviceCaps(LOGPIXELSX);
short cyInch= DC- >GetDeviceCaps(LOGPIXELSY);

//取当前字体大小
CFont *curFont = pDC- >GetCurrentFont();
LOGFONT curLogFont;
LOGFONT newLogFont;

curFont- >GetLogFont( &curLogFont );
long NewFontWidth = curLogFont.lfWidth;
long NewFontHeight = curLogFont.lfHeight;
newLogFont = curLogFont;

//计算新的字体大小--缩小一倍
newLogFont.lfWidth = (long)((float)NewFontWidth/2.0 
* ((float)cxInch / 72.0));
newLogFont.lfHeight = (long)((float)NewFontHeight/2.0 
* ((float)cyInch / 72.0)); 

//创建并设置新的字体，保留以前的字体
CFont newFont;
CFont *oldFont;
newFont.CreateFontIndirect(&newLogFont);
oldFont = pDC- >SelectObject(&newFont );

/////文字打印输出/////////////////
unsigned short CurrentStartLine ,
CurrentEndLine;
long StartPrintOffset, 
EndPrintOffset, 
PrintSize;
LPSTR tempPtr;
RECT rect1,rect2;
//根据当前打印页号计算文字起始行
CurrentStartLine=(pInfo- >m_nCurPage-1) * m_LinesPerPage;
//文字终止行
CurrentEndLine = CurrentStartLine+m_LinesPerPage;

if(CurrentEndLine > m_lines)
CurrentEndLine = m_lines;
//计算打印文字的起始位置和终止位置
StartPrintOffset=GetOffset(m_newBuffer,
m_file_length, CurrentStartLine);
EndPrintOffset = GetOffset(m_newBuffer,
m_file_length,CurrentEndLine);

PrintSize = EndPrintOffset - StartPrintOffset;

tempPtr = m_newBuffer + StartPrintOffset;
//文字输出
pDC- >DrawText(tempPtr, PrintSize,
&rectClip,
DT_NOCLIP |DT_NOPREFIX
|DT_EXPANDTABS);

//还原旧的打印字体
pDC- >SelectObject(oldFont );
}
---- 程序中的GetOffset 函数是根据给定的行号计算文字的位置，其实现如下：

//========================================
// GetOffset () 
//========================================
long CTestView::GetOffset(LPSTR buffer, 
long buffer_length, 
unsigned short StartLine)
{
if(StartLine == 0) return 0;

unsigned short lines=0;
long i,j;

i = 0;
while(i < buffer_length)
{
j=i+1;
if( (buffer[i++] == 0x0d && buffer[j] == 0x0a))
{
lines++;
if(lines == StartLine)
return i;
}
}
return buffer_length;
}
---- 以上是本人在编程中的一点心得，欢迎和大家共同交流。

