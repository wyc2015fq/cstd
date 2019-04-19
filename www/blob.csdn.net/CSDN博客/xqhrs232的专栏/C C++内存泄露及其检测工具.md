# C/C++内存泄露及其检测工具 - xqhrs232的专栏 - CSDN博客
2013年03月19日 23:21:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：738
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
原文地址::[http://blog.csdn.net/looob/article/details/5734606](http://blog.csdn.net/looob/article/details/5734606)
对于一个c/c++程序员来说，内存泄漏是一个常见的也是令人头疼的问题。已经有许多技术被研究出来以应对这个问题，比如 Smart Pointer，Garbage Collection等。Smart Pointer技术比较成熟，STL中已经包含支持Smart Pointer的class，但是它的使用似乎并不广泛，而且它也不能解决所有的问题；Garbage Collection技术在java中已经比较成熟，但是在c/c++领域的发展并不顺畅，虽然很早就有人思考在C++中也加入GC的支持。现实世界就是这样的，作为一个c/c++程序员，内存泄漏是你心中永远的痛。不过好在现在有许多工具能够帮助我们验证内存泄漏的存在，找出发生问题的代码。
　　内存泄漏的定义
　 　一般我们常说的内存泄漏是指堆内存的泄漏。堆内存是指程序从堆中分配的，大小任意的（内存块的大小可以在程序运行期决定），使用完后必须显示释放的内 存。应用程序一般使用malloc，realloc，new等函数从堆中分配到一块内存，使用完后，程序必须负责相应的调用free或delete释放该 内存块，否则，这块内存就不能被再次使用，我们就说这块内存泄漏了。以下这段小程序演示了堆内存发生泄漏的情形：
void MyFunction(int nSize)
{
　char* p= new char[nSize];
　if( !GetStringFrom( p, nSize ) ){
　　MessageBox(“Error”);
　　return;
　}
　…//using the string pointed by p;
　delete p;
}
　　例一
　　当函数GetStringFrom()返回零的时候，指针p指向的内存就不会被释放。这是一种常见的发生内存泄漏的情形。程序在入口处分配内存，在出口处释放内存，但是c函数可以在任何地方退出，所以一旦有某个出口处没有释放应该释放的内存，就会发生内存泄漏。
　 　广义的说，内存泄漏不仅仅包含堆内存的泄漏，还包含系统资源的泄漏(resource leak)，比如核心态HANDLE，GDI Object，SOCKET， Interface等，从根本上说这些由操作系统分配的对象也消耗内存，如果这些对象发生泄漏最终也会导致内存的泄漏。而且，某些对象消耗的是核心态内 存，这些对象严重泄漏时会导致整个操作系统不稳定。所以相比之下，系统资源的泄漏比堆内存的泄漏更为严重。
　　GDI Object的泄漏是一种常见的资源泄漏：
void CMyView::OnPaint( CDC* pDC )
{
　CBitmap bmp;
　CBitmap* pOldBmp;
　bmp.LoadBitmap(IDB_MYBMP);
　pOldBmp = pDC->SelectObject( &bmp );
　…
　if( Something() ){
　　return;
　}
　pDC->SelectObject( pOldBmp );
　return;
}
　　例二
　 　当函数Something()返回非零的时候，程序在退出前没有把pOldBmp选回pDC中，这会导致pOldBmp指向的HBITMAP对象发生泄 漏。这个程序如果长时间的运行，可能会导致整个系统花屏。这种问题在Win9x下比较容易暴露出来，因为Win9x的GDI堆比Win2k或NT的要小很 多。
　　内存泄漏的发生方式：
　　以发生的方式来分类，内存泄漏可以分为4类：
　　1. 常发性内存泄漏。发生内存泄漏的代码会被多次执行到，每次被执行的时候都会导致一块内存泄漏。比如例二，如果Something()函数一直返回True，那么pOldBmp指向的HBITMAP对象总是发生泄漏。
　 　2. 偶发性内存泄漏。发生内存泄漏的代码只有在某些特定环境或操作过程下才会发生。比如例二，如果Something()函数只有在特定环境下才返回 True，那么pOldBmp指向的HBITMAP对象并不总是发生泄漏。常发性和偶发性是相对的。对于特定的环境，偶发性的也许就变成了常发性的。所以 测试环境和测试方法对检测内存泄漏至关重要。
　　3. 一次性内存泄漏。发生内存泄漏的代码只会被执行一次，或者由于算法上的缺陷，导致总会有一块仅且一块内存发生泄漏。比如，在类的构造函数中分配内存，在析 构函数中却没有释放该内存，但是因为这个类是一个Singleton，所以内存泄漏只会发生一次。另一个例子：
char* g_lpszFileName = NULL;
void SetFileName( const char* lpcszFileName )
{
　if( g_lpszFileName ){
　　free( g_lpszFileName );
　}
　g_lpszFileName = strdup( lpcszFileName );
}
　　例三
　　如果程序在结束的时候没有释放g_lpszFileName指向的字符串，那么，即使多次调用SetFileName()，总会有一块内存，而且仅有一块内存发生泄漏。
　 　4. 隐式内存泄漏。程序在运行过程中不停的分配内存，但是直到结束的时候才释放内存。严格的说这里并没有发生内存泄漏，因为最终程序释放了所有申请的内存。但 是对于一个服务器程序，需要运行几天，几周甚至几个月，不及时释放内存也可能导致最终耗尽系统的所有内存。所以，我们称这类内存泄漏为隐式内存泄漏。举一 个例子：
class Connection
{
　public:
　　Connection( SOCKET s);
　　~Connection();
　　…
　private:
　　SOCKET _socket;
　　…
};
class ConnectionManager
{
　public:
　　ConnectionManager(){}
　　~ConnectionManager(){
　　　list::iterator it;
　　　for( it = _connlist.begin(); it != _connlist.end(); ++it ){
　　　　delete （*it）;
　　　}
　　　_connlist.clear();
　　}
　　void OnClientConnected( SOCKET s ){
　　　Connection* p = new Connection(s);
　　　_connlist.push_back(p);
　　}
　　void OnClientDisconnected( Connection* pconn ){
　　　_connlist.remove( pconn );
　　　delete pconn;
　　}
　private:
　　list _connlist;
};
　　例四
　 　假设在Client从Server端断开后，Server并没有呼叫OnClientDisconnected()函数，那么代表那次连接的 Connection对象就不会被及时的删除（在Server程序退出的时候，所有Connection对象会在ConnectionManager的析 构函数里被删除）。当不断的有连接建立、断开时隐式内存泄漏就发生了。
　　从用户使用程序的角度来看，内存泄漏本身不会产生什么危害，作 为一般的用户，根本感觉不到内存泄漏的存在。真正有危害的是内存泄漏的堆积，这会最终消耗尽系统所有的内存。从这个角度来说，一次性内存泄漏并没有什么危 害，因为它不会堆积，而隐式内存泄漏危害性则非常大，因为较之于常发性和偶发性内存泄漏它更难被检测到。 
检测内存泄漏
　　检测内存泄漏的关键是要能截获住对分配内存和释放内存的函数的调用。截获住这两个函数，我们就能跟踪每一 块内存的生命周期，比如，每当成功的分配一块内存后，就把它的指针加入一个全局的list中；每当释放一块内存，再把它的指针从list中删除。这样，当 程序结束的时候，list中剩余的指针就是指向那些没有被释放的内存。这里只是简单的描述了检测内存泄漏的基本原理，详细的算法可以参见Steve Maguire的<<Writing Solid Code>>。
　　如果要检测堆内存的泄漏，那么需要截获住 malloc/realloc/free和new/delete就可以了（其实new/delete最终也是用malloc/free的，所以只要截获前 面一组即可）。对于其他的泄漏，可以采用类似的方法，截获住相应的分配和释放函数。比如，要检测BSTR的泄漏，就需要截获 SysAllocString/SysFreeString；要检测HMENU的泄漏，就需要截获CreateMenu/ DestroyMenu。（有的资源的分配函数有多个，释放函数只有一个，比如，SysAllocStringLen也可以用来分配BSTR，这时就需要
 截获多个分配函数）
　　在Windows平台下，检测内存泄漏的工具常用的一般有三种，MS C-Runtime Library内建的检测功能；外挂式的检测工具，诸如，Purify，BoundsChecker等；利用Windows NT自带的Performance Monitor。这三种工具各有优缺点，MS C-Runtime Library虽然功能上较之外挂式的工具要弱，但是它是免费的；Performance Monitor虽然无法标示出发生问题的代码，但是它能检测出隐式的内存泄漏的存在，这是其他两类工具无能为力的地方。
　　以下我们详细讨论这三种检测工具：
　　VC下内存泄漏的检测方法
　　用MFC开发的应用程序，在DEBUG版模式下编译后，都会自动加入内存泄漏的检测代码。在程序结束后，如果发生了内存泄漏，在Debug窗口中会显示出所有发生泄漏的内存块的信息，以下两行显示了一块被泄漏的内存块的信息：
E:/TestMemLeak/TestDlg.cpp(70) : {59} normal block at 0x00881710, 200 bytes long.
Data: <abcdefghijklmnop> 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F 70
　 　第一行显示该内存块由TestDlg.cpp文件，第70行代码分配，地址在0x00881710，大小为200字节，{59}是指调用内存分配函数的 Request Order，关于它的详细信息可以参见MSDN中_CrtSetBreakAlloc()的帮助。第二行显示该内存块前16个字节的内容，尖括号内是以 ASCII方式显示，接着的是以16进制方式显示。
　　一般大家都误以为这些内存泄漏的检测功能是由MFC提供的，其实不然。MFC只是 封装和利用了MS C-Runtime Library的Debug Function。非MFC程序也可以利用MS C-Runtime Library的Debug Function加入内存泄漏的检测功能。MS C-Runtime Library在实现malloc/free，strdup等函数时已经内建了内存泄漏的检测功能。
　　注意观察一下由MFC Application Wizard生成的项目，在每一个cpp文件的头部都有这样一段宏定义：
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
　　有了这样的定义，在编译DEBUG版时，出现在这个cpp文件中的所有new都被替换成DEBUG_NEW了。那么DEBUG_NEW是什么呢？DEBUG_NEW也是一个宏，以下摘自afx.h，1632行
#define DEBUG_NEW new(THIS_FILE, __LINE__)
　　所以如果有这样一行代码：
char* p = new char[200];
　　经过宏替换就变成了：
char* p = new( THIS_FILE, __LINE__)char[200];
　　根据C++的标准，对于以上的new的使用方法，编译器会去找这样定义的operator new：
void* operator new(size_t, LPCSTR, int)
　　我们在afxmem.cpp 63行找到了一个这样的operator new 的实现
void* AFX_CDECL operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
{
　return ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
}
void* __cdecl operator new(size_t nSize, int nType, LPCSTR lpszFileName, int nLine)
{
　…
　pResult = _malloc_dbg(nSize, nType, lpszFileName, nLine);
　if (pResult != NULL)
　　return pResult;
　…
}
　 　第二个operator new函数比较长，为了简单期间，我只摘录了部分。很显然最后的内存分配还是通过_malloc_dbg函数实现的，这个函数属于MS C-Runtime Library 的Debug Function。这个函数不但要求传入内存的大小，另外还有文件名和行号两个参数。文件名和行号就是用来记录此次分配是由哪一段代码造成的。如果这块内 存在程序结束之前没有被释放，那么这些信息就会输出到Debug窗口里。
　　这里顺便提一下THIS_FILE，__FILE和 __LINE__。__FILE__和__LINE__都是编译器定义的宏。当碰到__FILE__时，编译器会把__FILE__替换成一个字符串，这 个字符串就是当前在编译的文件的路径名。当碰到__LINE__时，编译器会把__LINE__替换成一个数字，这个数字就是当前这行代码的行号。在 DEBUG_NEW的定义中没有直接使用__FILE__，而是用了THIS_FILE，其目的是为了减小目标文件的大小。假设在某个cpp文件中有 100处使用了new，如果直接使用__FILE__，那编译器会产生100个常量字符串，这100个字符串都是飧?/SPAN>cpp文件的路径
 名，显然十分冗余。如果使用THIS_FILE，编译器只会产生一个常量字符串，那100处new的调用使用的都是指向常量字符串的指针。
　 　再次观察一下由MFC Application Wizard生成的项目，我们会发现在cpp文件中只对new做了映射，如果你在程序中直接使用malloc函数分配内存，调用malloc的文件名和行 号是不会被记录下来的。如果这块内存发生了泄漏，MS C-Runtime Library仍然能检测到，但是当输出这块内存块的信息，不会包含分配它的的文件名和行号。
　　要在非MFC程序中打开内存泄漏的检测功能非常容易，你只要在程序的入口处加入以下几行代码：
int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG );
tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
_CrtSetDbgFlag( tmpFlag );
　　这样，在程序结束的时候，也就是winmain，main或dllmain函数返回之后，如果还有内存块没有释放，它们的信息会被打印到Debug窗口里。
　　如果你试着创建了一个非MFC应用程序，而且在程序的入口处加入了以上代码，并且故意在程序中不释放某些内存块，你会在Debug窗口里看到以下的信息：
{47} normal block at 0x00C91C90, 200 bytes long.
Data: < > 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
　　内存泄漏的确检测到了，但是和上面MFC程序的例子相比，缺少了文件名和行号。对于一个比较大的程序，没有这些信息，解决问题将变得十分困难。
　　为了能够知道泄漏的内存块是在哪里分配的，你需要实现类似MFC的映射功能，把new，maolloc等函数映射到_malloc_dbg函数上。这里我不再赘述，你可以参考MFC的源代码。
　 　由于Debug Function实现在MS C-RuntimeLibrary中，所以它只能检测到堆内存的泄漏，而且只限于malloc，realloc或strdup等分配的内存，而那些系统资 源，比如HANDLE，GDI Object，或是不通过C-Runtime Library分配的内存，比如VARIANT，BSTR的泄漏，它是无法检测到的，这是这种检测法的一个重大的局限性。另外，为了能记录内存块是在哪里 分配的，源代码必须相应的配合，这在调试一些老的程序非常麻烦，毕竟修改源代码不是一件省心的事，这是这种检测法的另一个局限性。
　　对 于开发一个大型的程序，MS C-Runtime Library提供的检测功能是远远不够的。接下来我们就看看外挂式的检测工具。我用的比较多的是BoundsChecker，一则因为它的功能比较全 面，更重要的是它的稳定性。这类工具如果不稳定，反而会忙里添乱。到底是出自鼎鼎大名的NuMega，我用下来基本上没有什么大问题。
 使用BoundsChecker检测内存泄漏：
