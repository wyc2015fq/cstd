# 034_PowerShell中的HOME环境变量 - 小灰笔记 - CSDN博客





2018年12月13日 22:55:09[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：53








         初学（应该叫做第二次初学）PowerShell，感觉PowerShell的很多设计方式跟曾经接触的linux有一些相似之处。有很多常用的命令、环境变量也与之前的习惯相符。不过，今天尝试做一个简单的测试发现一直不成功，后来发现其实是HOME的定义有关。

         先做一个bash的简单演示：

![](https://img-blog.csdnimg.cn/20181213225454760.png)

         应已经能够看明白了，这里的HOME的概念其实是被我定义成了一个C盘下面的目录。接下来在PowerShell中做一个类似的操作：

![](https://img-blog.csdnimg.cn/20181213225454784.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2dyZXlfY3Nkbg==,size_16,color_FFFFFF,t_70)

         最终导致我测试失败的就是这个，~在unix中一般是指home目录，我经常使用bash命令，Windows下也定义了这样的环境变量去模拟。不过，PowerShell中似乎有自己另一套识别机制。CMD中查看了一下环境变量信息：

![](https://img-blog.csdnimg.cn/20181213225454789.png)

         发现上面截图中的HOMEPATH似乎才是PowerShell寻找的位置，看起来这里还是有一点细微的差别需要注意。



