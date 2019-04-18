# 使用C++和Directx开发GUI(一) - gauss的专栏 - CSDN博客
2011年12月05日 12:20:49[gauss](https://me.csdn.net/mathlmx)阅读数：356
看起来我在做发明车轮的浪费时间的事--windows已经有一套非常复杂功能完善的GUI.不幸的是windows的GUI适用于办公软件,而这通常不适合游戏软件.游戏一般需要比windows更精确的控制(例如,使用自己的GUI实现用alpha混合创造的部分透明的窗口比较容易,而使用windows的GUI则几乎做不到).
这篇文章将讲述如何使用C++和DirectX来创建自己的GUI.文章分为几部分,每部分涉及一个特定的GUI编程主题.这并不需要按顺序阅读,所以你可以从你需要的部分开始.
这里我假设读者有以下的背景知识:事件驱动的编程原理;对PDL和C++的熟练掌握.我使用C++来建立我的GUI系统,因为我是C++爱好者俱乐部的持卡会员,并且C++的OOP很适用于windows编程和控制方式.
让我们从定义工作范围开始.应该认识到我们不是要设计windows95,我们只是想要为一个游戏开发一套简单的GUI.所以我们不用实现每一个简单控制和GUI结构.我们只需要几个部分而已:一个鼠标指针,一个普通窗口,一些在窗口中的对话框控制.我们也需要一个资源编辑器--一个允许我们在图形环境中使用拖动控制来设计对话框.
**从基础开始:渲染循环**
我将通过定义一个计算并绘制框线的函数开始.我们在RenderGUI()函数中调用这个函数.RenderGUI用PDL描述如下:
void CApplication::RenderGUI(void)
{
// get position and button status of mouse cursor
// calculate mouse cursor抯 effects on windows / send messages
// render all windows
// render mouse
// flip to screen
}
我们得到新的位置和鼠标光标的状态,计算由新的数据引起的任何改变,渲染所有的窗口,鼠标指针并在屏幕上绘制.
**鼠标**
class CMouse {
public:
CMouse(); // boring
~CMouse(); // boring
int Init(LPDIRECTINPUT di); // we抣l talk about this later
int Refresh(void); // we抣l talk about this later
int GetButton(int index)
{
if (index < 0 || index > NUMMOUSEBUTTONS) return(0);
return(m_button[index]);
}
void clear(void); // sets all vars to zero 
// makes sure p is a valid on-screen point
void ConstrainPosToScreenSize(CPoint &p);
CPoint GetAbsPosition(void) { return(m_absposition); }
CPoint GetRelPosition(void) { return(m_relposition); }
enum { NUMMOUSEBUTTONS = 3 }; // three button mouse
private:
LPDIRECTINPUTDEVICE m_mousedev;
char m_button[NUMMOUSEBUTTONS]; // state of buttons
CPoint m_absposition; // actual screen position
CPoint m_relposition; // relative position
};
**绝对位置和相对位置,DirectInput**
为什么我要使用DirectInput?这实际是一个口味问题.有两种方法可得到windows的鼠标信息,从DirectInput中或通过一条叫做GetCursorPos()的Win32函数.主要的区别在于DirectInput提供的是相对坐标,即相对于上次位置的当前位置;而GetCursorPos()将提供屏幕坐标系的绝对坐标.绝对坐标对GUI很有用;而相对坐标则适合于没有光标的鼠标,例如在FPS游戏中的环视(译者:即光标位置固定的情况).然而你可以由绝对坐标计算出相对坐标,反之亦然.
由于很多原因我选择了DirectInput,所有这些原因超出了本文的范围.对你来说GetCursorPos()可能是个更好的选择?如果是这样,鼠标类可能就会非常庞大.DirectInput更加技巧化(也更有趣),所以本文将集中于DirectInput.
**初始化DirectInput**
在我们详述CMouse之前,让我们看看如何初始化DirectInput.注意这些代码并不属于我们的CMouse::Init()函数;DirectInput的指针在整个游戏中都会使用,而并不仅仅在鼠标类中,所以DirectInput的初始化应在主初始化函数中和DirectDraw,DirectSound的初始化同时完成.DirectInput指针和DirectInput
 device指针不同;DirectInput指针用以得到DirectInput device指针.注意理解这一区别,下面是初始化主DirectInput接口指针的代码
LPDIRECTINPUT di = NULL;
hr = DirectInputCreate(hinst, DIRECTINPUT_VERSION, &di, NULL);
if (FAILED(hr)) {
// error
handle_error ();
}
// Now that we抳e got a DirectInput interface, let抯 begin
// fleshing out our CMouse by implementing CMouse::Init().  
bool CMouse::Init(LPDIRECTINPUT di)
{
// Obtain an interface to the system mouse device.
hr = di->CreateDevice(GUID_SysMouse, (LPDIRECTINPUTDEVICE*)&m_mousedev, NULL); 
if (FAILED(hr)) { /* handle errors! */ }
//
Set the data format to "mouse format".
hr = m_mousedev->SetDataFormat(&c_dfDIMouse);
if (FAILED(hr)) { /* handle errors! */ }
// Set the cooperativity level
hr = m_mousedev->SetCooperativeLevel(hwnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
if (FAILED(hr)) { /* handle errors! */ }
}
这段代码作了三件重要工作.第一,得到一个有效的DirectInput鼠标设备接口,并把他付给di_mouse.第二,设定数据格式和设备的协作度,仅仅让windows知道我们想查询鼠标设备,而不是要独占该设备.(独占意味着只有我们的应用程序可以使用鼠标,通过指定DISCL_NONEXCLUSIVE,我们通知windows我们将和其他程序共享鼠标)
**通过DirectInput检测鼠标状态**
现在让我们关注CMouse::Refresh().该函数负责更新CMouse的内部键状态和位置.下面是代码:
void CMouse::Refresh(void)
{
char done = 0;
int q;
HRESULT hr;
CPoint p;
DIMOUSESTATE dims;  
// clear our struct ?eventually, directinput will fill this in
memset(&dims, 0, sizeof(DIMOUSESTATE));
if (!m_mousedev) return; // we don抰 have a pointer! Bail!
while (!done)
{
// query DirectInput for newest mouse data
hr = m_mousedev->GetDeviceState(sizeof(DIMOUSESTATE), &dims);
if (FAILED(hr))
{
if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
{
// device lost... reacquire
hr = m_mousedev->Acquire();
if (FAILED(hr))
{
// error handling goes here
clear();
done=1;
}
}
else
{
// it抯 some other error ?handle it
clear();
done = 1;
}
}
else // read mouse successfully!
{
done = 1;
}
} // end while loop ?we抳e read DI correctly
// squirrel away newest rel position data
m_relposition.x = dims.lX;
m_relposition.y = dims.lY;
m_relposition.z = dims.lZ;
// now calc abs position from new relative data
m_absposition.z += dims.lZ;
m_absposition.x += dims.lX;
m_absposition.y += dims.lY; 
// keep the mouse pointer on-screen...
ConstrainPosToScreenSize(m_absposition);
// get button data
for (q=0; q < NUMMOUSEBUTTONS; q++)
{
m_button[q] = (dims.rgbButtons[q] & 0x80));
}
}
这段代码作了许多工作.首先,通过DirectInput得到新的绝对鼠标位置(使用while循环来自动重新检测).其次,把绝对位置数据付给m_absposition,然后得到新的相对位置.ConstrainPosToScreenSize()函数确保鼠标所在点在屏幕范围内.最后,通过循环更新所有的键.
**绘制鼠标**
绘制鼠标光标有两个主要的原则.如果你知道每一帧整个屏幕都会用新的像素数据刷新,你可以简单地blt鼠标光标到新的绝对位置,并由此完成绘制.然而,更好的方法是在你blt之前,得到鼠标光标下的像素数据得一份拷贝,然后,当鼠标移动时,通过blt保存过的像素数据从而擦除旧的blt.我更倾向于后一种方法.
本文将不会讨论具体的blt表面及其他,你应该懂得如何实现.
**线程和尾迹**
如果你不介意多线程,有一种更好的方法来解决鼠标问题.这里的方法是单一线程的,每一帧都会查询鼠标.
适用于高刷新率,而不是低刷新率.鼠标光标会显得很迟缓.最好的方法是建立独立的mouse-rendering线程,它会不断地检测MOUSEMOVE消息,并当鼠标移动时刷新和blt鼠标光标.多线程的优势在于不管你的游戏刷新率
是多少,鼠标光标都会很流畅.有一个单独的鼠标线程将使游戏在任何帧频率下都会相应很快.
建立鼠标轨迹对你显然是件容易的事.在一个数组中保存最后几个鼠标光标的位置.当鼠标移动时,丢弃最旧的坐标,把所有其他的坐标值向下移动一个栏位,并把新的坐标存入顶端的栏位.然后,如果你想要额外的效果.使用alpha混合,并以比新坐标更高的透明度渲染旧的坐标.
**期待...**
现在我们结束了鼠标光标的工作.下一节将讲述如何创建基本的窗体,及如何移动窗体.敬请期待.
