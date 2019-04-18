# 一线互联网常见的14个Java面试题，你颤抖了吗程序员 - 算法与数学之美 - CSDN博客
2019年01月13日 21:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：41
> 
**跳槽不算频繁，但参加过不少面试（电话面试、face to face面试），面过大/小公司、互联网/传统软件公司，面糊过（眼高手低，缺乏实战经验，挂掉），也面过人，所幸未因失败而气馁，在此过程中不断查缺补漏，养成了踏实、追本溯源、持续改进的习惯，特此将自己经历过、构思过的一些面试题记录下来，如果答案有问题，欢迎拍砖讨论，希望能对找工作或者感兴趣的同学有所帮助，陆续整理中。**
** 1. synchronized和reentrantlock异同 **
相同点
- 
都实现了多线程同步和内存可见性语义
- 
都是可重入锁
不同点
- 
实现机制不同 synchronized通过java对象头锁标记和Monitor对象实现 reentrantlock通过CAS、ASQ（AbstractQueuedSynchronizer）和locksupport（用于阻塞和解除阻塞）实现 synchronized依赖jvm内存模型保证包含共享变量的多线程内存可见性 reentrantlock通过ASQ的volatile state保证包含共享变量的多线程内存可见性
- 
使用方式不同 synchronized可以修饰实例方法（锁住实例对象）、静态方法（锁住类对象）、代码块（显示指定锁对象） reentrantlock显示调用trylock()/lock()方法，需要在finally块中释放锁
- 
功能丰富程度不同 reentrantlock提供有限时间等候锁（设置过期时间）、可中断锁（lockInterruptibly）、condition（提供await、signal等方法）等丰富语义 reentrantlock提供公平锁和非公平锁实现 synchronized不可设置等待时间、不可被中断（interrupted）
** 2. concurrenthashmap为何读不用加锁 **
- 
jdk1.7
- 
1）HashEntry中的key、hash、next 均为final 型，只能表头插入、删除结点
- 
2）HashEntry类的value域被声明为volatile型
- 
3）不允许用null作为键和值，当读线程读到某个HashEntry的 value域的值为null时，便知道产生了冲突——发生了重排序现象（put设置新value对象的字节码指令重排序），需要加锁后重新读入这个value值
- 
4）volatile变量count协调读写线程之间的内存可见性，写操作后修改count，读操作先读count，根据happen-before传递性原则写操作的修改读操作能够看到
- 
jdk1.8
- 
1）Node的val和next均为volatile型
- 
2）tabAt和casTabAt对应的unsafe操作实现了volatile语义
** 3. ContextClassLoader（线程上下文类加载器）的作用 **
- 
越过类加载器的双亲委派机制去加载类，如serviceloader实现
- 
使用线程上下文类加载器加载类，要注意保证多个需要通信的线程间的类加载器应该是同一个，防止因为不同的类加载器导致类型转换异常(ClassCastException)
** 4. tomcat 类加载机制 **
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabky8UA1YLWFFykoic3EXZCshFKQnQ2uWjuuTwrqeXesxkoWfRA4CGStWoWHCUK1MOCfeI57MBZv2fUA/640?wx_fmt=jpeg)
- 
不同应用使用不同的 webapp类加载器，实现应用隔离的效果，webapp类加载器下面是jsp类加载器
- 
不同应用共享的jar包可以放到Shared类加载器/shared目录下
** 5. osgi类加载机制 **
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabky8UA1YLWFFykoic3EXZCshFARrmFHAt8iamjMglcRXRywkuH0YHjyeVwU62FwLtxhs3JqKHk2exwCQ/640?wx_fmt=jpeg)
- 
osgi类加载模型是网状的，可以在模块（Bundle）间互相委托
- 
osgi实现模块化热部署的关键是自定义类加载器机制的实现，每个Bundle都有一个自己的类加载器，当需要更换一个Bundle时，就把Bundle连同类加载器一起换掉以实现代码的热替换
- 
当收到类加载请求时，osgi将按照下面的顺序进行类搜索：
- 
1）将以java.*开头的类委派给父类加载器加载
- 
2）否则，将委派列表名单（配置文件org.osgi.framework.bootdelegation中定义）内的类委派给父类加载器加载
- 
3）否则，检查是否在Import-Package中声明，如果是，则委派给Export这个类的Bundle的类加载器加载
- 
4）否则，检查是否在Require-Bundle中声明，如果是，则将类加载请求委托给required bundle的类加载器
- 
5）否则，查找当前Bundle的ClassPath，使用自己的类加载器加载
- 
6）否则，查找类是否在自己的Fragment Bundle中，如果在，则委派给Fragment Bundle的类加载器加载
- 
7）否则，查找Dynamic Import-Package（Dynamic Import只有在真正用到此Package的时候才进行加载）的Bundle，委派给对应Bundle的类加载器加载
- 
8）否则，类查找失败
** 6. 如何结束一个一直运行的线程 **
- 
使用退出标志，这个flag变量要多线程可见
- 
使用interrupt，结合isInterrupted()使用
** 7. threadlocal使用场景及问题 **
- 
threadlocal并不能解决多线程共享变量的问题，同一个 threadlocal所包含的对象，在不同的thread中有不同的副本，互不干扰
- 
用于存放线程上下文变量，方便同一线程对变量的前后多次读取，如事务、数据库connection连接，在web编程中使用的更多
- 
**问题：** 注意线程池场景使用threadlocal，因为实际变量值存放在了thread的threadlocalmap类型变量中，如果该值没有remove，也没有先set的话，可能会得到以前的旧值
- 
**问题：** 注意线程池场景下的内存泄露，虽然threadlocal的get/set会清除key（key为threadlocal的弱引用，value是强引用，导致value不释放）为null的entry，但是最好remove
** 8. 线程池从启动到工作的流程 **
- 
刚创建时，里面没有线程
- 
调用 execute() 添加任务时：
- 
1）如果正在运行的线程数量小于核心参数corePoolSize，继续创建线程运行这个任务
- 
2）否则，如果正在运行的线程数量大于或等于corePoolSize，将任务加入到阻塞队列中
- 
3）否则，如果队列已满，同时正在运行的线程数量小于核心参数maximumPoolSize，继续创建线程运行这个任务
- 
4）否则，如果队列已满，同时正在运行的线程数量大于或等于 maximumPoolSize，根据设置的拒绝策略处理
- 
5）完成一个任务，继续取下一个任务处理
- 
6）没有任务继续处理，线程被中断或者线程池被关闭时，线程退出执行，如果线程池被关闭，线程结束
- 
7）否则，判断线程池正在运行的线程数量是否大于核心线程数，如果是，线程结束，否则线程阻塞。因此线程池任务全部执行完成后，继续留存的线程池大小为corePoolSize
** 9. 阻塞队列BlockingQueue take和poll区别 **
- 
poll(time)：取走BlockingQueue里排在首位的对象,若不能立即取出，则可以等time参数规定的时间，取不到时返回null
- 
take()：取走BlockingQueue里排在首位的对象，若BlockingQueue为空，阻塞直到BlockingQueue有新的对象被加入
** 10. 如何从FutureTask不阻塞获取结果 **
- 
get(long timeout,TimeUnit unit)，超时则返回
- 
轮询，先通过isDone()判断是否结束，然后调用get()
** 11. blockingqueue如果存放了比较关键的数据，系统宕机该如何处理 **
开放性问题，欢迎讨论
将队列持久化，比较麻烦，需要将生产数据持久化到磁盘，持久化成功才返回，消费者线程从磁盘加载数据到内存阻塞队列中，维护消费offset，启动时，根据消费offset从磁盘加载数据
加入消息队列，保证消息不丢失，生成序列号，消费幂等，根据消费进程决定系统重启后的生产状态
** 12. NIO与传统I/O的区别 **
- 
节约线程，NIO由原来的每个线程都需要阻塞读写变成了由单线程（即Selector）负责处理多个channel注册（register）的兴趣事件（SelectionKey）集合（底层借助操作系统提供的epoll()），netty bossgroup处理accept连接（**没看明白为什么bossgroup设置多个thread的必要性**），workergroup处理具体业务流程和数据读写
- 
NIO提供非阻塞操作
- 
传统I/O 以流的方式处理数据，而 NIO 以块的方式处理数据，NIO提供bytebuffer，分为堆内和堆外缓冲区，读写时均先放到该缓冲区中，然后由内核通过channel传输到对端，堆外缓冲区不走内核，提升了性能
** 13. list中存放可重复字符串，如何删除某个字符串 **
- 
调用iterator相关方法删除
- 
倒删，防止正序删除导致的数组重排，index跳过数组元素问题
** 14. 有哪些GC ROOTS（跟日常开发比较相关的是和此相关的内存泄露） **
- 
所有Java线程当前活跃的栈帧里指向GC堆里的对象的引用，因此用不到的对象及时置null，提升内存回收效率
- 
静态变量引用的对象，因此减少静态变量特别是静态集合变量的大小，集合存放的对象覆写euqls()和hashcode()，防止持续增长
- 
本地方法JNI引用的对象
- 
方法区中的常量引用的对象，因此减少在长字符串上调用String.intern()
- 
classloader加载的class对象，因此自定义classloader无效时及时置null并且注意类加载器加载对象之间的隔离
- 
jvm里的一些静态数据结构里指向GC堆里的对象的引用
- 
…
微信公众号“算法数学之美”，由算法与数学之美团队打造的另一个公众号，欢迎大家扫码关注！
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyjQVC3BFiaFKQ4DJqz2xhrwkzeCPbjQdnnG8678fRf1sxc2ZQtvtVib2dqWUkeopYtmgckINoOoGoQ/640?wx_fmt=jpeg)
**更多精彩：**
☞[如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483726&idx=1&sn=e5e008fb68a7d837546d0ac5b5438042&chksm=ebe9cbf3dc9e42e5d625b2da6b9b3866dff9f08d442d8106f4cbf035d8602e1fdda86eec6476&scene=21#wechat_redirect)
☞[自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483726&idx=2&sn=18272b7eaa172794b51c30d0a2dd9c48&chksm=ebe9cbf3dc9e42e5ddf9a189822a2fa099543a631ad63a1d6ed0158b51c76212eb65ebbfe71b&scene=21#wechat_redirect)
[☞费马大定理，集惊险与武侠于一体](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483715&idx=1&sn=9069dadf4bbce2aa34bd64b85a69dcee&chksm=ebe9cbfedc9e42e81c27d72da15c0dbf848e505946f231051b8b4033d0941bc6f51cef32790e&scene=21#wechat_redirect)
☞[简单的解释，让你秒懂“最优化” 问题](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483715&idx=2&sn=55a16f37c89b27994b263e0dc9837561&chksm=ebe9cbfedc9e42e842deb581ea62b750cedd839abd58c2db3261bf9fbcd172a2cf18512e4d2d&scene=21#wechat_redirect)
[☞一分钟看懂一维空间到十维空间](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483676&idx=1&sn=2366a39bca3ef42a6e868e91ea718813&chksm=ebe9cba1dc9e42b70c7e147b9e43828c1a7c68401f442890471a06e5cf0704437f9813ca0e0f&scene=21#wechat_redirect)
☞ [本科、硕士和博士到底有什么区别？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483661&idx=1&sn=d822666a054ba70b37dfb06d14c60f3a&chksm=ebe9cbb0dc9e42a6c476f7f81095b772aa45d960bf516f60c5b2e1155c9093696222cea0a83d&scene=21#wechat_redirect)
☞[小波变换通俗解释](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=1&sn=7d0d05c78cd01df91495f1d14609cbce&chksm=ebe9cbbbdc9e42add13cfe99f3383745fa5c059df705a3a9e28644d073dff804569af94970e3&scene=21#wechat_redirect)
☞[微积分必背公式](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=4&sn=ce88086b650c601bdbf57ecfe5a490a1&chksm=ebe9cbbbdc9e42adfaf0e4ee644d254835c830ef47663315b70a39a2b47e6a7cf10d0826b88d&scene=21#wechat_redirect)
☞[影响计算机算法世界的十位大师](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=3&sn=ceaec6043bb0e8a851033482f8f572bf&chksm=ebe9cbbbdc9e42ad30fd38383cf1caa609ac6e81964da17277f8e2a7f17a933cd11e0f3840c8&scene=21#wechat_redirect)
☞[数据挖掘之七种常用的方法](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=2&sn=bf439d56bc7d42083708fa76434a6025&chksm=ebe9cbbbdc9e42ad9e5f2b3c1952e620e0e3d4452aae25b611e7e54be8678b0d80e002e7be6d&scene=21#wechat_redirect)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
