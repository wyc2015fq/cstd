# shell 实现对集群中的网络检测 - sxf_123456的博客 - CSDN博客
2017年04月30日 10:35:04[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：324
                
#!/bin/sh
#node_stat.sh
for node in `cat /etc/hosts|awk '(NR>=3){print $2}'`
do
#       echo "node="$node
        ping -c 1 $node &> /dev/null
        if [ $? = 0 ];then
                echo "$node is up"
        else
                sleep 1
                ping -c 1  $node &> /dev/null
                if [ $? = 0 ];then
                        echo "$node is up"
                else
                        echo "$node is down"
                fi
        fi
done
#cat /etc/hosts|awk '(NR>=3){print $2}  获取集群中的主机名
# ping -c 1 $node &> /dev/null     ping各个主机是否通
ping 参数
-c  数字       ping的次数
-i   秒           每次ping的间隔
![](https://img-blog.csdn.net/20130811144020390)
64 bytes:本次传输ICMP包的大小，此值为预设值
icmp_seq:ICMP贞次数，第一次为0，第二次为1.。。。。。
ttl:生存时间
time:响应时间
&>/dev/null 是指标准输出和错误输出都输出到/dev/null上，而不在界面上显示；
