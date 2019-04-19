# 解决GOOGLE Docs 打不开的方法 - 三少GG - CSDN博客
2010年12月02日 14:55:00[三少GG](https://me.csdn.net/scut1135)阅读数：8309标签：[google																[中国电信																[microsoft																[浏览器																[gmail																[电信](https://so.csdn.net/so/search/s.do?q=电信&t=blog)](https://so.csdn.net/so/search/s.do?q=gmail&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=中国电信&t=blog)](https://so.csdn.net/so/search/s.do?q=google&t=blog)
个人分类：[杂记/八卦](https://blog.csdn.net/scut1135/article/category/684984)
### 解决GOOGLE打不开文档的方法
[默认分类](http://hkdliujie.blog.163.com/blog/#m=0&t=1&c=fks_084066092086081069080086074066085083089064084094081)2009-10-09 14:16:28阅读3178评论4  字号：大中小订阅
中国电信这两天不知道吃错了什么药，昨天搞Google的DNS，今天又搞Google Docs，今天发现深圳地区的电信线路无法访问Google 
Docs，不过其他地区情况未知。
　　经过Tracert的测试，最后一个可访问的IP是中国电信骨干网广州的一个路由器，因此确定Google 
Docs的一些IP可能被屏蔽了，不过用户可以通过添加镜像IP的方法继续访问Google Docs服务。
**　方法是就是编辑HOSTS文件，请编辑你的C:/Windows/system32/drivers/etc/hosts文件，在文件结尾加上72.14.235.100 docs.google.com，保存退出，这时再打开一个浏览器，你就可以正常访问Google Docs服务了。**
**　　要是我们真的不用Microsoft Office而全用Google Docs的话，那么我们的命运真的就全部掌握在中国电信这个流氓手里了。**
其实直接输入IP一样的访问docs，还好电信只是屏蔽域名，要是IP的话一样的完，Gmail估计是没办法了。
DOCS最近是时断时续，等你编辑好后，突然提示网络故障，不能保存，又不能关浏览器，你就哭吧。
还好DOCS有离线编辑功能，没有网络连接时一样能用，连网后自动同步。
以下为DOCS离线功能的使用帮助：
此文转载自新席地网：[http://newxd.com/article.asp?id=3510](http://newxd.com/article.asp?id=3510)
