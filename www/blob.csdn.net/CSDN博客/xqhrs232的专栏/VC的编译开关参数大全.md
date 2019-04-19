# VC的编译开关参数大全 - xqhrs232的专栏 - CSDN博客
2012年09月02日 15:59:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1110标签：[编译器																[优化																[microsoft																[多线程																[build																[汇编](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=build&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://www.cnblogs.com/Kane_zzt/archive/2009/02/04/1383797.html](http://www.cnblogs.com/Kane_zzt/archive/2009/02/04/1383797.html)
可以通过Settings -->Project-->C/C++-->Customize来设置这个编译开关。
VC 编译命令开关
/C:      在预处理输出中保留注释语句
/c:       只编译，不连接，相当于在"Build"菜单下选择了"Compile"
/D:      定义常量和宏，与源程序里的#define 有相同效果
/E:      预处理C、C＋＋源文件，将源文件中所有的预编译指令及宏展开，将注释去掉，然后将预处理器的输出拷贝至标准输出设备输出，并且在每个文件的开头和末尾加入#line
/EH:    指定编译器用何种异常处理模型
/EP:     同/E,只是去掉了#line
/F:       设置程序的堆栈大小
/FA:     设置生成何种列表文件（汇编、汇编与机器码、汇编与源码、汇编与机器码以及源码）
/Fa:     指定用/FA设置的列表文件的存放路径及（或）文件名
/FD:     生成文件的相互依赖信息
/Fd:      设置程序数据库文件（PDB）的存放路径及（或）文件名
/Fe:      设置最终可执行文件的存放路径及（或）文件名
/FI:      预处理指定的头文件，与源文件中的＃include有相同效果
/Fm:     创建map文件
/Fo:      设置编译后Obj文件的存放路径及（或）文件名
/Fp:      设置预编译文件（pch）的存放路径及（或）文件名
/FR:      生成浏览信息（sbr）文件
/Fr:       同/FR,不同之处在于/Fr不包括局部变量信息
/G3:      为80386处理器优化代码生成
/G4:      为80486处理器优化代码生成
/G5:      为Pentium处理器优化代码生成
/G6:      为Pentium Pro处理器优化代码生成
/GA:      为Windows应用程序作优化
/GB:      为Pentium处理器优化代码生成，使用80386、80486、Pentium、Pentium Pro的混合指令集，是代码生成的默认选项（程序属性选项中Processor对应Blend）
/GD:      为Windows动态库（dll）作优化，此开关在VC6中没有实现
/Gd:      指定使用__cdecl的函数调用规则
/Ge:      激活堆栈检测
/GF:      消除程序中的重复的字符串，并将她放到只读的缓冲区中
/Gf:       消除程序中的重复字符串
/Gh:      在每个函数的开头调用钩子（hook）函数--penter
/Gi:       允许渐进编译
/Gm:     允许最小化rebuild
/GR:      允许运行时类型信息(Run-Time Type Infomation)
/Gr:       指定使用__fastcall的函数调用规则
/Gs:       控制堆栈检测所用内存大小
/GT:       支持用__declspec(thread)分配的数据的fier-safety
/GX:       允许同步异常处理，与/EHsc开关等价
/Gy:       允许编译器将每一个函数封装成COMDATs的形式，供连接器调用
/GZ:       允许在Debug build 的时候捕捉Release build的错误
/Gz:       指定使用__stdcall的函数调用规则
/H:         限制外部名字的长度
/HELP:    列出编译器的所有的命令开关
/I:         指定头文件的搜索路径
/J:         将char的缺省类型从signed char改成unsigned char
/LD:       创建一个动态连接库
/LDd:      创建一个Debug版本的动态链接库
/link:      将指定的选项传给连接器
/MD:       选择多线程、DLL版本的C Run－Time库
/MDd:     选择多线程、DLL、Debug版本的C Run－Time库
/ML:       选择单线程版本的C Run—Time库
/MLd:     选择单线程、Debug版本的C Run—Time库
/MT:       选择多线程版本的C Run-Time库
/MTd:     选择多线程、Debug版本的C Run—Time库
/nologo: 不显示程序的版权信息
/O1:      优化使产生的可执行代码最小
/O2:      优化使产生的可执行代码速度最快
/Oa:      指示编译器程序里没有使用别名，可以提高程序的执行速度
/Ob:      控制内联（inline）函数的展开
/Od:      禁止代码优化
/Og:      使用全局优化
/Oi:       用内部函数去代替程序里的函数调用，可以使程序运行的更快，但程序的长度变长
/Op:      提高浮点数比较运算的一致性
/Os:      产生尽可能小的可执行代码
/Ot:      产生尽可能块的可执行代码
/Ow:     指示编译器在函数体内部没有使用别名
/Ox:      组合了几个优化开关，达到尽可能多的优化
/Oy:      阻止调用堆栈里创建帧指针
/Q1f:     对核心级的设备驱动程序生成单独的调试信息
/QI0f:    对Pentium 0x0f错误指令作修正
/Qifdiv:  对Pentium FDIV错误指令作修正
/P:        将预处理输出写到指定文件里，文件的后缀名为I
/TC:      将命令行上的所有文件都当作C源程序编译，不管后缀名是否为.c
/Tc:      将指定的文件当作C源程序编译，不管后缀名是否为.c
/TP:      将命令行上的所有文件都当作C＋＋源程序编译，不管后缀名是否为.cpp
/Tp:      将指定文件当作C＋＋源程序编译，不管后缀名是否为.cpp
/U:       去掉一个指定的前面定义的符号或常量
/u:       去掉所有前面定义的符号或常量
/V:       在编译的obj文件里嵌入版本号
/vd:      禁止/允许构造函数置换
/vmb:   选择指针的表示方法，使用这个开关，在声明指向某个类的成员的指针之前，必须先定义这个类
/vmg:   选择指针的表示方法，使用这个开关，在声明指向某个类的成员的指针之前，不必先定义这个类，但要首先指定这个类是使用何种继承方法
/vmm:  设置指针的表示方法为Single Inheritance and Multiple Inheritance
/vms:   设置指针的表示方法为Single Inheritance
/vmv:   设置指针的表示方法为Any class
/W:      设置警告等级
/w:       禁止所有警告
/X:       阻止编译器搜索标准的include 目录
/Yc:     创建预编译头文件（pch）
/Yd:     在所有的obj文件里写上完全的调试信息
/Yu:     在build过程中使用指定的预编译头文件
/YX:     指示编译器若预编译头文件存在，则使用它，若不存在，则创建一个
/Z7:     生成MSC7.0兼容的调试信息
/Za:     禁止语言扩展(Microsoft Extensions to C)
/Zd:     调试信息只包含外部和全局的符号信息以及行号信息
/Ze:     允许语言扩展(Microsoft Extensions to C)
/Zg:     为源文件里面定义的每个函数生成函数原型
/ZI:      生成程序库文件（Pdb）并支持Edit and Continue调试特性
/Zi:      生成程序库文件（pdb），包含类型信息和符号调试信息
/ZL:     从obj文件里去掉缺省的库文件名
/Zm:    设置编译器的内存分配xianzhi
/Zn:     禁止浏览信息文件里面的封装
/Zp:     设置结构成员在内存里面的封装格式
/Zs:     快速检查语法错误
－－－－－－－－－－－－－－－－－－－－－－－－－－
vc所支持的文件类型
DSW:  全称是Developer Studio Workspace，最高级别的配置文件，记录了整个工作空间的配置信息，她是一个纯文本的文件，在vc创建新项目的时候自动生成
DSP:   全称是Developer Studio Project，也是一个配置文件，不过她记录的是一个项目的所有配置信息，纯文本文件
OPT： 与DSW、DSP配合使用的配置文件，她记录了与机器硬件有关的信息，同一个项目在不同的机器上的opt文件内容是不同的
CLW：记录了跟ClassWizard相关的信息，如果丢失了clw文件，那么在Class View面板里就没有类信息
PLG： 实际上是一个超文本文件，可以用Internet Explorer打开，记录了Build的过程，是一个日志型文件
RC：   资源描述文件，记录了所有的资源信息，在资源编辑器里作的修改，实际上都是对RC文件的修改
RC2： 附加的资源描述文件，不能直接资源编辑器修改，只能手工添加，可以用来添加额外的资源
RES： 经过资源编辑器编译之后的资源文件，以二进制方式存放
SBR： 编译器生成的浏览信息文件，在代码导航的时候非常有用，她需要在编译时指定/FR或者/Fr开关
BSC： BSCMAKE.EXE将所有的SBR文件作为输入，经过处理之后输出一个BSC文件，在代码导航的时候实际用到的是BSC文件
ILK： 当选定渐增型编译连接时，连接器自动生成ILK文件，记录连接信息
PDB： 全称是Program DataBase，即程序数据库文件，用来记录调试信息，是一个相当重要的文件，没有他，程序无法正常调试
LIB：  如果项目输出是Dll的话，一般会输出一个跟项目同名的Lib文件，记录输出的函数信息
EXP： 同Lib，是跟Dll一起生成的输出文件
PCH： 全称是PreCompiled Header，就是预先编译好的头文件，在编译时指定/Yu开关时编译器自动生成 
