
# Systemd基础篇：workingdirectory事前不创建会导致的问题 - 知行合一 止于至善 - CSDN博客

2019年04月09日 04:34:33[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：62


![在这里插入图片描述](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
Systemd下的服务配置，如果设定使用了workingdirectory，但是在systemctl start之前没有创建该目录，将会无法正常启动，并且根据缺省方式下的status提示信息和journalctl无法清楚地定位问题，尤其是在systemd的设定较多时，使用这篇文章memo一下。
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 再现示例: 事前准备
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
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
/usr/bin/echo
```
```python
"hello world"
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
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 启动与status确认
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)可以看到提示的错误信息中有200/CHDIR，在知道原因之后会发现这已经给予了提示，但是较为隐蔽。
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
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
# systemctl status liumiaocn
```
```python
● liumiaocn.service - Systemd Service Sample By liumiaocn
   Loaded: loaded
```
```python
(
```
```python
/usr/lib/systemd/system/liumiaocn.service
```
```python
;
```
```python
disabled
```
```python
;
```
```python
vendor preset: disabled
```
```python
)
```
```python
Active: failed
```
```python
(
```
```python
Result: exit-code
```
```python
)
```
```python
since Fri 2019-04-05 20:04:04 CST
```
```python
;
```
```python
13s ago
     Docs: https://liumiaocn.blog.csdn.net/
  Process: 5726 ExecStart
```
```python
=
```
```python
/usr/bin/echo hello world
```
```python
(
```
```python
code
```
```python
=
```
```python
exited, status
```
```python
=
```
```python
200/CHDIR
```
```python
)
```
```python
Main PID: 5726
```
```python
(
```
```python
code
```
```python
=
```
```python
exited, status
```
```python
=
```
```python
200/CHDIR
```
```python
)
```
```python
Apr 05 20:04:04 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Started Systemd Service Sample By liumiaocn.
Apr 05 20:04:04 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: liumiaocn.service: main process exited, code
```
```python
=
```
```python
exited, status
```
```python
=
```
```python
200/CHDIR
Apr 05 20:04:04 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Unit liumiaocn.service entered failed state.
Apr 05 20:04:04 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: liumiaocn.service failed.
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
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# journalctl确认
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)从journalctl中也看不出来有价值的内容
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
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
# journalctl -u liumiaocn
```
```python
-- Logs begin at Fri 2019-03-29 13:08:00 CST, end at Fri 2019-04-05 20:25:38 CST. --
Apr 05 20:03:14 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Started Systemd Service Sample By liumiaocn.
Apr 05 20:03:14 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: liumiaocn.service: main process exited, code
```
```python
=
```
```python
exited, status
```
```python
=
```
```python
200/CHDIR
Apr 05 20:03:14 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Unit liumiaocn.service entered failed state.
Apr 05 20:03:14 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: liumiaocn.service failed.
Apr 05 20:03:44 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Started Systemd Service Sample By liumiaocn.
Apr 05 20:03:44 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: liumiaocn.service: main process exited, code
```
```python
=
```
```python
exited, status
```
```python
=
```
```python
200/CHDIR
Apr 05 20:03:44 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Unit liumiaocn.service entered failed state.
Apr 05 20:03:44 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: liumiaocn.service failed.
Apr 05 20:04:04 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Started Systemd Service Sample By liumiaocn.
Apr 05 20:04:04 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: liumiaocn.service: main process exited, code
```
```python
=
```
```python
exited, status
```
```python
=
```
```python
200/CHDIR
Apr 05 20:04:04 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: Unit liumiaocn.service entered failed state.
Apr 05 20:04:04 liumiaocn systemd
```
```python
[
```
```python
1
```
```python
]
```
```python
: liumiaocn.service failed.
```
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 创建目录后能正常动作
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)`[root@liumiaocn system]# mkdir -p /tmp/systemd_working_dir
[root@liumiaocn system]# systemctl start liumiaocn
[root@liumiaocn system]# echo $?
0
[root@liumiaocn system]# 
[root@liumiaocn system]# systemctl status liumiaocn
● liumiaocn.service - Systemd Service Sample By liumiaocn
   Loaded: loaded (/usr/lib/systemd/system/liumiaocn.service; disabled; vendor preset: disabled)
   Active: inactive (dead)
     Docs: https://liumiaocn.blog.csdn.net/
Apr 05 20:03:44 liumiaocn systemd[1]: Started Systemd Service Sample By liumiaocn.
Apr 05 20:03:44 liumiaocn systemd[1]: liumiaocn.service: main process exited, code=exited, status=200/CHDIR
Apr 05 20:03:44 liumiaocn systemd[1]: Unit liumiaocn.service entered failed state.
Apr 05 20:03:44 liumiaocn systemd[1]: liumiaocn.service failed.
Apr 05 20:04:04 liumiaocn systemd[1]: Started Systemd Service Sample By liumiaocn.
Apr 05 20:04:04 liumiaocn systemd[1]: liumiaocn.service: main process exited, code=exited, status=200/CHDIR
Apr 05 20:04:04 liumiaocn systemd[1]: Unit liumiaocn.service entered failed state.
Apr 05 20:04:04 liumiaocn systemd[1]: liumiaocn.service failed.
Apr 05 20:12:38 liumiaocn systemd[1]: Started Systemd Service Sample By liumiaocn.
Apr 05 20:12:38 liumiaocn echo[5746]: hello world
[root@liumiaocn system]#`[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)
# 总结
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)Systemd的配置文件，如果使用到workingdirectory，需要事先保证该目录的创建。至于问题的追踪，可以考虑使用systemd提供的debug日志进行确认，至少会比确认status=200/CHDIR的信息要容易地多。
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)关于使用systemd下进行debug的方式，可参看如下文章：
[
](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)[https://liumiaocn.blog.csdn.net/article/details/89086548](https://liumiaocn.blog.csdn.net/article/details/89086548)
[
            ](https://img-blog.csdnimg.cn/20190408194655324.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saXVtaWFvY24uYmxvZy5jc2RuLm5ldA==,size_16,color_FFFFFF,t_70)

