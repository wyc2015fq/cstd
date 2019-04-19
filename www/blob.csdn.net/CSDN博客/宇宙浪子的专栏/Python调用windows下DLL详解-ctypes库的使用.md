# Python调用windows下DLL详解 - ctypes库的使用 - 宇宙浪子的专栏 - CSDN博客
2014年09月05日 16:05:44[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：9898
在python中某些时候需要C做效率上的补充，在实际应用中，需要做部分数据的交互。使用python中的ctypes模块可以很方便的调用windows的dll（也包括linux下的so等文件），下面将详细的讲解这个模块（以windows平台为例子），当然我假设你们已经对windows下怎么写一个DLL是没有问题的。
引入ctypes库 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- from ctypes import *   
假设你已经有了一个的DLL（名字是add.dll)，且该DLL有一个符合cdecl（这里强调调用约定是因为，stdcall调用约定和cdecl调用约定声明的导出函数，在使用python加载时使用的加载函数是不同的，后面会有说明）调用约定的导出函数Add。
建立一个Python文件DllCall.py测试： 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- from ctypes import *  
- dll = CDLL("add.dll")  
- print dll.Add(1, 102)  
结果：103 
上面是一个简单的例子。下面简单聊一下调用流程： 
**1、加载DLL**
上面已经说过，加载的时候要根据你将要调用的函数是符合什么调用约定的。 
stdcall调用约定：两种加载方式 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- Objdll = ctypes.windll.LoadLibrary("dllpath")  
- Objdll = ctypes.WinDLL("dllpath")   
cdecl调用约定：也有两种加载方式 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- Objdll = ctypes.cdll.LoadLibrary("dllpath")  
- Objdll = ctypes.CDLL("dllpath")  
- <span style="font-family:Microsoft YaHei;">其实windll和cdll分别是WinDLL类和CDll类的对象。</span>  
**2、调用dll中的方法**
在1中加载dll的时候会返回一个DLL对象（假设名字叫Objdll），利用该对象就可以调用dll中的方法。 
e.g.如果dll中有个方法名字叫Add（注意如果经过stdcall声明的方法，如果不是用def文件声明的导出函数或者extern “C” 声明的话，编译器会对函数名进行修改，这个要注意，我想你们懂的。）
调用：nRet = Objdll.Add(12, 15) 即完成一次调用。 
看起来调用似乎很简单，不要只看表象，呵呵，这是因为Add这个函数太简单了，现在假设函数需要你传入一个int类型的指针（int*），可以通过库中的byref关键字来实现，假设现在调用的函数的第三个参数是个int类型的指针。
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- intPara = c_int(9)  
- dll.sub(23, 102, byref(intPara))  
- print intPara.value  
如果是要传入一个char缓冲区指针，和缓冲区长度，方法至少有四种： 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- # 方法1
- szPara = create_string_buffer('/0'*100)  
- dll.PrintInfo(byref(szPara), 100);  
- print szPara.value  
- 
- # 方法2
- sBuf = 'aaaaaaaaaabbbbbbbbbbbbbb'
- pStr = c_char_p( )  
- pStr.value = sBuf  
- #pVoid = ctypes.cast( pStr, ctypes.c_void_p ).value
- dll.PrintInfo(pStr, len(pStr.value))  
- print pStr.value  
- 
- # 方法3
- strMa = "/0"*20
- FunPrint  = dll.PrintInfo  
- FunPrint.argtypes = [c_char_p, c_int]  
- #FunPrint.restypes = c_void_p
- nRst = FunPrint(strMa, len(strMa))  
- print strMa,len(strMa)  
- 
- # 方法4
- pStr2 = c_char_p("/0")  
- print pStr2.value  
- #pVoid = ctypes.cast( pStr, ctypes.c_void_p ).value
- dll.PrintInfo(pStr2, len(pStr.value))  
- print pStr2.value  
**3、C基本类型和ctypes中实现的类型映射表**
ctypes数据类型          C数据类型 
c_char                          char 
c_short                         short 
c_int                             int 
c_long                          long 
c_ulong                        unsign long 
c_float                          float 
c_double                      double 
c_void_p                       void 
对应的指针类型是在后面加上"_p"，如int*是c_int_p等等。 
在python中要实现c语言中的结构，需要用到类。 
**4、DLL中的函数返回一个指针。**
虽然这不是个好的编程方法，不过这种情况的处理方法也很简单，其实返回的都是地址，把他们转换相应的python类型，再通过value属性访问。 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- pchar = dll.getbuffer()  
- szbuffer = c_char_p(pchar)  
- print szbuffer.value  
**5、处理C中的结构体类型**
为什么把这个单独提出来说呢，因为这个是最麻烦也是最复杂的，在python里面申明一个类似c的结构体，要用到类，并且这个类必须继承自Structure。 
先看一个简单的例子： 
C里面dll的定义如下： 
**[cpp]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- typedefstruct _SimpleStruct  
- {  
- int    nNo;  
- float  fVirus;  
- char   szBuffer[512];  
- } SimpleStruct, *PSimpleStruct;  
- typedefconst SimpleStruct*  PCSimpleStruct;  
- 
- extern"C"int__declspec(dllexport) PrintStruct(PSimpleStruct simp);  
- int PrintStruct(PSimpleStruct simp)  
- {  
-     printf ("nMaxNum=%f, szContent=%s", simp->fVirus, simp->szBuffer);  
- return simp->nNo;  
- }  
Python的定义： 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- from ctypes import *  
- class SimpStruct(Structure):  
-     _fields_ = [ ("nNo", c_int),  
-               ("fVirus", c_float),  
-               ("szBuffer", c_char * 512)]  
- 
- dll = CDLL("AddDll.dll")  
- simple = SimpStruct();  
- simple.nNo = 16
- simple.fVirus = 3.1415926
- simple.szBuffer = "magicTong/0"
- print dll.PrintStruct(byref(simple))  
上面例子结构体很简单，但是如果结构体里面有指针，甚至是指向结构体的指针，处理起来会复杂很多，不过Python里面也有相应的处理方法，下面这个例子来自网上，本来想自己写个，懒得写了，能说明问题就行：
C代码如下： 
**[cpp]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- typedefstruct
- 
- {  
- 
- char words[10];  
- 
- }keywords;  
- 
- 
- 
- typedefstruct
- 
- {  
- 
- keywords *kws;  
- 
- unsigned int len;  
- 
- }outStruct;  
- 
- extern"C"int__declspec(dllexport) test(outStruct *o);  
- 
- int test(outStruct *o)  
- 
- {  
- 
- unsigned int i = 4;  
- 
- o->kws = (keywords *)malloc(sizeof(unsigned char) * 10 * i);  
- 
- strcpy(o->kws[0].words, "The First Data");  
- 
- strcpy(o->kws[1].words, "The Second Data");  
- 
- 
- 
- o->len = i;  
- 
- return 1;  
- 
- }  
Python代码如下： 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- class keywords(Structure):  
- 
-         _fields_ = [('words', c_char *10),]  
- 
- 
- 
- class outStruct(Structure):  
- 
-         _fields_ = [('kws', POINTER(keywords)),  
- 
-                     ('len', c_int),]  
- 
- o = outStruct()  
- 
- dll.test(byref(o))  
- 
- 
- 
- print o.kws[0].words;  
- 
- print o.kws[1].words;  
- 
- print o.len  
**6、例子**
说得天花乱坠，嘿嘿，还是看两个实际的例子。 
**例子1：**
这是一个GUID生成器，其实很多第三方的python库已经有封装好的库可以调用，不过这得装了那个库才行，如果想直接调用一些API，对于python来说，也要借助一个第三方库才行，这个例子比较简单，就是用C++调用win32 API来产生GUID，然后python通过调用C++写的dll来获得这个GUID。
C++代码如下： 
**[cpp]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- extern"C"__declspec(dllexport) char* newGUID();   
- 
- char* newGUID()   
- 
- {  
- 
- staticchar buf[64] = {0};  
- 
-      statc GUID guid;  
- 
- if (S_OK == ::CoCreateGuid(&guid))   
- 
-      {  
- 
-        _snprintf(buf, sizeof(buf),  
- 
- "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",   
- 
- guid.Data1,  
- 
- guid.Data2,  
- 
- guid.Data3,  
- 
- guid.Data4[0], guid.Data4[1],  
- 
- guid.Data4[2], guid.Data4[3],  
- 
- guid.Data4[4], guid.Data4[5],  
- 
- guid.Data4[6], guid.Data4[7]  
- 
- );   
- 
-        ::MessageBox(NULL, buf, "GUID", MB_OK);   
- 
-       }  
- 
- return (char*)buf;  
- 
- }  
Python代码如下： 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- def CreateGUID():  
- 
- """
- 
-     创建一个全局唯一标识符
- 
-     类似：E06093E2-699A-4BF2-A325-4F1EADB50E18
- 
-     NewVersion
- 
-     """
- 
- try:  
- 
- # dll path
- 
-         strDllPath = sys.path[0] + str(os.sep) + "createguid.dll"
- 
-         dll = CDLL(strDllPath)  
- 
-         b = dll.newGUID()  
- 
-         a = c_char_p(b)  
- 
- except Exception, error:  
- 
- print error  
- 
- return ""  
- 
- return a.value  
**例子2：**
这个例子是调用kernel32.dll中的createprocessA函数来启动一个记事本进程 
**[python]**[view
 plain](http://blog.csdn.net/magictong/article/details/3075478#)[copy](http://blog.csdn.net/magictong/article/details/3075478#)
- #  -*- coding:utf-8 -*- 
- 
- from ctypes import *   
- 
- 
- 
- # 定义_PROCESS_INFORMATION结构体
- 
- class _PROCESS_INFORMATION(Structure):  
- 
-     _fields_ = [('hProcess', c_void_p),  
- 
-                 ('hThread', c_void_p),  
- 
-                 ('dwProcessId', c_ulong),  
- 
-                 ('dwThreadId', c_ulong)]  
- 
- 
- 
- # 定义_STARTUPINFO结构体
- 
- class _STARTUPINFO(Structure):  
- 
-     _fields_ = [('cb',c_ulong),  
- 
-                 ('lpReserved', c_char_p),  
- 
-                 ('lpDesktop', c_char_p),  
- 
-                 ('lpTitle', c_char_p),  
- 
-                 ('dwX', c_ulong),  
- 
-                 ('dwY', c_ulong),  
- 
-                 ('dwXSize', c_ulong),  
- 
-                 ('dwYSize', c_ulong),  
- 
-                 ('dwXCountChars', c_ulong),  
- 
-                 ('dwYCountChars', c_ulong),  
- 
-                 ('dwFillAttribute', c_ulong),  
- 
-                 ('dwFlags', c_ulong),  
- 
-                 ('wShowWindow', c_ushort),  
- 
-                 ('cbReserved2', c_ushort),  
- 
-                 ('lpReserved2', c_char_p),  
- 
-                 ('hStdInput', c_ulong),  
- 
-                 ('hStdOutput', c_ulong),  
- 
-                 ('hStdError', c_ulong)]  
- 
- 
- 
- NORMAL_PRIORITY_CLASS = 0x00000020#定义NORMAL_PRIORITY_CLASS
- 
- kernel32 = windll.LoadLibrary("kernel32.dll")  #加载kernel32.dll
- 
- CreateProcess = kernel32.CreateProcessA   #获得CreateProcess函数地址
- 
- ReadProcessMemory = kernel32.ReadProcessMemory #获得ReadProcessMemory函数地址
- 
- WriteProcessMemory = kernel32.WriteProcessMemory #获得WriteProcessMemory函数地址
- 
- TerminateProcess = kernel32.TerminateProcess  
- 
- 
- 
- # 声明结构体
- 
- ProcessInfo = _PROCESS_INFORMATION()  
- 
- StartupInfo = _STARTUPINFO()  
- 
- fileName = 'c:/windows/notepad.exe'# 要进行修改的文件
- 
- address = 0x0040103c# 要修改的内存地址
- 
- strbuf = c_char_p("_")        # 缓冲区地址
- 
- bytesRead = c_ulong(0)       # 读入的字节数
- 
- bufferSize =  len(strbuf.value)     # 缓冲区大小
- 
- 
- 
- # 创建进程 
- 
- CreateProcess(fileName, 0, 0, 0, 0, NORMAL_PRIORITY_CLASS,0, 0, byref(StartupInfo), byref(ProcessInfo))  
## [Python:使用ctypes库调用外部DLL](http://www.cnblogs.com/wuchang/archive/2010/04/04/1704456.html)
2010-04-04 23:36 by 无常, 10109 阅读, 4 评论, [收藏](http://www.cnblogs.com/wuchang/archive/2010/04/04/1704456.html#), [编辑](http://www.cnblogs.com/wuchang/admin/EditPosts.aspx?postid=1704456)
# 前言
朋友的公司是做GPS的，上周联系到我要帮做个程序把他们平台的车辆定位跟踪数据和省里的平台对接。看一下官方提供的三个文档，洋洋洒洒共一百多页，一大堆协议的定义甚是齐全，好在官方的文件中也带有个封装好通信功能的DLL和一个调用此接口的c++ DEMO程序，既然有现成的可用，那就不必去看他的协议了。
说实话，参加工作之后就基本没用过c++，生疏了。特别是要用c++操作数据库，对我来说比割几刀还要痛苦。官方的API中已经很详尽，要做的就是从现有平台的数据库中获取车辆定位信息，通过官方的API发送到省中心平台。
本想用C#给官方API做个包装，省得再去动用C++，可是看到此API中定义有几个Struct，而且下行数据都是通过回调函数方式提供，google了一下，似乎C#对调用有回调函数的C DLL不是很顺畅，于是放弃了，想到了[Python](http://python.org/)。
# 一、Python之ctypes
[ctypes](http://docs.python.org/library/ctypes.html)是[Python](http://python.org/)的一个外部库，提供和C语言兼容的数据类型，可以很方便地调用C
 DLL中的函数。在Python2.5官方安装包都带有[ctypes](http://docs.python.org/library/ctypes.html) 1.1版。ctypes的官方文档在[这里](http://docs.python.org/library/ctypes.html)。
ctypes的使用非常简明，如调用cdecl方式的DLL只需这样：
```
```python
from
```
```python
ctypes
```
```python
import
```
```python
*
```
```python
;
```
```python
h
```
```python
=
```
```python
CDLL(
```
```python
'msvcrt.dll'
```
```python
)
```
```python
h.printf(
```
```python
'a=%d,b=%d,a+b=%d'
```
```python
,
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
+
```
```python
2
```
```python
);
```
```
以上代码运行后输出 a=1,b=2,a+b=3。
# 二、加载库和普通函数的调用
官方API提供的库中有几个主要的函数：
```
```cpp
//初始化
```
```cpp
int
```
```cpp
DCSPCLIENTDLL
 InitInterface(
```
```cpp
const
```
```cpp
char
```
```cpp
*pCenterIP,
```
```cpp
const
```
```cpp
unsigned
```
```cpp
short
```
```cpp
nUpLinkSvrPort,
```
```cpp
const
```
```cpp
unsigned
```
```cpp
short
```
```cpp
nDownLinkSvrPort
 );
```
```cpp
//释放资源
```
```cpp
int
```
```cpp
DCSPCLIENTDLL
 FiniInterface(
```
```cpp
void
```
```cpp
);
```
```cpp
//登录
```
```cpp
int
```
```cpp
DCSPCLIENTDLL
 Login(
```
```cpp
const
```
```cpp
unsigned
```
```cpp
int
```
```cpp
uiBranchPlatformID,
```
```cpp
const
```
```cpp
unsigned
```
```cpp
int
```
```cpp
nUserID,
```
```cpp
const
```
```cpp
char
```
```cpp
*pPassword
 );
```
```cpp
//注销
```
```cpp
int
```
```cpp
DCSPCLIENTDLL
 Logout(
```
```cpp
const
```
```cpp
unsigned
```
```cpp
int
```
```cpp
uiBranchPlatformID,
```
```cpp
const
```
```cpp
unsigned
```
```cpp
int
```
```cpp
nUserID,
```
```cpp
const
```
```cpp
char
```
```cpp
*pPassword
 );
```
```cpp
//发车辆实时定位数据
```
```cpp
int
```
```cpp
DCSPCLIENTDLL
 SendUPRealLocation(
```
```cpp
const
```
```cpp
char
```
```cpp
*
```
```cpp
const
```
```cpp
pDeviceId,
```
```cpp
const
```
```cpp
char
```
```cpp
cDeviceColor,
```
```cpp
```
```cpp
const
```
```cpp
unsigned
```
```cpp
short
```
```cpp
nMsgCode,
```
```cpp
const
```
```cpp
_stBPDynamicData
 *
```
```cpp
const
```
```cpp
pStGpsData
 );
```
```
在Python中加载使用：
```
```python
from
```
```python
ctypes
```
```python
import
```
```python
*
```
```python
#加载API库
```
```python
api
```
```python
=
```
```python
CDLL(
```
```python
'DCSPClientDLL.dll'
```
```python
);
```
```python
#初始化函数的参数类型
```
```python
api.InitInterface.argtypes
```
```python
=
```
```python
[c_char_p,c_ushort,c_ushort]
```
```python
api.Login.argtypes
```
```python
=
```
```python
[c_uint,c_uint,c_char_p]
```
```python
api.Logout.argtypes
```
```python
=
```
```python
[c_uint,c_uint,c_char_p]
```
```python
#初始化并登录
```
```python
api.InitInterface(u
```
```python
"中心服务器地址"
```
```python
,
 u
```
```python
'上行服务端端口'
```
```python
,
 u
```
```python
'下行客户端端口'
```
```python
)
```
```python
api.Login(platformID,userID,password);
```
```python
#.....其它操作
```
```python
api.Logout(platformID,userID,password);
```
```python
#注销
```
```
参数类型可以像上面的代码一样预先设定好，或者在调用函数时再把参数转成相应的c_***类型。ctypes的类型对应如下:
![image](http://images.cnblogs.com/cnblogs_com/wuchang/WindowsLiveWriter/PythonctypesDLL_12B8B/image_thumb.png)
如此，完成了简单的第一步。
# 三、C语言中的Struct数据结构
在发送实时定位数据的函数SendUPRealLocation中有一个参数是结构体类型 _stBPDynamicData。python中没有struct这种数据结构，ctypes很周全，对C的struct和union这二种数据类型都提供很好的支持。stBPDynamicData结构的定义如下：
```
```cpp
//
 车辆动态数据结构体
```
```cpp
struct
```
```cpp
_stBPDynamicData
```
```cpp
{
```
```cpp
```
```cpp
//
 加密状态
```
```cpp
```
```cpp
unsigned
```
```cpp
char
```
```cpp
encrypt;
```
```cpp
```
```cpp
//
 GPS 时间
```
```cpp
```
```cpp
_StructTime
 gpsTime;
```
```cpp
```
```cpp
//
 经度
```
```cpp
```
```cpp
unsigned
```
```cpp
int
```
```cpp
longitude;
```
```cpp
```
```cpp
//
 纬度
```
```cpp
```
```cpp
unsigned
```
```cpp
int
```
```cpp
latitude;
```
```cpp
```
```cpp
//
 GPS速度
```
```cpp
```
```cpp
unsigned
```
```cpp
short
```
```cpp
unGpsSpeed;
```
```cpp
```
```cpp
//
 行驶记录仪速度
```
```cpp
```
```cpp
unsigned
```
```cpp
short
```
```cpp
unTachographSpeed;
```
```cpp
```
```cpp
//
 车辆当前总里程数
```
```cpp
```
```cpp
unsigned
```
```cpp
int
```
```cpp
uiMileageTotal;
```
```cpp
```
```cpp
//
 角度
```
```cpp
```
```cpp
unsigned
```
```cpp
short
```
```cpp
angle;
```
```cpp
```
```cpp
//
 车辆状态
```
```cpp
```
```cpp
unsigned
```
```cpp
short
```
```cpp
state;
```
```cpp
```
```cpp
//
 报警状态
```
```cpp
```
```cpp
unsigned
```
```cpp
short
```
```cpp
alarm;
```
```cpp
};
```
```
在python中，需要定义一个与这兼容的类，继承于ctypes.Structure，其中还用到一个_StructTime结构，这里一并贴出代码：
```
`class``_StructTime(Structure):`
`    ``_fields_``=``[(``'day'``,c_ubyte),`
`               ``(``'month'``,c_ubyte),`
`               ``(``'year'``,c_ushort),`
`               ``(``'hour'``,c_ubyte),`
`               ``(``'minute'``,c_ubyte),`
`               ``(``'second'``,c_ubyte)];   `
`    ``def``__str__(``self``):`
`        ``return`
```
'{0}-{1}-{2}
 {3}:{4}:{5}'
```
`.``format``(``self``.year,``self``.month,``self``.day,``self``.hour,``self``.minute,``self``.second);`
`        `
`class``_stBPDynamicData(Structure):`
`    ``_fields_``=``[(``'encrypt'``,c_ubyte),`
`               ``(``'gpsTime'``,_StructTime),            `
`               ``(``'longitude'``,c_uint),`
`               ``(``'latitude'``,c_uint),`
`               ``(``'unGpsSpeed'``,c_ushort),`
`               ``(``'unTachographSpeed'``,c_ushort),`
`               ``(``'uiMileageTotal'``,c_uint),`
`               ``(``'angle'``,c_ushort),`
`               ``(``'state'``,c_ushort),`
`               ``(``'alarm'``,c_ushort)];`
`    ``def``__str__(``self``):`
`        ``return``u``'({0},{1}),{6},sp:{2},angle:{3},st:{4},al:{5}'``.``format``(``self``.longitude,`
`                ``self``.latitude,``self``.unGpsSpeed,`
`                ``self`
```
.angle
 ,
```
`self``.state,``self``.alarm,``self`
```
.gpsTime
 );
```
`    `
`class``gpsData(Structure):`
`    ``_fields_``=``[(``'strDeviceID'``,c_char_p),`
`               ``(``'cDeviceColor'``,c_char),            `
`               ``(``'nMsgCode'``,c_ushort),`
`               ``(``'stBPD'``,_stBPDynamicData)];`
`    ``def``__str__(``self``):`
`        ``return``u``'{0},{1}'``.``format``(``self``.strDeviceID,``self`
```
.stBPD
 );
```
```
gpsData是我自己加的一个类，用于记录每辆车的信息。
现在就可以使用SendUPRealLocation函数发送车辆实时数据了：
```
```python
tm
```
```python
=
```
```python
_StructTime();
```
```python
tm.year
```
```python
=
```
```python
2010
```
```python
;tm.month
```
```python
=
```
```python
4
```
```python
;tm.day
```
```python
=
```
```python
3
```
```python
;tm.hour
```
```python
=
```
```python
11
```
```python
;tm.minute
```
```python
=
```
```python
2
```
```python
;tm.second
```
```python
=
```
```python
11
```
```python
;
```
```python
bpd
```
```python
=
```
```python
_stBPDynamicData();
```
```python
bpd.gpsTime
```
```python
=
```
```python
tm;bpd.longitude
```
```python
=
```
```python
1234567
```
```python
;bpd.latitude
```
```python
=
```
```python
246898
```
```python
;
```
```python
#...其它参数
```
```python
data
```
```python
=
```
```python
gpsData();
```
```python
data.strDeviceID
```
```python
=
```
```python
u
```
```python
'桂Coo007'
```
```python
;data.stBPD
```
```python
=
```
```python
bpd;
```
```python
#调用
 API发送数据
```
```python
api.SendUPRealLocation(
 data.strDeviceID, data.cDeviceColor ,
```
```python
```
```python
data.nMsgCode,
 addressof( data.stBPD ) );
```
```
注意SendUPRealLocation第三个参数是_stBPDynamicData * 指针类型，所以要用ctypes.addressof()取参数的地址。
# 四、回调函数
写到这里就忍不住唠叨几句，这个系统的协议设计的太有 “个性”了。这个系统的功能说起来也不复杂，就是要GPS运营商把指定的车辆位置信息发送到中心平台，同时中心平台可以向各GPS终端发送一些数据和指令，比如传送文字信息到终端，或者要求终端拍张照片反馈到中心。
这个协议流程是这样，运营商端主动连接到中心服务器，然后此连接只用于传输向中心平台主动发送的数据。登录成功了之后呢，中心平台再向运营商的IP建立一个连接，用于中心下发的数据和指令。官方称为“双链路”。
于是，就要求运营商必须要有固定的公网IP（这个不是问题，据了解GPS运营商服务器都有固定IP），而且这个程序必须运行在有公网IP的电脑上或采用端口映射之类的方法。可是俺开发设计时是在大教育局域网中的，搞个端口映射都不可能更别谈公网IP了。于是，在调试下行数据部分功能时就只能远程到运营商服务器上去调试了。
回归正题。
要使用回调函数，需要先用 CFUNCTYPE 定义回调函数的类型，官方API中有十多个回调函数注册，定义摘抄：
```
```cpp
#define
 DCSPCLIENTDLL __declspec(dllexport)
```
```cpp
typedef
```
```cpp
void
```
```cpp
(*pDownTextInfoFv)
 (
```
```cpp
const
```
```cpp
char
```
```cpp
*
```
```cpp
const
```
```cpp
pDeviceID,
```
```cpp
```
```cpp
const
```
```cpp
char
```
```cpp
cDeviceColor,
```
```cpp
const
```
```cpp
char
```
```cpp
*
```
```cpp
const
```
```cpp
pInfo
 );
```
```cpp
typedef
```
```cpp
void
```
```cpp
(*pDownCommunicateReqFv)
 (
```
```cpp
const
```
```cpp
char
```
```cpp
*
```
```cpp
const
```
```cpp
pDeviceID,
```
```cpp
```
```cpp
const
```
```cpp
char
```
```cpp
cDeviceColor,
```
```cpp
const
```
```cpp
char
```
```cpp
*
```
```cpp
const
```
```cpp
pCalledTel
 );
```
```cpp
extern
```
```cpp
"C"
```
```cpp
{
```
```cpp
```
```cpp
void
```
```cpp
DCSPCLIENTDLL
 RegDownTextInfoFunc( pDownTextInfoFv pFv );
```
```cpp
```
```cpp
void
```
```cpp
DCSPCLIENTDLL
 RegDownCommunicateReqFunc( pDownCommunicateReqFv pFv );
```
```cpp
};
```
```
在python中，定义相应的类型和回调处理函数：
```
`""``"下发文字信息"``""`
```
def
 downTextInfo(pDeviceID,cDeviceColor,pInfo):
```
`    ``print(u``'<-[下发文字]:{0},{1}'`
```
.format(str(pDeviceID),str(pInfo))
 );
```
`    `
```
r=api.SendUpTextInfoAck(pDeviceID,
 cDeviceColor, True );
```
`    ``if``r==0:`
`        ``print(u``'->回复下发文字成功。'``);`
`    ``else``:`
`        ``print(u``'->回复下发文字失败。'``);`
```
pDownTextInfoFv
 = CFUNCTYPE(c_void_p,c_char_p, c_char, c_char_p)  #回调函数类型定义
```
```
pDownTextInfoHandle
 = pDownTextInfoFv(downTextInfo);
```
```
api.RegDownTextInfoFunc(pDownTextInfoHandle);  
 #注册回调函数
```
```
其中SendUpCommunicateAck是回应中心，告知已经收到信息。二个参数类型和downTextInfo中的参数类型一到，所以可以不用初始化声明此函数的参数定义。
其余的回调函数用相同的方法处理。
# 结尾
调试完API对接部分功能后，在想用哪个py库操作数据库比较方便呢，找了一下之后才想到为何不用ironPython而可以直接使用ado.net访问数据库，岂不是更爽。
于是把代码搬到ironPython2.6中试试，让我十分惊喜的是不用做任何个性代码直接运行成功！[ironPython](http://ironpython.net/) 2.6中的ctypes和Python2.6的一样都是1.1.0版。
PS:借用 [1号园友](http://www.cnblogs.com/JeffreyZhao)的主题和CSS。
### [Python SIP使用总结（Win&Linux通用）](http://blog.csdn.net/joeblackzqq/article/details/7405971)
分类： [Windows](http://blog.csdn.net/JoeBlackzqq/article/category/879065)[Python](http://blog.csdn.net/JoeBlackzqq/article/category/911152)[Linux](http://blog.csdn.net/JoeBlackzqq/article/category/837473)2012-03-29
 10:24109人阅读[评论](http://blog.csdn.net/joeblackzqq/article/details/7405971#comments)(0)收藏[举报](http://blog.csdn.net/joeblackzqq/article/details/7405971#report)
[python](http://www.csdn.net/tag/python)[linux](http://www.csdn.net/tag/linux)[makefile](http://www.csdn.net/tag/makefile)[reference](http://www.csdn.net/tag/reference)[windows](http://www.csdn.net/tag/windows)[开发工具](http://www.csdn.net/tag/%e5%bc%80%e5%8f%91%e5%b7%a5%e5%85%b7)
From: [http://topic.csdn.net/u/20120324/22/cd8796f8-c601-492c-992d-3d70d58ba51b.html?07033688857013248#replyachor](http://topic.csdn.net/u/20120324/22/cd8796f8-c601-492c-992d-3d70d58ba51b.html?07033688857013248#replyachor)
本文原先发表与我的博客。这是我做图像处理的时候摸索到的，现分享给大家。在用Python+OpenCV做影像分割、识别以及变换检测时，有时候会遇到处理速度瓶颈，于是想到用C++重写一些模块。在观察各个Python调用C++的方法中，在网上看了许多评价和介绍，最后选择了SIP作为工具。
  本着分享的精神，现将使用的方法和在使用中遇到的问题总结出来，供大家参考。欢迎拍砖。本文也参加CSDN技术大分享活动，欢迎大家捧场。
  如前所述，Python中使用C/C++模块有许多工具，大名鼎鼎的有SWIG（英文意思为：豪饮）、SIP（英文意思为：啜饮，小口的喝），还有boost.python等。其中SIP是从SWIG发展而来，专为Python调用C/C++模块使用的（看SIP的命名就能看出来，很有意思的）。
  或许有人问为什么不用ctypes模块，原因是使用SIP无需编写特定格式的C/C++文件。需要调用的C/C++源代码文件，只是最简单的函数即可。若说SIP有什么需要学习的，那就是sip文件（下文中会介绍到）的编写。不过sip文件是基于C/C++的头文件的，学起来也比较简单。
  SIP的使用方法，在官网的SIP Reference Guide中有介绍，不过那是针对至少有点经验的人员，对初学者来说，想十分钟快速上手还是有点难度的。这里就介绍下本人在使用SIP总结的需要注意的地方。
**前提：**本机上要装了Python、GCC（Windows下可以用MinGW）。并把python.exe和MinGW下的bin文件路径添加到环境变量中。
**第一部分：安装SIP  Windows：**
  将Riverbank上的sip的zip格式的代码包下载，将其解压到C:\Python27中，那么现在sip的文件夹路径为C:\Python27\sip-4.13.2\sip-4.13.2（你也可以将其解压到任何一个文件夹中，但这里为了叙述方便，就解压到我电脑上的Python的文件夹中）。
  打开Windows的cmd命令行，输如cd C:\Python27\sip-4.13.2\sip-4.13.2跳转到sip文件夹下。
  输入python configure.py --platform win32-g++。进行配置
  再完成上一步后，输入mingw32-make，接着输入mingw32-make install。若无意外，SIP就安装完成了。
**常见问题：**
  若是你的电脑上之前安装了Eclipse等其他软件，而将mingw32-make改成了make，那么就要将上面的mingw32-make命令改为make来使用。
  有个朋友说他按上面的方法使用make总是报错，仔细一问，才发现他的电脑上安装了Embacardero的RAD开发工具，也就是熟知的Delphi和C++ Builder的开发工具。在安装了这个工具后，在命令行中输入make，使用的总是Embacardero的C++ Builder的make。根据亲测，即使在配置的时候，使用python configure.py --platform win32-borland命令进行配置，对Emcarbadero的make也是无效的。这个配置命令产生的make文件，只是用于老版本的Borland
 C++ Builder的make命令。
  若要使用微软的VC的nmake，就要在配置时使用命令python configure.py --platform win32-msvc，其中win32-msvc使用的是VC6的nmake；win32-msvc.net使用的是VC2003的nmake；win32-msvc2005使用的是VC2005的nmake；win32-msvc2008使用的是VC2008的nmake。根据--show-platforms命令显示的结果，SIP的配置暂不支持VC2010和VS11下的nmake。若想知道SIP支持的所有平台和编译器，可以在命令行中使用python
 configure.py --show -platforms命令来查看。
**Linux：**
  Linux下的SIP安装与之类似，甚至更简单，Linux的发行版如Ubuntu，都自动安装了python和gcc，无需配置环境变量。
  所以只要在官网上下载SIP的Linux下的tar.gz格式的代码包，解压到某个目录中。然后在终端中进入该目录，依次输入python configure.py --platform linux-g++；make;make install即可。
**第二部分：在Python中使用C/C++写的函数  1、首先**，编写个C文件，功能是将两个数字相加并输出，命名为add.c，这个将成为在Python中的模块名，如下：
- 
C/C++ code
/* File : add.c */int add(int x, int y) 
{  
    int g;  
    g = x + y;
    return g;
}
**2、接着**，手工编写SIP文件，在使用SIP的过程中，一个C/C++的源码文件，必须对应一个同名的sip文件，命名为add.sip，如下：
- 
C/C++ code
/* Define the SIP wrapper to the add library. */%Module(name=add, language="C")
int add(int x, int y);
  如果是源程序是用C++写的，那么这里的（name=add, language="C"）就可以省去。
  这里的C源码文件没有头文件，所以对应的sip文件很简单。如果C/C++的源码是实现部分，在实现部分还包括接口部分，即头文件。那么在相应的sip文件中需要用
- 
C/C++ code
%TypeHeaderCode
#include <word.h>%End
  来包含相应的头文件。sip文件与正式的C/C++头文件相似，但与之不同的是：sip文件不包含相应的头文件的私有成员变量（private或protected）。更详细的sip文件编写规范，请参考riverbank官方网站上的说明文档SIP Reference Guide。
**3、编译C文件**。按照官网的说法，是编写configure.py，但别急，先做一些必不可少的工作。在命令行将add.c编译成add.o文件：输入
gcc -c add.c
接着，将这一步生成的add.o文件生产库文件：
ar -r libadd.a add.o
  这两步在这里是为一个单独的C模块测试的，如果是大量的C模块，可以用一个makefile一并批量完成，这也是初学者容易模糊的地方。记住，需要将libadd.a文件复制到Python文件夹下的libs文件夹中。也可以将源代码直接编译成dll，命令为gcc --shared add.c -o add.dll，记住要将生成的dll放到python\LIB\site-packages文件夹下。
**4、手工编写configure.py文件**，同样，这个configure文件的编写也不难，看下规范就会了（要会鸟语。。。）。这里，我们模仿官网的模版写一个自己的configure.py。
- 
Python code
import os
import sipconfig
# The name of the SIP build file generated by SIP and used by the build
# system.build_file ="add.sbf"# Get the SIP configuration information.config = sipconfig.Configuration()
# Run SIP to generate the code.os.system("".join([config.sip_bin, "-c", ".", "-b", build_file, "add.sip"]))
# Create the Makefile.makefile = sipconfig.SIPModuleMakefile(config, build_file)
# Add the library we are wrapping.  The name doesn't include any platform
# specific prefixes or extensions (e.g. the "lib" prefix on UNIX, or the
# ".dll" extension on Windows).makefile.extra_libs = ["add"]
# Generate the Makefile itself.makefile.generate()
**5、运行configure.py**，会生成一个makefile文件（直接用IDLE打开configure.py，按F5运行；或者命令行用python configure.py运行都可以）。
这里有个诡异的地方，有几个朋友在这一步会报错，说找不到add.sbf文件，而add.sbf文件应该是configure.py运行时调用相关函数自动产生的。若出现这个问题，请重新编译SIP。如果是Windows下，最好是在另一台机器上拷贝一个完整的包含能正常的SIP的Python文件夹，到有问题的机器上，将问题Python文件夹覆盖掉。
**6、在命令行输入make**（这里会生成一个关于函数的警告，不用管它，我们是用来测试的。。。其他的应该没什么问题，若有问题请检查前面的步骤），生成add.pyd文件。然后再输入make install（将add.pyd文件装入到Python的Lib文件夹下的sit-packages文件夹中）。
**7、打开Python 的命令行，进行测试**：
- 
Python code
>>>import add
>>>add.add(4, 7)
11>>>
（原谅我这么烂的模块名。。。）
**提示：**
（1）、这些文件可以放到Python的文件夹下新建的文件夹中（所有的操作在这个目录下的命令行窗口中使用）。注意，Python的父文件夹名不能有空格，否则会无法读取库文件。
