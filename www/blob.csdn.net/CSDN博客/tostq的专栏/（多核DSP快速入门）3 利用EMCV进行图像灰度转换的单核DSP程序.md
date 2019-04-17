# （多核DSP快速入门）3.利用EMCV进行图像灰度转换的单核DSP程序 - tostq的专栏 - CSDN博客





2016年04月25日 23:51:18[tostq](https://me.csdn.net/tostq)阅读数：7056
所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)









原创文章
转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)



教程目录：[http://blog.csdn.net/tostq/article/details/51245979](http://blog.csdn.net/tostq/article/details/51245979)

在CCS中应用EMCV有两种方法：

（1）通过导入EMCV静态库的方法

（2）直接将EMCV的源代码导入项目文件中，虽然EMCV的源文件是C++格式的，而C++基本兼容C，而项目文件一般是用C写的，所以还需要将.c文件改成.cpp就可以了。






**一、在新建空项目后，导入静态链接库和相关头文件**

（1）这里的头文件指的是.h文件，在当前项目中新建一个Include文件夹，将EMCV内的所有头文件复制到其中，这里我们也可以只选择我们需要的头文件导入，同时为了保证我们的头文件被搜索到，需要在项目属性的Build的Include选项中，将这个Include文件夹导入。否则会出现error:
 could not open source file "XXX" ![](https://img-blog.csdn.net/20160425234025482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20160425234048299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）将EMCV静态库文件复制到当前文件夹中，

（3）导入链接命令文件C6678.cmd（第二节有具体介绍）

（4）新建空白源文件rgb2gray.cpp（这里虽然是Cpp文件，但只能用C语言的语法）

        此时项目文件如下

![](https://img-blog.csdn.net/20160425234108607?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**二、编写源程序**

通过源程序我们可以看到，这里的源程序同原来的OpenCV代码基本上是一致，并没有进行额外修改了，但点击编译后出现如下错误：undefined symbol remain，这里说明灰度转换函数cvCvtColor没有定义，说明EMCV没有编写cvCvtColor函数。

![](https://img-blog.csdn.net/20160425234147655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们重新编写一个灰度转换函数CvtColor，点击小锤子完成编译，查看有没有错误，如果是完全按照上面的步骤，应该是没有问题的。

![](https://img-blog.csdn.net/20160425234214784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**三、仿真调试**

（1）新建一个C6678 Device Functional Simulator, Little Endian的目标配置文件

（2）将测试文件in.rgb复制到项目中，这里的.rgb文件是直接保存图像像素灰度信息的文件，以一个彩色图像文件为例，每个通道都是8位大小，红黄蓝三色依次排列成一个24位大小的像素。

（3）点击调试，此时会弹出核选择对话框，因为我们这里的程序是单核的，所以只需要选择一个核就可以了

（4）点击运行后，我们发现出现如下结果malloc buf failed

![](https://img-blog.csdn.net/20160425234300376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这说明了动态堆分配失败了，堆的分配是在链接命令文件.cmd中设置，我们打开C6678.cmd文件：

![](https://img-blog.csdn.net/20160425234330566?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们发现我们的堆只有0x8000，只有32kb大小，而我们分配的Buf有320*240*3个字节，完全不够，所以我们需要增加堆的容量：

![](https://img-blog.csdn.net/20160425234346252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


再点击运行，此时Buf成功分配了！

![](https://img-blog.csdn.net/20160425234405144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


为了方便调试，防止程序运行完后直接退出，我们可以在主体函数完成后，增加几个断点。

![](https://img-blog.csdn.net/20160425234437191?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




**四、使用调试工具**

（1）变量窗口的使用

![](https://img-blog.csdn.net/20160425234509207?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       还可以通过Expressions窗口输入变量式子

![](https://img-blog.csdn.net/20160425234538568?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）内存浏览器Memory Brower，输入变量名或变量地址就可以查看其内存

![](https://img-blog.csdn.net/20160425234608459?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





       或者通过Load Memory将电脑硬盘内的数据导入到内存中

![](https://img-blog.csdn.net/20160425234705895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）图像分析器Image Analyzer，可以在Tools选项中打开

        首先在Image Analyzer 中右击选择RGB

![](https://img-blog.csdn.net/20160425234754270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


然后在Properties中做如下设置，因为inBuf中已经存入了图像的数据，所以这里可以直接打开inBuf地址0xc000008，如果我们只想查看当前电脑硬盘内的图像，可以选择先在Memory
 Brower中将图像导入到内存中

![](https://img-blog.csdn.net/20160425234822942?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在Image Analyzer中右击刷新，可以看到图像数据

![](https://img-blog.csdn.net/20160425234847505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


OutBuf中的灰度图像的Properties中的设置

![](https://img-blog.csdn.net/20160425234909802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20160425234925365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






