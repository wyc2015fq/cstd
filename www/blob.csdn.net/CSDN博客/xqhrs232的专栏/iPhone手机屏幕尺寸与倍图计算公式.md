# iPhone手机屏幕尺寸与倍图计算公式 - xqhrs232的专栏 - CSDN博客
2019年03月01日 17:13:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：123
原文地址::[https://blog.csdn.net/jia12216/article/details/82881672](https://blog.csdn.net/jia12216/article/details/82881672)
相关文章
1、iphone关于单倍图和二倍图（导航 背景 变高）----[https://blog.csdn.net/weixin_34116110/article/details/86047697](https://blog.csdn.net/weixin_34116110/article/details/86047697)
2、ios切图（一倍图+二倍图+三倍图）----[https://blog.csdn.net/xysypch/article/details/84396830](https://blog.csdn.net/xysypch/article/details/84396830)
3、[大屏iPhone的适配 +iOS 图片尺寸要求](https://www.cnblogs.com/wj033/p/4267537.html)----[https://www.cnblogs.com/wj033/p/4267537.html](https://www.cnblogs.com/wj033/p/4267537.html)
4、iPhone设备屏幕尺寸及开发图片适配----[https://www.jianshu.com/p/80cd97653e49](https://www.jianshu.com/p/80cd97653e49)
iPhone手机屏幕尺寸一般指：物理像素，ps像素。
iPhone的app现在一般适配2倍图和3倍图。
iphont4s 是2倍图，但是你画一个粗为0.5的线，iphont4s显示不出来，iphont5s却可以看到一个像素（从截图上看到的）的线来。
识别手机机型使用的是几倍图，一般通过这个值来识别：[UIScreen mainScreen].scale。若为1就1倍图（iphone 4/iphone 4s是个例外），若为2就是2倍图，若是3就是3倍图。但是现在iphone 4/iphone 4s都是按照2倍图处理的，并且他们的屏幕截图也是640*960，说明它是2倍图。但是若你在iphone 4/iphone 4s手机上画条0.5像素的线是显示不出来的，但是在iphone 6及更高本本画一条1/([UIScreen mainScreen].scale)（就是0.5或1/3）的线是显示出来的，说明iphone 4/iphone 4s物理分辨率是1像素。
准确的倍图计算公式是：round(屏幕截图宽度/[[UIScreen mainScreen] bounds].size.width)。屏幕截图（上传苹果商店的屏幕快照或者用手机截屏得到的图片）的宽度（或高度）除以物理宽度（或高度，宽度是[[UIScreen mainScreen] bounds].size.width，高度是[[UIScreen mainScreen] bounds].size.height）的四舍五入的整数。iPhone XR的828/375=2.2.08，四舍五入值是2，它的[UIScreen mainScreen].scale也为2（FULL_WIDTH:375.000000, FULL_HEIGHT:812.000000, [UIScreen mainScreen].scale:2.000000），所以它是用2倍图。
下面是开发中用到的iphone,ipod,ipad对应的实际像素点和图片倍率：
iphont4,iphone4s实际像素点：
3.5英寸屏（320*480）
iphone5,iphone 5s,iphone 5se实际像素点：
4英寸屏 （320*568）
iphont6,iphone7,iphone8实际像素点：
4.7英寸屏（375*667）
iphont6 plus,iphone7 plus,iphone8 plus实际像素点：
5.5英寸屏 （414*736）
iphontX若不设置启动图片相当于iPhone8的放大模式（375*667） ，若设置了启动图片才是他的实际像素，实际像素点：
5.8英寸屏（375*812）
ipod4实际像素点：
4英寸屏（320*568）
iphontX若不设置启动图片相当于iPhone8的放大模式（375*667） ，若设置了启动图片才是他的实际像素，实际像素点：
5.8英寸屏（iPhone X，iPhone XS），6.1英寸屏(iPhone XR)，6.5英寸屏（iPhone XS Max）（375*812）
ipod4实际像素点：
4英寸屏（320*568）
所有的Ipad的实际像素点都相同： 768*1024
矢量图的图片倍率
iphont4 2倍图
iphont4s 2倍图
iphont5 2倍图
iphont5s 2倍图
iphont6 2倍图
iphont6 plus 3倍图
iphont7 2倍图
iphont7 plus 3倍图
iphont7s 2倍图
iphont7s plus 3倍图
iphont8 2倍图
iphont8 plus 3倍图
iPhone XR 2倍图
iphontX，iPhone XS， iPhone XS Max 3倍图
ipod4 2倍图
iphone4,iphone5,iphoneX无放大模式。iphone6，7，8的放大模式屏幕像素是：（320*568）。iphone6 plus，7 plus，8 plus的放大模式屏幕像素是：（375*667）。
下面是iphone的ps像素（手机截图尺寸）：
iphont4,iphone4s 像素尺寸：
3.5英寸屏（640*960）
iphone5,iphone 5s,iphone 5se 像素尺寸：
4英寸屏 （640*1136）
iphont6,iphone7,iphone8 像素尺寸：
4.7英寸屏（750*1334）
iphont6 plus,iphone7 plus,iphone8 plus 像素尺寸：
5.5英寸屏 （1242*2208）
iphontX，iPhone XS
5.8英寸屏（1125*2436）
iPhone XR
6.1英寸屏（828*1792）
iPhone XS Max
6.5英寸屏（1242 * 2688）
