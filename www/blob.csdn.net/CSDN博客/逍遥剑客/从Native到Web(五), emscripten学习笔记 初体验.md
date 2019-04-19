# 从Native到Web(五), emscripten学习笔记: 初体验 - 逍遥剑客 - CSDN博客
2013年04月30日 20:29:14[xoyojank](https://me.csdn.net/xoyojank)阅读数：13448
在《[传统3D游戏引擎的Web化](http://blog.csdn.net/xoyojank/article/details/8107373)》中, 当时我还说emscripten不够成熟, 可是短短几个月它就有了很大的进步, 可用性甚至超过了flascc!
在GDC2013上, Unreal发布了基于emscripten的移植Demo, 据说只用了4天时间, 足以看出这个移植过程的平滑
floh前几天发布了一个PPT: 《[C++ on the Web: Run your big 3D game in the browser](http://de.slideshare.net/andreweissflog3/quovadis2013-cpp-ontheweb)》, 里面提到了他们把游戏移植到网页上所做的各种尝试
主要提到了三种技术: Google NaCl, Adobe flascc, Mozilla emscripten. 到最后还是选择了emscripten, 这里有一些他的Demo: [http://www.flohofwoe.net/demos.html](http://www.flohofwoe.net/demos.html)
在他之前的Blog中也有提到各个方案的问题:
- NaCl: 仍然没有开放(必须是商店中的应用), 除Chrome外的浏览器不支持. 当然, 他的技术限制相对来说小一些, 性能也比较好
- flascc: 编译/链接效率巨低, 过程中占内存巨大. 运行效率不能让人满意. 目前来说, 优势也就是装机率高
- emscripten: IE支持是个问题. 线程支持也是个问题. API支持度比较好, 运行效率可以达到3D游戏的要求, 不限浏览器, 支持HTML5+WebGL即可
目前主要的问题是IE对于WebGL的支持, 但是IE11已经对WebGL开始实验性的支持了, 从这一点来说, 这个方案还是一种不错的预研选择
环境搭建的话, 跟NaCl比较像, 因为他们实现的思路说到底也都是托LLVM的福. 参考: [Using Emscripten from Visual Studio 2010](https://github.com/kripken/emscripten/wiki/Using-Emscripten-from-Visual-Studio-2010)
![](https://img-blog.csdn.net/20130430201856243)
从代码来看, 跟Native没什么两样, 所以只需要注意下平台相差的编码就可以了
![](https://img-blog.csdn.net/20130430201900927)
floh的demo运行效果(需要FireFox或者Chrome27以上版本):
![](https://img-blog.csdn.net/20130430203228234)
