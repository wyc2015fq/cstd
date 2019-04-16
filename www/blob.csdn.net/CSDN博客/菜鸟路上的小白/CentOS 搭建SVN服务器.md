# CentOS 搭建SVN服务器 - 菜鸟路上的小白 - CSDN博客





2016年10月26日 21:08:32[somnus_小凯](https://me.csdn.net/u012486840)阅读数：298
个人分类：[svn](https://blog.csdn.net/u012486840/article/category/6435095)









### ```1. 检测系统是否安装了svn :```

检测命令 : `rpm -qa subversion`

![](https://img-blog.csdn.net/20161026210611693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

`如果已经安装了svn，需要卸载可用命令 : yum remove subversion`



### `2.安装svn :`

安装命令 : 
```
yum install httpd httpd-devel
 subversion mod_dav_svn mod_auth_mysql
```


![](https://img-blog.csdn.net/20161026210625332?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

`安装结束后可以使用命令 : svnserve --version 查看当前svn版本`

### `3.创建一个版本库 :`

创建命令 : `svnadmin create /home/svn/project1`

备注：project1 是版本库目录

![](https://img-blog.csdn.net/20161026210637644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### `4.配置版本库文件 ：`

配置目录conf 有如下文件
![](https://img-blog.csdn.net/20161026210648785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### ```5.配置 passwd : 新增用户以及相关用户密码```

![](https://img-blog.csdn.net/20161026210700178?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### ```6.配置 authz 权限```

![](https://img-blog.csdn.net/20161026210711710?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### ```7.配置 svnserve.conf```

![](https://img-blog.csdn.net/20161026210723382?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### `8.启动svn :`

启动svn命令 : `svnserve -d -r /home/svn/`

### `9.关闭svn :`

![](https://img-blog.csdn.net/20161026210734770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 查询svnserve所在进程ID
 : `ps aux | grep svn` 执行关闭 : 
```
kill
 进程id
```


### `10.检出svn命令：`


```
svn co svn://127.0.0.1/fankey --username
 fankey --password 123456
```


![](https://img-blog.csdn.net/20161026210745054?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

备注:如果输错了密码，或者用户更改了密码处理方法:

1、进入记录svn密码文件的目录
`cd ~/.subversion/auth/svn.simple/`

2、使用cat命令查看 相关文件里面的内容
![](https://img-blog.csdn.net/20161026210756035?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

找到相应的文件删除文件即可



# svn自动提交带代码

找到相应svn项目下的hooks目录：创建 post-commit 文件（该文件必须有+x权限，否则发生255错误） 代码如下：

`#!/bin/sh

###### #获取SVN提交的项目

REPOS="$1"

###### #获取提交的当前版本号

REV="$2"

###### #导入中文字体

export LANG=zh_CN.GBK

###### #获取提交时填写的备注信息

BAKTEXT=$(svnlook log $REPOS -r $REV)

###### #echo "REPOS = ${REPOS} ; REV = ${REV} ; text = ${BAKTEXT}" >> home/svn/test/log/svn_hook_log.txt

###### #判断备注是否为 'update' , 如果是update则执行同步更新

if [ "$BAKTEXT" = "update" ]; then

```
#自动跟新代码到/home/wwwroot/test 目录下面
/usr/bin/svn update /home/wwwroot/test --username reader --password 123456987

#echo "you input bak is update;" >> /home/svn/test/log/svn_hook_log.txt
```

fi`

![](https://img-blog.csdn.net/20161026210809633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



