# bootargs中的环境变量说明和一些常用的uboot命令 - maopig的专栏 - CSDN博客
2012年01月11日 14:12:36[maopig](https://me.csdn.net/maopig)阅读数：1709标签：[command																[output																[video																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)
bootargs中的环境变量说明和一些常用的uboot命令
一些常见的uboot命令：
Help [command]在屏幕上打印命令的说明
Boom [addr]启动在内存储器的内核
Tftpboot通过tftp启动内核
Saveenv保存环境变量
setenv name [value]  设置环境变量
bootargs中的环境变量
console:告诉内核从哪接收启动信息
  例如：console=ttyS0,115200n8//115200比特率，8位Video:告诉内核信息怎么配置视频设备，例如可以选择：davincifb | omap24xxfb
        Davincifb是达芬奇，omap24xxfb是OMAP3，这个参数我们一般不变，就选Davincifb
1、vid[0-1]=[width x height x bpp,mem] ：说明帧缓冲区的尺寸大小，每秒比特数，和内存空间大小，如果利用V412驱动，而不用达芬奇，那么可以选择vid0=OFF:vid1=OFF，如果选择达芬奇，可以选择：720x4802吗，2500KB of memory，例如：
         vid0=720x480x16,2500K2、osd[0-1][width x height x bpp,mem]：说明尺寸大小，每秒比特数，和内存空间大小，例如可以分配osd0=720x480x16,2500Kdavinci_enc_mngr.ch0_output= 可以选择：COMPOSITE   COMPOSITE1、SVIDEO、SVIDEO1、COMPONENT、COMPONENT1、LCD、LCD1
davinci_enc_mngr.ch0_mode：设置达芬奇的值：
   ntsc   NTSC-RGB   PAL   pal   PAL-RGB   720P-24   720P-25   720P-30   720P-50   720P-60   1080I-25   1080I-30   1080P-25   1080P-30   1080P-50   1080P-60   480P-60   576P-50   640x480   640x400   640x350   800x480
