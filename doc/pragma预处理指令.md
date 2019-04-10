# pragma预处理指令


\#pragma预处理指令详解 (分享下！) 

**\#pragma预处理指令详解** 

在所有的预处理指令中，\#pragma  
指令可能是最复杂的了，它的作用是设定编译器的状态或者是指示编译器完成一些特定的动作。\#pragma指令对每个编译器给出了一个方法,在保持与C和 
C++语言完全兼容的情况下,给出主机或操作系统专有的特征。依据定义,编译指示是机器或操作系统专有的,且对于每个编译器都是不同的。 


其格式一般为:        \#pragma  Para  

       其中Para  为参数，下面来看一些常用的参数。  

 

       (1)message  参数。  Message  参数是我最喜欢的一个参数，它能够在编译信息输出窗  

口中输出相应的信息，这对于源代码信息的控制是非常重要的。其使用方法为：  

             \#pragma  message(“消息文本”)  

             当编译器遇到这条指令时就在编译输出窗口中将消息文本打印出来。

       当我们在程序中定义了许多宏来控制源代码版本的时候，我们自己有可能都会忘记有没有正确的设置这些宏，此时我们可以用这条指令在编译的时候就进行检查。假设我们希望判断自己有没有在源代码的什么地方定义了_X86这个宏可以用下面的方法 


             \#ifdef  _X86  

             \#pragma  message(“_X86  macro  activated!”)  

             \#endif  

             当我们定义了_X86这个宏以后，应用程序在编译时就会在编译输出窗口里显示“_  

X86  macro  activated!”。我们就不会因为不记得自己定义的一些特定的宏而抓耳挠腮了  

。  

       

     (2)另一个使用得比较多的pragma参数是code_seg。格式如：  

           \#pragma  code_seg(  ["section-name"[,"section-class"]  ]  )  

           它能够设置程序中函数代码存放的代码段，当我们开发驱动程序的时候就会使用到它。  

 

     (3)\#pragma  once  (比较常用）  

           只要在头文件的最开始加入这条指令就能够保证头文件被编译一次，这条指令实际上在VC6中就已经有了，但是考虑到兼容性并没有太多的使用它。  

       

     (4)\#pragma  hdrstop表示预编译头文件到此为止，后面的头文件不进行预编译。BCB可以预编译头文件以加快链接的速度，但如果所有头文件都进行预编译又可能占太多磁盘空间，所以使用这个选项排除一些头文件。   


         有时单元之间有依赖关系，比如单元A依赖单元B，所以单元B要先于单元A编译。你可以用\#pragma  
startup指定编译优先级，如果使用了\#pragma  package(smart_init)  ，BCB就会根据优先级的大小先后编译。   


       

     (5)\#pragma  resource  "*.dfm"表示把*.dfm文件中的资源加入工程。*.dfm中包括窗体  

外观的定义。    

         

     (6)\#pragma  warning(  disable  :  4507  34;  once  :  4385;  error  :  164  ) 


           等价于：  

           \#pragma  warning(disable:4507  34)    //  不显示4507和34号警告信息  

           \#pragma  warning(once:4385)                //  4385号警告信息仅报告一次  

           \#pragma  warning(error:164)                //  把164号警告信息作为一个错误。  

           同时这个pragma  warning  也支持如下格式：  

           \#pragma  warning(  push  [  ,n  ]  )  

           \#pragma  warning(  pop  )  

           这里n代表一个警告等级(1---4)。  

           \#pragma  warning(  push  )保存所有警告信息的现有的警告状态。  

           \#pragma  warning(  push,  n)保存所有警告信息的现有的警告状态，并且把全局警告等级设定为n。    

           \#pragma  warning(  pop  )向栈中弹出最后一个警告信息，在入栈和出栈之间所作的一切改动取消。例如：  

           \#pragma  warning(  push  )  

           \#pragma  warning(  disable  :  4705  )  

           \#pragma  warning(  disable  :  4706  )  

           \#pragma  warning(  disable  :  4707  )  

           //.......  

           \#pragma  warning(  pop  )    

           在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)。  

       （7）pragma  comment(...)  

             该指令将一个注释记录放入一个对象文件或可执行文件中。  

           常用的lib关键字，可以帮我们连入一个库文件。  

   

   

每个编译程序可以用\#pragma指令激活或终止该编译程序支持的一些编译功能。例如，对循环优化功能：  

\#pragma  loop_opt(on)            //  激活  

\#pragma  loop_opt(off)    //  终止  

有时，程序中会有些函数会使编译器发出你熟知而想忽略的警告，如“Parameter  xxx  is  never  used  in  function  xxx”，可以这样： 


\#pragma  warn  —100            //  Turn  off  the  warning  message  for  warning  #100 


int  insert_record(REC  *r)  

{  /*  function  body  */  }  

\#pragma  warn  +100                        //  Turn  the  warning  message  for  warning  #100  back  on 


函数会产生一条有唯一特征码100的警告信息，如此可暂时终止该警告。  

每个编译器对\#pragma的实现不同，在一个编译器中有效在别的编译器中几乎无效。可从编译器的文档中查看。


 用\#pragma data_seg建立一个新的数据段并定义共享数据，其具体格式为：

 　　\#pragma data_seg （"shareddata")

 　　HWND sharedwnd=NULL;//共享数据

 　　\#pragma data_seg()


 1，\#pragma data_seg()一般用于DLL中。也就是说，在DLL中定义一个共享的，有名字的数据段。**最关键的是：这个数据段中的全局变量可以被多个进程共享。否则多个进程之间无法共享DLL中的全局变量。**

 2，**共享数据必须初始化，否则微软编译器会把没有初始化的数据放到.BSS段中，从而导致多个进程之间的共享行为失败。**

 3。如果在一个DLL中这么写：

 \\#pragma data_seg("MyData")

 int g_Value; // Note that the global is not initialized.

 \\#pragma data_seg()

 DLL提供两个接口函数：

 int GetValue()
 {
     return g_Value;
 }

 void SetValue(int n)
 {
     g_Value = n;
 }

 然后启动两个进程A和B，A和B都调用了这个DLL，假如A调用了SetValue(5); B接着调用int m = GetValue(); 那么m的值不一定是5，而是一个未定义的值。因为DLL中的全局数据对于每一个调用它的进程而言，是私有的，不能共享的。**假如你对g_Value进行了初始化，那么g_Value就一定会被放进MyData段中。换句话说，如果A调用了SetValue(5); B接着调用int m = GetValue(); 那么m的值就一定是5！这就实现了跨进程之间的数据通信！**

**VC6常用编译指令** 

1.编译时打印字符串
 \#if __DEGUB
  \\#pragma message( "Debug" )
 \\#endif 

2.忽略某个warning
  编译期
  \\#pragma warning(disable:4800)
  ....
  \\#pragma warning(default:4800)

 连接期
  \\#pragma comment(linker,"/ignore:4078")

3.定义段名称

 \#pragma data_seg("segment-name") 

 \#pragma code_seg("segment-name") 

 \#pragma const_seg("segment-name") 

 \#pragma bss_seg("segment-name")


 4.定义为进程共享段
  \\#pragma data_seg("name");
  ...
  \\#pragma data_seg();
  \\#pragma comment(linker,"/section:name,RWS") 

5.合并code和代码段
  \\#pragma comment(linker, "/MERGE:.text=.data") 


 6.导入lib
  \\#pragma comment (lib, "shlwapi.lib") 

7.内存对齐
  \\#pragma pack(2)
  \\#pragma pack()

