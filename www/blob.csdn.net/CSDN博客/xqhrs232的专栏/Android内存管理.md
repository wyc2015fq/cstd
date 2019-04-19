# Android内存管理 - xqhrs232的专栏 - CSDN博客
2012年05月25日 14:42:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：975标签：[android																[开发工具																[path																[jni																[测试																[float](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=jni&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=开发工具&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://tianhe945.blog.163.com/blog/static/2017531222012417134797/](http://tianhe945.blog.163.com/blog/static/2017531222012417134797/)
相关网帖
1.基于 Android NDK的学习之旅-----资源释放----[http://www.cnblogs.com/luxiaofeng54/archive/2011/08/21/2147822.html](http://www.cnblogs.com/luxiaofeng54/archive/2011/08/21/2147822.html)
2.NDK C中malloc的使用----[http://iaiai.iteye.com/blog/1409237](http://iaiai.iteye.com/blog/1409237)
Android会自动管理内存，JAVA也有garbage collection (GC )内存回收机制。
但是如果程序在一次操作中打开几个M的文件，那么通常会出现下面的错误信息。
1.明确调用System.gc(); 
这种内存回收会有一定的作用，但是请不要太期待。
System.gc()在调用时程序是处于阻塞状态，系统考虑到程序运行的流畅性，会分批次进行垃圾回收，每次只回收一部分，我曾经遇到过的状况是只调用了一次System.gc()，然后看到LOG显示垃圾的确回收了，但很快Out of memory便接踵而至，看来是垃圾还没有回收完，系统内存就溢出了，最后采用了一个无奈的方法，连续调用10次甚至20次System.gc()，问题便解决了。
2.图片处理完成后回收内存。
请在调用BitMap进行图片处理后进行内存回收。
  bitmap.recycle(); 
这样会把刚刚用过的图片占用的内存释放。
3.图片读入内存时指定大小。
    BitmapFactory.Options options = new BitmapFactory.Options();
  options.inJustDecodeBounds = true;
  bitmap = BitmapFactory.decodeFile(path, options); // 此时返回bm为空
  options.inJustDecodeBounds = false;
  // 缩放比。由于是固定比例缩放，只用高或者宽其中一个数据进行计算即可
  int be = (int) (options.outHeight / (float) heigh);
  if (be <= 0)
   be = 1;
  options.inSampleSize = be;
  // 重新读入图片，注意此时已经把options.inJustDecodeBounds 设回false了
  bitmap = BitmapFactory.decodeFile(path, options);
4．Drawable和Bitmap的区别
|对比项|Bitmap|Drawable|
|----|----|----|
|显示清晰度|相同|相同|
|占用内存|大|小|
|支持缩放|是|是|
|支持色相色差调整|是|否|
|支持旋转|是|是|
|支持透明色|是|是|
|绘制速度|慢|快|
|支持像素操作|是|否|
Drawable在内存占用和绘制速度这两个非常关键的点上胜过Bitmap
5.问题排查
从以下几点进行分析和排查：
|1|所有不再使用的类对象你置空了吗？|
|----|----|
|2|被置空的那些对象里的那些申请了内存空间的子对象（比如Bitmap对象）你释放资源以及置空了吗？|
|3|置空对象后有没有调用System.gc()?|
6.Android框架，每一个进程都有内存限制,可以采用以下方法来躲开这个问题
每一个程序可以通过android.os.Debug.getNativeHeapAllocatedSize（）查看进程可以使用的内存。
一种方法是从本机代码分配内存。使用NDK的（本地开发工具包）和JNI，它可以从C级（如的malloc / free或新/删除），这种分配的内存分配，不计入内存的限制。
另一种方法，效果很好，图像，是使用OpenGL纹理 - 纹理内存同样不被计入内存的限制
据说可以达到300M的内存使用，哪位朋友测试了可以告诉我，这个方法的可行性。
7.软引用
8.有MAT测试
本人整理编写，转载请注明出处：[http://hi.baidu.com/weatherhao](http://hi.baidu.com/weatherhao)
//============================================================================
备注::
   1》OpenGL纹理 - 纹理内存同样不被计入内存的限制，据说可以达到300M的内存使用----跟我自己实践的情况基本相符！
      我的APK的纹理在不断地吃系统的可用内存，吃到系统可用内存到300MB（整个可用内存700MB左右，也就是纹理可用内存300~400MB的样子）左右的时候，我的APK跟系统就挂掉了！！！如果不是这样按ANDROID限制的前台进程可用的24/32MB内存的话，APK早就要死掉的！！！
  2》malloc / free创建/释放的内存也是不计入内存的限制，这个我也是确有这样的体会啊！！！因为我的程序在不断malloc
 / free，如果有限制程序马上就到其内存限制了，马上就会报错了！！！

