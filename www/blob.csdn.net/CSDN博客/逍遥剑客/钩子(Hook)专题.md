# 钩子(Hook)专题 - 逍遥剑客 - CSDN博客
2008年03月19日 16:07:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2735标签：[hook																[dll																[keyboard																[shell																[function																[callback](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=keyboard&t=blog)](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=hook&t=blog)
个人分类：[基础知识](https://blog.csdn.net/xoyojank/article/category/259553)
目录
- 基本概念 
- 运行机制 
- 钩子类型 
- 作者 
基本概念
钩子(Hook)，是Windows消息处理机制的一个平台,应用程序可以在上面设置子程以监视指定窗口的某种消息，而且所监视的窗口可以是其他进程所创建的。当消息到达后，在目标窗口处理函数之前处理它。钩子机制允许应用程序截获处理window消息或特定事件。
钩子实际上是一个处理消息的程序段，通过系统调用，把它挂入系统。每当特定的消息发出，在没有到达目的窗口前，钩子程序就先捕获该消息，亦即钩子函数先得到控
制权。这时钩子函数即可以加工处理（改变）该消息，也可以不作处理而继续传递该消息，还可以强制结束消息的传递。
运行机制
1、钩子链表和钩子子程：
每一个Hook都有一个与之相关联的指针列表，称之为钩子链表，由系统来维护。这个列表的指针指向指定的，应用程序定义的，被Hook子程调用的回调函数，也就是该钩子的各个处理子程。当与指定的Hook类型关联的消息发生时，系统就把这个消息传递到Hook子程。一些Hook子程可以只监视消息，或者修改消息，或者停止消息的前进，避免这些消息传递到下一个Hook子程或者目的窗口。最近安装的钩子放在链的开始，而最早安装的钩子放在最后，也就是后加入的先获得控制权。
Windows 并不要求钩子子程的卸载顺序一定得和安装顺序相反。每当有一个钩子被卸载，Windows 便释放其占用的内存，并更新整个Hook链表。如果程序安装了钩子，但是在尚未卸载钩子之前就结束了，那么系统会自动为它做卸载钩子的操作。
钩子子程是一个应用程序定义的回调函数(CALLBACK Function),不能定义成某个类的成员函数，只能定义为普通的C函数。用以监视系统或某一特定类型的事件，这些事件可以是与某一特定线程关联的，也可以是系统中所有线程的事件。
钩子子程必须按照以下的语法：
LRESULT CALLBACK HookProc
(
 	int nCode, 
     	WPARAM wParam, 
     	LPARAM lParam
);
HookProc是应用程序定义的名字。
nCode参数是Hook代码，Hook子程使用这个参数来确定任务。这个参数的值依赖于Hook类型，每一种Hook都有自己的Hook代码特征字符集。
wParam和lParam参数的值依赖于Hook代码，但是它们的典型值是包含了关于发送或者接收消息的信息。
2、钩子的安装与释放：
使用API函数SetWindowsHookEx()把一个应用程序定义的钩子子程安装到钩子链表中。SetWindowsHookEx函数总是在Hook链的开头安装Hook子程。当指定类型的Hook监视的事件发生时，系统就调用与这个Hook关联的Hook链的开头的Hook子程。每一个Hook链中的Hook子程都决定是否把这个事件传递到下一个Hook子程。Hook子程传递事件到下一个Hook子程需要调用CallNextHookEx函数。
HHOOK SetWindowsHookEx( 
　　　　　int idHook,      // 钩子的类型，即它处理的消息类型
　　　　　HOOKPROC lpfn,   // 钩子子程的地址指针。如果dwThreadId参数为0
			   // 或是一个由别的进程创建的线程的标识，
			   // lpfn必须指向DLL中的钩子子程。
			   // 除此以外，lpfn可以指向当前进程的一段钩子子程代码。
			   // 钩子函数的入口地址，当钩子钩到任何消息后便调用这个函数。
　　　　　HINSTANCE hMod,  // 应用程序实例的句柄。标识包含lpfn所指的子程的DLL。
			   // 如果dwThreadId 标识当前进程创建的一个线程，
			   // 而且子程代码位于当前进程，hMod必须为NULL。
			   // 可以很简单的设定其为本应用程序的实例句柄。
　　　　　DWORD dwThreadId // 与安装的钩子子程相关联的线程的标识符。
			   // 如果为0，钩子子程与所有的线程关联，即为全局钩子。
　　　　　            ); 
　　函数成功则返回钩子子程的句柄，失败返回NULL。
　　以上所说的钩子子程与线程相关联是指在一钩子链表中发给该线程的消息同时发送给钩子子程，且被钩子子程先处理。
在钩子子程中调用得到控制权的钩子函数在完成对消息的处理后，如果想要该消息继续传递，那么它必须调用另外一个SDK中的API函数CallNextHookEx来传递它，以执行钩子链表所指的下一个钩子子程。这个函数成功时返回钩子链中下一个钩子过程的返回值，返回值的类型依赖于钩子的类型。这个函数的原型如下：
LRESULT CallNextHookEx
			(
				HHOOK hhk;
				int nCode;
				WPARAM wParam;
				LPARAM lParam;
			 );
hhk为当前钩子的句柄，由SetWindowsHookEx()函数返回。
NCode为传给钩子过程的事件代码。
wParam和lParam 分别是传给钩子子程的wParam值，其具体含义与钩子类型有关。
钩子函数也可以通过直接返回TRUE来丢弃该消息，并阻止该消息的传递。否则的话，其他安装了钩子的应用程序将不会接收到钩子的通知而且还有可能产生不正确的结果。
钩子在使用完之后需要用UnHookWindowsHookEx()卸载，否则会造成麻烦。释放钩子比较简单，UnHookWindowsHookEx()只有一个参数。函数原型如下：
UnHookWindowsHookEx
(
	HHOOK hhk;
);
函数成功返回TRUE，否则返回FALSE。
3、一些运行机制：
在Win16环境中，DLL的全局数据对每个载入它的进程来说都是相同的；而在Win32环境中，情况却发生了变化，DLL函数中的代码所创建的任何对象（包括变量）都归调用它的线程或进程所有。当进程在载入DLL时，操作系统自动把DLL地址映射到该进程的私有空间，也就是进程的虚拟地址空间，而且也复制该DLL的全局数据的一份拷贝到该进程空间。也就是说每个进程所拥有的相同的DLL的全局数据，它们的名称相同，但其值却并不一定是相同的，而且是互不干涉的。
因此，在Win32环境下要想在多个进程中共享数据，就必须进行必要的设置。在访问同一个Dll的各进程之间共享存储器是通过存储器映射文件技术实现的。也可以把这些需要共享的数据分离出来，放置在一个独立的数据段里，并把该段的属性设置为共享。必须给这些变量赋初值，否则编译器会把没有赋初始值的变量放在一个叫未被初始化的数据段中。
#pragma data_seg预处理指令用于设置共享数据段。例如：
#pragma data_seg("SharedDataName")
HHOOK hHook=NULL;
#pragma data_seg()
在#pragma data_seg("SharedDataName")和#pragma data_seg()之间的所有变量将被访问该Dll的所有进程看到和共享。再加上一条指令#pragma comment(linker,"/section:.SharedDataName,rws"),那么这个数据节中的数据可以在所有DLL的实例之间共享。所有对这些数据的操作都针对同一个实例的，而不是在每个进程的地址空间中都有一份。
当进程隐式或显式调用一个动态库里的函数时，系统都要把这个动态库映射到这个进程的虚拟地址空间里(以下简称"地址空间")。这使得DLL成为进程的一部分，以这个进程的身份执行，使用这个进程的堆栈。
4、系统钩子与线程钩子：
SetWindowsHookEx()函数的最后一个参数决定了此钩子是系统钩子还是线程钩子。
线程勾子用于监视指定线程的事件消息。线程勾子一般在当前线程或者当前线程派生的线程内。
系统勾子监视系统中的所有线程的事件消息。因为系统勾子会影响系统中所有的应用程序，所以勾子函数必须放在独立的动态链接库(DLL) 中。系统自动将包含"钩子回调函数"的DLL映射到受钩子函数影响的所有进程的地址空间中，即将这个DLL注入了那些进程。
几点说明：
（1）如果对于同一事件（如鼠标消息）既安装了线程勾子又安装了系统勾子，那么系统会自动先调用线程勾子，然后调用系统勾子。 
（2）对同一事件消息可安装多个勾子处理过程，这些勾子处理过程形成了勾子链。当前勾子处理结束后应把勾子信息传递给下一个勾子函数。 
（3）勾子特别是系统勾子会消耗消息处理时间，降低系统性能。只有在必要的时候才安装勾子，在使用完毕后要及时卸载。
钩子类型
每一种类型的Hook可以使应用程序能够监视不同类型的系统消息处理机制。下面描述所有可以利用的Hook类型。
1、WH_CALLWNDPROC和WH_CALLWNDPROCRET Hooks
WH_CALLWNDPROC和WH_CALLWNDPROCRET Hooks使你可以监视发送到窗口过程的消息。系统在消息发送到接收窗口过程之前调用WH_CALLWNDPROC Hook子程，并且在窗口过程处理完消息之后调用WH_CALLWNDPROCRET Hook子程。
WH_CALLWNDPROCRET Hook传递指针到CWPRETSTRUCT结构，再传递到Hook子程。
CWPRETSTRUCT结构包含了来自处理消息的窗口过程的返回值，同样也包括了与这个消息关联的消息参数。
2、WH_CBT Hook
在以下事件之前，系统都会调用WH_CBT Hook子程，这些事件包括：
1. 激活，建立，销毁，最小化，最大化，移动，改变尺寸等窗口事件；
2. 完成系统指令；
3. 来自系统消息队列中的移动鼠标，键盘事件；
4. 设置输入焦点事件；
5. 同步系统消息队列事件。
Hook子程的返回值确定系统是否允许或者防止这些操作中的一个。
3、WH_DEBUG Hook
在系统调用系统中与其他Hook关联的Hook子程之前，系统会调用WH_DEBUG Hook子程。你可以使用这个Hook来决定是否允许系统调用与其他Hook关联的Hook子程。
4、WH_FOREGROUNDIDLE Hook
当应用程序的前台线程处于空闲状态时，可以使用WH_FOREGROUNDIDLE Hook执行低优先级的任务。当应用程序的前台线程大概要变成空闲状态时，系统就会调用WH_FOREGROUNDIDLE Hook子程。
5、WH_GETMESSAGE Hook
应用程序使用WH_GETMESSAGE Hook来监视从GetMessage or PeekMessage函数返回的消息。你可以使用WH_GETMESSAGE Hook去监视鼠标和键盘输入，以及其他发送到消息队列中的消息。
6、WH_JOURNALPLAYBACK Hook
WH_JOURNALPLAYBACK Hook使应用程序可以插入消息到系统消息队列。可以使用这个Hook回放通过使用WH_JOURNALRECORD Hook记录下来的连续的鼠标和键盘事件。只要WH_JOURNALPLAYBACK Hook已经安装，正常的鼠标和键盘事件就是无效的。
WH_JOURNALPLAYBACK Hook是全局Hook，它不能象线程特定Hook一样使用。
WH_JOURNALPLAYBACK Hook返回超时值，这个值告诉系统在处理来自回放Hook当前消息之前需要等待多长时间（毫秒）。这就使Hook可以控制实时事件的回放。
WH_JOURNALPLAYBACK是system-wide local hooks，它們不會被注射到任何行程位址空間。
7、WH_JOURNALRECORD Hook
WH_JOURNALRECORD Hook用来监视和记录输入事件。典型的，可以使用这个Hook记录连续的鼠标和键盘事件，然后通过使用WH_JOURNALPLAYBACK Hook来回放。
WH_JOURNALRECORD Hook是全局Hook，它不能象线程特定Hook一样使用。
WH_JOURNALRECORD是system-wide local hooks，它們不會被注射到任何行程位址空間。
8、WH_KEYBOARD Hook
在应用程序中，WH_KEYBOARD Hook用来监视WM_KEYDOWN and WM_KEYUP消息，这些消息通过GetMessage or PeekMessage function返回。可以使用这个Hook来监视输入到消息队列中的键盘消息。
9、WH_KEYBOARD_LL Hook
WH_KEYBOARD_LL Hook监视输入到线程消息队列中的键盘消息。
10、WH_MOUSE Hook
WH_MOUSE Hook监视从GetMessage 或者 PeekMessage 函数返回的鼠标消息。使用这个Hook监视输入到消息队列中的鼠标消息。
11、WH_MOUSE_LL Hook
WH_MOUSE_LL Hook监视输入到线程消息队列中的鼠标消息。
12、WH_MSGFILTER 和 WH_SYSMSGFILTER Hooks
WH_MSGFILTER 和 WH_SYSMSGFILTER Hooks使我们可以监视菜单，滚动条，消息框，对话框消息并且发现用户使用ALT+TAB or ALT+ESC 组合键切换窗口。WH_MSGFILTER Hook只能监视传递到菜单，滚动条，消息框的消息，以及传递到通过安装了Hook子程的应用程序建立的对话框的消息。WH_SYSMSGFILTER Hook监视所有应用程序消息。
WH_MSGFILTER 和 WH_SYSMSGFILTER Hooks使我们可以在模式循环期间过滤消息，这等价于在主消息循环中过滤消息。
通过调用CallMsgFilter function可以直接的调用WH_MSGFILTER Hook。通过使用这个函数，应用程序能够在模式循环期间使用相同的代码去过滤消息，如同在主消息循环里一样。
13、WH_SHELL Hook
外壳应用程序可以使用WH_SHELL Hook去接收重要的通知。当外壳应用程序是激活的并且当顶层窗口建立或者销毁时，系统调用WH_SHELL Hook子程。
WH_SHELL 共有５钟情況：
1. 只要有个top-level、unowned 窗口被产生、起作用、或是被摧毁；
2. 当Taskbar需要重画某个按钮；
3. 当系统需要显示关于Taskbar的一个程序的最小化形式；
4. 当目前的键盘布局状态改变；
5. 当使用者按Ctrl+Esc去执行Task Manager（或相同级别的程序）。
按照惯例，外壳应用程序都不接收WH_SHELL消息。所以，在应用程序能够接收WH_SHELL消息之前，应用程序必须调用SystemParametersInfo function注册它自己。
作者
姜山 2002年11月7日
