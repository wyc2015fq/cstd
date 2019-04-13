
# Ubuntu重置密码 passwd 出现 Authentication token manipulation error - 追求卓越,做到专业 - CSDN博客


2018年05月04日 14:21:37[Waldenz](https://me.csdn.net/enter89)阅读数：295


使用passwd命令重新设定密码：passwd walden，(walden是系统中已有的username)，并输入两次密码，出现
Authentication token manipulation error错误。如图
![](https://img-blog.csdn.net/20180504135254248?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解决方法，输入如下命令
```python
mount -o rw,remount /
```
则再输入 passwd walden，则可以重置密码成功；

参考外国朋友意见[https://linhost.info/2013/08/passwd-authentication-token-manipulation-error-ubuntu/](https://linhost.info/2013/08/passwd-authentication-token-manipulation-error-ubuntu/)
![](https://img-blog.csdn.net/20180504140124330?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


