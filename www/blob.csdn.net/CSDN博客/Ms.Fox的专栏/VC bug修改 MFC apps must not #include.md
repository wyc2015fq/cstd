# VC bug修改  MFC apps must not #include  - Ms.Fox的专栏 - CSDN博客
2014年07月15日 19:23:56[lulu-lu](https://me.csdn.net/smbluesky)阅读数：487
error WINDOWS.H already included.  MFC apps must not #include <windows.h>
以下转自CSDN论坛：[http://topic.csdn.net/t/20020425/20/676815.html](http://topic.csdn.net/t/20020425/20/676815.html)
compile   error:  
  c:\program   files\microsoft   visual   studio\vc98\mfc\include\afxv_w32.h(14)   :  
  fatal   error   C1189:   #error   :     WINDOWS.H   already   included.     MFC   apps   must  not   #include   <windows.h>  
  如果编译器在编译afxv_w32.h文件之前编译了windows.h文件，编译器会报上面的错误，因为在afxv_w32.h文件中有下面的一句预编译报警：  
  #ifdef   _WINDOWS_  
  #error   WINDOWS.H   already   included.     MFC   apps   must   not   #include   <windows.h>  
  #endif  
  问题在于为什么afxv_w32.h中要有这么一句预编译处理。看了afxv_w32.h和windows.h文件就有点明白了。  
  在afxv_w32.h中有下面的预编译语句：  
  ...   ...  
  #undef   NOLOGERROR  
  #undef   NOPROFILER  
  #undef   NOMEMMGR  
  #undef   NOLFILEIO  
  #undef   NOOPENFILE  
  #undef   NORESOURCE  
  #undef   NOATOM  
  ...   ...  
  在afxv_w32.h中还有一句：  
  #include   "windows.h"  
  而在windows.h文件中有下面的预编译语句：  
  ...   ...  
  #define   NOATOM  
  #define   NOGDI  
  #define   NOGDICAPMASKS  
  #define   NOMETAFILE  
  #define   NOMINMAX  
  #define   NOMSG  
  #define   NOOPENFILE  
  ...   ...  
  注意到在windows.h的开头有防止windows.h被重复编译的预编译开关：  
  #ifndef   _WINDOWS_  
  #define   _WINDOWS_  
  这样问题就明白了，虽然我不知道微软为什么要这么做，但是我知道如果在afxv_w32.h没有那句预编译报警，那么如果在编译afxv_w32.h之前  
  编译了windows.h，那么在windows.h中#define的NOATOM等宏就会被#undef掉，可能会导致相应的错误发生。  
  猜想原因可能如上所述，我的解决方法是，将包含有#include   “windows.h"的头文件放在所有包含的头文件的最后面，这样使得对afxv_w32文件  
  的编译处理发生在先，这样，由于在afxv_w32.h中已经包含了windows.h，那么宏_WINDOWS_将被定义，后继的#include   "windows.h"语句将形同虚设，  
  上面的编译报警也不会发生了。我觉得这种处理要比将所有的#include   "windows.h”语句删掉要好一点。  
  一句话，编译器必须在编译windows.h之前编译afxv_w32.h,因为我不是十分清除什么时候afxv_w32.h会被编译，所以我将可能包含有#include   "windows.h"的头文件放在其他头文件之后#include。
解释的不错。
