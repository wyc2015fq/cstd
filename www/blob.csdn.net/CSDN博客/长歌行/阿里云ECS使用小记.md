# 阿里云ECS使用小记 - 长歌行 - CSDN博客





置顶2019年01月23日 20:40:26[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：118








这篇文章主要还是做一个小记录，给自己备忘。>_<

## 停用DO

由于需要google上查阅一些资料，科学上网是必不可少的一步。之前一直是使用Digital Ocean（后文简称DO）的VPS，上面运行shaadowsock（后文简称ss），然后进行科学上网。最近发现，家里的机器已经连不上去了。ping得通IP，但甚至于部署在上面的web服务也连不上了。不知道不是不是网络服务商屏蔽了DO。之后又换了其他的网络，甚至手机热点，发现都连接不上。只好下定决心放弃这个快伴随了我三年的vps了。当时觉得这个ip还不错，就一直没换。现在看了一下，每个月5刀，这三年似乎也花了千元左右啊。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120110201910.png)

## 阿里云ECS方案

然后就考虑使用别的境外服务器，一开始考虑的是搬瓦工之类的，网络上这类的推荐的比较多，但是我觉得把，作为开发人员，当然是要了解下生态体系全面一点的服务商，最好能提供全家桶服务的，然后就决定先看看阿里云。

大体的思路就是：申请一个ECS->部署ss服务->本机安装ss客户端。当然由于自己也不确定我的网络服务提供商，支不支持阿里云科学上网，所以得是一个临时测试方案。

## 步骤

### 阿里云注册

由于只有海外服务器才支持科学上网，我一开始注册了一个香港站的账号，然后发现需要香港的手机验证，只好作罢，注册中国站的账号。可以使用淘宝登录，还挺方便的。生态体系果然全面。

### ECS购买

由于是测试方案，所以选择成本最小的就行了。我记得之前看到过一个优惠，一年的最低配ECS好像是293 [(link)](https://promotion.aliyun.com/ntms/act/qwbk.html?userCode=r3yteowb)，这个应该是年度的最低方案。不过我这次只是临时试试。直接选用最低配置，按量付费，一个小时7分钱，还不错。点击下一步，系统会提示需要预先充值100元。预充值费用应该是可以退的，所以我就充值了100。

[https://ecs-buy.aliyun.com/wizard?spm=5176.8789780.1092585.1.5fe257a8zju5Tb#/postpay/cn-hongkong](https://ecs-buy.aliyun.com/wizard?spm=5176.8789780.1092585.1.5fe257a8zju5Tb#/postpay/cn-hongkong)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120111620838.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)
### 网络和安全组

下一页需要选择网络和安全组。今天写博客的时候，我余额只有99.95，不足100，5分钱也不想继续充值。所以不能继续截图了。我就在这儿简单说下吧。主要就是网络带宽是使用固定带宽，还是按流量算。我当时选择了按带宽，1Mbps。价格就变成了0.15元一小时，反正能接受吧。

关于安全组，好像使用默认的就可以，也可以自己创建一个专有网络，然后基于这个专有网络创建安全组。专有网络和ecs的地区要对应上，我这里选择的的交换机可用区就是 香港可用区C
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120112452730.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)

安全组里面需要加上，将来对ss开放的端口，之前这一步都是在防火墙里面设置，阿里云里面需要在外面配置好安全组。
[https://ecs.console.aliyun.com/?spm=5176.2020520127.aliyun_sidebar.aliyun_sidebar_ecs.38231a782M763l#/securityGroup/region/cn-hongkong](https://ecs.console.aliyun.com/?spm=5176.2020520127.aliyun_sidebar.aliyun_sidebar_ecs.38231a782M763l#/securityGroup/region/cn-hongkong)
![](https://img-blog.csdnimg.cn/20190120112849919.png)

点击创建安全组，根据上一步创建的专有网络，创建一个安全组就行。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120112957757.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)

创建完毕之后，点击配置规则，把ss的端口加入就行。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120113049926.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)

全部配置完之后（后面好像就是设置密码之类的），就可以在实例列表里面看到自己的实例了。我这边已经是删除了，所以没有显示任何实例，选择了实例之后，就可以在网页里面，使用控制台远程连接了，不过那个远程连接码只出现一次，要好好记录。当然也可以直接ssh连接。
[https://ecs.console.aliyun.com/?spm=5176.2020520101.aliyun_sidebar.aliyun_sidebar_ecs.4d834df5Cvp5SC#/server/region/cn-hongkong](https://ecs.console.aliyun.com/?spm=5176.2020520101.aliyun_sidebar.aliyun_sidebar_ecs.4d834df5Cvp5SC#/server/region/cn-hongkong)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2019012011315150.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2l1aHNpaHNvdw==,size_16,color_FFFFFF,t_70)

这是最后创建的实例的配置情况。
> 
实例： 突发性能实例 t5系列 IV1核 512MB

I/O 优化实例： I/O 优化实例

系统盘： 高效云盘/dev/xvda40GB

公网带宽： 1Mbps按固定带宽

CPU： 1核

可用区： 香港可用区 C

操作系统： CentOS 7.6 64位

内存： 512MB

地域： 香港

虚拟交换机： vsw-j6cr6t*****kv4b6ikc

网络类型： 专有网络

云盾： 是（管家）
## SS配置

这部分主要就是参考这篇文章啦，里面内容很详细，我怕文章失效，在这儿放一些我当时的操作步骤。
[https://www.aliyunbaike.com/ecs/1374/](https://www.aliyunbaike.com/ecs/1374/)

### 服务端

使用root用户和之前设置的密码登录

运行三句话

```
wget --no-check-certificate -O shadowsocks-all.sh https://raw.githubusercontent.com/teddysun/shadowsocks_install/master/shadowsocks-all.sh

chmod +x shadowsocks-all.sh

./shadowsocks-all.sh 2>&1 | tee shadowsocks-all.log
```

后面需要输入数字做一些选择，我选的python， P@ssword,  8388，aec-256-cf。这里的选择要记住，后面配置客户端需要。

### 客户端

就是自己的机器啦，我这里就用window机器为例子。

先下载客户端，
[https://github.com/shadowsocks/shadowsocks-windows/releases](https://github.com/shadowsocks/shadowsocks-windows/releases)

解压之后，使用界面添加上之前的内容
```
{
      "server": "47.75.***.236",
      "server_port": 8388,
      "password": "P@ssword",
      "method": "aes-256-cfb",
      "plugin": "",
      "plugin_opts": "",
      "plugin_args": "",
      "remarks": "aliyun",
      "timeout": 5
    },
```

chrome端要上网的话，可以下载（或者别人传一个）switchyOmega，进行简单配置
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120114812706.png)

配置完之后，打开google什么的很顺畅。发现无论是控制台的访问速度，还是代理的网速都比DO的要快。但是包月一个月60多块钱，还是比5刀要贵不少。测试完毕之后，我删除ECS实例，但是账单还没有打出来，后来账单出来之后，5分钱。恩，所以临时上网，用这个方案还不错。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190120112615746.png)










