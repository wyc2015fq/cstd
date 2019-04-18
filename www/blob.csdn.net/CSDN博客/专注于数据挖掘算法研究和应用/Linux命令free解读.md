# Linux命令free解读 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月01日 11:38:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：1945








1、free命令

一般用free查看、监控系统内存使用情况，free命令是一个显示系统中空闲和已用内存大小的工具。

free 命令使用/proc/meminfo中的值作为基准来显示内存利用率信息。

free - displays the total amount of free and used physical and swap memory in the system, as well as the buffers used by the kernel. The shared memory column represents the ’Shmem’ value. The available memory column represents the ’MemAvailable’ value.





2、free参数



```
$ free --help

Usage:
 free [options]

Options:
 -b, --bytes         show output in bytes
 -k, --kilo          show output in kilobytes
 -m, --mega          show output in megabytes
 -g, --giga          show output in gigabytes
     --tera          show output in terabytes
 -h, --human         show human-readable output
     --si            use powers of 1000 not 1024
 -l, --lohi          show detailed low and high memory statistics
 -t, --total         show total for RAM + swap
 -s N, --seconds N   repeat printing every N seconds
 -c N, --count N     repeat printing N times, then exit
 -w, --wide          wide output

     --help     display this help and exit
 -V, --version  output version information and exit
```


3、free输出



```
$ free -g
              total        used        free      shared  buff/cache   available
Mem:             58           6          23           0          28          52
Swap:            15           0          15
```




total：总物理内存大小；

used：已使用的物理内存；

free：完全未使用的物理内存；

shared：应用程序共享的物理内存；

Buffers：缓存块设备，如用户目录、文件metadata等；

Cached：缓存文件；



