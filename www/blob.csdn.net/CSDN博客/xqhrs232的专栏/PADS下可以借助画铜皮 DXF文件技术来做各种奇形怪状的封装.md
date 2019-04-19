# PADS下可以借助画铜皮/DXF文件技术来做各种奇形怪状的封装 - xqhrs232的专栏 - CSDN博客
2015年03月23日 14:25:42[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1853

相关文章
1、[pads中,怎样将焊盘和覆铜combine起来](http://blog.csdn.net/xqhrs232/article/details/44410109)----[http://blog.csdn.net/xqhrs232/article/details/44410109](http://blog.csdn.net/xqhrs232/article/details/44410109)
2、PADS异形封装绘制----[http://www.docin.com/p-661982244.html](http://www.docin.com/p-661982244.html)
3、pads如何画方形过孔的封装----[http://zhidao.baidu.com/link?url=57aywD0Q6WTnl7XKbIHuEwoww4Oq1lIfSPqjou1Gmcm7HNJA3FrqJPxGDiPRhTBBxpx9zdyTGKeZh2tc3PyGNYq26vZ6EicuLlp9imtNnm3](http://zhidao.baidu.com/link?url=57aywD0Q6WTnl7XKbIHuEwoww4Oq1lIfSPqjou1Gmcm7HNJA3FrqJPxGDiPRhTBBxpx9zdyTGKeZh2tc3PyGNYq26vZ6EicuLlp9imtNnm3)
一般的放端点做出来的封装都是固定的，但借助画铜皮/DXF文件技术就可以去做各种奇形怪状的封装！！！
然后在画好的铜皮的基础上增加一些有固定形状的端点就可以形成一个奇形怪状的封装了。
1》铜皮跟端点怎么联合起来,associate操作
2》怎么画不规则的铜皮
3》怎么输入用AutoCAD这样的软件画好的各种形状的DXF文件,Import操作
//=====================================================================================================================
//备注::
1》现在很多芯片都会提供参考设计----包括原理图跟PCB图，怎么快速的引用别人已经验证过的封装(比如天线走线)？直接COPY走线，再去制作一个封装就好了，
封装做好了存到库，这样就可以随时随地地进行封装引用了。
2》如果COPY整个奇怪的封装做不到，就COPY一段段线来实现吧！！！只是这样做的工作量有点大而已！！！
