# Linux下minicom的配置和使用方法 - xqhrs232的专栏 - CSDN博客
2017年07月06日 18:31:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：173
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/acidgl8757/article/details/9025319](http://blog.csdn.net/acidgl8757/article/details/9025319)
相关文章
1、[linux超级终端minicom的使用方法](http://blog.csdn.net/bird67/article/details/2127235)----[http://blog.csdn.net/bird67/article/details/2127235](http://blog.csdn.net/bird67/article/details/2127235)
2、**linux下串口工具minicom使用----[http://www.2cto.com/os/201111/110568.html](http://www.2cto.com/os/201111/110568.html)**
安装配置minicom
--------------------------------------------------
# lsmod | grep usbserial (如果直接使用串口线，而没有用到USB转串口设备，此步可以跳过)
   如果有usbserial，说明系统支持USB转串口。 
安装minicom (Fedora自带有minicom，此步可以跳过)
apt-get install minicom
   apt-get install lrzsz
配置minicom
   # minicom -s
   Serial port setup [Enter]
    +-------------------------------------------------------------+
    | A -    Serial Device      : /dev/ttyUSB0                   
 |
    | B - Lockfile Location     : /var/lock                       |
    | C -   Callin Program      :                                 |
    | D - Callout Program      -:                                 |
    | E -    Bps/Par/Bits       : 115200 8N1                     
 |
    | F - Hardware Flow Control : No                              |
    | G - Software Flow Control : No                              |
    |                                                             |
    |    Change which setting?                                    |
    +-------------------------------------------------------------+
注: 如果没有使用USB转接口，Serial Device要配置为/dev/ttyS0
   Save setup as dfl
   Exit from Minicom
**---------------------------------------------------------------------**
**+-----| Cannot write to /etc/minicom/minirc.dfl | | File| | | File+-------------------------------------------+ | Serial port setup | | Modem and dialing | | Screen and keyboard | | Save setup as dfl | | Save setup as.. | | Exit |**
**导致这个问题的原因是写的权限不够：sudo chmod 777 /etc/minicom/minirc.dfl然后就OK了。**
**---------------------------------------------------------------------**
使用minicom传输文件
--------------------------------------------------
# minicom
(Ctrl + a) --> s --> zmodem --> [Okay] --> 
    +-----------------------------------------+ 
    |No file selected - enter filename:       | 
    |> /home/zxl/ssl/codes/led/lls_led.ko     | [Enter]
    +-----------------------------------------+ 
注: download kernel by minicom in ubuntu
--------------------------------------------------
    When I download linux kernel by x protocol in minicom, it says "Failure executing protocol, press any key to continue".
    The problem is the x protocol isn't installed bye ubuntu by default, so you should install it by hand:
# apt-get install lrzsz
    After successful download and install, you can send files by x protocol in minicom.
lrzsz包括了x/y/zmodem协议
安装lrzsz
--------------------------------------------------
1. 从下面的网站下载lrzsz-0.12.20.tar.gz
   http://www.filewatcher.com/m/lrzsz-0.12.20.tar.gz.280938.0.0.html 
2. tar zxvf lrzsz-0.12.20.tar.gz
3. 查看里面的INSTALL文档了解安装参数说明和细节
4. # cd lrzsz-0.12.20
5. # ./configure --prefix=/usr/local/lrzsz
6. # make
7. # make install
8. 建立软链接
   # cd /usr/bin
   # ln -s /usr/local/lrzsz/bin/lrz rz 
   # ln -s /usr/local/lrzsz/bin/lsz sz
   这样minicom就可以使用zmodem传输文件了
配置文件所在目录
--------------------------------------------------
Ctrl + a     o
         +-----[configuration]------+
         | Filenames and paths      |
         | File transfer protocols -|
         | Serial port setup        |
         | Modem and dialing        |
         | Screen and keyboard      |
         | Save setup as dfl        |
         | Save setup as..          |
         | Exit                     |
         +--------------------------+
+-----------------------------------------------------------------------+
| A - Download directory : /home/zxl                                    |
| B - Upload directory   : /tmp                                         |
| C - Script directory   :                                              |
| D - Script program     : runscript                                    |
| E - Kermit program     :                                              |
| F - Logging options                                                   |
|                                                                       |
|    Change which setting?                                              |
+-----------------------------------------------------------------------+
    A - download 下载的文件的存放位置 
    B - upload  从此处读取上传的文件
    download     开发板 ---> PC
    upload       PC     ---> 开发板 
    B - Upload directory   : /tmp 
    PC机向开发板发送文件，需要发送的文件在/tmp目录下(PC机上的目录)。做了此项配置后，每次向开发板发送文件时，只需输入文件名即可，无需输入文件所在目录的绝对路径
开发板 ---> PC
--------------------------------------------------
    开发板上的(操作)命令
    # sz filename
    开发板上的文件filename将被传输到PC机上/home/zxl目录下
录制开发板输入信息:
--------------------------------------------------
Ctrl + A + L 
    +-----------------------------------------+                 
    |Capture to which file?                   |                 
    |> /tmp/minicom.cap                       |                 
    +-----------------------------------------+        
