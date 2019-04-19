# 离线安装Hadoop最新发行版本hdp2.4 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [云戒](http://www.jobbole.com/members/wx3885314943) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
hdp2.4是hortonworks最新推出的100%开源的hadoop发行版本，其中包含了最新的spark 1.6，冲着这点，使用离线方式安装最新的2.4版本。
## 1. 主要点
- 假设服务器无网络环境，或者其中只有一台能上外网，或者上外网速度慢到相当于无网络。
- 离线下载数据包，解压，放到其中一台机器，开启http支持，其它内网能访问。
- 配置ubuntu的源，指向内网的机器，安装Ambari-server服务。再通过Ambari-server服务来安装其它所有的hadoop组件。
无网络环境安装，主要是访问hortonworks的速度太慢了，而且中间各种不稳定，因此采用离线的方式，能保证安装成功。
## 2. 具体的操作步骤
### 2.1 下载三个大文件，备用

Shell
```
# down.sh
nohup wget -c http://public-repo-1.hortonworks.com/ambari/ubuntu14/2.x/updates/2.2.1.0/ambari-2.2.1.0-ubuntu14.tar.gz > 1.log 2>&1 &
nohup wget -c http://public-repo-1.hortonworks.com/HDP/ubuntu14/2.x/updates/2.4.0.0/HDP-2.4.0.0-ubuntu14-deb.tar.gz > 1.log 2>&1 &
nohup wget -c http://public-repo-1.hortonworks.com/HDP-UTILS-1.1.0.20/repos/ubuntu14/HDP-UTILS-1.1.0.20-ubuntu14.tar.gz > 2.log 2>&1 &
```
### 2.2 解压
解压三个文件到某个目录，最后，目录中有如下

Shell
```
$ ls
AMBARI-2.2.1.0
ambari-2.2.1.0-ubuntu14.tar.gz
down.sh
HDP
HDP-2.4.0.0-ubuntu14-deb.tar.gz
HDP-UTILS-1.1.0.20
HDP-UTILS-1.1.0.20-ubuntu14.tar.gz
```
### 2.3 开启http
在目录下运行http服务，不需要Apache，也不需要Nginx以及他们的各种配置，使用一个命令即可：

Shell
```
$ python -m SimpleHTTPServer
```
看提示，已经开启了8000端口，web的根目录就为当前目录。
### 2.4 在内网其它机器，添加官方源

Shell
```
wget -nv http://public-repo-1.hortonworks.com/ambari/ubuntu14/2.x/updates/2.2.1.0/ambari.list -O /etc/apt/sources.list.d/ambari.list
apt-key adv --recv-keys --keyserver keyserver.ubuntu.com B9733A7A07513CAD
```
### 2.5 修改官方源配置文件

Shell
```
#VERSION_NUMBER=2.2.1.0-161
#deb http://public-repo-1.hortonworks.com/ambari/ubuntu14/2.x/updates/2.2.1.0 Ambari main
deb http://1.2.3.4:8000/AMBARI-2.2.1.0/ubuntu14/2.2.1.0-161 Ambari main
```
其中地址：http://1.2.3.4:8000即是上面的那台开启http的内网机器。后面的目录，可以根据前面下载解压后的目录来修改。
### 2.5 更新源并安装程序：

Shell
```
apt-get update
apt-get install ambari-server
```
### 2.6 配置ambari服务

Shell
```
ambari-server setup
```
其中自定义jdk版本，填写JAVA_HOME的路径为，根据具体的jdk路径来设置，比如：/opt/jdk
### 2.7 启动服务

Shell
```
ambari-server start
```
### 2.8 访问web界面
> 
http://ip:8080 使用默认的admin/admin登录。
### 2.9 在选择源的地方，填写路径

Shell
```
# http://public-repo-1.hortonworks.com/HDP/ubuntu14/2.x/updates/2.4.0.0
http://1.2.3.4:8000/HDP/ubuntu14/2.x/updates/2.4.0.0
# http://public-repo-1.hortonworks.com/HDP-UTILS-1.1.0.20/repos/ubuntu12
http://1.2.3.4:8000/HDP-UTILS-1.1.0.20/repos/ubuntu14
```
注释的部分为hdp2.4的默认官方源，用自己的内网源替换即可。
### 2.10 后续服务安装
配置各机器证书登录，选择安装服务等等后续步骤。
安装好后，关闭前面的http服务即可。
就是这么简单，就是这么方便。走过，路过，请不要跳过，在大数据圈混，迟早有一天用得到的。
最后，如果遇到问题，请留言。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/b947d0acc8cb4e3b0649a27c606466362.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/04/f22f5454bd93395413ea7e7507db150a.jpg)
