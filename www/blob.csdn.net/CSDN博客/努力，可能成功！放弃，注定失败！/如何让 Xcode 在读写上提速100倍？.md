# 如何让 Xcode 在读写上提速100倍？ - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月24日 16:51:21[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1215
![](http://upload-images.jianshu.io/upload_images/1721232-e5dcce65c3e2c402.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
本文来自简书，原文地址:[http://www.jianshu.com/p/5861beb5bb75](http://www.jianshu.com/p/5861beb5bb75)
上个月参加了一场西雅图当地的线下 iOS 开发者聚会。Jeff Szuhay 作为一个有20+年开发经验的资深程序员，跟我讲了一套提高 iOS 开发效率的方法。相比于其他程序员在 App 启动时间、架构优化方面的经验，老爷子 Jeff 的优化基于硬件层面，匠心独运，极客风十足。以下是他的经验分享和我个人的实测。
# 问题来源
我们都知道 Xcode 在运行或编译时，会有大量的读写操作。例如从硬盘中调用图片，我们会这么操作：
`let image = UIImage(named: "imageName")`
这时候 Xcode 就会去电脑的硬盘中去找到图片，完成读写操作。类似的操作还有存取文件等等。如果这类读取数量比较少，那么无伤大雅，但是一旦多起来，尤其是大项目在后期产生了大量的 DerivedData 存在硬盘上，Xcode 在编译时就会花大量时间去硬盘（Disk）上完成读写这些数据的操作。更不幸的是有时候还会遇到硬盘故障等问题。
# 解决思路
正所谓“哪里需要优化，哪里就需要程序员”，Jeff 在这个时候作为一名白衣骑士登场了。多年的计算机研究让他对整个计算机架构非常熟悉。下图是他展示的计算机结构简图。
![](http://upload-images.jianshu.io/upload_images/1721232-adf5f8a841eb5116.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
计算机结构简图
此图简洁明了得说明了计算机的基本架构。左上角是计算机的大脑，CPU，负责核心计算和处理工作；右上角是内存（RAM），用来运行程序并与 CPU 进行数据交流；中间的线是总线，负责各个模块之间传递信息和信号；图下侧是基本的 System IO。
再回来看我们的问题：Xcode 现在是在 RAM 中运行，然后到 Storage 中读写数据，数据接着再传回 RAM。这种方式有两个瓶颈：
- Storage 速度很慢。即使是最先进的 SSD，其速度也比 RAM 慢了400倍。也就是无论你怎么在软件层优化，其速度也无法突破 SSD 的瓶颈；
- 数据要不停的在各个模块之间传递。传递过程中亦有延时和无谓的时间消耗。
针对以上两个瓶颈，Jeff 认为，如果我们可以让所有的读写操作都在内存（RAM）中完成，那么必然能大幅提高 Xcode 的工作效率。问题是，怎么实现？
# 实现方法
方法的思路很简单，大概可以分两步：
- 配置 RAM。在内存中专门开出一块让 Xcode 使用。
- 连接 Xcode。让 Xcode 连接到我们开辟出来的专属内存空间。
下面就是见证奇迹的时刻。
第一步， 创建 .sh 文件。代码如下。
```bash
#!/bin/bash
RAMDISK=”ramdisk”
SIZE=1024         #size in MB for ramdisk.
diskutil erasevolume HFS+ $RAMDISK \
     `hdiutil attach -nomount ram://$[SIZE*2048]`
```
第二步， 运行 .sh 文件。在命令行中敲下。
![](http://upload-images.jianshu.io/upload_images/1721232-bb3796ea2f216f41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
之后你会发现你会多出一个叫 ramdisk 的内存空间，有大概 1 GB 大小。
![](http://upload-images.jianshu.io/upload_images/1721232-14e9522d7e98fd63.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
第三步，连接 Xcode。Xcode -> Preferences -> Locations -> Locations Tab，配置 DerivedData。
![](http://upload-images.jianshu.io/upload_images/1721232-ba3d35d2401dfc28.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Advanced... 也要配置成下图所示
![](http://upload-images.jianshu.io/upload_images/1721232-14fc390ed45351fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
以上就是全部步骤。这时候你就可以享受飞一般的开发了。现在 Project 中所有文件都在内存中，相比于 SSD，理论上是要快上一个数量级。
# 注意事项
- 
合理分配内存空间。我这里分配了 1GB 的内存当硬盘使，是因为我电脑本身有 16GB 内存空间。假如你电脑内存只有 4GB，我不建议你使用这个方法，或者建议只分配 256M 空间给 Xcode。总之，注意内存不足或溢出的情况。
- 
只把 DerivedData 放在 Ram Disk 中。为了极限速度，你当然可以把 App 相关所有的文件都放在内存空间中。但是要知道，我们创造的 Ram Disk 本质是内存，当关机或重启的时候，在 Ram Disk 中的数据是会丢失的。而 DerivedData 是可以重新生成的，所以放在 Ram Disk 中可以最大限度的提高 Xcode 开发中的读取速度，且十分安全。
## 参考
- [Performance Considerations for
 macOS/iOS Development in the “New Frontier”](http://www.szuhay.org/jeff/Xcoders/PerformanceInNewFrontier_Presentation.pdf)
- [Cache In Your Pocket: Use a RAM disk for Xcode](http://www.blinddogsoftware.com/goodies/#CacheInYourPocket)
