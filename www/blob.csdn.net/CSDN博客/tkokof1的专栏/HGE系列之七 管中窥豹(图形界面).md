# HGE系列之七 管中窥豹(图形界面) - tkokof1的专栏 - CSDN博客

2010年08月31日 07:22:00[tkokof1](https://me.csdn.net/tkokof1)阅读数：4058


    HGE系列之七管中窥豹(图形界面)

这次的HGE源码之旅，让我们来看看HGE的图形用户界面（GUI）的实现，话说电脑技术发展至今，当年轰动一时的图形用户界面，而今早已司空见惯，想来不得不感叹一下技术的日新月异啊……HGE作为一款出色的2D游戏引擎，GUI方面的支持自然不在话下，并且从实现方面来说还是相当具有扩展性的：）好了，简略的介绍就到此为止吧，就让我们马上来看看源码：）

**类名**：hgeGUIObject

**功能**：界面对象基类，像文本、按钮之类的GUI控件皆派生于她

**头文件**：hge/hge181/include/hgegui.h

**实现文件**：无

以下是该类的头文件声明，注意一下其中的各个虚拟函数：

class hgeGUIObject

{

public:

// 默认的构造函数，用以创建hge对象以及设置颜色(color)

hgeGUIObject(){ hge=hgeCreate(HGE_VERSION); color=0xFFFFFFFF; }

// 虚拟析构函数，用以释放之前创建的HGE

virtual~hgeGUIObject() { hge->Release(); }

// 纯虚函数Render，功能顾名思义了：)

virtual voidRender() = 0;

// 根据间隔时间（dt）来更新内部状态

virtual voidUpdate(float dt) {}

// “进入”此界面对象时执行的函数

virtual voidEnter() {}

// “离开”此界面对象时执行的函数

virtual voidLeave() {}

// 重置界面对象状态

virtual voidReset() {}

// 是否Enter或者Leave的调用已经结束

virtual boolIsDone() { return true; }

// 当控件获取或失去焦点时调用

virtual voidFocus(bool bFocused) {}

// 当鼠标进入或者离开控件范围时调用

virtual voidMouseOver(bool bOver) {}

// 响应鼠标移动

virtual boolMouseMove(float x, float y) { return false; }

// 响应鼠标左键

virtual boolMouseLButton(bool bDown) { return false; }

// 响应鼠标右键

virtual boolMouseRButton(bool bDown) { return false; }

// 响应鼠标滑轮

virtual boolMouseWheel(int nNotches) { return false; }

// 响应按键

virtual boolKeyClick(int key, int chr) { return false; }

// 设置控件颜色

virtual voidSetColor(DWORD _color) { color=_color; }

// 控件id

intid;

// 是否静态

boolbStatic;

// 是否可见

boolbVisible;

// 是否使能

boolbEnabled;

// 区域范围，使用了hgeRect ：）

hgeRectrect;

// 控件颜色，简单的使用了一个DWORD进行表示

DWORDcolor;

// hgeGUI对象，用于管理hgeGUIObject，详述见后

hgeGUI*gui;

// 控件链表后向指针

hgeGUIObject*next;

// 控件链表前向指针

hgeGUIObject*prev;

protected:

// 将复制构造函数及对应的赋值函数定义为保护类型，

// 禁止外部的调用（子类和自身成员函数除外）

hgeGUIObject(const hgeGUIObject &go);

hgeGUIObject&operator= (const hgeGUIObject &go);

// 静态hge指针，用于获取hge提供的核心（core）函数

static HGE*hge;

};

一目了然，这个控件基类还是相对简单的，各种成员函数和变量的意义都十分清晰，借助于上面的注释和HGE文档的帮助，相信大家都能够了其大意，唯一想再说一说的，只是这么一段：

// 将复制构造函数及对应的赋值函数定义为保护类型，

// 禁止外部的调用（子类和自身成员函数除外）

hgeGUIObject(const hgeGUIObject &go);

hgeGUIObject&operator= (const hgeGUIObject &go);

正如注释所解，这段代码的作用是屏蔽外界对于这两个函数的调用，但是作者在这方面还是做的不是特别彻底，因为将其设置为保护域（protected）并不能防止派生类对其的不当调用，虽然由于这两个函数源代码中只是进行了声明，并未进行实现，这意味着即使派生类中调用了这两个函数，也总会遇到链接错误（Link），但是我想为什么不直接做的更彻底一点呢？很简单，将保护域（protected）声明为私有域（private）就OK了，当然最后的

static HGE*hge;

前还是要再加一个“protected：”，否则派生类就用不了hge了（至少不能直接使用）：）

**类名**：hgeGUI

**功能**：界面控件管理类，用以控制各个界面

**头文件**：hge/hge181/include/hgegui.h

**实现文件**：hge/hge181/src/helpers/hgegui.cpp

同样的步骤，让我们想看一看该类的声明：

class hgeGUI

{

public:

hgeGUI();

~hgeGUI();

// 添加控件

voidAddCtrl(hgeGUIObject *ctrl);

// 移除控件

voidDelCtrl(int id);

// 通过控件id获取相对应控件

hgeGUIObject*GetCtrl(int id) const;

// 移动控件

voidMoveCtrl(int id, float x, float y);

// 设置控件的显示或隐藏

voidShowCtrl(int id, bool bVisible);

// 使能控件

voidEnableCtrl(int id, bool bEnabled);

// 设置控件的显示模式

voidSetNavMode(int mode);

// 设置鼠标光标

voidSetCursor(hgeSprite *spr);

// 设置所有控件的颜色

voidSetColor(DWORD color);

// 设置控件的失焦获焦状态

voidSetFocus(int id);

// 获取控件的失焦获焦状态

intGetFocus() const;

// 开启GUI“进入”动画

voidEnter();

// 开启GUI“离开”动画

voidLeave();

// 重置控件状态

voidReset();

// 移动所有控件

voidMove(float dx, float dy);

// 更新控件状态

intUpdate(float dt);

// 绘制控件

voidRender();

private:

// 私有化复制构造函数和赋值函数，防止外界调用

hgeGUI(const hgeGUI &);

hgeGUI&operator= (const hgeGUI&);

// 处理控件逻辑

boolProcessCtrl(hgeGUIObject *ctrl);

static HGE*hge;

// 控件链表（双向）表头

hgeGUIObject*ctrls;

// 指向被锁定的控件

hgeGUIObject*ctrlLock;

// 指向获的焦点的控件

hgeGUIObject*ctrlFocus;

// 指向鼠标移至其上的控件

hgeGUIObject*ctrlOver;

// 显示模式

intnavmode;

// Enter/Leave 标记

intnEnterLeave;

// 鼠标光标

hgeSprite*sprCursor;

// 鼠标x、y坐标

floatmx,my;

// 鼠标滑轮移动值

intnWheel;

// 鼠标左键是否按下及释放

boolbLPressed, bLReleased;

// 鼠标右键是否按下及释放

boolbRPressed, bRReleased;

};

可以看到。hgeGUI的代码也依然十分清晰，在此也不必完整列出实现的所有源码，挑选一些值得注意的成员函数实现我想便足矣足矣：

首先是构造函数，很简单，创建HGE并初始化成员变量，不过如果使用成员初始化列表的话效率会略高一些：）

hgeGUI::hgeGUI()

{

hge=hgeCreate(HGE_VERSION);

ctrls=0;

ctrlLock=0;

ctrlFocus=0;

ctrlOver=0;

navmode=HGEGUI_NONAVKEYS;

bLPressed=bLReleased=false;

bRPressed=bRReleased=false;

nWheel=0;

mx=my=0.0f;

nEnterLeave=0;

sprCursor=0;

}

析构函数也很平常，除了释放资源之外再无其他：

hgeGUI::~hgeGUI()

{

hgeGUIObject *ctrl=ctrls, *nextctrl;

// 依次释放链表（双向）元素

while(ctrl)

{

nextctrl=ctrl->next;

delete ctrl;

ctrl=nextctrl;

}

// 释放hge

hge->Release();

}

接着让我们看看AddCtrl ：

void hgeGUI::AddCtrl(hgeGUIObject *ctrl)

{

hgeGUIObject *last=ctrls;

// 设置控件的hgeGUI指针

ctrl->gui=this;

// 如果指针链表为空，则设置指针链表

if(!ctrls)

{

ctrls=ctrl;

ctrl->prev=0;

ctrl->next=0;

}

// 否则，将控件添加至链表尾部

else

{

while(last->next) last=last->next;

last->next=ctrl;

ctrl->prev=last;

ctrl->next=0;

}

}

同样道理，void hgeGUI::DelCtrl(int id) 也便是依次查找对应id的控件，并删除之。

而 hgeGUIObject* hgeGUI::GetCtrl(int id) const 这个成员变量则更加简单，依次查找并返回控件指针，如果未有找到，则返回NULL。

移动控件的函数也并不困难：

void hgeGUI::MoveCtrl(int id, float x, float y)

{

// 首先获取控件的指针

hgeGUIObject *ctrl=GetCtrl(id);

// 将控件的区域范围设置为 x、y 坐标为起点

ctrl->rect.x2=x + (ctrl->rect.x2 - ctrl->rect.x1);

ctrl->rect.y2=y + (ctrl->rect.y2 - ctrl->rect.y1);

ctrl->rect.x1=x;

ctrl->rect.y1=y;

}

之后的一些设置函数相对简单，仅是赋值而已，在此不再罗列。有兴趣的朋友可以看一看源文件：）

Reset函数将控件链表中的元素依次重置：

void hgeGUI::Reset()

{

hgeGUIObject *ctrl=ctrls;

while(ctrl)

{

ctrl->Reset();

ctrl=ctrl->next;

}

ctrlLock=0;

ctrlOver=0;

ctrlFocus=0;

}

Move函数则是将所有控件依次移动dx、dy位移：

void hgeGUI::Move(float dx, float dy)

{

hgeGUIObject *ctrl=ctrls;

while(ctrl)

{

ctrl->rect.x1 += dx;

ctrl->rect.y1 += dy;

ctrl->rect.x2 += dx;

ctrl->rect.y2 += dy;

ctrl=ctrl->next;

}

}

让我们再来看看SetFoucus函数：

void hgeGUI::SetFocus(int id)

{

// 获取给定id的控件指针

hgeGUIObject *ctrlNewFocus=GetCtrl(id);

// 如果当前的控件已经是焦点控件，则立即返回

if(ctrlNewFocus==ctrlFocus) return;

// 如果获取的控件为空

if(!ctrlNewFocus)

{

// 则将原先的焦点控件失焦

if(ctrlFocus) ctrlFocus->Focus(false);

// 并将焦点控件置空

ctrlFocus=0;

}

// 否则（找到了），如果空间不是静态的，并且可见、使能

else if(!ctrlNewFocus->bStatic && ctrlNewFocus->bVisible && ctrlNewFocus->bEnabled)

{

// 原先的焦点控件失焦

if(ctrlFocus) ctrlFocus->Focus(false);

// 当前的控件获焦

if(ctrlNewFocus) ctrlNewFocus->Focus(true);

// 设置焦点控件指针

ctrlFocus=ctrlNewFocus;

}

}

看来代码依然十分清晰明了：）

接着便是Enter和Leave函数：

void hgeGUI::Enter()

{

hgeGUIObject *ctrl=ctrls;

// 依次调用控件链表中元素的Enter函数

while(ctrl)

{

ctrl->Enter();

ctrl=ctrl->next;

}

// Enter/Leave标记

nEnterLeave=2;

}

void hgeGUI::Leave()

{

hgeGUIObject *ctrl=ctrls;

// 依次调用控件链表中的Leave函数

while(ctrl)

{

ctrl->Leave();

ctrl=ctrl->next;

}

ctrlFocus=0;

ctrlOver=0;

ctrlLock=0;

// Enter/Leave标记

nEnterLeave=1;

}

Render函数与之类似，亦是遍历并渲染的那一套，在此就不列出代码了：）

用于处理控件的ProcessCtrl如下：

bool hgeGUI::ProcessCtrl(hgeGUIObject *ctrl)

{

bool bResult=false;

// 处理鼠标左键按下

if(bLPressed){ ctrlLock=ctrl;SetFocus(ctrl->id);bResult=bResult || ctrl->MouseLButton(true); }

// 处理鼠标右键按下

if(bRPressed){ ctrlLock=ctrl;SetFocus(ctrl->id);bResult=bResult || ctrl->MouseRButton(true); }

// 处理鼠标左键释放

if(bLReleased){ bResult=bResult || ctrl->MouseLButton(false); }

// 处理鼠标右键释放

if(bRReleased){ bResult=bResult || ctrl->MouseRButton(false); }

// 处理鼠标滑轮

if(nWheel){ bResult=bResult || ctrl->MouseWheel(nWheel); }

// 最后处理鼠标的移动

bResult=bResult || ctrl->MouseMove(mx-ctrl->rect.x1,my-ctrl->rect.y1);

// 只要前面有一个消息处理成功，则bResult为真

return bResult;

}

注释中基本道明了这个函数的处理流程和作用，最后，让我们来看一看hgeGUI的Update函数，不像之前遇到的成员函数，这可是个大家伙，所以做好准备了：

首先，他获取鼠标的各个按键情况：

// 更新鼠标变量

hge->Input_GetMousePos(&mx, &my);

bLPressed= hge->Input_KeyDown(HGEK_LBUTTON);

bLReleased = hge->Input_KeyUp(HGEK_LBUTTON);

bRPressed= hge->Input_KeyDown(HGEK_RBUTTON);

bRReleased = hge->Input_KeyUp(HGEK_RBUTTON);

nWheel=hge->Input_GetMouseWheel();

接着，其遍历控件链表并依次调用各个控件的Update函数，用以更新控件的内部状态：

// Update all controls

ctrl=ctrls;

while(ctrl)

{

ctrl->Update(dt);

ctrl=ctrl->next;

}

然后，处理Enter和Leave

// Handle Enter/Leave

if(nEnterLeave)

{

ctrl=ctrls; bDone=true;

// 查看各个控件的Enter和Leave是否结束

while(ctrl)

{

if(!ctrl->IsDone()) { bDone=false; break; }

ctrl=ctrl->next;

}

// 如果任意一个控件的Enter/Leave 没有结束便返回0

if(!bDone) return 0;

else

{

// 如果nEnterLeave为1（即调用Leave时），则返回-1

if(nEnterLeave==1) return -1;

// 否则返回0

else nEnterLeave=0;

}

}

再接着，便是处理键盘按键：

// Handle keys

key=hge->Input_GetKey();

// 如果显示模式为左右模式并且Left键被按下

// 或者显示模式为上下模式并且Up键被按下

if(((navmode & HGEGUI_LEFTRIGHT) && key==HGEK_LEFT) ||

((navmode & HGEGUI_UPDOWN) && key==HGEK_UP))

{

// 获取焦点控件

ctrl=ctrlFocus;

// 如果焦点控件为空，则尝试使用第一个链表控件元素

if(!ctrl)

{

ctrl=ctrls;

// 如果控件还是为空（此时没有控件），则直接返回0

if(!ctrl) return 0;

}

// 查找位于焦点控件之前的符合显示条件的控件

do {

ctrl=ctrl->prev;

if(!ctrl && ((navmode & HGEGUI_CYCLED) || !ctrlFocus))

{

ctrl=ctrls;

while(ctrl->next) ctrl=ctrl->next;

}

if(!ctrl || ctrl==ctrlFocus) break;

} while(ctrl->bStatic==true || ctrl->bVisible==false || ctrl->bEnabled==false);

// 重新设置焦点控件

if(ctrl && ctrl!=ctrlFocus)

{

if(ctrlFocus) ctrlFocus->Focus(false);

if(ctrl) ctrl->Focus(true);

ctrlFocus=ctrl;

}

}

// 如果显示模式为左右模式并且Right键被按下

// 或者显示模式为上下模式并且Down键被按下

else if(((navmode & HGEGUI_LEFTRIGHT) && key==HGEK_RIGHT) ||

((navmode & HGEGUI_UPDOWN) && key==HGEK_DOWN))

{

ctrl=ctrlFocus;

if(!ctrl)

{

ctrl=ctrls;

if(!ctrl) return 0;

// 获取控件链表最后的元素

while(ctrl->next) ctrl=ctrl->next;

}

// 查找位于焦点控件之后的符合显示条件的控件

do {

ctrl=ctrl->next;

if(!ctrl && ((navmode & HGEGUI_CYCLED) || !ctrlFocus)) ctrl=ctrls;

if(!ctrl || ctrl==ctrlFocus) break;

} while(ctrl->bStatic==true || ctrl->bVisible==false || ctrl->bEnabled==false);

// 重新设置焦点控件

if(ctrl && ctrl!=ctrlFocus)

{

if(ctrlFocus) ctrlFocus->Focus(false);

if(ctrl) ctrl->Focus(true);

ctrlFocus=ctrl;

}

}

// 处理其他按键情况

else if(ctrlFocus && key && key!=HGEK_LBUTTON && key!=HGEK_RBUTTON)

{

if(ctrlFocus->KeyClick(key, hge->Input_GetChar())) return ctrlFocus->id;

}

最后则是对于鼠标的处理，使用了之前所见的ProcessCtrl函数，不过私认为这个函数的名字不是太妥当，因为其内部只是处理里鼠标的逻辑，所以称其为ProcessMouseCtrl之类的名字可能更适合，再者，既然单独分离出了一个鼠标控制的函数，那么其他类型的控制为何也不依势分离一下呢？譬如来个ProcessKeyboardCtrl，当然，这仅是一家之言：)

