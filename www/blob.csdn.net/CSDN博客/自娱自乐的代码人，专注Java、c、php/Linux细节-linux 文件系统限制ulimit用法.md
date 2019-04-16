# Linux细节 - linux 文件系统限制ulimit用法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月19日 19:45:15[initphp](https://me.csdn.net/initphp)阅读数：727








                
1. 查看所有限制的配置



```
ulimit -a
#如下：
core file size          (blocks, -c) 0    #0为不限制
data seg size           (kbytes, -d) unlimited
scheduling priority             (-e) 20
file size               (blocks, -f) 1024
pending signals                 (-i) 16382
max locked memory       (kbytes, -l) 64
max memory size         (kbytes, -m) unlimited
open files                      (-n) 1024   #限制打开的文件
pipe size            (512 bytes, -p) 8
POSIX message queues     (bytes, -q) 819200
real-time priority              (-r) 0
stack size              (kbytes, -s) 8192
cpu time               (seconds, -t) unlimited
max user processes              (-u) unlimited
virtual memory          (kbytes, -v) unlimited
file locks                      (-x) unlimited
```


2. 更改配置



```
#括号中都有-f 之类的，可以通过这种参数修改
#例如修改限制用户仅能创建10M的文件
ulimit -f 10240
```





