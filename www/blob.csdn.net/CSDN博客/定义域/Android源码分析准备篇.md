# Android源码分析准备篇 - 定义域 - CSDN博客
2015年09月17日 09:38:55[定义域](https://me.csdn.net/u010278882)阅读数：536
## 计划
源码分析的基本要求 
1. 熟悉C/C++/Java语言的基本语法 
2. 熟悉Linux的基本知识，文件系统、权限管理 
3. 熟悉基本工具的使用，git、source insight 
4. 资料准备：源码下载，工具下载和安装
知识准备 
1. 复习C语言的基本语法，特别是涉及指针相关的部分 
2. 学习C++中相对于Java不同的面向对象的实现语法 
阅读《C++程序设计：现代方法》 
整理语法表（思维导图的形式最佳） 
3. 复习Linux的基础知识 
阅读《精通Linux（第2版）》 
系统基本结构、指令参考 
4. Git工具的使用 
5. source insight 工具的使用
源码阅读参考 
《深入理解Android卷I》 
《深入理解Android卷II》 
《深入理解Android卷III》
## 开始
### 目标
```
* 更全面：不再零散的了解整个Android Framework应用层的内容，希望能够从更整体、更完整的了解其面貌。
* 更深入：希望通过跟随几本书的作者，阅读和调试部分重要的源码，从源码（内部运作原理）上了解其工作原理。至少能够了解到大部分常用的组件/模块的工作原理。
```
#### 搭建环境
```
* 源码环境
    * 系统：（跟随作者尝试）Ubuntu 10.04 x86_64
    * JDK：jdk-6u27-linux-x64
    * IDE：Eclipse
    * 源码：Android 4.0.1
* 开发环境
    * 系统：Windows 7
    * JDK：1.7.0.045
    * IDE：Android Studio（之前尝试多次，这次一定要过渡成功）
```
##### 任务1 安装Windows7和Ubuntu双系统
###### 需求
根据Android源码构建说明，首先不支持Window系统，支持Linux和MacOS系统。 
其次，对于Linux系统，不再支持10.04和12.04版本，而推荐使用14.04版本。 
Android 2.3及以上系统需要使用64位环境进行构建。所以最终选择Ubuntu14.04_amd64。 
参考：[构建Android系统的软硬件要求](http://source.android.com/source/requirements.html)
###### 方案
参考 
1. [预装Win7系统安装Ubuntu且Win7引导启动](http://jingyan.baidu.com/season/39743)
2. [EasyBCD官方下载](http://neosmart.net/EasyBCD/)
###### 执行
- 计算机预装Windows7系统 √
- 
计算机支持USB启动 √
- 划分安装Ubuntu所需硬盘空间（根据Android系统构建的需求描述：单个系统构建 
需要至少150G），这里分配300G（含系统大小）。
- 使用UltraISO将系统写入U盘 
1）安装并注册 
用户名：王涛 
注册码：7C81-1689-4046-626F 
2）使用UltraISO打开 ubuntu-14.04.3-desktop-amd64.iso 
3）菜单栏：启动-写入硬盘镜像，弹出窗口其中的“硬盘驱动器”选择自己的U盘，“写入方式”选择USB-HDD+，然后点击“写入”。之后会格式化U盘，将系统数据写入U盘，待进度条完成后便完成了制作。
- 插入U盘，重启电脑，将电脑设置成U盘启动，设置成功后进入安装界面。
- 系统安装配置： 
1）安装界面语言：简体中文 
2）操作：安装Ubuntu 
3）系统语言：简体中文 
4）安装类型：其他选项，不要选择“安装Ubtuntu，与Window7共存” 
5）磁盘管理：选择“空闲”设备（这里应该是300G），点击“添加”，按照如下配置进行设置。
```
正常情况：
/dev/sda
    /dev/sda1
    空闲
安装启动引导器的设备
/dev/sda ATA VBOX HARDDISK
配置
* 引导分区 /boot 200M
* 系统分区 / 50G
* 交换分区 /swap 8G
* 个人文件分区 /home 剩余空间
配置后
/dev/sda
    /dev/sda1 ntfs 
    /dev/sda2 ext4 /boot 格式化
    /dev/sda3 ext4 /         格式化
    /dev/sda4 swap
```
```
6）安装启动引导设备，选择 /boot所在分区
7）正常安装其他配置，重启后进入Windows系统
```
5. 使用EasyBCD修改添加启动项选择 
    1）安装完成后选择”添加新条目“，选择”Linux/BSD“选项卡 
    2）类型“GRUB”，驱动器“/boot所在分区”，完成后点击“添加条目” 
如果不行，配置“GRUB 2”，没有驱动器可以选择，同样点击“添加条目”
**问题1 安装类型阶段分区的选择**
在磁盘管理阶段出现问题，之前Windows系统下存在两个分区（C盘和D盘），在Linux中查看分别表示/sda1和/sda5，在设置了/boot和/后（分别为/sda3和/sda4）剩余的磁盘不可用。应该如何解决？ 
系统支持的主分区只有四个，Linux的所有分区选择为逻辑分区即可。
**问题2 U盘安装盘恢复普通U盘**
UltraISO制作的U盘安装盘无法格式化，如何恢复U盘？ 
参考：[http://jingyan.baidu.com/article/95c9d20da2ba5eec4f756154.html](http://jingyan.baidu.com/article/95c9d20da2ba5eec4f756154.html)
1. 插入U盘，启动UltraISO 
2. 菜单栏：启动-写入硬盘镜像，选择需要恢复的U盘，镜像留空即可 
3. 单击“写入”即可恢复
**问题3 系统配置：修改host文件**
网络问题，之前访问Android开发站点采用修改Host文件的方式，那么，在Linux系统上（Ubuntu）如何修改Host文件呢（并恢复）？ 
修改host文件 
1. Linux的host文件位于/etc/host，可以通过vi进行编辑 sudo vi /etc/hosts 
2. 输入i（进入编辑模式） 
3. 拷贝网址 
4. 按Esc 
5. 输入:wq
##### 任务2 Android源码下载和编译环境准备
参考 
1. [编译环境](http://source.android.com/source/initializing.html)
2. [源码下载](http://source.android.com/source/downloading.html)
3. [高速镜像下载]()
###### 方案
**Android源码编译环境配置**
- 下载JDK 
使用OpenJDK7
```
$ sudo apt-get update // 更新系统软件
$ sudo apt-get install openjdk-7-jdk
$ sudo update-alternatives --config java // 更新默认的Java版本
$ sudo update-alternatives --config javac
```
> 
**背景知识**
  1. sudo 以root用户身份执行需要root权限的命令 
```
sudo命令 
      用途：使普通用户可以像root用户一样执行某些操作。 
      只有在sudoers配置文件中的用户才能执行sudo命令， 
  除了用户需要在配置文件中，执行命令还需要输入执行sudo命令的密码。 
  位置： 
  /usr/bin/sudo 
  该文件时root用户所拥有的 
  配置： 
  配置文件位置：/etc/sudoers 
  任务： 
  * 用户查看自己是否具有sudo权限 sudo -v 
  * 修改可以使用sudo命令的用户及其执行密码 
  条件：只有root用户才能编辑该文件，编辑该文件需要使用 visudo
```
  2. apt-get apt:Advanced Package Tools，高级包管理工具，是管理应用程序的工具 
  3. update-alternatives 用于切换同一软件的多个不同版本的工具 
`--display CMD` // 列出一个命令的所有可选链接 
`--config CMD` // 设置一个可用值，每次执行会进行切换，*指向的link即为选择的link 
`--install` // 安装一个命令link 
`--remove` // 移除一个命令link 
  参考：[update-alternatives命令详解](http://coolnull.com/3339.html)
  4. 环境变量及其设置 
  环境变量分两类：对所有用户都起作用的系统环境变量和仅对个别用户起作用的用户环境便令。系统环境变量设置文件位于/etc目录下，该目录下的 environment、profile、bash.bashrc；用户环境变量设置文件位于 ~/（home目录下），该目录下的 .profile、.bash_profile、.bashrc。 
  1）environment 和 profile 的区别 
  enviroment是针对系统进行设置的，profile是针对所有用户进行设置的。 
  模型：系统是先启动系统程序，当用户登录的时候才会创建用户进程。 
  environment无论用户是否登录都起作用，profile只有用户登录后对用户的进程才起作用。bash.bashrc是针对bash shell这ge个应用设置的环境变量。 
  2）用户环境变量的搜索顺序 
  /etc/profile -> /etc/environment -> ~/.profile -> /.env 
  3）环境便令设置指令
**a. export**
该命令用于显示或者设置环境变量
```
export  // 显示环境变量
export NAME=VALUE // 设置环境便令
$NAME // 取得之前的环境变量
```
**b. source FILE** // 在当前bash环境下读取并执行文件中的命令 
设置Java环境变量[示例]（注意：路径之间使用分号分割）
```bash
export JAVA_HOME=/usr/lib/jdk1.8.0_45
export CLASSPATH=.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib:$CLASSPATH
export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH
```
> 
5.vi编辑器的基本使用 
  vi有三种工作模式：普通模式、命令模式和编辑模式。 
  vi打开之后进入普通模式，输入i进入编辑模式，输入:回到命令模式，再输入wq退出。
###### 2.安装编译需求包
Ubuntu14.04 安装 
$ sudo apt-get install bison g++-multilib git gperf libxml2-utils make python-networkx zlib1g-dev:i386 zip
**问题1 软件更新机制：更新源和依赖**
不能正常安装某些软件包，少依赖 
因为系统无法达到您要求的状态造成的。该版本中可能会有一些您需要的软件 
包尚未被创建或是它们已被从新到(Incoming)目录移出。 
下列信息可能会对解决问题有所帮助：
下列软件包有未满足的依赖关系： 
 g++-multilib : 依赖: gcc-multilib (>= 4:4.8.2-1ubuntu6) 但是它将不会被安装 
                依赖: g++ (>= 4:4.8.2-1ubuntu6) 但是它将不会被安装 
                依赖: g++-4.8-multilib (>= 4.8.2-5~) 但是它将不会被安装 
E: 无法修正错误，因为您要求某些软件包保持现状，就是它们破坏了软件包间的依赖关系 
原因：可能是源没有这些软件，可以通过修改软件更新源的方式来修改。 
参考：[修改更新源](http://jingyan.baidu.com/article/e3c78d6460a9893c4d85f560.html)
更新软件源的操作：Ubuntu软件中心 - 菜单栏“编辑” - 软件源 - Ubuntu软件选项卡 - 重新选择“下载自”的来源即可。推荐使用网易的源。 
解决方法：分别安装独立的依赖即可。执行如下两条指令： 
$sudo apt-get install g++  $sudo apt-get install gcc-multilib
**问题2 资源锁定的原因和解锁**
之前执行apt-get命令更新软件包后发现软件源无法找到相应的包，然后更改软件源后执行，出现如下的错误信息： 
E: 无法获得锁 /var/lib/dpkg/lock - open (11: 资源暂时不可用) 
E: 无法锁定管理目录(/var/lib/dpkg/)，是否有其他进程正占用它？ 
原因：apt-get的进程没有结束，占用了相应的资源，关闭还在运行的apt-get进程即刻。操作：ps -aux，记录apt-get进程的id，然后执行sudo kill pid。执行该命令后发现进程数量太大，难以找寻，这里通过强制解锁解决。操作：sudo rm /var/lib/dpkg/lock。
###### Android源码下载
参照说明直接执行指令即可(官网) 
1.在home目录创建名称为bin的目录，并将其添加到环境变量中
```bash
$mkdir ~/bin
$PATH=~/bin:$PATH
```
2.下载Repo工具并确保其可执行
```
$curl https://storage.googleapis.com/git-repo-downloads/repo > ~/bin/repo
$chmod a+x ~/bin/repo
```
3.在主目录创建工作目录WORKING_DIRECTORY
```bash
$mkdir WORKING_DIRECTORY
$cd WORKING_DIRECTORY
```
4.初始化repo
`$ repo init -u https://android.googlesource.com/platform/manifest -b android-4.0.1_r1`
如果没有指定 -b 版本，那么将直接下载master版本
5.弹出窗口中输入自己的google账户信息 
初始化成功后会在工作目录中创建一个.repo文件夹，其中包含了manifest文件
6.下载
```bash
$repo sync
```
> 
背景知识 
  1. curl命令（**?**） 
  2. chomd命令（**?**） 
  3. repo工具使用（**?**）
**使用国内镜像（清华大学）下载AOSP**
参考：[AOSP镜像](http://mirrors.tuna.tsinghua.edu.cn/help/#AOSP)
1.准备好自己的Google帐号，记录用户名和邮箱，配置git全局信息
```
git config --global user.email "google邮箱"
git config --global user.name "代码提交用户名"
```
2.主目录中创建bin目录，用于存放repo
3.从镜像站下载repo，然后拷贝到bin目录
```
[bin]git clone git://aosp.tuna.tsinghua.edu.cn/android/git-repo.git
[bin]cp git-repo/repo ~/bin/
```
4.修改repo通讯地址，改为清华大学
```
[bin] gedit ~/bin/repo
REPO_URL = 'git://aosp.tuna.tsinghua.edu.cn/android/git-repo'
```
5.主目录创建WORKING_DIRECTORY，保存Android项目源码
6.初始化repo
`$[WORKING_DIRECTORY] repo init -u git://aosp.tuna.tsinghua.edu.cn/android/platform/manifest -b android-4.0.1_r1`
7.下载
`$[WORKING_DIRECTORY] repo sync`
8.同步配置 
将 **repo/mainifest.xml** 文件中的 
```
<remote  name="aosp" 
         fetch=".." />
```
替换为 
```
<remote  name="aosp" 
        fetch="git://aosp.tuna.tsinghua.edu.cn/android/" 
         review="https://android-review.googlesource.com/" />
```
