
# MIPI屏数据发送命令解析 - 嵌入式Linux - CSDN博客

2015年04月03日 15:58:21[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2399


![](https://img-blog.csdn.net/20150403155727985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2VpcWlmYTA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
MIPI数组发送那里有一个数组，这个数组包含寄存器和寄存器的值：

相当于: 0XC480寄存器下参数0X9C；
0XFF00寄存器下参数0XFF,0XFF,0XFF；
0XC0B5 尝试一下寄存器 0x08 0x18 0x48 0x58 试一下能不能旋转

![](https://img-blog.csdn.net/20151204095856857?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


