# zabbix cpu 负载不对的原因 - DoubleLi - 博客园






最近给客户安装了一个zabbix服务器，运行了几天发现cpu load值不准确，

请教了运维和系统工程师，说是zabbix2.0以后的问题。




解决方案如下1(推荐)：

修改模板（Template OS Linux）--项目--Processor load (1 min average per core)--键值：



把 system.cpu.load[percpu,avg1] 改为 system.cpu.load[all,avg1] 




解决方案如下2：

修改模板（Template OS Linux）--项目--Processor load (1 min average per core)--使用自定义倍数（勾选）--填写倍数（CPU的核数，8核机器就写8）--（建议将“数据更新间隔(秒)”改为3秒）--存档（保存），如果监控的服务器有的是8核有的是4核，那么需要复制多个模板（使用模板的“全克隆”），不同核数的主机选择相应的模板





同样修改Processor load (5 min average per core)和Processor load (15 min average per core)两个项目





监控windows的CPU：

[http://blog.sina.com.cn/s/blog_62079f620101b48k.html](http://blog.sina.com.cn/s/blog_62079f620101b48k.html)