　　BoundsChecker采用一种被称为 Code Injection的技术，来截获对分配内存和释放内存的函数的调用。简单地说，当你的程序开始运行时，BoundsChecker的DLL被自动载入进 程的地址空间（这可以通过system-level的Hook实现），然后它会修改进程中对内存分配和释放的函数调用，让这些调用首先转入它的代码，然后 再执行原来的代码。BoundsChecker在做这些动作的时，无须修改被调试程序的源代码或工程配置文件，这使得使用它非常的简便、直接。
　　这里我们以malloc函数为例，截获其他的函数方法与此类似。
　　需要被截获的函数可能在DLL中，也可能在程序的代码里。比如，如果静态连结C-Runtime Library，那么malloc函数的代码会被连结到程序里。为了截获住对这类函数的调用，BoundsChecker会动态修改这些函数的指令。
　　以下两段汇编代码，一段没有BoundsChecker介入，另一段则有BoundsChecker的介入：
126: _CRTIMP void * __cdecl malloc (
127: size_t nSize
128: )
129: {
00403C10 push ebp
00403C11 mov ebp,esp
130: return _nh_malloc_dbg(nSize, _newmode, _NORMAL_BLOCK, NULL, 0);
00403C13 push 0
00403C15 push 0
00403C17 push 1
00403C19 mov eax,[__newmode (0042376c)]
00403C1E push eax
00403C1F mov ecx,dword ptr [nSize]
00403C22 push ecx
00403C23 call _nh_malloc_dbg (00403c80)
00403C28 add esp,14h
131: }
　　以下这一段代码有BoundsChecker介入：
126: _CRTIMP void * __cdecl malloc (
127: size_t nSize
128: )
129: {
00403C10 jmp 01F41EC8
00403C15 push 0
00403C17 push 1
00403C19 mov eax,[__newmode (0042376c)]
00403C1E push eax
00403C1F mov ecx,dword ptr [nSize]
00403C22 push ecx
00403C23 call _nh_malloc_dbg (00403c80)
00403C28 add esp,14h
131: }
　 　当BoundsChecker介入后，函数malloc的前三条汇编指令被替换成一条jmp指令，原来的三条指令被搬到地址01F41EC8处了。当程 序进入malloc后先jmp到01F41EC8，执行原来的三条指令，然后就是BoundsChecker的天下了。大致上它会先记录函数的返回地址 （函数的返回地址在stack上，所以很容易修改），然后把返回地址指向属于BoundsChecker的代码，接着跳到malloc函数原来的指令，也 就是在00403c15的地方。当malloc函数结束的时候，由于返回地址被修改，它会返回到BoundsChecker的代码中，此时
 BoundsChecker会记录由malloc分配的内存的指针，然后再跳转到到原来的返回地址去。
