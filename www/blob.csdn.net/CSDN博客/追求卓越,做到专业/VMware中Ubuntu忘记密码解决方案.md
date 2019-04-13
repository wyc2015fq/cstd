
# VMware中Ubuntu忘记密码解决方案 - 追求卓越,做到专业 - CSDN博客


2018年05月04日 14:20:35[Waldenz](https://me.csdn.net/enter89)阅读数：240


1. 重启Ubuntu虚拟机，随即长按shift进入grub菜单；
2. 选择recovery mode，回车确认；
3. 在Recovery Menu中，选择“Root Drop to root shell prompt”，回车；如图
![](https://img-blog.csdn.net/20180504134511559?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4. 进入shell界面，使用passwd命令重新设定密码：passwd walden，(walden是系统中已有的username)，回车确认；
看到密码设定成功提示：passwd: password updated successfully
![](https://img-blog.csdn.net/20180504135254248?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果出现 Authentication token manipulation error, 则可以先输入
```python
mount -o rw,remount /
```
则再输入 passwd walden，则可以重置密码成功；注意： remount和/中间有一空格
[Ubuntu重置密码 passwd 出现 Authentication token manipulation error](https://mp.csdn.net/postedit/80194487)
5. 重启系统：sudo reboot；

