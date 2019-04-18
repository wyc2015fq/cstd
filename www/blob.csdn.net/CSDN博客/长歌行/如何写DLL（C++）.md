# 如何写DLL（C++） - 长歌行 - CSDN博客





2012年10月16日 21:39:50[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：3086








                
很久以前，刚入职的时候，什么都不懂，有同事教了下怎么写dll，然后自己开始慢慢接触dll君。感谢当初与我分享该方面知识的hzc同学（貌似是一个周六上午）。

所以也想和大家分享一下。


iuhsihsow个人心得。 


做稍微大一点的工程就不能不接触Dll了，将自己写的一些相对独立的，比较基础的，功能需求稳定的模块写成动态库，会减少后期很多工作量。

优点如下：

1.利于修改，无需重新编译整个工程。

2.利于复用，写个算法库什么的，到处用。（话说准备写一个图像方面的算法库的，结果拖着拖着就没有动力了。）

3.减少EXE大小。。。囧。。

缺点如下：

1.dll版本控制，需要在写dll的用点心。

2.增加了额外的工作量，也就是多几行代码，多配置几行。


环境。。VS2008  C ++  windows操作系统

liunx不讨论，平时用得不多，学了之后就忘了。

1.创建工程

新建工程->C++控制台->Dll

2.配置

配置属性->

                常规->输出目录                                 dll的输出目录

                C/C++->附加包含目录                      项目中头文件一般放的地方

                链接器->常规->

                                                                         输出文件                                            dll的输出路径

                                                                         附加库目录                                         第三方库



                              高级->                                 导入库                                               生成的库的名字如$(ProjectDir)..\..\..\..\lib\$(TargetName).lib

                生成事件->生成后事件->命令行          用于生成成功后copy源文件                 例如copy Base\*.h            $(ProjectDir)\..\..\..\..\include\

3.各种头文件
3.1.引用头文件

为了方便引用，一般定义一个头文件，里面包括了库的导入导出属性，库中需要引用的各个类，第三方库。如

#                ifdef XXXXX_EXPORTS 

#                          define  XXXXX _API __declspec( dllexport )

#                else

#                define  XXXXX _API __declspec( dllimport )

#                ifdef        _DEBUG

#                        pragma comment(lib, " XXXXX D.lib")

#                else

#                        pragma comment(lib, " XXXXX .lib")

#                endif

#                endif


#include " XXXXXDefine.h"

#include " XXXXX .h"


XXX为自己的工程名，XXX_EXPORTS为预编译头，可以去属性中设置


3.2数据头

将自己定义的数据放在一个头文件中可以保持一致性，用起来很方便


3.3预编译头

放些工程中引用很多的东西，<windows.h>引用的第三方库，本工程常用的头文件。等等，看各人习惯好坏了。

4.导出类

写好类后，在类前面加个 XXXX_API，这个在前面有定义过。

5.可以使用DEPENDS.EXE检测库。

6.使用自己写的库

引用头文件，dll放在客户程序能够读到得地方，就能用了。。。。



