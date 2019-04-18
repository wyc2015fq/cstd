# 内网穿透：在公网访问你家的 NAS - 知乎
# 



家里 Nas 中安装了一系列服务，如果外网访问不了，手机上连不上，就十分不方便。所以我们需要内网穿透，方法主要有两个：动态域名和反向代理，下面对比一下：

**动态域名**

最早期的方法，要求家中拨号网络有一个外网 ip，每次断线拨号虽然 ip 变了，通过定时请求动态域名提供商的一个接口，把域名指向改成你最新的外网地址，这样其他地方就可以通过域名访问你家里的 IP 了。

一般动态域名路由器都支持设置，不支持也没关系，你家里的服务器的定时请求一次 DDNS 服务商接口即可，比如 no-ip 的 [http://ddns.net](https://link.zhihu.com/?target=http%3A//ddns.net) 域名更新：


```bash
https://USER:PASSWD@dynupdate.no-ip.com/nic/update?hostname=test.ddns.net
```


在家里服务器的 crontab 中设置每 15 分钟 curl 访问一下上面的地址，把用户名密码和hostname替换成你申请的域名，即可更新。

最后家中路由器需配置端口转发，比如把外网 ip 的 443 端口转给内网服务器的 443 端口。

这是最廉价的解决方案，基本没成本，各类 DDNS 服务商也是免费的。唯一问题是当 IP 更新后，最长可能需要 15 分钟才能更新域名指向（更新接口调用快会被 ban），再算上各级 DNS 缓存时间，前后可能会有半小时你无法访问，不过我一周顶多碰到 1-2 次。

如果家里有外网 IP ，这是首选方案，现在越来越多宽带新开户无法得到外网 IP 地址，比如电信。联通还好，曾经不经我允许把我换成内网 IP，然后我打电话去骂，骂来了一个外网ip。但随着 IPv4 资源减少，IPV6 普及以前，外网 IP 肯定越来越少，所以我们需要反向代理。




**反向代理**

家中没有外网 IP 时，如果你有一台固定 IP 的外网 VPS，那么可用它做跳板访问家里内网端口。在 VPS 上启动一个反向代理服务，同时在家里内网服务器上启动另外一个映射服务，那么链接外网 VPS 的 443 端口，可以被反向代理转发到内网的 443 端口上。

最简单的方案是 ssh -R 反向代理，比如我想把内网另外 192.168.1.3:443 端口暴露给 VPS （ 202.115.8.1 ）的 8443 端口，可以在任意一台内网机器上运行：


```bash
ssh -R 0.0.0.0:8443:192.168.1.3:443  user@202.115.8.1
```


这样会让远端（-R ）创建一个新端口（ 8443 ），并把该端口的数据通过 ssh 链接传回来给我，我再把它转发给我能访问的 192.168.1.3:443。在 vps 修改下 /etc/ssh/sshd_config：

`GatewayPorts yes`

这样连接外网 vps 上的 8443 端口相当于连接内网 192.168.1.3 的 443 端口了。

该方法没有断线重连，连接断了 ssh 进程就会退出，需要额外写脚本来检测重启，或者用 autossh 保活。正儿八经长期用的话，推荐使用专业程序来提供更加稳定高效的方向代理。

这类服务程序有很多，比如老牌的 [ngrok](https://link.zhihu.com/?target=https%3A//github.com/inconshreveable/ngrok)，python 写的 [shootback](https://link.zhihu.com/?target=https%3A//github.com/aploium/shootback)，前者使用麻烦，如果想跑在自架服务器上，还要重新编译代码，配置证书，而后者性能和并发又不是特别好，所以现在大家都用性能更好，配置更简单的 [frp](https://link.zhihu.com/?target=https%3A//github.com/fatedier/frp) 。




**实际部署**

Frp 结构很简单，分为 frps/frpc 两个可执行程序，在有公网地址的 VPS 上运行 frps 然后在家中内网运行 frpc 就行：
![](https://pic1.zhimg.com/v2-f6b5daab2acf5e8b74a01d8d7fd9386c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='532' height='456'></svg>)
比如想把家中服务器的 ssh 端口 22 代理代理到外网 vps 202.115.8.1:221 上，那么在 vps 上编辑 frps.ini 配置文件：


```
# frps.ini
[common]
bind_port = 7000
token = 1234567
```


然后 vps 上启动 frps：


```bash
frps -c /usr/local/etc/frps.ini
```


然后再家中内网服务器上编辑 frpc.ini 配置，token需要保持一致：


```
# frpc.ini
[common]
server_addr = 202.115.8.1
server_port = 7000
token = 1234567

[ssh]
type = tcp
local_ip = 127.0.0.1
local_port = 22
remote_port = 221
```


然后内网服务器启动 frpc：


```bash
frpc -c /usr/local/etc/frpc.ini
```


反向代理就架设成功了，内网的 frpc 启动后会链接到外网的 frps 上的 7000 管理端口，申明自己想把内部的 22 端口暴露成外部的 221 端口。然后外网的 frps 一旦碰到任何 221 端口的链接，都会通过 frpc<->frps 的数据链路转发到内网的 22 端口上来，你在外网任意地方：

`ssh -p221 user@202.115.8.1`

相当于登陆家里服务器的 22 端口。你还可以同时配置多个服务，比如增加一个 https 服务：


```
# frpc.ini
[common]
server_addr = 202.115.8.1
server_port = 7000
token = 1234567

[ssh]
type = tcp
local_ip = 127.0.0.1
local_port = 22
remote_port = 221

[https]
type = tcp
local_ip = 192.168.1.3
local_port = 443
remote_port = 8443
```


这样可以把内网服务器的 [https://192.168.1.3:443](https://link.zhihu.com/?target=https%3A//192.168.1.3/) 暴露成：[https://202.115.8.1:8443](https://link.zhihu.com/?target=https%3A//202.115.8.1%3A8443/) 。




**HTTP/HTTPS 的穿透优化**

其实 frps 对于 http/https 还有更友好的配置方式，你有多个域名的话，你可以用多个二级域名的 A 记录同时指向外网 vps 的地址：
- [nextcloud.skywind.com](https://link.zhihu.com/?target=http%3A//nextcloud.skywind.com/)   -> 202.115.8.1
- [gitlab.skywind.com](https://link.zhihu.com/?target=http%3A//gitlab.skywind.com/) -> 202.115.8.1

配置两个二级域名都指向 VPS 外网地址，对于多个 http/https 转发，在 frps 那里都可以共用同一个端口，我们配置下 frps.ini：


```
# frps.ini
[common]
bind_port = 7000
token = 1234567
vhost_http_port = 80
vhost_https_port = 443
```


然后配置内网的 frpc.ini ：


```
# frpc.ini
[common] 
server_addr = 202.115.8.1 
server_port = 7000 
token = 1234567

[nextcloud]
type = https
local_ip = 192.168.1.3
local_port = 443
custom_domains = nextcloud.skywind.com

[gitlab]
type = https
local_ip = 192.168.1.4
local_port = 443
custom_domains = gitlab.skywind.com
```


那么这两个服务都可以公用外网 VPS 的 443 端口，当你用浏览器访问：

[https://nextcloud.skywind.com](https://link.zhihu.com/?target=http%3A//nextcloud.skywind.com/)

外网 VPS 上的 frps 就能根据域名知道该请求需要代理到内网 nextcloud 地址，而访问

[https://gitlab.skywind.com](https://link.zhihu.com/?target=https%3A//gitlab.skywind.com/)

时，因为两个域名都是指向外网 VPS 的，frps 根据域名判断就能转发到内网 gitlab 地址。

如果你的 frpc.ini 中 http/https 服务你用的 type=tcp，那每个服务都会占用一个外网 vps 的端口，而把 type 设置成 http/https 的话，只要你有几个二级域名，就能共用 frps 的端口：
![](https://pic4.zhimg.com/v2-1e2cdc04b115883f99b58dbfea11fe43_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1267' height='768'></svg>)
上图时我在公司通过外网 frps 跳板，访问家中 GitBucket 代码服务的截图。通过 type = https，你可以把家中的：文件服务，照片服务，代码服务，笔记服务 等一些列常用的服务全部代理到外网 VPS 的 443 端口上来，然后用域名进行区分。

这样在外面不管用 PC 还是手机访问家中的服务，外网 URL 会变得很漂亮。




**内网静态域名**

那么我们把内网的一个站点用反向代理做穿透时，我们在外面访问的是 VPS 的域名，在家又访问另外一个 192.168.1.x 的裸 IP 的 URL，有没有办法都用一个地址呢？

如果你家里的路由器支持配置静态 DNS 的话，可以把外网 VPS 的 DNS 配置成内网地址，这样你连上家里 wifi 时，[git.skywind.com](https://link.zhihu.com/?target=http%3A//skywind.com/) 被解析成 192.168.1.x，在外网时，[git.skywind.com](https://link.zhihu.com/?target=http%3A//skywind.com/) 又会被解析成 202.115.8.1 ，总能用同一个域名，以最快的速度访问。

比如手机上设置 nextcloud 客户端时就只需要设置唯一的一个服务器 URL 不用改来改去。




**使用 KCP 加速**

如果你手上只有境外 VPS，那么可以通过 UDP 加速传输。Frp 支持使用基于 UDP 的 KCP 协议加速传输，这样对于一些网速慢的 vps 而言，能有比 tcp 更平滑更快速的传输效果。




**虚拟网络与 P2P**

评论区提到的 zerotier，用起来和 frp 的 p2p 模式差不多，都是需要在客户端启动一个软件。采用 p2p 模式对带宽利用更充分，但/是稳定性不如 c/s 模型，我的意见是，只要你不传电影，只是和内网之间传送一些文档，照片，代码之类的东西，那用更稳定 c/s 模式。

如果你希望像 zerotier 一样，搞出个虚拟网卡来，然后像在内网一样的直接访问各个 IP 地址，那么你可以在内网启动个 openvpn 嘛，然后用 frp 把 openvpn 的端口暴露出来，你在外面就可以用 openvpn 的虚拟网卡模拟链入到内网，直接访问内网的所有东西了。




**其他穿透方案**

如果你不想用 frp 也这类软件，更不想自己架设外网服务器的话，可以用花生壳的反向代理付费服务，或者 ngrok 的付费服务，总之消耗带宽的事情，都是要成本的。

其实反向代理唯一的问题就是 VPS 的费用，好歹程序员一般手上都会有几台 VPS，带宽又没跑满，正好拿来做跳板进行内网穿透。以前我喜欢把服务直接架设在外网 VPS 上，比如我的笔记服务，但是经常 VPS 换 IP，换服务商，数据就没法保证，VPS 迁移了又要面临服务架设和数据迁移，十分复杂。

现在我所有服务都跑在家里服务器上，所有数据都自己掌握，外网 VPS 仅作为一个转发，几年下来，数据安全性和稳定性比直接把服务架设在 VPS 好多了，不用每换一台 VPS 就重新弄一遍。




--

有网友给我推荐了个牛逼的项目，[Sakura Frp](https://link.zhihu.com/?target=https%3A//www.natfrp.org/)，基于 Frp 二次开发的，免费提供内网穿透服务。普通用户的映射最高速率可达到 8Mbps，且不限流量。

这种良心项目必须支持一下。

--

参考阅读：

[韦易笑：常用公有云的替换方案](https://zhuanlan.zhihu.com/p/50335990)

[韦易笑：Nas 系统的虚拟化方案](https://zhuanlan.zhihu.com/p/55025102)






