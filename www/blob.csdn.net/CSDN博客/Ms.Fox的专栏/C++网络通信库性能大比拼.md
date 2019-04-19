# C++网络通信库性能大比拼 - Ms.Fox的专栏 - CSDN博客
2017年06月19日 11:47:10[lulu-lu](https://me.csdn.net/smbluesky)阅读数：895
http://blog.csdn.net/csfreebird/article/details/49283743
C/C++网络通信库有不少，本次benchmark的目的是为了公平的评估它们的网络I/O性能，当然是作为REST server, 因此每个server都写了一些代码，好在不是特别复杂。这个测试经过了好几轮，本文给出了最终的结论。
先上结论，大家都忙：）
候选者：cppcms, boost asio, libevent, muduo和 nginx，nginx不是库，这里做测试使用它作为基准，毕竟很多人心里，它是不可挑战的。
结论：QPS的比拼结果，cppcms最弱，boost asio好很多，libevent好更多，nginx比libevent还要好点，muduo最好。
但是，不要小瞧这里最弱的cppcms，它仍可以轻易击败php, ruby, java, go, python, nodejs等语言编写的rest server。
下面介绍测试的方法
# 测试工具：wrk
之前选用过apache的ab test工具，不要被名字骗了，和常说的ab test方法没什么关系。这是一个压力测试工具，但是明显不能将压力升到最高，还是wrk效果最好。因为压测的目标是，击穿服务器，然后减少点压力，找到能够让服务器网络程序正常工作的最大压力。
找测试工具的唯一标准就是能不能用足压测客户机器的资源，释放最大的压力。其他非C/C++的压测工具也就直接谢绝了，这不是什么开发效率至上的场合。
wrk -t4 -c200 -d60s "[http://167serverip:serverport/collect?v=1](http://167serverip/collect?v=1)"
wrk tool; 4 threads; 200 concurrency connection; duration 60s.
# 测试服务器
|Machine|CPU Model|MEM|NETCARD(Gbps)|OS|
|----|----|----|----|----|
|client|Intel(R) Xeon(R) CPU E5620 @2.40GHz 8 core|16G|1|CentOS release 6.5 (Final) x86_64|
|server|Intel(R) Xeon(R) CPU E5620 @2.40GHz 8 core|16G|1|CentOS release 6.5 (Final) x86_64|
8核服务器，CentOS系统是乐视大运维同事优化过内核的，性能应该超过下载下来的默认系统。测试服务器分成两种，client负责发出http调用，server负责接受http请求，我们测量的就是server成功处理的最大QPS.
# 网络程序架构
都使用epoll,不过还是有差别，主要在多线程模型上。
|Server|Process Num|Thread Num|IO Pattern|
|----|----|----|----|
|cppcms_based|1|default threads = core*5|epoll one thread io_loop|
|asio_based|1|8|epoll io_loop-per-thread|
|muduo_based|1|8|epoll io_loop-per-thread|
|libevent_based|1|8|epoll io_loop-per-thread|
|nginx|8 worker|1|epoll one thread io_loop|

# 测试结果
format: min/max/average
|Server|Cpu|Mem|Disk|Netcard(in/out)|Requests/sec|
|----|----|----|----|----|----|
|libevent_based|684% / 768% / 756%|7276 / 9.9M / 8231|-|-|104797 / 112025 / 111998|
|asio_based|302% / 357% / 309%|5522 / 5976 / 5743|-|-|18971 / 19246 / 19163|
|cppcms_based|230% / 249% / 231%|9210 / 9428 / 9378|-|-|15434 / 16391 / 15500|
|muduo_based|680% / 754% / 702%|6644 / 7332 / 6720|-|-|286586 / 289423 / 287332|
|nginx|8*80% / 8*86 / 8*82|8*44M / 8*44M / 8*44M|-|-|112658 / 114127 / 113406|
如果使用O3优化选项， 发现对于boost asio和muduo有较大提升。
|Server|Compiler|Optimization option|Cpu|Mem|Disk|Netcard(in/out)|Requests/sec|
|----|----|----|----|----|----|----|----|
|cppcms_based|clang++ 3.6.2|-O3|228% / 239% / 228%|9356 / 9416 / 9416|-|-|14252 / 16124 / 15820|
|asio_based|clang++ 3.6.2|-O3|300% / 305% / 303%|4368 / 4564 /4416|-|-|33069 / 34247 / 33360|
|libevent_based|clang++ 3.6.2|-O3|763% / 764% / 764%|5560 / 10M / 5520|-|-|113373 / 114072 / 113713|
|muduo_based|clang++ 3.6.2|-O3|650% / 694% / 658%|6272 / 6324 / 6312|-|-|303202 / 307204 / 305839|
# 其他测试结论
我们使用的是clang++编译器和g++编译器，优化选项是O3, 发现两个编译器编译出来的程序性能相当。
我们做了C++11和之前的C++03版本的对比，发现C++11略好于C++03。
还有很多轮测试，限于篇幅，不在这里描述。
# 为什么cppcms和boost asio性能不高？
因为mutex，对epoll使用最高效的方式在多线程里面每个线程都调用epoll_wait，而这两个库都增加了mutex去锁，导致性能低下。
mudoo性能特别高的原因是它的设计只考虑linux平台，只把一个平台的通信库做到极致，设计目的单纯，因此能够做到最好。
具体的原因后面会专门发表文章剖析。今天先写这么多。
