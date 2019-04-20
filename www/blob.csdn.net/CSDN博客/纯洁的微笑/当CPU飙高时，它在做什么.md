# 当CPU飙高时，它在做什么 - 纯洁的微笑 - CSDN博客
2018年06月26日 08:03:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：788
在开发过程中，有时候我们发现JVM占用的CPU居高不下，跟我们的预期不符，这时，CPU在做什么呢？是什么线程让CPU如此忙碌呢？我们通过如下几步，可以查看CPU在执行什么线程。
1.查找jvm进程ID: jps -lv 或者 ps aux | grep java
2.根据pid，查找占用cpu较高的线程：ps -mp pid  -o THREAD,tid,time 如图所示：找到占用cpu最高的tid (可以使用sort命令排序：sort -k 3 -r -n)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnol7psEELcmXicic5UIdpbmAaB1lDjiaiaUpcYfT7WwAQ12MQGKnuk247VrkgCACMKUs1dq1rLI7bpdow/640?wx_fmt=png)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnol7psEELcmXicic5UIdpbmAaM5h2uBu7bLp6qc4ryJicn2gF0LmSRZxayDLJuLTyvahHtj1WBVrc4cA/640?wx_fmt=jpeg)
3.将tid转换为16进制的数字：printf “%x\n” tid
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnol7psEELcmXicic5UIdpbmAaFXhts86vxUvia6kmnFnWWvia5seGV89KNI1Vk1h50VK29YmSQpNwD0FA/640?wx_fmt=png)
4.使用jstack命令，查询线程信息，从而定位到具体线程和代码：jstack pid | grep 7ccd -A 30
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnol7psEELcmXicic5UIdpbmAaUiaAJoswoxlicJsh6y70iaiaU9IHgCqGdPT5CJu6iaq1EcjEJzrMRXSnl2g/640?wx_fmt=png)
这样，你就看到CPU这么高，是什么线程在捣乱了！
怎么样，是不是觉得有点儿麻烦，没有关系，我把这几个步骤写成了一个脚本，直接使用就OK了。
```java
#!/bin/bash
#
# 当JVM占用CPU特别高时，查看CPU正在做什么
# 可输入两个参数：1、pid Java进程ID，必须参数  2、打印线程ID上下文行数，可选参数，默认打印10行
#
pid=$1
if test -z $pid
then
 echo "pid can not be null!"
 exit
else
 echo "checking pid($pid)"
fi
if test -z "$(jps -l | cut -d '' -f 1 | grep $pid)"
then
 echo "process of $pid is not exists"
 exit
fi
lineNum=$2
if test -z $lineNum
then
    $lineNum=10
fi
jstack $pid >> "$pid".bak
ps -mp $pid -o THREAD,tid,time | sort -k2r | awk '{if ($1 !="USER" && $2 != "0.0" && $8 !="-") print $8;}' | xargs printf "%x\n" >> "$pid".tmp
tidArray="$( cat $pid.tmp)"
for tid in $tidArray
do
    echo "******************************************************************* ThreadId=$tid **************************************************************************"
    cat "$pid".bak | grep $tid -A $lineNum
done
rm -rf $pid.bak
rm -rf $pid.tmp
```
> 
原文出处：https://www.jianshu.com/p/90579ec3113f
END
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/PgqYrEEtEnpcR0YpEk9cSITT9srthjLfnyo5a7BroaQnXA0uMk8tvhZtMVLkQDYSsXP1ibtlmYibkvBLRRe6ibD7g/640?wx_fmt=png)
更多精彩内容，尽在阅读原文
