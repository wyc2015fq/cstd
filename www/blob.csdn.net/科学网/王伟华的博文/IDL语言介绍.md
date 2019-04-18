# 科学网—IDL语言介绍 - 王伟华的博文




# IDL语言介绍                           

已有 13148 次阅读2010-5-5 19:50|个人分类:[计算机&大数据](http://blog.sciencenet.cn/home.php?mod=space&uid=81613&do=blog&classid=11842&view=me)|系统分类:[科研笔记](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=81613&catid=1)|关键词:IDL语言介绍|[IDL语言介绍](misc.php?mod=tag&id=61394)



     和一个老乡聊天，她在做什么？答曰处理数据。无心地问了一句，你们用什么处理数据呢，答曰： IDL、  MatLab. 出于职业地敏感，我详细地问了一下IDL的一些情况，不问不知道，一问吓一跳，或许是咕噜寡闻，或许是隔行如隔山，如此强大的软件，我竟然不知道了。实在惭愧！（以前觉得，在数据处理方面，SARS, SPSS, Matlab, Mathematic,Maple，Fortran 几乎可以打遍天下！）从网上找了些IDL的资料，恶补一下。

1、IDL是什么？

美国RSI公司（现并入ittvis－－编者注）的旗舰产品－－IDL（Interactive Data Language）交互式数据语言是进行二维及多维数据可视化表现、分析及应用开发的理想软件工具。作为面向矩阵、语法简单的第四代可视化语言,IDL致力于科学数据的可视化和分析，是跨平台应用开发的最佳选择。早在1982年NASA的火星飞越航空器的开发就使用IDL软件，到现在IDL已经被列为国外许多大学的标准课程，IDL使科研人员无须编写传统程序就可直接研究数据 

IDL(Interface Definition Language)即接口定义语言，是CORBA规范的一部分，是跨平台开发的基础。IDL提供一套通用的数据类型，并以这些数据类型来定义更为复杂的数据类型。可变化 IDL 基本类型 整数类型 OMG IDL 摒弃int 类型在不同平台上取值范围不同带来的多义性的问题。常数定义常数可以是整数、字符、浮点数、字符串、Boolean、octet 或枚举型，不能是 any 类型或用户定义的类型。OMG IDL数组类型IDL array 和 sequence，可以轻易地被映射到实现语言中。序列可以包含所有类型的元素，不管是基本类型还是用户定义的类型。

2、IDL语言是什么？

简单的说，IDL是VC、VB、JAVA、FORTRAN、MATLAB、OPENGL等语言的集成。 IDL语言与大型图形和GIS应用软件相距近，应用IDL可以快速的开发出功能强大的三维图形图像处理软件和三维GIS应用系统。

从本质上讲，OMG IDL接口定义语言不是作为程序设计语言体现在CORBA体系结构中的，而是用来描述产生对象调用请求的客户对象和服务对象之间的接口的语言。OMG IDL文件描述数据类型和方法框架，而服务对象则为一个指定的对象实现提供上述数据和方法。 
　　OMG IDL文件描述了服务器提供的服务功能，客户机可以根据该接口文件描述的方法向服务器提出业务请求。在大多数CORBA产品中都提供IDL到相关编程语言的编译器。程序设计人员只需将定义的接口文件输入编译器，设定编译选项后，就可以得到与程序设计语言相关的接口框架文件和辅助文件。

IDL文件应用过程如图1所示。 
![](http://www.cppblog.com/images/cppblog_com/alantop/corba_3.jpg)

图1 OMG IDL文件编译过程



3、IDL语言能干什么？

由于其强大的功能和独特的特点，IDL语言可以应用于任何领域的三维数据可视化、数值计算、三维图形建模、科学数据读取等功能中。概括说来，在地球科学（包括气象、水文、海洋、土壤、地质、地下水等）、医学影像、图像处理、GIS系统、软件开发，大学教学，实验室，测试技术，天文，航空航天、信号处理，防御工程，数学统计及分析，环境工程等很多领域，IDL语言都可以得到广泛而又深远的应用。

采用OMG IDL这样的说明性语言，其目的在于克服特定编程语言在软件系统集成及互操作方面的限制，这正是CORBA的诱人之处，同样也体现出采用CORBA构造分布式应用程序在网络时代的强大生命力。OMG IDL已经为C、C++、Java等主要高级程序设计语言制定了IDL到高级编程语言的映射标准。项目开发人员可以根据需要选择自己最熟悉的编程语言来独立开发基于CORBA的应用，而对软件系统的互操作能力不产生影响。 

4、IDL已干出了什么？

目前应用IDL语言，已经开发出了ENVI、IMAGIS、RiverTools、医学等成熟产品。具体的应用实例也非常多，如在2000年澳大利亚悉尼奥运会综合预报系统、美国国家环境卫星数据和信息服务中心的厄尔尼诺现象分析等工作中得到了成功的应用。作为面向矩阵、语法简单的第四代可视化语言，IDL致力于科学数据的可视化和分析，是跨平台应用开发的最佳选择。它集可视化、交互分析、大型商业开发为一体，为用户提供完善、灵活、有效地开发环境。

OMG IDL的语法规则 
1. OMG IDL文件举例 
　　module Compute 
　　{ typedef double radius; 
　　typedef long times; 
　　interface PI 
　　{ double getResult( in radius aRadius, in times time); } 
　　} 
　　上述接口定义文件主要用于客户端向服务对象提出请求：计算π值。因此，模块中定义了一个方法getResult()，以圆的直径（aRadius）和迭代次数（times）作为该方法的输入参数。 
2. OMG IDL词法规则 
　　OMG IDL采用ASCII字符集构成接口定义的所有标识符。标识符由字母、数字和下划线的任意组合构成，但第一个字符必须是ASCII字母。IDL认为大写字母和小写字母具有相同的含义，例如anExample和AnExample是相同的。 
　　与C++和Java类似，采用以“/*”开始，以“*/”结束来注释一段代码，以“//”开始注释从“//”开始直至行尾的所有内容。 
　　另外，IDL保留了47个关键字，程序设计人员不能将关键字用作变量或方法名。需要注意的是关键字的大小写，例如： 
　　typedef double context; 
　　//错误：定义的变量context是关键字 
　　typedef double CONTEXT; 
　　//错误：CONTEXT与关键字context冲突 

3,接口（interface） 
　　接口作为服务对象功能的详细描述，封装了服务对象提供服务方法的全部信息，客户对象利用该接口获取服务对象的属性、访问服务对象中的方法。 
　　接口用关键字interface声明，其中包含的属性和方法对所有提出服务请求的客户对象是公开的，如下例： 
　　interface JobManager 
　　{ readonly attribute string FirstName; 
　　attribute string status; 
　　string QueryJobStatus( in long Number, out string property); }


5、 IDL在数字信号与图像处理的优势： 1.高级的智能工具箱 iTools(intelligent tools)的出现是IDL6.0向易用性和易扩展性迈出的重要一步。iTools是由一批工具箱组成，其中包括iPlot、iSurface、iContour、iImage、iVolume等等。IDL使用者可以通过鼠标进行拖放即可完成数据的可视化以及分析工作。 2.极高的运算效率 IDL采用面向矩阵的技术，具有高效编程特点的同时，自动支持多CPUs机制，可以大大提高计算速度。另外，由于IDL对于数据类型的要求比较灵活，可以避免Matlab中强制64Bit参与计算的瓶颈，使得IDL更加适用于工程计算和相关软件的开发。 3.支持海量数据随着计算机应用水品的不断提高，人们数据量的要求也在不断提高。一些朋友反映说好像 IDL对于开辟矩阵的大小有些置疑，这里我需要补充一下。大部分朋友是受了硬件厂商的错觉，硬件厂商宣称自己的计算机能够有多大的内存，实际上是指内存的管理而不是说明能够一次开辟这么大的矩阵。并且，还有一些操作系统方面的限制。举例说明，Windows系列都是32位的操作系统，一次性最大开辟内存是2G，除了系统本身占用资源外，最多能够开到1.2－1.5G就非常不错了，因此，由此看来，IDL在利用内存上还是不错的。如果在64位的操作系统上，问题就会得到缓解。有人曾经在Linux上测试，利用IDL开辟4G的矩阵没有问题。由此完全客服了数据量的瓶颈限制和分块读取的麻烦。 4.灵活的外部语言接口做为高级的语言工具，IDL提供丰富而灵活的语言接口。可以与C、C＋＋、Fortran、VB、 Java、VC等商业开发平台进行相互调用，尤其是IDL中Java Bridge技术，可以在IDL编程环境中直接创建和管理Jave的对象。与此同时，IDL还可以直接调用IMS以及Lapack等算法库，使得科学计算与分析更为方便快捷。同时，IDL还提供数据库的标准接口，可以对各种大型商业数据进行管理以及的高级应用开发。 

IDL语言数据类型有什么？

（1）基本数据类型：OMG IDL基本数据类型包括short、long和相应的无符号（unsigned）类型，表示的字长分别为16、32位。 
　　（2）浮点数类型：OMG IDL浮点数类型包括float、double和long double类型。其中float表示单精度浮点数，double表示双精度浮点数，long double表示扩展的双精度浮点数。 
　　（3）字符和超大字符类型：OMG IDL定义字符类型char为面向字节的码集中编码的单字节字符; 定义类型wchar为从任意字符集中编码的超大字符。 
　　（4）逻辑类型：用boolean关键字定义的一个变量，取值只有true和false。 
　　（5）八进制类型：用octet关键字定义，在网络传输过程中不进行高低位转换的位元序列。 octet 是 8 位类型。因为保证了 octet 在地址空间之间传送时不会有任何表示更改，因此这就使它变成了一种非常重要的类型。这就表示您可以发送二进制数据，并且知道当将它打包时，它的形式仍然相同。其它每种 IDL 类型在传输时都有表示变化。例如，根据 IOR 代码集信息的指示，char 数组会经历代码集转换。而 octet 数组却不会。
　　（6）any数据类型：引入该类型用于表示OMG IDL中任意数据类型。 
（7） 常量 
　　OMG IDL用const关键字声明一个常量，用于模块（module）或接口（interface）中定义保持不变的量，如： 
　　const double PI = 3.1415926; 
　　在IDL中，可以定义long、unsigned long、unsigned short、char、boolean、float、double、string类型的常量。 

（8）结构类型： 
　　typedef long GoodsNumber; 
　　struct 
　　{ GoodsNumber number; 
　　string name; 
　　float price; } 
（9）联合类型： 
　　union stockIn switch( short ) 
　　{ case 1: stocker : long; 
　　case 2: goodsName1 : string; 
　　case 3: goodsName2 : string; } 
（10）枚举类型： 
　　enum GoodsStatus { GOODS_SALED, GOODS_INSTOCK}; 
（11） 数组类型 
　　OMG IDL的数组类型提供了多维定长、统一数据格式的数据存储方式——数组。每一维的长度必须在定义时给定，所有数据单元必须存储相同类型的元素。如下例定义一个长度为20×100的整数数组： 
　　typedef long aDimension[20][100]; 
（12）模板（template）类型 
　　OMG IDL提供两种类型的模板： 
（13）序列（sequence）类型: 
　　用该方法定义长度可变的任意数值类型的存储序列，通常在定义时可以指定长度，也可以不指定，如： 
typedef sequence <long，80> aSequence; 
　　//长度定义为80 
　　typedef sequence <long> anotherSequence; 
　　//长度不定 字符串（string）序列： 
　　同样对于字符串序列类型，也有两种定义方式： 
　　typedef string <80> aName; //长度定义为80 
　　typedef string anotherName; //长度不定 



附：以下来自CSDN博客：[http://blog.csdn.net/lfkupc/archive/2009/08/08/4426019.aspx](http://blog.csdn.net/lfkupc/archive/2009/08/08/4426019.aspx)


1. IDL-Interactive Data Language

　　IDL 是一种数据分析和图像化应用程序及编程语言，先由美国ITT公司所有。最初在七十年代后期用于帮助科学家分析火星探险卫星发回的数据，将浩瀚的数据转换为图形。从此后，IDL 得到广泛运用，使用者日众，呈几何级别增长。

　　IDL 使用者可以迅速且方便地运用此软件将数据转换为图象，促进分析和理解。通过软件转化的图像既可以是简单色彩，也可以是全色三维图像和模型。

　　一。 IDL开发环境

　　IDLDE是用户快速编写、测试并调试代码的最佳环境，用户可以在该环境中方便地进行应用开发。IDLDE具有完善的代码编译、调试环境。IDLGUIBuilder 可以迅速开发跨平台的用户图形界面(GUI),而无需熟悉IDL控件编程知识。用户可以拖放式建立图形用户界面GUI，非常灵活、快速地产生应用程序的界面。IDL GUIBuilder已被集成到Idl开发环境中(IDLDE)。用户还可方便地输出project，生成可执行文件。

　　二。 灵活的数据输入和输出

　　IDL是完全面向矩阵的，因此它具有快速分析超大规模数据的能力。IDL可以通过灵活方便的I/O分析任何数据。可以读取和输出任意有格式或者无格式的数据类型，支持通用文本及图形数据，并且支持在NASA、TPT、NOAA等机构中大量使用的HDF、CDF及netCDF等科学数据格式，以及医学扫描设备的DICOM标准格式。IDL还支持字符、字节、16位整型、长整型、浮点、双精度、复数等多种数据类型。从IDL5.5起还可以支持MrSID压缩数据格式。

　　三。 IDL——快速实现可视化

　　IDL支持OpenGL软件或硬件加速，可加速交互式的2D及3D数据分析、图像处理及可视化，除了保留传统的直接图形法外，IDL还采用了先进的面向对象技术；可以实现曲面的旋转和飞行; 用多光源进行阴影或照明处理；可观察实体(Volume)内部复杂的细节；一旦创建对象后，可从各个不同的视角对对象进行可视分析，而不用费时地反复重画。IDL具有强大的数据分析能力，从IDL5.5起支持多进程运算，IDL带有完善的数学分析和统计软件包，提供强大的科学计算模型，支持IMSL函数库。它的图像处理软件包提供了大量方便的分析工具、地图投影变换软件包，使您开发GIS易如反掌。
　　IDL提供了可缩放的TrueType字体，可以注记中文;能将结果存为标准图像格式或PostScript格式，并尽可能地使图像质量最优化。
　　例如： 2－D 绘图
　　等直线分析

　　四。外部语言接口

　　IDL支持COM/ActiveX组件，可将您的IDL应用开发集成到与COM兼容的环境中。从Visual Basic、Visual C++等访问IDL，还可以通过动态连接库和COM组件方式在IDL程序里调用C 、Fortran 等程序。用 IDL DataMiner 可快速访问、查询并管理与ODBC兼容的数据库，支持Oracle, Informix, Sybase, MS SQL 等数据库。可以创建、删除、查询表格，执行任意的SQL命令，读取/设置/查询/增加/删除记录等操作。
　　例如：图像和信号处理
　　3D、4D曲线和曲面分析
　　三维体数据分析
　　IDL的小波变换工具包，主要用于信号处理和图像处理、去除噪声、图像压缩、特征提取、提取图像细节、其信息量损失比 FFT小得多。

　　五。应用领域

　　IDL为用户提供了可视数据分析的解决方案，早在1982年NASA火星飞越航空器的开发就使用了IDL软件。IDL使科学家无须写传统程序就可直接研究数据。IDL还被广泛应用于地球科学、医学影像、图像处理、软件开发、大学教学、实验室研究、测试技术、天文、信号处理、防御工程、数学分析、统计等诸多领域。

　　六。 广泛的支持平台：
　　Microsoft Windows 95/98/2000
　　Microsoft Windows NT(x86/Alpha)
　　Open VMS
　　SunOS
　　Solaris
　　Solaris X86
　　AIX
　　Digital UNIX
　　HP-UX
　　IRIX
　　Linux(Intel x86)
　　MacOS

　　2。 接口定义语言（Interface Definition Language）的缩写，使用Java(TM)编写的API提供基于标准的和CORBA的交互性和连接性。

　　·Used to describe "interfaces"
　　similar to RPC .x file
　　Like a contract
　　Defines protocol to access objects
　　·Builds on OOP principle of encapsulation
　　Clear boundary between implementation and interface
　　·Well-specified, Language-independent
　　interface specifies names and methods that clients can use (invoke)
　　same lexical rules as C++ syntax
　　IDL modules
　　– logical grouping of interface and type definitions
　　– defines naming scope
　　IDL interface
　　– methods that are available in CORBA objects implementing the interface
　　– inheritance using ’:’ for example interface Z inherits interfaces A and B interface Z:A,B{ };
　　IDL methods
　　– specify signatures
　　– parameters are labeled as in, out and inout
　　– one way indicates that client will not be blocked when invoking this method
　　these methods use maybe invocation semantics
　　– raises: user-defined exceptions


以下来自[http://www.gispark.com/html/programme/2007/0913/1280.html](http://www.gispark.com/html/programme/2007/0913/1280.html)

一、IDL语言是什么？ 



　　IDL语言的英文全称是Interactive Data Language。
　　简单的说，IDL是VC、VB、JAVA、FORTRAN、MATLAB、OPENGL等语言的集成。 





　　1、IDL ＝VC 本文来GIS公园

　　从菜单的定制、消息的传递、类的定义与继承等方面来说，IDL如同VC一样具备了强大的功能。 





　　2、IDL＝VB 





　　从可视化界面的设计、语言的通俗易懂、编程的入门等方面来说，IDL如同VB一样，让初学者心旷神怡。 





　　3、IDL＝JAVA GIS公园-GIS专业网站

　　从跨平台的移植来说，IDL的功能与JAVA一样强大。应用IDL开发的产品，只要微不足道的工作就可以在各平台之间任逍遥。 





　　4、IDL＝FORTRAN www.gispark.com

　　FORTRAN语言是科学计算领域一门最普遍的语言，是科学家最为广泛应用的语言。从函数、子程序的调用、数据传递、语言风格、语言组织等来说，IDL语言与FORTRAN则是如此的相像，见到了IDL，你就会想起FORTRAN语言。 本文来GIS公园

　　5、IDL＝MATLAB 本文来GIS公园

　　IDL语言象MATLAB一样，提供了大量封装和参数化了的数学函数，为科学计算插上了强大的翅膀。 www.gispark.com

　　6、IDL＝OPENGL 





　　IDL的提供了丰富的二维、三维图形图像类，其功能可与OPENGL媲美，而且其封装好的图形函数类的编程功能远超过OPENGL函数库。
　　同时IDL又是如此完美的ActiveX控件，在VB、VC中通过ActiveX控件技术能完整的再现IDL的丰富而又巨大的功能。
　　此外，从另一种意义上说，IDL语言与大型图形和GIS应用软件相距又是如此之近。应用IDL可以快速的开发出功能强大的三维图形图像处理软件和三维GIS应用系统。 本文来GIS公园

　　二、IDL语言能干什么？ 





　　由于其强大的功能和独特的特点，IDL语言可以应用于任何领域的三维数据可视化、数值计算、三维图形建模、科学数据读取等功能中。
　　概括说来，在地球科学（包括气象、水文、海洋、土壤、地质、地下水等）、医学影像、图像处理、GIS系统、软件开发，大学教学，实验室，测试技术，天文，航空航天、信号处理，防御工程，数学统计及分析，环境工程等很多领域，IDL语言都可以得到广泛而又深远的应用。 





　　三、IDL已干出了什么？ 





　　目前应用IDL语言，已经开发出了ENVI、IMAGIS、RiverTools、医学等成熟产品。具体的应用实例也非常多，如在2000年澳大利亚悉尼奥运会综合预报系统、美国国家环境卫星数据和信息服务中心的厄尔尼诺现象分析等工作中得到了成功的应用。
　　我们的团队应用IDL语言，已开发了功能非常强大的真三维地质分析系统AutoDig，能够直接对简单的地质数据，或其它带层次性的数据实现科学的、完整的三维建模；同时也提供真三维显示功能，不仅能对三维体实现任意的旋转、放大、缩小，而且也能实现交互式的真三维切割功能。 



附

[科学数据可视化工具IDL 产品白皮书](/upload/blog/file/2010/5/2010519162457487218.pdf)

[可视化软件](http://www.sciencenet.cn/m/user_content.aspx?id=318000)

常微分方程互动软件
- [DsTools](http://www.lorentz.leidenuniv.nl/%7Epatra/dstool/) (Unix, X window, Linux). 
- [Kltools](http://archive.ncsa.uiuc.edu/People/heiland/kltool/) (Unix) 
- [Phaser](http://www.math.miami.edu/%7Ephaser/) (Window) 
- [MDEP](http://archives.math.utk.edu/software/msdos/diff.equations/mdep/.html) (Dos) 




转载本文请联系原作者获取授权，同时请注明本文来自王伟华科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-81613-320261.html](http://blog.sciencenet.cn/blog-81613-320261.html)

上一篇：[[转载]刘道玉 我的人才理念与实践](blog-81613-316693.html)
下一篇：[[转载]火车带你游西藏（犹如身临其境）](blog-81613-321101.html)


