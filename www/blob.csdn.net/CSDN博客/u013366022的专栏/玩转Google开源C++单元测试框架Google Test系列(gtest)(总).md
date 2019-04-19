# 玩转Google开源C++单元测试框架Google Test系列(gtest)(总) - u013366022的专栏 - CSDN博客
2019年04月10日 14:38:44[slitaz](https://me.csdn.net/u013366022)阅读数：6
前段时间学习和了解了下Google的开源C++单元测试框架Google Test，简称gtest，非常的不错。 我们原来使用的是自己实现的一套单元测试框架，在使用过程中，发现越来越多使用不便之处，而这样不便之处，gtest恰恰很好的解决了。
其实gtest本身的实现并不复杂，我们完全可以模仿gtest，不断的完善我们的测试框架， 但最后我们还是决定使用gtest取代掉原来的自己的测试框架，原因是：
1.不断完善我们的测试框架之后就会发觉相当于把gtest重新做了一遍，虽然轮子造的很爽，但是不是必要的。
2.使用gtest可以免去维护测试框架的麻烦，让我们有更多精力投入到案例设计上。
3.gtest提高了非常完善的功能，并且简单易用，极大的提高了编写测试案例的效率。
gtest的官方网站是：
[http://code.google.com/p/googletest/](http://code.google.com/p/googletest/)
从官方的使用文档里，你几乎可以获得你想要的所有东西 
[http://code.google.com/p/googletest/wiki/GoogleTestPrimer](http://code.google.com/p/googletest/wiki/GoogleTestPrimer)
[http://code.google.com/p/googletest/wiki/GoogleTestAdvancedGuide](http://code.google.com/p/googletest/wiki/GoogleTestAdvancedGuide)
如果还想对gtest内部探个究竟，就把它的代码下载下来研究吧，这就是开源的好处，哈！ 
官方已经有如此完备的文档了，为什么我还要写呢？一方面是自己记记笔记，好记性不如烂笔头，以后自己想查查一些用法也可以直接在这里查到，一方面是对于不想去看一大堆英文文档的朋友，在我这里可以快速的找到gtest相关的内容。
下面是该系列的目录：
[1.玩转Google开源C++单元测试框架Google Test系列(gtest)之一 - 初识gtest](http://www.cnblogs.com/coderzh/archive/2009/03/31/1426758.html)
[2.玩转Google开源C++单元测试框架Google Test系列(gtest)之二 - 断言](http://www.cnblogs.com/coderzh/archive/2009/04/06/1430364.html)
[3.玩转Google开源C++单元测试框架Google Test系列(gtest)之三 - 事件机制](http://www.cnblogs.com/coderzh/archive/2009/04/06/1430396.html)
[4.玩转Google开源C++单元测试框架Google Test系列(gtest)之四 - 参数化](http://www.cnblogs.com/coderzh/archive/2009/04/08/1431297.html)
[5.玩转Google开源C++单元测试框架Google Test系列(gtest)之五 - 死亡测试](http://www.cnblogs.com/coderzh/archive/2009/04/08/1432043.html)
[6.玩转Google开源C++单元测试框架Google Test系列(gtest)之六 - 运行参数](http://www.cnblogs.com/coderzh/archive/2009/04/10/1432789.html)
[7.玩转Google开源C++单元测试框架Google Test系列(gtest)之七 - 深入解析gtest](http://www.cnblogs.com/coderzh/archive/2009/04/11/1433744.html)
[8.玩转Google开源C++单元测试框架Google Test系列(gtest)之八 - 打造自己的单元测试框架](http://www.cnblogs.com/coderzh/archive/2009/04/12/1434155.html)
额外篇：
[1.gtest中如何跳出当前测试案例](http://www.cnblogs.com/coderzh/archive/2009/08/02/1536901.html)
[2.编写优美的GTest测试案例](http://www.cnblogs.com/coderzh/archive/2010/01/09/beautiful-testcase.html)
[3.gtest 参数化测试代码示例](http://www.cnblogs.com/coderzh/archive/2010/03/19/gtest_demo.html) (内含完整工程示例)
![](https://blog.coderzh.com/public/qrcode.jpg)
微信扫一扫交流
作者：[CoderZh](http://blog.coderzh.com/)
公众号：hacker-thinking （一个程序员的思考） 
独立博客：[http://blog.coderzh.com](http://blog.coderzh.com/)
博客园博客将不再更新，请关注我的「微信公众号」或「独立博客」。 
作为一个程序员，思考程序的每一行代码，思考生活的每一个细节，思考人生的每一种可能。 
文章版权归本人所有，欢迎转载，但未经作者同意必须保留此段声明，且在文章页面明显位置给出原文连接，否则保留追究法律责任的权利。
