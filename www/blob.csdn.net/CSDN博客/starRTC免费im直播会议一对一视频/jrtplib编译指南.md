# jrtplib编译指南 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年02月05日 11:07:55[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：97
The library offers support for the Real-time Transport Protocol (RTP),
The library uses the [JThread](http://research.edm.uhasselt.be/jori/page/CS/Jthread.html) library to automatically poll 轮询 for incoming data in the background, so
 you may want to install it too.
 Of course, if you'd rather not have it installed, the library will also work without JThread (but you'll have to poll for incoming data yourself).
[http://research.edm.uhasselt.be/jori/page/CS/Jrtplib.html](http://research.edm.uhasselt.be/jori/page/CS/Jrtplib.html)
[https://github.com/j0r1/JRTPLIB](https://github.com/j0r1/JRTPLIB)
如果需要安装jThread,需要CMake 3.0 or higher is required.
升级cmake
cmake -version
安装jtread：
 cmake  CMakeLists.txt 
make && make install
如果不需要jthread
就直接互jstplib目录下执行
cmake .或 
 cmake  CMakeLists.txt 
make && make install

文档地址：[https://jrtplib.readthedocs.io/en/stable/](https://jrtplib.readthedocs.io/en/stable/)
参考：[http://blog.csdn.net/zhangjikuan/article/details/27974733](http://blog.csdn.net/zhangjikuan/article/details/27974733)
