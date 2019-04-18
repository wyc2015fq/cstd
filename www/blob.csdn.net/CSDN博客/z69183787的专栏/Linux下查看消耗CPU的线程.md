# Linux下查看消耗CPU的线程 - z69183787的专栏 - CSDN博客
2018年08月03日 18:04:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：282
[https://blog.csdn.net/qq_24949727/article/details/70738723](https://blog.csdn.net/qq_24949727/article/details/70738723)
javaweb 项目部署后发现很耗cpu，需要查出问题所在
写个测试程序，记相关步骤：
故意写个死循环
- 
`public class TestCpu {`
- 
- 
`public static void main(String[] args) {`
- 
`while (true){`
- 
`new Object();`
- 
`}`
- 
`}`
- 
`}`
终端：
top
查看消耗cpu的进程 PID=2864
![](https://img-blog.csdn.net/20170425140534507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjQ5NDk3Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
ps -mp 2864 -o THREAD,tid,time 查看线程TID=2866
![](https://img-blog.csdn.net/20170425141135999?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjQ5NDk3Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
把线程ID转为16进制
printf "%x\n" 2866
然后查看堆栈信息
jstack 2864 |grep b32 -A 30
![](https://img-blog.csdn.net/20170425141422425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjQ5NDk3Mjc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
