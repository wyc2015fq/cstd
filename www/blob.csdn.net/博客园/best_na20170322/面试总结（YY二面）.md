# 面试总结（YY二面） - best_na20170322 - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/believe-in-me/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/best_na20170322)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/believe-in-me/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [面试总结（YY二面）](https://www.cnblogs.com/believe-in-me/p/6613307.html)





（1）一幅M*N大小的图像大小是多少？

（2）项目是图像处理的，用到了图像处理里面的那些算法？项目中读取一副图片的让它显示出来的过程是怎样的，数据流是怎样的？图像是如何存的（我说是二维数组他说数组的大小是如何确定的，我说是通过Matlab确定的，可是如果不同厂家的图片大小不一样，难道不能在程序中确定大小吗，我说没有想过呀）

还问项目中遇到了哪些问题？

答：比如说先开始是1024*1024*2字节的图片，处理起来比较慢，然后就说。。。。是如何改进的。

又问项目中的方法是已有的呢，还是你们发明的的？

项目中几个人啊？分工是怎样的啊？

（3）有小汽车、摩托车、卡车，让你设计一个基类和子类？

（4）虚函数是怎样实现的？基类指针指向派生类对象如何实现调用不同对象的函数？

（5）进程和线程的区别？线程共享的变量有哪些？

进程是资源分配的基本单位；线程是系统调度的基本单位。

平时我们写的程序都是作为线程运行的；进程可以看做是包括一系列线程和资源的统称；一个进程至少包括一个

线程（主线程，进入main函数时产生的）；在其中可以创建其它线程，也可以不创建。

同一进程间的线程究竟共享哪些资源呢，而又各自独享哪些资源呢？

共享的资源有

a. 堆  由于堆是在进程空间中开辟出来的，所以它是理所当然地被共享的；因此new出来的都是共享的（16位平台上分全局堆和局部堆，局部堆是独享的）

b. 全局变量 它是与具体某一函数无关的，所以也与特定线程无关；因此也是共享的

c. 静态变量 虽然对于局部变量来说，它在代码中是“放”在某一函数中的，但是其存放位置和全局变量一样，存于堆中开辟的.bss和.data段，是共享的

d. 文件等公用资源  这个是共享的，使用这些公共资源的线程必须同步。Win32 提供了几种同步资源的方式，包括信号、临界区、事件和互斥体。

独享的资源有

a. 栈 栈是独享的

b. 寄存器  这个可能会误解，因为电脑的寄存器是物理的，每个线程去取值难道不一样吗？其实线程里存放的是副本，包括程序计数器PC



（6）你更偏向于做哪一块？我说后台开发，他说你知道后台开发是怎样的吗？（实际上我知道的并不清楚，就说前端改变后，通过程序对数据进行处理）

（7）网络中在输入。。。然后点击搜索，接下来的过程是怎样的呢？

（7）有没有什么要问的？我说刚才我解释的后台开发并不是那么清楚，能不能麻烦你给我解释一下？

后台开发：

首先我们要先对一个网站的架构有基本的了解，网站/软件的组成由：网站后台程序+前端页面+域名+服务器+数据库，这几大项组合在一起，才是我们平常所访问的平台。 
    一般我们访问一个平台的时候，所看到的基本都是前端内容，而前端内容又通过用户点击，然后触发后台的功能和数据库。所以我们可以访问浏览很多很多网页或者使用一些平台上特定的功能。 
    其实做后台的很多方向，部门很多，不同部门做后台的做的事情也不一样。要求熟悉linux，[tcp/ip](https://www.baidu.com/s?wd=tcp/ip&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9nHFBuW7BmWb4njI9rjmY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH64n164rHbLrHmknjnvP16zn0)是必须的。 
     涉及到后台的， 一般都是要跟数据库打交道的大型数据量的处理问题。 
     以类 client - server 为基础的架构， 或者变形后的架构。 
     客户端处理用户的输入和数据，  
     然后大量的客户端（多个客户端的多种数据） 反馈到服务端统一处理和协调， 然后服务端对客户端发出相应的指令。 
     其中 后台程序 代表的就是服务端的程序。 包含以下几点： 
     1. 网络通信，  要跟远程的client打交道，只能用网络 
     2. 并发 和 并行处理。0。。 多个客户端可能在同一时间同时需要处理同一个类型的数据， 谁先谁后，谁的有效谁的无效？ 服务端的并发，同步显得非常重要。 同理，每个客户端外表看起来都是立刻接收到服务的，这需要并行化处理。 
     3. [数据库设计](https://www.baidu.com/s?wd=%E6%95%B0%E6%8D%AE%E5%BA%93%E8%AE%BE%E8%AE%A1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9nHFBuW7BmWb4njI9rjmY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH64n164rHbLrHmknjnvP16zn0)。 大量的数据，必须通过数据库进行管理。但是频繁的数据库写入会导致性能下降，所以要有合理的[数据库设计](https://www.baidu.com/s?wd=%E6%95%B0%E6%8D%AE%E5%BA%93%E8%AE%BE%E8%AE%A1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9nHFBuW7BmWb4njI9rjmY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH64n164rHbLrHmknjnvP16zn0)，以及读写控制设计。  然后数据库的备份设计， 万一down掉了怎么办，只能用备份的。这一点在银行等[电子商务](https://www.baidu.com/s?wd=%E7%94%B5%E5%AD%90%E5%95%86%E5%8A%A1&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1d9nHFBuW7BmWb4njI9rjmY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EnH64n164rHbLrHmknjnvP16zn0)的后台设计中是重中之重。  
     4. 良好的扩展性的架构， 后台一直在变的， 因为客户端的需求一直是增加的， 不能因为为了添加一个新功能，影响到旧的功能。