// Handle mouse

bool bLDown = hge->Input_GetKeyState(HGEK_LBUTTON);

bool bRDown = hge->Input_GetKeyState(HGEK_RBUTTON);

// 如果锁定控件不为空

if(ctrlLock)

{

ctrl=ctrlLock;

// 如果鼠标左键和右键都不为空，则置锁定控件为空

if(!bLDown && !bRDown) ctrlLock=0;

// 使用ProcessCtrl处理鼠标按键

if(ProcessCtrl(ctrl)) return ctrl->id;

}

else

{

// Find last (topmost) control

ctrl=ctrls;

if(ctrl)

// 获取最后一个控件元素

while(ctrl->next) ctrl=ctrl->next;

while(ctrl)

{

// 如果鼠标位于控件区域内并且控件使能

if(ctrl->rect.TestPoint(mx,my) && ctrl->bEnabled)

{

// 重新设置ctrlOver控件

if(ctrlOver != ctrl)

{

if(ctrlOver) ctrlOver->MouseOver(false);

ctrl->MouseOver(true);

ctrlOver=ctrl;

}

// 调用ProcessCtrl处理鼠标按键

if(ProcessCtrl(ctrl)) return ctrl->id;

else return 0;

}

// 处理前一个控件

ctrl=ctrl->prev;

}

// 处理完毕之后，将ctrlOver控件置为空

if(ctrlOver) {ctrlOver->MouseOver(false); ctrlOver=0;}

}

