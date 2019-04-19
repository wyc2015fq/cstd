# hadoop启动后，长久不用，无法关闭 - BlueSky - CSDN博客
2016年04月05日 20:14:06[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：2221
上次配好了hadoop的环境，已经大概过去一周了，查看hadoop的运行情况：
上次配好了hadoop的环境，已经大概过去一周了，查看hadoop的运行情况：
![](http://attach.dataguru.cn/attachments/forum/201310/25/1739197thotbdhkmpllblo.jpg)
在master节点上查看jps状态：
![](http://attach.dataguru.cn/attachments/forum/201310/25/174145o9jglrwgnojf9gv9.jpg)
执行停止命令的时候发现下面情况：
![](http://attach.dataguru.cn/attachments/forum/201310/25/173919kmf3kf5m8fffkxm3.jpg)
在网上查了一些资料觉得下面的解释比较靠谱：
出现这个问题的最常见原因是hadoop在stop的时候依据的是datanode上的mapred和dfs进程号。而默认的进程号保存在/tmp下，linux默认会每隔一段时间（一般是一个月或者7天左右）去删除这个目录下的文件。因此删掉hadoop-hadoop-jobtracker.pid和hadoop-hadoop-namenode.pid两个文件后，namenode自然就找不到datanode上的这两个进程了。
另外还有两个原因可能引起这个问题：
1：环境变量 $HADOOP_PID_DIR 在你启动hadoop后改变了
2：用另外的用户身份执行stop-all
解决方法：
1：永久解决方法，修改$HADOOP_HOME/conf/hadoop-env.sh里边，去掉export HADOOP_PID_DIR=/var/hadoop/pids的#号，创建/var/hadoop/pids或者你自己指定目录
发现问题后的解决方法：
1：这个时候通过脚本已经无法停止进程了，不过我们可以手工停止，方法是到各mfs master和各datanode执行ps -ef | grep java | grep hadoop找到进程号强制杀掉，然后在master执行start-all脚本重新启动，就能正常启动和关闭了。
