# GetKeyState 和 GetAsyncKeyState 区别 - L_Andy的专栏 - CSDN博客

2014年01月05日 17:36:50[卡哥](https://me.csdn.net/L_Andy)阅读数：1243



使用::GetKeyState()返回一个short型的数，short型是16位有符号的数据类型，如果要查询的键被按下，返回值最高位被置1，则这个数表示负数，所以可以用<0或>0来判断。

0x8000是16进制数，用2进制表示为1000 0000 0000 0000， &是按位与

同样，如果键被按下，返回值最高位为1，则1xxx xxxx xxxx xxxx & 1000 0000 0000 0000得到的结果为1，否则为0，同样可以判断最高位的值。

需要说明的是，::GetKeyState()只能在键盘消息处理程序中使用，因为它只有在线程从消息队列中读取键盘消息时才会报告被查询键的状态，如果需要在键盘消息处理程序以外查询按键状态，则需要使用::GetAsyncKeyState()来代替.

————————————————————————————

函数功能：该函数检取指定虚拟键的状态。该状态指定此键是UP状态，DOWN状态，还是被触发的（开关每次按下此键时进行切换）。

函数原型：SHORT GetKeyState（int nVirtKey）；

函数：

nVrtKey：定义一虚拟键。若要求的虚拟键是字母或数字（A～Z，a～z或0～9），nVirtKey必须被置为相应字符的ASCII码值，对于其他的键，nVirtKey必须是一虚拟键码。若使用非英语键盘布局，则取值在ASCIIa～z和0～9的虚拟键被用于定义绝大多数的字符键。例如，对于德语键盘格式，值为ASCII0（OX4F）的虚拟键指的是"0"键，而VK_OEM_1指"带变音的0键"

返回值：返回值给出了给定虚拟键的状态，状态如下：

若高序位为1，则键处于DOWN状态，否则为UP状态。

若低序位为1，则键被触发。例如CAPS LOCK键，被找开时将被触发。若低序位置为0，则键被关闭，且不被触发。触发键在键盘上的指示灯，当键被触发时即亮，键不被触发时即灭。

备注：当给定线程从它的消息队列中读键消息时，该函数返回的键状态发生改变。该状态井不反映与硬件相关的中断级的状态。使用SetKeyboardState可获取这一信息。

欲检取所有虚拟键状态信息，可以使用SetKeyboardState函数。

应用程序可以使用虚拟键码常数VK_SHIFT，VK_CONTROL和VK_MENU作为nVirtKey参数的值。它给出shift，ctrl或alt键的值而不区分左右键，应用程序也可以使用如下的虚拟键码常数作nVirtKey的值来区分前述键的左键、右键的情形。

VK_LSHIFT，VK_RSHIFT；VK_LCONTROL，VK_RCONTROL；VK_LMENU，VK_RMENU

仅当应用程序调用GetKeyboardSlate，SetKeyboardState，GetAsyncKeystate；GetKeyState和MapVirtualKey函数时，才可用这些区分左右键的常数。

Windows CE：GetKeyState函数仅能用于检查如下虚拟键的DOWN状态。

VK_LSHIFT，VKRSHIFT，VK_LCONTROL；VK_RCONTROL；VK_LMENU，VK_RMENU

GetKeyState函数只能用于检查VK_CAPITAL虚拟键的触发状态。

——————————————————————————

**GetAsyncKeyState**

就是调用GetAsyncKeyState函数得到某个键的状态，然后判断返回值的最高位是不是1，如果是表示这个处于按下状态

1、&与操作，&0x8000就是判断这个返回值的high-order bit（高位字节）

2、如果high-order bit是1,则是按下状态，否则UP状态

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

意思你应该看的懂吧。

说明一下，带的参数要是虚拟打描码！

比如下的码是 VK_DOWN ，ESC是 VK_ESCAPE

GetAsyncKeyState 取异步键状态。

比如，一般情况下，你可以在操作系统的控制面板中设置鼠标左右键的映射（实际的鼠标左键可以映射成右键点击事件），或者通过程序也可以这样设置，这样就产生了物理键状态（实际的），逻辑键状态（虚拟的，）。使用 GetKeyState,GetKeyboardState,等函数得到的就是逻辑键状态。

而GetAsyncKeyState 读取的是物理键状态，也是就是不管你怎么鼠标键盘映射，它只读取实际的按键状态。

摄像机调节

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000)? 1 : 0)

struct PLAYER

{

D3DXVECTOR3 LookAt[3]; //eyepos lookat up

};

PLAYER g_Player; //追随视角

FLOAT g_Angle = 0.0f; //旋转角度

// 调用InitMy3D函数进行3D对象和设备的创建，传入参数为窗口的句柄hWnd

if( SUCCEEDED( InitMy3D( hWnd ) ) )

{

// 显示窗口

ShowWindow( hWnd, SW_SHOWDEFAULT );

UpdateWindow( hWnd );

// 程序主循环，进行窗口消息的分发，消息的处理在MsgProc函数中完成

MSG msg;

ZeroMemory(&msg,sizeof(MSG));

while( msg.message != WM_QUIT)

{

if(PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ))

{

TranslateMessage( &msg );

DispatchMessage( &msg );

}

ProcInput();

MatricesTrans();

RenderSene();

}

}

VOID SetupMatrices()

{

D3DXMATRIXA16 matWorld;

D3DXMatrixRotationY( &matWorld, timeGetTime() / 1000.0f );

g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );

D3DXMATRIXA16 matView; //视图变换矩阵
D3DXMatrixLookAtLH( &matView, &g_Player.LookAt[0], &g_Player.LookAt[1], &g_Player.LookAt[2] );

g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

D3DXMATRIXA16 matProj; //投影变换矩阵

D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 20000.0f );

g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

}

void ProcInput()

{

if(KEYDOWN(VK_DOWN) || KEYDOWN('S') )

{

g_Player.LookAt[0].x = g_Player.LookAt[0].x-sinf(g_Angle)/20; //eyepos

g_Player.LookAt[0].z = g_Player.LookAt[0].z-cosf(g_Angle)/20; //lookat

g_Player.LookAt[1].x = g_Player.LookAt[1].x-sinf(g_Angle)/20;

g_Player.LookAt[1].z = g_Player.LookAt[1].z-cosf(g_Angle)/20;

}

if(KEYDOWN(VK_UP) || KEYDOWN('W'))

{

g_Player.LookAt[0].x = g_Player.LookAt[0].x+sinf(g_Angle)/20;

g_Player.LookAt[0].z = g_Player.LookAt[0].z+cosf(g_Angle)/20;

g_Player.LookAt[1].x = g_Player.LookAt[1].x+sinf(g_Angle)/20;

g_Player.LookAt[1].z = g_Player.LookAt[1].z+cosf(g_Angle)/20;

}

if(KEYDOWN(VK_RIGHT) || KEYDOWN('D'))

{

g_Angle+=0.01f;

g_Player.LookAt[1].x = g_Player.LookAt[0].x+sinf(g_Angle)/100;

g_Player.LookAt[1].z = g_Player.LookAt[0].z+cosf(g_Angle)/100;

}

if(KEYDOWN(VK_LEFT) || KEYDOWN('A'))

{

g_Angle-=0.01f;

g_Player.LookAt[1].x = g_Player.LookAt[0].x+sinf(g_Angle)/100;

g_Player.LookAt[1].z = g_Player.LookAt[0].z+cosf(g_Angle)/100;

}

}

