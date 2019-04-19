# Android的adb工具 - 三少GG - CSDN博客
2011年09月05日 14:37:25[三少GG](https://me.csdn.net/scut1135)阅读数：857
Android的adb工具的使用：
该工具主要用于与模拟器或真机交互。
位置：<android sdk path>/tools/adb **!!!Grant: 现在sdk r5应该为****<android sdk path>/platform-tools/**adb
可以将路径添加到系统环境变量 
**设置环境变量的具体方法参见[http://www.blogjava.net/leekiang/archive/2010/12/30/341946.html](http://www.blogjava.net/leekiang/archive/2010/12/30/341946.html)**
解决方案：采用文中的方法2：修改.bashrc文件
如果你需要给某个用户权限使用这些环境变量，你只需要修改其个人用户主目录下的.bashrc就可以了,而不像第一种方法给所有用户权限。
<1>在 shell终端执行命令：gedit /home/<username>/.bashrc
<2>在.bashrc文件末尾加入：
**export PATH=/home/grant/AndroidDev/android-sdk-linux_x86/platform-tools:$PATH**
打开终端，输入env查看最新PATH。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
【注意】未添加环境变量之前 需要用指令 ./adb
                添加环境变量之后 需要用指令  adb      原因尚不明。。。
常用命令：
1、**安装adk程序：$./adb install <apk file name>**
**2、进入shell$./adb shell**
3、执行一条shell命令：
$./adb shell <command>
4、删除：
首先进入shell：
$./adb shell
再执行删除操作：
#rm [-r] <file name>
-r参数：用于删除文件夹。
5、**拷贝文件$./adb push <local file name><target path>$./adb pull <target file name><local path>**
6、取得ID及序列号：
$./adb get-product
$./adb get-serialno
7、提高执行权限：
$./adb remount
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
[http://book.51cto.com/art/200912/173706.htm](http://book.51cto.com/art/200912/173706.htm)
**3.2.1  adb+push上载文件到SD存储卡**
使用Android仿真器加上-sdcard指令装入SD存储卡后，似乎一切都没有发生。其实不然，Android是Linux操作系统的一种，它将SD存储卡挂接（mount）在系统的/sdcard目录中，我们要访问SD存储卡，是无法直接在Android emulator的手机中来完成的，Android的仿真器并没有资源管理器可以做一般操作系统上的复制删除文件动作，一切必须靠adb (Android Debug Bridge) 指令来完成。现在必须开启另外一个终端机窗口，然后运行Android
 SDK tools目录底下的adb指令加上push参数，将计算机中的文件复制到刚刚制作好的SD存储卡图像档中，指令如下：
- adb push 001.jpg /sdcard             (复制001.jpg文件到 /sdcard 目录下)  
- adb push pictures /sdcard (复制pictures照片目录到 /sdcard 目录下)  
- adb push mp3 /sdcard                 (复制mp3音乐目录到 /sdcard 目录下) 
复制好文件后，您必须运行adb shell指令进入Android系统，然后使用Linux系统的ls与cd指令浏览/sdcard存储卡文件。
- adb shell                       (Android仿真器启动命令列模式)  
- # cd /sdcard                        (进入 /sdcard目录)  
- # ls                            (查看SD存储卡中的文件) 
如果成功的话，则已经将计算机中的文件复制到Android系统的SD存储卡了。现在使用Android仿真器启动手机里的Gallery图片浏览程序是不是已经看到刚刚已经加载的图片了，或是使用Music音乐播放程序，也可以直接看到加载的MP3音乐了，如图3-14所示，而不会再显示 no SD card了。如果还是显示no SD card或是抓不到信息的话，请关掉仿真器，重新运行emulator -sdcard sdcard.img，就可以正确抓到SD存储卡内的文件了。
（点击查看大图）图3-14  读取SD存储卡的图片与音乐
**3.2  Android仿真器启用SD存储卡的方法**
不论是听MP3音乐或是浏览图片，Android手机的保存媒介就是常用的Micro SD存储卡。若想在Android仿真器播放音乐或者看数位相片，就必须先仿真出SD存储卡，并让Android仿真器能够访问仿真出来的SD存储卡。Android SDK组件的tools目录里，除了emulator这个运行文件外，另外放置mksdcard这个程序，它可以仿真出您所需要的SD存储卡空间，例如使用mksdcard指令仿真出1 GB的存储卡，指令如下：
- mksdcard 1024M sdcard.img 
经过一小段的硬盘读取时间后，就会在Android SDK tools/ 目录下生成一个文件大小为1GB的sdcard.img文件，也可以依照自己的需要更改1 024 MB为其他数字，制造出自己所需的存储卡大小，例如4 096 MB (4 GB) 或8 192 MB (8 GB)。然后我们以emulator指令启动Android仿真器时，必须添加-sdcard参数指定该sdcard.img存储卡文件，就可以让Android仿真器拥有插入SD存储卡到T-Mobile G1手机的仿真效果。
- emulator -sdcard sdcard.img 
