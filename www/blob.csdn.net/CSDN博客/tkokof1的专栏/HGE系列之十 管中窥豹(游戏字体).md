# HGE系列之十 管中窥豹(游戏字体) - tkokof1的专栏 - CSDN博客

2011年03月13日 19:45:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：2705


HGE系列之十 管中窥豹(游戏字体)

对于一款游戏引擎来说，支持显示**字体**自然是必备的功能，HGE内建的字体功能虽然仅支持一般的位图字体，但是也算是简洁明了，这次的HGE源码之旅就让我们来看一看他的各中实现：）

**类名** ：hgeFont 

**功能** ：字体类

**头文件** ：hge/hge181/include/hgeFont.h 

**实现文件** ：hge/hge181/src/helpers/hgeFont.cpp

整个hge字体的功能支持皆实现与此，让我们依照惯例来看一看他的头文件声明：

**class** hgeFont

{

**public:**

// 构造函数，注意参数

hgeFont(const char *filename, bool bMipmap=false);

~hgeFont();

// 渲染函数

voidRender(float x, float y, int align, const char *string);

// format string渲染

voidprintf(float x, float y, int align, const char *format, ...);

// format string 矩形区域渲染

voidprintfb(float x, float y, float w, float h, int align, const char *format, ...);

// 设置字体颜色

voidSetColor(DWORD col);

// 设置字体“深度”（Z缓存）

voidSetZ(float z);

// 设置混合模式

voidSetBlendMode(int blend);

// 设置缩放

voidSetScale(float scale) {fScale=scale;}

// 设置比例（宽度缩放值）

voidSetProportion(float prop) { fProportion=prop; }

// 设置旋转

voidSetRotation(float rot) {fRot=rot;}

// 设置字体间距

voidSetTracking(float tracking) {fTracking=tracking;}

// 设置字体行距

voidSetSpacing(float spacing) {fSpacing=spacing;}

// 获取颜色

DWORDGetColor() const {return dwCol;}

// 获取“深度”

floatGetZ() const {return fZ;}

// 获取混合模式

intGetBlendMode() const {return nBlend;}

// 获取缩放值

floatGetScale() const {return fScale;}

// 获取宽度比例

floatGetProportion() const { return fProportion; }

// 获取旋转

floatGetRotation() const {return fRot;}

// 获取字体间距

floatGetTracking() const {return fTracking;}

// 获取行距

floatGetSpacing() const {return fSpacing;}

// 获取字体精灵 

hgeSprite*GetSprite(char chr) const { return letters[(unsigned char)chr]; }

// 获取先前宽度

floatGetPreWidth(char chr) const { return pre[(unsigned char)chr]; }

// 获取之后宽度

floatGetPostWidth(char chr) const { return post[(unsigned char)chr]; }

// 获取高度

floatGetHeight() const { return fHeight; }

// 获取字符串宽度

floatGetStringWidth(const char *string, bool bMultiline=true) const;

**private:**

// 私有化构造函数和赋值函数，已达到禁用目的

hgeFont();

hgeFont(const hgeFont &fnt);

hgeFont&operator= (const hgeFont &fnt);

// 以下为实现的一些细节

char*_get_line(char *file, char *line);

static HGE*hge;

static charbuffer[1024];

HTEXTUREhTexture;

hgeSprite*letters[256];

floatpre[256];

floatpost[256];

floatfHeight;

floatfScale;

floatfProportion;

floatfRot;

floatfTracking;

floatfSpacing;

DWORDdwCol;

floatfZ;

intnBlend;

};

可以看到，虽然hgeFont本身并未有多少复杂，但是仍然提供了不少方便字体操控的函数功能，接下来就让我们细细的翻看翻看：

首先请让我们跳过hgeFont的构造函数（原因后面再提），先来看看hgeFont的析构函数：

hgeFont::~hgeFont()

{

// 依次释放letters精灵数组（最大256个）

// 每一个精灵即代表一个字符

for(int i=0; i<256; i++)

if(letters[i]) delete letters[i];

// 释放字体贴图

if(hTexture) hge->Texture_Free(hTexture);

// 释放hge引用

hge->Release();

}

看来没有什么特别的地方，好的，让我们继续往下看：

// 字体渲染

void hgeFont::Render(float x, float y, int align, const char *string)