呼，终算是将hgeGUI的Update函数讲完了，不过我们还不能休息，因为目前讲述的两个类还不能让我们在屏幕上组织图形界面（不要忘了hgeGUIObject还是个虚基类！），我们必须手工编写派生自hgeGUIObject的子类，并重载那些hgeGUIObject中留下的虚拟函数，看来工作量不小啊……幸而HGE为我们已经编写好了四个派生自hgeGUIObject的子类，他们分别是：hgeGUIText（文本）、hgeGUIButton（按钮）、hgeGUISlider（滚动条）以及hgeGUIListbox（列表框），限于篇幅在此不能全部讲述，但由于其间原理大同小异，所以让我们先细看看其中比较简单的hgeGUIText，相信其他三个的类亦能触类旁通：）

**类名**：hgeGUIText

**功能**：文本类

**头文件**：hge/hge181/include/hgeguictrls.h

**实现文件**：hge/hge181/src/helpers/hgeguictrls.cpp

首先自然是头文件的声明：

class hgeGUIText : public hgeGUIObject

{

public:

hgeGUIText(int id, float x, float y, float w, float h, hgeFont *fnt);

// 设置对其方式

voidSetMode(int _align);

// 设置文本内容

voidSetText(const char *_text);

// 不定参数形式的输出函数

voidprintf(const char *format, ...);

// 渲染函数

virtual voidRender();

private:

// 使用hgeFont进行渲染

hgeFont*font;

// 文本显示的起始坐标

floattx, ty;

// 对其方式

intalign;

// 文本缓冲区

chartext[256];

};

