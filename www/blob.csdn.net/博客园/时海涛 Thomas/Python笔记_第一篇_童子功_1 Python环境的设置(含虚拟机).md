# Python笔记_第一篇_童子功_1.Python环境的设置(含虚拟机) - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [Python笔记_第一篇_童子功_1.Python环境的设置(含虚拟机)](https://www.cnblogs.com/noah0532/p/8392818.html)





***Python环境的设置**

Python的环境设置也就是所需工作平台的软件搭建。常用的搭建平台IOS系统+Linux系统和Windows+Linux系统的搭建。这里主要说明的是Windows+Linux搭建。这样你就会有Windows和Linux平台直接的相互调用，方便编写程序和其他操作。

主要内容分为：

**1.虚拟机的安装和配置**

**2.Ubuntu系统的安装和配置**

**3.Python的安装和配置**

**4.其他相关的安装**

**5.其他补遗**



****虚拟机的安装和配置**：

*****安装**：

 第一步：Setup.exe安装虚拟机

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163244953-1327212254.png)

第二步：下一步

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163315671-36163075.png)

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163325890-1514397358.png)

第三步：最好不要安装在C盘

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163356203-35923396.png)

第四步：破解版的话，这里就不用选了

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163419843-1586203855.png)

第五步：下一步

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163433984-921875378.png)

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163442937-610080746.png)

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163449484-1431436482.png)

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163638500-892401605.png)

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163718265-616684562.png)

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163723250-1084186292.png)



*****配置**：

第一步：双击桌面图标打开安装好的虚拟机

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163830687-1050223907.png)

第二步：典型即可

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163906296-1141461427.png)

第三步：先不要安装Ubuntu系统

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131163932781-210777854.png)

第四步：这里随便

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131164000265-1552022384.png)

第五步：指定系统安装位置（也一样最好不要C盘）

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131164033234-1641035118.png)

第六步：这里自动配置即可

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131164059984-612452912.png)

第七步：自动完成

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131164112750-1482195422.png)

第八步：找到配置选项进行搭建好的Ubuntu虚拟机系统的简单配置

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131164152562-756651822.png)

第九步：网络适配器改为桥接模式

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131164319171-374520243.png)

第十步：其他位置可以根据自己需求自动选择

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131164421734-541381350.png)



** **Ubuntu系统的安装和配置**

有了虚拟机之后，我们就要在虚拟机上面安装我们的Ubuntu版本的Linux系统了。

首先先去Ubuntu的官网去下载64位的Ubuntu系统（这里的平台都是64位，不同平台根据不同需求选择）

官网：http://cn.ubuntu.com/download/

选择下载最新版的镜像

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131165254375-1993521937.png)



*****安装**

第一步：在虚拟机位置打开CDROM加载下载好的镜像文件

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131165108156-1922958729.png)

 第二步：开启虚拟机就会进行Ubuntu系统的安装

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131165201906-194395487.png)



*****配置**

第一步：然后就会来到如下界面

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131165624296-1940548481.png)

第二步：到了如下界面，我们点击继续

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131165720015-402141351.png)

第三步：然后点击，开始安装

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131165757140-168193995.png)

第四步：继续

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131165818453-1969076037.png)

第五步：输入你的位置，随便输入就好了

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131165912156-1612590514.png)

第六步：选择语言

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131170044890-1682173320.png)

第七步：设置管理员的账号密码等

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131170112906-11534119.png)

第八步：正式开始安装

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131170140015-1433810228.png)

第九步：安装完成并重启

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131170246406-2081615358.png)

第十步：进入到Ubuntu系统，登录时需要填写账号密码（安装完成）

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131170305718-2123582465.png)



****Python的安装和配置**

在Ubuntu环境下一般都会按照Python语言，不需要额外安装，但是现在的Python主要是Python3版本的语言，有的Ubuntu系统的Python可能还是Python2，可能会导致有些最新的命令无法执行，因此需要在Ubuntu环境下进行一下Python3语言的安装和配置。

*****在Ubuntu环境下的安装和配置（以Python最新版本3.6.4在Linux的Ubuntu下面的安装为例子，详细：适合小白）**

1.在Ubuntu环境中，默认的版本是2.7.13

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131201235234-1470999721.png)

**根据Python的官方说明，Python2将不再更新，2.7.13其实一个Python2到Python3的一个过渡版本，这个版本暂时不用卸载，可以保留使用，因为很多Linux里面用Python写的程序还有老版本的兼容问题。**

2.在Ubuntu中，用浏览器访问Python的官网。https://www.python.org/downloads/source/  这是找到的Linux/Unix版本。

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201010409375-43392652.png)

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201010534593-478416555.png)

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201010648171-2105840207.png)

