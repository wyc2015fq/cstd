# DNW源码的下载和配置 - xqhrs232的专栏 - CSDN博客
2013年05月03日 20:07:46[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：876

【学习】DNW源码的下载和配置
2007年12月13日 星期四 01:18DNW源码的下载和配置
DNW（全称待查）是一个工具软件，用于配置串口通信和USB通信。而阅读其源代码，则能尽快的熟悉和掌握pc机和目标设备之间的通信机制，更快更好的完成应用程序的开发——其功能是PC机和特定开发板之间通信，比如下载数据（PC机——>目标板），当然了，还有上传。
DNW的源码下载地址是：http://www.eetop.com.cn/bbs/viewthread.php?tid=42859&extra=&page=1今天的时代是开源的时代，但是这并不意味着不劳而获。你必须要有所付出，才有所回报。比如你得上传你自己优秀的源程序，或是提供好的开发资料（当然不是公共的资料了），或是干脆——输入银行账号，这一切都可以得到你所想要的（源码）。不过看来，我的确不是诚实的人，或许是闲那些动作太繁琐把——我想直接得到货物而不用付帐。幸运的是，又一次我无意中捡到了金子。要完成这个下载，只需小小的注册即可，无需付费，无需等待。
DNW的源码下载之后，直接编译和运行——当然这是你迫切想去做的了。不过此时可能遇麻烦，是一系列“头文件无法识别”的错误。此时你要做的：
1. 将WINDDK下的WXP目录下的头文件添加到vc下的include文件中。此时你只需要将E:\WINDDK\2600\inc\wxp中的 wxp文件，"ctrl+c"&&"ctrl+v"到C:\Program Files\Microsoft VisualStudio\VC98\Include中。当然，前提是你已经安装了WINDDK和VC到相应的目录之下。这样你的编译器便不会对这些重要的“.h”视而不见了。
2.将WINDDK目录E:\WINDDK\2600\lib\wxp\i386下的usbd.lib,setupapi.lib添加到vc下的lib文件中
3.如果碰到：fatal   error   LNK1112:  module   machine   type  "IA64"   conflicts   with  target   machine   type  "IX86" （模块计算机类型“IA64”与目标计算机类型“IX86”冲突），首先，进入VC下的LINK，如图
并且要将ProjectOptions下的内容手动修改成图中的内容——我特别指的是machine:IX86和最后一行的\lib\i386\... 然后将E:\WINDDK\2600\lib\wxp\i386下的库文件mfc42u.lib到C:\Program Files\Microsoft Visual Studio\VC98\Lib之中。注意，不是E:\WINDDK\2600\lib\wxp\ia64目录下的那个mfc42u.lib
编译，运行，通过。OK，大功告成！！
//================================================================================================================================
DNW的USB下载操作流程
一、           数据传输格式
USB Txformat: 
addr(4)+size(4)+data(n)+cs(2)
addr：下载到开发板RAM的目标地址，      4字节
size： 传输文件的大小，                       4字节
data：文件数据流，                                  n字节
cs：   checksum 校验和，                            2字节
二、           MenuUsbTransmit发送文件流程
1.       打开usb输出管道文件open_file( outPipe)，如果失败，报错；
2.       选择并打开所要传输的文件；
3.       获取文件大小；fileSize=GetFileSize(hFile,NULL);
4.       分配一个(fileSize+10)大小的缓冲区；4+4+2=10
5.       将文件读入缓冲区txBuf+8（即保留前8个字节）开始处；
6.       将所设置的下载地址赋给txBuf[0:3]4个字节；downloadAddress;
7.       将fileSize+10赋给txBuf[4:7]4个字节
8.       将校验和赋给缓冲区的最后2字节(txBuf+8+fileSize))=cs;
9.       启动usb传输UsbTxFile()
以上只是粗略的了解了一下代码，具体细节需要深入。
