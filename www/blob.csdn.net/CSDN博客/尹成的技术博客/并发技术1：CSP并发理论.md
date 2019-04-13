
# 并发技术1：CSP并发理论 - 尹成的技术博客 - CSDN博客

2018年11月16日 09:34:21[尹成](https://me.csdn.net/yincheng01)阅读数：147个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**异步async**
并行：多个任务并发执行
**同步sync**
串行：多个任务依次执行
**阻塞block**
某个并发任务由于拿不到资源没法干活，从而无所事事地干等
**进程并发-线程并发-协程并发**
**异步回调async callback**
A线程唤起B线程，令其干活
同时给B一个回调函数
命令B在干完活以后，执行这个回调函数
这个回调函数会与A线程发生交互
A不必阻塞等待B执行的结果，AB两个线程可以并发执行
利弊
效率高
回调地狱CallbackHell,逻辑线不清晰
**共享内存**
多个并发线程通过共享内存的方式交互数据
线程安全问题:AB间共享的数据地址可能被C并发修改
**同步锁/资源锁**
为了解决共享内存所导致的线程安全问题，共享的内存地址在特定时间段被特定线程锁定
加锁期间，其它线程无法访问，带来低效率问题
**死锁**
A锁住B的资源
B锁住A要的资源
AB同时阻塞
案例：小两口的冷战
女：锁住女人的尊严，得到男人的尊严后才释放
男：锁住男人的尊严，得到女人的尊严后才释放
**线程池**
背景：线程的开销大
内存：保存上下文数据
CPU：线程调度
为了避免无度创建线程（内存溢出OutOfMemory）,在一个池中创建一堆线程,循环利用这些线程,用完了以后重置并丢回池中.
利弊
利：避免了无度创建线程，降低了OOM的风险
 弊：用不用都占去了一大块内存开销
**线程并发的弊端**
开线程占内存
啥也不干就拿走1M栈空间
1024条线程就占用1G内存
线程切换占CPU
内存共享不安全
加了锁效率又低下
回调地狱导致开发难度高
**堆栈**
栈
变量和对象的名称
引用堆地址
堆
杂乱无章地堆放各种数据
没有栈对其进行引用时，就由nil进行引用
被nil引用的堆地中的内容随时可能被垃圾回收器回收
垃圾回收
一块堆内存如果没有被栈引用，就会被0号栈（空nil）所引用
一切被nil引用的对内存，会随时被垃圾回收器（GarbageCollector=GC）回收
**CSP模型**
CommunicatingSequentialProcess
可通信的序列化进程
并发的进程间通过管道进行通信
**共享内存 VS 管道**
内存共享:通过内存共享通信
管道:通过通信共享内存
**管道**
最早由CSP模型提出
以点对点管道代替内存共享实现并发进程间的数据交互
相比内存共享数据交互的相率要高很多
**协程**
coroutine
coorperte
协作
IO时让出CPU
routine
事务
微线程/纤程
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181116092700978.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

