# OpenCV集成到Qt的尝试-zximage - xiahouzuoxin - CSDN博客





2015年01月14日 19:19:50[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：2702








这一个多月的时间一直忙着写程序，做论文，都没来得及更新博客，当时定下的伟大目标——每周一篇——就这样一溜烟的随着时间的缝隙划过，又留下我低徊忏悔。这是2015年的第一篇博文，希望自己继续坚持下去。回头看看之前写得博文，回忆渐渐涌上心头啊……

回顾了下之前的OpenCV博文，这次用Qt简单做了个界面程序（且给个名字zximage），关于在Qt中集成OpenCV在[搭建Qt界面的OpenCV开发环境](http://xiahouzuoxin.github.io/notes/html/%E6%90%AD%E5%BB%BAQt%E7%95%8C%E9%9D%A2%E7%9A%84OpenCV%E5%BC%80%E5%8F%91%E7%8E%AF%E5%A2%83.html)一文中详细的讨论过了。我在做zximage之前C++不熟，Qt基本上0基础，这种滥竽充数的作品按理是拿不上台面的，但虚荣心作怪，在初学者面前显摆显摆还是可以的，这里就展示展示吧，也许将来哪天回过头来看看（明年毕业的时候回来看这些幼稚的东西又会是什么感受呢？）

一开始是主界面，菜单栏做得很简单，就File,Tools,About，图片载入和保存对话框那是必须的，都在File菜单下，

![界面介绍](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E7%95%8C%E9%9D%A2%E4%BB%8B%E7%BB%8D.png)
界面介绍


Tools中是一些图像处理的算法（大部分都来自OpenCV），选择算法后都会弹出对话框，用于设置算法相关参数。下面就用Blur滤波器来进行磨皮的美化操作，

![滤波器的美化——磨皮](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E6%BB%A4%E6%B3%A2%E5%99%A8%E7%9A%84%E7%BE%8E%E5%8C%96%E2%80%94%E2%80%94%E7%A3%A8%E7%9A%AE.png)
滤波器的美化——磨皮


![幂次变换调对比度](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E5%B9%82%E6%AC%A1%E5%8F%98%E6%8D%A2%E8%B0%83%E5%AF%B9%E6%AF%94%E5%BA%A6.png)
幂次变换调对比度


图像中边缘检测很常见，zximage当然给加上了，

![边缘检测](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E8%BE%B9%E7%BC%98%E6%A3%80%E6%B5%8B.png)
边缘检测


Hough变换检测道路边界，

![Hough直线检测](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/Hough%E7%9B%B4%E7%BA%BF%E6%A3%80%E6%B5%8B.png)
Hough直线检测


形态学操作在OpenCV的图像处理篇中也用过，

![形态学操作](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E5%BD%A2%E6%80%81%E5%AD%A6%E6%93%8D%E4%BD%9C.png)
形态学操作


直方图均衡化在图像处理中应用还是很普遍的，优点就是能自适应——不用设置参数，

![直方图及其均衡化](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E7%9B%B4%E6%96%B9%E5%9B%BE%E5%8F%8A%E5%85%B6%E5%9D%87%E8%A1%A1%E5%8C%96.png)
直方图及其均衡化


人脸检测简直碉堡了，设计这些算法的人吃的都是啥啊，下面是Haar算子进行的人脸检测结果，

![人脸检测](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E4%BA%BA%E8%84%B8%E6%A3%80%E6%B5%8B.png)
人脸检测


为了证明这个软件的摄像头功能确实是可用的，我拿着摄像头对着屏幕的运行的zximage拍。瞬间大吃一惊，这是什么！！怎么这么多窗口！！！好像发明避孕套的人发现了避孕套一样，很是惊喜，我似乎看到了鸡生蛋蛋生鸡的问题。。。。拍下软件窗口，摄像头界面显示窗口，又将显示额窗口拍下，又显示。。。所以中心出现极亮的光斑。。。

![软件自拍-鸡生蛋蛋生鸡](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E8%BD%AF%E4%BB%B6%E8%87%AA%E6%8B%8D-%E9%B8%A1%E7%94%9F%E8%9B%8B%E8%9B%8B%E7%94%9F%E9%B8%A1.png)
软件自拍-鸡生蛋蛋生鸡


然后我把这个现象给实验室的师兄看，顺带给帅锅师兄来了一张，

![摄像头拍到的帅锅师兄](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/%E6%91%84%E5%83%8F%E5%A4%B4%E6%8B%8D%E5%88%B0%E7%9A%84%E5%B8%85%E9%94%85%E5%B8%88%E5%85%84.png)
摄像头拍到的帅锅师兄


最后来一张zximage的自拍人脸检测，这个是手机拍下的屏幕照片，屏幕太亮没办法，

![zximage摄像头自拍人脸识别](http://xiahouzuoxin.github.io/notes/images/OpenCV%E9%9B%86%E6%88%90%E5%88%B0Qt%E7%9A%84%E5%B0%9D%E8%AF%95-zximage/zximage%E6%91%84%E5%83%8F%E5%A4%B4%E8%87%AA%E6%8B%8D%E4%BA%BA%E8%84%B8%E8%AF%86%E5%88%AB.png)
zximage摄像头自拍人脸识别


## 源码

源码下载请到我的Github： [https://github.com/xiahouzuoxin/opencv_study](https://github.com/xiahouzuoxin/opencv_study) ,那里不仅包括zximage的源码，还包括我的OpenCV系列所有博文的源码，OpenCV系列博文链接可以在 [http://xiahouzuoxin.github.io/notes/README.html](http://xiahouzuoxin.github.io/notes/README.html) 找到。



