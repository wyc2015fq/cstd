# Shell+VIM 命令及快捷键 - xiahouzuoxin - CSDN博客





2012年10月06日 17:00:06[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：4554








每次使用Linux都会遇到一些新的问题，尤其像我们这样的初学者。

因此这里打算记录每次使用Linux过程中学到的有用的命令，以备不时之需。




## **PART1：Shell命令**

**=================**

**1、**查看磁盘信息及使用容量，此命令不能查看为挂载的磁盘         **df -hT**

     查看所有磁盘信息（包括未挂载的磁盘）  **fdisk -l（root用户下）**

**2、**查看file文件夹下文件占用空间大小     **du** file**-hc --max-depth=1**

--max-depth=1表示只显示当前目录下1级文件及文件夹信息，不指定则会显示当前文件夹及子文件夹所有文件

**3、**使用ls命令查看文件信息    **ls -**[option]

      option=a       查看隐藏文件

      option=l         显示详细信息

      option=h        以KB,MB的方式显示文件大小

      选项可结合使用。


**4、**批量移动文件（find + [-exec]/[-ok] + cmd）  


      eg：f1文件夹下有100多个.c文件，想要移动到f2文件夹下，命令如下，注意最后"\"前有空格

**find** ./f1**-name** "*.c"**-exec** mv {} ./f2 \;

       为了安全起见，可以将-exec替换为-ok，则每一个操作都会提示，尤其在批量删除中尤为注意。

**find** ./f1**-name** "*.c"**-ok** rm {} \;


**5、**mount命令

（1）挂载光盘到/mnt/cdrom目录下 **mount** /dev/cdrom /mnt/cdrom


（2）挂在NFS服务共享目录 **mount -t nfs** 192.168.1.1:/sharedir /mnt

（3）挂载ISO文件 **mount -o loop** ./file.ISO  /mnt/MyIso

**6、**查看操作系统类型   
uname -a 或uname -r


**7、**显示系统进程，杀死进程 


（1）显示当前用户进程 **ps**


（2）显示所有进程 **ps ef**

（3）杀死进程号为m的进程 **kill -9** m

（4）杀死名字为n的进程 **kill -9** n


**8、**计算程序运行时间，在考虑程序效率时很有用**time** program

**9、**操作系统登录账户相关命令

（1）创建账户 **useradd** name


（2）删除账户** userdel** name 只删除用户不删除用户目录


userdel -r name 删除用户和目录

**10、**文件解压

（1）.tar.bz2解压  **tar -xjvf** file.tar.bz2  /home/document 


（2）.tar.gz解压 **tar -xzvf** file.tar.gz /home/document

（3）zip文件 **unzip**file.zip/home/document

**11、**echo命令，输出信息

**echo** file 查看文件内容

**echo** $PATH查看环境变量

**12、**查找库位置或文件位置。locate命令其实是“find -name”的另一种写法，但是要比后者快得多，

原因在于它不搜索具体目录，而是搜索一个数据库（/var/lib/locatedb）。  eg. **locate** libjpeg

**13、**查找命令位置 **whereis** grep

**14、**rpm包的安装/卸载/查询

安装：rpm -ivh [pacakges]

卸载：rpm -e [pacakges]

查询（以key为关键字）：rpm -qa | grep [key]

查询已安装包路径：rpm -ql [pacakge]

**15、**清空终端

（1）reset：终端缓冲清空

（2）clear（Ctrl+L快捷键）：显示清空

**16、**显示文件或目录下的代码总行数  wc -l [filename/dirname]




## **PART2：Shell快捷键**

**=================**


**1、**Ubuntu中打开终端快捷键 
Ctrl+Alt+T


**2、**从终端打开另一个终端 
Ctrl+Shift+T，终端之间切换使用Ctrl+PgDn和Ctrl+PgUp

**3、**打开类似于Windows下的“运行”框 
Alt+F2




## **PART3：vim使用**

**=================**


**1、**跳转到第N行，[N]gg

**2、**跳转到第N列，[N] |

**3、**跳到局部变量的定义处，gd

**4、**跳到全局变量的定义处，gD

**5、**查找单词 

N /[word]用于查找word第N次出现的地方，往前查找，没有N则默认为当前第一个查找到的word

N ?[word]  用于查找word第N次出现的地方，往后查找，没有N则默认为当前第一个查找到的word

/<Enter回车> 继续上次的查找，查找下一个，往后查找

?<Enter回车>继续上次的查找，查找下一个，往前查找

**6、**跳转到指针移动到的上一个位置，CTRL+O

**7、**跳转到指针移动到的下一个位置，CTRL+I

**8、**确定成对的括号（比如（）、[]、{}等），%

**9、**跳转到行的开头，Home，跳转到行的结尾，End

**10、**插入文本（注意大小写）

**i**，在当前光标前面编辑

**I**，在当前光标所在的行的开头开始编辑

**a**，在当前光标后面编辑

**A**，在当前光标所在行的末尾开始编辑

**o**，在当前光标所在行的下面开启一新行编辑

**O**，在当前光标所在行的上面开启一新行编辑

**11、**删除

[**N] x或[N] Del**，从光标处开始（包括光标位置）往后删除N个字符，默认N=1

**[N] X**，从光标处开始（包括光标位置）往前删除N个字符，默认N=1

**[N] dd**，从当前行开始删除N行

**12、**复制粘贴

**[N] yy**，复制N行

**p**，粘贴在光标后

从系统剪切板复制粘贴：**"+y，"+p**

**13、**修改

**[N] .**重复N次前面的操作，默认N=1，注意后面有个句点

**u**，恢复到上一次的更改状态

**14、**在命令行显示当前文件名以及文件总行数，**CTRL+G**

显示当前文件路径，**:pwd**

注：在命令行模式下，可以使用很多可以再Shell中使用的命令，比如cd等。

**15、**显示当前目标处的字符的ASCII码，同时用十进制、十六进制和八进制显示，**ga**

**16、**显示光标位置（行、列），**g CTRL+G**

**17、commond=open/read/wirte**时，分别打开、读和写新的文件，会打开文件对话框：**browse {commond} [file]**

**vim/edit/sp/vsp [file]** 这三个都是直接打开文件编辑的命令，sp和vsp会打开多窗口编辑，不同窗口之间使用**CTRL+W**切换

**18、**vim结合tag的使用

（1）**:set tags+=[tagpath]  ** 设置tags路径，这个也可以写到.vimrc文件中

（2）**vim -t [tagname]  **在vim中打开tagneme所在的文件，如果匹配多个则会弹出选择条

         当不自动弹出选择条时，可以使用**:tselect [tagname]**

（3）**:tag [tagname]**跳转到tagname的第一个匹配处

**19、**VIM选择

（1）**v**：行选择

（2）**Ctrl+v**：可视块，列选择（在Windows下是**Ctrl+p**）






**删除列**1.光标定位到要操作的地方。

2.CTRL+v 进入“可视块”模式，选取这一列操作多少行。

3.d删除。
**插入列**插入操作的话知识稍有区别。例如我们在每一行前都插入"//"注释：

1.光标定位到要操作的地方。

2.CTRL+v 进入“可视 块”模式，选取这一列操作多少行。

3.SHIFT+i(I) 输入要插入的内容。

4.ESC 按两次，会在每行的选定的区域出现插入的内容。






**20、**安装了WinManager插件后，普通模式下使用**wm**可以打开文件管理窗口

**21、**替换——命令行模式下

（1）:%s/[word_a]/[word_b]/g 将文件中的所有word_a替换成word_b

（2）:%s/oldstring/newstring/ 替换所有行中首次出现的oldstring

（3）:%s/oldstring/newstring/g 替换所有行中所有的oldstring

（4）:n,$s/oldstring/newstring/g  替换从第n行开始到最后一行的所有oldstring（$表示最后一行，可以是m,表示第n行到第m行）

（5）:.,$s/oldstring/newstring/g 替换从当前行开始到最后一行的所有oldstring

（6）:s#oldstring/#newstring/# 把oldstring/替换成newstring/，#号用在分隔符，中间的/不会分隔

**21、**多文件编辑（文件缓冲区操作）


使用**e [filename]**编辑新文件

查看文件缓冲区，命令行**:ls**或**:buffers**

接着使用 **:b [buffer number]**跳转到对应的buffer，从而实现多文件的切换

![](https://img-blog.csdn.net/20140125102042625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