**简要记录sizeof和内存对齐** 

    本来，一般是不自己计算sizeof的，知道内存对齐会对sizeof有影响，所以从来不手算，而是代码里写上sizeof。今天又看到http://blog.vckbase.com/smileonce/archive/2005/08/08/10658.html，翻来了http://blog.vckbase.com/billdavid/archive/2004/06/23/509.html
 ，自己想想还是也记录一下，万一以后自己真的也要计算sizeof，忘了，还能有个提示，也给不是很明白的朋友一个参考。

struct sample1

{

    char a;        /// sizeof(char) = 1

    double b;    /// sizeof(double) = 8

};

///default(  缺省#pragam pack(8) ——VC6和VC71，其它编译器，个人未知 )

    ///1+8 = 9 —> 16(  8 < 9 < 16  )


\#pragma pack( 4 )

    ///1+8 = 9 —> 12(  8 < 9 < 12  )


\#pragma pack( 2 )

    ///1+8 = 9 —> 10(  8 < 9 < 10  )


\#pragma pack( 1 )

    ///1+8 = 9 —> 9


\#pragma pack( 16 )

    ///1+8 = 9 —> 16(  16—>8 ---- 8 < 9 < 16  )


struct sample2

{

    char a;     ///1

    int b;        ///4

};

\#pragma pack( 8 )

    /// 1 + 4  = 5 —> 8(  8 —> 4  )


\#pragma pack( 16 )

    /// 1 + 4 = 5 —> 8( 16 —> 4  )


    说明：\#pragma pack告诉编译器进行内存边界对齐，一般都是采用编译器的设置对整个项目采用同一对齐方案，而且通常为缺省8字节对齐。

**/////////////////////////////////以下内容于 2005-12-10 添加/////////////////////////////////**

今天又看到以前测试的一段代码，突然不明白了起来，又稍写了几个测试。

```c
struct sample3
{
    char a;  ///1
    int b;    ///4
    char c;  ///1
};

///default                  ///12
#pragma pack( 4 )   ///12
#pragma pack( 2 )   ///08
#pragma pack( 1 )   ///06
#pragma pack( 16 ) ///12
```


    原来，其实编译器，根据对齐指示的对齐字节和最大成员的字节，对每个成员进行了对齐：编译器会取对齐指示和最大成员字节中较小的一个用于补齐其它成员。那么，上面的sample1/2/3也就都在情理之中了。为了证实这点，我们还再看一个例子：

```
struct sample4

{

    char a;      ///1

    int b;         ///4

    double c;  ///8

    char d;      ///1

};

///default:                ///8+8+8+8 = 32

\#pragma pack( 4 )   ///4+4+8+4 = 20

\#pragma pack( 2 )   ///2+4+8+2 = 16

\#pragma pack( 1 )   ///1+4+8+1 = 14

\#pragma pack( 16 ) ///8+8+8+8 = 32
```

而实际上，编译器给出的值是：24、20、16、14、24

那么说明我错了。注意一下，我发现char a,int b加起来也才5<8，难到编译器进行了联合对齐？

```
struct sample5

{

    char a;      ///1

    double c;  ///8

    int b;         ///4

    char d;      ///1

};
```

编译器给出结果：24、20、16、14、24


    这用联合对齐的解释正好符合，我又试验了不同的数据，发现这个结论并不太准确确。于是，我输出了每一个对象成员地址进行分析。由于试验数据量很大，这里就不列出了。


最后得到了以下结论：

    1. 成员的对齐是按声明顺序进行的；

    2. 对齐值由编译指示和最大成员两者较小的值决定；

    3. 未对齐到对齐值的成员一起形成块对齐（联合对齐）；

    4. 上一个（下一个）对齐采用自己较大则不变，自己较小则填充自己对齐到上一个（下一个）大小；

    5. 每成员对齐：如果前面已对齐到对齐值，下一个对齐自己。如果前面未对齐到对齐值，如果加上下一个成员不大于对齐值，下一个对齐自己，否则填充自己块对齐到对齐值。

    6. 最后还未对齐到对齐值的，填充空间块对齐到对齐值。


从这些结论，可以得到：

    1. 以上的对齐原则其实是尽量整齐排列、尽量节省内存。

    2. 声明成员应该尽量避免不同类型错杂开来，最好采用从小到大或者从大到小的顺序（错开后，会因为上对齐和下对齐而增加填充开销）。

    3. 编译器缺省采用8字节对齐主要是因为最大基本类型为8自己（以前自己不明白，在论坛提过问，后来，以为是SSE指令的原因）。

    4. 手算sizeof是没有必要的，负责的（可以先对齐出对齐块，用块数乘对齐值）。 

 **pragma的作用** 

不同的主机和操作系统都有其特点。为了利用好这些特点，c和c++的编译器必须能够针对不同的特性进行处理。比方说，有些程序需要准确地对内存进行管理（如应当将数据存放到哪里）或者需要指定函数接受参数的方式。\#pragma指示器便用于此处！通过\#pragma，我们的c和c++程序即可以保证兼容性也可以利用到低层系统的特色！
 语法规则： **\#pragma token-string** 其中token-string给出了特定的编译指令和参数。
 利用条件申明，pragma可以提供预处理功能和对编译器进行定制。c和c++可以识别以下pragma:

| alloc_text  | comment       | init_seg1        | optimize             |
| ----------- | ------------- | ---------------- | -------------------- |
| auto_inline | component     | inline_depth     | pack                 |
| bss_seg     | data_seg      | inline_recursion | pointers_to_members1 |
| check_stack | function      | intrinsic        | setlocale            |
| code_seg    | hdrstop       | message          | vtordisp1            |
| const_seg   | include_alias | once             | warning              |

以下我们便就一些重要的pragma进行探讨:
\#pragma comment(将注释放置到生成的目标文件或者可执行文件中去。commet-type一共有5种类型。可选的参数commenstring为这5种类型中的个别类型提供额外的参数。以下我们就5种类型进行分析与介绍：
compiler:将编译器名字和版本号放如目标文件。

exestr:将commentstring放到目标文件里面。

lib:将查找库的信息放如目标文件里面。此时，必须使用commentstring参数，用于记录lib的名字，最好还包括路径。

            \#pragma comment( lib, "emapi" )首先在当前目录底下查找emapi.lib,然后在lib环境变量的路径里面寻找

linker:将linker option放入目标文件里面。通过注释类型确定linker的选项，从而避免在Setting对话框里面进行设置。比如：

           \#pragma comment(linker, "/include:__mySymbol")

user:将一般的注释放入到目标文件里面。
\#pragma optimize( "：在函数一级对进行优化。该语句必须出现在函数外面，并且只会对该声明后的第一个函数有作用。on或者off确定是否起作用。[optimization-list]",
 {on | off} )
comment-type [, commentstring] )

| **Parameter(s)** | **Type of optimization**                         |
| ---------------- | ------------------------------------------------ |
| **a**            | Assume no aliasing.                              |
| **g**            | Enable global optimizations.                     |
| **p**            | Improve floating-point consistency.              |
| **s** or **t**   | Specify short or fast sequences of machine code. |
| **w**            | Assume no aliasing across function calls.        |
| **y**            | Generate frame pointers on the program stack.    |

