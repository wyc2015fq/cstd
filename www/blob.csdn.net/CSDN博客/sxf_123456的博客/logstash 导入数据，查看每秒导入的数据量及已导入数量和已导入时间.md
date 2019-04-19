# logstash 导入数据，查看每秒导入的数据量及已导入数量和已导入时间 - sxf_123456的博客 - CSDN博客
2018年03月30日 20:57:23[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：783
                logstash 导入数据，查看每秒导入的数据量及已导入数量和已导入时间
pv命令可以作为实时的标准输入、标准输出监控。
$ ./bin/logstash -f generator_dots.conf | pv -abt > /dev/null
2.2MiB 0:03:00 [12.5kiB/s]
刚开始的时候JVM没有启动起来，速度是 0 B/s，会慢慢增长到稳定状态。
在CentOS通过yum安装pv命令，版本可能比较低，就不支持-a参数，使用-bt也可以，不过看着不容易。            
