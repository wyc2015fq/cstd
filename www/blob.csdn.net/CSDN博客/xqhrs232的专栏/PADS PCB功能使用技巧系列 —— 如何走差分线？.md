# PADS PCB功能使用技巧系列 —— 如何走差分线？ - xqhrs232的专栏 - CSDN博客
2016年10月02日 23:34:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1663
原文地址::[http://bbs.elecfans.com/jishu_465496_1_1.html](http://bbs.elecfans.com/jishu_465496_1_1.html)
相关文章
1、USB差分走线----[http://wenku.baidu.com/link?url=QVc-gum-JzotO6Ss17A1n4l6sJijkdG3rXMvoy5wyU2XN1EhnxQiiXjcjpdSwoawD3-KkhB0AE1Cwzw5PcwJggUjTW2dRFI5KqW55kU4Zqe](http://wenku.baidu.com/link?url=QVc-gum-JzotO6Ss17A1n4l6sJijkdG3rXMvoy5wyU2XN1EhnxQiiXjcjpdSwoawD3-KkhB0AE1Cwzw5PcwJggUjTW2dRFI5KqW55kU4Zqe)
2、USB差分走线----[http://www.chinadmd.com/file/spxcieprs6rx6ocr3vrecexa_2.html](http://www.chinadmd.com/file/spxcieprs6rx6ocr3vrecexa_2.html)
3、**USB2.0差分走线注意事项----[http://www.52rd.com/Blog/Detail_RD.Blog_coral_xin_27416.html](http://www.52rd.com/Blog/Detail_RD.Blog_coral_xin_27416.html)**
在网上看到的好帖子
差分信号在高速[电路](http://bbs.elecfans.com/zhuti_dianlu_1.html)设计中应用越来越广泛，如USB、HDMI、PCI、DDR*等，承载差分信号的差分线主要优势有：抗干扰能力强，能有效抑制EMI、时序定位精确等，对于[PCB](http://bbs.elecfans.com/zhuti_pads_1.html)工程师来说，最关注的是如何确保在实际走线中能完全发挥差分线的这些优势。
（1）定义差分对信号：在Router中，同时选定需要走差分线的网络（Net），右击后选择Make Differen[ti](http://bbs.elecfans.com/zhuti_715_1.html)al
 Net，如下图所示。
![](http://images.cnitblog.com/blog/560322/201309/08125748-52a76aba39bd4c92bb1d96e23150e949.jpg)
（2）打开项目浏览器Project Explorer窗口，展开Net Objects树形列表下的Differential Pairs项，刚刚定义的差分对DM<->DP就在这里，选定该差分对后右击选择Properties，如下图所示。
![](http://images.cnitblog.com/blog/560322/201309/08125757-3cd6cacea51a4b14ac1365f539058e6b.jpg)
（3）在弹出的对话框中，可设置相应的线宽及线距，此处分别设置为8与6（8:8:6）。
![](http://images.cnitblog.com/blog/560322/201309/08125823-7d6d4efda15147d19344fc3ad889e8e5.jpg)
点击OK，即完成差分对的定义。
线宽及线距影响差分线的阻抗，其值可由Polar SI8000软件粗略估算一下，如下图所示，对于阻抗要求高的可与PCB厂家沟通确定。
![](http://images.cnitblog.com/blog/560322/201309/08125836-208319adfa88493193e2310558623ece.jpg)
（4）选择其中的一个引脚，按F3或单击工具栏上 图标，即可开始差分对布线（与常规布线一致），如下图所示。
![](http://images.cnitblog.com/blog/560322/201309/08125850-87d3fd30699f46eeb72e7da6b9b0121f.jpg)
（5）在终端处需要分开布线，右击后选择弹出菜单中的Route Separately，如下图所示。
![](http://images.cnitblog.com/blog/560322/201309/08125903-e19a16d3806c4e2cb02662348695eb9a.jpg)
（6）按如上操作后，按常规走线完成即可，如下图所示。
![](http://images.cnitblog.com/blog/560322/201309/08125915-e4780273ffe74600a7155e91a97dd002.jpg)
至此已经完成了差分线的走线，有些读者可能会见过一些如DDR*的板子，差分线也走了蛇形线，其实将PADS PCB功能使用技巧（NO.001与NO.002）两个结合起来是很容易做到的，这里就不赘述了。
