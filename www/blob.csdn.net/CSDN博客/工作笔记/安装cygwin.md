# 安装cygwin - 工作笔记 - CSDN博客





2012年01月17日 13:44:15[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5400标签：[internet																[gcc																[服务器																[浏览器																[编译器																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)




安装 GCC 最快捷的方式是在您的机器上安装完整的 Cygwin 环境。Cygwin 是 Windows 上类似于 Linux 的环境。它包括一个提供 UNIX 功能性基本子集的 DLL 以及在这之上的一组工具。安装好 Cygwin 之后，通常可以忽略它，即使您是命令行的爱好者，您仍能发现您活得更舒坦了。

确保您具有相当快速的 Internet 连接。GCC 编译器及相关实用程序大约有 50 兆字节。另外确认您的连接是否需要一个代理服务器。这可通过您的 Internet 浏览器选项：
**工具 -> Internet 选项 ->连接 ->局域网（LAN）设置**来确定。 

确保在您的管理员帐户下安装这个程序，并选择为“所有用户”安装。

将您的 Internet 浏览器指到 
www.cygwin.com
。在屏幕的中央是一个黑绿两色组成的 “C” 徽标，标题是“Install or update now.”。在该链接上点击，这将下载一个小程序 setup.exe（250KB）。这个安装程序将在您的机器上引导 Cygwin 环境的安装或更新过程。

**运行安装程序**

将 setup.exe 下载到一个已知地方，如桌面，然后运行它。

它将打开安装向导，如下图所示：
**图 1. Cygwin 环境的安装向导**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure01.gif)


点击 **Next**按钮。 
**选择下载位置**

在这个页面上，选择"Install from Internet"。
**图 2. 选择安装类型**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure02.gif)


点击 **Next**按钮。 
**选择一个安装目录**

在这个页面上，选择执行 Cygwin 所在的目录。

设置图3所示的剩余选项。
**图 3.选择一个安装目录**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure03.gif)


点击 **Next**按钮。 
**选择一个临时安装目录**

在这个页面（图4），为安装包选择一个临时目录。当 Cygwin 安装完成后，您可以删除这个目录和它的内容。

注意安装程序记住了这个目录，并在更新时只下载比这个目录中的包更新的版本。如果您删除这个目录并重新安装， **setup**将重新安装您选择的所有内容而不管这些包的安装程序在运行目录中已经存在。

**图 4.选择一个临时安装目录**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure04.gif)


点击 **Next**按钮。 
**指定您的 Internet 连接**

在这个页面上（图5），指定 Internet 连接。如果你有代理服务器，选择“使用 IE5 设置”，或从您的浏览器选项获得服务器名，选择使用代理，然后输入代理服务器的主机名和端口号。
**图 5. 选择连接类型**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure05.gif)


点击 **Next**按钮。 
**选择下载站点**

在这个页面上（图6），选择一个下载站点。因为这些站点是义务的镜像，所以选择一个下载点有点风险（通常它们是可用的，但有时不可用）。通常，试试离你比较近的那个站点。
**图6. 选择一个下载站点**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure06.gif)


点击 **Next**按钮。 
**选择包进行安装**

在这个页面上，您将选择要安装的包。默认情况下，Cygwin 基本包将不安装 GCC，因此，您必须修改默认设置。

将鼠标移动到 **Devel**边的加号（+）上，然后点击它展开 **Devel**类。 
**图 7. 选择包进行安装**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure07.gif)
**在你还没确定哪些需要哪些不需要时，最好全部下载，点击All后的default为install。即是完全安装了。保证了GCC编辑器的使用。**

您会看到在 Devel 下有许多入口，其中只有三个是您需要的，次序如下：
binutils gcc gcc-mingw 

不是所有默认的基本包都需要运行 GCC，但它们不太大，因此为简单化，就安装它们。

注意很容易选错包或版本，尤其在重新安装的时候，因为选择窗口中的循环按钮是不标准的。在这一步我们建议的就是多加小心，尤其是有多个 GCC 版本的情况下。

为选择这三个包，点击列 **New**中每个双向箭头直到显示每个包的版本号。根据您机器上已安装的以及镜像上所拥有的合适的版本，列入口在各图标间循环：

![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure08.gif)


如果有多个版本可用，选择数字最大的一个。要下载二进制文件，确保选中了“Bin”列的复选框。
**图 9. 下载二进制文件**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure09.gif)

**完成下载**

选择完三个包之后，Cygwin 就访问选择的镜像主机，然后下载那三个包以及默认的基本包。根据您的连接速度这将持续几分钟，在安装期间，将打开和关闭许多弹出窗口。

因为存放 Cygwin 安装的镜像站点是义务的，有时不可用或负荷过重。如果发现选择的镜像没有响应，就按取消 （ **Cancel**），这将终止整个安装程序。


惟一的办法就是重新开始安装并选择另一个镜像；这非常让人感到沮丧。幸运的是，安装程序记住了大多数您上次所做的，因此通常取默认情况。在重新选择正确的包时必须小心，因为在后面的安装中不帮你选择这些内容。
**setup**程序然后询问是否在您的桌面或开始菜单创建一个 Cygwin 快捷键。该快捷键启动 Cygwin shell (称为 bash), 这对我们计划中的 DB2 使用并不是必须的，但对测试我们的安装是有用的。因此让
**setup**至少创建一个桌面快捷键。我们将在安装的后面使用它。 

最后，点击 **Finish**按钮。我们将在 
后面
测试安装结果。 
**图 10. 完成安装**
![](http://www-128.ibm.com/developerworks/cn/db2/library/techarticles/0306haungs/images/Figure10.gif)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=gcc&t=blog)](https://so.csdn.net/so/search/s.do?q=internet&t=blog)




