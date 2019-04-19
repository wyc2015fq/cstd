# dm365 resize - maopig的专栏 - CSDN博客
2011年11月30日 23:34:43[maopig](https://me.csdn.net/maopig)阅读数：5680
DM368支持视频的缩放功能，例如DM365可以编码一个720P的，同时可以以任意分辨率（小于720P的分辨率）输出。其中有两种模式：IMP_MODE_SINGLE_SHOT，IMP_MODE_CONTINUOUS.
在用dm365的时候，用resizer的ioctl，一直不好使，后来在ti论坛上看到一个人的讲解，原来是bootargs的错。
在配置的时候，有一项dm365_imp.oper_mode=1，这个要配成1.
下面是我的配置，仅供参考
setenv bootargs mem=80M console=ttyS0,115200n8 noinitrd rw ip=192.168.0.7:192.168.0.1:255.255.255.0 nfsroot=192.168.0.8:/home/mick/davinci/nfs_filesystem/dm365_filesys davinci_enc_mngr.ch0_output=LCD video=davincifb:vid0=720x480x16,2025K@0,0:osd0=720x480x16,1350K@0,0
 davinci_capture.device_type=4 dm365_imp.oper_mode=1
DM365同时实现H.264和JPEG双码流输出
DM365的HDVCP和IMCOP是两个独立单元，H.264编码和JPEG编码运行在两个不同的硬件引擎，但是目前TI的编码库(H.264和JPEG)共享一些资源，比如EDMA通道和内存等。因为这个原因，要在DM365上实现H.264和JPEG双码流输出，就得让二者按序运行。TI目前提供了一些机制来并行输出双码流，相应的设置流程为：
1. Use scratchGroup Id = 1 for H.264 encoder, scratchGroupId = 2 for JPEG in your .cfg file
2. Release some EDMA channels from the kernel. You have to free atleast 4-6 channels and it can be done in edma.c in the linux kernel source code. The choice of channels to remove depends on your use case and the peripherals you use.
3. Turn on enableDDR parameter of H.264 encoder.
If you have IPNC Reference Design software, you can refer to it for item #1 and #3. But in IPNC also, we do not run JPEG and H.264 in parallel.
Please note that MPEG4 and H.264 cannot run in parallel due to EDMA channel limitation, so keep them both in some scratchGroupId.
dm365的resizer使用
在用dm365的时候，用resizer的ioctl，一直不好使，后来在ti论坛上看到一个人的讲解，原来是bootargs的错。在配置的时候，有一项dm365_imp.oper_mode=1，这个要配成1.
下面是我的配置，仅供参考
setenv bootargs mem=64M console=ttyS0,115200n8 noinitrd rw ip=192.168.0.7:192.168.0.1:255.255.255.0 nfsroot=192.168.0.8:/home/mick/davinci/nfs_filesystem/dm365_filesys davinci_enc_mngr.ch0_output=LCD video=davincifb:vid0=720x480x16,2025K@0,0:osd0=720x480x16,1350K@0,0
 davinci_capture.device_type=4 dm365_imp.oper_mode=1
