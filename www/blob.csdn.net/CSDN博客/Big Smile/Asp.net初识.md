# Asp.net初识 - Big Smile - CSDN博客
2016年04月24日 18:40:48[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1365标签：[asp.net](https://so.csdn.net/so/search/s.do?q=asp.net&t=blog)
个人分类：[B/S																[编码学习																[Asp.net](https://blog.csdn.net/tr1912/article/category/6200039)](https://blog.csdn.net/tr1912/article/category/5787481)](https://blog.csdn.net/tr1912/article/category/6184179)
   最近学习Asp.net的相关知识，对于Asp.net是什么东西，怎么学习有些生疏，在此总结一下Asp.net的内容。
# 一、导图
![](https://img-blog.csdn.net/20160424180648591?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        总的来说，Asp.net是一种使嵌入网页中的脚本可由因特网服务器执行的服务器端脚本技术，Asp.net是.netFrameWork中的成员之一，属于动态网页技术中的一种，那么什是动态网页技术呢？
**        动态网页技术**是指的网页URL不固定，能通过后台与用户交互。完成用户查询，提交等动作。
# **二、那么我们为什么要用Asp.net来编辑网页呢？**
用Asp.net的好处在于，Asp.net支持的语言众多，.net中的语言基本都是支持的，而且由于是运行在.netFrameWork框架上的程序，所以跨平台性也很好，在什么平台的浏览器上基本都可以流畅运行，而且由于平台的特性，他对于多核心多线程的CPU有很好的支持，可以提高网页的运行效率。
# 三、学习Asp.net
        我们在学习的时候要注意，学习Asp.net学习的是动态网页的设计，是基于脚本和后台代码的一种综合页面设计，这是和设计HTML网页的最大区别。动态的网页难点就在于数据的传递、数据的安全、用户的体验和多浏览器的动态支持。
![](https://img-blog.csdn.net/20160424182530083?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        在看视频的时候，老师强调的两点，第一就是网页的加载和数据的传递，有一张图很生动（不明）的解释了访问网页时的数据传输过程，这个过程的理解对于编写网页代码和设计效果很有帮助。
![](https://img-blog.csdn.net/20160424183048418?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**解释一下这张图：**用户查看网页的时候先要提出HTTP请求，要求服务器把处理后的网页传回本机，然后就是用户端的网页在提交请求后去服务器运行相应的后台程序/脚本，生成新的HTML页发回用户端，进行了更改的网页在客户端得到了显示。
        在这个过程中一共对服务器进行了两次请求，也就是说进行一次更改要向服务器发送两次请求，在这两次的事件中插入相应的代码是非常重要的。
# **四、总结**
        ASP.NET不是一种语言，而是创建动态Web页的一种强大的服务器端技术，它是Microsoft .NET Framework中一套用于生成Web应用程序和Web服务的技术，利用公共语言运行时（Common Language Runtime）在服务器后端为用户提供建立强大的企业级Web应用服务的编程框架。
        我们在学习Asp.net的时候要注意对比着HTML来学习，更要注意动态网页的请求方式，前台和后台代码的交互也就是Asp.net的根本所在，我们以后要学习的JS以及Ajax和JQurey都是要基于这些东西来的所以要打好这里的基础非常的重要。
