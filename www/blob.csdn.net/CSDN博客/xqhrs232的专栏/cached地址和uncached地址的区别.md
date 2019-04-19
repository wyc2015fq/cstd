# cached地址和uncached地址的区别 - xqhrs232的专栏 - CSDN博客
2014年04月08日 15:50:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：649
原文地址::[http://blog.csdn.net/linuxandroidwince/article/details/7638318](http://blog.csdn.net/linuxandroidwince/article/details/7638318)
相关文章
1、[WinCE OAL中的Memory函数介绍](http://blog.csdn.net/nanjianhui/article/details/4038734)----[http://blog.csdn.net/nanjianhui/article/details/4038734](http://blog.csdn.net/nanjianhui/article/details/4038734)
2、[关于WinCE驱动编写时的IO地址映射](http://blog.csdn.net/backstrokefish/article/details/376201)----[http://blog.csdn.net/backstrokefish/article/details/376201](http://blog.csdn.net/backstrokefish/article/details/376201)
3、w​i​n​c​e​内​存​管​理----[http://wenku.baidu.com/link?url=QNMt18KvpLpqE-oRQjgqThAMcDnQwff3gY2B23aefLR4UphhXvMFuR3iXcWUW71vvEqiMDpi_sAO9K5dITjIRsaAnjzp7OtTnBnjlMKS9v7](http://wenku.baidu.com/link?url=QNMt18KvpLpqE-oRQjgqThAMcDnQwff3gY2B23aefLR4UphhXvMFuR3iXcWUW71vvEqiMDpi_sAO9K5dITjIRsaAnjzp7OtTnBnjlMKS9v7)
4、2008年最后一帖——了结MMU和cache（顺便纠正以前的错误）----[http://bbs.csdn.net/topics/290087801](http://bbs.csdn.net/topics/290087801)
cached地址和uncached地址的区别是，对cached地址的访问是委托给CPU进行的，也就是说你的操作到底是提交给真正的外设或内存，还是转到CPU缓存，是由CPU决定的。CPU有一套缓存策略来决定什么时候从缓存中读取数据，什么时候同步缓存。对unchached地址的访问是告诉CPU忽略缓存，访问操作直接反映到外设或内存上。 对于IO设备一定要用uncached地址访问，是因为你的IO输出操作肯定是希望立即反映到IO设备上，不希望让CPU缓存你的操作；另一方面，IO设备的状态是独立于CPU的，也就是说IO口状态的改变CPU是不知道，这样就导致缓存和外设的内容不一致，你从IO设备读取数据时，肯定是希望直接读取IO设备的当前状态，而不是CPU缓存的过期值。
 一般只有对主内存的访问才用cached地址，因为内存里的数据一般是通过CPU来改的，它不会自己变，所以CPU知道什么时候可以用缓存代替内存，什么时候应该同步。当然一个重要的例外是DMA，DMA显然也会不通过CPU而改变内存数据。也就是说，DMA在内存和内存之间，或者内存和外设之间搬运数据时，也会导致缓存和内存数据的不一致。这个不一致可以由硬件方法或软件方法解决，硬件方法是说DMA搬运数据时自动通知CPU把相关缓存设为无效，软件方法是程序员要在启动DMA前自己清缓存。
