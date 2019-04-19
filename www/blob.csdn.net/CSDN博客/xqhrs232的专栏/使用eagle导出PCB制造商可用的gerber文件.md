# 使用eagle导出PCB制造商可用的gerber文件 - xqhrs232的专栏 - CSDN博客
2015年02月06日 15:31:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4179
原文地址::[http://blog.chinaunix.net/uid-25652313-id-3244881.html](http://blog.chinaunix.net/uid-25652313-id-3244881.html)
相关文章
1、在Eagle中生成Gerber文件----[http://wenku.baidu.com/link?url=ixtubAZdU0veribWxhIzrrCMv0utpMGGDGJecekromYvO4Xykskghz_W42K1SgKOWUpYvoPTcECk0NxMTUVpqSe8Z75nW10J6446TSRaK5m](http://wenku.baidu.com/link?url=ixtubAZdU0veribWxhIzrrCMv0utpMGGDGJecekromYvO4Xykskghz_W42K1SgKOWUpYvoPTcECk0NxMTUVpqSe8Z75nW10J6446TSRaK5m)
2、Eagle学习笔记---Eagle 中PCB导出Gerber文件操作----[http://cooldianzi.blog.163.com/blog/static/671115382010230103148387/](http://cooldianzi.blog.163.com/blog/static/671115382010230103148387/)
3、Eagle5.11 PCB生成Gerber方法----[http://wenku.baidu.com/link?url=8TlDneSFBD_gAq8fLlZXCc8IFydC4dmwTmF35dqC1hoMVhHf3c1zNuPRoJFQCzc9-lgX0hGsYF-zk2tsD7r9iC2THCGxpgf4_-JGYNVW9oa](http://wenku.baidu.com/link?url=8TlDneSFBD_gAq8fLlZXCc8IFydC4dmwTmF35dqC1hoMVhHf3c1zNuPRoJFQCzc9-lgX0hGsYF-zk2tsD7r9iC2THCGxpgf4_-JGYNVW9oa)
4、[EAGLE的PCB文件转成PROTEL格式PCB文件](http://blog.chinaunix.net/uid-25652313-id-3238701.html)----[http://blog.chinaunix.net/uid-25652313-id-3238701.html](http://blog.chinaunix.net/uid-25652313-id-3238701.html)
5、[使用eagle导出国内PCB制板商可用的gerber文件](http://blog.chinaunix.net/uid-21198646-id-3420202.html) ----[http://blog.chinaunix.net/uid-21198646-id-3420202.html](http://blog.chinaunix.net/uid-21198646-id-3420202.html)
6、[Eagle
 输出 Gerber 文件，并在 CAM350 中正确导入的方法](http://www.geek-workshop.com/thread-286-1-1.html)** ----[http://www.geek-workshop.com/thread-286-1-1.html](http://www.geek-workshop.com/thread-286-1-1.html)**
7、**关于eagle 生成gerber 底层丝印问题**** ----[http://www.amobbs.com/thread-4385229-1-1.html](http://www.amobbs.com/thread-4385229-1-1.html)**
8、Eagle输出Gerber文件，并在CAM350 中正确导入的方法----[http://www.geek-workshop.com/thread-286-1-1.html](http://www.geek-workshop.com/thread-286-1-1.html)
eagle是一款在国外广泛流行的电路板绘制CAD软件，与protel99se类似。但功能远比protel99se强大。最近自己的项目需要参考国外开源网站的一些资料，可在做电路板的时候发现，国内的PCB厂商绝大多数并不支持eagle直接绘制的.brd扩展文件（相当于protel中的.PCB文件），于是通过自身实践，记录下使用eagle的.brd扩展文件，制作PCB制造商可使用的gerber文件。
1.打开已经绘制好的.brd文件，如下图所示。在红色的画圈的地方输入 run
 drillcfg ，回车运行。
![](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810118Bs49.jpg)
2.运行这个ulp文件后，得到如下图的对话框，选择mm或者inch（因为无论哪一种，最后都是生成inch单位的文件）。
[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810118Bs49.jpg)![](http://blog.chinaunix.net/attachment/201206/16/25652313_13398101384hN7.jpg)
3.确定后到这个界面，直接OK
[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810118Bs49.jpg)[](http://blog.chinaunix.net/attachment/201206/16/25652313_13398101384hN7.jpg)![](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810207g1Dt.jpg)
4.选择保存的位置后（我保存在与.sch原理图文件及.brd的PCB文件一个文件夹），可以看到多出了.drl文件。
[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810118Bs49.jpg)[](http://blog.chinaunix.net/attachment/201206/16/25652313_13398101384hN7.jpg)[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810207g1Dt.jpg)![](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810226Ew5c.jpg)
5.然后我们运行两个cam文件。点击图中的CAM标志。
[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810118Bs49.jpg)[](http://blog.chinaunix.net/attachment/201206/16/25652313_13398101384hN7.jpg)[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810207g1Dt.jpg)[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810226Ew5c.jpg)![](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810255FGKy.jpg)
6.选择“文件”>>“打开”>>“作业”
[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810310ByBw.jpg)
![](http://blog.chinaunix.net/attachment/201206/16/25652313_1339813900CffH.jpg)
7.选择gerb274x-4layer.cam，注意不要修改默认的层设置。因为如果不注意，可能会把丝印层和top层重叠，导致短路的发生。按照默认设置，选择“处理作业”。界面如下图所示。
![](http://blog.chinaunix.net/attachment/201206/16/25652313_13398103161aZL.jpg)
![](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810325s217.jpg)
8.完成后再按照同样方式打开excellon.cam，并“处理作业”。此步骤是生成钻孔文件：
![](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810335Jf18.jpg)[](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810325s217.jpg)
9.把这些上述所有步骤产生的文件打包发送给PCB产商就可以了。各层的含义可以参考我的其他博文，可以使用CAM350查看生成的gerber文件（如下图所示）。
![](http://blog.chinaunix.net/attachment/201206/16/25652313_1339810350BSGY.jpg)
//================================================================================================================================
//备注::
1》
这里介绍下各个文件在CircuitCAM 5.2.713 软件中对应的层。
CMP  ——TopLayer         顶层
SOL  ——BottomLayer      底层
STC  ——SolderMaskTop    顶层阻焊层
STS  ——SolderMaskBottom 底层阻焊层
PLC  ——SilkScreenTop     顶层丝印层
PLS  ——BoardOutline      边框
DRL  ——Eagle 4.15 dri Exc Inch Inch24AbsTrail.txt (NCDrill)        钻头列表
TXT  ——DrillPlated        孔坐标文件
2》GERBER文件一般都是gerb274x格式的！！！