　　如果内存分配/释放函数在DLL中，BoundsChecker则采用另一种方法来截获对这些函数的调用。BoundsChecker通过修改程序的DLL Import Table让table中的函数地址指向自己的地址，以达到截获的目的。
　 　截获住这些分配和释放函数，BoundsChecker就能记录被分配的内存或资源的生命周期。接下来的问题是如何与源代码相关，也就是说当 BoundsChecker检测到内存泄漏，它如何报告这块内存块是哪段代码分配的。答案是调试信息（Debug Information）。当我们编译一个Debug版的程序时，编译器会把源代码和二进制代码之间的对应关系记录下来，放到一个单独的文件里 (.pdb)或者直接连结进目标程序，通过直接读取调试信息就能得到分配某块内存的源代码在哪个文件，哪一行上。使用Code Injection和Debug
 Information，使BoundsChecker不但能记录呼叫分配函数的源代码的位置，而且还能记录分配时的Call Stack，以及Call Stack上的函数的源代码位置。这在使用像MFC这样的类库时非常有用，以下我用一个例子来说明：
void ShowXItemMenu()
{
　…
　CMenu menu;
　menu.CreatePopupMenu();
　//add menu items.
　menu.TrackPropupMenu();
　…
}
void ShowYItemMenu( )
{
　…
　CMenu menu;
　menu.CreatePopupMenu();
　//add menu items.
　menu.TrackPropupMenu();
　menu.Detach();//this will cause HMENU leak
　…
}
BOOL CMenu::CreatePopupMenu()
{
　…
　hMenu = CreatePopupMenu();
　…
}
　 　当调用ShowYItemMenu()时，我们故意造成HMENU的泄漏。但是，对于BoundsChecker来说被泄漏的HMENU是在class CMenu::CreatePopupMenu()中分配的。假设的你的程序有许多地方使用了CMenu的CreatePopupMenu()函数，如 CMenu::CreatePopupMenu()造成的，你依然无法确认问题的根结到底在哪里，在ShowXItemMenu()中还是在 ShowYItemMenu()中，或者还有其它的地方也使用了CreatePopupMenu()？有了Call
 Stack的信息，问题就容易了。BoundsChecker会如下报告泄漏的HMENU的信息：
