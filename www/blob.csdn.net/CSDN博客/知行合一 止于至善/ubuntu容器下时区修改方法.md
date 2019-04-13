
# ubuntu容器下时区修改方法 - 知行合一 止于至善 - CSDN博客

2019年04月12日 05:32:15[liumiaocn](https://me.csdn.net/liumiaocn)阅读数：33标签：[ubuntu																](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)[容器																](https://so.csdn.net/so/search/s.do?q=容器&t=blog)[时区修改																](https://so.csdn.net/so/search/s.do?q=时区修改&t=blog)[tzdata																](https://so.csdn.net/so/search/s.do?q=tzdata&t=blog)[timedatectl																](https://so.csdn.net/so/search/s.do?q=timedatectl&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=tzdata&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=时区修改&t=blog)个人分类：[Docker																](https://blog.csdn.net/liumiaocn/article/category/6328276)
[
																								](https://so.csdn.net/so/search/s.do?q=时区修改&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=容器&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=容器&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)

这篇文章简单memo一下ubuntu或者debian容器下的时区设定。这本来是timedatectl 直接设定就可以解决的问题。
# 问题1: No such file or directory错误提示
```python
sh-4.3$ timedatectl
Failed to create bus connection: No such
```
```python
file
```
```python
or directory
sh-4.3$
```
确认之后，发现没有设定/etc/localtime，决定直接设定与设定指向Asia/Shanghai的链接进行解决。
# 问题2: 权限不足
```python
sh-4.3$
```
```python
ln
```
```python
-sf /usr/share/zoneinfo/Asia/Shanghai  /etc/localtime
ln: cannot remove
```
```python
'/etc/localtime'
```
```python
:
```
```python
Permission denied
sh-4.3$
```
使用sudo解决问题
```python
sh-4.3$
```
```python
sudo
```
```python
ln
```
```python
-sf /usr/share/zoneinfo/Asia/Shanghai  /etc/localtime
sh-4.3$
```
```python
ls
```
```python
/etc/localtime
/etc/localtime
sh-4.3$
```
# 问题3: 不存在zoneinfo
```python
sh-4.3$
```
```python
ls
```
```python
-l /usr/share/zoneinfo/Asia/Shanghai
ls: cannot access
```
```python
'/usr/share/zoneinfo/Asia/Shanghai'
```
```python
:
```
```python
No such
```
```python
file
```
```python
or directory
sh-4.3$
```
因为缺少tzdata，安装tzdata，提示如下问题
# 问题4: unable to locate package
```python
sh-4.3$
```
```python
sudo
```
```python
apt-get
```
```python
install
```
```python
tzdata
Reading package lists
```
```python
..
```
```python
. Done
Building dependency tree       
Reading state information
```
```python
..
```
```python
. Done
E: Unable to
```
```python
locate
```
```python
package tzdata
sh-4.3$
```
因为apt-get没有更新本地源，apt-get update解决问题
```python
sh-4.3$
```
```python
apt-get
```
```python
update
Reading package lists
```
```python
..
```
```python
. Done
E: List directory /var/lib/apt/lists/partial is missing. - Acquire
```
```python
(
```
```python
13: Permission denied
```
```python
)
```
```python
sh-4.3$
```
```python
sudo
```
```python
apt-get
```
```python
update
Get:1 mirror://mirrors.ubuntu.com/mirrors.txt xenial InRelease
```
```python
[
```
```python
247 kB
```
```python
]
```
```python
Get:2 mirror://mirrors.ubuntu.com/mirrors.txt xenial-updates InRelease
```
```python
[
```
```python
109 kB
```
```python
]
```
```python
Get:3 mirror://mirrors.ubuntu.com/mirrors.txt xenial-backports InRelease
```
```python
[
```
```python
107 kB
```
```python
]
```
```python
Get:4 mirror://mirrors.ubuntu.com/mirrors.txt xenial-security InRelease
```
```python
[
```
```python
109 kB
```
```python
]
```
```python
Get:5 mirror://mirrors.ubuntu.com/mirrors.txt xenial/main amd64 Packages
```
```python
[
```
```python
1558 kB
```
```python
]
```
```python
Get:6 mirror://mirrors.ubuntu.com/mirrors.txt xenial/restricted amd64 Packages
```
```python
[
```
```python
14.1 kB
```
```python
]
```
```python
Get:7 mirror://mirrors.ubuntu.com/mirrors.txt xenial/universe amd64 Packages
```
```python
[
```
```python
9827 kB
```
```python
]
```
```python
Get:8 mirror://mirrors.ubuntu.com/mirrors.txt xenial/multiverse amd64 Packages
```
```python
[
```
```python
176 kB
```
```python
]
```
```python
Get:9 mirror://mirrors.ubuntu.com/mirrors.txt xenial-updates/main amd64 Packages
```
```python
[
```
```python
1211 kB
```
```python
]
```
```python
Get:10 mirror://mirrors.ubuntu.com/mirrors.txt xenial-updates/restricted amd64 Packages
```
```python
[
```
```python
13.1 kB
```
```python
]
```
```python
Get:11 mirror://mirrors.ubuntu.com/mirrors.txt xenial-updates/universe amd64 Packages
```
```python
[
```
```python
962 kB
```
```python
]
```
```python
Get:12 mirror://mirrors.ubuntu.com/mirrors.txt xenial-updates/multiverse amd64 Packages
```
```python
[
```
```python
19.1 kB
```
```python
]
```
```python
Get:13 mirror://mirrors.ubuntu.com/mirrors.txt xenial-backports/main amd64 Packages
```
```python
[
```
```python
7942 B
```
```python
]
```
```python
Get:14 mirror://mirrors.ubuntu.com/mirrors.txt xenial-backports/universe amd64 Packages
```
```python
[
```
```python
8532 B
```
```python
]
```
```python
Get:15 mirror://mirrors.ubuntu.com/mirrors.txt xenial-security/main amd64 Packages
```
```python
[
```
```python
816 kB
```
```python
]
```
```python
Get:16 mirror://mirrors.ubuntu.com/mirrors.txt xenial-security/restricted amd64 Packages
```
```python
[
```
```python
12.7 kB
```
```python
]
```
```python
Get:17 mirror://mirrors.ubuntu.com/mirrors.txt xenial-security/universe amd64 Packages
```
```python
[
```
```python
551 kB
```
```python
]
```
```python
Get:18 mirror://mirrors.ubuntu.com/mirrors.txt xenial-security/multiverse amd64 Packages
```
```python
[
```
```python
6117 B
```
```python
]
```
```python
Fetched 15.8 MB
```
```python
in
```
```python
1min 3s
```
```python
(
```
```python
250 kB/s
```
```python
)
```
```python
Reading package lists
```
```python
..
```
```python
. Done
sh-4.3
```
# 对应方法：安装tzdata
安装tzdata，问题得到解决。
```python
sh-4.3$
```
```python
sudo
```
```python
apt-get
```
```python
install
```
```python
tzdata
Reading package lists
```
```python
..
```
```python
. Done
Building dependency tree       
Reading state information
```
```python
..
```
```python
. Done
The following NEW packages will be installed:
  tzdata
0 upgraded, 1 newly installed, 0 to remove and 7 not upgraded.
Need to get 168 kB of archives.
After this operation, 2867 kB of additional disk space will be used.
Get:1 mirror://mirrors.ubuntu.com/mirrors.txt xenial-updates/main amd64 tzdata all 2018i-0ubuntu0.16.04
```
```python
[
```
```python
168 kB
```
```python
]
```
```python
Fetched 168 kB
```
```python
in
```
```python
1s
```
```python
(
```
```python
123 kB/s
```
```python
)
```
```python
debconf: delaying package configuration, since apt-utils is not installed
Selecting previously unselected package tzdata.
```
```python
(
```
```python
Reading database
```
```python
..
```
```python
. 6445 files and directories currently installed.
```
```python
)
```
```python
Preparing to unpack
```
```python
..
```
```python
./tzdata_2018i-0ubuntu0.16.04_all.deb
```
```python
..
```
```python
.
Unpacking tzdata
```
```python
(
```
```python
2018i-0ubuntu0.16.04
```
```python
)
```
```python
..
```
```python
.
Setting up tzdata
```
```python
(
```
```python
2018i-0ubuntu0.16.04
```
```python
)
```
```python
..
```
```python
.
debconf: unable to initialize frontend: Dialog
debconf:
```
```python
(
```
```python
No usable dialog-like program is installed, so the dialog based frontend cannot be used. at /usr/share/perl5/Debconf/FrontEnd/Dialog.pm line 76.
```
```python
)
```
```python
debconf: falling back to frontend: Readline
debconf: unable to initialize frontend: Readline
debconf:
```
```python
(
```
```python
Can
```
```python
't locate Term/ReadLine.pm in @INC (you may need to install the Term::ReadLine module) (@INC contains: /etc/perl /usr/local/lib/x86_64-linux-gnu/perl/5.22.1 /usr/local/share/perl/5.22.1 /usr/lib/x86_64-linux-gnu/perl5/5.22 /usr/share/perl5 /usr/lib/x86_64-linux-gnu/perl/5.22 /usr/share/perl/5.22 /usr/local/lib/site_perl /usr/lib/x86_64-linux-gnu/perl-base .) at /usr/share/perl5/Debconf/FrontEnd/Readline.pm line 7.)
debconf: falling back to frontend: Teletype
Current default time zone: '
```
```python
Asia/Shanghai
```
```python
'
Local time is now:      Tue Apr  9 22:08:16 CST 2019.
Universal Time is now:  Tue Apr  9 14:08:16 UTC 2019.
Run '
```
```python
dpkg-reconfigure tzdata'
```
```python
if
```
```python
you wish to change it.
sh-4.3$
```
# 结果确认
可以看到结果已从UTC切换至CST了
```python
sh-4.3$
```
```python
date
```
```python
Tue Apr  9 22:10:43 CST 2019
sh-4.3$
```

