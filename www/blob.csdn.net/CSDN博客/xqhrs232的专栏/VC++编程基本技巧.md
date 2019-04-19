# VC++编程基本技巧 - xqhrs232的专栏 - CSDN博客
2008年09月19日 21:16:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1064
 2008--7--27
1。通过类向导增加类-----类继承自MFC类库
2。怎么调试-------TRACE0/1/2/3宏的使用
3。MessageBox的使用----AfxMessageBox
4。C文件在CPP工程里面的编译-----预定义/预编译
5。怎么增加一个预定义
6。怎么产生MAP文件
7。怎么通过MAP文件定位有问题的函数与代码
8。怎么通过调试定位有问题的代码
9。选择WINDOW属性，出现更多的消息----用于映射
10。变量跟控件的绑定
11。UpdateDate(TRUE)/UpdateData(FALSE)的使用-----控件到变量，变量到控件
12。怎么修改系统的颜色-----改变默认的灰色
13。Static静态变量的使用-----属于类只创建一次
14。Static静态函数的使用-----属于类只创建一次
15。打开工程，看不到类列表------去掉几个无关的文件，CLW包括了类列表
16。怎么插入一个ACTIVEX控件
17。调试的情况下，DEBUG窗口的打开方法------其它各种窗口的打开方法（寄存器窗口等等）
18。控件菜单工具栏的打开与关闭----各种工具栏的调出
19。H头文件/LIB库文件/EXE可执行文件的路径定位与优先级选择
20。程序如何优化-----编译选项/警告等级
21。程序怎么连到静态库/动态库
22。DLL文件怎么导出类接口
23。模块间怎么共享变量
24。怎么通过DLL的句柄结合GETPROCESS（ ）函数得到DLL中的函数指针
25。CPP文件中怎么引用C文件中定义的函数--------EXTERN  C
26。错误号的定位与含义
27。GetLastError( )函数的使用----用于查看函数执行的结果
28。SPY++工具的使用
29。DUMPBIN工具的使用，用于查看DLL的信息----出口/入口/各种头
30。PE文件格式分析
31。WM_CLOSE/WM_DESTORY/WM_QUIT三个消息的联系与区别
32。WM_PAINT消息的处理
33。WM_ERASEBKGND消息的响应
34。WM_COMMAND消息对控件的响应
35。WM_INITDialog消息
36。RGB跟COLORREF的区别与联系
37。::全局运算符的使用与全局函数
38。定时器的使用
39。GDI/GDI+
40。WIN32/MFC/SDK/DDK
41。COM/DCOM
42。OLE DB/ADO/DAO/ODBC------数据库操作接口
43。ACTIVEX/OLE
44。DIRECTX/WINDOWS MEDIA OBJECT
45。STL----各种容器
46。泛型编程/通用算法
47。WTL/ATL
48。C运行时库
49。各种输出格式-----%d/%x/%s%f
50。如何进行断点与单步调试
51。MFC下的各种常用控件的使用----控件类的成员函数与成员变量
52。CString类的使用----GetAt( )+++SetAt( )
53。PostMessage( )++++SendMessage( )的区别与联系，一个不等执行结果返回，一个等执行结果返回
54。GetForegroundWindow( )函数的使用，得到最上层的那个窗口
55。FindWindow( )函数的使用-----通过窗口类的信息来查窗口
56。BringWindowToTop( )函数改变窗口的Z序
57。Cin/Cout----Sprintf/printf
58。StrCat/StrCpy/Strlen函数
59。ASSERT断言机制
60。无模式对话框/模式对话框的区别-----等到输出才返回
61。内存泻漏问题
62。NEW与DELETE的配套使用----资源申请了就要释放
63。Const修饰符的使用
64。this指针
65。MFC框架程序的运行框架
66。SQL语句
67。线程，纤程，进程
68。事件，消息，互斥体，关键区（用于同一个进程里的线程间的通信）
69。WM_ACTIVE/WM_INACTIVE消息
70。PeekMessage( )看一下线程的消息队列----可以决定处理那个消息不处理那个消息
71。WIN32程序的消息泵
72。双缓冲技术----减少闪烁
73。SetWindowText( )
74。MoveWindow( )
75。GlobalAlloc/GlobalFree
76。LocalFree/LocalFree
77。ENUMWINDOWS( )
78。链表操作
79。协议栈
80。GDI/DIRECT3D
81。LIB/DLL技术
82。内存映射文件
83。事件/关键区/信号量/消息/互斥体/
84。导出头文件----导出类
85。WIN32程序怎么调试------用OutputDebugString函数可以向调试器输出消息
86。调试的各个窗口怎么调出来
87。设置断点--------程序执行到断点处停下----就可以进行单步调试了
88。CONSOLE程序的调试-----可以通过SCANF++++PRINTF函数来输出调试消息
89。同一个工作台下，各个工程间的依赖关系设置
90。CRT的多线程
91。使用MFC的静态库+++动态库
92。通过设置菜单怎么优化程序
