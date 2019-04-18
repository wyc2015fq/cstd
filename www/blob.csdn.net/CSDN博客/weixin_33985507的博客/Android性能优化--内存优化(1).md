# Android性能优化--内存优化(1) - weixin_33985507的博客 - CSDN博客
2016年11月04日 11:46:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
![2249451-8fed8deb0d53b85c.jpg](https://upload-images.jianshu.io/upload_images/2249451-8fed8deb0d53b85c.jpg)
Performance Optimization
##### 接下来一段时间会持续更新“性能优化”专题中有关于Android方面的文章，敬请关注！
[Android性能优化--内存优化(2)](https://www.jianshu.com/p/f92d009fcd8d)
#### 1.什么是内存泄露?
当出现对Activity、View或drawable等类的对象长期持有无用的引用，就会造成被引用的对象无法在GC(垃圾回收器)时回收，而是长期占用堆空间，此时就会发生内存泄漏。
#### 2.常见的几种导致内存泄露的原因
(1).static关键字的使用，例如使用这样的代码：private static Activity context；当我们使用static来修改成员变量时，该变量就属于该类，而不是这个类的实例。
(2).线程的使用,由于线程生命周期的不可控，很容易造成内存泄露，另外，当和handler一起使用来实现线程间的通信时，在sendMessage完成之后如果不及时的将msg和handler置为null，也将造成内粗泄露。
(3).Bitmap对象不再使用时没有及时调用recycle()释放内存。
(4).Context泄露
```
private static Drawable sBackground;
@Override
protectedvoid onCreate(Bundle state) {
super.onCreate(state);
TextView label =new TextView(this);
label.setText("Leaks are bad");
if (sBackground ==null) {
sBackground = getDrawable(R.drawable.large_bitmap);
}
label.setBackgroundDrawable(sBackground);
setContentView(label);
}
/-----------------------------------------------------/
在第一次屏幕方向切换时它泄露了一开始创建的Activity。
当一个Drawable附加到一个 View上时，View会将其作为一个callback设定到Drawable上。
上述的代码片段，意味着Drawable拥有一个TextView的引用,
而TextView又拥有Activity（Context类型）的引用，换句话说，Drawable拥有了更多的对象引用。
即使Activity被销毁，内存仍然不会被释放。
```
。。。(有关内存泄露的原因此处不再过多的列举，相关的文字网上有很多)
#### 3.问题分析
今天我们借助Android Studio来分析问题
(1)利用AS自带的代码检查工具进行初步检查
![2249451-6aea41146b425023.png](https://upload-images.jianshu.io/upload_images/2249451-6aea41146b425023.png)
代码检查工具的入口
![2249451-2056496feabcabad.png](https://upload-images.jianshu.io/upload_images/2249451-2056496feabcabad.png)
选择需要分析的文件或工程
![2249451-c94cf330baf50ac9.png](https://upload-images.jianshu.io/upload_images/2249451-c94cf330baf50ac9.png)
分析结果
(2).利用AS自带的内存分析工具查看内存使用以及是否存在泄露
![2249451-1e393356770d0982.png](https://upload-images.jianshu.io/upload_images/2249451-1e393356770d0982.png)
通过点击AS左下方的Android Monitor可以查看内存、CPU等使用情况。点击Dump Java Heap按钮就可查看到上图的分析结果。通过对上图的分析结果的查看，可以看到哪些地方可能存在内存泄露。
(具体的分析结果可以自己尝试使用AS的分析工具查看)
#### 4.问题的解决
对于遇见比较多的类似于Static关键字、Context引用和Bitmap等内存泄露问题，网上已经有很多的解决方案，本文暂时不重复造轮子，后续的文章中会逐步的完善更新解决方案，达到优化的效果。
#### 总结
本专题中关于"Android性能优化"的文章会逐步更新，如果有不足之处还望指教与谅解，谢谢！
也欢迎各位大牛分享自己关于性能优化方面的经验，供后来者学习。
(网上关于该类文章可能已经随处可见，但是如果能够按照自己的理解坚持完成一个章节的总结，还是很有帮助的，所以文章有很多不足之处，还望谅解，我会继续努力做到更好。)
