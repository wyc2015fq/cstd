
# 编译安装spark2.2.0 - leofionn的博客 - CSDN博客


2018年03月06日 12:42:54[leofionn](https://me.csdn.net/qq_36142114)阅读数：185


环境：centos6.8
软件准备：spark-2.2.0.tgz源码
jdk-8u144-linux-x64.tar.gz   JDK1.8以上
apache-maven-3.3.9-bin.tar.gz
scala-2.11.8.tgz
hadoop-2.6.0-cdh5.7.0.tar.gz
1.安装jdk
tar -xf jdk-8u144-linux-x64.tar.gz
vi  /etc/profile
export  JAVA_HOME=/usr/java/jdk1.8.0_144
export  PATH=$JAVA_HOME/bin:$PATH
source /etc/profile
java -version
2.安装maven
tar -xf apache-maven-3.3.9-bin.tar.gz
vi /etc/profile
export MAVEN_HOME =/usr/local/spark-test/app/apache-maven-3.3.9
export PATH=$MAVEN_HOME/bin:$PATH
source /etc/profile
mvn -version
建议修改maven本地仓库的地址：
cd /usr/local/spark-test/app/apache-maven-3.3.9/conf
vi  settings.xml
添加一下   <localRepository>/usr/local/spark-test/maven_repo</localRepository>
3.安装scala
tar -xf scala-2.11.8.tgz
vi /etc/profile
export SCALA_HOME=/usr/local/spark-test/app/scala-2.11.8
export PATH=$SCALA_HOME/bin:$PATH
source /etc/profile
scala
4.建议安装下git
yum -y install git
5.spark源码编译,按照官网来(编译spark要内存够大，1g根本后面会出现各种问题)
vi /etc/profile
export MAVEN_OPTS="-Xmx2g -XX:ReservedCodeCacheSize=512m"
官网中有这句./build/mvn -DskipTests clean package 这是使用的自带的maven，一般我们都是自己下载maven，这句话的意思是跳过测试，打包，一般这个编译出来的hadoop版本是默认的。究竟默认的版本是什么，我们可以在github上面查看[https://github.com/apache/spark](https://github.com/apache/spark)找到pom.xml
查看下pom.xml文件，当中有profile字眼的，就是设置版本，如：
|
|<|profile|>|
|<|id|>hadoop-2.7</|id|>|
|<|properties|>|
|<|hadoop|.version>2.7.3</|hadoop|.version>|
|<|curator|.version>2.7.1</|curator|.version>|
|</|properties|>|
|</|profile|>|
|
|<|profile|>|
|<|id|>yarn</|id|>|
|<|modules|>|
|<|module|>resource-managers/yarn</|module|>|
|<|module|>common/network-yarn</|module|>|
|</|modules|>|
|</|profile|>|
|


\# Apache Hadoop 2.7.X and later（指定hadoop版本）
mvn -Pyarn -Phadoop-2.7 -Dhadoop.version=2.7.3 -DskipTests clean package

\# With Hive 1.2.1 support（指定hive的版本）
mvn -Pyarn -Phive -Phive-thriftserver -DskipTests clean package

指定scala版本
./dev/change-scala-version.sh 2.10
mvn -Pyarn -Dscala-2.10 -DskipTests clean package

由于我们生产上的CDH的版本如下：hadoop-2.6.0-cdh5.7.0.tar.gz
mvn -Pyarn-Phive -Phive-thriftserver-Phadoop-2.6 -Dhadoop.version=2.6.0-cdh5.7.0-DskipTests clean package  （采用这种方式编译出来的不是tar,gz的格式）
./dev/make-distribution.sh --name2.6.0-cdh5.7.0--tgz   -Phadoop-2.6-Dhadoop.version=2.6.0-cdh5.7.0-Phive -Phive-thriftserver  -Pyarn（这种方法编译出来的就是tgz形式，但是速度不快）
要想速度快，修改/dev/make-distribution.sh里面的脚本
1.将VERSION，SCALA_VERSION，SPARK_HADOOP_VERSION，SPARK_HIVE注释掉，直接写上自己的版本
\#VERSION=$("$MVN" help:evaluate -Dexpression=project.version $@ 2>/dev/null | grep -v "INFO" | tail -n 1)        指的是spark2.2.0这个版本
\#SCALA_VERSION=$("$MVN" help:evaluate -Dexpression=scala.binary.version $@ 2>/dev/null\    指的是scala 2.11
\#    | grep -v "INFO"\
\#    | tail -n 1)
\#SPARK_HADOOP_VERSION=$("$MVN" help:evaluate -Dexpression=hadoop.version $@ 2>/dev/null\  指的是hadoop.version=2.6.0-cdh5.7.0
\#    | grep -v "INFO"\
\#    | tail -n 1)
\#SPARK_HIVE=$("$MVN" help:evaluate -Dexpression=project.activeProfiles -pl sql/hive $@ 2>/dev/null\SPARK_HIVE为1表示支持
\#    | grep -v "INFO"\
\#    | fgrep --count "<id>hive</id>";\
\#    \# Reset exit status to 0, otherwise the script stops here if the last grep finds nothing\
\#    \# because we use "set -o pipefail"
\#    echo -n)
将以下的内容贴在注释掉的那个脚本的后面即可
VERSION=2.2.0
SCALA_VERSION=2.11   大版本
SPARK_HADOOP_VERSION=2.6.0-cdh5.7.0
SPARK_HIVE=1
然后执行
./dev/make-distribution.sh --name2.6.0-cdh5.7.0--tgz   -Phadoop-2.6-Dhadoop.version=2.6.0-cdh5.7.0-Phive -Phive-thriftserver  -Pyarn    在spark的主目录执行

执行时会遇到各种问题：
问题1：[ERROR] Failed to execute goal on project spark-launcher_2.11: Could not resolve dependencies for project org.apache.spark:spark-launcher_2.11:jar:2.2.0: Failure to find org.apache.hadoop:hadoop-client:jar:2.6.0-cdh5.7.0 in https://repo1.maven.org/maven2 was cached in the local repository, resolution will not be reattempted until the update interval of central has elapsed or updates are forced -> [Help 1]
这是因为默认的是apache的仓库，但是我们hadoop的版本写的是CDH，这时要将CDH的仓库配进来，打开spark目录下的pom.xml文件，将CDH的仓库配进去
vi  /usr/local/spark-test/app/spark-2.2.0/pom.xml     添加如下
<repository>
<id>cloudera</id>
<name>cloudera Repository</name>
<url>https://repository.cloudera.com/artifactory/cloudera-repos</url>
</repository>
./dev/make-distribution.sh --name2.6.0-cdh5.7.0--tgz   -Phadoop-2.6-Dhadoop.version=2.6.0-cdh5.7.0-Phive -Phive-thriftserver  -Pyarn


将编译好的spark-2.2.0-bin-2.6.0-cdh5.7.0.tgz这个包进行解压，看到解压后的目录中有这些目录:
bin           \#存放客户端相关的脚本
conf\#配置文件
data\#存放测试数据
examples\#spark自带的测试用例    想学好的同学，一定重点查看spark自带的测试代码
jars\#存放spark相应的jar包   最佳实践以后说
sbin\#存放服务端相关的脚本：启停集群等
yarn\#存放yarn相关的jar包，这是动态资源调度用到的

【来自@若泽大数据】



