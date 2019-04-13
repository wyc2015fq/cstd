
# Fiddler在Chrome中的问题。 - 追求卓越,做到专业 - CSDN博客


2014年04月18日 11:19:52[Waldenz](https://me.csdn.net/enter89)阅读数：2850


由于经常访问google文档，gmail等，所以安装[FreeMan](http://dongtaiwang.com/loc/phome.php?v=0)，在Chrome中安装[Proxy
 Swicth](https://chrome.google.com/webstore/detail/proxy-switchysharp/dpplabbmogkhghncfbfdeeokoefdjegm)控制代理的地址。 问题是Fiddler没有捕获到Chrome的URL，原来是由于Proxy Switch本身也是设置HTTP代理地址，所以和Fiddler有了冲突，暂时就只能禁用Proxy Switch 来使Fiddler监控Chrome。各位同学如果有什么建议，欢迎指导。

