
# Systemd基础：5：定时器Timer的实现示例 - 知行合一 止于至善 - CSDN博客

2019年04月09日 20:46:05[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：20标签：[linux																](https://so.csdn.net/so/search/s.do?q=linux&t=blog)[systemd																](https://so.csdn.net/so/search/s.do?q=systemd&t=blog)[timer																](https://so.csdn.net/so/search/s.do?q=timer&t=blog)[计时器																](https://so.csdn.net/so/search/s.do?q=计时器&t=blog)[例子																](https://so.csdn.net/so/search/s.do?q=例子&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=计时器&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=timer&t=blog)个人分类：[Unix/Linux																](https://blog.csdn.net/liumiaocn/article/category/6328278)
[
																								](https://so.csdn.net/so/search/s.do?q=timer&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=systemd&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=systemd&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=linux&t=blog)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
在前面的文章中对于[service]的介绍中提到，并非所有的systemd配置文件都有此段的设定，本文将介绍一下在Systemd下使用定时器的方式。
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 场景介绍
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
> 在这个示例中将会通过一个计时器和一个服务进行关联，达到每分钟调用的目的。服务使用前面文章中所
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 服务示例
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
# cat liumiaocn.service
```
```python
[
```
```python
Unit
```
```python
]
```
```python
Description
```
```python
=
```
```python
Systemd Service Sample By liumiaocn
Documentation
```
```python
=
```
```python
https://liumiaocn.blog.csdn.net/
```
```python
[
```
```python
Service
```
```python
]
```
```python
WorkingDirectory
```
```python
=
```
```python
/tmp/systemd_working_dir
ExecStart
```
```python
=
```
```python
/bin/sh -c
```
```python
'/usr/bin/echo "hello world" >>/tmp/systemd_working_dir/hello.log'
```
```python
[
```
```python
Install
```
```python
]
```
```python
WantedBy
```
```python
=
```
```python
multi-user.target
```
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# Timer示例
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)`[root@liumiaocn system]# cat liumiaocn.timer 
[Unit]
Description=Systemd Sample: say hello every min
[Timer]
OnCalendar=*-*-* *:*:00
Unit=liumiaocn.service
[Install]
WantedBy=timers.target
[root@liumiaocn system]#`[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)代码说明：
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)文件名称以timer为后缀，可以清楚地确认出其计时器的作用
通过Unit与执行的服务进行关联
简单的情况下可以直接设定OnCalendar进行设定
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)OnCalendar的设定格式示例
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)|日期格式|OnCalendar设定格式|
|---|---|
|Sat,Thu,Mon-Wed,Sat-Sun|Mon-Thu,Sat,Sun|-|-* 00:00:00|
|Mon,Sun 12-|-|2,1:23|Mon,Sun 2012-|-|01,02:23:00|
|Wed *-1|Wed|-|-01 00:00:00|
|Wed-Wed,Wed *-1|Wed|-|-01 00:00:00|
|Wed, 17:48|Wed|-|-* 17:48:00|
|Wed-Sat,Tue 12-10-15 1:2:3|Tue-Sat 2012-10-15 01:02:03|
|-|-7 0:0:0|-|-07 00:00:00|
|10-15|*-10-15 00:00:00|
|monday|-12-|17:00|Mon|-12-|17:00:00|
|Mon,Fri|-|-3,1,2 *:30:45|Mon,Fri|-|-01,02,03 *:30:45|
|12,14,13,12:20,10,30|-|-* 12,13,14:10,20,30:00|
|mon,fri *-1/2-1,3 *:30:45|Mon,Fri *-01/2-01,03 *:30:45|
|03-05 08:05:40|*-03-05 08:05:40|
|08:05:40|-|-* 08:05:40|
|05:40|-|-* 05:40:00|
|Sat,Sun 12-05 08:05:40|Sat,Sun *-12-05 08:05:40|
|Sat,Sun 08:05:40|Sat,Sun|-|-* 08:05:40|
|2003-03-05 05:40|2003-03-05 05:40:00|
|2003-03-05|2003-03-05 00:00:00|
|03-05|*-03-05 00:00:00|
|hourly|-|-* *:00:00|
|daily|-|-* 00:00:00|
|monthly|-|-01 00:00:00|
|weekly|Mon|-|-* 00:00:00|
|*:20/15|-|-* *:20/15:00|
# daemon-reload重新加载
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@liumiaocn ~
```
```python
]
```
```python
# systemctl daemon-reload
```
```python
[
```
```python
root@liumiaocn ~
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 创建workingdirectory
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@liumiaocn ~
```
```python
]
```
```python
# mkdir -p /tmp/systemd_working_dir
```
```python
[
```
```python
root@liumiaocn ~
```
```python
]
```
```python
# ls /tmp/systemd_working_dir
```
```python
[
```
```python
root@liumiaocn ~
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# enable 计时器
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
# systemctl enable liumiaocn.timer
```
```python
Created symlink from /etc/systemd/system/timers.target.wants/liumiaocn.timer to /usr/lib/systemd/system/liumiaocn.timer.
```
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 启动计时器
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
# systemctl start liumiaocn.timer
```
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 确认状态
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
# systemctl is-enabled liumiaocn.timer
```
```python
enabled
```
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
#
```
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
# systemctl is-active liumiaocn.timer
```
```python
active
```
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 结果确认
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)等几分钟后，通过重定向的文件可以看到timer的多次执行结果
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
# cat /tmp/systemd_working_dir/hello.log
```
```python
hello world
hello world
hello world
```
```python
[
```
```python
root@liumiaocn system
```
```python
]
```
```python
#
```
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 参考文章
[
](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)[https://www.certdepot.net/rhel7-use-systemd-timers/](https://www.certdepot.net/rhel7-use-systemd-timers/)
[
            ](https://img-blog.csdnimg.cn/2019040819445927.jpeg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)

