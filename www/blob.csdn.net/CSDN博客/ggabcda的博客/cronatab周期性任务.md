# cronatab周期性任务 - ggabcda的博客 - CSDN博客





2017年08月29日 12:05:29[ggabcda](https://me.csdn.net/ggabcda)阅读数：156








![1240](http://upload-images.jianshu.io/upload_images/6852280-b0d6d1d8e9b2d211.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

要执行周期性任务，要保证服务运行。服务名为crond；service crond start ; systemctl start crond;

配置文件

/etc/crontab

cron的日志文件

cat /var/log/cron

# 一、简介

crond是linux下用来周期性的执行某种任务或等待处理某些事件的一个守护进程，与windows下的计划任务类似，当安装完成操作系统后，默认会安装此服务工具，并且会自动启动crond进程，crond进程每分钟会定期检查是否有要执行的任务，如果有要执行的任务，则自动执行该任务。

# 二、任务调度的分类

Linux下的任务调度分为两类，**系统任务调度**和**用户任务调度**。

系统任务调度：系统周期性所要执行的工作，比如写缓存数据到硬盘、日志清理等。在/etc目录下有一个crontab文件，这个就是系统任务调度的配置文件。

### 1.系统任务调度

通过编辑配置文件/etc/crontab，以下图的格式标明需要运行的时间即可。一行对应一个任务，格式如下图。

![1240](http://upload-images.jianshu.io/upload_images/6852280-2d0c496b0f08909c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 2.用户任务调度

在当前用户下使用命令`crontab -e`，会以当前用户的身份建立任务调度。与系统任务调度文件格式不同的是不需要指名以哪个用户的身份运行。
![1240](http://upload-images.jianshu.io/upload_images/6852280-dd5f60128594f774.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

### 3.时间表示
- 指定值：指定具体的数字 
- 范围值：* 表示每，每到这这个时间段执行一次任务。 
- 离散取值：10 2,3,4 * * *；每天的2,3,4点的10分执行一次。(总共执行了3次) 
- 连续范围： 10 2-10 * * *；每天的2点10分到10点10分分别执行(总共执行了8次) 
- 步长范围：*/3 ** * *；每分钟内执行3次(即每20秒执行一次)举例：10 10 */6 * *；每6天的10点10分执行一次任务 
- 日期与星期

10 10 1-10 * 0,6 #每个月的1-10号的10点10分，或者周六日的10点10分都执行

举例：

1 1 */6 ** root echo "hello word " ；每隔6天的1点1分执行一次任务。

1 1-10/3 * * * root echo “hello word”；在每天1-10点范围内每3小时内的第一分钟执行一次 

### 4.创建周期性任务

```
crontab -e
* * * * * /bin/echo "`date +\%F \%H:\%M:\%S`"
#命令要写绝对路径。除非标明变量
#如果在用户下新建crontab的话%需要转义
```

### 5.创建时的其他设置

创建周期性任务时，默认的编辑器为vi所以没有语法高亮。可以通过以下办法将默认编辑器修改为vim

```
#写入/etc/porfile.d   对所有用户生效
#写入~/bash_profile 对当前用户
export EDITOP=vim
```

### 6.创建任务的权限
- 允许指定用户创建任务

cat /etc/cron.deny #在文件内写入用户名，使其不能创建新任务，但已创建的依然会执行 
- 拒绝指定用户创建任务

cat /etc/cron.allow #默认文件不存在 
- 允许与拒绝优先级

如果allow文件存在，那么deny文件不生效。

如果allow为空，拒绝所有用户

如果allow、deny都不存在，任何用户不能创建计划任务 

### 7.实现毫秒、秒级别的周期性任务
- 利用usleep实现微妙级别的运行

usleep 1000000；1秒

usleep 1000；1毫秒

usleep 1；1微秒 
- 利用循环体加sleep实现秒级别的。在每分钟要执行的任务中sleep20秒。 

![1240](http://upload-images.jianshu.io/upload_images/6852280-83b190c991d5c88a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)



