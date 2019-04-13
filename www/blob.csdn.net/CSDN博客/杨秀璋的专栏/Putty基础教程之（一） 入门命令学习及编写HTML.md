
# Putty基础教程之（一）.入门命令学习及编写HTML - 杨秀璋的专栏 - CSDN博客

2016年10月08日 03:09:25[Eastmount](https://me.csdn.net/Eastmount)阅读数：34664



这篇文章主要介绍Putty的基础用法，同时通过Putty来编辑HTML文件。一方面是自己最近学习的在线笔记，另一方面希望文章对你有所帮助。Putty是Windows下操作Linux命令的小工具，也是一个跨平台的远程登陆工具，非常好用。
常见命令包括：

```python
cd 文件夹名 ：查看路径
dir ：查看当前路径下的所有文件
ls ：列出当前文件夹下所有内容
pwd ：显示当前路径
clear ： 清空终端屏幕命令，Ctrl+L清除屏幕
mkdir [dir] ： 新建文件夹
mv *** **** ：移动或者重命名
rm *** *** ：删除一次可以删除多个
cp ：拷贝
touch 文件名 ： 新建文件
vi 文件名 ： 编辑文件命令
cat 文件名 ： 查看文件内容
wget （路径）：下载
unzip ***.zip ：压缩文件到当前目录
ls -ltr  |  cat  -vte ： 显示文件夹下所有内容详细信息
```


## 0. 简介
PuTTY是一个Telnet、SSH、rlogin、纯TCP以及串行接口连接软件。PuTTY为一开放源代码软件，主要由Simon Tatham维护，使用MIT licence授权。
随着Linux在服务器端应用的普及，Linux系统管理越来越依赖于远程。在各种远程登录工具中，Putty是出色的工具之一。它是一个免费的、Windows x86平台下的Telnet、SSH和rlogin客户端，但是功能丝毫不逊色于商业的Telnet类工具。
下载地址：[http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html)
Here are the PuTTY files themselves:
PuTTY (the SSH and Telnet client itself)
PSCP (an SCP client, i.e. command-line secure file copy)
PSFTP (an SFTP client, i.e. general file transfer sessions much like FTP)
PuTTYtel (a Telnet-only client)
Plink (a command-line interface to the PuTTY back ends)
Pageant (an SSH authentication agent for PuTTY, PSCP, PSFTP, and Plink)
PuTTYgen (an RSA and DSA key generation utility).

## 1. 运行Putty
PuTTY 是一个准绿色软件，说它绿色是因为直接就能使用，完全没有任何的安装程序。
运行界面如下所示：
![](https://img-blog.csdn.net/20161008011749496)
这里作者直接登陆自己的远程服务器，如下图所示，输入Host Name，选择SSH点击Open即可。密码：gufe@2017
![](https://img-blog.csdn.net/20161008011943621)
点击“Open”按钮后，进入下面这个界面，同时输入密码（不可见）即可登录。
![](https://img-blog.csdn.net/20161008012120795)


## 2. 查看编码方式
**命令：echo $LANG $LANGUAGE**
通过命令可以查看编码方式，显示如下所示，通常是UTF-8编码（支持中文）。
补充：在linux中，\#表示管理员，$表示普通用户，$开头表示变量，echo命令功能是在显示器上显示一段文字，一般起到一个提示的作用。
![](https://img-blog.csdn.net/20161008012625155)
修改编码方式的操作如下图所示：
![](https://img-blog.csdn.net/20161008012656327)

## 3. Clear 清屏
**命令：clear**
也可以使用Ctrl+L清除屏幕，这是在编写Linux常用的一个命令。
![](https://img-blog.csdn.net/20161008013840238)


## 4.dir ls 查看文件夹下内容
**命令：dir**
如下图所示，显示当前登录xzyang用户的目录下，存在一个pub文件夹；然后执行cd pub去到该目录，病调用dir显示没有任何内容，再输入mkdir yxz创建一个yxz的文件夹。
注意：~账户的home目录，cd ~表示返回home目录。
![](https://img-blog.csdn.net/20161008014357872)
**命令：ls**
Listing Files/SubFolders（ls）
$ls 列出当前文件夹下所有内容
$ls -o 列出当前文件夹中所有内容，含详细信息，但不列出group
$ls -l 同上，含group信息
$ls -a 列出当前文件夹中所有内容，包含以”.”开头的文件
$ls -t 按更改时间排序
$ls -v 按版本先后排序

![](https://img-blog.csdn.net/20161008014810783)
这里如果想显示文件夹下所有内容详细信息，使用如下命令。
**命令：ls -ltr  |  cat  -vte**
![](https://img-blog.csdn.net/20161008014955535)

## 5. cd 查看路径 | pwd 显示当前路径
**命令：cd [dir] 进入文件夹**
Change Directory ，"cd yxz"表示进入yxz文件夹，"cd ~"表示返回home根目录，"cd .."表示返回上一层目录（类似DOS）。
更改目录 Change Directory (cd)：cd /path/to/directory/，有DOS基础的同学应该知道，路径可以用相对也可以用绝对。
**命令：pwd**
注意：pwd不是password，而是查看当前目录，会输出完整的路径。
![](https://img-blog.csdn.net/20161008015804185)


## 6. mkdir 新建文件夹
**命令：mkdir 文件夹**
创建文件夹yxz，原来dir没有显示，在执行"mkdir yxz"后显示新建文件夹yxz。
![](https://img-blog.csdn.net/20161008014357872)


## 7. touch 创建文件
**命令：touch yxz.html**
创建yxz.html文件，如果touch了已有的文件，则会更新那个文件的日期为当前。
![](https://img-blog.csdn.net/20161008020640385)


## 8. vi 编辑HTML代码
**命令： vi  文件名**
注意：
(1) 输入"vi yxz.html"后，用vi打开文件后，是处于「命令行模式（command mode）」，您要切换到「插入模式（Insert mode）」才能够输入文字。切换方法：在「命令行模式（command mode）」下按一下字母「i」就可以进入「插入模式（Insert mode）」，这时候你就可以开始输入文字了。
(2) 编辑好后，需从插入模式切换为命令行模式才能对文件进行保存，切换方法：按「ESC」键。
(3) 保存并退出文件：在命令模式下输入:wq即可！（别忘了wq前面的:）
参考：[http://blog.csdn.net/bruce0532/article/details/7842384](http://blog.csdn.net/bruce0532/article/details/7842384)
![](https://img-blog.csdn.net/20161008021509348)
代码如下：
```python
<html>
<head>
      <title>Putty HTML</title>
</head>
<body>
      <div align="center">
          <h1>杨秀璋个人简历</h1>
          <hr width="80%" color="red" size="2">
          <ul>
              <li>高中：贵州师大附中</li>
              <li>本科：北京理工大学-软件学院-软件工程</li>
              <li>硕士：北京理工大学-软件学院-web数据挖掘</li>
          </ul>
      </div>
</body>
</html>
~
~
~
~
~
~
~
-- INSERT --                                                  11,72-55
```
VI编辑过程如下所示：

![](https://img-blog.csdn.net/20161008022636302)
HTML显示如下所示：[http://parlab.gufe.edu.cn/~xzyang/yxz.html](http://parlab.gufe.edu.cn/~xzyang/yxz.html)
![](https://img-blog.csdn.net/20161008022804152)
基础知识介绍--参考：[Putty文件查看/编辑命令](http://hi.baidu.com/dmgygo/item/453ee7431e7ee932fb89606f)
vi 是”Visual Interface” 的简称，它在Linux 上的地位就仿佛Edit 程序在DOS上一样。它可以执行输出、删除、查找、替换、块操作等众多文本操作，而且用户可以根据自己的需要对其进行定制，这是其他编辑程序所没有的。
vi不是一个排版程序，它不像Word 或WPS 那样可以对字体、格式、段落等其他属性进行编排，它只是一个文本编辑程序。
vi 没有菜单，只有命令，且命令繁多。限于篇幅，本文只介绍常用的命令。vi 有三种基本工作模式：命令行模式，文本输入模式和末行模式。
（1）命令行模式：
任何时候，不管用户处于何种模式，只要按一下“ESC” 键，即可使vi 进入命令行模式；当在shell 环境下输入vi 命令启动vi 编辑器时，也是处于该模式下。
在该模式下 ，用户可以输入各种合法的vi 命令，用于管理自己的文档。此时从键盘上输入的任何字符都被当作编辑命令来解释，若输入的字符是合法的vi 命令，则vi 在接受用户命令之后完成相应的动作（但需注意的是，所输入的命令并不在屏幕上显示出来）。若输入的字符不是vi 的合法命令，vi 会响铃报警。
（2）文本输入模式：
在命令模式下输入插入命令 i、 附加命令a 、打开命令 o、 修改命令c 、取代命令r 或替换命令s 都可以进入文本输入模式。在该模式下，用户输入的任何字符都被vi 当做文件内容保存起来，并将其显示在屏幕上。在文本输入过程中，若想回到命令模式下，按“ESC” 键即可。
（3）末行模式：
在命令模式下用户按“:” 键即可进入末行模式下，此时Vi 会在显示窗口的最后一行（通常也是屏幕的最后一行）显示一个“:” 作为末行模式的提示符，等待用户输入命令。多数文件管理命令都是在此模式下执行的（如把编辑缓冲区的内容写到文件中等）末行命令执行完后，vi 自动回到命令模式。
若在末行模式下输入命令过程中改变了主意， 可按“ESC” 键或用退格键将输入的命令全部删除之后，再按一下退格键，即可使vi 回到命令模式下。


## 9. cat 显示文件源代码
**命令：cat yxz.html**
cat 命令的原含义为连接（concatenate）， 用于连接多个文件内容并输出到标准输出流中（标准输出流默认为屏幕）。实际运用过程中，我们常使用它来显示文件内容。如果您熟悉MS-DOS 下的type 命令，相信不难掌握cat 命令。
该命令的常用示例如下：
cat file1.txt 显示 file1.txt 文件的内容。
cat file1.txt file2.txt 显示 file1.txt 和file2.txt 文件的内容。
cat -n file1.txt 显示 file1.txt 文件的内容同时显示行号。
![](https://img-blog.csdn.net/20161008022945058)


## 10. mv 移动文件/重命名
**命令：mv yxz.html index.html**
重命名yxz.html为index.html，如果加路径,就是移动到某个路径并重命名为index.html。
linux下重命名文件或文件夹使用mv既可实现。
① 将一个名为abc.txt的文件重命名为1234.txt
[root@station90 root]\#mv abc.txt 1234.txt
② 将目录A重命名为B
[root@station90 root]\#mv A B
③ 将a.txt移动到/b下，并重命名为c.txt
[root@station90 root]\#mv a.txt /b/c.txt

![](https://img-blog.csdn.net/20161008023125888)


## 11. rm 删除文件
**命令：rm 文件名**
删除文件 Remove files (rm) ，少用该命令。
rm -vf miao.in：强制删除miao.in这个文件并不需确认,列出删除文件列表。
rm -rf xxx：删除xxx这个文件夹,包含它的子文件和子文件夹。

![](https://img-blog.csdn.net/20161008023710233)


## 12. df du 文件系统命令
**命令：df**
df：显示整个主机的总容量以及比率。
**命令：du**
du：列表文件/子目录使用率 (du)
du -sh
-s就是summary,只输出当前文件夹总容量
-h一般在Linux就是human给人看的意思,会把1048580b转换为1mb显示
du -h --max-depth=1
只列出当前文件夹和第一级子目录占用大小

![](https://img-blog.csdn.net/20161008025059879)

## 13. help 帮助命令
**命令：wget --help**
任何命令后面带一个 –help 都会显示一个简洁版的帮助,记不住参数就查一下。
![](https://img-blog.csdn.net/20161008025312413)

## 14. wget 获取远程文件
**命令：wget http://xxx/me.jpg**
获取远程的me.jpg文件到当前目录,一般搬家的时候用这个,让服务器去获取另一个服务器的数据,速度很快。
参考：[常用putty命令汇总 - putty中文网](http://www.putty.ws/putty-minglingdaquan)
例子：wget https://www.baidu.com/img/bd_logo1.png

![](https://img-blog.csdn.net/20161008030324136)
注意：在编写过程中，你可能会遇到一个问题，命令没有写完就回车了，怎么解决呢？按Ctrl+C您现进入cat命令，Ctrl+C中断执行信号。
![](https://img-blog.csdn.net/20161008031324937)

总之，希望文章对你有所帮助，尤其是刚入门学习Putty基础知识的学生，这篇文章主要记录了Putty的基础命令，包括如何新建HTML文件并进行编辑，确实是一个非常好用的操作Linux的工具。国庆放假结束了，又是一个在办公室的深夜学习写文章，但我很享受这样的时光。晚安，贵财！
（By：Eastmount 2016-10-08 半夜3点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


