# DNS 解析器性能比较：CloudFlare vs Google vs Quad9 - 文章 - 伯乐在线
原文出处： [medium](https://medium.com/@nykolas.z/dns-resolvers-performance-compared-cloudflare-x-google-x-quad9-x-opendns-149e803734e5)   译文出处：[开源中国社区](https://www.oschina.net/news/94836/dns-resolvers-performance-compared)
几个月前，我做了一些可用的顶级免费 DNS 解析器之间的性能比较。恰好在 Quad9 发布之后，我试图根据性能决定使用哪一个：Google，OpenDNS，Quad9 等等…
CloudFlare 是最了解互联网性能的公司之一，最近推出了自己的免费 DNS 解析器。它默认支持基于 TLS 和 HTTPS 上的 DNS。
现在进行一个更加有趣的测试：Google 的 8.8.8.8，Quad9 的 9.9.9.9 和 CloudFlare 的 1.1.1.1  ，以及OpenDNS 的 208.67.222.222 和一些其他供应商进行比较。
### 供应商测试
这些是选择评估的前 8 位免费 DNS 提供商：
- **Google 8.8.8.8**：私有和未过滤，最受欢迎。
- **CloudFlare 1.1.1.1**：私有和未过滤的新玩家。
- **Quad 9 9.9.9.9**：私有和安全意识很高的新玩家。
- **OpenDNS 208.67.222.222**：阻止恶意网域并提供阻止成人内容的旧玩家。
- **Norton DNS 199.85.126.20**：阻止恶意域名并与其防病毒软件集成的旧玩家。
- **CleanBrowsing 185.228.168.168**：私有和阻止访问成人内容，新玩家。
- **Yandex DNS 77.88.8.7**：阻止恶意域名的旧玩家，在俄罗斯非常受欢迎。
- **Comodo DNS 8.26.56.26**：阻止恶意域名的旧玩家。
这是他们之间关于隐私选项的快速功能分解：
![](https://static.oschina.net/uploads/space/2018/0402/175742_67ED_3703517.png)
### 地点
从全球18个地点运行测试。主要使用 VPS 提供商和一些宽带位置尝试从尽可能多的地方查询他们的DNS。它会告诉我们它们的连接程度如何，数据中心的位置以及某个人在该领域的经验有多接近。选择的地点：
- **北美**：圣地亚哥，洛杉矶，纽约，多伦多，蒙特利尔，亚特兰大，达拉斯，弗里蒙特，旧金山
- **欧洲**：伦敦，巴黎，阿姆斯特丹，法兰克福
- **亚洲**：东京，新加坡，班加罗尔（印度），悉尼，布里斯班（澳大利亚）
- **南美洲**：圣保罗
### 结果摘要
测试非常简单，针对不同流行的域名（google, facebook, twitter, gmail 等）在整个一小时内执行了 70 次DNS 查询。对每个位置的所有请求进行平均，以获得每个 DNS 解析程序的整体性能指标。
#### TLDR /摘要
- 所有供应商（Yandex 除外）在北美和欧洲表现非常好。他们在美国，加拿大和欧洲的响应时间均小于 15 毫秒。然而，亚洲和南美洲在总体平均数上有所不同，因为一些提供商在那里连接不好。
- CloudFlare 是所有位置中最快的 DNS。它在全球的平均值为 4.98 毫秒，令人惊叹。
- 谷歌和 Quad9 分别接近第二和第三。Quad9 在北美和欧洲比谷歌速度快，但在亚洲/南美洲表现不佳。
- CloudFlare 在任何地方都有强大的存在。尽管 Google 和 Quad9 在某些地方的响应时间很高，但CloudFlare 在各个地方都表现良好。
- Yandex 仅适用于俄罗斯。它不像其他提供商那样使用 Anycast，并且在任何地方都非常缓慢。
- CleanBrowsing 是成人（色情）内容过滤的最快速的提供商。
#### 全球平均
> 
＃1 CloudFlare：4.98 ms
＃2 Google：16.44 ms
＃3 Quad9：18.25 ms
＃4 CleanBrowsing：19.14 ms
＃5 Norton：34.75 ms
＃6 OpenDNS：46.51 ms
＃7 Comodo：71.90
＃8 Yandex：169.91
#### 北美平均水平
> 
＃1 CloudFlare：3.93 ms
＃2 Quad9：7.21 ms
＃3 Norton：8.32 ms
＃4 Google：8.53 ms
＃5 CleanBrowsing：11.83 ms
＃6 OpenDNS：14.66 ms
＃7 Comodo：25.91 ms
＃8 Yandex：119.09 ms
#### 欧洲平均
> 
＃1 CloudFlare：2.96
＃2 Quad9：4.35
＃3 CleanBrowsing：5.74
＃4 Google：7.17
＃5 OpenDNS：8.99
＃6 Norton：10.35
＃7 Comodo：13.06
＃8 Yandex：35.74
