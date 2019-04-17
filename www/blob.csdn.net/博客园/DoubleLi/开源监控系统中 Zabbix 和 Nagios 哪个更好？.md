# 开源监控系统中 Zabbix 和 Nagios 哪个更好？ - DoubleLi - 博客园







监控平台的话，各有优劣，但基本都可以满足需求。
等达到一定监控指标后，发现，最困难的是监控项目的管理。 CMDB
中小规模（服务器<=1k）：Zabbix
大规模（1k>=服务器<=10k）：Nagios进行二次开发
超大规模（服务器>=10k）：开发适应自己平台的监控软件吧
另推荐个牛逼的东西：[http://prometheus.io](https://link.zhihu.com/?target=http%3A//prometheus.io)


作者：好撑
链接：https://www.zhihu.com/question/19973178/answer/131911060
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。



链接：https://www.zhihu.com/question/19973178/answer/19666150
来源：知乎



我比较看好zabbix这款监控软件，理由如下：
1.分布式监控，天生具有的功能，适合于构建分布式监控系统，具有node，proxy2种分布式模式
2.自动化功能，自动发现，自动注册主机，自动添加模板，自动添加分组，是天生的自动化运维利器的首选，当然于自动化运维工具搭配，puppet+zabbix，或者saltstack+zabbix，那是如鱼得水。
3.自定义监控比较方便，自定义监控项非常简单，支持变量，支持low level  discovery，可以参考我写的文档[自动化运维之监控篇---利用zabbix自动发现功能实现批量web url监控](https://link.zhihu.com/?target=http%3A//itnihao.blog.51cto.com/1741976/1129725)
4.触发器，也就是报警条件有多重判断机制，当然，这个需要你去研究一下，这也是zabbix的精华之处，
5.支持多种监控方式，agentd，snmp，ipmi，jmx，逻辑图如下


6.提供api功能，二次开发方便，你可以选用zabbix来进行二次深度开发，结合cmdb资产管理系统，业务管理系统，从而使你的自动化运维系统达到新的高度。
7.当然zabbix还有很多其他功能，这里不一一介绍了。
很多人说zabbix不简单，其实是zabbix的设计理念有些超前，当你都研究到一定程度，你不得不佩服zabbix的团队是非常强悍的，这种工作机制也是相对先进的。
   国内的大厂，都有一套自己的监控系统，自己设计，自己开发，其功能也能和zabbix一样，更能适合于自己的需求，但一般企业用，特别是中型互联网公司，还是极力推荐zabbix。
另外附上我的文档[Zabbix使用手册V1.4.pdf](https://link.zhihu.com/?target=http%3A//vdisk.weibo.com/s/EyvbQ)，这里面有我的经验总结，以及一些使用心得与技巧
最后建议大家多看官方文档
新浪微盘下载地址：最新文档版本为[Zabbix使用手册V2.0.pdf](https://link.zhihu.com/?target=http%3A//vdisk.weibo.com/s/dlQQwAEtuXKT/1389686000)
百度网盘下载地址：[Zabbix使用手册V2.0.pdf_免费高速下载](https://link.zhihu.com/?target=http%3A//pan.baidu.com/s/1dDEb7Rr)



同时提供zabbix的安装二次定制的RPM包，该项目地址为：
[https://github.com/itnihao/zabbix-rpm/tree/master/zabbix-2.2.2](https://link.zhihu.com/?target=https%3A//github.com/itnihao/zabbix-rpm/tree/master/zabbix-2.2.2)
[Zabbix企业级分布式监控系统](https://link.zhihu.com/?target=http%3A//search.jd.com/Search%3Fkeyword%3DZabbix%25E4%25BC%2581%25E4%25B8%259A%25E7%25BA%25A7%25E5%2588%2586%25E5%25B8%2583%25E5%25BC%258F%25E7%259B%2591%25E6%258E%25A7%25E7%25B3%25BB%25E7%25BB%259F%26enc%3Dutf-8%26book%3Dy)











