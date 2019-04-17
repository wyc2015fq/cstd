# HTTP/HLS/RTMP超级负载测试工具 - DoubleLi - 博客园






这个负载测试工具是网游分享的工具，可以在[http://blog.csdn.net/win_lin/article/details/11835011](http://blog.csdn.net/win_lin/article/details/11835011) 或者[https://github.com/winlinvip/st-load](https://github.com/winlinvip/st-load) 找到非常详细的介绍，包括如何安装，如何使用。

服务器负载测试工具(st-load)：
- 模拟huge并发：2G内存就可以开300k连接。基于states-threads的协程。
- 支持HLS解析和测试，下载ts片后等待一个切片长度，模拟客户端。支持HLS点播和直播。
- 支持HTTP负载测试，所有并发重复下载一个http文件。可将80Gbps带宽测试的72Gbps。
- 支持RTMP流测试，一个进程支持5k并发。使用nginx-rtmp的协议直接将chunk流解析为messgae。

## 流媒体相关补充

rtmp://10.169.0.66/rtmp/live 这样的格式，我们称之为一个rtmp流。通用格式为 rtmp://ip_addr/application_name/stream_name(id)

## 使用方法

将工具下载下来：git clone [https://github.com/winlinvip/st-load.git](https://github.com/winlinvip/st-load.git)

编译：
- cd st-load
- ./configure
- make

使用：./objs/sb_rtmp_load -c 1 -r rtmp://127.0.0.1:1935/live/livestream

### 更多使用方法

Usage: ./sb_rtmp_publish <Options> <-u URL>

./sb_rtmp_publish base on st(state-threads), support huge concurrency.

Options:
 -c CLIENTS, --clients CLIENTS    The concurrency client to start to request. defaut: 1
 -r URL, --url URL                The load test url for each client to download/process. 
 -t REPEAT, --repeat REPEAT       The repeat is the number for each client to download the url. 
                                  ie. rtmp://127.0.0.1:1935/live/livestream_{i}
                                  default: 0. 0 means infinity.
 -s STARTUP, --start STARTUP      The start is the ramdom sleep when  thread startup in seconds. 
                                  defaut: 5.00. 0 means no delay.
 -d DELAY, --delay DELAY          The delay is the ramdom sleep when success in seconds. 
                                  default: 1.00. 0 means no delay. -1 means to use HLS EXTINF duration(HLS only).
 -e ERROR, --error ERROR          The error is the sleep when error in seconds. 
                                  defaut: 3.00. 0 means no delay. 
 -m SUMMARY, --summary SUMMARY    The summary is the sleep when report in seconds. 
                                  etasks is error_tasks, statks is sub_tasks, estatks is error_sub_tasks.
                                  duration is the running time in seconds, tduration is the avarage duation of tasks.
                                  nread/nwrite in Mbps, duration/tduration in seconds.
                                  defaut: 30.00. 0 means no delay. 
 -v, --version                    Print the version and exit.
 -h, --help                       Print this help message and exit.

Examples:1. start a client
  ./sb_rtmp_publish -i doc/source.200kbps.768x320.flv -c 1 -r rtmp://127.0.0.1:1935/live/livestream

2. start 1000 clients
  ./sb_rtmp_publish -i doc/source.200kbps.768x320.flv -c 1000 -r rtmp://127.0.0.1:1935/live/livestream_{i}

3. start 10000 clients
  ./sb_rtmp_publish -i doc/source.200kbps.768x320.flv -c 10000 -r rtmp://127.0.0.1:1935/live/livestream_{i}

4. start 100000 clients
  ./sb_rtmp_publish -i doc/source.200kbps.768x320.flv -c 100000 -r rtmp://127.0.0.1:1935/live/livestream_{i}



http负载：./objs/sb_http_load

hls负载: ./objs/sb_hls_load

rtmp收看负载: ./objs/sb_rtmp_load

rtmp创建直播流负载: ./objs/sb_rtmp_publish


如果不知道需要跟什么参数可以使用 ./objs/sb_http_load -help 查看

## Report分析

[2015-06-01 17:00:06.981][0][trace] params url=rtmp://10.169.0.66/rtmp/live, threads=1, start=5.00, delay=1.00, error=3.00, report=30.00, count=0

[2015-06-01 17:00:06.981][0][trace] create thread for task #1 success

[2015-06-01 17:00:06.981] [report] [4391] threads:0 alive:0 duration:0 tduration:0 nread:0.00 nwrite:0.00 tasks:0 etasks:0 stasks:0 estasks:0

[2015-06-01 17:00:06.981][1][trace] start random sleep 4338ms

[2015-06-01 17:00:11.324][1][trace] start to process RTMP play task #1, schema=rtmp, host=10.169.0.66, port=1935, tcUrl=rtmp://10.169.0.66:1935/rtmp, stream=live, startup=5.00, delay=1.00, error=3.00, count=0

[2015-06-01 17:00:36.981] [report] [4391] threads:1 alive:1 duration:30 tduration:0 nread:2.62 nwrite:0.00 tasks:1 etasks:0 stasks:0 estasks:0

[2015-06-01 17:01:06.981] [report] [4391] threads:1 alive:1 duration:60 tduration:0 nread:2.88 nwrite:0.00 tasks:1 etasks:0 stasks:0 estasks:0

[2015-06-01 17:01:36.981] [report] [4391] threads:1 alive:1 duration:90 tduration:0 nread:2.96 nwrite:0.00 tasks:1 etasks:0 stasks:0 estasks:0

[2015-06-01 17:02:06.981] [report] [4391] threads:1 alive:1 duration:120 tduration:0 nread:3.01 nwrite:0.00 tasks:1 etasks:0 stasks:0 estasks:0


收看直播与创建直播只需要关注：nread与nwrite即可。其他内容有待进一步调查 









