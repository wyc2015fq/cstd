# zabbix 监控特定进程 - DoubleLi - 博客园






由于一些服务器上跑着一些重要程序，需要对它们进行监控，公司用的是zabbix监控，之前都是在zabbix中添加自定义脚本对特定程序进行监控，最近看了zabbix的官方文档，发现原来强大的zabbix居然能监控程序是否运行，并能监控其使用的内存大小，以下是我的实践记录：

      1. 在特定机器或模板上创建新的监控项，点击Key 后面的Select 按钮，选择如下两项，一项是用来监控特定进程的数量，另一项是用来监控进程使用内存的大小。

![](https://img-blog.csdn.net/20140519170928140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFpNDUxOTU0NzA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

    2.以下是对squid进程的监控配置，key中的参数说明，第一个参数是进程名字，没必要填写，填了反而会使监控不太准确（仅个人测试），第二个参数是运行进程的用户名，第三个为进程的状态 ，包括：*all* (default), *run*, *sleep*, *zomb* ，第四个参数用来指定进程名中包含的字符，对进程进行过滤。

![](https://img-blog.csdn.net/20140519170947734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFpNDUxOTU0NzA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

3.配置好监控项后，添加触发器，如下触发器表示最后两次的值都是0，说明没有squid进程在运行，则出发报警。

![](https://img-blog.csdn.net/20140519171007437?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFpNDUxOTU0NzA2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)











