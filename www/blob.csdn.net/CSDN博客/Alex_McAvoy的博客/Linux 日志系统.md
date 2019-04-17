# Linux 日志系统 - Alex_McAvoy的博客 - CSDN博客





2018年07月16日 20:53:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：75








# 【概述】

日志数据是有价值的信息宝库，也可以是毫无价值的数据泥潭。

日志可以记录系统产生的所有行为，并按照某种规范表达出来。

我们可以使用日志系统所记录的信息为系统进行排错，优化系统的性能，或者根据这些信息调整系统的行为。

收集想要的数据，分析出有价值的信息，可以提高系统、产品的安全性，还可以帮助开发完善代码，优化产品。

日志是在事故发生后查明“发生了什么”的一个很好的取证信息来源。

# 【系统日志】

在 Linux 中大部分的发行版都内置使用 syslog 系统日志，常见的日志一般存放在 /var/log 中，可以根据服务对象粗略的将日志分为：系统日志与应用日志两类。

根据下图中所显示的日志，系统日志主要是存放系统内置程序或系统内核之类的日志信息，如：alternatives.log 、btmp 等；应用日志主要是存放第三方应用所产生的日志，如：apache2 等。

![](https://img-blog.csdn.net/2018071520430891?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

常见的系统日志有：
|alternatives.log|系统更新替代信息记录|
|----|----|
|apport.log|应用程序崩溃信息记录|
|apt/history.log|使用 apt-get 安装卸载软件的信息记录|
|apt/term.log|使用 apt-get 时的具体操作|
|auth.log|登录认证的信息记录|
|boot.log|系统启动时的程序服务的日志信息|
|btmp|错误的信息记录|
|Consolekit/history|控制台的信息记录|
|dist-upgrade|dist-upgrade 这种更新方式的信息记录|
|dmesg|启动时，显示屏幕上内核缓冲信息、与硬件有关的信息|
|dpkg.log|dpkg 命令管理包的日志|
|faillog|用户登录失败详细信息记录|
|fontconfig.log|与字体配置有关的信息记录|
|kern.log|内核产生的信息记录，在自己修改内核时有很大帮助|
|lastlog|用户的最近信息记录|
|wtmp|登录信息的记录。可以找出谁正在进入系统，谁使用命令显示这个文件或信息等|
|syslog|系统信息记录|

查看日志的常见工具有：less、more、cat、last、lastlog 等命令。

# **【日志的配置】**

## 1.概述

实现日志的配置主要有两种方式：一种是由软件开发商自己来自定义日志格式然后指定输出日志位置，另一种是由 Linux 提供的日志服务程序。

在早期的大部分 Linux 发行版都是内置 syslog 的，syslog 是一个系统日志记录程序，让其作为系统的默认日志收集工具。

随着时代的进步，syslog 已跟不上时代的需求，所以他被 rsyslog 所代替了，较新的 Ubuntu、Fedora 等都是默认使用 rsyslog 作为系统的日志收集工具。

rsyslog 的全称是 rocket-fast system for log，它提供了高性能，高安全功能和模块化设计，能够接受各种各样的来源，将其输入，输出的结果到不同的目的地，最高可以提供超过每秒一百万条消息给目标文件。

## 2.rsyslog 的结构框架与数据流走向

如下图，可以知道 rsyslog 主要是由 Input、Output、Parser 三个模块构成，数据首先通过 Input module 来收集消息，然后将得到的消息传给 Parser module，通过分析模块的层层处理，将真正需要的消息传给 Output module，然后便输出至日志文件中。

![](https://img-blog.csdn.net/2018071621150277?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但 rsyslog 的实际架构要比上图复杂的多，通过下图可以知道，rsyslog 还有一个核心的功能模块便是 Queue，也正是因为它，rsyslog 才能做到每秒传递超过一百万的消息，达到一个极高并发值。

![](https://img-blog.csdn.net/20180716211713382?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 3.rsyslog 的配置

既然 rsyslog 是一个服务，那么它便可以配置，为我们提供一些我们自定义的服务。

1）配置文件

rsyslog 的配置文件有两个：一个是 /etc/rsyslog.conf，一个是 /etc/rsyslog.d/50-default.conf。

第一个主要是配置的环境，也就是 rsyslog 加载什么模块，文件的所属者等；第二个主要是配置的过滤条件（Filter Conditions）。

我们可以通过 vim 来配置其内容：

![](https://img-blog.csdn.net/20180716214015743?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180716213745832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**注：rsyslog 模块接口的使用需要通过 $ModLoad 指令来加载，可以看到上图配置生效的头两行，默认加载了 imklog、imuxsock 这两个模块。**

2）语法格式：

rsyslog 支持三种配置语法格式
- sysklogd
- legacy rsyslog
- RainerScript

sysklogd 是老的简单格式，一些新的语法特性不支持；legacy rsyslog 是以 $ 符号开头的语法，其指令是全局指令，即 rsyslogd 守护进程的配置指令，要求每行只能有一个指令；RainnerScript 是最新的语法。

除此之外，新、老语法格式还有一重要的区别：老的语法格式（sysklogd & legacy rsyslog）是以行为单位，新的语法格式（RainnerScript）可以分割多行。

3）注释语法：

注释有两种语法
- 井号：#
- C 语言风格：/* .. */

4）执行顺序：

指令在 rsyslog.conf 文件中是从上到下，顺序执行的。

5）模版：

模板是 rsyslog 一个重要的属性，它可以控制日志的格式，支持类似 template() 语句的基于 string 或 plugin 的模板，通过它我们可以自定义日志格式。

模板定义的形式有四种，适用于不同的输出模块，一般简单的格式，可以使用 string 的形式，复杂的格式，建议使用 list 的形式，使用 list 的形式，可以使用一些额外的属性字段（property statement）。

如果不指定输出模板，rsyslog 会默认使用 RSYSLOG_DEFAULT。

注：如果想要更深入的学习配置 rsyrlog 服务，可以查看其官方文档：[点击这里](https://www.rsyslog.com/doc/v8-stable/configuration/index.html)

6）过滤条件

关于 /etc/rsyslog.d/50-default.conf 这个配置文件，其主要是配置的过滤条件（Filter Conditions），也就是我们在流程图中所看见的 **Parser & Filter Engine**，它的名字叫 Selectors ，是过滤 syslog 的传统方法，主要由两部分组成：facility 与 priority，其配置格式为：

```bash
facility.priority　　　　　log_location
```

其中一个 priority 可以指定多个 facility，多个 facility 之间使用逗号分隔。

rsyslog 通过 Facility 的概念来定义日志消息的来源，以便对日志进行分类，Facility 的种类有：
|kern|内核消息|
|----|----|
|user|用户信息|
|mail|邮件系统消息|
|daemon|系统服务消息|
|auth|认证系统|
|authpriv|权限系统|
|syslog|日志系统自身消息|
|cron|计划安排|
|news|新闻信息|
|local 0~7|由自定义程序使用|

priority 也称之为 serverity level，除了日志的来源以外，对统一源产生日志消息还需要进行优先级的划分，而优先级的类别有一下几种：
|emergency|系统已无法使用|
|----|----|
|alert|必须立即处理的问题|
|critical|很严重|
|error|错误|
|warning|警告信息|
|notice|系统正常，但是比较重要|
|informational|正常|
|debug|debug 的调试信息|
|panic|很严重但是已淘汰不常用|
|none|没有优先级，不记录任何日志消息|

通过 vim 我们可以查看 /etc/rsyslog.d/50-default.conf 这个配置文件：

![](https://img-blog.csdn.net/20180716215257404?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180716220045812?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

以前以下两行为例：

auth,authpriv.*       /var/log/auth.log ：代表 auth 与 authpriv 的所有优先级的信息全都输出于 /var/log/auth.log 日志中。

kern.*      -/var/log/kern.log ：- 代表异步写入，即日志写入时不需要等待系统缓存的同步，也就是日志还在内存中缓存也可以继续写入无需等待完全写入硬盘后再写入。

# 【日志转储】

在本地的机器中，每天都有成百上千条日志被写入文件中，会有数十兆甚至更多的日志信息被写入文件中，如果是这样的话，每天看着我们的日志文件不断的膨胀，会占用大量的空间，为解决这种问题，有个叫 logrotate 的程序诞生了。

logrotate 程序是一个日志文件管理工具，用来把旧的日志文件删除，并创建新的日志文件，可以根据日志文件的大小、天数等来切割日志、管理日志，这个过程叫做：转储。

logrotate 程序不但可以压缩日志文件，减少存储空间，还可以将日志发送到指定 E-mail，方便管理员及时查看日志。

logrotate 是基于 CRON 来运行的，其脚本是 /etc/cron.daily/logrotate，可以在 /etc/logrotate 中找到其配置文件

![](https://img-blog.csdn.net/20180716222347569?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



