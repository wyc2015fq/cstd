# wince 6.0 回调函数的理解（转） - xqhrs232的专栏 - CSDN博客
2013年12月19日 22:52:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：803
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://hi.baidu.com/jiandanbinxin/item/3ddb0414517ac1483b176e6b](http://hi.baidu.com/jiandanbinxin/item/3ddb0414517ac1483b176e6b)
相关文章
1、[回调函数](http://blog.chinaunix.net/uid-20459533-id-1944403.html) ----[http://blog.chinaunix.net/uid-20459533-id-1944403.html](http://blog.chinaunix.net/uid-20459533-id-1944403.html)
2、[wince 串口与回调函数](http://blog.csdn.net/xiaodan007/article/details/5939296)----[http://blog.csdn.net/xiaodan007/article/details/5939296](http://blog.csdn.net/xiaodan007/article/details/5939296)
**什么是回调函数**
回调函数，就是由你自己写的你需要调用另外一个函数，而这个函数的其中一个参数，就
是你的这个回调函数名这样，系统在必要的时候，就会调用你写的回调函数，这样你就可
以在回调函数里完成你要做的事
模块A有一个函数foo，它向模块B传递foo的地址，然后在B里面发生某种事件（event）时，通过从A里面传递过来的foo的地址调用foo，通知A发生了什么事情，让A作出相应反应那么我们就把foo称为回调函数
例子：
    回调函数是一个很有用，也很重要的概念当发生某种事件时，系统或其他函数将会自动调用你定义的一段函数回调函数在windows编程使用的场合很多，比如Hook回调函数：MouseProc,GetMsgProc以及EnumWindows,DrawState的回调函数等等，还有很多系统级的回调过程本文不准备介绍这些函数和过程，而是谈谈实现自己的回调函数的一些经验
    之所以产生使用回调函数这个想法，是因为现在使用VC和Delphi混合编程，用VC写的一个DLL程序进行一些时间比较长的异步工作，工作完成之后，需要通知使用DLL的应用程序：某些事件已经完成,请处理事件的后续部分开始想过使用同步对象，文件影射，消息等实现DLL函数到应用程序的通知，后来突然想到可不可以在应用程序端先写一个函数，等需要处理后续事宜的时候，在DLL里直接调用这个函数即可  
     于是就动手，写了个回调函数的原形在VC和 Delphi里都进行了测试
一：声明回调函数类型
      vc版
             typedef int (WINAPI *PFCALLBACK)(int Param1,int Param2) ;
      Delph版
             PFCALLBACK = function(Param1:integer;Param2:integer):integer;stdcall;
      实际上是声明了一个返回值为int,传入参数为两个int的指向函数的指针
      由于C++和PASCAL编译器对参数入栈和函数返回的处理有可能不一致，把函数类型用WINAPI(WINAPI宏展开就是__stdcall)或stdcall统一修饰
二：声明回调函数原形
      声明函数原形
     vc版
              int WINAPI CBFunc(int Param1,int Param2)；
      Delphi版
         function CBFunc(Param1,Param2:integer):integer;stdcall;            
     以上函数为全局函数，如果要使用一个类里的函数作为回调函数原形，把该类函数声明为静态函数即可
三： 回调函数调用调用者
        调用回调函数的函数我把它放到了DLL里，这是一个很简单的VC生成的WIN32 DLL.并使用DEF文件输出其函数名 TestCallBack实现如下：
             PFCALLBACK gCallBack=0;
           void WINAPI TestCallBack(PFCALLBACK Func)
          {
                 if(Func==NULL)return;
                 gCallBack=Func;
                 DWORD ThreadID=0;
                 HANDLE hThread = CreateThread( NULL, NULL, Thread1,  LPVOID(0),         &ThreadID );
                  return;
            }
     此函数的工作把传入的 PFCALLBACK Func参数保存起来等待使用，并且启动一个线程声明了一个函数指针PFCALLBACK gCallBack保存传入的函数地址
四： 回调函数如何被使用：
         TestCallBack函数被调用后，启动了一个线程，作为演示，线程人为的进行了延时处理，并且把线程运行的过程打印在屏幕上.
本段线程的代码也在DLL工程里实现
     ULONG WINAPI Thread1(LPVOID Param)
    {
            TCHAR Buffer[256];
            HDC hDC = GetDC(HWND_DESKTOP);
            int Step=1;
            MSG Msg;
             DWORD StartTick;
       //一个延时循环
            for(;Step<200;Step++)
            {
                       StartTick = GetTickCount();
                      for(;GetTickCount()-StartTick<10;)
                        {
                                if(PeekMessage(&Msg,NULL,0,0,PM_NOREMOVE) )
                                {
                                  TranslateMessage(&Msg);
                                  DispatchMessage(&Msg);
                                  }
                          }                               
          sprintf(Buffer,"Running %04d",Step);
                        if(hDC!=NULL)
                                 TextOut(hDC,30,50,Buffer,strlen(Buffer));
                  }
               (*gCallback)(Step,1);
                  ::ReleaseDC (HWND_DESKTOP,hDC);
                 return 0;
     }
五：万事具备
       使用vc和Delphi各建立了一个工程，编写回调函数的实现部分
      VC版
    int WINAPI CBFunc(int Param1,int Param2)
      {
              int res= Param1+Param2;
            TCHAR Buffer[256]="";
           sprintf(Buffer,"callback result = %d",res);
           MessageBox(NULL,Buffer,"Testing",MB_OK); //演示回调函数被调用
            return res;           
      }  
        Delphi版
         function CBFunc(Param1,Param2:integer):integer;
         begin
                 result:= Param1+Param2;
                 TForm1.Edit1.Text:=inttostr(result);   / /演示回调函数被调用
          end;
      使用静态连接的方法连接DLL里的出口函数 TestCallBack,在工程里添加 Button( 对于Delphi的工程，还需要在Form1上放一个Edit控件，默认名为Edit1)
       响应ButtonClick事件调用 TestCallBack
             TestCallBack(CBFunc) //函数的参数CBFunc为回调函数的地址
       函数调用创建线程后立刻返回，应用程序可以同时干别的事情去了现在可以看到屏幕上不停的显示字符串，表示dll里创建的线程运行正常一会之后，线程延时部分结束结束，vc的应用程序弹出MessageBox,表示回调函数被调用并显示根据Param1，Param2运算的结果，Delphi的程序edit控件里的文本则被改写成Param1，Param2 的运算结果
       可见使用回调函数的编程模式，可以根据不同的需求传递不同的回调函数地址，或者定义各种回调函数的原形（同时也需要改变使用回调函数的参数和返回值约定），实现多种回调事件处理，可以使程序的控制灵活多变，也是一种高效率的，清晰的程序模块之间的耦合方式在一些异步或复杂的程序系统里尤其有用 -- 你可以在一个模块（如DLL）里专心实现模块核心的业务流程和技术功能，外围的扩展的功能只给出一个回调函数的接口，通过调用其他模块传递过来的回调函数地址的方式，将后续处理无缝地交给另一个模块，随它按自定义的方式处理
     本文的例子使用了在DLL里的多线程延时后调用回调函数的方式，只是为了突出一下回调函数的效果，其实只要是在本进程之内，都可以随你高兴可以把函数地址传递来传递去，当成回调函数使用
      这样的编程模式原理非常简单单一：就是把函数也看成一个指针一个地址来调用，没有什么别的复杂的东西，仅仅是编程里的一个小技巧至于回调函数模式究竟能为你带来多少好处，就看你是否使用，如何使用这种编程模式了
msdn上这么说的：
有关函数指针的知识
使用例子可以很好地说明函数指针的用法首先，看一看 Win32 API 中的 EnumWindows 函数：
Declare Function EnumWindows lib "user32" _
(ByVal lpEnumFunc as Long, _
ByVal lParam as Long ) As Long
EnumWindows 是一个枚举函数，它能够列出系统中每一个打开的窗口的句柄EnumWindows 的工作方式是重复地调用传递给它的第一个参数（lpEnumFunc，函数指针）每当 EnumWindows 调用函数，EnumWindows 都传递一个打开窗口的句柄
在代码中调用 EnumWindows 时，可以将一个自定义函数作为第一个参数传递给它，用来处理一系列的值例如，可以编写一个函数将所有的值添加到一个列表框中，将 hWnd 值转换为窗口的名字，以及其它任何操作！
为了表明传递的参数是一个自定义函数，在函数名称的前面要加上 AddressOf 关键字第二个参数可以是合适的任何值例如，如果要把 MyProc 作为函数参数，可以按下面的方式调用 EnumWindows：
x = EnumWindows(AddressOf MyProc, 5)
在调用过程时指定的自定义函数被称为回调函数回调函数（通常简称为回调）能够对过程提供的数据执行指定的操作
回调函数的参数集必须具有规定的形式，这是由使用回调函数的 API 决定的关于需要什么参数，如何调用它们，请参阅 API 文档
我谈一下自己对回调函数的一点理解, 不对的地方请指教.
   我刚开始接触回调时, 也是一团雾水.很多人解释这个问题时, 总是拿API来举例子, 本来菜鸟最惧怕的就是API, ^_^. 回调跟API没有必然联系.
   其实回调就是一种利用函数指针进行函数调用的过程.
   为什么要用回调呢?比如我要写一个子模块给你用, 来接收远程socket发来的命令.当我接收到命令后, 需要调用你的主模块的函数, 来进行相应的处理.但是我不知道你要用哪个函数来处理这个命令, 我也不知道你的主模块是什么.cpp或者.h, 或者说, 我根本不用关心你在主模块里怎么处理它, 也不应该关心用什么函数处理它...... 怎么办?
   使用回调.
   我在我的模块里先定义回调函数类型, 以及回调函数指针.
   typedef void (CALLBACK *cbkSendCmdToMain) (AnsiString sCmd);
   cbkSendCmdToMain   SendCmdToMain;
   这样SendCmdToMain就是一个指向拥有一个AnsiString形参, 返回值为void的函数指针.
   这样, 在我接收到命令时, 就可以调用这个函数啦.
   ...
   SendCmdToMain(sCommand);
   ...
   但是这样还不够, 我得给一个接口函数(比如Init), 让你在主模块里调用Init来注册这个回调函数.
   在你的主模块里, 可能这样
   void CALLBACK YourSendCmdFun(AnsiString sCmd); //声明
   ...
   void CALLBACK YourSendCmdFun(AnsiString sCmd); //定义
   {
       ShowMessage(sCmd);
   }
   ...
   调用Init函数向我的模块注册回调.可能这样:
   Init(YourSendCmdFun, ...);
   这样, 预期目的就达到了.
   需要注意一点, 回调函数一般都要声明为全局的. 如果要在类里使用回调函数, 前面需要加上 static , 其实也相当于全局的.
