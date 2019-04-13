
# Glide源码分析1 - 框架 - 谢杨易的博客 - CSDN博客

2016年08月07日 12:18:53[谢杨易](https://me.csdn.net/u013510838)阅读数：1229标签：[源码																](https://so.csdn.net/so/search/s.do?q=源码&t=blog)[Glide																](https://so.csdn.net/so/search/s.do?q=Glide&t=blog)[框架																](https://so.csdn.net/so/search/s.do?q=框架&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Glide&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=源码&t=blog)个人分类：[Android																](https://blog.csdn.net/u013510838/article/category/6308183)
[
																								](https://so.csdn.net/so/search/s.do?q=源码&t=blog)



## 1. Glide介绍
Glide是由bumptech开发的一款Android图片加载库，得到了Google的大力推荐，并广泛应用于Android app开发中。Glide支持功能众多，代码量大，但模块分层清晰，相互独立，是一个非常经典的框架设计案例。读者不仅可以从源码中学到Glide加载图片的机理，也可以学到很多不错的设计模式案例。Glide的有如下优点
1）图片request跟随Activity或Fragment生命周期。Activity onStart()时，request得到恢复；Activity onStop()时，request会被暂停；Activity onDestroy()时，request会被清除。这个功能对用户完全透明，故而用户完全不用担心Activity退出而request仍然存在等内存泄露问题。
2）采用LruCache和DiskLruCache两级缓存，分别对应内存缓存和磁盘缓存
3）支持丰富的图片格式，支持GIF，webp等格式
4）保存到磁盘上的图片是经过裁剪和压缩的，故存储数据小
5）采用BitmapPool对象池管理BitmapResource的创建和回收，采用线程池管理执行耗时request的子线程。可以减少对象的创建和垃圾回收，并控制对象个数以保持足够的空余内存。
6）简单易用，Glide.with(context).load(url).into(imageView)，采用builder模式，这是常见的使用方式。用户只需要知道如何利用builder传入参数即可。Glide的很多特性对用户来说完全透明，用户不需要去操心。
## 2. Glide框架
Glide功能众多，代码也十分复杂。GitHub地址为[https://github.com/bumptech/glide](https://github.com/bumptech/glide)。下载后发现代码量有33M之多，足见其复杂度。但是得益于开发者良好的分层思想和完美的设计模式，各子模块之间相互独立，减小了模块内的复杂度。模块间依赖抽象，耦合松散，也大大提高了可扩展性。主要分为三大模块：
1）request创建与发送。这个部分也是我们使用者碰到的地方。采用Builder模式创建Request，加入到队列中，然后发送出去。发送最终利用了另一个模块，即Engine。后面文章详细分析了这个模块的核心类和代码流程。参见[Glide源码分析2 – request创建与发送过程](http://blog.csdn.net/u013510838/article/details/52143086)
2）绑定Activity生命周期。这个部分实现了request随Activity或Fragment生命周期而恢复，暂停和清除的过程。原理为创建一个空白的Fragment，并添加到当前Activity或Fragment中。在空白Fragment的生命周期方法中回调request的恢复，暂停和清除的方法。参见[Glide源码分析3 – 绑定Activity生命周期](http://blog.csdn.net/u013510838/article/details/52143097)
3）Engine，缓存，编解码和网络请求。这个部分是实现request发送的底层。分为request任务的发送和request任务的执行两个部分。先尝试从内存缓存中获取Resources，再在线程池内新建子线程，并提交任务。等子线程执行时，进入到任务的执行阶段。先尝试从磁盘缓存中获取Resources，然后利用DataFetcher从网络或本地文件中获取原始数据，解码并转码为我们所需的格式。参见[Glide源码分析4 – 缓存，编解码和网络请求](http://blog.csdn.net/u013510838/article/details/52143113)
![这里写图片描述](http://i1.piimg.com/567571/f8770ae602c68a35.png)
## 3. 总结
Glide代码量虽然很大，但是分层很好，还是很方便阅读的。学习Glide源码可以帮助我们理解Glide内部实现机理，学会更好的开发和调试使用Glide的代码。同时Glide中有很多对于图片request的管理的方法，值得我们在网络request等其他场景下学习。最后，Glide涉及到了很多不错的设计模式，阅读源码也能帮助我们更好的理解设计模式。后面三篇文章会和大家一起去学习Glide源码。

