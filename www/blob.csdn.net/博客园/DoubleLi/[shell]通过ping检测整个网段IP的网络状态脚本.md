# [shell]通过ping检测整个网段IP的网络状态脚本 - DoubleLi - 博客园






要实现Ping一个网段的所有IP，并检测网络连接状态是否正常，很多方法都可以实现，下面简单介绍两种，如下：
脚本1
#!/bin/sh
# Ping网段所有IP
# 2012/02/05
ip=1 #通过修改初值
while [ $ip != "254]; do 
#yes正常，no主机不存在或不正常
ping 192.168.0.$ip -c 2 | grep -q "ttl=" && echo "192.168.0.$ip yes" || echo "192.168.0.$ip no"
ip=`expr "$ip" "+" "1"`
done

输出结果：
192.168.0.1 yes
192.168.0.2 no
192.168.0.3 no
192.168.0.4 no
192.168.0.5 yes
192.168.0.6 no
192.168.0.7 yes
...

脚本2
#!/bin/sh
# Ping网段所有IP
# 2012/02/05
ip="192.168.0."
for i in `seq 1 254`
do
  ping -c 2 $ip$i | grep -q 'ttl=' && echo "$ip$i yes"|| echo "$ip$i no"
  #yes正常，no主机不存在或不正常
done

输出结果：
192.168.0.1 yes
192.168.0.2 no
192.168.0.3 no
192.168.0.4 no
192.168.0.5 yes
192.168.0.6 no
192.168.0.7 yes
...

如果您有更好的方法实现，可以给我留言~









