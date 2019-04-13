
# 简单VR照片 使用陀螺仪、姿态角（Roll、Pitch、Yaw ）、四元数 - 小小情意 - 博客园






# [简单VR照片 使用陀螺仪、姿态角（Roll、Pitch、Yaw ）、四元数](https://www.cnblogs.com/xiaoxiaoqingyi/p/6932008.html)
最近在做一个类似VR照片的demo，跟全景图片也很像，只是VR照片与全景720度显示，我只做了180度。但我发现他们实现的原理有一丝相似，希望可以给一些想入行AR、VR的朋友一些提示吧。
要想根据用户摇晃手机的行为轨迹展示相应的场景，那必须要使用移动端的陀螺仪、加速器等传感器来做相应的协调。现在的移动端已经提供了很多传感器，你可以根据自己的需要获取相应的数据。
刚开始的时候，我使用传感器提供的姿态角，也称为欧拉角：pitch yaw roll 来显示。这种 姿态角/欧拉角 比较常用在航空上，无人机技术应该也使用到了这个技术点。我用飞机的模型来展示一下这三个角，就一目了然了（不同作者使用不同的坐标系，使用哪种坐标系，个人而定。）：
![](http://img.blog.csdn.net/20170601233358225?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图一
姿态角/欧拉角 在数学上理解起来会有点抽象，我也是稍理解一点。在维基百科上，欧拉角定义为：用来描述刚体在三维欧几里得空间的取向，对于任何参考系，一个刚体的取向，是依照顺序，从这参考系，做三个欧拉角的旋转而设定的。有兴趣了解得深入一点，可以参考(需FQ)：
[https://zh.wikipedia.org/wiki/%E6%AC%A7%E6%8B%89%E8%A7%92](https://zh.wikipedia.org/wiki/%E6%AC%A7%E6%8B%89%E8%A7%92)
我们也可以简单理解这三个角代表什么意思：
1、俯仰角θ（pitch）：围绕Y轴旋转的。
![](http://img.blog.csdn.net/20170601233429064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图二
2、偏航角ψ（yaw）：围绕Z轴旋转的角度。
![](http://img.blog.csdn.net/20170601233454883?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图三
3、滚转角Φ（roll）：围绕X轴旋转的角度。
![](http://img.blog.csdn.net/20170601233517862?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图四
手机的坐标系：
![](http://img.blog.csdn.net/20170601233629478?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
手机定义的坐标系统：X轴是水平且指向右边，Y轴是垂直且指向前方，Z轴指向屏幕的正面正上方。
当手机左右摇摆时（绕 y 轴旋转），得到变化的 滚转角Φ（roll），范围为 (-90 to 90)
当手机前后摇摆时（绕 x 轴旋转），得到变化的 俯仰角θ（pitch），范围为 (-180 to 180)
当手机横屏转换成竖屏或竖屏转换成横屏时（绕 z 轴旋转），即得到变化的 偏航角ψ（yaw）。
当手机旋转得到相应的角度时，我们把屏幕的图片替换成相应角度的图片，这让你感觉身临其境，屏幕里的场景会跟着我们视觉和谐变化，这就是VR大概实现的原理吧。
Android提供了类似这个 姿态角/欧拉角 的数据，但两者有些区别，Android 把 偏航角ψ（yaw）定义为方位角 Azimuth。具体可查看Android官方文档：[https://developer.android.google.cn/reference/android/hardware/SensorEvent.html\#values](https://developer.android.google.cn/reference/android/hardware/SensorEvent.html#values)
接收的传感器类型为：[Sensor.TYPE_ORIENTATION](https://developer.android.google.cn/reference/android/hardware/Sensor.html#TYPE_ORIENTATION)（Android官方已经摈弃，换了其它方法来获取）
使用这种方式去获取的数据有些波动，当拿着手机不动的时候，数据有时变化也比较大，所以后期我改用了 四元数 来处理这个问题。后来听说了一个方案，通过滤波方法，可以把 姿态角/欧拉角 一些波动比较大的数据过滤点， 得到的数据就比较平滑了。（这个方式，我还没有去试，大家可以去了解了解）
使用 “四元数” 也可以实现。四元数理解起来也很抽象：简单的超复数，是复数的不可交换延伸。
我是这么简单理解：我们都知道向量，向量是用来形容一个二维空间的轨迹与方向，形象化地表示为带箭头的线段。四元数则表示为在一个四维空间的轨迹与方向。
来看一个四元数的表现形式：
![](http://img.blog.csdn.net/20170601233955223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
公式一
这是 A 到 B 的四元数表示，由四个元素组成的数组。我主要讲一些实现方法，里面有一些名词的含义，需要大家找其它的资料慢慢参透。
我们再来看一个公式，x、y、z轴的偏移量分别为：
![](http://img.blog.csdn.net/20170601234014248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
公式二
由该公式我们看得出，只要知道一个四元数的值， 我们就能算出 A 到 B 的x、y、z轴的偏移量。
我们从陀螺仪传感器获得的四元数数据，是相对于手机平放在桌面的 xyz 轴的偏移量，我们需要转换成相对上一个位置的偏移量，然后根据偏移量更换相应的图片。
现在我们看一下，怎么获取相对的偏移量：
在平面二维向量中，我们知道向量的运算： 向量AC = 向量AB + 向量BC
![](http://img.blog.csdn.net/20170601234102521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
公式三
那么在四元数中，我们假设从传感器获取到
第一个 四元数为![](http://img.blog.csdn.net/20170601234251381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，
假设A点原点，即手机平放在桌面水平位置上。第二个四元数为：![](http://img.blog.csdn.net/20170601234309221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，
我们需要得到四元数：![](http://img.blog.csdn.net/20170601234325456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这样才能算出B 到C 的相对 xyz 轴相对位移。通过下面公式可以得到![](http://img.blog.csdn.net/20170601234344425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)：
![](http://img.blog.csdn.net/20170601234442911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](http://img.blog.csdn.net/20170601234458349?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
公式四

现在知道![](http://img.blog.csdn.net/20170601234309221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)和![](http://img.blog.csdn.net/20170601234653539?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
由下面公式可以得到：
![](http://img.blog.csdn.net/20170601234714024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
公式五
从A到B的 四元数 转换成 B到A 的四元数公式，只需把第二、第三、第四个元素取负。
知道了![](http://img.blog.csdn.net/20170601234309221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)和![](http://img.blog.csdn.net/20170601234653539?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们用 公式四 得出![](http://img.blog.csdn.net/20170601234344425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，然后通过公式二求出 xyz 轴的偏移量。
四元数 x 轴的偏移量 与 欧拉角的俯仰角θ（pitch）类似，都是指手机竖屏时，左右摇摆的数值。
四元数 y 轴的偏移量 与 欧拉角的滚转角Φ（roll）类似，都是指手机竖屏时，前后摇摆的数值。
四元数 z 轴的偏移量 与 欧拉角的偏航角ψ（yaw）类似，都是指手机竖屏时，左右旋转的数值。
在移动端比较常用到 x、y轴偏移量，就是手机左右摇摆与前后摇摆间的偏移。
我做了一个Android demo，用的就是四元数 实现方案，通过四元数得到的y轴偏移量，不断替换图片。
![](http://img.blog.csdn.net/20170601235513066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
<-------------------------------
当手机横屏，向左倾斜的时候，就会看到右边的视野。也就是判断y轴的相对偏移量，替换不同角度的图片。（gif 图做得有些卡顿，demo里的例子就平滑多了）
![](http://img.blog.csdn.net/20170601235533535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDIwMzcxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
-------------------------------->
当手机横屏，向右倾斜的时候，就会看到左边的视野。
demo 链接：[https://github.com/xiaoxiaoqingyi/VRPhoto](https://github.com/xiaoxiaoqingyi/VRPhoto)
四元数国外资料：[http://files.cnblogs.com/files/xiaoxiaoqingyi/madgwick_internal_report.pdf](http://files.cnblogs.com/files/xiaoxiaoqingyi/madgwick_internal_report.pdf)





