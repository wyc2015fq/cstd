# Linux下如何查看CPU、内存占用率 - xqhrs232的专栏 - CSDN博客
2016年04月15日 17:24:19[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1563
原文地址::[http://www.zhixing123.cn/ubuntu/43193.html](http://www.zhixing123.cn/ubuntu/43193.html)
相关文章
1、Linux - 如何查看cpu使用率----[http://blog.sina.com.cn/s/blog_5acb430f0100aehj.html](http://blog.sina.com.cn/s/blog_5acb430f0100aehj.html)
2、[Linux下查看CPU使用率!系统资源使用情况！](http://blog.chinaunix.net/uid-12368035-id-66019.html) ----[http://blog.chinaunix.net/uid-12368035-id-66019.html](http://blog.chinaunix.net/uid-12368035-id-66019.html)
无论是使用VPS主机还是调试Linux程序，经常需要查看CPU和内存占用率，否则一旦存在情况很快就会有IDC（或客户）找上门，Linux上查看资源使用情况有多种命令可以参考，CPU、内存、IO、NETWORK等资源使用情况都可以通过某些命令查询到，本文只说CPU/内存及简单的问题追踪方法。
## 一、top查看Linux资源占用情况
top命令可以查看CPU、内存利用率，当然这些值都是平均利用率，以下是安装APACHE+PHP+MYSQL后的运行情况，示例抓图如下：
![](http://www.zhixing123.cn/uploads/allimg/141024/213G53A1-0.jpg)
其中，
- PID - 进程标示号
- USER - 进程所有者
- PR - 进程优先级
- NI - 进程优先级别数值
- VIRT - 进程占用的虚拟内存值
- RES - 进程占用的物理内存值
- SHR - 进程使用的共享内存值
- S - 进程的状态，其中S表示休眠，R表示正在运行，Z表示僵死
- %CPU - 进程占用的CPU使用率
- %MEM - 进程占用的物理内存百分比
- TIME＋ - 进程启动后占用的总的CPU时间
- Command - 进程启动的启动命令名称
## 二、free命令查看内存使用情况
free命令查看总内存、使用、空闲等情况，如下图：
![](http://www.zhixing123.cn/uploads/allimg/141024/213G55D5-1.jpg)
## 三、ps命令查看CPU状态
ps（process status）命令用来汇报处理器状态信息，示例用法：
ps ux
ps -H -eo user,pid,ppid,tid,time,%cpu,cmd --sort=%cpu
上述命令：第一条按默认方式查看状态，第二条命令指定显示列和排序方式，使用时任选其一。
![](http://www.zhixing123.cn/uploads/allimg/141024/213G54641-2.jpg)
ps详细参考：http://linux.about.com/od/commands/l/blcmdl1_ps.htm
## 四、简单的CPU高占用定位方法
首先需获取到该进程的PID，通过上文命令即可查到
根据PID找到对应的文件，使用命令（其中PID需替换为查到高占用进程ID）：
ll /proc/PID/fd
通过以上命令能找到高占用进程跟调用文件之间的关系，接下来的就是分析文件问题点。