由于比较简单，上面的英语我就不介绍了！！！调用可以是下面的形式：\#pragma optimize( "atp", on )
**\#pragma function(** *function1* [**,** *function2***,** ...]  **)            \#pragma intrinsic(** *function1* [**,**  *function2***,** ...] **)**以上两个声明确定了所选函数的调用类型。如果有一函数名出现在function的参数里面，则说明以一般函数的形式调用。如果出现在了intrinsic里面则编译器在编译的时候将函数代码转化了inline代码。
**\#pragma data_seg(** [**"***section-name***"**[**, "***section-class***"**] ] **)****：**为数据指定默认的存储区。如：
 \\#pragma data_seg( "MY_DATA" )。这句声明将使得在#pramga后申明的数据将在名为MY_DATA存储区里面进行分配。
 先介绍到此，告一段落。以后遇到了问题，再来添加！！！

**struct 关于sizeof的大小问题** 

```
typedef  struct  tagSocketData  {  

   BYTE          nSize;  

   BYTE          nType;  

   DWORD        nDataSize;  

   DWORD        nIndex;  

   SOCKET      socket;  

   DWORD        nDataLength;  

}  SOCKETDATA,  *LPSOCKETDATA;  

 

SOCKETDATA  sd;  

sd.nSize  =  sizeof(SOCKETDATA);    //本来应该是18,  可却是20  

 

sizeof(SOCKETDATA)  =  20,  本来是18字节的大小  
```

实际大小却是20  

可存为文件的时候却是按照18字节的大小存储  

 

应该怎么解决??  

 

 

---------------------------------------------------------------

 

看看这个  

 

```
struct  tagHDD  

{  

char  c1;  

DWORD  n;  

}HDD;  

long  n=sizeof(HDD);//n会返回8，为什么，真怪。  
```

 

回复人：  bluebohe(薄荷)  

这不怪，就怪在缺省的对齐方式上。  

在结构中，编译器为结构的每个成员按其自然对界（alignment）条件分配空间；各个成员按照它们被声明的顺序在内存中顺序存储，第一个成员的地址和整个结构的地址相同。在缺省情况下，C编译器为每一个变量或是数据单元按其自然对界条件分配空间。  

 

例如，下面的结构各成员空间分配情况。  

 

```
struct  test  {  

char  x1;  

short  x2;  

float  x3;  

char  x4;  

};  
```

　　结构的第一个成员x1，其偏移地址为0，占据了第1个字节。第二个成员x2为short类型，其起始地址必须2字节对界，因此，编译器在x2和x1之间填充了一个空字节。结构的第三个成员x3和第四个成员x4恰好落在其自然对界地址上，在它们前面不需要额外的填充字节。在test结构中，成员x3要求4

字节对界，是该结构所有成员中要求的最大对界单元，因而test结构的自然对界条件为4字节，编译器在成员x4后面填充了3个空字节。整个结构所占据空间为12字节。


现在你知道怎么回事了吧？  

更改C编译器的缺省分配策略  

　　一般地，可以通过下面的方法改变缺省的对界条件：  

　　?  使用伪指令\#pragma  pack  ([n])  

　　\#pragma  pack  ([n])伪指令允许你选择编译器为数据分配空间所采取的对界策略。  

例如，在使用了\#pragma  pack  (1)伪指令后，test结构各成员的空间分配情况就是按照一个字节对齐了，格式如下：  

```
#pragma  pack(push)  //保存对齐状态  

#pragma  pack(1)  

//定义你的结构  

//…………  

#pragma  pack(pop)  
```

 

---------------------------------------------------------------

 

在编译选项中可以设置：菜单中“工程”-->“设置”，    弹出一个“project 
 setting”对话框。然后选择“C/C++”标签--->在“Y分类”中选择“Code 
 Generation”--->在“Struct  member  alignment”中选择“1  Byte”。  

 

重新编译。ok了！！  

---------------------------------------------------------------

 

```
#pragma  pack(push,1)  

typedef  struct  tagSocketData  {  

   BYTE          nSize;  

   BYTE          nType;  

   DWORD        nDataSize;  

   DWORD        nIndex;  

   SOCKET      socket;  

   DWORD        nDataLength;  

}  SOCKETDATA,  *LPSOCKETDATA;  

# progma  pack(pop,1)  
```

 

就可以了~  

 

**内存对齐** 

从计算结构变量字节大小的问题开始

问题：
        

在32位编译系统中，
        typedef struct
        {
             int  A
             char B
         }T_s;
         请问T_s的长度为几个字节?

答案:

​       

题目不够严谨，跟编译器和编译选项有关的。
         pc上的32位编译器一般缺省是4位对齐，所以长度会是8，此时在B后填充3字节,但对齐方式可以改的；
         而有些嵌入系统的编译器缺省是不对齐的，所以长度为5。

​       

比如在vc中，
         如果
         \\#pragma pack(4)     //缺省
         则sizeof（T_s）等于8
         如果
         \\#pragma pack
         则sizeof（T_s）等于5

​        而且和cpu有关，在有的机器上int不一定就是32位

​        

要因结构对齐而定,对齐方式可以是1, 2, 4, 8, or 16
         1对齐:5;2对齐:6;4对齐:8;8对齐:8;16对齐:8

​      

char是   8位的，结构长度是8字节，在B之前没有对齐被位，但在B之后要补3个字节以便于在数组中把下一个元素的A对齐到4字节边界，当然这都是一般编译器在4字节对齐的情况下，如果某一编译器偏不这样实现，你也不能说它错了。因此如果我写一个编译器就把它的长度设为6字节，那么说它是6字节也正确。其实字节对齐的知识对编写代码并没有什么帮助，并且也不应该利用这些知识。我以前曾经说过正确的态度是“不假设没有进行字节对齐，不假设编译时的对齐方式（包括可以不进行对齐）”。


​       

计算结构变量的大小就必须讨论数据对齐问题。为了CPU存取的速度最快（这同CPU取数操作有关，详细的介绍可以参考一些计算机原理方面的书），C++在处理数据时经常把结构变量中的成员的大小按照4  或8的倍数计算，这就叫数据对齐（data   alignment）。这样做可能会浪费一些内存，但理论上速度快了。当然这样的设置会在读写一些别的应用程序生成的数据文件或交换数据时带来不便。MS  VC++中的对齐设定，有时候sizeof得到的与实际不等。一般在VC++中加上\#pragma  pack(n)的设定即可.或者如果要按字节存储，而不进行数据对齐，可以在Options对话框中修改Advanced  compiler页中的Data alignment为按字节对齐。 

 

看一个例子:

将A写入二进制文件
 \\#pragma pack (1)
 struct A
 {
  char a;
  int b;
 };
 文件
 61 06 00 00 00

\\#pragma pack (2)
 struct A
 {
  char a;
  int b;
 };
文件
 61 CC 06 00 00 00

\\#pragma pack (4)
 struct A
 {
  char a;
  int b;
 };
文件
 61 CC CC CC 06 00 00 00

\\#pragma pack (4)
 struct A
 {
  char a;
  int b;
 };
文件
 61 CC CC CC 06 00 00 00

\\#pragma pack (8)
 struct A
 {
  char a;
  int b;
 };
文件
 61 CC CC CC 06 00 00 00

该例子作者得出的结论:
 结论是 实际的对齐长度 = [pack指定的对齐长度]和[struct中最长成员的长度]较小的一个。

**[C] 结构对齐** 

http://community.csdn.net/Expert/FAQ/FAQ_Index.asp?id=182474




        在结构中，编译器为结构的每个成员按其自然对界（alignment）条件分配空间；各个成员按照它们被声明的顺序在内存中顺序存储，第一个成员的地址和整个结构的地址相同。在缺省情况下，C编译器为每一个变量或是数据单元按其自然对界条件分配空间  

 

例如，下面的结构各成员空间分配情况  

 

struct  test  

