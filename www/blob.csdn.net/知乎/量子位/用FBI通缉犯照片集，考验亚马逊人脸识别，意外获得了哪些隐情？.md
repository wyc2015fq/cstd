# 用FBI通缉犯照片集，考验亚马逊人脸识别，意外获得了哪些隐情？ - 知乎
# 



> 识别栗 发自 麦蒿寺
量子位 出品 | 公众号 QbitAI

BuzzFeed的人类们，一路喜欢搞事情。
![](https://pic3.zhimg.com/v2-7568cc6697036ee56e542d69d7ac863e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='580' height='400'></svg>)
自从亚马逊的人脸识别系统Rekognition，把28位美国议员认成了罪犯，他们就决定亲自实验一下。

不过，BuzzFeed测试的不是这套系统，而是同属亚马逊的一个**名人识别AI**。

把**FBI通缉犯**的507幅图像拿去给AI做匹配。
![](https://pic4.zhimg.com/v2-b259b49a1e90163a18f10d08da51894f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='627' height='282'></svg>)△ 画家Bob Ross与通缉令上的Mike W. Jackson
结果，吉他手啊，演员啊，画家啊，纷纷与通缉犯**高分**匹配成功。AI一共完成了**17次**置信度 (Confidence) **95%以上**的匹配。

在另外一项用NIST脸部数据集做的测试里，AI还把美国前国务卿赖斯，和一**非裔男子**被捕后的照片，匹配在了一起，置信分是**96%**。
![](https://pic2.zhimg.com/v2-364fc9fba32be3c11e8c6de1509ad4e1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='682' height='424'></svg>)
机智的你可能发现了，重点不在明星和通缉犯的匹配，却在于这些匹配**并非勉勉强强**拉个人凑数，而是AI**很有信心**的答案。

为什么会这样？BuzzFeed也想知道，于是联系了亚马逊。

## **亚马逊：这不是人脸识别**

得到的答复，可能比实验结果，还要让人惊喜。
![](https://pic4.zhimg.com/v2-15850f0582030c43f9d5ecb388d6963b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='224'></svg>)
亚马逊说，别看**名人AI**是作为**Rekognition**的一次**更新**，发布出来的，它们两个其实是完全不同的产品啊。

名人AI是个**娱乐**产品，会在社交网络或者搞笑App这样的地方出现。

所以，它要做的就是给出更多相似的组合。
![](https://pic4.zhimg.com/v2-3882d5ab5b16df76122ff5026ff2f59b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='706'></svg>)
比如，加勒比海盗的杰克船长，画了很浓的妆，跟强尼戴普平时的样子相差不少。

即便这样，**名人AI**还是会给你一个**很高的置信分**，告诉你他们两个是同一个人。

而同样的一组图，喂给**Rekognition**的话，两者匹配的置信分数，可能就只有**50%-60%**了。

毕竟，后者作为严肃的人脸识别AI，设定里就包含了**执法**用途。
![](https://pic4.zhimg.com/v2-2d7998b4d166cbf0978a51b1d776109f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='863'></svg>)
为了强调两者的差异，亚马逊甚至表示，**名人AI**根本不是人脸识别系统，它和Rekognition在模型的结构上，就有明显的不同。

## **未曾写清差别**

可问题是，普通人没有办法感觉到这样的差异。

至少，亚马逊的官方文件和宣传文，都没有体现它们之间的区别。 
![](https://pic4.zhimg.com/v2-15850f0582030c43f9d5ecb388d6963b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='224'></svg>)
复习一次这张图，如果两者结构和用途都完全不同，如此摆在一处真有些匪夷所思。

在跟BuzzFeed聊过之后，亚马逊的人类才修改了Rekognition的文件。修改过的版本，已经可见二者**结构**上的不同了。

亚马逊的一位发言人说，之前的文章写得太傻了，只想着吸引人点进去，没有想到应该让用户看清楚。

就像刚才说的，名人AI针对的是，会在影视作品里伪装自己的演员，把他们的扮相和真实的长相匹配起来。

所以，如果只对着一群通缉犯去匹配，当然匹配不出正确的结果了。
![](https://pic1.zhimg.com/v2-ab18a3596345090e6552c450f7f3200c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='620' height='506'></svg>)
亚马逊还表示，大家**不用在意**名人AI给的置信分，因为真正的人脸识别系统，打分的规则是完全不一样的。

## **特意不写清楚？**

这样，问题又来了。

加州大学伯克利分校的学者Joshua Kroll说，执法用的AI系统，大部分都**不会公开**技术细节。 
![](https://pic2.zhimg.com/v2-67f96c7b5f5f866ec829e6d1f612848d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='600'></svg>)
这样的话，公司说什么，就是什么。

像亚马逊，Rekognition被爆出“把议员认成罪犯”之后，公司才向外界解释说，95%以上是**推荐**的置信度。

可置信分数到底是怎样得出的，代表怎样的含义，亚马逊从来**没有解释**过。

Kroll说，让外界产生那么大的困惑，这是亚马逊**自己选择**的。

如果公司愿意透露更多的细节，比如系统是如何开发，或如何运行的，那么针对亚马逊人脸识别技术的质疑和困惑就会少一些。

## **如何解读置信分？**

可是现在，只是给出了**95%**的推荐置信阈值，执法人员又应该怎样理解这个数字呢？

来自乔治城法律中心的Clare Garvie说，一个人如果看到99%的置信度，那ta大概会认为系统给的结果是99%肯定的。
![](https://pic2.zhimg.com/v2-d95c628c62fec27f76af25ed1ead06fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='428' height='424'></svg>)
可事实并不是这样，那只是AI系统自己的信心而已。

而执法人员会怎么解读这个数字，取决于他们接受的是怎样的培训。

这个解读，又可能影响到案件调查的方向，人类很可能更偏重于那些置信分高的匹配结果。

人们之所以会有各种各样的担心，一部分也是因为，AI可以调查各种各样的人，可对人类又很难对AI的工作原理展开调查。
![](https://pic2.zhimg.com/v2-f0e72a5d450582534a83a2b20f21d099_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='444' height='190'></svg>)
人类在明，AI在暗。

## **无休止的调查？**

除此之外，当人脸识别被用于执法，你一旦被监控拍到，就不知道AI对你的调查，什么时候是个头了。

亚马逊说，存下图像，只是为了维护系统，改进系统。

可是，图像会不会被滥用，谁也不知道。
![](https://pic2.zhimg.com/v2-59d94f20ec546fdf2b24d0793fbd78c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='380' height='267'></svg>)
 — **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


