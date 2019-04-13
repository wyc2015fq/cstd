
# Systemd基础：3：写第一个HelloWorld的Systemd服务 - 知行合一 止于至善 - CSDN博客

2019年04月09日 04:06:30[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：71


这篇文章通过一个最为简单的服务来说明systemd服务的使用。
# Step 1: 设定配置文件
配置文件示例
> 文件名：/usr/lib/systemd/system/liumiaocn.service

## 配置文件内容
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
# systemctl cat liumiaocn
```
```python
# /usr/lib/systemd/system/liumiaocn.service
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
ExecStart
```
```python
=
```
```python
/usr/bin/echo
```
```python
"hello liumiaocn"
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
root@liumiaocn ~
```
```python
]
```
```python
#
```
## 设定选项说明
|设定内容|说明|
|---|---|
|Description|对服务的描述内容|
|Documentation|文档地址信息说明|
|ExecStart|启动当前服务的命令|
|WantedBy|用于设定systemd的target（类似于init的RunLevel），根据enable/disable对ln进行操作|
此时如果进行确认，则会得到如下结果
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
Active: inactive
```
```python
(
```
```python
dead
```
```python
)
```
```python
Docs: https://liumiaocn.blog.csdn.net/
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
可以看到description和documentation的设定内容。
# Step 2: 通过systemctl启动服务
## daemon-reload
一旦配置文件有所变更，则需要通过daemon-reload命令执行来进行加载
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
## 启动服务
使用systemctl start liumiaocn
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
# systemctl start liumiaocn
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
# echo $?
```
```python
0
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
然后使用status来确认结果，可以看到
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
Active: inactive
```
```python
(
```
```python
dead
```
```python
)
```
```python
Docs: https://liumiaocn.blog.csdn.net/
Apr 05 22:01:01 liumiaocn systemd
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
Apr 05 22:01:01 liumiaocn echo
```
```python
[
```
```python
4129
```
```python
]
```
```python
: hello liumiaocn
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
之所以是dead的状态，这是因为echo的命令无法作为守护进程在后台一直运行，而是执行一次就结束了，这也可以从status的执行结果看到输出的情况。
# Step 3: enable服务
使用enable命令进行服务开机启动设定，而实际就是创建相关链接,从下面的日志中可以清楚地看到这一点。
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
# systemctl enable liumiaocn
```
```python
Created symlink from /etc/systemd/system/multi-user.target.wants/liumiaocn.service to /usr/lib/systemd/system/liumiaocn.service.
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
# ls -l /etc/systemd/system/multi-user.target.wants/liumiaocn.service
```
```python
lrwxrwxrwx 1 root root 41 Apr  5 22:04 /etc/systemd/system/multi-user.target.wants/liumiaocn.service -
```
```python
>
```
```python
/usr/lib/systemd/system/liumiaocn.service
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
# systemctl is-enabled liumiaocn
```
```python
enabled
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
# 其他相关文章
目录：Systemd实践指南
[https://liumiaocn.blog.csdn.net/article/details/89079485](https://liumiaocn.blog.csdn.net/article/details/89079485)
总结
这篇文章用4行的systemd的一个示例文件，介绍了使用systemd服务的使用过程。