Function
File
Line
CMenu::CreatePopupMenu
E:/8168/vc98/mfc/mfc/include/afxwin1.inl
1009
ShowYItemMenu
E:/testmemleak/mytest.cpp
100
　　这里省略了其他的函数调用
　　如此，我们很容易找到发生问题的函数是ShowYItemMenu()。当使用MFC之类的类库编程时，大部分的API调用都被封装在类库的class里，有了Call Stack信息，我们就可以非常容易的追踪到真正发生泄漏的代码。
　　记录Call Stack信息会使程序的运行变得非常慢，因此默认情况下BoundsChecker不会记录Call Stack信息。可以按照以下的步骤打开记录Call Stack信息的选项开关：
　　1. 打开菜单：BoundsChecker|Setting…
　　2. 在Error Detection页中，在Error Detection Scheme的List中选择Custom
　　3. 在Category的Combox中选择 Pointer and leak error check
　　4. 钩上Report Call Stack复选框
　　5. 点击Ok
　　基于Code Injection，BoundsChecker还提供了API Parameter的校验功能，memory over run等功能。这些功能对于程序的开发都非常有益。由于这些内容不属于本文的主题，所以不在此详述了。
　　尽管BoundsChecker的功能如此强大，但是面对隐式内存泄漏仍然显得苍白无力。所以接下来我们看看如何用Performance Monitor检测内存泄漏。
　　使用Performance Monitor检测内存泄漏
　　NT的内核在设计过程中已经加入了系统监视功能，比如CPU的使用率，内存的使用情况，I/O操作的频繁度等都作为一个个Counter，应用程序可以通过读取这些Counter了解整个系统的或者某个进程的运行状况。Performance Monitor就是这样一个应用程序。
　 　为了检测内存泄漏，我们一般可以监视Process对象的Handle Count，Virutal Bytes 和Working Set三个Counter。Handle Count记录了进程当前打开的HANDLE的个数，监视这个Counter有助于我们发现程序是否有Handle泄漏；Virtual Bytes记录了该进程当前在虚地址空间上使用的虚拟内存的大小，NT的内存分配采用了两步走的方法，首先，在虚地址空间上保留一段空间，这时操作系统并 没有分配物理内存，只是保留了一段地址。然后，再提交这段空间，这时操作系统才会分配物理内存。所以，Virtual
 Bytes一般总大于程序的Working Set。监视Virutal Bytes可以帮助我们发现一些系统底层的问题; Working Set记录了操作系统为进程已提交的内存的总量，这个值和程序申请的内存总量存在密切的关系，如果程序存在内存的泄漏这个值会持续增加，但是 Virtual Bytes却是跳跃式增加的。
