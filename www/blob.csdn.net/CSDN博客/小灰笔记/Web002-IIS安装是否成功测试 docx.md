# Web002-IIS安装是否成功测试.docx - 小灰笔记 - CSDN博客





2018年01月06日 01:47:33[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：104
个人分类：[Web](https://blog.csdn.net/grey_csdn/article/category/7385134)








如同测试LAMP，写个简单的PHP脚本就能够测试一下自己搭建的服务是否生效。比较不错的是IIS配置完成之后这部分用于检测的功能已经存在。
直接在浏览器中输入http://localhost即可，如果配置成功将会出现下面的页面。
![](https://img-blog.csdn.net/20180106014634569?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其实，这个页面倒是也同时解答了我的另一个疑惑。我安装的IIS到底是什么版本？是IIS6还是IIS7？通过这个检测，很容易知道了，因为中间的这个IIS7的字眼太引人注目了。这样，联想一下现在公司中让我一直头疼的工作中使用的应该也是这个了。很遗憾的是，公司电脑中的localhost解析不能用，究竟是如何配置导致的暂时也弄不明白。这是我后期学习中需要探索hack的一个小目标了。
            