“清澈见底”：）实现也是分外清明：

// 简单的构造函数，还是使用成员初始化列表更好一些：）

hgeGUIText::hgeGUIText(int _id, float x, float y, float w, float h, hgeFont *fnt)

{

id=_id;

bStatic=true;

bVisible=true;

bEnabled=true;

rect.Set(x, y, x+w, y+h);

font=fnt;

tx=x;

// 用于居中显示

ty=y+(h-fnt->GetHeight())/2.0f;

text[0]=0;

}

// 设置对其模式，注意坐标的设置

void hgeGUIText::SetMode(int _align)

{

align=_align;

if(align==HGETEXT_RIGHT) tx=rect.x2;

else if(align==HGETEXT_CENTER) tx=(rect.x1+rect.x2)/2.0f;

else tx=rect.x1;

}

// 仅是一个strcpy，应该加个inline ：）

void hgeGUIText::SetText(const char *_text)

{

strcpy(text, _text);

}

// 使用vsprintf完成任务，同样应该加个inline ：）

void hgeGUIText::printf(const char *format, ...)

{

vsprintf(text, format, (char *)&format+sizeof(format));

}

// 依旧简单，设置颜色，然后渲染！

void hgeGUIText::Render()

{

font->SetColor(color);

font->Render(tx,ty,align,text);

}

好了，至此hge的GUI部分也算是讲解完毕了，一路上并不轻松，但是也未有遇到多大的羁绊，不过相信大家对于HGE的GUI系统应该有了一些认识，另外一提的便是，HGE自带的教程（tutorials）中，tutorial6正好是关于GUI的一个示例，有兴趣的朋友可以看一看：）

OK，就此停笔了，我觉得我说的已经够多，再说一下去自己都要受不了了，不过呢，最后我还是得照例来上一句：下次再见喽：）