　　监视这些Counter可以让我们了解进程使用内存的情况，如果发生了泄漏，即使是隐 式内存泄漏，这些Counter的值也会持续增加。但是，我们知道有问题却不知道哪里有问题，所以一般使用Performance Monitor来验证是否有内存泄漏，而使用BoundsChecker来找到和解决。
　　当Performance Monitor显示有内存泄漏，而BoundsChecker却无法检测到，这时有两种可能：第一种，发生了偶发性内存泄漏。这时你要确保使用 Performance Monitor和使用BoundsChecker时，程序的运行环境和操作方法是一致的。第二种，发生了隐式的内存泄漏。这时你要重新审查程序的设计，然 后仔细研究Performance Monitor记录的Counter的值的变化图，分析其中的变化和程序运行逻辑的关系，找到一些可能的原因。这是一个痛苦的过程，充满了假设、猜想、验
 证、失败，但这也是一个积累经验的绝好机会。
　　总结
　　内存泄漏是个大而复杂的问题，即使是Java和.Net这样有 Gabarge Collection机制的环境，也存在着泄漏的可能，比如隐式内存泄漏。由于篇幅和能力的限制，本文只能对这个主题做一个粗浅的研究。其他的问题，比如 多模块下的泄漏检测，如何在程序运行时对内存使用情况进行分析等等，都是可以深入研究的题目
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/rem2002/archive/2009/01/14/3777455.aspx](http://blog.csdn.net/rem2002/archive/2009/01/14/3777455.aspx)
