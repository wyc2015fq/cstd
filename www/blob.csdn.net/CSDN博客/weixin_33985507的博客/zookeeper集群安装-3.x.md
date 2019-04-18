# zookeeper集群安装-3.x - weixin_33985507的博客 - CSDN博客
2017年08月31日 22:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
|作者|时间|
|----|----|
|刘权|2017-08-28|
基础环境准备：
|编号|说明|
|----|----|
|1|卸载openJDK,安装SunJDK,并配置环境变量|
|2|关闭防火墙|
|3|准备zookeeper安装包([http://mirrors.hust.edu.cn/apache/zookeeper/](http://mirrors.hust.edu.cn/apache/zookeeper/))|
- 上传压缩包
- 解压：`tar -zxvf zookeeper-3.4.5.tar.gz`
- 配置ZOOKEEPER_HOME
```
export ZOOKEEPER_HOME=/home/hadoop/zookeeper-3.4.5
export PATH=$PATH:$ZOOKEEPER_HOME/bin
```
- 重新加载配置:`source /etc/profile`
- 注意：3台zookeeper都需要修改
- 修改zookeeper配置文件
```
cd zookeeper-3.4.5/conf
cp zoo_sample.cfg zoo.cfg
vi zoo.cfg
添加一下内容：
dataDir=/home/hadoop/zookeeper-3.4.5/data
dataLogDir=/home/hadoop/zookeeper-3.4.5/log
server.1=slave1:2888:3888
server.2=slave2:2888:3888
server.3=slave3:2888:3888
```
- 创建目录：
```
mkdir -m 755 /home/hadoop/zookeeper-3.4.5/data
mkdir -m 755 /home/hadoop/zookeeper-3.4.5/log
```
- 写入myid :`echo "1" > /home/hadoop/zookeeper-3.4.5/data/myid`
- 将zookeeper发送到集群其他机器：
```
scp -r /home/hadoop/zookeeper-3.4.5 hadoop@slave2:/home/hadoop/
scp -r /home/hadoop/zookeeper-3.4.5 hadoop@slave3:/home/hadoop/
```
- 修改其他机器的配置文件
```
到slave2上：修改myid为：2 (echo "2" > /home/hadoop/zookeeper-3.4.5/data/myid)
到slave3上：修改myid为：3 (echo "3" > /home/hadoop/zookeeper-3.4.5/data/myid)
```
- 启动（每台机器）：`zkServer.sh start`
- 查看集群状态
```
1、jps（查看进程）
2、zkServer.sh status（查看集群状态，主从信息）
```
