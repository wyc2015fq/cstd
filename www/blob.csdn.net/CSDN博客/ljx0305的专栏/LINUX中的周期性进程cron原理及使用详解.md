# LINUX中的周期性进程cron原理及使用详解 - ljx0305的专栏 - CSDN博客
2009年08月19日 17:30:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：953标签：[cron																[linux																[任务																[脚本																[redhat																[table](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=redhat&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=cron&t=blog)
个人分类：[linux下软件安装																[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)](https://blog.csdn.net/ljx0305/article/category/440323)
[申明：本文为“老毛”原创，如需转载，请与作者联系。否则将视为侵权行为，作者将追究其法律责任。](http://arhat.blog.51cto.com/101503/114148)
在LINUX中，周期执行的任务一般由cron这个守护进程来处理。cron读取一个或多个配置文件，这些配置文件中包含了命令行及其调用时间。cron的配置文件称为“crontab”，是“cron table”的简写。
一，cron在3个地方查找配置文件：
    1，/var/spool/cron/ 这个目录下存放的是每个用户包括root的crontab任务，每个任务以创建者的名字命名，比如tom建的crontab任务对应的文件就是/var/spool/cron/tom。一般一个用户最多只有一个crontab文件。（对于不同的LINUX系统，目录的位置可能不太一样，这里我是以REDHAT 5.1为例）
    2，/etc/crontab 这个文件负责安排由系统管理员制定的维护系统以及其他任务的crontab。它的文件格式与用户自己创建的crontab不太一样。格式如下：
![](http://img1.51cto.com/attachment/200812/200812151229310966390.jpg)
           第一部分定义的是环境，第二部分定义的是每小时，每天，每周，每个月要自动执行的任务，它的格式是：
           分 时 日 月 周 [用户] 命令 （具体后面再讲）
           这个地方比较特殊，它后面接的是目录名称，之所以这么做是为了方便管理。也就是说，你可以把要每天执行的脚本放在/etc/cron.dail/目录下，每月要执行的脚本放在/etc/cron.monthly/下。
      3，/etc/cron.d/ 这个目录用来存放任何要执行的crontab文件或脚本。
      一旦cron进程启动，它就会读取配置文件，并将其保存在内存中，接着自己转入到休眠状态。以后每分钟会醒来一次检查配置文件，读取修改过的，并执行为这一刻安排的任务，然后再转入休眠。
二，crontab格式
       分 时 日 月 周 [用户] 命令
     前6个字段用空格隔开，命令中如果有空格按原意对待。“用户”字段只出现在/etc/crontab文件和/etc/cron.d目录下的文件里，它指出命令以哪个用户的身份运行。这个字段没必要写在用户自定义的crontab文件（/var/spool/cron/)中。
      定义的范围：
                        分  0--59
                        时  0--23
                        日  1--31
                        月  1--12
                        周  0--6
                *   表示所有都匹配
                */n   表示每个n长时间重复一次
                x，y    表示每到x，每到y时执行
                k-m   表示在k和m之间执行
咱们来看几个例子
        */5  *  *  *  *  ls /tmp >> /tmp/123
        每隔5分钟，将/tmp目录中的文件名重定向（追加）到/tmp/123文件中
        1  0-3  *  *  *  (cd /home/tom ;make)
         每天凌晨的12点到3点过一分时，在/home/tom目录下运行make命令
        0，30   *   13   *   5  mail -s Reminder [evi@anchor](mailto:evi@anchor)  % Don't forget to write your chapters.
         这是一个比较经典的例子。首先，它并不是表示执行的时间是周5恰好13号，这是很多人容易犯错误的地方。它表示的是周5或者每月13号。如果同时指定了“日”和“周”，那么满足两个条件之一的执行。
         后面的命令表示发一份邮件，邮件的内容是“%”后的内容。“%”后的内容只是前面命令的标准输出。   
三，cron的使用和管理
        1,  crontab 文件名     
        将文件安装为crontab，它将替换crontab文件任何以前的版本。这个文件要事先手动建立，并且严格按照crontab的格式。例如：
# vim /tmp/123
* * * * * ls >> /tmp/ls
# crontab /tmp/123
       2,   crontab
        直接编辑crontab，但每次只能做一条计划任务。下次再使用时上一次的自动被覆盖。一般情况下用得比较少。
        3，crontab -e
         使用这种方法时，cron会调用vi编辑器，相对于方法2可以写多条计划任务。每行一条。
         其他：
         crontab -l  查看任务
         crontab -r   删除任务
         对于普通用户而言，每个用户自己定义的crontab只有用户自己能看到。当然root例外。如果root想删除普通用户的crontab任务，可以用
         crontab -u 用户名 -r 
          关于使用crontab的限制
          利用/etc/cron.allow（以下简称A)和/etc/cron.deny(以下简称B）这两个文件可以指定哪些用户可以提交crontab文件。
          当A,B都不存在时，只用root可以建crontab任务。
          当仅A存在时，A中定义的用户才能建任务。格式为每行一个用户。
          当仅B存在时，B中定义的用户不能建任务，其他用户都可以。格式和A一样，每行一个用户。
           当A,B都存在时，忽略B中的定义。
           值得注意的是，在一个网络中，有时候cron会导致轻微且重复的震荡，这是因为管理员配置的cron正好在同一时间多台机器上运行，如果使用NTP问题可能会恶化。所以最好制定任务时把时间错开一下。
四，最后，不得不说的问题
        制定好crontab后，系统会自动执行管理员预先制定好的任务。但是，如果系统当机了，或者因为其他原因错过了任务执行的时间怎么办呢？
        这个时候，咱们就得依赖anacron了。默认情况下RHEL5已经安装了此服务。
        anacron只管多长时间（按天计算）执行一次任务就OK，对于具体时间则没有要求。它的配置文件是/etc/anacrontab
![](http://img1.51cto.com/attachment/200812/200812151229317751125.jpg)
和/etc/crontab类似，第一部分定义了环境。第二部分后半部分也和/etc/crontab相似，就不再重复。前半部分中第一字段定义的是天数（这个地方是1，7，30），第二字段定义延时多长分钟（这里是65，70和75）。
对于 1       65      cron.daily              run-parts /etc/cron.daily
表示的意思就是：尝试一天，每65分钟再尝试执行/etc/cron.daily中定义的任务。
这样一来，就解决了过期未执行crontab的问题
引用:http://arhat.blog.51cto.com/101503/119524