3.切记要进入root根目录下面进行Python3.6.4版本的安装，其中很多命令在上层目录执行不了。另外很多人不知道如进入根目录，如下说明：

Ctrl+Alt+T或者![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201033402640-947206612.png)Ubuntu左上角的这个图标均可进入linux的命令模式。

```
# 首先输入
sudo passwd root
# 去设置root进入的密码，新用户都没设置，设置过的可以pass这一步

# 然后输入
su root 
#即可进入root根目录模式
```

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201033715218-7057506.png)

（这里是一个简单的截图）

sudo 和su ，是super 超级用户的简写；设置完毕根目录模式，后面的操作都在这目录下面进行

4.开始下载前面所找到的Python3.6.4版本的压缩包。

进入到/usr，用户目录下，然后创建一个download文件夹用于存放准备要下载的压缩包

```
# cd 到这个目录下
cd /usr
# 查看所有文件
ls -a
# 创建一个文件夹
mkdir download
```

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201034219562-1980750098.png)

在这里已经创建好了（当然你叫啥都可以这个不限制）

5.在root命令行输入Wget网站下载抓取命令

```
wget https://www.python.org/ftp/python/3.6.4/Python-3.6.4.tgz
```

（根据网速不同下载速度也不一样）

6.好了下载完毕之后我们对他进行一次解压缩

```
# 解压缩命令
tar -xvf Python-3.6.4
```

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201034555312-799527508.png)

（如图所示，左侧蓝色的即为解压缩后的文件夹，右侧为压缩包，其实这里跟windows操作类似）

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201034712031-1921922193.png)

（另外我们可以ls -a以下，看看里面都是啥东西，反正一堆，看不懂也没啥关系）

7.这一步非常关键，新下载的文件要找一个安装路径，一般都是按照在/usr/local这目录下面，因为Liunx和windows最大的区别是没有盘符的概念因此都是通过树型目录的方式存放和查找文件的

```
# 创建一个文件夹在local下面
mkdir /usr/local/python3

# 这里要特别注意的是 / .. /这种路径必须要写对，如果你就在这个目录下面就不用直接创建即可，其实是废话，为了小白能看清。
```

8.我们还是保持在**/usr/download/Python-3.6.4**这个文件夹的路径下面，把刚才创建的那个要按照的目录进行一下配置configure以下，看出麻烦来吧。不过也行，Windows下面这些步骤都嗖嗖的过去了。

```
# 输入如下代码
./configure --prefix=/usr/local/python3
# python3文件夹和上面新创建的文件目录都是一样的，就是为了配置一下，姑且理解为# 毛坯房有了，给刷一下墙
```

### 9.继续保持在这个文件夹，开始安装了。

这里特别说明一下：在Windows下面直接点击Setup.exe就可以安装了，但是Linux不行，把这个Setup分成两步来进行，第一步先把代码进行编译处理，第二步才是进行安装处理。因此要输入如下两个代码

```
# 进行编译
make
# 然后在进行安装
make install

# 当然也可以把这两段代码连起来执行
make && make install
# 效果跟上面作两步一样，这样就可以做一步了。
```

10. 创建软连接。

所谓创建软连接可以理解为在桌面创建快捷方式（可能比喻不太恰当），也可以理解为给安装好的灯泡拉上电线。这里有两个目录注意第一目录是/usr/bin，还有一个目录是前面的/usr/local。/bin是可执行文件（很像Windows下面的.exe文件）这里先要把原先的进行一下备份，然后把local里面的“快捷方式”（比喻不太恰当）拿到可执行文件目录里面，这样就可以执行了。所谓这个过程也叫软连接。

（注意，如下代码是退出到cd根目录下，如果不是可以选择文件夹直接更改）

```
# 修改旧版本
mv /usr/bin/python /usr/bin/python_bak
# 把原先的软连接位置python备份一下，进行一下重命名

# 创建新的软连接
ln -s /usr/local/python3/bin/python3  /usr/bin/python
# 这是是把local安装后文件夹下面的/bin执行文件python3，拿到刚才备份的位置（准确# 的说是重新建立一个python软连接），占原先的位置。
```

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201040748171-2010009812.png)

（截个图一目了然）

11.完成，查看一下版本，都OK了。现在的3.6.4版本的解释器要用python这个命令来打开。

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201041119890-451285662.png)

（**都搞定了！另外说一下，网上很多说新的3.6.4版本需要用python3.6这个命令打开解释器，准确的说是因为我们前面创建软连接的时候，是创建的python这个软连接，如果直接创建python3.6，而不备份原先的python这个软连接的话那就是打这个python3.6这个命令，其实无妨这个地方**）

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201041511140-569599190.png)

