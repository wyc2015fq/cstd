# Linux细节 - Linux Crontab简单使用和管理 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月22日 16:04:14[initphp](https://me.csdn.net/initphp)阅读数：723标签：[linux																[service																[脚本																[cron																[sun](https://so.csdn.net/so/search/s.do?q=sun&t=blog)
个人分类：[Linux细节](https://blog.csdn.net/initphp/article/category/5942619)




- 使用：

> 
1. service crond  start           启动

2. service crond  stop            停止

3. service  crond restart    重启

4. service  crond reload           重新加载配置

5. /etc/rc.d/rc.local脚本中加入 service crond start  开机自动启动



- 
管理：


> 
1. crontab –l  查看cron的列表

2. crontab –e  编辑cron的列表

3. 目录： /var/spool/cron/

4. 12 3 * * * root tar czf /usr/local/backups/daily/etc.tar.gz /etc >> /dev/null 2>&1 说明 分钟（0-59）小时（2-24）日期（1-31）月份（1-12；或英文缩写Jan、Feb等）周几（0-6，0为周日；或单词缩写Sun、Mon等）用户名（执行命令时以此用户的身份）要执行的命令（路径）

5. 12 3 * * * root tar czf /usr/local/backups/daily/etc.tar.gz /etc >> /dev/null 2>&1        每天早上3点12分去运行脚本

6. 15 * * * * joey /usr/bin/somecommand >> /dev/null 2>&1  joey身份每小时的15分运行脚本

7.     * * * * *  cd /usr/local/web/www/ &&  /usr/local/web/php.5.3.3/bin/php test.php    每分钟去跑一下PHP脚本](https://so.csdn.net/so/search/s.do?q=cron&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=service&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