{  

char  x1;  

short  x2;  

float  x3;  

char  x4;  

};  

　　　　　　　　　　　　　　　　　  

　　结构的第一个成员x1，其偏移地址为0，占据了第1个字节。第二个成员x2为short类型，其起始地址必须2字节对界，因此，编译器在x2和x1之间填充了一个空字节。结构的第三个成员x3和第四个成员x4恰好落在其自然对界地址上，在它们前面不需要额外的填充字节。在test结构中，成员x3要求4

字节对界，是该结构所有成员中要求的最大对界单元，因而test结构的自然对界条件为4字节，编译器在成员x4后面填充了3个空字节。整个结构所占据空间为12字节。  

  

更改C编译器的缺省分配策略  

　　一般地，可以通过下面的两种方法改变缺省的对界条件：  

　　·  使用伪指令\#pragma  pack  ([n])  

　　·  在编译时使用命令行参数  

\#pragma  pack  ([n])伪指令允许你选择编译器为数据分配空间所采取的对界策略：  

 

　　    

例如，在使用了\#pragma  pack  (1)伪指令后，test结构各成员的空间分配情况就是按照一个字节对齐了  


\#pragma  pack(push)  //保存对齐状态  

\#pragma  pack(1)   

\#pragma  pack(pop)  


编译器默认都是8字节对齐;



=============================================================================

http://data.gameres.com/message.asp?TopicID=13636


什么是内存对齐

​    考虑下面的结构：

​         struct foo
​          {
​            char c1;
​            short s;
​            char c2;
​            int i;
​           };
​     
​     假设这个结构的成员在内存中是紧凑排列的，假设c1的地址是0，那么s的地址就应该是1，c2的地址就是3，i的地址就是4。也就是
​     c1 00000000, s 00000001, c2 00000003, i 00000004。

​    可是，我们在Visual c/c++ 6中写一个简单的程序：

​         struct foo a;
​     printf("c1 %p, s %p, c2 %p, i %p/n",
​         (unsigned int)(void*)&a.c1 - (unsigned int)(void*)&a,
​         (unsigned int)(void*)&a.s - (unsigned int)(void*)&a,
​         (unsigned int)(void*)&a.c2 - (unsigned int)(void*)&a,
​         (unsigned int)(void*)&a.i - (unsigned int)(void*)&a);
​     运行，输出：
​          c1 00000000, s 00000002, c2 00000004, i 00000008。

​    为什么会这样？这就是内存对齐而导致的问题。

为什么会有内存对齐

​    以下内容节选自《Intel Architecture 32 Manual》。
​     字，双字，和四字在自然边界上不需要在内存中对齐。（对字，双字，和四字来说，自然边界分别是偶数地址，可以被4整除的地址，和可以被8整除的地址。）
​     无论如何，为了提高程序的性能，数据结构（尤其是栈）应该尽可能地在自然边界上对齐。原因在于，为了访问未对齐的内存，处理器需要作两次内存访问；然而，对齐的内存访问仅需要一次访问。
​     一个字或双字操作数跨越了4字节边界，或者一个四字操作数跨越了8字节边界，被认为是未对齐的，从而需要两次总线周期来访问内存。一个字起始地址是奇数但却没有跨越字边界被认为是对齐的，能够在一个总线周期中被访问。
​     某些操作双四字的指令需要内存操作数在自然边界上对齐。如果操作数没有对齐，这些指令将会产生一个通用保护异常（#GP）。双四字的自然边界是能够被16整除的地址。其他的操作双四字的指令允许未对齐的访问（不会产生通用保护异常），然而，需要额外的内存总线周期来访问内存中未对齐的数据。

编译器对内存对齐的处理

​    缺省情况下，c/c++编译器默认将结构、栈中的成员数据进行内存对齐。因此，上面的程序输出就变成了：
 c1 00000000, s 00000002, c2 00000004, i 00000008。
 编译器将未对齐的成员向后移，将每一个都成员对齐到自然边界上，从而也导致了整个结构的尺寸变大。尽管会牺牲一点空间（成员之间有空洞），但提高了性能。
 也正是这个原因，我们不可以断言sizeof(foo) == 8。在这个例子中，sizeof(foo) == 12。

如何避免内存对齐的影响

​    那么，能不能既达到提高性能的目的，又能节约一点空间呢？有一点小技巧可以使用。比如我们可以将上面的结构改成：

struct bar
 {
     char c1; 
     char c2;
     short s;
     int i;
 };
     这样一来，每个成员都对齐在其自然边界上，从而避免了编译器自动对齐。在这个例子中，sizeof(bar) == 8。

​    这个技巧有一个重要的作用，尤其是这个结构作为API的一部分提供给第三方开发使用的时候。第三方开发者可能将编译器的默认对齐选项改变，从而造成这个结构在你的发行的DLL中使用某种对齐方式，而在第三方开发者哪里却使用另外一种对齐方式。这将会导致重大问题。
​     比如，foo结构，我们的DLL使用默认对齐选项，对齐为
 c1 00000000, s 00000002, c2 00000004, i 00000008，同时sizeof(foo) == 12。
 而第三方将对齐选项关闭，导致
​     c1 00000000, s 00000001, c2 00000003, i 00000004，同时sizeof(foo) == 8。

如何使用c/c++中的对齐选项

​    vc6中的编译选项有 /Zp[1|2|4|8|16] ，/Zp1表示以1字节边界对齐，相应的，/Zpn表示以n字节边界对齐。n字节边界对齐的意思是说，一个成员的地址必须安排在成员的尺寸的整数倍地址上或者是n的整数倍地址上，取它们中的最小值。也就是：
​     min ( sizeof ( member ),  n)
​     实际上，1字节边界对齐也就表示了结构成员之间没有空洞。
​     /Zpn选项是应用于整个工程的，影响所有的参与编译的结构。
​     要使用这个选项，可以在vc6中打开工程属性页，c/c++页，选择Code Generation分类，在Struct member alignment可以选择。

​    要专门针对某些结构定义使用对齐选项，可以使用\#pragma pack编译指令。指令语法如下：
 \\#pragma pack( [ show ] | [ push | pop ] [, identifier ] , n  )
​     意义和/Zpn选项相同。比如：

\\#pragma pack(1)
 struct foo_pack
 {
     char c1;
     short s;
     char c2;
     int i;
 };
 \\#pragma pack()

栈内存对齐

​    我们可以观察到，在vc6中栈的对齐方式不受结构成员对齐选项的影响。（本来就是两码事）。它总是保持对齐，而且对齐在4字节边界上。

验证代码

\#include <stdio.h>

struct foo
 {
     char c1;
     short s;
     char c2;
     int i;
 };

struct bar
 {
     char c1; 
     char c2;
     short s;
     int i;
 };

