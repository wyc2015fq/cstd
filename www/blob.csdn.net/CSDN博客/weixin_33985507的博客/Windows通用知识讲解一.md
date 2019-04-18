# Windows通用知识讲解一 - weixin_33985507的博客 - CSDN博客
2017年04月06日 23:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
```
Window应用程序的类型
--控制台程序Console
    DOS程序，本身没有窗口，通过Windows DOS窗口执行
--窗口程序
    拥有自己的窗口，可以与用户交互
--库程序
    存放代码、数据的程序，执行文件可以从中取出代码执行和或获取数据
    -静态库程序
        扩展名LIB,在执行文件执行时从中获取代码
    -动态库程序
        扩展名DLL，在编译链接程序时，讲函数地址放入到执行文件中
```
```
三种应用程序的对比
--入口函数
    控制台程序--main
    窗口程序--WinMain
    动态库程序--DllMain
    静态库程序--无入口函数
--文件存在方式
    控制台程序，窗口程序--EXE文件
    动态库程序--DLL文件
    静态库程序--LIB文件
--执行方式
    控制台程序--在DOS窗口内执行
    窗口程序--拥有自己的窗口，在自己的窗口内执行
    动态库程序--本身无法执行，由可执行程序或其他DLL调用
    静态库程序--执行不存在，代码会嵌入到可执行文件或DLL中
```
```
VC编译工具
--编译器cl.exe    讲源代码(.c或.cpp)编译成目标代码（汇编.obj）
--链接器link.exe    将目标代码(.obj)，库链接生成最终文件.exe|.dll|.lib（二进制文件）
--资源编译器rc.exe    将资源编译，最终通过链接器存入最终文件.rc
```
```
Windows库和头文件
--Windows库
    kernel32.dll--提供核心的API，例如进程、线程、内存管理等。
    user32.dll--提供了窗口、消息等API。
    gdi32.dll--绘图相关的API。
--头文件
    Windows.h--所有头文件的集合
    windef,h--windows数据类型
    winbase.h--kernel32的API
    wingdi.h--gdi32的API
    winuser.h--user32的API
    winnt.h--UNICODE字符集支持
```
```
int WinMain(
    HINSTANCE hInstance,//当前进程的实例句柄
    HINSTANCE hPrevInstance,//当前程序前一个实例句柄(hPrevInstance-Win32下，一般为NULL)
    LPSTR lpCmdLine,//命令行参数字符串
    int nCmdShow//窗口显示方式
    );
```
```
int MessageBox(
    HWND hWnd,//父窗口句柄
    LPCTSTR lpText,//显示在消息框中的文字
    LPCTSTR lpCaption,//显示在标题栏中的文字
    UINT uType//消息框中的按钮、图标显示类型
    );//返回点击的按钮ID
```
```
编译程序
cl.exe -? 显示cl的帮助
cl.exe /c 只编译不链接
cl.exe /Tc 编译C文件
cl.exe /Tp 编译C++文件
```
```
链接程序
link.exe xxx.obj xxx.lib
例子：link.exe 1.obj user32.lib--user32.lib链接的库文件
```
```
编译资源的文件-.rc资源脚本文件
编译rc文件 rc.exe
将资源链接到程序中 link.exe
```
