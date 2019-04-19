# iOS开发之——从零开始完成页面切换形变动画 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月17日 13:13:55[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：625
# 前言
某天我接到了UI发给我的两张图：
![](http://upload-images.jianshu.io/upload_images/2478094-bca4484347b5bd51.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
需求图.png
看到图的时候我一脸懵逼，显然我需要做一个页面切换的指示动画。老实说，从大三暑假开始做iOS开发也一年有余了，但是遇到复杂动画总是唯恐避之不及，只做过一些简单的位移动画。大脑中的舒适区总是让我下意识避开麻烦的问题选择简单的解决方式。UI很善解人意得告诉我，你可以先用图片替代动画，以后有空慢慢完善。我突然不知哪里来的底气拍了拍胸脯：“没问题，包在我身上”。装出去的b泼出去的水，没办法，我只好下定决心趁此机会好好钻研一下形变动画。（就在写这篇文章的时候，UI看到了我最终的成果并点赞，顿时成就感爆棚）。
##### 最终成果如图：
![](http://upload-images.jianshu.io/upload_images/2478094-c1537d49e202333a.gif?imageMogr2/auto-orient/strip)
demo.gif
###### 具体代码详见：[https://github.com/lfny2580832/NYTubeAnimation](https://github.com/lfny2580832/NYTubeAnimation)
# 需求分析
打定主意要搞定这个动画，我首先在脑海中一遍遍模拟了整体效果。想象这是一个封闭光滑的管道，管道两端是两个可伸缩变形的白块，当点击下一步滑动到第二个页面时，管道左边的白块被一股向右的力推动，逐步压缩进管道中，最后从右边释放出来。这种感觉很抽象，只能隐隐约约想象出大概的效果。我在纸上一遍遍得模拟每一个细节，开始进行简化和分解。
### 简化
如果只看设计图，显然中间管道过窄，白块无法通过。于是我尝试着将中间管道变宽，并用keynote利用圆弧与矩形画出了简化图：
![](http://upload-images.jianshu.io/upload_images/2478094-9e1a4995efb4af2a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
需求简化图.png
思路是不是立即清晰很多！实际上简化的过程我花了很长时间。一开始我并没有用圆形，而是用贝塞尔曲线来拟合图形，但是在拟合过程后中遇到了重重困难。比如要根据角度来确定某段弧线中贝塞尔点的控制点的长度（一小段曲线中至少需要计算五个点的坐标，计算非常复杂，且拟合效果不是很好），以及多个贝塞尔曲线UIBeizerPath闭合时各种错位等等问题。多次尝试无果后，我退而求其次，使用了UIBeizerPath 自带圆弧方法来构造整体图形以及动画。这段时间我花了整整两天。
简化图画出来之后，心中有了思路，即用UIBeizerPath拟合图形构造CAShapeLayer，然后使用CADisplayLink完成帧动画。对CAShapeLayer和CADisplayLink做动画不太熟悉的同学可以参考我之前的一个水波动画，github地址：[https://github.com/lfny2580832/NYWaterWave](https://github.com/lfny2580832/NYWaterWave)
![](http://upload-images.jianshu.io/upload_images/2478094-7bab0b3131b1052b.gif?imageMogr2/auto-orient/strip)
waterwave.gif
### 分解
看着上面的简化图，我又懵逼了。这丫该怎么动起来？而且要想让效果看起来自然流畅，在形状开始压缩和在管道中移动速度肯定是不一样的，由管道的狭窄程度决定，要计算的话难度太大，于是我便手动指定这两块区域的速度（所有参数都可以在代码中指定）。经过简单的分解后我画出了如下的参考图，并标注了参数：
![](http://upload-images.jianshu.io/upload_images/2478094-c7802212320a21fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
整体分解图.png
看着上图脑海里有了些思路。我不可能只用一个CAShapelayer就做完这一系列的动画，当然只能将其分解成各个部分，分别进行动画。想象每个部分在整个动画中的位置及大小，当每个部分都做好之后，再完美拼接起来，整个动画不就完成了吗？
# 实现
### 属性与实例变量
下图属性与实例变量位置及命名只是个人习惯，方便开发时自己查看，其中所有点都是根据上面的参考图来命名的，大家可以对照查看：
![](http://upload-images.jianshu.io/upload_images/2478094-62dd4821089c7eb8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
变量.png
看起来一脸懵逼？没关系，我会将每个分解出来的模块完整动画向大家展示出来。由于代码有点多有点复杂，就直接以图片形式像大家展示。这其中大部分都只是很多简单的动画，但将他们组合起来就不一样啦！
### 速度控制点—dynamic_Q_d和dynamic_Q2_d
这两个点来控制在不同阶段的速度，只需改变自增量即可，逻辑稍稍复杂。
![](http://upload-images.jianshu.io/upload_images/2478094-0dbb21cbf99e582d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
dynamic_Q_d.png
![](http://upload-images.jianshu.io/upload_images/2478094-2ea7879e37321487.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
dynamic_Q2_d.png
### 左边的圆弧— leftSemiShape
![](http://upload-images.jianshu.io/upload_images/2478094-a9bff92cbba0f9b4.gif?imageMogr2/auto-orient/strip)
leftSemiShape.gif
![](http://upload-images.jianshu.io/upload_images/2478094-1befba29cc5bdf14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
leftSemiShape.png
### 主体矩形区域—maintubeShape
![](http://upload-images.jianshu.io/upload_images/2478094-45793b0accbb5b08.gif?imageMogr2/auto-orient/strip)
mainTubeShape.gif
![](http://upload-images.jianshu.io/upload_images/2478094-dc23f0b8472cffa0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
mainTubeShape.png
### 火山形状—volcanoShape
![](http://upload-images.jianshu.io/upload_images/2478094-a76ad44f8da20396.gif?imageMogr2/auto-orient/strip)
volcanoShape.gif
火山形状也是整个动画中最复杂的一部分，需要一些简单的计算，下面附上计算使用的参考图：
![](http://upload-images.jianshu.io/upload_images/2478094-39455089151365da.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
火山形状参考图.png
我们可以根据Q点移动的距离（dynamic_Q2_d）计算出b夹角，再通过UIBeizerPath画出相应的形状：
![](http://upload-images.jianshu.io/upload_images/2478094-bdddd89cfa6080cf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
volcanoShape.png
### 白块右方圆形—rightCircleShape
![](http://upload-images.jianshu.io/upload_images/2478094-b614100d8e35452a.gif?imageMogr2/auto-orient/strip)
rightCircleShape.gif
![](http://upload-images.jianshu.io/upload_images/2478094-8b32f7c2e6ebb0db.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
rightCircleShape.png
### 尾部圆形形状—tailCircleShape
![](http://upload-images.jianshu.io/upload_images/2478094-170937cc63cd6d13.gif?imageMogr2/auto-orient/strip)
tailCircleShape.gif
![](http://upload-images.jianshu.io/upload_images/2478094-e46e62afd6d5dd14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
tailCircleShape.png
### 管道形状—tubeShape
![](http://upload-images.jianshu.io/upload_images/2478094-cfef568e2bdd6444.gif?imageMogr2/auto-orient/strip)
tubeShape.gif
![](http://upload-images.jianshu.io/upload_images/2478094-e955f04354127b3a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
tubeShape.png
### 背景形状—wholeShape
![](http://upload-images.jianshu.io/upload_images/2478094-2959933d04a768c2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
wholeShape.png
背景形状只需要将上方所有图形拼合起来并扩大一圈即可，在此就不附代码了。
### 拼合
![](http://upload-images.jianshu.io/upload_images/2478094-e3d936df894df18f.gif?imageMogr2/auto-orient/strip)
整体效果.gif
# 总结
当动画效果做出来后，再将其封装起来，开始事件、完成委托都变得那么简单。写这篇文章记录我的思考过程是想让更多人敢于挑战自己，其实很多东西并没有那么难，认真下去就能完成。如果总是待在自己的舒适区，很难提高自己的水平。主动挑战自己，将一个复杂的问题分解成一个个小目标，然后一一达成，问题就迎刃而解啦!
##### 顺便安利一波我和几个同样年轻的小伙伴一起创建的公众号“萌新码路”，记录一些有趣的问题和解决方案以及萌新码农的日常，欢迎关注！
![](http://upload-images.jianshu.io/upload_images/2478094-2bb9c629595557e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
萌新码路.png
文／牛严（简书作者）
原文链接：http://www.jianshu.com/p/8a569dfd1c4b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