\\#pragma pack(1)
 struct foo_pack
 {
     char c1;
     short s;
     char c2;
     int i;
 };
 \\#pragma pack()


 int main(int argc, char* argv[])
 {
     char c1;
     short s;
     char c2;
     int i;

​    struct foo a;
​     struct bar b;
​     struct foo_pack p;

​    printf("stack c1 %p, s %p, c2 %p, i %p/n",
​         (unsigned int)(void*)&c1 - (unsigned int)(void*)&i,
​         (unsigned int)(void*)&s - (unsigned int)(void*)&i,
​         (unsigned int)(void*)&c2 - (unsigned int)(void*)&i,
​         (unsigned int)(void*)&i - (unsigned int)(void*)&i);

​    printf("struct foo c1 %p, s %p, c2 %p, i %p/n",
​         (unsigned int)(void*)&a.c1 - (unsigned int)(void*)&a,
​         (unsigned int)(void*)&a.s - (unsigned int)(void*)&a,
​         (unsigned int)(void*)&a.c2 - (unsigned int)(void*)&a,
​         (unsigned int)(void*)&a.i - (unsigned int)(void*)&a);

​    printf("struct bar c1 %p, c2 %p, s %p, i %p/n",
​         (unsigned int)(void*)&b.c1 - (unsigned int)(void*)&b,
​         (unsigned int)(void*)&b.c2 - (unsigned int)(void*)&b,
​         (unsigned int)(void*)&b.s - (unsigned int)(void*)&b,
​         (unsigned int)(void*)&b.i - (unsigned int)(void*)&b);

​    printf("struct foo_pack c1 %p, s %p, c2 %p, i %p/n",
​         (unsigned int)(void*)&p.c1 - (unsigned int)(void*)&p,
​         (unsigned int)(void*)&p.s - (unsigned int)(void*)&p,
​         (unsigned int)(void*)&p.c2 - (unsigned int)(void*)&p,
​         (unsigned int)(void*)&p.i - (unsigned int)(void*)&p);

​    printf("sizeof foo is %d/n", sizeof(foo));
​     printf("sizeof bar is %d/n", sizeof(bar));
​     printf("sizeof foo_pack is %d/n", sizeof(foo_pack));
​     
​     return 0;
 }

正文完

**VC中预处理指令的详解** 

    在所有的预处理指令中，\#pragma  指令可能是最复杂的了，它的作用是设定编译器的状态或者是指示编译器完成一些特定的动作。\#pragma指令对每个编译器给出了一个方法,在保持与C和
 C++语言完全兼容的情况下,给出主机或操作系统专有的特征。依据定义,编译指示是机器或操作系统专有的,且对于每个编译器都是不同的。  

其格式一般为:        \#pragma  Para  

       其中Para  为参数，下面来看一些常用的参数。  


       (1)message  参数。  

       Message  参数是我最喜欢的一个参数，它能够在编译信息输出窗口中输出相应的信息，这对于源代码信息的控制是非常重要的。其使用方法为：  

             \#pragma  message(“消息文本”)  

             当编译器遇到这条指令时就在编译输出窗口中将消息文本打印出来。  

       当我们在程序中定义了许多宏来控制源代码版本的时候，我们自己有可能都会忘记有没有正确的设置这些宏，此时我们可以用这条指令在编译的时候就进行检查。假设我们希望判断自己有没有在源代码的什么地方定义了_X86这个宏可以用下面的方法  

             \#ifdef  _X86  

             \#pragma  message(“_X86  macro  activated!”)  

             \#endif  

             当我们定义了_X86这个宏以后，应用程序在编译时就会在编译输出窗口里显示“_  

X86  macro  activated!”。我们就不会因为不记得自己定义的一些特定的宏而抓耳挠腮了。  

       

     (2)另一个使用得比较多的pragma参数是code_seg。

          格式如：  

           \#pragma  code_seg(  ["section-name"[,"section-class"]  ]  )  

           它能够设置程序中函数代码存放的代码段，当我们开发驱动程序的时候就会使用到它。  


     (3)\#pragma  once  (比较常用）  

           只要在头文件的最开始加入这条指令就能够保证头文件被编译一次，这条指令实际上在VC6中就已经有了，但是考虑到兼容性并没有太多的使用它。  

       

     (4)\#pragma  hdrstop表示预编译头文件到此为止，后面的头文件不进行预编译。

       BCB可以预编译头文件以加快链接的速度，但如果所有头文件都进行预编译又可能占太多磁盘空间，所以使用这个选项排除一些头文件。    


有时单元之间有依赖关系，比如单元A依赖单元B，所以单元B要先于单元A编译。你可以用\#pragma  startup指定编译优先级，如果使用了\#pragma  package(smart_init)  ，BCB就会根据优先级的大小先后编译。    

       

     (5)\#pragma  resource  "*.dfm"表示把*.dfm文件中的资源加入工程。*.dfm中包括窗体  

外观的定义。    

         

     (6)\#pragma  warning(  disable  :  4507  34;  once  :  4385;  error  :  164  )  

           等价于：  

           \#pragma  warning(disable:4507  34)    //  不显示4507和34号警告信息  

           \#pragma  warning(once:4385)                //  4385号警告信息仅报告一次  

           \#pragma  warning(error:164)                //  把164号警告信息作为一个错误。  

           同时这个pragma  warning  也支持如下格式：  

           \#pragma  warning(  push  [  ,n  ]  )  

           \#pragma  warning(  pop  )  

           这里n代表一个警告等级(1---4)。  

           \#pragma  warning(  push  )保存所有警告信息的现有的警告状态。  

           \#pragma  warning(  push,  n)保存所有警告信息的现有的警告状态，并且把全局警告  

等级设定为n。    

           \#pragma  warning(  pop  )向栈中弹出最后一个警告信息，在入栈和出栈之间所作的  

一切改动取消。例如：  

           \#pragma  warning(  push  )  

           \#pragma  warning(  disable  :  4705  )  

           \#pragma  warning(  disable  :  4706  )  

           \#pragma  warning(  disable  :  4707  )  

           //.......  

           \#pragma  warning(  pop  )    

           在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)。  

   （7）pragma  comment(...)               该指令将一个注释记录放入一个对象文件或可执行文件中。  

           常用的lib关键字，可以帮我们连入一个库文件。  


（8）用pragma导出dll中的函数    传统的到出 DLL 函数的方法是使用模块定义文件 (.def)，Visual C++ 
提供了更简洁方便的方法，那就是“__declspec()”关键字后面跟“dllexport”，告诉连接去要导出这个函数，例如：__declspec(dllexport)
 int __stdcall MyExportFunction(int iTest);    
把“__declspec(dllexport)”放在函数声明的最前面，连接生成的 DLL 就会导出函数“_MyExportFunction@4”。

    上面的导出函数的名称也许不是我的希望的，我们希望导出的是原版的“MyExportFunction”。还好，VC 提供了一个预处理指示符“\#pragma”来指定连接选项 (不仅仅是这一个功能，还有很多指示功能) ，如下：

    \#pragma comment(linker,"/EXPORT:MyExportFunction=_MyExportFunction@4")

    这下就天如人愿了：）。如果你想指定导出的顺序，或者只将函数导出为序号，没有 Entryname，这个预处理指示符 (确切地说是连接器) 都能够实现，看看 MSDN 的语法说明：

       /EXPORT:entryname[,@ordinal[,NONAME]][,DATA]

     @ordinal 指定顺序；NONAME 指定只将函数导出为序号；DATA 关键字指定导出项为数据项。


  （9）每个编译程序可以用\#pragma指令激活或终止该编译程序支持的一些编译功能。

     例如，对循环优化功能：  

     \#pragma  loop_opt(on)            //  激活  

     \#pragma  loop_opt(off)    //  终止  

有时，程序中会有些函数会使编译器发出你熟知而想忽略的警告，如“Parameter  xxx  is  never  used  in  function  xxx”，可以这样：  

\#pragma  warn  —100            //  Turn  off  the  warning  message  for  warning  #100  

int  insert_record(REC  *r)  

{  /*  function  body  */  }  

\#pragma  warn  +100                        //  Turn  the  warning  message  for  warning  #100  back  on  

函数会产生一条有唯一特征码100的警告信息，如此可暂时终止该警告。  

每个编译器对\#pragma的实现不同，在一个编译器中有效在别的编译器中几乎无效。可从编译器的文档中查看。  

