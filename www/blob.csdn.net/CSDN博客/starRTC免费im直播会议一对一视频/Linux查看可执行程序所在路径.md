# Linux查看可执行程序所在路径 - starRTC免费im直播会议一对一视频 - CSDN博客
2016年10月10日 16:46:50[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1435
首先通过命令获得进程PID：如4285，然后执行下述命令
cd /proc/4285
ls -l
或直接ls -l /proc/4285
其中exe所在行即为可执行文件的全路经。如下图所示：
