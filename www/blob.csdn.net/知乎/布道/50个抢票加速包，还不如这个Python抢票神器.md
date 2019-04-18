# 50个抢票加速包，还不如这个Python抢票神器 - 知乎
# 



> 如点赞超过5K，我就做成移动版Web工具，免费提供给大家使用！需要的，顺便点个赞，想用的请关注专栏！

———————————————————
**抢到票的同学，别忘了把文章分享给身边需要的人！考虑到有些同学上不了 GitHub，我已经上传到百度网盘了，需要的请点击链接下载**
[https://pan.baidu.com/s/1YvtnJljnaAC0vU0rdEttRA](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1YvtnJljnaAC0vU0rdEttRA)
**提取码：zr7u **

但今年各种抢票软件的横行，还有官方出的加速包，导致连黄牛都不敢保证能买到票。你无奈的只能一起加入抢票大军。

从不花钱的低速，到中速、高速、极速、光速、VIP，六种抢票速度，越快当然抢到的几率也就更高，但每升一级都需要你花 10 个加速包才能实现。

而加速包的获取方式有两种：1 直接花钱买，1 元 1 个价格，全套下来大概 50 元左右。2 是好友助力，每邀请一个好友帮你助力就能获取 2～5 不等的加速包。
![](https://pic2.zhimg.com/v2-0e81e3fc24414a43602411db58a21e61_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='858'></svg>)![](https://pic1.zhimg.com/v2-9a997c93144cfc9ae71f63aeddbc7d4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='789'></svg>)
大部分人都是不愿意花钱买加速的，所以你开始群发好友，轰炸微信群，朋友圈吆喝，挂 N 个抢票软件，各种你能想到的手段，都尝试一遍，但无一都失败了。

今天我就给大家介绍一个开源的 Python 抢票程序，亲测有效！我身边已经有很多好友，通过这个程序抢到车票了。不仅免费开源，还非常有用。

抢票神器：12306 购票小助手
![](https://pic2.zhimg.com/v2-70635a6b1f2504a159e7e919ad8d6ad1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='899'></svg>)
项目已经在 GitHub 上开源了，地址：

`https://github.com/testerSunshine/12306`

作者也很用心的把程序的思路给画了出来，我们可以简单的看一下，整个思路其实就是模拟一个正常人购票的方式：
- 首先查询下车票剩余的票数，如果有座位提交订单，出现验证码就识别验证码。
- 随后就循环点击提交按钮，这里作者就做了很多的条件判断，比如出现异常则重新查询，提交订单失败也重新查询，直至获取订单成功。
- 订票成功之后还有一个通知机制，即发送到你的邮箱里。

项目使用到的 Python 库

接下来我们查看这个项目都用到哪些 Python 库。打开 requirements.txt 文件，这里面把项目所用到的库大致都写了出来。
![](https://pic3.zhimg.com/v2-8d35b029240b117588b9de74b2427f42_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='695'></svg>)
网络请求用到了经典的 requests 库，而页面解析用到了 bs4 和 beautifulsoup4。

有人可能会问这两个解析库有什么区别？bs4 是用在 python3 版本上的，而 beautifulsoup4 是用在 python2 版本上。我最开始学 beautifulsoup 的时候就被这个坑过。

**Pillow：**Pillow 是很经典的 Python 图像处理库，可以大致猜测作者用这个库来解决简单验证码的问题。

**PyYAML：**YAML 是一种直观的能够被电脑识别的的数据序列化格式，容易被人类阅读，并且容易和脚本语言交互。

YAML 类似于 XML，但是语法比 XML 简单得多。PyYAML 是 YAML 的 Python 实现，可以用于参数化 Python 对象，用来当做配置文件。

**Six：**众所周知 Python 2 和 Python 3 版本的分裂给 Python 开发者们带来了很大的烦恼，为了使代码同时兼容两个版本，往往要增加大量的代码，于是 Six 出现了。

正如它的介绍所说，它是一个专门用来兼容 Python 2 和 Python 3 的库。它解决了诸如 urllib 的部分方法不兼容，str 和 bytes 类型不兼容等“知名”问题。

到这里就可以看出作者这个程序是考虑到 Python 不同版本之间的兼容性问题的，简直业界良心。

**wrapcache：**一个基于 Python 装饰器 Decorators 的方法缓存系统，用于缓存 Python 方法的输出值，可以支持复杂数据类型，可以缓存到 Redis 中、Python dict、LUR 算法存储中。

**ntplib：**这个库是用来在 Windows 上同步校准当地时间的。

安装使用
![](https://pic1.zhimg.com/v2-cad1826aeaf2c105feed748a32d9cf68_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='819'></svg>)
上图在 GitHub 仓库中都有写到。我这里就简单的讲下，首先你需要注册「若快」的打码平台，为了可以让程序自动打码。随后安装程序依赖包，安装方法上面已经写到了。

最后程序有一个很重要的文件叫「ticket_config.yaml」：
![](https://pic1.zhimg.com/v2-feaeba95a87230d8b485fe616c8d33ac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='866' height='642'></svg>)
所有购买车票的信息都是在这个文件里配置的，比如车票时间，12306 账号密码，乘车人信息，通知邮箱等等，作者在文件中都有详细的注释。
![](https://pic3.zhimg.com/v2-6adc4fd59f7270ef272318e94c7dc012_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='1220'></svg>)
把这个配置文件按你的需求填写完毕之后，大家就可以开始运行了。

运行 run.py 文件即可，亲测有效，如下图：
![](https://pic3.zhimg.com/v2-579533b3f5318e43f4deffcf0a19993a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='407'></svg>)![](https://pic4.zhimg.com/v2-9838366954ecdda4c5d31bc92545896f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='488'></svg>)



**抢到票的同学，别忘了把文章分享给身边需要的人！考虑到有些同学上不了 GitHub，我已经上传到百度网盘了，需要的请点击链接下载**

[https://pan.baidu.com/s/1YvtnJljnaAC0vU0rdEttRA](https://link.zhihu.com/?target=https%3A//pan.baidu.com/s/1YvtnJljnaAC0vU0rdEttRA)

**提取码：zr7u **




**如果帮您抢到票了，请主人打赏一下，在这里先谢过了！**




> *作者：痴海*
*简介：只要把握风口，猪也能飞起来。而如今编程的风口，值得每个人关注。痴海，一个专注于 Python 编程，每天推送各种 Python 干货文章，资源分享，面试真题，爬虫实战、数据分析。但这不仅仅是一个技术公众号，每周还会分享职场、人生、认知原创文章，想要「提高认知和编程」的强烈推荐关注。*


