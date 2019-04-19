# win32 GDI 学习 - xqhrs232的专栏 - CSDN博客
2012年06月19日 16:21:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：743标签：[random																[null																[winapi																[colors																[delete																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=colors&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=random&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/blackworld/archive/2011/09/08/2171541.html](http://www.cnblogs.com/blackworld/archive/2011/09/08/2171541.html)
 //确定头文件的包含逻辑 即此为win32程序，不要包含MFC内容。 
#define WIN32_LEAN_AND_MEAN
#include <windows.h> //批量包含头文件
#include <windowsx.h> //内部包含宏和常量 
#include <mmsystem.h>  // very important and include WINMM.LIB too!
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "resources.h"
//定义窗口类类名 
#define WINDOW_CLASS_NAME "WINCLASS1" 
#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
HINSTANCE hinstance_app      = NULL; // 只是为了全局使用 应用程序实例
HDC    hdc;    // 要重画的窗口设备句柄 
HWND hwnd;            //窗口句柄 
int xv=5;    //物体移动速度
int yv=5;    //物体移动速度
int x=WINDOW_WIDTH/2;
int y=WINDOW_HEIGHT/2;
// 窗口事件回调函数 
LRESULT CALLBACK WindowProc(HWND hwnd, //该回调函数对应的窗口句柄 
                            UINT msg, //事件消息 
                            WPARAM wparam, 
                            LPARAM lparam)
{
PAINTSTRUCT        ps;        // 指向图形环境的句柄 WM_PAINT事件中使用 
switch(msg)
    {    
    case WM_CREATE: //窗口第一次创建时调用 
        {
        // 初始化代码 
            PlaySound(MAKEINTRESOURCE(ID_WAVE1),hinstance_app,SND_RESOURCE|SND_ASYNC|SND_LOOP);
        // 返回
        return(0);
        } break;
    case WM_COMMAND:  //菜单事件
        {
            switch(LOWORD(wparam)){
            case MENU_FILE_ID_OPEN:
                {
                    break;
                }
            case MENU_FILE_ID_CLOSE:
                {
                    break;
                }
            case MENU_FILE_ID_SAVE:
                {
                    break;
                }
            case MENU_FILE_ID_EXIT:
                {
                    PostQuitMessage(0);
                    break;
                }
            case MENU_HELP_ABOUT:
                {
                    // 弹出框方法解析 
                    //MessageBox(hwnd,lptext,lpcaption,utype)
                    //hwnd 表示父窗口的句柄  如果为NULL 那么桌面为父窗口 
                    //lptext 内容
                    //lpcaption 标题
                    //utype 控制按钮和文本的显示 
                    MessageBox(hwnd,"win32程序开发模板V1.0\n---------------------\n                by LL","关于 CFrame",MB_OK|MB_ICONEXCLAMATION);
                    break;
                }
            default:break;
            }
        }break;
    case WM_ACTIVATE:
        {
            if(LOWORD(wparam) != WA_INACTIVE)
            {
                //窗口得到焦点事件
            }
            else
            {
                //窗口失去焦点事件
            }
        }break;
    case WM_CLOSE:
        {
            //用户点击关闭窗口按钮时触发事件
            int result = MessageBox(hwnd,"你确定离开游戏么？","离开游戏",MB_YESNO|MB_ICONQUESTION);
            if(result == IDYES)
            {
                return (DefWindowProc(hwnd,msg,wparam,lparam));
            }
            else return(0);
        }break;
    case WM_PAINT: //窗口内容需要重画时调用 
        {
        //BeginPaint测试 这个只获得被破坏的范围 不能获得整个窗口
        hdc = BeginPaint(hwnd,&ps);     //BeginPaint hdc仅代表了窗口被破坏的区域 并不能操作整个屏幕
        // 在此做你自己的绘制 
        EndPaint(hwnd,&ps);
        // 返回
        return(0);
           } break;
    case WM_DESTROY: //窗口销毁时调用 
        {
        PlaySound(NULL,hinstance_app,SND_PURGE);
        // 这句代码发送 WM_QUIT 信息 
        PostQuitMessage(0);
        // 返回
        return(0);
        } break;
    default:break;
    } 
// 默认处理 
return (DefWindowProc(hwnd, msg, wparam, lparam));
}
//画文本
void Draw_Text()
{
    SetTextColor(hdc, RGB(rand()%256,rand()%256,rand()%256));
    SetBkColor(hdc, RGB(0,0,0));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc,rand()%WINDOW_WIDTH,rand()%WINDOW_HEIGHT, "CFrame文本测试", strlen("CFrame文本测试"));
    Sleep(20);
}
void Draw_Pixel()
{
    for (int index=0; index < 1000; index++)
    {
        // get random position
        int x = rand()%WINDOW_WIDTH;
        int y = rand()%WINDOW_HEIGHT;
        COLORREF color = RGB(rand()%255,rand()%255,rand()%255);
        SetPixel(hdc, x,y, color);
     } // end for index
}
void Draw_Line()
{
    // create a random colored pen
       HPEN hpen = CreatePen(PS_SOLID,1,RGB(rand()%256,rand()%256,rand()%256));
       // select the pen into context
       HPEN old_hpen = (HPEN)SelectObject(hdc,hpen);
       // move to a random postion
       MoveToEx(hdc, rand()%WINDOW_WIDTH, rand()%WINDOW_HEIGHT, NULL);
       // draw a line
       LineTo(hdc,rand()%WINDOW_WIDTH, rand()%WINDOW_HEIGHT);
       // now delete the pen
       SelectObject(hdc,old_hpen);
       DeleteObject(hpen);
}
void Draw_Rect()
{
    RECT rect; // used to hold rect info
    // create a random rectangle
    rect.left   = rand()%WINDOW_WIDTH;
    rect.top    = rand()%WINDOW_HEIGHT;
    rect.right  = rand()%WINDOW_WIDTH;
    rect.bottom = rand()%WINDOW_HEIGHT;
    // create a random brush
    HBRUSH hbrush = CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));
    // draw either a filled rect or a wireframe rect
    if ((rand()%2)==1)
        //不使用边界画笔绘制填充矩形 如果画（10，10）到（20，20） 那么填充为（10，10）（21，21）
        //包括左上角 不包含右下角
        FillRect(hdc,&rect,hbrush); 
    else
        //绘制仅有边界的中空矩形
        //只是用画刷 不适用画笔
        FrameRect(hdc,&rect,hbrush);
    // now delete the brush
    DeleteObject(hbrush);
}
void Draw_Ellipse()
{
    // create the pens and brushes
    HPEN   white_pen = CreatePen(PS_SOLID, 1, RGB(255,255,255));
    HPEN   black_pen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
    HBRUSH green_brush = CreateSolidBrush(RGB(0,255,0));
    HBRUSH black_brush = CreateSolidBrush(RGB(0,0,0));
     // first select the black pen and brush into context
    //下面绘制的目的是消除圆形的上一次的图像，即用黑色覆盖住 这样玩家就看不到了
    SelectObject(hdc, black_pen);
    SelectObject(hdc, black_brush);
     // draw the ball
    Ellipse(hdc, x, y, x + 32, y + 32);
    // MOVE the ball
    x+=xv;
    y+=yv;
    // test for collisions, first x-axis
    if (x < 0 || x > WINDOW_WIDTH - 32)
       { 
       // invert x-velocity of ball
       xv=-xv;
       // push ball back
       x+=xv;
       } // end if
    else
    // test for y-axis collisions
    if (y < 0 || y > WINDOW_HEIGHT - 32)
       { 
       // invert y-velocity of ball
       yv=-yv;
       // push ball back
       y+=yv;
       } // end if
    // now select the green and white colors for brush and pen
    SelectObject(hdc, white_pen);
    SelectObject(hdc, green_brush);
    // DRAW the ball
    Ellipse(hdc, x, y, x + 32, y + 32);
    // delete all the objects
    DeleteObject(white_pen);
    DeleteObject(black_pen);
    DeleteObject(green_brush);
    DeleteObject(black_brush);
}
void Draw_Polygon()
{
     // select random colors for polygon 
    HPEN   pen_color   = CreatePen(PS_SOLID, 1, RGB(rand()%256,rand()%256,rand()%256));
    HBRUSH brush_color = CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));
    // select them into dc
    HPEN old_hpen =(HPEN)SelectObject(hdc,pen_color);
    HBRUSH old_brush =(HBRUSH)SelectObject(hdc,brush_color);
    // now create list of random points for polygon
    int num_points = 3+rand()%8;
    // this will hold the point list
    POINT point_list[10];
    // create array of points
    for (int index = 0; index < num_points; index++)
        {
        // set next random point in list
        point_list[index].x = rand()%WINDOW_WIDTH;
        point_list[index].y = rand()%WINDOW_HEIGHT;
        } // end for index
    // draw the polygon
    Polygon(hdc, point_list, num_points);
     // now delete the pen
     SelectObject(hdc,old_hpen);
     SelectObject(hdc,old_brush);
     DeleteObject(pen_color);
     DeleteObject(brush_color);
}
//游戏主循环
void Game_Main()
{
    hdc = GetDC(hwnd);
    Draw_Text(); //绘制文字测试
    Draw_Pixel(); //绘制像素测试
    Draw_Line(); //绘制线段
    Draw_Rect(); //绘制矩形
    Draw_Ellipse(); //绘制并且移动圆形 有闪烁
    Draw_Polygon(); //绘制多边形
    ReleaseDC(hwnd, hdc);
}
// WINAPI 强制参数从左到右传递
// hInstance 系统为此应用程序生成的实例句柄 可以用来跟踪这个应用程序
// hPrevInstance 过时了 在旧版本中 负责跟踪这个应用程序以前的实例 
//lpCmdLine 用来命令行运行时传递参数
// nCmdShow 控制该窗口打开时的状态 
int WINAPI WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
     // TODO: Place code here.
    WNDCLASSEX winclass; //窗口类对象 
    MSG msg;    
    hinstance_app = hInstance;
    //创建窗口类 
    //所有同一个窗口类创建的对象有相同的事件回调函数
    //如果需要每个窗口都有自己的回调函数 那么需要创建多余一个的窗口类
    winclass.cbSize = sizeof(WNDCLASSEX);
    winclass.style = CS_DBLCLKS | CS_OWNDC |  //窗口类 类型 
                      CS_HREDRAW | CS_VREDRAW;
    winclass.lpfnWndProc = WindowProc;
    winclass.cbClsExtra        = 0;
    winclass.cbWndExtra        = 0;
    winclass.hInstance        = hInstance;
    winclass.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(ID_ICON2)); // 控制ALT+TAB时的图标
    winclass.hCursor        = LoadCursor(hInstance, MAKEINTRESOURCE(ID_CURSOR1)); //鼠标样式
    winclass.hbrBackground    = (HBRUSH)GetStockObject(BLACK_BRUSH); //背景画刷 
    winclass.lpszMenuName    = NULL;
    winclass.lpszClassName    = WINDOW_CLASS_NAME;
    //注意 资源文件中的第一个图标就是程序图标
    //控制任务管理器 窗口左上角 平面下方任务栏的图标
    winclass.hIconSm        = LoadIcon(hInstance, MAKEINTRESOURCE(ID_ICON2)); 
    //注册窗口类
    if (!RegisterClassEx(&winclass))
    return(0);
    //创建窗口
    if (!(hwnd = CreateWindowEx(NULL, // 高级特性 一般不用 
                            WINDOW_CLASS_NAME,   // 窗口类类名 
                            "Your Basic Window", // 标题
                            WS_OVERLAPPEDWINDOW | WS_VISIBLE,//窗口外观和行为标志 
,200,        // x坐标 y坐标 
                            WINDOW_WIDTH,WINDOW_HEIGHT,  // 宽高
                            NULL,        // 父窗口句柄 如果为NULL 则父窗口是桌面 
                            NULL,        // 父窗口菜单 
                            hInstance,// 该应用程序实例 
                            NULL)))    // 高级特性 一般不用 
    return(0);
    HMENU hmenu = LoadMenu(hInstance,"MainMenu");
    SetMenu(hwnd,hmenu);
    //hdc = GetDC(hwnd);
     /*主事件循环 这个不实时
    while(GetMessage(&msg,NULL,0,0))
      { 
     // 转换
     TranslateMessage(&msg);
     // 发送事件到窗口事件回调函数 
     DispatchMessage(&msg);
     } 
    */
     //主事件循环 实时循环 
    while(TRUE)
    {
        DWORD start_time = GetTickCount(); //获得windows启动后的毫秒数
         if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
         {
             if(msg.message == WM_QUIT) break; //跳出无限循环条件
             TranslateMessage(&msg);
             DispatchMessage(&msg);
         }
         Game_Main(); //此处为游戏主循环
         while((GetTickCount() - start_time) <33); //33毫秒后执行下一帧显示 此代码锁定帧频为1秒30帧
         //检测键盘事件 主动发送消息结束进程
         //用户可以自定义消息 也可以使用PoseMessage来发送消息
         if (KEYDOWN(VK_ESCAPE)) SendMessage(hwnd, WM_CLOSE, 0,0);
         // slow system down a little
         //Sleep(50);  //仅仅是延迟程序
    }
    //ReleaseDC(hwnd,hdc);
    return(msg.wParam);  
}
