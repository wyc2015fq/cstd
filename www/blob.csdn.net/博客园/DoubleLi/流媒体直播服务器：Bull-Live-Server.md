# 流媒体直播服务器：Bull-Live-Server - DoubleLi - 博客园






Bull Live Server 简称 BLS ，旨在使用C++语言提供强大功能和高性能的流媒体直播服务器。

为何要写 BLS ？

1、simple rtmp server [https://github.com/winlinvip/simple-rtmp-server](https://github.com/winlinvip/simple-rtmp-server)
SRS很简洁却功能太少，BLS 使用C++语言强有力的现代特性，完全重新架构和编码， 结合基于state thread library的基础库和ST架构可以功能更强大，可以让编码更简洁，功能更强大。

2、crtmp server [http://www.rtmpd.com/](http://www.rtmpd.com/)
CRTMPD功能很完善，架构上却很复杂，C++代码也写得晦涩难懂。
而BLS会使用更优秀的C++特性。

3、nginx-rtmp [https://github.com/arut/nginx-rtmp-module](https://github.com/arut/nginx-rtmp-module)
这个基于nginx，性能自不必说，功能比crtmp少，但是纯异步架构的复杂度和简洁性都不够好。

4、red 5 [http://www.red5.org/](http://www.red5.org/)
red 5 性能有瓶颈，而且和新的一些rtmp 服务器对接会有问题。

5、wowza [http://www.wowza.com/](http://www.wowza.com/)
基于java语言，收费版，不适合屌丝们部署，但架构比较陈旧，而且配置很繁琐，正在作古之中。

6、FMS [http://www.adobe.com](http://www.adobe.com/)
现在叫AMS，功能和性能都不差，基本什么功能都有，毕竟是adobe自家的，但是license很昂贵！



比较了这么多，BLS要做什么？目标是什么？定位是什么?
BLS 就是为了汲取上述软件的特点，做一个更好用的直播服务器，要免费，要开源，功能和性能不打折，
代码要简洁、易懂。

https://github.com/wenjiegit/Bull-Live-Server









