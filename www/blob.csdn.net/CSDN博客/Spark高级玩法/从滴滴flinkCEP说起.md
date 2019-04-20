# 从滴滴flinkCEP说起 - Spark高级玩法 - CSDN博客
2018年08月29日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1207
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
本文留言，邀请好友点赞，点赞数超过20有奖励，具体奖励内容，可以细读本文。
ppt全文可关注公众号，后台输入 **flink **获取。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)**CEP业务场景**
复杂事件处理（Complex Event Process，简称CEP）用来检测无尽数据流中的复杂模式，拥有从不同的数据行中辨识查找模式的能力。模式匹配是复杂事件处理的一个强大援助。例子包括受一系列事件驱动的各种业务流程，例如在安全应用中侦测异常行为；在金融应用中查找价格、交易量和其他行为的模式。其他常见的用途如欺诈检测应用和传感器数据的分析等。
说了这么多可能还是觉得比较抽象，那么我们可以看看这次滴滴分享的FlinkCEP在滴滴中的业务场景。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngIt2kLwvyc9jKEYuVMY6zDBtG5T0S1K4g24N8R2e8ys93jQGcJNtyOQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngIJx7dbrsWdn4OQDogp3XZFgX58hllRL0OzRqZEaRb3n1FzJicsk2Etw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngS4fUEKav2MN9ZbwyrJHOLvKoqnXlwicz4s2HuPtc9o6pR3tvZxemhBA/640?wx_fmt=png)
吐槽时刻：
虽然，业务场景ppt写的很好，但是最近几次顺风车事故，给大家留下了糟糕的印象。大数据没用起来，cep其实应该也可以用在顺风车安全检测上吧。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)Flink CEP
Flink的CEP是基于Flink Runtime构建的实时数据规则引擎，擅长解决跨事件的匹配问题。
可以看看，滴滴的屁屁踢上给出的两个demo
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngobTnvkgfQdjQ4lwe4oLLibAwq2nibjMGpU0vVacA9fYhto4mPoYKFqcQ/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngY0Pd8AicwTJemC6AicqyWoomGtXvft8K1NbvQWRBxicYicmpxFkrY66cKA/640?wx_fmt=png)
**Flink CEP的特点**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngSjJ4ClE8jialtPOYE3ayibsPgTQia6uWb3rNPMFrKb1ib4pa1mo7fxxQZw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngv4gkSKicB02GA2SYWBPoo3Ept1SHZNviclP7iae9ev1KYMAicCQAqe0QcA/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)动态规则
其实，对于实时领域的规则引擎，我们不想每次修改都要打包编码，只希望简单修改一下规则就让它能执行。
当然，最好规则是sql 的形式，运营人员直接参与规则编写而不是频繁提需求，很麻烦。。。。此处，省略万字。。
要知道flink CEP官网给出的API也还是很丰富的，虽然滴滴这比也给出了他们完善的内容。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngVHFxLHqfyeJic4sOxVZCGjNNQQV9cYkLJ90gVWGW7hh4pSN8Cb8z5Ng/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngXIueeHicFQkTIA6fWUgbjMCKibjchfHkQFQcSv4tv5BM26ObXz5Ke53g/640?wx_fmt=png)
flink官方的CEP文章，浪尖及浪尖组织的flink小团队，已经翻译过了。链接如下：
https://github.com/crestofwave1/oneFlink/blob/master/doc/CEP/FlinkCEPOfficeWeb.md
那么，为了实现动态规则编写，滴滴的架构如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngoA1xAWRgW4kCw1qGRCdCL6S1C7WXw2AicGZWUQDqYiaicgGKZQZ9yD6cQ/640?wx_fmt=png)
具体的规则实现如下：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngRAMgPeGpICZorcaTFBh8CyL6eu47x6icMGN7LsjwESHGnMnXDy1tuhQ/640?wx_fmt=png)
可以看到，其规则还是要编码成java代码，然后再用groovy动态编译解析，不知道效率如何。。。
对于规则引擎，当然很多人想到的是drools，这个跟flink结合也很简单，但是效率不怎么苟同。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)Flink CEP的SQL实现
熟悉flink的小伙伴肯定都知道Flink的SQL引擎是基于Calcite来实现的。那么细心的小伙伴，在calcite官网可以发现，calcite有个关键字MATCH_RECOGNIZE。可以在这个网页搜索，找到MATCH_RECOGNIZE关键字使用。
http://calcite.apache.org/docs/reference.html
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sng0lpicNXnl7a6ibicWgHWss3WTAQU7ia1iakRTvGYeK6baaYlIbPqHRdX8Aw/640?wx_fmt=png)
那么这时候可能会兴冲冲写个demo。
```
final String sql = "select frequency,word,timestamp1 "
     + "  from wc match_recognize "
     + "  ("
     + "       order by  timestamp1 "
     + "       measures A.timestamp1 as timestamp1  ,"
     + "       A.word as  word ,"
     + "       A.frequency as  frequency "
     + "       ONE ROW PER MATCH "
     + "    pattern (A B) "
     + "    within interval '5' second "
     + "    define "
     + "      A AS A.word = 'bob' , "
     + "      B AS B.word = 'kaka' "
     + "  ) mr";
```
很扫兴的它报错了：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngKRpJQKBwOPzG0UTYZW1YOWftzLK1EG79VibCEA1D1qvzMlOEXdtr7jg/640?wx_fmt=png)
那么问题来了，calcite支持而flink不支持，为啥？
***赶紧发了个issue，然后迅速得到官方回复：***
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngdI88KM0RsQMys0JowuSbWtVAY1sCHFenwtjpj2g0okIKNygRUaRicRw/640?wx_fmt=png)
但是，翻翻**阿里****的blink使用手册和华为的flink使用手****册**发现两者都支持。
好吧。其实，很不服气，周末，除了**健身**就是**加班**。
波折一番，解决了，需要修改flink-table相关的内容，执行计划，coden等。
最终，实现了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngEiawGYtgxhNnAsFTWCZL2frIqtPBAuPPBfEzCEQGtU97ZvYpI2YJ9icg/640?wx_fmt=png)
欢迎留言，留言被点赞**超过20**个，即可获得该运行通过完整代码。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFXlBzgRoWvuicJgvDgwu9sngdZe2FDPg2ogedicYfUMleTLaF4fbcLPgBq2b0ngQdyzIwTOOT8jAUyg/640?wx_fmt=jpeg)
欢迎关注浪尖微信公众号，学习掌握大数据生态技术。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
更多优质内容，欢迎扫描二维码加入浪尖知识星球，各类教程，浪尖原创源码视频，优质技术短文应有尽有。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/z9433rAGTDdpO0B0FqCuTT6IVUeJeupR8DrCPat3Iqu9rfDbpxOwwsZrzLecngibudlf85XCJnDHVMtk9Xw9pzw/640?wx_fmt=gif) 有意义留言，然后邀请好友点赞，超过20，即可获得前面所述源码！
