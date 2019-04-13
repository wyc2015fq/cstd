
# 阿里云虚拟机搭建Hadoop-2.6.0-cdh5.7.1安装详解（伪分布式环境） - leofionn的博客 - CSDN博客


2018年01月05日 12:27:08[leofionn](https://me.csdn.net/qq_36142114)阅读数：747


首先先搭配安全组
![](https://img-blog.csdn.net/20180105122755813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzYxNDIxMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
打开镜像后输入以下内容：
重要：  yum -y install lrzsz
```python
HOSTNAME=（自己的主机名字）
hostname $HOSTNAME
echo "$(grep -E '127|::1' /etc/hosts)">/etc/hosts
echo "$(ifconfig eth0|grep inet|awk -F'[ :]' '{print $13}') $(hostname)">>/etc/hosts
sed -i "s/^HOSTNAME.*$/HOSTNAME=$HOSTNAME/g" /etc/sysconfig/network
```
编译工作需要以下环境支持：
jdk1.7(
## 1.8也是可以的 有一个包冲突了 改下hadoop-annotations下的jdk版本指向1.8就行了
)
apache-maven-3
protobuf-2.5.0
findbugs-1.3.9
gcc gcc-c++ make cmake
openssl zlib ncurses snappy bzip2
lzo lzop autoconf automake
PS：将所有文件上传到 /tmp 目录下
配置jdk1.7：

```python
mkdir /usr/java
cd /usr/java/
tar -xf /tmp/server-jre-7u80-linux-x64.tar.gz
chown root: /usr/java/jdk1.7.0_80/ -R
echo 'export JAVA_HOME=/usr/java/jdk1.7.0_80'>>/etc/profile
```
配置maven：

```python
cd /usr/local/
yum -y install unzip
unzip /tmp/apache-maven-3.3.9-bin.zip
chown root: /usr/local/apache-maven-3.3.9 -R
echo 'export MAVEN_HOME=/usr/local/apache-maven-3.3.9'>>/etc/profile
echo 'export MAVEN_OPTS="-Xms256m -Xmx512m"'>>/etc/profile
```
编译安装protobuf：
```python
yum -y install gcc gcc-c++ make cmake
cd /tmp/
tar -xf protobuf-2.5.0.tar.gz
cd protobuf-2.5.0
./configure --prefix=/usr/local/protobuf
make && make install
echo 'export PROTOC_HOME=/usr/local/protobuf'>>/etc/profile
```
配置findbugs：
```python
cd /usr/local/
unzip /tmp/findbugs-1.3.9.zip
chown root: findbugs-1.3.9 -R
echo 'export FINDBUGS_HOME=/usr/local/findbugs-1.3.9'>>/etc/profile
```
最终PATH变量和其他依赖包的安装
```python
echo "export PATH=\
\$FINDBUGS_HOME/bin:\
\$PROTOC_HOME/bin:\
\$MAVEN_HOME/bin:\
\$JAVA_HOME/bin:\
\$PATH">>/etc/profile
source /etc/profile
java -version
mvn -version
protoc --version
findbugs -version
# 测试java和maven以及protoc和findbugs是否配置成功
yum install -y openssl openssl-devel \
    svn ncurses-devel zlib-devel libtool \
    snappy snappy-devel bzip2 bzip2-devel \
    lzo lzo-devel lzop autoconf automake \
    openssh-clients
```

## 编译hadoop2.6-cdh5.7.1的安装包
```python
cd /tmp
tar -xf hadoop-2.6.0-cdh5.7.1-src.tar.gz
cd hadoop-2.6.0-cdh5.7.1
mvn clean package -Pdist,native -DskipTests -Dtar
# 该步需要通过网络下载和编译，耗时较长
# 参数解释：
# clean package 清除原来的包
# -Pdist,native 将生成的包放入本地的dist目录
# -DskipTests 忽略测试
# -Dtar 生成tar包
# 编译生成 hadoop-2.6.0-cdh5.7.1.tar.gz
# 目录：/tmp/hadoop-2.6.0-cdh5.7.1/hadoop-dist/target
# 可以将编译生成的tar包保存下来，就不用再次编译了。
```
（注意：进入 /tmp/hadoop-2.6.0-cdh5.7.1/hadoop-dist/target
输入mv 将文件移出）
编译出来的安装包的部署不在需要findbugs和maven以及protobuf，只需要jdk即可。
可以重新克隆一台实验机，然后配置jdk，也可以清掉编译机器的环境进行部署：
```python
cd /usr/local/
rm -rf findbugs-1.3.9 apache-maven-3.3.9 protobuf
sed -i '/^export MAVEN_HOME=/d'    /etc/profile
sed -i '/^export MAVEN_OPTS=/d'    /etc/profile
sed -i '/^export FINDBUGS_HOME=/d' /etc/profile
sed -i '/^export PROTOC_HOME=/d'   /etc/profile
sed -i '/^export PATH=/d'          /etc/profile
echo 'export PATH=$JAVA_HOME/bin:$PATH'>>/etc/profile
source /etc/profile
```
## 配置HDFS
添加hadoop用户，配置sudo权限：
`useradd hadoop
echo hadoop|passwd --stdin hadoop
sed -i 's/^root.*/&\nhadoop  ALL=(ALL)       ALL/g' /etc/sudoers`安装编译生成的hadoop包：
`cd /usr/local
tar -xf /tmp/hadoop-2.6.0-cdh5.7.1.tar.gz
chown hadoop: hadoop-2.6.0-cdh5.7.1 -R
ln -s hadoop-2.6.0-cdh5.7.1/ hadoop
cd hadoop
echo 'export HADOOP_HOME=/usr/local/hadoop'>>/etc/profile
echo 'export PATH=$HADOOP_HOME/bin:$HADOOP_HOME/sbin:$PATH'>>/etc/profile``source /etc/profile`切换到hadoop用户，配置ssh互信
`su - hadoop
ssh-keygen -t rsa -P '' -f ~/.ssh/id_rsa
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
chmod 0600 ~/.ssh/authorized_keys
ssh -o StrictHostKeyChecking=no localhost date
ssh -o StrictHostKeyChecking=no 127.0.0.1 date
ssh -o StrictHostKeyChecking=no $(hostname) date`配置HDFS：
`cd /usr/local/hadoop
vi etc/hadoop/core-site.xml
# 在<configuration>这里添加配置</configuration>
# 添加完配置之后如下：
<configuration>
    <property>
        <name>fs.defaultFS</name>
        <value>hdfs://hadoop001:9000</value>
    </property>
</configuration>
vi etc/hadoop/hdfs-site.xml
# 在<configuration>这里添加配置</configuration>
# 添加完配置之后如下：
<configuration>
    <property>
        <name>dfs.replication</name>
        <value>1</value>
    </property>
    <property>
        <name>dfs.namenode.secondary.http-address</name>
        <value>hadoop001:50090</value>
    </property>
    <property>
        <name>dfs.namenode.secondary.https-address</name>
        <value>hadoop001:50091</value>
    </property>
</configuration>
cd /usr/local/hadoop/etc/hadoop
echo $(hostname)>slaves``# 修改hadoop的slave的解析地址`格式化namenode，启动HDFS：
`cd /usr/local/hadoop/etc/hadoop/
sed -i "s#export JAVA_HOME=.*#export JAVA_HOME=$JAVA_HOME#g" hadoop-env.sh
hdfs namenode -format
start-dfs.sh`进程状态查看：
`jps
# 查看NameNode、SecondaryNameNode和DataNode是否启动
# 网页访问：
# http://自己IP:50070`
## 伪分布式配置MR和Yarn
MR配置部署：
`cd /usr/local/hadoop/etc/hadoop
cp -av mapred-site.xml{.template,}
vi mapred-site.xml
# 在<configuration>这里添加配置</configuration>
# 添加完配置之后如下：
<configuration>
    <property>
        <name>mapreduce.framework.name</name>
        <value>yarn</value>
    </property>
</configuration>`YARN配置部署：
`vi yarn-site.xml
# 在<configuration>这里添加配置</configuration>
# 添加完配置之后如下：
<configuration>
    <property>
        <name>yarn.nodemanager.aux-services</name>
        <value>mapreduce_shuffle</value>
    </property>
</configuration>`启动：
`start-yarn.sh`设置开机自动启动HDFS和YARN：
`echo "/bin/su - hadoop -c '/usr/local/hadoop/sbin/start-all.sh'">>/etc/rc.local`来自@若泽大数据



