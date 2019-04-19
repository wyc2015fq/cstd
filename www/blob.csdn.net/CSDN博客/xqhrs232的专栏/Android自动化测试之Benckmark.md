# Android自动化测试之Benckmark - xqhrs232的专栏 - CSDN博客
2012年02月29日 16:05:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：467标签：[测试																[android																[javascript																[浏览器																[mozilla																[google](https://so.csdn.net/so/search/s.do?q=google&t=blog)](https://so.csdn.net/so/search/s.do?q=mozilla&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/andy_android/article/details/7242109](http://blog.csdn.net/andy_android/article/details/7242109)
一、Android设备上的Benckmark测试概述
同PC相比，在Android设备上的性能测试还没有一个公认的标准。也没有PC上那么多的测试程序集。但我们可以通过一些工具所得到的信息更好的了解设备的特性，毫无疑问，这也是非常有价值的参考信息。Benchmark Test的关键就在于如何找到这一个标准以及能够提供测试各种性能的功能强大的程序集。
二、目前Android平台上的测试工具收集（以Android手机为例）
1、  Quadrant（Aurora Softworks推出）
针对CPU、内存、I/O输入输出、2D及3D图像的性能，提供了一键式的完整测试，或是根据需要选择其中某些测试项目单独测试。它还提供了内容格式清晰的系统信息查看功能，让你方便的了解非常详尽的手机处理器、内存及系统版本等特性。
该公司网站为：[http://www.aurorasoftworks.com](http://www.aurorasoftworks.com/)
版本一：Quadrant Standard   
免费版，2010年8月18日更新最新版本v1.1.2，在Google应用商店有超过3000个评论，综合评分高达4.5星，免费的标准版可以提供所有的测试功能，唯一不便的地方是在测试后需要向极光软件（AuroraSoftworks）的服务器提交数据进行分析后，才能显示结果。
版本二：Quadrant Advanced
高级版，2010年8月8日更新最新版本v1.1.1，售价2.99美元，这个版本在测试功能上与免费的标准版完全一样，但在测试结束后会直接显示一个测试结果直方图：
版本三：QuadrantProfessional
专业版，2010年8月8日更新最新版本v1.1.1，售价高达24.99美元，主要的差别其实就是“授权”费，允许使用此版本进行测试获得的结果可以用于商业用途，包括将测试结果出版在商业媒体上。
2、 Benchmark 1.03
一个测试系统性能的软件,可以测试Android系统的2D图形、cpu、内存和文件系统等方面的性能.测试项目非常多,且测试速度快,可以给出一个参考 分数供用户参考使用。
3、 Linpack for android
用于测试系统整体性能，偏向单任务下的表现。
官方网址：[http://www.greenecomputing.com/apps/linpack/](http://www.greenecomputing.com/apps/linpack/)
4、  Sunspider benchmark
测试webkit内核浏览器的JavaScript表现。
测试地址：
[http://www.webkit.org/perf/sunspider-0.9.1/sunspider-0.9.1/driver.html](http://www.webkit.org/perf/sunspider-0.9.1/sunspider-0.9.1/driver.html)
测试方式：直接在浏览器上打开该网页即可，测试结果会直接显示在网页上。
5、  Google V8 benchmark
GoogleV8 Benchmark测试是Google放出的Javascript测试集。V8 Benchmark Suite会载入JavaScript代码，对系统的内核、加密、解密、渲染等进行速度测试。
测试地址：http://v8.googlecode.com/svn/data/benchmarks/v6/run.html
测试方式：直接在浏览器上打开该网页即可，测试结果会直接显示在网页上。
说明：测试得分越高说明浏览器的网页显示速度越快。
6、  Dromaeo benchmark
由Mozilla基金会发布，它提供了浏览器表现的第三方视角。
测试网址：[http://dromaeo.com/](http://dromaeo.com/)
测试方式：在浏览器上打开该网页，可以选择自己想做的测试。
