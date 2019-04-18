# wget--Windows10下的安装与使用 - wsp_1138886114的博客 - CSDN博客





2019年01月30日 09:54:46[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：233











### 文章目录
- [一、下载与安装](#_2)
- [二、使用方法wget -P 目录 网址](#wget_P___14)
- [三、详情参数](#_42)




### 一、下载与安装

GNU Wget是一个在网络上进行下载的简单而强大的自由软件，其本身也是GNU计划的一部分。它的名字是“World Wide Web”和“Get”的结合，同时也隐含了软件的主要功能。目前它支持通过HTTP、HTTPS，以及FTP这三个最常见的TCP/IP协议协议下载。
- 下载安装
官网：[http://gnuwin32.sourceforge.net/packages/wget.htm](http://gnuwin32.sourceforge.net/packages/wget.htm)

下载地址：[http://downloads.sourceforge.net/gnuwin32/wget-1.11.4-1-setup.exe](http://downloads.sourceforge.net/gnuwin32/wget-1.11.4-1-setup.exe)

安装，双击一步步安装即可- 系统环境变量配置
新建变量`GNU_HOME`：`GNU_HOME=C:\Program Files (x86)\GnuWin32`(自己的安装地址)

在`Path`变量中添加：`%GNU_HOME%\bin`
### 二、使用方法wget -P 目录 网址

举例来说，如果你要放到/root底下，你可以打下列的指令：

```bash
wget -P /root 网址
wget -P D:\xxx.zip http://www.xdown.com/xxx.zip
wget -O "D:\xxx.zip" http://www.xdown.com/xxx.zip
```

用wget下载东西，它会自动重连并断点续传。让人很放心。

经常要下载一个网站或网站的某个目录。

下载一个目录，[例如下载网站www.example.com/目录mydir下的所有内容](http://xn--www-y28dp2elv0ab78cqqht21b.example.com/%E7%9B%AE%E5%BD%95mydir%E4%B8%8B%E7%9A%84%E6%89%80%E6%9C%89%E5%86%85%E5%AE%B9)
```bash
命令如下：
wget -r -p -k -np -nc -e robots=off http://www.example.com/mydir/
如果要想下载整个网站，最好去除-np参数。   
wget -r -p -k -nc -e robots=off http://www.example.com/mydir/
 
-r 递归；对于HTTP主机，wget首先下载URL指定的文件，然后（如果该文件是一个HTML文档的话）递归下载该文件所引用（超级连接）的所有文件（递 归深度由参数-l指定）。对FTP主机，该参数意味着要下载URL指定的目录中的所有文件，递归方法与HTTP主机类似。   
-c 指定断点续传功能。实际上，wget默认具有断点续传功能，只有当你使用别的ftp工具下载了某一文件的一部分，并希望wget接着完成此工作的时候，才 需要指定此参数。
 
-nc 不下载已经存在的文件
-np 表示不追溯至父目录，不跟随链接，只下载指定目录及子目录里的东西；
-p 下载页面显示所需的所有文件。比如页面中包含了图片，但是图片并不在/yourdir目录中，而在/images目录下，有此参数，图片依然会被正常下 载。
 
-k 修复下载文件中的绝对连接为相对连接，这样方便本地阅读。
-o down.log 记录日记到down.log
-e robots=off 忽略robots.txt
```

### 三、详情参数
- Wget 的使用
1）支持断点下传功能

2）同时支持FTP和HTTP下载方式

3）支持代理服务器

4）设置方便简单

5）程序小，完全免费；
命令格式：  `wget [参数列表] [目标软件、网页的网址]`
- 1、启动类参数

这一类参数主要提供软件的一些基本信息；

-V,–version 显示软件版本号然后退出；

-h,–help显示软件帮助信息；

-e,–execute=COMMAND 执行一个 “.wgetrc”命令
以上每一个功能有长短两个参数，长短功能一样，都可以使用。需要注意的是，这里的-e参数是执行一个.wgettrc的命令，.wgettrc命令其实是一个参数列表，直接将软件需要的参数写在一起就可以了。
- 2、文件处理参数

这类参数定义软件log文件的输出方式等；

-o,–output-file=FILE 将软件输出信息保存到文件；

-a,–append-output=FILE将软件输出信息追加到文件；

-d,–debug显示输出信息；

-q,–quiet 不显示输出信息；

-i,–input-file=FILE 从文件中取得URL；
以上参数对于攻击者比较有用，我们来看看具体使用；

例1：下载192.168.1.168首页并且显示下载信息

wget -dhttp://192.168.1.168
例2：下载192.168.1.168首页并且不显示任何信息

wget -qhttp://192.168.1.168

例3：下载filelist.txt中所包含的链接的所有文件

wget -i filelist.txt

wget -np -m -l5 [http://jpstone.bokee.com](http://jpstone.bokee.com) //不下载本站所链接的其它站点内容，5级目录结构
- 3、下载参数

下载参数定义下载重复次数、**保存文件名**等；

-t,–tries=NUMBER 是否下载次数（0表示无穷次）

-O --output-document=FILE下载文件保存为别的文件名

-nc, --no-clobber 不要覆盖已经存在的文件

-N,–timestamping只下载比本地新的文件

-T,–timeout=SECONDS 设置超时时间

-Y,–proxy=on/off 关闭代理
例：下载192.168.1.168的首页并将下载过程中的的输入信息保存到test.htm文件中

wget -o test.htm [http://192.168.1.168](http://192.168.1.168)
- 4、目录参数

目录参数主要设置下载文件保存目录与原来文件（服务器文件）的目录对应关系；

-nd --no-directories 不建立目录

-x,–force-directories 强制建立目录

可能现在我们对这里的目录还不是很了解，我们来看一个举例
例：下载192.168.1.168的首页，并且保持网站结构

wget -x [http://192.168.1.168](http://192.168.1.168)
- 5、HTTP参数

HTTP参数设置一些与HTTP下载有关的属性；

–http-user=USER设置HTTP用户

–http-passwd=PASS设置HTTP密码

–proxy-user=USER设置代理用户

–proxy-passwd=PASS设置代理密码
以上参数主要设置HTTP和代理的用户、密码；
- 6、递归参数设置

在下载一个网站或者网站的一个目录的时候，我们需要知道的下载的层次，这些参数就可以设置；

-r,–recursive 下载整个网站、目录（小心使用）

-l,–level=NUMBER 下载层次
例：下载整个网站

wget -r [http://192.168.1.168](http://192.168.1.168)
- 7、递归允许与拒绝选项参数

下载一个网站的时候，为了尽量快，有些文件可以选择下载，比如图片和声音，在这里可以设置；

-A,–accept=LIST 可以接受的文件类型

-R,–reject=LIST拒绝接受的文件类型

-D,–domains=LIST可以接受的域名

–exclude-domains=LIST拒绝的域名

-L,–relative 下载关联链接

–follow-ftp 只下载FTP链接

-H,–span-hosts 可以下载外面的主机

-I,–include-directories=LIST允许的目录

-X,–exclude-directories=LIST 拒绝的目录
如何设定wget所使用的代理服务器

wget可以使用用户设置文件".wgetrc"来读取很多设置，我们这里主要利用这个文件来是

设置代理服务器。使用者用什么用户登录，那么什么用户主目录下的".wgetrc"文件就起

作用。例如，“root"用户如果想使用”.wgetrc"来设置代理服务器，"/root/.wgetrc"就起

作用，下面给出一个".wgetrc"文件的内容，读者可以参照这个例子来编写自己的"wgetrc"文件：

http-proxy = 111.111.111.111:8080

ftp-proxy = 111.111.111.111:8080

这两行的含义是，代理服务器IP地址为：111.111.111.111，端口号为：80。第一行指定

HTTP协议所使用的代理服务器，第二行指定FTP协议所使用的代理服务器。












