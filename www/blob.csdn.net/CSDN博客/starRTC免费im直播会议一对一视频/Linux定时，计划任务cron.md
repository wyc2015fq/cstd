# Linux定时，计划任务cron - starRTC免费im直播会议一对一视频 - CSDN博客
2016年11月17日 16:24:37[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：327
假如你有一些任务要定期执行，比如清理磁盘、删除过期文件、发送邮件和提醒，这个时候可以用cron来实现。
crond是后台进程，而crontab则是定制好的计划任务表。
启动与停止
- 查看状态/sbin/service crond status
- 启动cron服务 /sbin/service crond start
- 停止服务 /sbin/service crond stop
- 重启服务 /sbin/service crond restart
你也可以将这个服务开机自动启动，在/etc/rc.d/rc.local这个脚本的末尾加上:
 /sbin/service crond start
## 使用crontab命令
- crontab -u 设定某个用户的cron服务
- crontab -l 列出当前用户的cron服务的详细内容
- crontab -l -u elesos //列出用户elesos的所有调度任务
- crontab -r 删除当前用户的cron服务
- crontab -e 编辑当前用户的cron服务
例如当前登陆的用户是root
运行crontab -e 会给root用户创建一个计划任务；并进入vi编辑计划任务内容。
分     小时     日      月       星期        命令
0-59  0-23    1-31   1-12      0-6       **command****(**一般一行对应一个任务**)**
第1个数字表示分钟（0-59），第2个数字表示小时（0-23），第3个数字表示天（1-31），第4个数字表示月份（1-12），第5个数字表示星期（0-6），其中0表示周日。后面便是你要执行的任务。
各部分之间使用空格分开。
上面的时间除了使用数字外还有几个特殊符号：
- “*”表示所有数值 ，如第一位使用* 表示每分钟
- “/”表示每， 如果第一位 使用 */5 表示每5分钟
- “-”表示数值范围，“ ，”用来隔开离散的数值，如第2位 是1-6，8 表示1点到6点，还有8点。
还有一种方法可以添加计划任务，那就是自己编辑/etc/crontab （cron服务每分钟也读取一次/etc/crontab文件）
在文件的末尾添加计划任务，格式和上述方法完全一样。
/etc/crontab里面还有其他的东西：
MAILTO=root **//**如果出现错误，或者有数据输出，数据会作为邮件发给这个帐号
0 1 *** ***** ***** ******* root **run-parts****/**etc**/**cron.hourly   **//**每个小时去执行一遍**/**etc**/**cron.hourly内的脚本
02 4*************** root **run-parts****/**etc**/**cron.daily    **//**每天去执行一遍**/**etc**/**cron.daily内的脚本
224**********0 root **run-parts****/**etc**/**cron.weekly   **//**每星期去执行一遍**/**etc**/**cron.weekly内的脚本
4241********** root **run-parts****/**etc**/**cron.monthly   **//**每个月去执行一遍**/**etc**/**cron.monthly内的脚本
注意上面的 run-parts 参数，如果去掉run-parts ，后面就要跟某个脚本名，如果加上run-parts参数，后面就要跟目录名了。
## 示例
- 每天1点向某个文件写入一段话
0   1   *  *  * echo "hello elesos" >> /tmp/test.txt
- 每5分钟执行一个程序或者命令
*/5  *  *  *  *   执行的命令
- 每隔1分钟向文件输出”hello elesos”
*/1 * * * * echo "hello elesos" >> /data/leijh/test.txt
可通过tail -f text.txt进行验证，或查cron的日志tail -f /var/log/cron
- 执行脚本
先新建一个output.sh脚本
echo "hello elesos" >> /data/leijh/output.txt
再修改计划任务
*/1 * * * * /data/leijh/output.sh
也能达到同样效果。
## 艺搜参考
[http://os.51cto.com/art/201011/233945.htm](http://os.51cto.com/art/201011/233945.htm)
[http://os.51cto.com/art/201003/187722.htm](http://os.51cto.com/art/201003/187722.htm)
[http://aub.iteye.com/blog/1326200](http://aub.iteye.com/blog/1326200)
