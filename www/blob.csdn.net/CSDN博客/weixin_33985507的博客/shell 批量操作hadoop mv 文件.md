# shell 批量操作hadoop mv 文件 - weixin_33985507的博客 - CSDN博客
2018年04月23日 13:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
shell 批量操作hadoop mv 文件
## 遇到的问题：
需要批量将某个目录下的文件有规则的mv到另外一个目录
示例：
/tmp/archive/bg/b1201804/2018-04-17/imaga-r-00007
/tmp/archive/bg/b201502/2015-02-27/imaga-r-00001
/tmp/archive/bg/b201503/2015-03-27/imaga-r-00001
/tmp/archive/bg/b201504/2015-02-27/imaga-r-00001
结构为 /tmp/archive/bg/{b1,b}201???/${y}-${m}-${d}/*-r-*
拷贝到目录 /tmp/archive2/bg/${y}-${m}-${d}/*-r-* 下面
解决办法：
----------./run.sh-------------------
export HADOOP_HOME=/xxx/hadoop
$HADOOP_HOME/bin/hadoop fs -ls /tmp/archive/bg/{b1,b}201???/????-??-??/*-r-* | awk '{print $8}' | xargs -L1 ./mmu_mv2.sh
---------./mmu_mv.sh-----------------
output=/tmp/archive2/bg
echo $1 ##拿到hdfs路径 /tmp/archive/bg/b1201804/2018-04-17/imaga-r-00007
date=`echo $1 | awk -F'/' '{print $6}'`  ##拿到日期那一个值
echo $date
filename=`basename $1`        ## 拿到文件名imaga-r-000072018-04-17
echo $filename
echo $output/$date/$filename   ## 拼接输出路径
$HADOOP_HOME/bin/hadoop fs -test -e $output/$date/
if [ $? -eq 0 ] ;then
echo ""
else
$HADOOP_HOME/bin/hadoop fs -mkdir $output/$date/ ##创建输出路径
fi
$HADOOP_HOME/bin/hadoop fs -mv $1 $output/$date/$filename.$RANDOM$RANDOM   ## 添加随机数 位了保证重复文件不冲突
## 用到的命令：
- awk
- xargs
- basename: [http://codingstandards.iteye.com/blog/840784](https://link.jianshu.com?t=http%3A%2F%2Fcodingstandards.iteye.com%2Fblog%2F840784)
$ xargs --help
用法: xargs [-0prtx] [--interactive] [--null] [-d|--delimiter=delim]
[-E eof-str] [-e[eof-str]]  [--eof[=eof-str]]
[-L max-lines] [-l[max-lines]] [--max-lines[=max-lines]]
[-I replace-str] [-i[replace-str]] [--replace[=replace-str]]
[-n max-args] [--max-args=max-args]
[-s max-chars] [--max-chars=max-chars]
[-P max-procs]  [--max-procs=max-procs] [--show-limits]
[--verbose] [--exit] [--no-run-if-empty] [--arg-file=file]
[--version] [--help] [command [initial-arguments]]
