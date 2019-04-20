# idea以yarn-client 提交任务到yarn - Spark高级玩法 - CSDN博客
2018年09月07日 09:36:48[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：581
鉴于很多小白经常问我如何用idea提交任务到yarn，这样测试的时候不用频繁打包。
昨天，晚上健身回来录了一个小视频，说是小视频但是耗时也比较长，将近40min。可能是健身脱水太多，忘了补充盐分，无力感十足。
构建工程的命令：
```
mvn archetype:generate \
-DarchetypeGroupId=org.scala-tools.archetypes \
-DarchetypeArtifactId=scala-archetype-simple  \
-DremoteRepositories=http://scala-tools.org/repo-releases \
-DgroupId=bigdata.spark.app \
-DartifactId=bigdata \
-Dversion=1.0
```

这样做的目的是，为我接下来分享spark streaming视频做铺垫，材料精细组织，浪尖也不会说废话，都是面试和多年企业中实战经验。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFViaZrvyvwFlVxZHunGoicxSH5q6sy5B67ibqY1SY7H2CBc262Lfictgqwgwsz0sX9mBXcEliaBkT5ubXg/640?wx_fmt=png)
本次，小视频工程，关注公众号，输入**yarn**即可获得。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
