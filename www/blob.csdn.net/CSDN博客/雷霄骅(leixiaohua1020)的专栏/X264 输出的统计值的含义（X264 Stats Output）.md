# X264 输出的统计值的含义（X264 Stats Output） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月22日 00:51:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：27
个人分类：[视频编码](https://blog.csdn.net/leixiaohua1020/article/category/1638135)










典型的x264输出（Stats Output）如下：
avis [info]: 1280x720 @ 1.77 fps (40997 frames)
yuv4mpeg: 640x480@30/1fps, 0:0
x264 [info]: 352x288 (given by file name) @ 25.00 fps

取决于输入源会有不同，但大致形式一样，以上三行分别对应于avisynth, y4m, yuv输入。
以下行对应于x264 CLI，使用libx264时不会显示。
x264 [info]: using cpu capabilities: MMX2 SSE2Fast SSSE3 Cache64

信息未必对应上CPU所有的指令集。有些芯片上，x264不会使用某些指令集，因为用了反而更慢。
x264 [info]: profile High 4:2:2, level 3.1, 4:2:2 8-bit

视频的h264 profile, output-csp, colorspace和level信息。profile由使用的参数决定(如，B帧决定了至少是main profile，8x8dct决定了至少是high profile)，只是在此显示该信息。而level值会写入码流。可以用--level来手工指定所需level，或不指定，让x264来猜(还是比较准的)。显示的位深与level取决于x264的版本及编码参数。
x264 [info]: frame I:461   Avg QP:16.16  size: 51489  PSNR Mean Y:45.96 U:50.20 V:49.59 Avg:46.85 Global:44.69
x264 [info]: frame P:11836 Avg QP:17.35  size: 28198  PSNR Mean Y:42.89 U:47.43 V:46.95 Avg:43.81 Global:42.82
x264 [info]: frame B:28700 Avg QP:16.99  size: 11522  PSNR Mean Y:41.85 U:45.69 V:45.32 Avg:42.68 Global:42.27

对于三种不同类型的帧，分别显示该类型总帧数、该类型帧内每个宏块的平均量化值、该类型帧的平均大小、以及PSNR结果(需设置)
x264 [info]: consecutive B-frames:  3.5% 10.8% 20.6% 28.2% 16.8%  4.5% 15.6%

各个连续B帧数的百分比， P = 1, PB = 2, PBB = 3, 以此类推
x264 [info]: mb I  I16..4: 18.3% 52.4% 29.2%
x264 [info]: mb P  I16..4:  3.6%  9.4%  3.0%  P16..4: 45.3% 18.5% 10.0%  0.0%  0.0%    skip:10.3%
x264 [info]: mb B  I16..4:  0.3%  1.0%  0.2%  B16..8: 35.2%  1.4%  1.6%  direct: 6.1%  skip:54.3%  L0:50.6% L1:43.0% BI: 6.3%

对三种不同类型的宏块(I,P,B)，分别显示区块中是I或native type(P表示P宏块，B表示B宏块)的百分比。
I区块的三个数，分别代表16x16, 8x8和4x4。
对于P区块，5个数分别代表16x16, 16x8/8x16, 8x8, 8x4/4x8和4x4。
对于B区块，3个数分别代表16x16, 16x8/8x16和8x8。
skip显示不用residual而使用skip vector的宏块数量(编码器不将运动向量写入比特流，跳过之)，而direct显示使用skip vector且使用residual的宏块数。解码器自身计算获取skip/direct块的运动向量：利用附近已解码的其它块、平均已知且已预估的运动向量。
L0和L1分别表示向前和向后参考帧的百分比。译者注：BI为同时参考了之前和之后的帧。
x264 [info]: mb I  I16..4..PCM:, P I16..4..PCM:, mb B I16..4..PCM:
当编码器决定使用PCM宏块时(相同大小的原始未压缩的块)，会显示出类似上面的宏块表格。但块的名字前加上PCM前缀。
x264 [info]: final ratefactor: 27.45

在使用ABR pass时显示，表示编码的平均QP
x264 [info]: 8x8 transform intra:59.0% inter:63.9%x264 [info]: direct mvs  spatial:99.3%  temporal:0.7%

direct/skip vector计算的各种方法的使用百分比。
x264 [info]: ref P L0  46.2% 18.9% 13.6%  7.7%  6.9%  6.6%
x264 [info]: ref B L0  51.2% 20.8% 13.7%  9.2%  5.2%
x264 [info]: ref B L1  84.7% 15.3%

使用这么多个参考帧的宏块数(每个类型分别列出)。
ref B L1这一行在开启b-pyramid时显示，表明向后参考B帧与P帧的百分数。若b-pyramid关闭，则L1永远参考下一个P帧，因此无需显示百分比。L0与L1的比例信息出现在上面(mb-B那块)。
x264 [info]: coded y,uvDC,uvAC intra: 39.9% 60.7% 56.2% inter: 29.3% 47.8% 37.6%

编码成intra or inter的块数百分比，三个数分别是亮度(luma)，色度直流值(chroma DC)和色度交流值(chroma AC)
x264 [info]: i16 v,h,dc,p: 47% 42%  8%  3%
x264 [info]: i8 v,h,dc,ddl,ddr,vr,hd,vl,hu:  3% 87%  7%  0%  0%  1%  0%  0%  1%
x264 [info]: i4 v,h,dc,ddl,ddr,vr,hd,vl,hu: 24% 27% 18%  6%  5%  5%  4%  5%  7%
x264 [info]: i8c dc,h,v,p: 33% 51% 12%  4%

i8c那行仅在色度分辨率与亮度分辨率不匹配的情况(输出色彩空间：yuv420, yuv422)下出现，显示以下各帧内预测模式的使用情况。
- v - vertical
- h - horizontal
- dc - mean of corner pixels touching this macroblock
- ddl - diagonal down-left
- ddr - diagonal down-right
- vr - vertical-right
- hd - horizontal-down
- vl - vertical-left
- hu - horizontal-up
- p - plane

详见[H.264 intra predicion](http://www.vcodex.com/files/h264_intrapred.pdf)
x264 [info]: Weighted P-Frames: Y:5.2%

加权P参考帧(weighted-p ref frames)的百分比
x264 [info]: SSIM Mean Y:0.9851369 (17.254db)

当指定计算SSIM时，显示其数值及分贝数
x264 [info]: PSNR Mean Y:42.506 U:46.641 V:46.212 Avg:43.374 Global:42.523
当指定计算PSNR时，数值与码率一同显示。(不考虑容器占用体积)
x264 [info]: kb/s:237.1

当不指定输出PSNR时，x264的最终输出码率的格式如上，不考虑容器占用体积。
encoded 300 frames, 119.24 fps, 722.37 kb/s

最后给出编码的帧数，平均编码速度，以及最终码率(包含容器体积)
此行仅在x264 CLI中出现，使用libx264时不会显示。