{

int i;

floatfx=x;

// 首先将align位与HGETEXT_HORZMASK（水平掩码）

align &= HGETEXT_HORZMASK;

// 如果对齐方式为靠右，修正fx的坐标值

if(align==HGETEXT_RIGHT) fx-=GetStringWidth(string, false);

// 如果对齐方式为居中，修正fx的坐标值

if(align==HGETEXT_CENTER) fx-=int(GetStringWidth(string, false)/2.0f);

// 当前字符不为空 (/0)

while(*string)

{

// 如果当前为换行符

if(*string=='/n')

{

// 更行y坐标，注意计算公式，为 **高度*缩放比例*行距比例**

y += int(fHeight*fScale*fSpacing);

fx = x;

// 根据对齐方式继续修正fx坐标

if(align == HGETEXT_RIGHT)fx -= GetStringWidth(string+1, false);

if(align == HGETEXT_CENTER) fx -= int(GetStringWidth(string+1, false)/2.0f);

}

Else// 其他字符

{

// 获取当前字符值

i=(unsigned char)*string;

// 如果当前精灵字符数组中找不到，便以 '?' 代替

if(!letters[i]) i='?';

if(letters[i])

{

// 更新fx坐标，注意计算公式，为 **前坐标*缩放比例*字宽比例**

fx += pre[i]*fScale*fProportion;

// 调用精灵（hgeSprite，可以参看[这里](http://blog.csdn.net/tkokof1/archive/2011/01/02/6112995.aspx)）提供的渲染函数进行渲染

letters[i]->RenderEx(fx, y, fRot, fScale*fProportion, fScale);

// 渲染之后继续更新fx坐标，以正确渲染下一字符

// 注意计算公式，为 **（字宽+后位移+间距）*缩放*宽比**

fx += (letters[i]->GetWidth()+post[i]+fTracking)*fScale*fProportion;

}

}

// 更新至下一个字符的处理

string++;

}

}

相关的实现本身并不复杂，但是仍然有几点值的一看：例如HGETEXT_HORZMASK这个掩码的使用，以及各个渲染坐标的更新方式等等，需要我们悉心关注一下。

接着让我们来看看hgeFont的printf函数：

void hgeFont::printf(float x, float y, int align, const char *format, ...)

{

// 获取可变参数的起始位置

char*pArg=(char *) &format+sizeof(format);

// 使用_vsnprintf将格式化字符串打印至buffer中

_vsnprintf(buffer, sizeof(buffer)-1, format, pArg);

// 将字符串最后一位置空

buffer[sizeof(buffer)-1]=0;

//vsprintf(buffer, format, pArg);

// 调用自身的Render函数进行渲染

Render(x,y,align,buffer);

}

