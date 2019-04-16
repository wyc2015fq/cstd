# Java 后台性能优化简要 - 我相信...... - CSDN博客





2015年03月15日 13:35:15[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：4192








                业务系统性能优化的前提时观察和诊断，观察工具如下：
前端优化工具：YSlow
页面响应时间：Firebug
方法相应时间：btrace
GC日志分析：JVM 启动参数
数据库优化：慢查询
系统资源调用：监控

性能诊断测试工具如下：
ab: httpd.apache.org/docs/2.2/programs/ab.html
JMeter: jmeter.apache.org
Loadrunner
反向代理引流，TCPcopy：请求复制工具 github.com/wangbin579/tcpcopy

性能优化方法：
前端性能：http请求数量，CDN网络，使用压缩
Java程序优化：单例，Future模式的非阻塞，线程池，使用NIO，减少上下文切换，减低锁竞争
传输压缩与结果缓存
数据库查询优化：合理索引，减少关联查询的反范式设计，使用查询缓存，搜索引擎，使用Key-Value数据库，
GC优化
提升硬件性能

Java应用排查工具：
JPS：输出JVM的进程信息
JSTAT：JVM状态监控工具
JInfo：应用程序的配置参数
JStack：线程快照
JMap：回收对象的队列，堆栈信息
Btrace：动态跟踪工具 kenai.com/projects/btrace
Jconsole:图形化工具
MAT： 图形化堆分析工具

VisualVM：全能复合工具




Linux 日志分析常用命令：
查看文件内容：cat 
分页显示文件：more，less
显示文件头尾：tail head
内容排序： sort
字符统计：wc
查看重复出现的行：uniq
字符串查找：grep
文件查找：find
归档：tar
URL访问工具：curl

查看最耗时的页面：
cat localaccess.log |sort -k 2 -n -r |head -10

进一步，sed，awk shell了




