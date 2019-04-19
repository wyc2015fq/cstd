# 每隔10秒自动更新svn  定时更新svn 脚本 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月11日 21:25:19[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：2166
任务：
*/1 * * * *  /opt/adobe/fms/webroot/api/svn.sh
这个命令的问题：只更新了版本号，没有更新实际内容，但是直接运行上面shell脚本能更新。
解决：需要在svn里加上用户名和密码。
如下：
1，新建一个更新脚本svn.sh
*#!/bin/bash***cd ****/**opt**/**adobe**/**fms**/**webroot**/**api
**export **LC_CTYPE=en_US.UTF-8**svn up **--username ray --password 12345 **/**opt**/**adobe**/**fms**/**webroot**/**api
需要加上编码那行，不然有时会报错。
然后crontab -e
*/1 * * * *  /opt/adobe/fms/webroot/api/svn.sh
上面是每隔1分钟自动更新。下面是每隔10s
***/**1 *** ***** ***** ***** ****/**opt**/**adobe**/**fms**/**webroot**/**api**/**svn.sh
***/**1 *** ***** ***** ***** ****(****sleep **10; **/**opt**/**adobe**/**fms**/**webroot**/**api**/**svn.sh**)*****/**1 *** ***** ***** ***** ****(****sleep **20; **/**opt**/**adobe**/**fms**/**webroot**/**api**/**svn.sh**)*****/**1 *** ***** ***** ***** ****(****sleep **30; **/**opt**/**adobe**/**fms**/**webroot**/**api**/**svn.sh**)*****/**1 *** ***** ***** ***** ****(****sleep **40; **/**opt**/**adobe**/**fms**/**webroot**/**api**/**svn.sh**)*****/**1 *** ***** ***** ***** ****(****sleep **50; **/**opt**/**adobe**/**fms**/**webroot**/**api**/**svn.sh**)****注意空格**
重启服务 /sbin/service crond restart
## 艺搜参考
[http://stackoverflow.com/questions/10009987/how-to-update-code-using-svn-with-crontab](http://stackoverflow.com/questions/10009987/how-to-update-code-using-svn-with-crontab)
[http://stackoverflow.com/questions/9619362/running-a-cron-every-30-seconds](http://stackoverflow.com/questions/9619362/running-a-cron-every-30-seconds)