_vsnprintf是C语言中用以支持可变参数的库函数之一，不太熟悉的朋友可以参看一下[这里](http://msdn.microsoft.com/en-us/library/1kt27hek.aspx)：)

让我们接着看看printfb的相关实现：

void hgeFont::printfb(float x, float y, float w, float h, int align, const char *format, ...)

{

charchr, *pbuf, *prevword, *linestart;

inti,lines=0;

floattx, ty, hh, ww;

// 取得可变参数起始位置

char*pArg=(char *) &format+sizeof(format);

// 使用_vsnprintf将格式化字符串打印至buffer中

_vsnprintf(buffer, sizeof(buffer)-1, format, pArg);

// 将字符串最后一位置空

buffer[sizeof(buffer)-1]=0;

//vsprintf(buffer, format, pArg);

linestart=buffer;

pbuf=buffer;

prevword=0;

for(;;)

{

i=0;

// 寻找下一个**空格**或者**换行**符号

while(pbuf[i] && pbuf[i]!=' ' && pbuf[i]!='/n') i++;

// 存储该字符

chr=pbuf[i];

// 将原字符位置置空

pbuf[i]=0;

// 重新获取字符串长度

ww=GetStringWidth(linestart);

// 重新置回原字符

pbuf[i]=chr;

// 如果当前字符串的长度大于所给宽度参数（w）

if(ww > w)

{

// 如果pbuf指向字符串首

if(pbuf==linestart)

{

// 置当前位置为换行符

pbuf[i]='/n';

// 更新行首指针

linestart=&pbuf[i+1];

}

else

{

// 将前字符置为换行符

*prevword='/n';

// 更新行首指针

linestart=prevword+1;

}

// 递增行数

lines++;

}

// 如果当前字符串为换行符

if(pbuf[i]=='/n')

{

// 设置前字符为当前位置

prevword=&pbuf[i];

// 设置行起始为当前+1位置

linestart=&pbuf[i+1];

// pbuf为当前+1位置

pbuf=&pbuf[i+1];

// 递增行数

lines++;

// 回到循环起始（**for(;;)**），继续执行

continue;

}

// 如果当前字符位置为空，则递增行数，并跳出循环

if(!pbuf[i]) {lines++;break;}

// 否则更新前字符为当前位置，pbuf为当前+1位置

prevword=&pbuf[i];

pbuf=&pbuf[i+1];

}

tx=x;

ty=y;

// 计算高度，注意计算公式，为 **字体高度*行距比例*缩放比例*行数**

hh=fHeight*fSpacing*fScale*lines;

// 根据对齐方式调整渲染坐标

switch(align & HGETEXT_HORZMASK)

{

case HGETEXT_LEFT: break;

case HGETEXT_RIGHT: tx+=w; break;

case HGETEXT_CENTER: tx+=int(w/2); break;

}

switch(align & HGETEXT_VERTMASK)

{

case HGETEXT_TOP: break;

case HGETEXT_BOTTOM: ty+=h-hh; break;

case HGETEXT_MIDDLE: ty+=int((h-hh)/2); break;

}

// 调用自身Render函数完成最后的渲染

Render(tx,ty,align,buffer);

}

printfb的思路基本上类同于先前的printf，只是在其基础上根据给定的矩形渲染范围做一些渲染坐标上的调整，最后的渲染也都是转给自己的Render，流程上还是相当清晰的 ：）

说完了这些，让我们再来看看hgeFont实现的几个辅助函数，首先便是GetStringWidth：

// 获取字符串宽度

float hgeFont::GetStringWidth(const char *string, bool bMultiline) const

{

int i;

float linew, w = 0;

// 当前字符不为空

while(*string)

{

linew = 0;

// 当字符不为空并且不为换行符时

while(*string && *string != '/n')

{

// 获取当前字符

i=(unsigned char)*string;

// 如果为未定义字符则以' ?' 代替

if(!letters[i]) i='?';

// 如果当前字符存在

if(letters[i])

// 递增行宽，注意递增公式，为 **字体宽度+前位移+后位移+字体间距**

linew += letters[i]->GetWidth() + pre[i] + post[i] + fTracking;

// 转至下一个字符继续处理

string++;

}

// 如果不处理多行情况，则直接返回第一行行宽，计算公式为 **行宽*缩放*宽比**

if(!bMultiline) return linew*fScale*fProportion;

// 否则更新最长行宽

if(linew > w) w = linew;

// 当前字符为换行符或者回车符时，则跳过

while (*string == '/n' || *string == '/r') string++;

}

// 返回最长行宽

return w*fScale*fProportion;

}

不难看出，该函数的作用便是取得给定字符串的行宽，并且考虑了多行情况。

接下来的Set* 函数并未有多少内容，基本思想便是设置成员参数，并依次设置hgeFont内部的256个字符精灵，具体实现在此不再赘述，有兴趣的朋友可以参看源码。

最后，让我们来瞅一瞅先前被我故意跳过的hgeFont构造函数，首先来看一看其中用到的一个辅助函数：_get_line

char *hgeFont::_get_line(char *file, char *line)

{

int i=0;

// 如果当前字符为空，则返回0

if(!file[i]) return 0;

// 当前字符不为空，并且不为换行符，回车符

while(file[i] && file[i]!='/n' && file[i]!='/r')

{

// 将fine中对应字符拷贝至line中

line[i]=file[i];

i++;

}

// line末尾置空

line[i]=0;

// 跳过换行符以及回车符

while(file[i] && (file[i]=='/n' || file[i]=='/r')) i++;

// 返回file的下一字符串位置

return file + i;

}

通过代码不难看出，该函数的作用其实就是从给定字符串中提取由/n,/r分隔的字串，平心而论，该段代码似乎有些重造车轮，因为CRT中的strtok也可用以完成同样的工作（当然如果考虑到多线程环境的话就不尽然了：）），再者该函数的命令以下划线起头，这点也令我不是特别舒服，因为很容易造成与C/C++库函数的冲突。（btw：上面的printf/printfb的命名其实我也觉得不是很妥，内部使用的‘256’这个魔数也应该至少用个const或者#define包装一下...）

废话了不少，其实以上提及的这些问题也多是一些瑕疵，让我真正觉得确实需要改正的倒是hgeFont的构造函数，相关的原因[之前](http://blog.csdn.net/tkokof1/archive/2010/06/07/5652143.aspx)我也有所提及，在此就不再讲述了，其实现的源码讲解我也在此略去，仅讲一讲其间实现的功能，有兴趣的朋友可以自行查看相应源码：

hgeFont的构造函数其实是实现了一个特定文本文件格式的完整解析，该文件的名字便是hgeFont构造函数的第一个参数，而该定义文件的格式则基本如下所示：

// 首先是字段定义，必须为HGEFONT，大小写一致

[HGEFONT]

// 接着定义字体贴图文件路径，为字体定义文件（即本文件）的相对路径

// 注意，该贴图文件必须包含你所要定义的所有字符

Bitmap=font_bitmap.png

// 定义各个字符，相关格式如下：

// Char = "字符",x坐标，y坐标，宽度，高度，前位移，后位移

Char=" ",1,1,3,30,-1,4

Char="!",5,1,7,30,1,0

Char=""",13,1,8,30,3,2

// "字符" 也可以使用十六进制数表示，当然，大小上不要超过255（FF）

Char=FE,445,187,17,30,0,0

Char=FF,463,187,16,30,-2,-1

至于hgeFont构造函数的第二个参数是与渲染效率相关的mipmap，有兴趣的朋友可以从[这里](http://en.wikipedia.org/wiki/Mipmap)开始了解，另外一提的是，由于需要支持mipmap的关系，字体贴图大小必须为2的幂次，譬如64*64，使用时需要注意一下。

当然，手工编写这个定义文件（默认后缀名为fnt）还是相当费劲的，所以HGE提供了一个简单的工具来帮助你完成这件事情，他便是**fonted**，位置位于：

hge181/tools/fonted/fonted.exe

有兴趣的朋友可以自己捣鼓捣鼓，其相应的代码实现在这里可以在这里找到：

hge/hge181/src/fonted/...

好了，hgeFont的讲解就到此结束吧，有兴趣的朋友可以继续了解，如果弄出了什么好看的字体，或是做出了什么功能上的改进，到时一定要通知一下啊：）

OK，那么下次再见吧！

