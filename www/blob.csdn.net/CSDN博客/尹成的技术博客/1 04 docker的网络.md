
# 1.04 docker的网络 - 尹成的技术博客 - CSDN博客

2018年12月10日 09:32:08[尹成](https://me.csdn.net/yincheng01)阅读数：76个人分类：[GO语言](https://blog.csdn.net/yincheng01/article/category/7679307)[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)[](https://blog.csdn.net/yincheng01/article/category/7679307)



**1.网络分类**
单机Bridge Network
Host Network
None Network
多机Overlay Network
2.网络基础
公有IP和私有IPpublic IP：互联网上的唯一标识，可以访问Internet
private IP：不可在互联网上使用，仅限机构内部使用
A类：10.0.0.0~10.255.255.255（10.0.0.0/8）
B类：172.16.0.0~172.31.255.255（172.16.0.0/12）
C类：192.168.0.0~192.168.255.255（192.168.0.0/16）
网络地址转换NAT
有一个公有IP，路由来做NAT转换
ping：验证IP的可达性
telnet：验证服务的可用性
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142417310.png)
3.Linux网络命名空间namespace
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142525509.png)
查看后台进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142547575.png)
交互运行
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142600260.png)
exit退出后，本机也可以查看网络接口，但他们是不同的，是隔离开的，这就是通过namespace实现的网络隔离
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142612820.png)
直接查看容器的网络端口，在本机可以ping通，因为本机也虚拟出了一个网段
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142832524.png)
查看本机的network namespace
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142843125.png)
创建namespace
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142850884.png)
删除namespace
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142904789.png)
再次创建namespace
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120514291325.png)
在namespace中执行ip a创建一个回环口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142920615.png)
查看回环口状态
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142930314.png)
使之变为up状态
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142938544.png)
再次查看回环口状态
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205142951837.png)
创建另一个namespace test2
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143003604.png)
添加link，然后查看，多了两个veth的接口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143012904.png)
查看veth的接口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143026584.png)
将veth-test1添加到namespace的test1中，然后查看test1中的接口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143041181.png)
可以在本地查看ip link，本地的veth-test1不见了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143050740.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143059896.png)
将veth-test2添加到namespace的test2中，然后查看test2中的接口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143111686.png)
然后可以在本地查看ip link，本地的veth-test2不见了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143208269.png)
为test1和test2分配IP地址
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143217620.png)
此时查看test1和test2，并没有IP
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143237345.png)
开启2个端口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143242998.png)
再次查看test1和test2
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120514325580.png)
test1和test2此时已经有IP了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143303529.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTA5ODY3NzY=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143312348.png)
test1和test2互相可以ping通
这就是docker底层应用namespace网络隔离的原理
创建容器，就会同时创建一个namespace
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143339537.png)
4.Bridge详解
关掉test6，就留一个test5
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143402411.png)
列举当前机器docker有哪些网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143429307.png)
查看bridge网络的连接情况
可以看到test5的container连接的是docker bridge网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143441472.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120514345051.png)
查看本机的网卡
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143505699.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143531286.png)
查看test5的ip a，下面的eth0@if15就与上面的veth是一对的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143540806.png)
可以通过brctl查看所有的bridge
可以看到如下veth与 ip a 查看到的docker0的veth是一样的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143551628.png)
再创建一个container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143559950.png)
再次查看所有的bridge
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143605211.png)
查看ip a，docker0下也多了一个veth与之对应
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143612738.png)
进入容器交互运行
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120514381396.png)
ping 外网
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205143819745.png)
本机的eth0可以去访问外网，NAT网络地址转换实现了docker0也可以访问外网，也就是iptables实现的
5.容器通信
创建一个container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144054673.png)
创建另一个container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144104903.png)
查看进程
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144110744.png)
交互访问test8，然后ping IP和ping 主机名，都是通的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144121863.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144127490.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144133618.png)
这里假设，访问test7的mysql，就可以test7:3306
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120514413924.png)
反过来，交互运行test7，不能直接ping 主机名，是有方向的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144147187.png)
查看，停止，删除test8
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144205579.png)
6.端口映射
首先，后台创建并运行一个nginx的container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144215429.png)
查看进程，nginx默认使用80端口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144219847.png)
查看网络状况
web连接到了bridge上，IP是172.17.0.2
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144227209.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144232616.png)
本机可以ping通nginx的container
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144242305.png)
80端口也是可连通的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144246821.png)
可以查看nginx的欢迎页面
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144258116.png)
停止并删掉刚才的web容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144302684.png)
重新创建容器，指定-p参数，将容器的80端口映射到本地的80端口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144310604.png)
查看进程，已经映射到了本地的80端口
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144317673.png)
此时访问本地的80端口，也会返回nginx欢迎页
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144323730.png)
通过浏览器访问虚拟机IP也可以
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144330361.png)
拓扑图如下，nginx是绑定到172.17.0.2:80，刚才-p参数是将192.168.205.10:80转发到了172.17.0.2:80
如果是在阿里云创建的，192.168.205.10就是对外的IP
阿里云也是给两个IP，有一个是对外的IP
7.网络的none和host
介绍host和none网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144507342.png)
创建新的容器，使用none网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144512359.png)
查看none网络，连接了test10
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144519972.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144525487.png)
外界查看不到test10的网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144531727.png)
进入test10容器，并查看网络，没有对外的接口，是一个孤立的容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144541181.png)
应用场景：安全性要求极高，存储绝密数据等
创建新的容器，使用host网络
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144547514.png)
查看host网络，连接了test11
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144553726.png)
进入test11容器，并查看网络，与本机看到的是一样的，与主机共享namespace，类似虚拟机NAT模式
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144622258.png)
8.多容器部署和应用
创建redis容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144803197.png)
创建并进入目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144808508.png)
[创建app.py](http://xn--app-th2el20d.py)，写入以下内容
from flask import Flask
from redis import Redis
import os
import socket
app = Flask(name)
redis = Redis(host=os.environ.get(‘REDIS_HOST’, ‘127.0.0.1’), port=6379)
@app.route(’/’)
def hello():
redis.incr(‘hits’)
return ‘Hello Container World! I have been seen %s times and my hostname is %s.\n’ % (redis.get(‘hits’),socket.gethostname())
ifname== “main”:
app.run(host=“0.0.0.0”, port=5000, debug=True)
创建Dockerfile，写入以下内容
FROM python:2.7
LABEL[maintaner="sjc_job@126.com](mailto:maintaner=%22sjc_job@126.com)"
COPY . /app
WORKDIR /app
RUN pip install flask redis
EXPOSE 5000
CMD [ “python”, “[app.py](http://app.py)” ]
构建image
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144844587.png)
创建flask容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144850768.png)
进入容器，交互执行
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144856433.png)
查看环境变量
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144901341.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144906340.png)
直接可以ping redis了
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144912634.png)
多次访问5000端口，实现自增
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144922374.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144928116.png)
停止并删掉flask-redis
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144934680.png)
重启启动容器，加-p参数指定端口映射
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144942155.png)
本地也可以访问容器内的服务了
这里的自增1是redis做的，web服务是flask发布的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144949563.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205144956482.png)
[](https://img-blog.csdnimg.cn/20181205144949563.png)9.overlay网络和etcd通信
[](https://img-blog.csdnimg.cn/20181205144949563.png)etcd是一个开源免费的K-V的分布式存储
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145006862.png)
解压
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145018293.png)
进入解压目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145024189.png)
运行如下启动命令，然后回车，后台运行
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145031661.png)
nohup ./etcd --name docker-node1 --initial-advertise-peer-urls[http://192.168.174.141:2380](http://192.168.174.141:2380)
–listen-peer-urls[http://192.168.174.141:2380](http://192.168.174.141:2380)
–listen-client-urls[http://192.168.174.141:2379](http://192.168.174.141:2379),[http://127.0.0.1:2379](http://127.0.0.1:2379)
–advertise-client-urls[http://192.168.174.141:2379](http://192.168.174.141:2379)
–initial-cluster-token etcd-cluster
–initial-cluster docker-node1=http://192.168.174.141:2380,docker-node2=http://192.168.174.142:2380
–initial-cluster-state new&
第二台机器，进入/usr/local目录，下载tar包
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145039250.png)
解压
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145044418.png)
进入解压目录
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145050588.png)
运行如下启动命令，然后回车，后台运行
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145055447.png)
nohup ./etcd --name docker-node2 --initial-advertise-peer-urls[http://192.168.174.142:2380](http://192.168.174.142:2380)
–listen-peer-urls[http://192.168.174.142:2380](http://192.168.174.142:2380)
–listen-client-urls[http://192.168.174.142:2379](http://192.168.174.142:2379),[http://127.0.0.1:2379](http://127.0.0.1:2379)
–advertise-client-urls[http://192.168.174.142:2379](http://192.168.174.142:2379)
–initial-cluster-token etcd-cluster
–initial-cluster docker-node1=http://192.168.174.141:2380,docker-node2=http://192.168.174.142:2380
–initial-cluster-state new&
在第一台机器etcd文件夹中执行，查看etcd集群状态
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145111659.png)
第二台机器也同样执行，检查状态
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145118960.png)
第一台机器停止docker
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145316928.png)
手动后台运行docker，命令执行完打回车
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145324796.png)
第二台机器停止docker
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145330680.png)
手动后台运行docker，命令执行完打回车
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145344191.png)
可以将两个机器都exit退出，然后再重新连接
node01中，创建网络，overlay是驱动
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120514535631.png)
查看网络，多个demo
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145403654.png)
node02中查看网络，也多个demo，两边数据是同步的，etcd实现的
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145412647.png)
node01中查看etcd记录的信息，最终看到id与创建network产生的id一致
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145423574.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120514542917.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145434580.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145440205.png)
可以查看网络的基本信息
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145447106.png)
node01中创建容器
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145454490.png)
此时将命令复制到node02运行，会直接报错，说test12已经存在
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145500849.png)
node02中重新创建，改名为test13
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205145506651.png)
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[](https://img-blog.csdnimg.cn/20181205145506651.png)[清华团队带你实战区块链开发]
([https://ke.qq.com/course/344443?tuin=3d17195d](https://ke.qq.com/course/344443?tuin=3d17195d))
扫码获取海量视频及源码   QQ群：
721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111611182187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

