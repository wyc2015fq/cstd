# 谁说导航一定要用地图？谷歌DeepMind的强化学习模型靠街景认路 - 知乎
# 



> Root 编译自 DeepMind
量子位 出品 | 公众号 QbitAI

还记得小时候从家到学校的那段路怎么走吗？

那会儿你可能还不知道什么是地图，也没有导航软件。但那条路的画面都在脑子里刻着。茂密的竹林，很多蛇出没的小山丘，还有泥鳅抓不完的池塘。
![](https://pic1.zhimg.com/v2-0692c27c2e8ae6563c290cba35ddb3f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
这就是我们从小对空间世界感知的途径，通过**路的特征画面**构建地理认知。


等到慢慢长大了，需要去更遥远的地方，路上可参考的熟悉的地标越来少，尤其是新的城市时，才开始用起了地图。

DeepMind团队希望，造出一个**深度强化学习模型**，用符合我们直觉的空间认知方式——串联所有街景画面，重塑出行导航方式。
![](https://pic3.zhimg.com/v2-a2e2d7af493e45c6c18c58e7c7390bd2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='180'></svg>)
用户只要给定起点终点的地理坐标，DeepMind就能**结合谷歌地图里真实采集的街景数据，训练出能根据地标画面信息规划出行路线的街景导航模型**。

出于保护隐私的考虑，街景中所有的人脸和车牌都会作打码处理。

比起现有的地图导航，街景导航的最大优点是非常适合步行，这是因为步行对地理信息的精度要求更高。

像我们平时去一个地方，大方向上靠高德或百度地图导航过去没问题，但最后一公里总是会被带到各种沟里去：不是此路不通，就是绕了一大圈。

## **街景导航的工作原理**

传统的地图绘制，是让人绘出路段的二维俯视图，定位只能参考GPS信息。而街景的信息含量更大，更符合我们人置身环境所看到的视觉画面。

基于这点，DeepMind构建了一个神经网络，把伦敦、巴黎、纽约这些城市的谷歌街景数据，这些依托真实环境的信息作为输入源，然后让模型自行预测每一个可能的前行方向。采样颗粒度可以达到每一个小道，十字路口，隧道，以及复杂的岔路。

当街景导航模型能顺利规划出前往目的地的路径时，DeepMind会给予一定的奖励。然后经过训练之后，这个街景导航模型能够逐渐熟悉整个城市的所有路段。

## **可迁移的模块化的神经网络架构**

DeepMind所造的这个街景导航模型包含三个部分：
- 视觉处理网络：一个可以处理图像和提取视觉特征的卷积神经网络；
- 视觉信息网络：一个专门记忆特定城市街景的循环神经网络，可以快速锁定代表起点终点的实景环境；
- 路线规划网络：与特定城市无关的循环神经网络，专门负责制定出行路线。

其中，第二个视觉信息循环神经网络是可以替换的，不同的城市可以用不同的网络。而视觉处理网络和路线规划网络可适用于所有城市。
![](https://pic1.zhimg.com/v2-4aa894ad9e352bb281b781cc85c7c9f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='877' height='331'></svg>)
和城市导航CityNav模型(a)相比，多城市导航MultiCityNav模型是需要有一个针对特定城市路段信息的视觉信息模块的，如图(b)所示。多城市导航MultiCityNav架构的训练过程可以从(c)看出，模型是怎么适配其他城市信息以及迁移相应的路线规划能力。
![](https://pic4.zhimg.com/v2-622809b82d5676673ec7f3a6110c8b57_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='958' height='546'></svg>)
和谷歌街景交互界面相似，街景导航模型可以旋转输入信息的观察视角，或选择跳到下一个全景镜头。

不同的地方在于，街景模型**看不到指示方向的箭头**，它需要自己根据画面中的路径延伸方向判断出前行的几种可能。

如果说目的地是在几公里以外的地方，那么街景模型要自己把所有的全景镜头采下来的画面信息里的可能走法跑一遍，才能得出结论。

现在DeepMind已经证明了这种多城市街景模型可学习其他新城市的可行性。

作为人类来说，我们不希望这个AI拿到新城市的数据时又要再学一遍怎么处理街景信息，比如放大某个可能的岔路口或者在十字路口转弯等行为。不过花时间在提取新的地表特征上是可以接受的。

这种方法，获得新知识的同时也还记得之前所学过的东西，和DeepMind之前**连续学习网络Progressive neural network**非常相似。

也许等到这个算法上线应用的时候，我们再也不会遇到跟着地图导航走却根本到不了地儿的囧事了。

最后，附论文地址：

Learning to Navigate in Cities Without a Map
[https://arxiv.org/abs/1804.00168](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1804.00168)

**你可能感兴趣**

[马克龙宣布15亿欧元投资AI，DeepMind拥吻巴黎](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247496285%26idx%3D2%26sn%3Deea41cc0a54323e4dce94270125e8e9c%26chksm%3De8d0452fdfa7cc39f23782e1f11d7d7af73b5c34516362dc69bbfc8e71df35d22ddceeba1479%26scene%3D21%23wechat_redirect)

[DeepMind黄士杰：深度学习有创造性，正参与星际2项目](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495989%26idx%3D3%26sn%3D5a44528398456082b34c3fe1223b7f89%26chksm%3De8d04647dfa7cf517fe591f9b9de685cdafba8c6076696c0c6173b0b025e56548f70e9986c48%26scene%3D21%23wechat_redirect)

[“寓教于乐”，DeepMind新研究让机器人从0开始学习复杂精细动作](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247495030%26idx%3D2%26sn%3Da32f9ae8de8d24a2b046b1eb68e1588d%26chksm%3De8d05a04dfa7d312da0246f664cb00c5fd397ec8bb8c01f78ea07bf88596bf30846d14ee93b7%26scene%3D21%23wechat_redirect)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


