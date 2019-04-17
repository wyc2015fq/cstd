# 徐锦江、洪世贤、梁非凡，B站名场面全被AI换脸调戏了一遍 - 知乎
# 



> 边策 问耕 假装发自 凹非寺
量子位 出品 | 公众号 QbitAI

没想到又看到徐锦江新片了。

而且是国际巨作。

比方对战灭霸。
![](https://pic3.zhimg.com/v2-e6361180bb2cb9280de53a67f645814a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='334'></svg>)
这是什么新片？？？怎么感觉似曾相识？？？好吧，这不是新片，这又是一次利用AI技术，成功换脸影视作品的案例。

光荣归属于微博用户@慢三与偏见 。他找了好多素材，最终把海王、雷神的脸，换成了徐锦江的脸。

我们截取了几个片段，看看效果：
![](https://pic4.zhimg.com/v2-70cc16abea9e6937f3249dbb4c8208df_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='334'></svg>)![](https://pic3.zhimg.com/v2-2137f43c40fedbdfe707f769466fa4c6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='334'></svg>)![](https://pic4.zhimg.com/v2-31e069bc1534566cfcc99572653a4ed3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='334'></svg>)
这段视频一放出，网友们就炸了。

微博上大家纷纷留言说：“？？？原本海王长什么样来着”、“雷神短发真的毫无违和”、“鳌拜，你怎么了鳌拜”、“谁能想到徐锦江息影之后这么火”……

B站上也是如此。
![](https://pic2.zhimg.com/v2-18e8a16d92f25a7cfff9bc7985a25d55_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='412'></svg>)![](https://pic1.zhimg.com/v2-1a01a796942d92d143a3fff6d5bdd38c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='412'></svg>)
连徐锦江本人都被炸了出来：
![](https://pic1.zhimg.com/v2-6fd1660102c75254a286115ef1ae07ac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='618' height='324'></svg>)
AI换脸，出现已经有一段时间了。我们第一次报道还是去年1月。不过，之前都是以女星路线为主，从国外到国内都是如此，比方前段时间的朱茵换脸杨幂。
![](https://pic2.zhimg.com/v2-ceaadc83684ed29238e766eae10460b5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
现在趋势变了，男星路线也开始流行。除了徐锦江，最近比较火的还有两段“名场面”加入换脸套餐。第一段是，梁非凡和刘醒。

这段出自电视剧《巾帼枭雄之义海豪情》，刑侦队长刘醒面对上司，多年的积怨终于爆发了，脱口而出那句名言：“吃屎吧，梁非凡。”

B站up主“用户名未填写”，把这个经典片断中的两位主角，刘醒和梁非凡换了个脸。于是，这个场景变成了下面这个样子……
![](https://pic4.zhimg.com/v2-88eb32c249b8c82f7fb8a44c4e3741b3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='336'></svg>)![](https://pic4.zhimg.com/v2-81f6574e4274c0e7bc441fdf940a34df_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='336'></svg>)
还是这位up主，几天后又上传了一段更火爆的AI换脸作品。

这段视频的出自电视剧《回家的诱惑》，在AI换脸技术的帮助下，男主角洪世贤自己的脸，被换到了女性角色艾莉的脸上。

所以这个场景，现在成了这般模样。
![](https://pic4.zhimg.com/v2-dd0a9c6db817826878328cfaa137d7b7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='334'></svg>)![](https://pic3.zhimg.com/v2-d9290206e6477d047abb2a5504914b16_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='334'></svg>)
一大票网友被这个换脸笑死，说魔性、说邪术……

搞定这个难不难？我们之前介绍过怎么才能自学这门技术。今天，我们再从硬件上来说，怎么才能搭建一套这样的系统。

## **有台这样的电脑，你也能换脸**

目前网上已经有fakeapp、faceswap、deepfacelab等一大批现成换脸软件，只要有够强的硬件，你也能自制一段换脸视频。

这些软件都一个基本要求，就是必须支持英伟达的CUDA。

通俗地说，如果你最近几年为了玩吃鸡这类游戏配置了一台PC，而且安装的是英伟达的独立显卡，那么运行Deepfakes应该是没问题的。

根据国外网友的实测效果，要运行fakeapp且电脑不崩溃，至少需要以下配置：
- 至少有2GB显存的英伟达GPU
- 英特尔i3或者AMD 9处理器
- 8GB内存
- 20GB剩余硬盘空间

以上只是最低的配置，制作一个换脸视频可能需要几天才行，是不是感到电费在燃烧？如果为了省事中途截断，效果会惨不忍睹。

去年科技媒体The Verge的记者还真试了一把，她的电脑给人换脸大概需要一天的时间，如果只训练几个小时，结果根本没法看：
![](https://pic2.zhimg.com/v2-fd387e9b309cfe1d92bf35180c8b83ed_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='478' height='268'></svg>)
从电脑屏幕上来看，训练过程是这样的：
![](https://pic4.zhimg.com/v2-a54069c48c0259355f020a58ece14c3f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='296'></svg>)
如果你不想每天早晨打开电脑都看到上面的画面，那么我们推荐以下配置，它能讲训练时间缩短到只有几个小时：
- 至少4GB显存的英伟达GPU（GTX 9系或者更高，多数网友推荐GTX 1060 6GB显存，售价1699元）
- 英特尔i5或AMD Ryzen处理器（以i5-8400为例，售价1599元）
12GB内存（购买2条8GB内存，总价700元）
- 100GB剩余硬盘空间（考虑到其他占用空间，购买250GB固态硬盘，价格大约300元）

以上只是主要部件的价格，再加上主板、电源、机箱，总的主机（不包括显示器）价格区间在**5000~6000元**之间。

需要注意的是，在这里，硬盘空间比硬盘读写速度更重要。

因为训练Deepfake过程中大约每分钟会产生1GB的图像文件，通常SSD的空间较小，如果被训练数据占满，将会导致SSD性能下降。而机械硬盘的性能几乎不受剩余空间影响，非常适合用在训练Deepfake中。

当然，除了软硬件条件，你还得准备相应的视频素材。

为了完成开头讲到的徐锦江新剧，@慢三与偏见 说：“找了好多素材终于完成了可累死我啦”。

最后，推荐一下我们之前的报道：
- [朱茵变杨幂，流量一个亿丨AI变脸指南](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247515331%26idx%3D1%26sn%3D69a21c76bb01eaf0a0249d1ca494660f%26chksm%3De8d00bb1dfa782a7e500ec870ae57f763e9111b1da523c680da6f5093d01c9b56cac3f8e5671%26scene%3D21%23wechat_redirect)
- [不可描述，技术进步](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493660%26idx%3D1%26sn%3Dadfebdd9ab976ee19b17f29ff383dac4%26scene%3D21%23wechat_redirect)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


