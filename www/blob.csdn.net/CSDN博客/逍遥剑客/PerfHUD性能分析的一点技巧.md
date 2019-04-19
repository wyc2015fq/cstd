# PerfHUD性能分析的一点技巧 - 逍遥剑客 - CSDN博客
2007年12月08日 19:11:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：1968
(1)    在程序中将颜色缓存从32位改为16位，查看FPS值是否增加，以确定瓶颈是否因为帧真缓存带宽引起。
(2)    在NVPerfHUD中按下Ctrl+T将纹理强行设置为2*2大小，查看FPS值是否增加，以确定瓶颈是否因为纹理带宽引起。
(3)    在NVPerfHUD中按下Ctrl+1、Ctrl+2……Ctrl+9,NVPerfHUD会自动强行简化各个Profile的Pixel Shader程序，查看FPS值是否增加，以确定瓶颈是否因为复杂的像素程序引起。
(4)    在NVPerfHUD中按下Ctrl+V去掉顶点单元，查看FPS值是否增加，以确定瓶颈是否因为顶点处理引起，包括顶点/索引缓存的传输、Vertex Shader程序和无效的缓存Lock。
(5)    在程序中更改顶点格式的大小，查看FPS值是否增加，如果增加则瓶颈在顶点/索引缓存的传输。
(6)    可以利用Ctrl+N去掉GPU的工作，也就是这个时候GPU的计算速度无限快，如果FPS增加则瓶颈在CPU上。
