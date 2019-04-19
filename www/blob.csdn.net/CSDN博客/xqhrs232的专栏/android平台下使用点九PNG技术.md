# android平台下使用点九PNG技术 - xqhrs232的专栏 - CSDN博客
2012年04月20日 14:58:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1123
原文地址::[http://www.yixieshi.com/ucd/9142.html](http://www.yixieshi.com/ucd/9142.html)
相关网帖
1.[Android学习系列(4)--App自适应draw9patch不失真背景](http://www.cnblogs.com/qianxudetianxia/archive/2011/04/17/2017591.html) ----[http://www.cnblogs.com/qianxudetianxia/archive/2011/04/17/2017591.html](http://www.cnblogs.com/qianxudetianxia/archive/2011/04/17/2017591.html)
2.android 9patch  ----[http://meiyitianabc.blog.163.com/blog/static/10502212720115354948909/](http://meiyitianabc.blog.163.com/blog/static/10502212720115354948909/)
3.android-制作自定义背景Button按钮、自定义形状Button,以及draw9patch工具使用----[http://hi.baidu.com/tzq668766/blog/item/d4e4c90210103fc8267fb5d2.html](http://hi.baidu.com/tzq668766/blog/item/d4e4c90210103fc8267fb5d2.html)
“点九”是andriod平台的应用软件开发里的一种特殊的图片形式，文件扩展名为：.9.png 互联网的一些事
　　智能手机中有自动横屏的功能,同一幅界面会在随着手机(或平板电脑)中的方向传感器的参数不同而改变显示的方向,在界面改变方向后,界面上的图形会因为长宽的变化而产生拉伸,造成图形的失真变形。
一些事
　　我们都知道android平台有多种不同的分辨率，很多控件的切图文件在被放大拉伸后，边角会模糊失真。 
互联网的一些事
　　OK，在android平台下使用点九PNG技术，可以将图片横向和纵向同时进行拉伸，以实现在多分辨率下的完美显示效果。 
yixieshi
**　　【普通拉伸和点九拉伸效果对比】**yixieshi
![](http://www.yixieshi.com/uploads/allimg/110908/020RQB7-0.jpg)
　　对比很明显，使用点九后，仍能保留图像的渐变质感，和圆角的精细度。 
yixieshi
　　从中我们也可以理解为什么叫“点九PNG”，其实相当于把一张png图分成了9个部分(九宫格)，分别为4个角，4条边，以及一个中间区域，4个角是不做拉升的，所以还能一直保持圆角的清晰状态，而2条水 平边和垂直边分别只做水平和垂直拉伸，所以不会出现边会被拉粗的情况，只有中间用黑线指定的区域做拉伸。结果是图片不会走样
互联网的一些事
**　　二.“点九”的制作方法**互联网的一些事
　　方法1：使用 “draw9patch”工具绘制，流程如下 
一些事
![](http://www.yixieshi.com/uploads/allimg/110908/020RQ532-1.jpg)
**　　1. 安装工具**
yixieshi
　　首先你需要给自己的电脑安装上java于系统的默认目录下。没有安装java的同学可以在百度搜索：jdk-6u20-windows-i586，安装包大小80M左右。
一些事
　　然后使用andriod模拟器—android-sdk-windows，打开SDK/tools目录下的“draw9patch.bat”文件，出现载入窗口：
互联网的一些事
![](http://www.yixieshi.com/uploads/allimg/110908/020RV249-2.jpg)
**　　2.导入并编辑**yixieshi
　　将png图片拖拽到该窗口中 
yixieshi
　　如下图，自动进入编辑界面。图中介绍了每个区域的内容及功能注释。 yixieshi
![](http://www.yixieshi.com/uploads/allimg/110908/020RVZ2-3.jpg)
　　预览右侧的视图发现，图片的边缘处于普通拉伸状态。 一些事
　　现在我们在图片边缘点击左键，绘制出黑线，即图片需要被拉伸的部分。如下图，对4条黑线做了注释。 
yixieshi
![](http://www.yixieshi.com/uploads/allimg/110908/020RW320-4.jpg)
　　如果失误多绘的部分，可按住shift键的同时点击鼠标左键擦除)。 一些事
　　如图所见，三种拉伸结果均已完美显示，已实现我们想要的拉伸效果，假设这是一个有显示文字的窗体，那么文字显示的区域，程序也会控制在黑线对应范围。 
互联网的一些事
![](http://www.yixieshi.com/uploads/allimg/110908/020RU3b-5.jpg)
　　以下这些图片，包括异性(非规则图形)图片，也可以通过点九PNG实现横纵向的自然拉伸。 互联网的一些事
![](http://www.yixieshi.com/uploads/allimg/110908/020RWJ8-6.jpg)![](http://www.yixieshi.com/uploads/allimg/110908/020RWS1-7.jpg)
　　【draw9patch.bat其他功能说明】 
yixieshi
　　② Show lock：显示不可绘区域 一些事
　　② Show patches：预览这个绘图区中的可延伸宫格(粉红色代表一个可延伸区域) 互联网的一些事
　　③ Show patches：预览视图中的高亮区域(紫色区域) 一些事
　　④ Show bad patches：在宫格区域四周增加一个红色边界，这可能会在图像被延伸时产生人工痕迹。如果你消除所有的坏宫格，延伸视图的视觉一致性将得到维护。
一些事
**　　3.保存和输出**互联网的一些事
　　点击左上file- save，保存文件，自动生成一张后缀名为“*.9.png”格式的图片，图片上下左右各增加了1px的黑线。 
一些事
　　方法2：直接使用 PS等平面工具绘制，流程图如下 yixieshi
![](http://www.yixieshi.com/uploads/allimg/110908/020RR003-8.jpg)
　　如流程图所示，相对与方法1，只需2个步骤就可得到.9.png图片，具体步骤为： 互联网的一些事
　　1. 确定切图后直接改变图片的画布大小， 
互联网的一些事
　　2. 手动将上下左右各增加1px 
互联网的一些事
　　3. 使用铅笔工具，手动绘制拉伸区域，色值必须为黑色(#000000)。 一些事
　　4. 存储为web所用格式，选择png-24，储存时手动将后缀名改为.9.png 一些事
　　不过这种方法的缺点是不能实时预览，判断并测试拉伸区域的准确性。 一些事
　　使用此方法需要注意以下2点: 
一些事
　　1. 手绘的黑线拉伸区必须是#000000，透明度100%，并且图像四边不能出现半透明像素; 
一些事
　　2. 你的.9.png必须绘有拉伸区域的黑线; 一些事
　　否则，图片不会通过android系统编译，导致程序报错。还有，有同学疑惑解压缩apk文件后，.9.png图片里的黑线怎么没了? 
一些事
　　那是因为andriod程序在把文件打包成apk的时候，程序会自动把*.9.png图片边缘的黑线去掉，所以解压缩apk后看到的.9.png文件是没有黑线的。
yixieshi
**　　三.使用“点九”的意义**互联网的一些事
　　关于下图，经过测试发现使用普通png的显示效果出现明显的变色横纹。而.9.png图片的显示效果明显优于普通png。 
互联网的一些事
![](http://www.yixieshi.com/uploads/allimg/110908/020RW501-9.jpg)
　　使用.9.png格式后，横纹问题基本已解决。因为对于.9.png图片，android系统程序有对其优化的算法。 
一些事
　　由于android手机屏幕的材质质量差距大。很多屏幕不支持16位以上的颜色显示。 一些事
　　所以渲染后结果出现丢失颜色，故造成横纹显示。 
互联网的一些事
　　经与多款android手机对比后发现，屏幕越次的手机横纹越明显。 
一些事
　　而使用了*.9.png图片技术后，只需要采用一套界面切图去适配不同的分辨率，而且大幅减少安装包的大小。而且这样程序不需要专门做处理的就可以实现其拉伸，也减少了代码量和开发工作量。
互联网的一些事
　　相信每个人android平台的切图工作，会有不同的技巧和心得，非常欢迎大家能在评论中留下自己的个人经验及心得。相互交流会让我们的工作模式更灵活，更高效，同时带给大家更多优质的移动端应用：)
互联网的一些事
　　转载请注明出自”百度MUX” 
一些事
本文链接：[http://www.yixieshi.com/ucd/9142.html](http://www.yixieshi.com/ucd/9142.html)
关键字：[交互设计](http://www.yixieshi.com/tags/%BD%BB%BB%A5%C9%E8%BC%C6.html)|[视觉设计](http://www.yixieshi.com/tags/%CA%D3%BE%F5%C9%E8%BC%C6.html)|[图片拉伸](http://www.yixieshi.com/tags/%CD%BC%C6%AC%C0%AD%C9%EC.html)|
本文版权归作者本人所有，转载请注明原文出处。