一个很重要的参数

\#pragma pack(n)

数据边界对齐方式:

以如下结构为例: struct {

                   char a;

                   WORD b;

                   DWORD c;

                   char d;

                  }

在Windows默认结构大小: sizeof(struct) = 4+4+4+4=16;

与\#pragma pack(4)一样

若设为 \#pragma pack(1), 则结构大小: sizeof(struct) = 1+2+4+1=8;

若设为 \#pragma pack(2), 则结构大小: sizeof(struct) = 2+2+4+2=10;

在\#pragma pack(1)时:空间是节省了,但访问速度降低了;

有什么用处???

在系统通讯中,如和硬件设备通信,和其他的操作系统进行通信时等,必须保证双方的一致性。

[解析\#pragma指令](http://blog.csdn.net/scrub/archive/2005/10/02/494171.aspx) 

在所有的预处理指令中，\#pragma 指令可能是最复杂的了，它的作用是设定编译器的状态或 

者是指示编译器完成一些特定的动作。\#pragma指令对每个编译器给出了一个方法,在保持与

C和C++语言完全兼容的情况下,给出主机或操作系统专有的特征。依据定义,编译指示是机器

或操作系统专有的,且对于每个编译器都是不同的。

其格式一般为: \#pragma Para

其中Para 为参数，下面来看一些常用的参数。

 

(1)message 参数。 Message 参数是我最喜欢的一个参数，它能够在编译信息输出窗

口中输出相应的信息，这对于源代码信息的控制是非常重要的。其使用方法为：


 \\#pragma message(“消息文本”)

当编译器遇到这条指令时就在编译输出窗口中将消息文本打印出来。

当我们在程序中定义了许多宏来控制源代码版本的时候，我们自己有可能都会忘记有没有正

确的设置这些宏，此时我们可以用这条指令在编译的时候就进行检查。假设我们希望判断自

己有没有在源代码的什么地方定义了_X86这个宏可以用下面的方法

\\#ifdef _X86

\\#pragma message(“_X86 macro activated!”)

\\#endif

当我们定义了_X86这个宏以后，应用程序在编译时就会在编译输出窗口里显示“_

X86 macro activated!”。我们就不会因为不记得自己定义的一些特定的宏而抓耳挠腮了

。

 

(2)另一个使用得比较多的pragma参数是code_seg。格式如：

\\#pragma code_seg( [/section-name/[,/section-class/] ] )

它能够设置程序中函数代码存放的代码段，当我们开发驱动程序的时候就会使用到它。

 

(3)\#pragma once (比较常用）

只要在头文件的最开始加入这条指令就能够保证头文件被编译一次，这条指令实际上在VC6

中就已经有了，但是考虑到兼容性并没有太多的使用它。

 

(4)\#pragma hdrstop表示预编译头文件到此为止，后面的头文件不进行预编译。BCB可以预

编译头文件以加快链接的速度，但如果所有头文件都进行预编译又可能占太多磁盘空间，所


以使用这个选项排除一些头文件。

有时单元之间有依赖关系，比如单元A依赖单元B，所以单元B要先于单元A编译。你可以用#p

ragma startup指定编译优先级，如果使用了\#pragma package(smart_init) ，BCB就会根据

优先级的大小先后编译。

 

(5)\#pragma resource /*.dfm/表示把*.dfm文件中的资源加入工程。*.dfm中包括窗体

外观的定义。

 

(6)\#pragma warning( disable : 4507 34; once : 4385; error : 164 )

等价于：

\\#pragma warning(disable:4507 34) // 不显示4507和34号警告信息

\\#pragma warning(once:4385) // 4385号警告信息仅报告一次

\\#pragma warning(error:164) // 把164号警告信息作为一个错误。

同时这个pragma warning 也支持如下格式：

\\#pragma warning( push [ ,n ] )

\\#pragma warning( pop )

这里n代表一个警告等级(1---4)。

\\#pragma warning( push )保存所有警告信息的现有的警告状态。

\\#pragma warning( push, n)保存所有警告信息的现有的警告状态，并且把全局警告

等级设定为n。

\\#pragma warning( pop )向栈中弹出最后一个警告信息，在入栈和出栈之间所作的

一切改动取消。例如：


 \\#pragma warning( push )

\\#pragma warning( disable : 4705 )

\\#pragma warning( disable : 4706 )

\\#pragma warning( disable : 4707 )

//.......

\\#pragma warning( pop )

在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)。

（7）pragma comment(...)

该指令将一个注释记录放入一个对象文件或可执行文件中。

常用的lib关键字，可以帮我们连入一个库文件。

（8）·通过\#pragma pack(n)改变C编译器的字节对齐方式 
 在  C语言中，结构是一种复合数据类型，其构成元素既可以是基本数据类型（如int、long、float等）的变量，也可以是一些复合数据类型（如数组、结构、联合等）的数据单元。在结构中，编译器为结构的每个成员按其自然对界（alignment）条件分配空间。各个成员按照它们被声明的顺序在内存中顺序存储，第一个成员的地址和整个结构的地址相同。

​     例如，下面的结构各成员空间分配情况：
 struct test 
 {
​      char x1;
​      short x2;
​      float x3;
​      char x4;
 };
​       结构的第一个成员x1，其偏移地址为0，占据了第1个字节。第二个成员x2为short类型，其起始地址必须2字节对界，因此，编译器在x2和x1之间填充了一个空字节。结构的第三个成员x3和第四个成员x4恰好落在其自然对界地址上，在它们前面不需要额外的填充字节。在test结构中，成员x3要求4字节对界，是该结构所有成员中要求的最大对界单元，因而test结构的自然对界条件为4字节，编译器在成员x4后面填充了3个空字节。整个结构所占据空间为  12字节。更改C编译器的缺省字节对齐方式
​      在缺省情况下，C编译器为每一个变量或是数据单元按其自然对界条件分配空间。一般地，可以通过下面的方法来改变缺省的对界条件：
 　　· 使用伪指令\#pragma pack (n)，C编译器将按照n个字节对齐。
​      · 使用伪指令\#pragma pack ()，取消自定义字节对齐方式。

​     另外，还有如下的一种方式：
​      · __attribute((aligned (n)))，让所作用的结构成员对齐在n字节自然边界上。如果结构中有成员的长度大于n，则按照最大成员的长度来对齐。
​      · __attribute__ ((packed))，取消结构在编译过程中的优化对齐，按照实际占用字节数进行对齐。

以上的n = 1, 2, 4, 8, 16... 第一种方式较为常见。

应用实例

　　在网络协议编程中，经常会处理不同协议的数据报文。一种方法是通过指针偏移的方法来得到各种信息，但这样做不仅编程复杂，而且一旦协议有变化，程序修改起来也比较麻烦。在了解了编译器对结构空间的分配原则之后，我们完全可以利用这一特性定义自己的协议结构，通过访问结构的成员来获取各种信息。这样做，不仅简化了编程，而且即使协议发生变化，我们也只需修改协议结构的定义即可，其它程序无需修改，省时省力。下面以TCP协议首部为例，说明如何定义协议结构。其协议结构定义如下：

