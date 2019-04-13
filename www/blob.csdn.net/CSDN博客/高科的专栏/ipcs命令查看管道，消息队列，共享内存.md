
# ipcs命令查看管道，消息队列，共享内存 - 高科的专栏 - CSDN博客

2012年09月12日 11:35:07[高科](https://me.csdn.net/pbymw8iwm)阅读数：5526


修改消息队列大小：
root：用户： /etc/sysctl.conf
kernel.msgmnb =4203520
\#kernel.msgmnb =3520
kernel.msgmni = 2878
保存后需要执行 sysctl -p,然后重建所有消息队列
ipcs -q   :  显示所有的消息队列
ipcs -qt :   显示消息队列的创建时间，发送和接收最后一条消息的时间
[mas@[172.16.9.38:/mas]$](mailto:mas@%5B172.16.9.38:/mas%5D$)ipcs -qt
------ Message Queues Send/Recv/Change Times --------
msqid    owner      send                 recv                 change
65536    mas        Not set              Not set              May 15 15:56:39
98305    mas        May 15 15:59:22      May 15 15:59:22      May 15 15:56:39
ipcs -qp:    显示往消息队列中放消息和从消息队列中取消息的进程ID
[mas@[172.16.9.38:/mas]$](mailto:mas@%5B172.16.9.38:/mas%5D$)ipcs -qp
------ Message Queues PIDs --------
msqid      owner      lspid      lrpid
65536    mas             0         0
98305    mas         21941     21941
ipcs -q -i  msgid:  显示该消息队列结构体中的消息信息：
[mas@[172.16.9.38:/mas]$](mailto:mas@%5B172.16.9.38:/mas%5D$)ipcs -q -i 98305
Message Queue msqid=98305
uid=512 gid=500 cuid=512        cgid=500        mode=0600
cbytes=0        qbytes=4203520  qnum=0  lspid=21941     lrpid=21941
send_time=Thu May 15 16:00:19 2008
rcv_time=Thu May 15 16:00:19 2008
change_time=Thu May 15 15:56:39 2008
ipcs -ql :     显示消息队列的限制信息：
[mas@[172.16.9.38:/mas]$](mailto:mas@%5B172.16.9.38:/mas%5D$)ipcs -ql
------ Messages: Limits --------
max queues system wide = 2878               //最大进程数
max size of message (bytes) = 8192         //队列中最大消息长度
default max size of queue (bytes) = 4203520
其它相关的命令使用：ipcs -h查看
此外还可以显示管道，消息队列，共享内存等信息