（截个图，一目了然！）

12.配置路径

(**有些服务器不需要加入 可能已经加入可 Python3 看看，不需要就不用配置路径**)

```
# 我查看有的好像是.profile里面
# vim ~/.bash_profile
# .bash_profile
# Get the aliases and functions
if [ -f ~/.bashrc ]; then
. ~/.bashrc
fi
# User specific environment and startup programs
PATH=$PATH:$HOME/bin:/usr/local/python3/bin
export PATH
```

 生效后让上一步生效

```
# source ~/.bash_profile
```



*****在Windows环境下的安装和配置（以Windows7为例）**

 1.跟前面的第一步都一样，去官网找到Windows版本的下载链接到你的电脑上

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201143656125-2067268853.png)

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201143752171-1499673057.png)



![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201143844109-540670505.png)

这里面有几个不同的类型，讲解一下：

其中看到X86-64的这种类型表示的是64位版本；X86的这种类型表示是32位版本，以64位版本为例。

[Windows x86-64 embeddable zip file](https://www.python.org/ftp/python/3.6.3/python-3.6.3-embed-amd64.zip)   压缩型文件的版本

[Windows x86-64 executable installer](https://www.python.org/ftp/python/3.6.3/python-3.6.3-amd64.exe)   可执行文件版本（**一般下载这个**）

[Windows x86-64 web-based installer](https://www.python.org/ftp/python/3.6.3/python-3.6.3-amd64-webinstall.exe)   （基于web的版本）

2.安装过程都很简单，其中到这个页面的时候，主要要选全部安装，并把Python添加到路径的环境变量配置。然后安装完成

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201145304562-1711201887.png)



3.这里多说一点的是，如果没有在这里添加路径，有两种方式可以添加到路径。

### 环境变量配置： 

### 方法一：使用cmd命令添加path环境变量

### 在cmd下输入： path=%path%;D:\Python36  接着按"Enter"回车键。   

### 其中**: **D:\Python36  是Python的安装目录。

### 方法二：在环境变量中添加Python目录

### (1) 右键点击"计算机"，然后点击"属性"

###  (2) 然后点击"高级系统设置"

### (3) 选择"系统变量"窗口下面的"Path",双击即可！

### (4) 然后在"Path"行，添加python安装路径即可(我的D:\Python36)。 **ps：记住，路径直接用分号"；"隔开！**

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201145529750-1390520459.png)



******其他相关的安装****

 ***常用的软件：

　　常用的软件，有人选择Notpad++，我平时用的Sublime多一些，安装过程忽略，都很简单，另外，把神器Pycharm安装上去



*****Pycharm的安装**：

　　Pycharm的安装可以去官网下载Pycharm的软件，分社区版本和专业版，其中专业版是收费的。

　　网站：http://www.jetbrains.com/pycharm/

******Python在Ubuntu下面的安装**：

1.用前面的Wget方式，或者之间在Ubuntu打开网页的方式也可以下载Pycharm的Linux版本。当然这个压缩包也是.gz的压缩包

2.解压缩压缩包，tar -zxvf Pycharmxxx.gz

3.把解压缩后的Pycharm放到/opt目录下面，方便其他用户使用:sudo mv Pycharmxxx/  /opt/

4.切换到/opt目录  cd /opt/Pycharmxxx/bin

5.打开pycharm，在bin目录下输入 ./pycharm.sh就可以正常运行了。

6.如果桌面没有pycharm的图标的话，可以安装如下两种方式添加图标到桌面上面来：

**第一种方法**（最简单的方法）：当pycharm安装好之后（先配置JDK），首先找到pycharm的安装bin目录并复制其路径（一定要包含bin目录），

终端命令行粘贴路径回车进入bin;然后输入命令:./pycharm.sh，然后直接右击快捷图标锁定到启动器或任务栏即可

**第二种方法**（生成桌面快捷图标）：首先在命令行输入：sudo gedit /usr/share/applications/Pycharm.desktop；然后粘贴如下命令并保存:

[Desktop Entry]
Type=Application
Name=Pycharm
GenericName=Pycharm3
Comment=Pycharm3:The Python IDE
Exec=sh /home/zoe/pycharm/pycharm-2016.2.3/bin/pycharm.sh (pycharm.sh所在路径)
Icon=/home/zoe/pycharm/pycharm-2016.2.3/bin/pycharm.png  （pycharm.png所在路径）
Terminal=pycharm
Categories=Pycharm  

最后在计算机目录中进入/usr/share/applications 将Pycharm图标文件复制到桌面上 ,然后修改可执行命令行输入：chmod +777 ./Pycharm.desktop



******如果是专业版的话，需要破解，破解的方式如下**：

第一步：切换到Pycharm的目录这里以2017.1.4版本为例

输入：java -version 后出现如下内容则表示在系统上已配置好java环境

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201153428484-282795137.png)