\\#pragma pack(1) // 按照1字节方式进行对齐
 struct TCPHEADER 
 {
      short SrcPort; // 16位源端口号
      short DstPort; // 16位目的端口号
      int SerialNo; // 32位序列号
      int AckNo; // 32位确认号
      unsigned char HaderLen : 4; // 4位首部长度
      unsigned char Reserved1 : 4; // 保留6位中的4位
      unsigned char Reserved2 : 2; // 保留6位中的2位
      unsigned char URG : 1;
      unsigned char ACK : 1;
      unsigned char PSH : 1;
      unsigned char RST : 1;
      unsigned char SYN : 1;
      unsigned char FIN : 1;
      short WindowSize; // 16位窗口大小
      short TcpChkSum; // 16位TCP检验和
      short UrgentPointer; // 16位紧急指针
 }; 
 \\#pragma pack() // 取消1字节对齐方式 

-- 选择自 vcforever 的 Blog 

在编写程序的时候,我们经常要用到\#pragma指令来设定编译器的状态或者是指示编译器完成一些特定的动作.
 下面介绍了一下该指令的一些常用参数,希望对大家有所帮助!

**一. message 参数**。 

message 
它能够在编译信息输出窗  
 口中输出相应的信息，这对于源代码信息的控制是非常重要的。

其使用方法为：    \#pragma message("消息文本")  

当编译器遇到这条指令时就在编译输出窗口中将消息文本打印出来。  
 当我们在程序中定义了许多宏来控制源代码版本的时候，我们自己有可能都会忘记有没有正确的设置这些宏，此时我们可以用这条
 指令在编译的时候就进行检查。假设我们希望判断自己有没有在源代码的什么地方定义了_X86这个宏可以用下面的方法  
 \\#ifdef _X86  
 \\#pragma message("_X86 macro activated!")  
 \\#endif  
 当我们定义了_X86这个宏以后，应用程序在编译时就会在编译输出窗口里显示"_  
 X86 macro activated!"。我们就不会因为不记得自己定义的一些特定的宏而抓耳挠腮了  
     
**二. 另一个使用得比较多的\#pragma参数是code_seg**。

格式如：

\\#pragma code_seg( [ [ { push | pop}, ] [ identifier, ] ] [ "segment-name" [, "segment-class" ] ) 
该指令用来指定函数在.obj文件中存放的节,观察OBJ文件可以使用VC自带的dumpbin命令行程序,函数在.obj文件中默认的存放节
 为.text节
 如果code_seg没有带参数的话,则函数存放在.text节中
 push (可选参数) 将一个记录放到内部编译器的堆栈中,可选参数可以为一个标识符或者节名
 pop(可选参数) 将一个记录从堆栈顶端弹出,该记录可以为一个标识符或者节名
 identifier (可选参数) 当使用push指令时,为压入堆栈的记录指派的一个标识符,当该标识符被删除的时候和其相关的堆栈中的记录将被弹出堆栈 
 "segment-name" (可选参数) 表示函数存放的节名
 例如:
 //默认情况下,函数被存放在.text节中
 void func1() {                  // stored in .text
 }

//将函数存放在.my_data1节中
 \\#pragma code_seg(".my_data1")
 void func2() {                  // stored in my_data1
 }

//r1为标识符,将函数放入.my_data2节中
 \\#pragma code_seg(push, r1, ".my_data2")
 void func3() {                  // stored in my_data2
 }

int main() {
 }

**三. \#pragma once (比较常用）** 

这是一个比较常用的指令,只要在头文件的最开始加入这条指令就能够保证头文件被编译一次
     
**四. \#pragma hdrstop表示预编译头文件到此为止，后面的头文件不进行预编译**。

BCB可以预编译头文件以加快链接的速度，但如果所有头文件都进行预编译又可能占太多磁盘空间，所以使用这个选项排除一些头文件。   
 有时单元之间有依赖关系，比如单元A依赖单元B，所以单元B要先于单元A编译。你可以用\#pragma startup指定编译优先级，
 如果使用了\#pragma package(smart_init) ，BCB就会根据优先级的大小先后编译。   

**五. \#pragma warning指令**

该指令允许有选择性的修改编译器的警告消息的行为
 指令格式如下:
 \\#pragma warning( warning-specifier : warning-number-list [; warning-specifier : warning-number-list...]
 \\#pragma warning( push[ ,n ] )
 \\#pragma warning( pop )

主要用到的警告表示有如下几个:

once:只显示一次(警告/错误等)消息
 default:重置编译器的警告行为到默认状态
 1,2,3,4:四个警告级别
 disable:禁止指定的警告信息
 error:将指定的警告信息作为错误报告

如果大家对上面的解释不是很理解,可以参考一下下面的例子及说明

 

\#pragma warning( disable : 4507 34; once : 4385; error : 164 )  

等价于：  

\#pragma warning(disable:4507 34)  // 不显示4507和34号警告信息  

\#pragma warning(once:4385)        // 4385号警告信息仅报告一次  

\#pragma warning(error:164)        // 把164号警告信息作为一个错误。  

同时这个pragma warning 也支持如下格式：  

\#pragma warning( push [ ,n ] )  

\#pragma warning( pop )  

这里n代表一个警告等级(1---4)。  

\#pragma warning( push )保存所有警告信息的现有的警告状态。  

\#pragma warning( push, n)保存所有警告信息的现有的警告状态，并且把全局警告等级设定为n。   

\#pragma warning( pop )向栈中弹出最后一个警告信息，在入栈和出栈之间所作的一切改动取消。例如：  

\#pragma warning( push )  

\#pragma warning( disable : 4705 )  

\#pragma warning( disable : 4706 )  

\#pragma warning( disable : 4707 )  

\#pragma warning( pop )

在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)

在使用标准C++进行编程的时候经常会得到很多的警告信息,而这些警告信息都是不必要的提示,所以我们可以使用\#pragma   warning(disable:4786)来禁止该类型的警告在vc中使用ADO的时候也会得到不必要的警告信息,这个时候我们可以通过\#pragma  warning(disable:4146)来消除该类型的警告信息

**六. pragma comment(...)**
该指令的格式为：  \#pragma comment( "comment-type" [, commentstring] )
      该指令将一个注释记录放入一个对象文件或可执行文件中,comment-type(注释类型):可以指定为五种预定义的标识符的其中一种
     五种预定义的标识符为:

​     compiler:将编译器的版本号和名称放入目标文件中,本条注释记录将被编译器忽略
​                   如果你为该记录类型提供了commentstring参数,编译器将会产生一个警告
​                   例如:\#pragma comment( compiler )

​     exestr:将   commentstring参数放入目标文件中,在链接的时候这个字符串将被放入到可执行文件中,当操作系统加载可执行文件的时候,该参数字符串不会被加载到内存中.但是,该字符串可以被dumpbin之类的程序查找出并打印出来,你可以用这个标识符将版本号码之类的信息嵌入到可        执行文件中!

​     lib:这是一个非常常用的关键字,用来将一个库文件链接到目标文件中常用的lib关键字，可以帮我们连入一个库文件。  
​          例如:               \#pragma comment(lib, "user32.lib")  
​                                该指令用来将user32.lib库文件加入到本工程中


      linker:将一个链接选项放入目标文件中,你可以使用这个指令来代替由命令行传入的或者在开发环境中设置的链接选项,你可以指定/include选项来强制包含某个对象,例如:        \#pragma comment(linker, "/include:__mySymbol")

​               你可以在程序中设置下列链接选项

​                          /DEFAULTLIB 
​                           /EXPORT 
​                           /INCLUDE 
​                           /MERGE 
​                          /SECTION 


               这些选项在这里就不一一说明了,详细信息请看msdn!

​      user:将一般的注释信息放入目标文件中commentstring参数包含注释的文本信息,这个注释记录将被链接器忽略
​       例如:     \#pragma comment( user, "Compiled on " __DATE__ " at " __TIME__ )

   解析\#pragma指令(转载)    

 

