# matlab打开和生成使用.p文件和exe文件 - 心纯净，行致远 - CSDN博客





2019年03月13日 13:01:32[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：68








首先，P文件为了保护知识产权设计的一种加密文件，是不能查看的。运行比较简单，和调用m文件方法一样。

下面是一些解释。

       P文件是对应M文件的一种预解析版本（preparsedversion）。因为当你第一次执行M文件时，Matlab需要将其解析（parse）一次（第一次执行后的已解析内容会放入内存作第二次执行时使用，即第二次执行时无需再解析），这无形中增加了执行时间。所以我们就预先作解释，那么以后再使用该M文件时，便会直接执行对应的已解析版本，即P文件。但又因为Matlab的解析速度非常快，一般不用自己作预解析。只有当一些程序要调用到非常多的M文件时，如GUI应用程序时，才会作预解析，以增加以后的调用速度。

P文件可以用来作保密代码之用，如果你给别人一个M文件，别人可以打开来看到你所有的代码和算法。如果你的代码不想被别人看到，那可以给他P文件。


一、如何使用.p文件

1.首先，将当前工作目录切换到.p文件所在的目录，然后就可以在左侧的工作空间窗口看见该目录所包含的所有文件了，当然包括.p文件啦~


2.由于.p文件是.m文件的预解析版本（不能查看、编辑），显然.p文件同.m文件一样都是一个函数体，只不过此时该函数是一个黑盒子罢了。举例如test.p文件便意味着存在一个已经编好的函数test（如何实现我们不知道），为调用这个函数test，我们需要知道它的参数形式此时在命令窗口下键入help test，即可查看到test函数的参数然后我们就可以调用啦



如何自己制作.p文件


在Matlab中，我们经常把语句或函数写在一个M文件里，便于模块化和代码重用，比如，写一个test.m的文件（即自定义一个test函数）：

%———————-

%test.m

a =1;

b =2;

%———————-

这个M语句文件，或：

%———————-

%test2.m

functiona = test2(b)

a =b * 2;

%———————-

这个M函数。

我们在Command Window里输入：

>>test

即是等于输入了

>>a = 1;

>>b = 2;

而输入：

>>a = test2(2)

a =

     4

便是执行了一个函数。

以上的内容我想大部份人都知道是怎么一回事了，以下说一说P代码文件。

如果在Command Window里输入：

>>pcode test

便会在相应test.m的文件夹里产生了test.p。如果这时在Command Window里输入：

>>test

其实是运行了test.p，而不是test.m。


P文件是对应M文件的一种预解析版本（preparsed version）。因为当你第一次执行M文件时，Matlab需要将其解析（parse）一次（第一次执行后的已解析内容会放入内存作第二次执行时使用，即第二次执行时无需再解析），这无形中增加了执行时间。所以我们就预先作解释，那么以后再使用该M文件时，便会直接执行对应的已解析版本，即P文件。但又因为Matlab的解析速度非常快，一般不用自己作预解析。只有当一些程序要调用到非常多的M文件时，如GUI应用程序时，才会作预解析，以增加以后的调用速度。

如Matlab的当前目录（Current Directory）有test.m文件，作预解析后，又有test.p文件。因为P文件的调用优先级比M文件要高，所以当你调用test时，会作优先选择而调用了test.p。

可以修改test.m的代码为：

%———————-

%test.m

a =3;

b =4;

%———————-

再在Command Window里调用test：

>>test

Warning:P-file C:\Program Files\MATLAB71\work\test.p is older than M-file C:\ProgramFiles\MATLAB71\work\test.m.

C:\ProgramFiles\MATLAB71\work\test.p may be obsolete and may need to be regenerated.

Type"help pcode" for information about generating P-files from M-files.

会出现一个Warning，警告你所调用的P文件比同名的M文件要旧，即表示M文件已被修改了。尽管如此，调用的还是旧的P文件，即得出a = 1和 b = 2。

P文件可以用来作保密代码之用，如果你给别人一个M文件，别人可以打开来看到你所有的代码和算法。如果你的代码不想被别人看到，那可以给他P文件。

pcode函数也可以应用在M函数文件。

二、生成.exe文件加密

他的缺点是不能够脱离Matlab单独运行，必须还在command window里面调用，可移植性不强。如果生成.exe文件就可以解决这个问题。

生成.exe文件的方法稍微复杂点：

1、首先要保证你的程序是函数型的，而不是脚本形式的。如果是脚本形式的也不要紧，只需要在脚本文件第一行添加 function []=name()即可。

2、然后，在command window里用mcc -m -B    sgl file.m命令生成.exe文件，期间会让你选择编译器（我的是VC6.0），会发现生成了一堆文件。

3、将上步生成的那一堆文件一起拷到待运行的机器。此时仍需matlab所必需的动态连接库。将 <matlab path>/extern/lib/win32/mglinstallar.exe拷贝到到待运行机器上。

4.在待运行的机器上先运行mglinstallar.exe,然后选择解压目录，将在将在指定目录下解压缩出bin和toolbox两个子目录，其中在bin\win32目录下就是数学库和图形库脱离MATLAB运行所需的所有动态连接库，共有n个。可以将这些.dll考入system32,也可以直接放在应用程序目录下（我把它拷在应用程序目录下了）。而toolbox目录则必须与应用程序同一目录。

5.大功告成，双击.exe即可运行了。

总结：两种方法都可以达到保护源代码的目的，但是各有各的优缺点。生成p文件的优点是方法简单，生成文件大小只是源代码的3倍左右，缺点是不能脱离matlab环境运行。生成.exe文件的优点是可以脱离matlab单独运行，缺点是生成的文件大小是原来的几千倍（5555555555，当然包含那些.dll文件，体积最庞大的一部分）。就拿我的程序做个比较，源码是8.34K，生成p文件22.6K，生成可执行文件（含那些.dll）共30多兆。

 生成exe的参考地址;http://wenku.baidu.com/link?url=ketdfiY7WyeYLxud6n9NPGLL_f7KX9dPTZtCevvj0ADTTTUf6GQ_A4KgUH1RxDh39gjHij9XF-6K4OZlgyudpCrYnhu6W0WCn7QwmasbuV3

原文地址：http://zmryyj.blog.163.com/blog/static/98450751201132081451490/

---------------------

作者：小木匠_

来源：CSDN

原文：https://blog.csdn.net/qq_20823641/article/details/51329365

版权声明：本文为博主原创文章，转载请附上博文链接！



