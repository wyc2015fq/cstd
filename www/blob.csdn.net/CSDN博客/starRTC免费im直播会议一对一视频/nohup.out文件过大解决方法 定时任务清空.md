# nohup.out文件过大解决方法 定时任务清空 - starRTC免费im直播会议一对一视频 - CSDN博客
2019年04月10日 15:01:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：35
0、1和2分别表示标准输入、标准输出和标准错误信息输出，可以用来指定需要重定向的标准输入或输出。
在一般使用时，默认的是标准输出，既1.当我们需要特殊用途时，可以使用其他标号。例如，将某个程序的错误信息输出到log文件中：./program 2>log。这样标准输出还是在屏幕上，但是错误信息会输出到log文件中。
另外，也可以实现0，1，2之间的重定向。2>&1：将错误信息重定向到标准输出。
Linux下还有一个特殊的文件/dev/null，它就像一个无底洞，所有重定向到它的信息都会消失得无影无踪。这一点非常有用，当我们不需要回显程序的所有信息时，就可以将输出重定向到/dev/null。
如果想要正常输出和错误信息都不显示，则要把标准输出和标准错误都重定向到/dev/null， 例如：
# ls 1>/dev/null 2>/dev/null
还有一种做法是将错误重定向到标准输出，然后再重定向到 /dev/null，例如：
# ls >/dev/null 2>&1
注意：此处的顺序不能更改，否则达不到想要的效果
#!/bin/bash
# Author: Ljohn
# Last Update: 2018.02.24
# Description: nohup.out 日志分割
this_path=$(cd `dirname $0`;pwd) #根据脚本所在路径
current_date=`date -d "-1 day" "+%Y%m%d"` #列出时间
cd $this_path
echo $this_path
echo $current_date
do_split () {
[ ! -d logs ] && mkdir -p logs
split -b 10m -d -a 4 ./nohup.out ./logs/nohup-${current_date} #切分10兆每块至logs文件中，格式为：nohup-xxxxxxxxxx
if [ $? -eq 0 ];then
echo "Split is finished!"
else
echo "Split is Failed!"
exit 1
fi
}
do_del_log() {
find ./logs -type f -ctime +7 | xargs rm -rf #清理7天前创建的日志
cat /dev/null > nohup.out #清空当前目录的nohup.out文件
}
if do_split ;then
do_del_log
echo "nohup is split Success"
else
echo "nohup is split Failure"
exit 2
fi
# crontab -e 添加定时任务:每周第一天的1点执行一次
#0 1 * * */1 /server/scripts/clearNohup.sh &>/dev/null
IM和视频聊天的，可以参考下这个 https://github.com/starrtc/starrtc-android-demo
[https://my.oschina.net/u/2391658/blog/1525941](https://my.oschina.net/u/2391658/blog/1525941)
[https://blog.csdn.net/shawnhu007/article/details/50971084](https://blog.csdn.net/shawnhu007/article/details/50971084)
[https://blog.csdn.net/soi_yu/article/details/82977220](https://blog.csdn.net/soi_yu/article/details/82977220)