|      | 作者：[albue](http://www.tianya.cn/browse/listwriter.asp?vwriter=albue) 提交日期：2007-3-15 13:20:00 |      |
| ---- | ------------------------------------------------------------ | ---- |
|      |                                                              |      |

 在所有的预处理指令中，\#pragma  指令可能是最复杂的了，它的作用是设定编译器的状态或者是指示编译器完成一些特定的动作。\#pragma指令对每个编译器给出了一个方法,在保持与C和  C++语言完全兼容的情况下,给出主机或操作系统专有的特征。依据定义,编译指示是机器或操作系统专有的,且对于每个编译器都是不同的。
 
 其格式一般为: \#pragma Para 其中Para 为参数，下面来看一些常用的参数。
 
 (1)message 参数。 
  Message 参数是我最喜欢的一个参数，它能够在编译信息输出窗口中输出相应的信息，这对于源代码信息的控制是非常重要的。其使用方法为：
 \#pragma message(“消息文本”)
 当编译器遇到这条指令时就在编译输出窗口中将消息文本打印出来。
 当我们在程序中定义了许多宏来控制源代码版本的时候，我们自己有可能都会忘记有没有正确的设置这些宏，此时我们可以用这条指令在编译的时候就进行检查。假设我们希望判断自己有没有在源代码的什么地方定义了_X86这个宏可以用下面的方法
 
 \#ifdef _X86
 \#pragma message(“_X86 macro activated!”)
 \#endif
 
 当我们定义了_X86这个宏以后，应用程序在编译时就会在编译输出窗口里显示“_X86 macro activated!”。我们就不会因为不记得自己定义的一些特定的宏而抓耳挠腮了。
 
 (2)另一个使用得比较多的pragma参数是code_seg。格式如：
 
 \#pragma code_seg( [/section-name/[,/section-class/] ] )
 
 它能够设置程序中函数代码存放的代码段，当我们开发驱动程序的时候就会使用到它。
 
 (3)\#pragma once (比较常用）
 
 只要在头文件的最开始加入这条指令就能够保证头文件被编译一次，这条指令实际上在VC6中就已经有了，但是考虑到兼容性并没有太多的使用它。
 
 (4)\#pragma hdrstop表示预编译头文件到此为止，后面的头文件不进行预编译。BCB可以预编译头文件以加快链接的速度，但如果所有头文件都进行预编译又可能占太多磁盘空间，所以使用这个选项排除一些头文件。
 有时单元之间有依赖关系，比如单元A依赖单元B，所以单元B要先于单元A编译。你可以用\#pragma startup指定编译优先级，如果使用了\#pragma package(smart_init) ，BCB就会根据优先级的大小先后编译。
 
 (5)\#pragma resource /*.dfm/表示把*.dfm文件中的资源加入工程。*.dfm中包括窗体外观的定义。
 
 (6)\#pragma warning( disable : 4507 34; once : 4385; error : 164 )
 等价于：
 \#pragma warning(disable:4507 34) // 不显示4507和34号警告信息
 \#pragma warning(once:4385) // 4385号警告信息仅报告一次
 \#pragma warning(error:164) // 把164号警告信息作为一个错误。
 
 同时这个pragma warning 也支持如下格式：
 
 \#pragma warning( push [ ,n ] )
 \#pragma warning( pop )
 这里n代表一个警告等级(1---4)。
 
 \#pragma warning( push )保存所有警告信息的现有的警告状态。
 \#pragma warning( push, n)保存所有警告信息的现有的警告状态，并且把全局警告等级设定为n。
 \#pragma warning( pop )向栈中弹出最后一个警告信息，在入栈和出栈之间所作的一切改动取消。例如：
 
 \#pragma warning( push )
 \#pragma warning( disable : 4705 )
 \#pragma warning( disable : 4706 )
 \#pragma warning( disable : 4707 )
 //.......
 \#pragma warning( pop )
 
 在这段代码的最后，重新保存所有的警告信息(包括4705，4706和4707)。
 
 （7）pragma comment(...)
 
 该指令将一个注释记录放入一个对象文件或可执行文件中。
 常用的lib关键字，可以帮我们连入一个库文件。
 
 （8）·通过\#pragma pack(n)改变C编译器的字节对齐方式 
 在C语言中，结构是一种复合数据类型，其构成元素既可以是基本数据类型（如int、long、float等）的变量，也可以是一些复合数据类型（如数组、结构、联合等）的数据单元。在结构中，编译器为结构的每个成员按其自然对界（alignment）条件分配空间。各个成员按照它们被声明的顺序在内存中顺序存储，第一个成员的地址和整个结构的地址相同。
  例如，下面的结构各成员空间分配情况：
 struct test 
 {
  char x1;
  short x2;
  float x3;
  char x4;
 };
 结构的第一个成员x1，其偏移地址为0，占据了第1个字节。第二个成员x2为short类型，其起始地址必须2字节对界，因此，编译器在x2和x1之间填充了一个空字节。结构的第三个成员x3和第四个成员x4恰好落在其自然对界地址上，在它们前面不需要额外的填充字节。在test结构中，成员x3要求4字节对界，是该结构所有成员中要求的最大对界单元，因而test结构的自然对界条件为4字节，编译器在成员x4后面填充了3个空字节。整个结构所占据空间为  12字节。更改C编译器的缺省字节对齐方式
  在缺省情况下，C编译器为每一个变量或是数据单元按其自然对界条件分配空间。一般地，可以通过下面的方法来改变缺省的对界条件：
 　　· 使用伪指令\#pragma pack (n)，C编译器将按照n个字节对齐。
  · 使用伪指令\#pragma pack ()，取消自定义字节对齐方式。
 
  另外，还有如下的一种方式：
  · __attribute((aligned (n)))，让所作用的结构成员对齐在n字节自然边界上。如果结构中有成员的长度大于n，则按照最大成员的长度来对齐。
  · __attribute__ ((packed))，取消结构在编译过程中的优化对齐，按照实际占用字节数进行对齐。
 
 以上的n = 1, 2, 4, 8, 16... 第一种方式较为常见。
 
 应用实例
 
 　　在网络协议编程中，经常会处理不同协议的数据报文。一种方法是通过指针偏移的方法来得到各种信息，但这样做不仅编程复杂，而且一旦协议有变化，程序修改起来也比较麻烦。在了解了编译器对结构空间的分配原则之后，我们完全可以利用这一特性定义自己的协议结构，通过访问结构的成员来获取各种信息。这样做，不仅简化了编程，而且即使协议发生变化，我们也只需修改协议结构的定义即可，其它程序无需修改，省时省力。下面以TCP协议首部为例，说明如何定义协议结构。其协议结构定义如下：
 
 \#pragma pack(1) // 按照1字节方式进行对齐
 struct TCPHEADER 
 {
  short SrcPort; // 16位源端口号
  short DstPort; // 16位目的端口号
  int SerialNo; // 32位序列号
  int AckNo; // 32位确认号
  unsigned char HaderLen : 4; // 4位首部长度
  unsigned char Reserved1 : 4; // 保留6位中的4位
  unsigned char Reserved2 : 2; // 保留6位中的2位
  unsigned char URG : 1;
  unsigned char ACK : 1;
  unsigned char PSH : 1;
  unsigned char RST : 1;
  unsigned char SYN : 1;
  unsigned char FIN : 1;
  short WindowSize; // 16位窗口大小
  short TcpChkSum; // 16位TCP检验和
  short UrgentPointer; // 16位紧急指针
 }; 
 \#pragma pack() // 取消1字节对齐方式 