# APP有趣的Loading设计 - weixin_33985507的博客 - CSDN博客
2015年09月07日 15:37:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：13
Loading在APP中非常常见，app的loading也是五花八门，各种loading，各种特色。然而回归需求本质，loading之所以存在，是因为APP获取或提交数据的时间较长，通过loading反馈用户，让用户等待，通俗的说就是APP在数据处理过程中，给用户反馈，避免用户烦躁，打发用户无聊的时间。
OK，明白了Loading的需求本质，APP 的loading设计就简单了，loading需要做两件事：一、告诉用户APP在处理数据，二、要避免用户因等待而烦躁。因此为了留住用户继续使用APP，loading的设计就可以情趣化或者呆萌可爱起来，这里介绍一些比较有趣的loading。这些加载动画让等待的过程变成了一种享受，用户能感受到产品的情怀，体会新鲜有趣的等待过程。提升了产品情趣化的设计语言，让等待的焦躁感一扫而空。
Google加载动画
[](https://link.jianshu.com?t=http://elya.cc/wp-content/uploads/2014/03/14.gif)
![815442-ab6dd3ea902022b6.gif](https://upload-images.jianshu.io/upload_images/815442-ab6dd3ea902022b6.gif)
Digg的加载动画，嵌入自家logo，强化产品品牌
[](https://link.jianshu.com?t=http://elya.cc/wp-content/uploads/2014/03/15.gif)
![815442-668ae4eaf45aab9b.gif](https://upload-images.jianshu.io/upload_images/815442-668ae4eaf45aab9b.gif)
其他加载动画欣赏
[](https://link.jianshu.com?t=http://elya.cc/wp-content/uploads/2014/03/16.gif)
![815442-8b205877158f2b0d.gif](https://upload-images.jianshu.io/upload_images/815442-8b205877158f2b0d.gif)

![815442-6f23db9af75006ff.gif](https://upload-images.jianshu.io/upload_images/815442-6f23db9af75006ff.gif)

![815442-2b3797edda51349f.gif](https://upload-images.jianshu.io/upload_images/815442-2b3797edda51349f.gif)

![815442-b0b53e2a86279228.gif](https://upload-images.jianshu.io/upload_images/815442-b0b53e2a86279228.gif)

![815442-8ea5b40e988f4f0f.gif](https://upload-images.jianshu.io/upload_images/815442-8ea5b40e988f4f0f.gif)

![815442-4fc6da69f0dbf5c4.gif](https://upload-images.jianshu.io/upload_images/815442-4fc6da69f0dbf5c4.gif)

![815442-ab6036070e2309be.gif](https://upload-images.jianshu.io/upload_images/815442-ab6036070e2309be.gif)

![815442-9ce5a2b52ad82600.gif](https://upload-images.jianshu.io/upload_images/815442-9ce5a2b52ad82600.gif)
需要指出的是，动画太花哨，会给用户造成一定的干扰，loading采用的色彩种类要尽量少，不要为了玄而设计，那就本末倒置。APP每次做数据处理都loading，用户会不会觉得烦呢？网络不好时，用户等待几十秒甚至几分钟都看不到处理结果，甚至导致界面卡顿，这当然不是我们想要的结果。
咱们再回到loading产生的原因，APP需要较长的时间来获取或提交数据，因此需要loading来提示用户，那有无方法让用户无感知中做数据处理呢？答案肯定是有的。
1、**提前加载**
尽可能的利用预加载或有WiFi的情况下离线缓存的方式，把内容提前加载下来，这样能做到最大限度的降低加载给用户带来的卡顿感。如果能判断出来用户下一步要做的事情，提前帮用户加载相应的内容，肯定是最符合需求场景的事情。当我开始读第一页的时候，第二页第三页就开始陆续缓存下来了。
2、**异步提交**
当用户进行发送消息，发布评论、提交意见等操作，APP先提示提交成功，让用户不受提交数据耗时的影响，继续使用APP其他功能，同时APP开启后台线程静默进行数据提交，当前网络环境不好，下次再自动提交，让用户无缝完成数据提交操作。
**3、数据缓存**
当用户没网的时候，往往很多功能都不能用了，内容也无法加载出来，导致APP变得根本不可用，这时候就要考虑预加载+离线缓存的设计了。首先在有网的时候把数据提前加载下来，缓存到本地，当没网的时候，直接加载已经缓存下来的内容。一般会提供给用户选择，是否开启有WiFi的情况下预加载功能，或者是否开始WiFi下全部离线缓存的功能。这样就能保证在地铁上，过个隧道就看不了内容的尴尬了。
#本文同步发布到[pm263.com](https://link.jianshu.com?t=http://pm263.com/)，pm263为你提供更多更全的产品经理干货，欢迎大家访问。
