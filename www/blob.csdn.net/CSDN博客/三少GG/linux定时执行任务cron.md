# linux定时执行任务cron - 三少GG - CSDN博客
2013年07月25日 17:27:23[三少GG](https://me.csdn.net/scut1135)阅读数：1115
cron用法很简单： 
第一步：写cron脚本文件。例如：取名一个 crontest.txt的文本文件，只需要写一行： 
             15,30,45,59 * * * * echo "xgmtest.........." >> xgmtest.txt 
             表示，每隔15分钟，执行打印一次命令 
第二步：添加定时任务。执行命令 “crontab crontest.cron”，搞定 。
第三步：如不放心，可以输入 "crontab -l" 查看是否有定时任务 。
**附：查看服务是否已经运行用 ps -ax | grep cron**
具体的部分参数说明如下：
**crontab file [-u user]-用指定的文件替代目前的crontab。**
crontab-[-u user]-用标准输入替代目前的crontab. 
crontab- l[user]-列出用户目前的crontab. 
**crontab-e[user]-编辑用户目前的crontab. **
crontab-d[user]-删除用户目前的crontab. 
crontab-c dir- 指定crontab的目录。 
crontab文件的格式：M H D m d cmd
时程表的基本格式 :
*　　*　　*　　*　　*　　program
分　 时　 日　 月　 周　       命令
第1列表示分钟1～59 每分钟用*或者 */1表示
第2列表示小时1～23（0表示0点）
第3列表示日期1～31
第4列表示月份1～12
第5列标识号星期0～6（0表示星期天）
第6列要运行的命令
当第1列 为 * 时表示每分钟都要执行 program,第2列为 * 时表示每小时都要执行程式,其余类推
当第1列为 a-b 时表示从第 a 分钟到第 b 分钟这段时间内要执行,第2列为 a-b 时表示从第 a 到第 b 小时都要执行,其余类推
当第1列为 */n 时表示每 n 分钟个时间间隔执行一次,第2列 为 */n 表示每 n 小时个时间间隔执行一次,其余类推
当第1列为 a, b, c,... 时表示第 a, b, c,... 分钟要执行,第2列 为 a, b, c,... 时表示第 a, b, c...个小时要执行,其余类推
crontab文件的一些例子：
30 21 * * * /usr/local/etc/rc.d/lighttpd restart
上面的例子表示每晚的21:30重启lighttpd 。
45 4 1,10,22 * * /usr/local/etc/rc.d/lighttpd restart
上面的例子表示每月1、10、22日的4 : 45重启lighttpd 。
10 1 * * 6,0 /usr/local/etc/rc.d/lighttpd restart
上面的例子表示每周六、周日的1 : 10重启lighttpd 。
0,30 18-23 * * * /usr/local/etc/rc.d/lighttpd restart
上面的例子表示在每天18 : 00至23 : 00之间每隔30分钟重启lighttpd 。
0 23 * * 6 /usr/local/etc/rc.d/lighttpd restart
上面的例子表示每星期六的11 : 00 pm重启lighttpd 。
* */1 * * * /usr/local/etc/rc.d/lighttpd restart
每一小时重启lighttpd
* 23-7/1 * * * /usr/local/etc/rc.d/lighttpd restart
晚上11点到早上7点之间，每隔一小时重启lighttpd
0 11 4 * mon-wed /usr/local/etc/rc.d/lighttpd restart
每月的4号与每周一到周三的11点重启lighttpd
0 4 1 jan * /usr/local/etc/rc.d/lighttpd restart
一月一号的4点重启lighttpd 
每天7：50以root 身份执行/etc/cron.daily目录中的所有可执行文件
　　50 7 * * * root run-parts /etc/cron.daily [ 注：run-parts参数表示，执行后面目录中的所有可执行文件。 ]
------------------------------------------------------------
在一线上服务器上设置计划任务， 不小心选择了nano， 由于以前接触的是redhat系列的， 不习惯用nano， 于是想重设下， 网上查了下， 貌似没有。。
无奈之下，重装一台服务器测试， 发现当你第一次crontab -e选择编辑器的时候就会提醒你： 以后若要该改变选择， 输入select-editor 唉，还是不够细心啊。。 sudo select-editor 选择vim 搞定。。
```
```bash
root@ubuntu:
```
```bash
/var/www
```
```bash
#
 select-editor
```
```bash
```
```bash
Select
 an editor. To change later, run
```
```bash
'select-editor'
```
```bash
.
```
```bash
```
```bash
1.
```
```bash
/bin/ed
```
```bash
```
```bash
2.
```
```bash
/bin/nano
```
```bash
<---- easiest
```
```bash
```
```bash
3.
```
```bash
/usr/bin/vim
```
```bash
.basic
```
```bash
```
```bash
4.
```
```bash
/usr/bin/vim
```
```bash
.tiny
```
```
选择的数字是“3”， **/usr/bin/vim.basic**。
