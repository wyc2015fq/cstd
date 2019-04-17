# 还没抢到票？快试试新开源的Python工具，多任务多日期多账号同时抢 - 知乎
# 



## 栗子 发自 凹非寺 量子位 报道 | 公众号 QbitAI

> 据统计，量子位有超过50%的员工，目前为止没抢到票。

买了加速包，好久也没动静。

能依靠的只有勤劳的双手，但每天收到的都是12306送来的温馨：
![](https://pic1.zhimg.com/v2-1c3e46c79465c96ddcb84404e59f4780_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='825'></svg>)
家门口没票，就买去终点；北京没票，就坐车去下一站；实在不行少买几站上车再补……就算接受一切妥协，也还是失败。

还好这个世界有**Python**，可以弥补人类手速的缺陷。

一位名叫pjialin的程序猿，试用众多开源抢票助手 (包括testerSunshine出品，以及bypass12306) 无果，就自己写了新的分布式算法，名曰**py12306**，刚刚发布不久。
![](https://pic3.zhimg.com/v2-66eb4a9001bad567bed5c1591796e3ce_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='696' height='230'></svg>)
它可以在一班火车上，观察许多个出发站和到达站的排列组合，还有其他许多机智的功能。

有了它，程序猿开心地买到了好几张票。而且，不止他一人亲测成功：
![](https://pic2.zhimg.com/v2-5752ed9df83c807be4dae4dc8160bf61_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='448' height='92'></svg>)
有些小伙伴已在运行代码，也有多人使用前先表示了资瓷，GitHub标星与日俱增：
![](https://pic2.zhimg.com/v2-7ccaff308f64e2c1a379cd7bff30b281_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='732' height='442'></svg>)
## **多任务，多日期，多账号**

py12306购票助手，主要功能有这些：

> **· 多日期查询余票**
**· 自动打码下单**
· 用户状态恢复
· 电话语音通知
**· 多账号多任务多线程支持**
**· 单个任务多站点查询**
· 分布式运行
· Docker支持
· 动态修改配置文件
· 邮件通知

就像开头提到的，它能在一个任务 (一班列车) 里，同时查看多个出发站，多个到达站；

并且支持**多任务** (多班列车) 、**多日期**、**多账号**一同查看。

总之，是人类难以企及的多线程。
![](https://pic1.zhimg.com/v2-72fade46c36170f292da852bef4c0208_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='308' height='201'></svg>)
另外，也支持**自动下单**和**电话通知**。

程序员说，这个拥有分布式集群的系统，每个节点都能处理任务，一个服务器挂了不影响其他的。所以，有条件的同学可以让好几台服务器一起刷。
![](https://pic2.zhimg.com/v2-27e52fb635823dd04d038fad9eb75f95_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1052' height='172'></svg>)
## **怎么用呢？**

首先，程序猿提示，请选用python 3.6以上版本。

**第一步**，安装依赖项：


```
1git clone https://github.com/pjialin/py12306
2
3pip install -r requirements.txt
```


**第二步**，配置程序：

`1cp env.py.example env.py`

**· 自动打码功能**

支持**免费打码**和**若快打码**。免费打码无法保证持续可用，失效需手动切换到若快平台：

到[http://ruokuai.com](https://link.zhihu.com/?target=http%3A//ruokuai.com)注册一个账号，再填到配置中。

**· 语音通知功能**

语音验证码来自阿里云API市场的一个服务商。

要到[https://market.aliyun.com/products/57126001/cmapi019902.html](https://link.zhihu.com/?target=https%3A//market.aliyun.com/products/57126001/cmapi019902.html) 购买，再把appcode填到配置中。
![](https://pic1.zhimg.com/v2-c5d7c5056a59004b42cf35499c8442a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='240' height='240'></svg>)
**第三步**，启动前测试：

py12306提供了一些简单测试，比如用户账号检测，乘客信息检测，车站检测等等。

**· 开始测试配置信息 -t**

`1python main.py -t`

**· 测试通知消息 -t -n**


```
1# 默认不会进行通知测试，要对通知进行测试需要加上 -n 参数 
2python main.py -t -n
```


**第四步**，运行程序：

`1python main.py`

然后，来观摩一下程序猿抢到票的喜悦：
![](https://pic3.zhimg.com/v2-ceaf7430fdb13c9b042b2be8bf33e8ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='838' height='1168'></svg>)
订单提交成功，真是值得庆祝的瞬间，有接二连三的**叹号**为证。

值此良辰，不免要向不幸没抢到票的同学显摆与没买到票的小伙伴分享经验。

pjialin把代码放上GitHub，并对同样写过购票小助手的**testerSunshine**前辈表示了感谢，因为向前辈借鉴了部分实现。

另外，虽然今年的票买好了，温柔的程序猿依然在继续更新这个项目：
![](https://pic1.zhimg.com/v2-933d5c5cdf7b578c5646b395e161ad78_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='408' height='512'></svg>)
下一步，可能会给最后两个方框也打上勾：
![](https://pic1.zhimg.com/v2-31cf9d23e7fc46aebfad4e1caf3b9700_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='346' height='150'></svg>)
这里，遥祝pjialin好人一生平安，每年都能轻易买到回家的车票。

## **4000星的大前辈**

方才提到，py12306吸取的经验来自**TesterSunshine** (文贤平/文先森) 。
![](https://pic4.zhimg.com/v2-c722bfea1ad9a0f929df7f46b7f7cb0b_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='496' height='94'></svg>)
文先森的**12306购票小助手**在2018年年初诞生，如今已在抢票界德高望重：目前标星**4000+**，昨天一天上涨200+。

从余票查询到下单完成，节奏紧张而明快：
![](https://pic1.zhimg.com/v2-6a5719925eb6fb211fb7dc21fcb72624_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='968' height='790'></svg>)
文先森还在项目页面挂了两个二维码，方便抢票成功的小伙伴们激动地打赏。

如果你还没抢到票，也先观赏一下成功的样子：
![](https://pic2.zhimg.com/v2-ed9acfe92ccc3b1214cefb2ffc4d3ca5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='391'></svg>)
哪个步骤失败的话，还可以加群汇报一下，说不定大佬能帮你调出来：
![](https://pic3.zhimg.com/v2-8f666e9fa08b7d6b3a8253920bdf00ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='555'></svg>)
祝你成功。

## **程序猿拯救世界**

2010年，**[http://12306.cn](https://link.zhihu.com/?target=http%3A//12306.cn)**含着祝福来到这个世界，于**1月30日**春运首日开通试运行。

而程序猿用自己的方式查询余票的活动，在这之前就已经开始了 (只要下图日期没有bug) ：
![](https://pic2.zhimg.com/v2-772fceaf26bf483ff94b2244328e3d79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='531'></svg>)△ 来自名叫李凡希的江苏少年
或许是职业敏感，让程序猿群体早早开始担心，12306体弱多病，会令人类买不到票。

只是，(上图) 这个古老的项目已经404了。

而GitHub上依然健在的、最年长的订票助手，可能是**iFish**在2012年1月9日发布的浏览器插件：

## **一个插件，拖垮GitHub**
![](https://pic1.zhimg.com/v2-3271aaba4015b02ae574ac4c1299f220_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='766' height='270'></svg>)
就是这只助手，在2013年1月15日订票小高峰来临之时，引发了著名的“12306订票助手拖垮GitHub”事件。

当年，许多浏览器推出的春节版本都搭载了这个插件，连12306官方也引用了项目里的一个资源。

而插件的早期版本，用GitHub的Raw File服务作CDN，并且如果返回403错误，就5秒重试一次，永久重试。

巨大的访问量，导致Github受到DDOS攻击，速度扑街。GitHub甚至想找人联系12306官方，让对方去除这个引用。

iFish大佬，显然低估了订票助手的用户数。
![](https://pic3.zhimg.com/v2-7b7ede94c7c4cb8291e84c491463e58a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='170'></svg>)△ 我要回家了
一个程序猿想回家，可能没有谁能阻拦。

毕竟，连GitHub都能拖垮，还怕买不到票？

## **还是有风险**

那么，抢票应用会不会被河蟹？

新京报说，抢票软件的存在**确实会扰乱正常的市场购票秩序**，但抢票软件提供的服务属于在技术上通过计算机的方式代替人工购票，性质和黄牛倒票不同。

目前为止，还没有抢票软件受到过行政处罚。

不过，相关法规说不定已经在路上了。

所以，有资源的好时节，大家一定要珍惜啊。

py12306购票小助手传送门 (pjialin) ：
[https://github.com/pjialin/py12306/](https://link.zhihu.com/?target=https%3A//github.com/pjialin/py12306/)

12306购票小助手 (testerSunshine) ：
[https://github.com/testerSunshine/12306](https://link.zhihu.com/?target=https%3A//github.com/testerSunshine/12306)

[http://12306.CN](https://link.zhihu.com/?target=http%3A//12306.CN)订票助手 (iFish) ：
[https://github.com/iccfish/12306_ticket_helper](https://link.zhihu.com/?target=https%3A//github.com/iccfish/12306_ticket_helper)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


