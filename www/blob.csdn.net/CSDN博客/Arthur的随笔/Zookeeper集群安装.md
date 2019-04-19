# Zookeeper集群安装 - Arthur的随笔 - CSDN博客
2013年04月02日 11:01:14[largetalk](https://me.csdn.net/largetalk)阅读数：1384
# Zookeeper集群安装[](#zookeeper)
前提:准备3台机器，ip分别是
172.16.97.11
172.16.97.12
172.16.120.4
- 
下载zookeeper, [http://zookeeper.apache.org/releases.html](http://zookeeper.apache.org/releases.html) ,
 我下载的最新的3.4.5版
- 
下面操作在3台机器上都要做:
sudo tar xvf zookeeper-3.4.5.tar.gz -C /opt/
sudo mkdir -p /var/lib/zookeeper
sudo mkdir -p /var/log/zookeeper
cd /opt/zookeeper-3.4.5
cp conf/zoo_sample.cfg conf/zoo.cfg
vim conf/zoo.cfg并修改下面几行
    dataDir=/var/lib/zookeeper
    dataLogDir=/var/log/zookeeper
    server.1=172.16.97.11:2888:3888
    server.2=172.16.97.12:2888:3888
    server.3=172.16.120.4:2888:3888
- 
创建myid
> 
in 172.16.97.11
> 
echo “1” > /var/lib/zookeeper/myid
sudo bin/zkServer.sh start
in 172.16.97.12
> 
echo “2” > /var/lib/zookeeper/myid
sudo bin/zkServer.sh start
in 172.16.120.4
> 
echo “3” > /var/lib/Zookeeper/myid
sudo bin/zkServer.sh start
- 
connect zookeeper
bin/zkCli.sh -server 172.16.97.11:2181
###########################################################################
通过zookeeper来配置程序并自动重启
test.py
```python
import os
#from kazoo.client import KazooClient
import zc.zk
zk = zc.zk.ZooKeeper('172.16.97.11:2181')
pid = os.getpid()
flag = False
@zk.properties('/new_test')
def data_update(data):
    global flag
    if flag:
        os.kill(pid, 1)
        #print("Version: %s, data: %s" % (stat.version, data.decode("utf-8")))
    else:
        flag = True
def application(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/plain')])
    yield 'Blocking on read(), pid is %s \n' % pid   
    if not os.path.exists('/tmp/my-fifo'):
        os.mkfifo('/tmp/my-fifo')
    fd = open('/tmp/my-fifo', 'r').read(1)
    yield 'Read done\n'
```
gunicorn -w 2 test:application
然后从python命令行：
```python
In [1]: import zc.zk
In [2]: zk = zc.zk.ZooKeeper('172.16.97.11:2181')
In [3]: data = zk.properties('/new_test')
In [4]: data.update(abcd=12345)
```
可以从gunicorn看到进程重启了。
ps. 目前有个问题是如果gunicorn -k gevent -w 2 test:application启动的话，不能实现自动重启， 待fix
