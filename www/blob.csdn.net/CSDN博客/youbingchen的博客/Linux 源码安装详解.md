# Linux 源码安装详解 - youbingchen的博客 - CSDN博客





2016年06月07日 18:39:00[youbingchen](https://me.csdn.net/youbingchen)阅读数：456标签：[源码																[linux																[gnu](https://so.csdn.net/so/search/s.do?q=gnu&t=blog)
个人分类：[linux](https://blog.csdn.net/youbingchen/article/category/6260822)




`./configure --help`
主要分为5个类别  

+ 路径相关 

+ 编译相关 

+ 依赖软件相关 

+ 模块相关 

+ 其他
# /configure,make,make install的作用

这些都是典型的使用GNU的AUTOCONF和AUTOMAKE产生的程序的安装步骤。./configure是用来检测你的安装平台的目标特征的。比如它会检测你是不是有CC或GCC，并不是需要CC或GCC，它是个shell脚本。make是用来编译的，它从Makefile中读取指令，然后编译。make install是用来安装的，它也从Makefile中读取指令，安装到指定的位置。

AUTOMAKE和AUTOCONF是非常有用的用来发布C程序的东西

> 
make的原理是执行一个叫Makefile文件里的指令，make的基本用处是自动根据makefile里的指令来编译源文件。它还可以用来做比如安装软件，卸载软件等事情，但前提是作者在makefile里写了。比如makefile里有这么些内容： 

  一些编译好的步骤 

  … 

  install :  < commands >
然后用make install的话，make程序就会按照上面install：后面的指令< commands >执行安装，uninstall也是一样的道理，大部分的作者会写有卸载的部分，这时只要简单地执行make unistall就可以，如果作者懒没有写，那就只有根据make install中的步骤，看它把什么文件拷到哪去了，然后分别手动删除。 

还有关键的一点是，编译安装完成后，不要删除源代码，不然就算作者写了unnistall目标，你也没有makefile可以执行了。

－－－－－－－－扩展－－－－－－－－－－－－

/configure,make,make install的作用

这些都是典型的使用GNU的AUTOCONF和AUTOMAKE产生的程序的安装步骤。

./configure是用来检测你的安装平台的目标特征的。比如它会检测你是不是有CC或GCC，并不是需要CC或GCC，它是个shell脚本。 

make是用来编译的，它从Makefile中读取指令，然后编译。 

make install是用来安装的，它也从Makefile中读取指令，安装到指定的位置。
AUTOMAKE和AUTOCONF是非常有用的用来发布C程序的东西。

> 
1、configure，这一步一般用来生成 Makefile，为下一步的编译做准备，你可以通过在 configure 后加上参数来对安装进行控制，比如代码:./configure –prefix=/usr 意思是将该软件安装在 /usr 下面，执行文件就会安装在 /usr/bin （而不是默认的 /usr/local/bin),资源文件就会安装在/usr/share（而不是默认的/usr/local/share）。同时一些软件的配置文件你可以通过指定–sys-config=参数进行设定。有一些软件还可以加上 –with、–enable、–without、–disable 等等参数对编译加以控制，你可以通过允许 ./configure –help 察看详细的说明帮助。 

  2、make，这一步就是编译，大多数的源代码包都经过这一步进行编译（当然有些perl或python编写的软件需要调用perl或python来进行编译）。如果 在 make 过程中出现 error ，你就要记下错误代码（注意不仅仅是最后一行），然后你可以向开发者提交 bugreport（一般在 INSTALL里有提交地址），或者你的系统少了一些依赖库等，这些需要自己仔细研究错误代码。 

  3、make insatll，这条命令来进行安装（当然有些软件需要先运行 make check 或 make test 来进行一些测试），这一步一般需要你有 root 权限（因为要向系统写入文件）。
Linux的用户可能知道，在Linux下安装一个应用程序时，一般先运行脚本configure，然后用make来编译源程序，在运行make install，最后运行makeclean删除一些临时文件。使用上述三个自动工具，就可以生成configure脚本。运行configure脚本，就可以生成Makefile文件，然后就可以运行make、make install和make clean。

configure是一个shell脚本，它可以自动设定源程序以符合各种不同平台上Unix系统的特性，并且根据系统叁数及环境产生合适的Makefile文件或是C的头文件(header file)，让源程序可以很方便地在这些不同的平台上被编译连接。](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=源码&t=blog)




