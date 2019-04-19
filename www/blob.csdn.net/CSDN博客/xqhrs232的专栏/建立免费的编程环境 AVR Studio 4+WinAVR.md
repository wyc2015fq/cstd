# 建立免费的编程环境 AVR Studio 4+WinAVR - xqhrs232的专栏 - CSDN博客
2019年03月26日 15:26:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：40
原文地址::[https://blog.csdn.net/scliu12345/article/details/79869058](https://blog.csdn.net/scliu12345/article/details/79869058)
相关文章
1、avrstudio+winavr开发环境的建立和使用----[https://wenku.baidu.com/view/225b571010a6f524ccbf85bc.html](https://wenku.baidu.com/view/225b571010a6f524ccbf85bc.html)
转自：https://blog.csdn.net/dearwind153/article/details/7414187
从网上下载AVR Studio 4和 WinAVR 并安装.
AVR Studio 4.18 (build 684)
下载地址:http://www.atmel.com/dyn/resources/prod_documents/AvrStudio4Setup.exe
（http://mycomsats.com/download-atmel-avr-studio-4-free/）
在从官方网站下载AVR Studio 4.18 SP2补丁
WinAVR-20100110(开源而且免费) 它支持AVR AVR32
下载地址:http://sourceforge.net/projects/winavr/files/
在下一教程中将介绍简单学习板USB AVR ISP mkII编程器的制作.
打开AVR Studio 4 在弹出新建工程向导窗口中点击[New Project]
![](https://img-my.csdn.net/uploads/201203/31/1333156646_8852.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156650_8699.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156657_1343.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156754_3697.JPG)
![](https://img-my.csdn.net/uploads/201203/31/1333156670_1381.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156675_9397.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156680_5404.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156685_5854.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156862_4452.JPG)
![](https://img-my.csdn.net/uploads/201203/31/1333156693_5155.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156699_7476.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156707_4605.jpg)
![](https://img-my.csdn.net/uploads/201203/31/1333156714_2841.jpg)
转自：http://pobo10.blog.163.com/blog/static/4556386820144183143270/
目前在做AVR的项目，做些备忘。
1.使用AVR GCC做为编译器
![WinAVR+AVR Studio编译环境配置 - 枫 - 枫的博客](http://img1.ph.126.net/70Xla47B6FQJus6fSgo2XA==/6608846635701147067.jpg)
 2.选择仿真调试平台 
![WinAVR+AVR Studio编译环境配置 - 枫 - 枫的博客](http://img1.ph.126.net/xHZaX2MAYQGKZ6qGzr-54g==/1072982611321424581.jpg)
3.选择Device，设置Frequency，代码优化选项Optimization，输出hex文件 
![WinAVR+AVR Studio编译环境配置 - 枫 - 枫的博客](http://img0.ph.126.net/WM9WoUMPkmyjYdDsP5YITA==/3677470570825018056.jpg)
4.设置包含文件（头文件）路径 
![WinAVR+AVR Studio编译环境配置 - 枫 - 枫的博客](http://img0.ph.126.net/Mui9lbuJNmcsX8FP3L6NkQ==/1415256183001944315.jpg)
5.设置库文件路径 
![WinAVR+AVR Studio编译环境配置 - 枫 - 枫的博客](http://img1.ph.126.net/2i_IwhOmcMHRk76n1VAZxQ==/6608836740096497038.jpg)
6.设置工具链路径（不使用AVR Toolchain） 
![WinAVR+AVR Studio编译环境配置 - 枫 - 枫的博客](http://img2.ph.126.net/AuUhi8dCIIT5DxG59R9f5A==/3309582776264284826.jpg)
 如此便可使用winavr作为c编译器，不会出现avr-objcopy: '****.elf': No such file的错误。
