# Ubuntu系统常用操作命令 - 心纯净，行致远 - CSDN博客





2019年04月08日 15:50:28[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：20








1、命令名 --help ：用于获取某个命令的用法帮助。

![](https://upload-images.jianshu.io/upload_images/6857741-a0b42f64570851d0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/603)

2、man 1 命令名：获取某命令的使用手册帮助。（和help作用相同）

![](https://upload-images.jianshu.io/upload_images/6857741-17e4fb702d862afc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/625)

3、ls：查看文件信息

a、ls -a：显示指定目录下的所有子目录和文件（包括隐藏文件）；

![](https://upload-images.jianshu.io/upload_images/6857741-a163ed492444d6dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/614)

b、ls -l：以列表的方式显示指定目录下的所有子目录和文件的详细信息；

![](https://upload-images.jianshu.io/upload_images/6857741-857c7bb2ef4fe5e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/598)

![](https://upload-images.jianshu.io/upload_images/6857741-09d037eb9a9a3f82.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/555)

c、ls -h：配合-l以人性化的方式显示文件大小

![](https://upload-images.jianshu.io/upload_images/6857741-c35e28cb7e29a5f1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/570)

4、tab键补全命令：有些命令忘记全名了可以只输入前几个字符然后按tab键自动补全。比如ls只输个l，然后按tab键。

![](https://upload-images.jianshu.io/upload_images/6857741-690bb776f577d232.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/373)

5、history：查看历史记录

![](https://upload-images.jianshu.io/upload_images/6857741-0fe721dc5cd0f8c2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/546)

6、> :重定向命令，将命令的结果重定向到指定文件

![](https://upload-images.jianshu.io/upload_images/6857741-bddd5cd349282f39.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/545)

7、more：分屏显示，查看内容时，在信息过长无法在一屏上显示时，会出现快速滚屏，使得用户无法看清文件的内容，此时可以使用more命令，每次只显示一页，按下空格键可以显示下一页，按下q键退出显示，按下h键可以获取帮助。

![](https://upload-images.jianshu.io/upload_images/6857741-8220989d8a8828b7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/448)

![](https://upload-images.jianshu.io/upload_images/6857741-cfbd6d14572eba3c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/628)

![](https://upload-images.jianshu.io/upload_images/6857741-92163291a8d9cfa2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/481)

8、|：管道命令，前一个命令的输出作为后一个命令的输入，管道我们可以理解现实生活中的管子，管子的一头塞东西进去，另一头取出来，这里“ | ”的左右分为两端，左端塞东西(写)，右端取东西(读)。

![](https://upload-images.jianshu.io/upload_images/6857741-b048f4d34e420aee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/450)

显示出所有的进程，并通过管道搜索出包含ssh字串的进程

9、clear：清屏命令。（没有实现真正意义的清屏，相当于往下翻了一个空白页）

10、cd：切换目录（绝对路径和相对路径）

![](https://upload-images.jianshu.io/upload_images/6857741-ffdb901a250c53f4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/580)

相对路径

![](https://upload-images.jianshu.io/upload_images/6857741-26f8559ee996dd14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/591)

![](https://upload-images.jianshu.io/upload_images/6857741-9bcd124ae6aa7dc9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/579)

../切换到上层目录；./切换到当前目录；cd ~切换到主目录；cd -切换到上次进入的路径；/切换到根目录。

![](https://upload-images.jianshu.io/upload_images/6857741-8d01a30da645a76d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/455)

绝对路径

11、pwd：显示当前路径

![](https://upload-images.jianshu.io/upload_images/6857741-16218fe930af3e84.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/458)

12、mkdir：创建一个新的目录。参数-p可递归创建目录。需要注意的是新建目录的名称不能与当前目录中已有的目录或文件同名，并且目录创建者必须对当前目录具有写权限。

![](https://upload-images.jianshu.io/upload_images/6857741-7c64254c6b1a1dda.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/434)

单纯的mkdir命令只能创建一个新的目录，不能递归地创建目录

![](https://upload-images.jianshu.io/upload_images/6857741-40ab883b83c9d179.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/519)

最后加上参数-p可以递归地创建目录

13、rmdir：删除一个目录。必须离开目录，并且目录必须为空目录，不然提示删除失败。

![](https://upload-images.jianshu.io/upload_images/6857741-edbc447c091b9297.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/573)

14、rm：删除文件或目录。使用rm命令要小心，因为文件删除后不能恢复。为了防止文件误删，可以在rm后使用-i参数以逐个确认要删除的文件。

-r：递归删除目录

![](https://upload-images.jianshu.io/upload_images/6857741-fd8ac0c4d015dca3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/560)

-i：以交互式方式执行，提示用户是否进行指定的删除操作

![](https://upload-images.jianshu.io/upload_images/6857741-d4c153dc6fe552f6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/563)

-f：强制删除，忽略不存在的文件，无需提示

![](https://upload-images.jianshu.io/upload_images/6857741-598d879c6150aecc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/521)

当前目录下并不存在a,使用参数-f强制删除a也不会报错

![](https://upload-images.jianshu.io/upload_images/6857741-3d59d5c190241039.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/561)

15、ln：建立链接文件，链接文件类似于Windows下的快捷方式。链接文件分为软链接和硬链接。

软链接：软链接不占用磁盘空间，源文件删除则软链接失效。ln -s 源文件 链接文件

硬链接：硬链接只能链接普通文件，不能链接目录。两个文件占用相同大小的硬盘空间，即使删除了源文件，链接文件还是存在，  ln 源文件 链接文件

![](https://upload-images.jianshu.io/upload_images/6857741-210f5c34e6645f46.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/590)

注意：如果软链接文件和源文件不在同一个目录，源文件要使用绝对路径，不能使用相对路径。

16、cat：查看文件内容

![](https://upload-images.jianshu.io/upload_images/6857741-7ec6055e3da65725.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/600)

17、grep:文本搜索，grep允许对文本文件进行模式查找。如果找到匹配模式， grep打印包含模式的所有行。grep一般格式为：grep [-选项] ‘搜索内容串’文件名

在grep命令中输入字符串参数时，最好引号或双引号括起来。例如：grep‘a ’1.txt

-v：显示不包含匹配文本的所有行（相当于取反）

![](https://upload-images.jianshu.io/upload_images/6857741-3d74299231bee9d0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/474)

-n：显示匹配行及行号

![](https://upload-images.jianshu.io/upload_images/6857741-a28f2eb953e17e7f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/436)

-i：不区分大小写

![](https://upload-images.jianshu.io/upload_images/6857741-1dc04524de8867a0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/541)

![](https://upload-images.jianshu.io/upload_images/6857741-a907d503f78ee748.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/765)

18、find：查找文件，通常用来在特定的目录下搜索符合条件的文件，也可以用来搜索特定用户属主的文件。

![](https://upload-images.jianshu.io/upload_images/6857741-857e38dd2e66e892.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/778)

![](https://upload-images.jianshu.io/upload_images/6857741-56906cf1b281b50f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/500)

19、cp：拷贝文件，将给出的文件或目录复制到另一个文件或目录中，相当于DOS下的copy命令

![](https://upload-images.jianshu.io/upload_images/6857741-d53079c47bc7d5dd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/781)

![](https://upload-images.jianshu.io/upload_images/6857741-6d7b4a5e892946cf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/567)

20、mv：移动文件，使用mv命令来移动文件或目录，也可以给文件或目录重命名。（此处的重命名值得是复制一份内容相同名字不同的文件出来）

![](https://upload-images.jianshu.io/upload_images/6857741-44e36ddb30cb9ec8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/776)

![](https://upload-images.jianshu.io/upload_images/6857741-f57165725c8137e3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/518)

21、tar：归档管理，计算机中的数据经常需要备份，tar是Unix/Linux中最常用的备份工具，此命令可以把一系列文件归档到一个大文件中，也可以把档案文件解开以恢复数据。

tar使用格式 tar [参数] 打包文件名 文件

tar命令很特殊，其参数前面可以使用“-”，也可以不使用。

![](https://upload-images.jianshu.io/upload_images/6857741-660b3de3fff7612d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/691)

22、ps：查看进程信息

![](https://upload-images.jianshu.io/upload_images/6857741-e1f4fcdff9463684.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/654)

![](https://upload-images.jianshu.io/upload_images/6857741-fba12eaec9f1defa.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/564)

![](https://upload-images.jianshu.io/upload_images/6857741-42d9d5e061ff907f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/685)

23、top：动态显示进程

![](https://upload-images.jianshu.io/upload_images/6857741-6e802a443d1b58e9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/623)

会不断变化

24、kill：终止进程

![](https://upload-images.jianshu.io/upload_images/6857741-83c1f1beab4e1ab8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/560)

25、ifconfig：查看网卡信息

![](https://upload-images.jianshu.io/upload_images/6857741-cfb03772aa6f5634.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/574)

26、ping：测试远程主机连通性

27、whoami：查看当前用户

![](https://upload-images.jianshu.io/upload_images/6857741-9efd51b2dbcdb654.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/537)

who：查看所有登录系统的用户

![](https://upload-images.jianshu.io/upload_images/6857741-f463d43fbf6b429f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/505)

28、exit：如果是图形界面，退出当前终端；如果是使用ssh远程登录，退出登陆账户；

如果是切换后的登陆用户，退出则返回上一个登陆账号。

29、useradd：添加用户账号

Linux每个用户都要有一个主目录，主目录就是第一次登陆系统，用户的默认当前目录(/home/用户)；每一个用户必须有一个主目录，所以用useradd创建用户的时候，一定给用户指定一个主目录；用户的主目录一般要放到根目录的home目录下，用户的主目录和用户名是相同的；如果创建用户的时候，不指定组名，那么系统会自动创建一个和用户名一样的组名。

![](https://upload-images.jianshu.io/upload_images/6857741-29fbe62b098ad3e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/765)

![](https://upload-images.jianshu.io/upload_images/6857741-8c612f38d193cc61.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/591)

![](https://upload-images.jianshu.io/upload_images/6857741-d85178990d2d7c25.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/592)

30、which:查看命令所在

![](https://upload-images.jianshu.io/upload_images/6857741-69b6b89c0763c9cd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/442)

31、whoami：查看当前系统当前账号的用户名。可通过cat /etc/passwd查看系统用户信息。

![](https://upload-images.jianshu.io/upload_images/6857741-67adc46b2f1265e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/413)

32、who：用于查看当前所有登录系统的用户信息

![](https://upload-images.jianshu.io/upload_images/6857741-ad3d780a48844911.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/442)

![](https://upload-images.jianshu.io/upload_images/6857741-f28092de355a53d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/554)

33、exit：退出当前用户

如果是图形界面，退出当前终端；

如果是使用ssh远程登录，退出登陆账户；

如果是切换后的登陆用户，退出则返回上一个登陆账号

34、useradd：添加新用户。adduser或useradd命令，因为adduser命令是指向useradd命令的一个链接，因此，这两个命令的使用格式完全一样。

![](https://upload-images.jianshu.io/upload_images/6857741-cd3b68b8f56be01e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/686)

![](https://upload-images.jianshu.io/upload_images/6857741-c7c53267d4911e44.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/427)

![](https://upload-images.jianshu.io/upload_images/6857741-24cc57adf8a6f382.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/461)

![](https://upload-images.jianshu.io/upload_images/6857741-1fbcaa58441cb49c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/557)

35、passwd:设置用户密码

![](https://upload-images.jianshu.io/upload_images/6857741-3748e64a6ade1840.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/590)

36、userdel：删除用户

![](https://upload-images.jianshu.io/upload_images/6857741-1886ddb97d5c3363.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/665)

![](https://upload-images.jianshu.io/upload_images/6857741-dcba42ee8d647221.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/554)

37、su:切换用户（可以通过su命令切换用户，su后面可以加“-”。su和su –命令不同之处在于，su -切换到对应的用户时会将当前的工作目录自动转换到切换后的用户主目录）

![](https://upload-images.jianshu.io/upload_images/6857741-f8711771ffe17102.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/660)

38、groupadd、groupdel：添加、删除组账号 （cat /etc/group查看用户组）

![](https://upload-images.jianshu.io/upload_images/6857741-ce6fe6dc0b471f85.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/418)

![](https://upload-images.jianshu.io/upload_images/6857741-be4f2d127d497a68.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/341)

![](https://upload-images.jianshu.io/upload_images/6857741-476c3564153101ae.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/457)

![](https://upload-images.jianshu.io/upload_images/6857741-8bfd925a52bbf2e2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/457)

39、usermod:修改用户所在组（使用方法：usermod -g 用户组 用户名）

-g用来制定这个用户默认的用户组；-G一般配合'-a'来完成向其它组添加

![](https://upload-images.jianshu.io/upload_images/6857741-4c036cdd6ecb9453.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/569)

![](https://upload-images.jianshu.io/upload_images/6857741-4088000d6de81b7b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/442)

![](https://upload-images.jianshu.io/upload_images/6857741-d219b1aeeff4cbe8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/553)

40、groups：查看用户在哪些组（groups 用户名）



41、为普通用户添加sudo权限：新创建的用户，默认不能sudo，需要进行一下操作

sudo usermod -a -G **adm**用户名；       sudo usermod -a -G **sudo**用户名

![](https://upload-images.jianshu.io/upload_images/6857741-40543aebf8eb25fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/554)

42、查看有哪些用户组。（方法一:cat /etc/group方法二:groupmod +三次tab键）

43、chmod:修改文件权限。

![](https://upload-images.jianshu.io/upload_images/6857741-9b8e6229360c1378.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/554)

![](https://upload-images.jianshu.io/upload_images/6857741-a1ede6cfbd9cf879.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/469)

![](https://upload-images.jianshu.io/upload_images/6857741-97322ed540b85b71.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/554)

![](https://upload-images.jianshu.io/upload_images/6857741-8b0aa2a1574b6f7c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/618)

44、chown:修改文件所有者。



![](https://upload-images.jianshu.io/upload_images/6857741-fd038639401f6ccf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/669)



45、修改文件所属组。



![](https://upload-images.jianshu.io/upload_images/6857741-a3b2121b97e1c051.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/661)

46、关机重启：

![](https://upload-images.jianshu.io/upload_images/6857741-7b9cd2327f21f993.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/554)

**本文列举了常用的APT命令参数：**

apt-cache search package搜索软件包

apt-cache show package获取包的相关信息，如说明、大小、版本等

sudo apt-get install package安装包

sudo apt-get install package --reinstall重新安装包

sudo apt-get -f install修复安装

sudo apt-get remove package删除包

sudo apt-get remove package --purge删除包，包括配置文件等

sudo apt-get update更新源

sudo apt-get upgrade更新已安装的包

sudo apt-get dist-upgrade升级系统

apt-cache depends package了解使用该包依赖那些包

apt-cache rdepends package查看该包被哪些包依赖

sudo apt-get build-dep package安装相关的编译环境

apt-get source package下载该包的源代码

sudo apt-get clean && sudo apt-get autoclean清理无用的包

sudo apt-get check检查是否有损坏的依赖






