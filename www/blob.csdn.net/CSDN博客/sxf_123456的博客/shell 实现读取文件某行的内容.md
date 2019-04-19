# shell 实现读取文件某行的内容 - sxf_123456的博客 - CSDN博客
2017年04月25日 14:38:45[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：3855
                
[root@node2 sxf]# cat /etc/hosts  #查看文件内容
127.0.0.1   localhost localhost.localdomain localhost4 localhost4.localdomain4
::1         localhost localhost.localdomain localhost6 localhost6.localdomain6
192.168.26.10 node0
192.168.26.11 node1
192.168.26.12 node2
192.168.26.13 node3
192.168.26.14 node4
[root@node2 sxf]# sed -n 4p /etc/hosts #查看文件第四行内容
192.168.26.11 node1
[root@node2 sxf]# sed -n 4p /etc/hosts|cut -d ' ' -f1 #查看文件第四行第一列内容
192.168.26.11
[root@node2 sxf]# sed -n 4p /etc/hosts|cut -d ' ' -f2 #查看文件第四行第二列内容
node1
在得到数行输出时，需要得到指定行的字段，可以使用该指令。
sed -n Np 文件名
如sed -n 2p xxx 输出结果即为第二行的内容。
