
# Linux下的Perforce客户端使用 – P4 - Augusdi的专栏 - CSDN博客


2015年04月27日 16:14:42[Augusdi](https://me.csdn.net/Augusdi)阅读数：1133


﻿﻿
使用的时候只需要从perforce官方网站上下载一个P4可执行文件，放到 /usr/local/bin下面，给予它可执行的权限。
然后只要在全局变量里配置几个变量就可以开始使用p4命令了，最关键的是P4PORT这个参数，这个参数指定了服务器的地址，其他参数可以不给，在命令调用的时候附加就可以了。
**1. 参数设置：**

```python
export P4PORT=192.168.4.88:1666  #P4所在的主机
export P4CLIENT=dev-client       #指定了与perforce服务器交流的client是什么
export P4USER=daihh              #P4用户名
export P4PASSWD=123456           #P4密码
export P4CHARSET=utf8            #调用命令时使用的字符集
p4 -h                            #查看帮助
```


**2. 罗列一些命令：**

```python
p4 client                      #配置本地client信息
p4 sync                        #从perforce同步文件
p4 login                       #登陆perforce
p4 help                        #显示关于命令的帮助
p4 -h                          #显示关于p4的帮助
p4 labels ...                  #显示和这个目录相关的标签
p4 sync @labelX                #同步标签labelX中的所有文件
p4 files @labelX               #查看标签labelX所包含的文件列表
p4 revert                      #恢复所有打开的文件
p4 revert -n ...               #恢复所有打开却没提交的文件
p4 branch brachX               #新建分支
p4 integrate -b brachX         #合并分支brachX中描述的文件
p4 opened                      #查看打开的文件
p4 help commands               #查看p4所有命令的帮助
p4 dirs -H .                   #显示当前目录
p4 branches                    #显示所有的分支
p4 delete filename             #从p4删除文件
p4 changelists -L ...          #显示当前目录下面所有文件的修改注释
p4 label labelX                #新建标签 labelX，编辑标签
p4 tag -l labelX ...           #把当前目录下面的所有文件添加到标签 labelX
p4 changes ...                 #当前目录的changelist
p4 sync @10931                 #同步某个changelist
p4 sync ...@10929              #only sync  localfile@changelist
p4 help revisions              #查看关于文件范围的帮助
```
**3. 实际使用的一些例子：**
```python
p4 -u $p4user -C $p4charset login
p4 -u $p4user -c $p4client -C $p4charset sync -f //depot/......
p4 -u $p4user -c $p4client -C $p4charset add $p4codebase/....../filename.txt
p4 -u $p4user -c $p4client -C $p4charset submit -d submitcomment $p4codebase/....../filename.txt
```

**4. 附：hostname的修改**

```python
vi /etc/hosts             #最后一个修改为xxxx
vi /etc/sysconfig/network #HOSTNAME=xxxx
hostname=xxxx
```


参考：~~http://www.indang.net/html/linux-perforce-p4-clients.html~~
[下載文章的PDF](http://zh.pdf24.org/)
[http://xenojoshua.com/2011/04/perforce-linux-client-commands/](http://xenojoshua.com/2011/04/perforce-linux-client-commands/)


