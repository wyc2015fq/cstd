# d3d在mfc窗口下渲染延迟 - tifentan的专栏 - CSDN博客

2017年07月28日 16:16:37[露蛇](https://me.csdn.net/tifentan)阅读数：155


发现使用d3d在一个线程中同时渲染到2个隐藏的窗口，会变得很慢，50ms左右，如果将窗口显示出来，就很快，2-6ms。

经调试发现，跟渲染几个无关，只不过50ms刚好触发了我的延迟警报，渲染一个隐藏窗口大概30ms左右。幸好无端端渲染两个，不然永远不知道这个bug。

现在问题在于我有将窗口隐藏的需求。可以在渲染之前IsWindowVisible() ,判断要渲染的窗口是否可见，再决定要不要渲染。

顺便在这里记录一下，视频显示方面：surface 比texture效果要好。当然效率texture高。

我这边测试，surface渲染第一帧要22ms，texture14ms。第一帧后就很快了，surface1-2ms，texture0-1ms。