第二步：取消原有的license，如果之前没有输入过license，仅仅是试用一个月到期了，则无需执行该步骤：

工具栏：Help   ->   Register   ->   Remove License   ->  Exit

第三步：下载Rover12421大神的破解软件，百度JetbrainsCrack-2.6.2.jar能找到很多下载的链接，根下面Windows的破解的哪个补丁一样，哪个版本也行

第四步：将下载的JetbrainsCrack-2.6.2.jar 拷贝到pycharm安装的目录bin下，比如我本机的目录为：/home/lixiang/pycharm-2017.1.4/bin

第五步：在pycharm安装的目录bin下打开一个终端，执行以下命令：sudo java -jar JetbrainsCrack.2.6.2.jar

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201153711468-1971549530.png)

如果出现如图所示，则需要编辑bin文件夹下的pycharm.vmoptions和pycharm64.vmoptions，在这两个文件的最后一行都加上JetbrainsCrack.jar的绝对路径，

如：-javaagent:/home/lixiang/pycharm-2017.1.4/bin/JetbrainsCrack-2.6.2.jar

第六步：在pycharm安装的目录bin下打开一个终端，执行以下命令：sh pycharm.sh开启PyCharm，待出现需要激活界面后，选择中间激活码一栏，

从上图中的products列表中随便选择一个code（sudo java -jar JetbrainsCrack.2.6.2.jar执行后显示的列表信息中），

这里我输入："code":"RD","paidUpTo":"2099-12-31"，然后点击“OK”激活，如下图：

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201153920953-1407431088.png)

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201153858890-862727940.png)

上图的这一堆都是，随便选择一个

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201153955328-1728656050.png)



（完成破解）



******Python在Windows下面的安装**：

安装过程很简单忽略

下面对于专业版的破解进行一下讲解

第一步：找到Pycharm的安装目录，把下载好的破解补丁打到里面

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201151131390-603449527.png)

然后找到这两个文件，用记事本打开他们

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201151222843-116501959.png)

在最后添加相同的这门一句话，其中前半部分指的是你要添加的pycharm安装路径，安装地方不一样，别搞混。

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201151328437-90217317.png)



****其他补遗**

**1.安装Ubuntu时可能遇到无法安装并提示这样的内容**

**“已将该虚拟机配置为使用64位客户机操作系统。但是无法执行64位操作”**

**“如果已在BIOS/固件设置中禁用了Intel VT-x，或主机自更改此设置后未从重新启动”**

**![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131170508843-290017001.png)**

**进入到主板的BIOS系统进行修改虚拟化选项打开**

**![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131170549625-2079178445.png)**

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131170559375-110515890.png)

**进入到主板的BIOS系统进行修改虚拟化选项打开**

****选择Virtualization，回车进入，按+键修改俩个选项为enabled****



******2.进入到Ubuntu系统后，Home目录（家目录）如果是中文，觉得不顺眼，可改为英文目录。具体修改方式如下：******

******右上角的小齿轮位置，点进去有个All Settings******

******![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131171038250-512683789.png)******

**单击语言支持（Language support）按钮（一个旗帜的图标），将会弹出语言支持的窗口，**

**这里显示当前桌面语言的名称，以及已经安装的其他语言。**

![](https://images2017.cnblogs.com/blog/1328368/201801/1328368-20180131171101453-2087175361.png)

**中文改为英文即可，当然也可以来回切换。**

**或者用命令的方式：**

export LANG=en_US

xdg-user-dirs-gtk-update

跳出对话框询问是否将目录转化为英文路径，并以并关闭，在终端中输入命令：

export LANG=zh_CN



3.虚拟机的拍照功能

在虚拟机的菜单栏有一个==》虚拟机==》拍照，这个功能就类似于Windows的系统还原功能一样，可以备份以下。



4.VMTools的安装

初次安装虚拟机后，感觉屏幕不能自由缩放，另外不能和当前的宿主机进行交换文件，相互切换非常麻烦的事情。因此需要安装一个虚拟机工具箱的东西。

![](https://images2017.cnblogs.com/blog/1328368/201802/1328368-20180201044335000-312237467.png)

然后进入你的系统后，会在CDROM光驱的位置找到这个文件夹，这样顺利完成后，虚拟机就会变得非常强大了。














