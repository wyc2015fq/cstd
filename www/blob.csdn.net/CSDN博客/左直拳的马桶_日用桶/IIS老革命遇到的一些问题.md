# IIS老革命遇到的一些问题 - 左直拳的马桶_日用桶 - CSDN博客
2015年07月08日 15:12:23[左直拳](https://me.csdn.net/leftfist)阅读数：1187标签：[iis](https://so.csdn.net/so/search/s.do?q=iis&t=blog)
个人分类：[IIS](https://blog.csdn.net/leftfist/article/category/2459577)
今天部署一个网站到IIS，遇到了一些问题。老革命遇上新问题。前不久搞java，接触了一下tomcat，觉得真麻烦。而tomcat大概是java阵营中最简单的了吧。想不到，IIS7，友好的图形界面下，也有很多坑。总结如下：
1、服务器如果为64位操作系统，那么对于我们平时在32位机器上开发的系统，应用程序池或许需要开启32位应用程序选项。通常问题可能是出在一些支撑的DLL上，换成64位的应该就OK。否则我们要在应用程序池上开启32位模式： 
方法是右击该应用程序池，高级设置，启用32位应用程序选项。
2、  如果提示“由于 Web 服务器上的“ISAPI 和 CGI 限制”列表设置，无法提供您请求的页面”，除了考虑是否需要注册asp.net外，还要注意IIS是否禁用了.NET4.0： 
注意这个“ISAPI 和 CGI 限制”选项需要点击IIS的根节点，也就是服务器节点才可看到。
![这里写图片描述](https://img-blog.csdn.net/20150708151109817)
![这里写图片描述](https://img-blog.csdn.net/20150708151126338)
3、  服务器也许默认关闭了服务：ASP.NET State Service，需要开启，并设为自动开启。
