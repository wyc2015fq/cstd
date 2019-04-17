# MIT机器人闭上眼睛，靠触觉也爬得上凶险的楼梯 | 施工现场 - 知乎
# 



> 圆栗子 发自 凹非寺 
量子位 出品 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-c4d787821475a50743029eb788cdf403_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
麻省理工的Cheetah (猎豹) 机器人，如今已经进化到了第三代。

比起前辈们，Cheetah 3最耀眼的升级，可能就是**不太需要摄像头**了。

不管是走在凹凸不平的地面，还是爬上布满**施工残余**的楼梯，它都可以悠然地**闭着眼睛**。

MIT团队希望，机器人对**视觉**的依赖，越小越好。

## **闭眼，走路不怕滑**

Cheetah 3是一只四足机器人，体重大约40公斤。

虽然看不见，但它一样可以轻松处理障碍物。
![](https://pic2.zhimg.com/v2-36a262887bc287cc99f6e21b0f34a739_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
 如果，我要走上这样的楼梯，定会战战兢兢如履薄冰，并祝福造楼梯者的全家。

不过，Cheetah 3看上去毫无踌躇之意，在一秒一变的环境里，淡定地保持了平衡。
![](https://pic3.zhimg.com/v2-55f9be401b869af6001d6dc4d7f432de_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
这样一来，蹚过**一地碎石**属于送分题了。

毕竟，这只猎豹的平衡能力，也算接受过炼狱考验了。
![](https://pic2.zhimg.com/v2-64fdc09aee6bdb68225cbf7c0ccf5ca9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
强行拖拽，很稳。
![](https://pic1.zhimg.com/v2-bd0ca467d46c29523d9366741aee2d4c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
捅它一下，很稳。
![](https://pic3.zhimg.com/v2-c7fd60068970e9aea461758d519276b6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
三腿走路，看上去有些“**拖着一条伤腿**”的沉重，不过依然很稳。

就因为这样，它才不怕睁不开眼吧。

## **为何不装摄像头？**

不过，为什么不让它睁眼？
![](https://pic4.zhimg.com/v2-4191fb0dd4a1e339b161328a44dfe207_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='304' height='230'></svg>)
猎豹的爸爸，Sangbae Kim副教授说，真实世界里，机器人有许多**意外**需要处理。

这样的话，它们就不能过多地依赖**视觉**。

因为，**视觉数据**有时候很嘈杂，有时候不够准确。
![](https://pic4.zhimg.com/v2-692127df7b3ebad8a55dbe1ef30fa21f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='298' height='203'></svg>)
如果太依靠视觉，机器人就要获取很精确的**位置数据**，那样它就**跑不快**了。

那么，不靠眼睛靠什么呢？

研究人员希望，机器人可以更好地利用**触觉**信息。
![](https://pic2.zhimg.com/v2-0a70554b3d4f7b01ad39617fa13106c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1392' height='894'></svg>)△ 来自波士顿动力
这样，即便**飞奔**到模糊，机器人还是可以应付突然出现的障碍物，**迅速**恢复平衡。

## **平衡的力道来自算法**

机器人能闭着眼穿过复杂的地形，Kim团队开发的两个**算法**很坚挺——

一个是**接触检测算法** (Contact Detection Algorithm) ，可以让机器人把握准确的时机，从**摆腿状态**切换到**着陆状态**。
![](https://pic2.zhimg.com/v2-c24c2015cb1d9c2e4b4e2f31ec90e7f9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='601' height='400'></svg>)△ 嘎嘣脆，鸡肉味
 这很重要，因为碰到脆脆的**树枝**，和碰到坚硬的**石头**，反应应该是**不一样**的。

**每条腿**的每个动作，都会决定机器人，能不能保持平衡。

不过，接触检测算法也只会告诉机器人，该落地了。 
![](https://pic3.zhimg.com/v2-d40fa5cbfe7b37f06715b41b55e5a69e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='164'></svg>)
落地之后，就交给**模型预测控制算法** (Model-Predictive Control Algorithm) 了。

算法会决定某条腿应该用**什么样的力**，来走下一步。

这里，就要预测，这个力用过之后的**半秒**内，机器人的身体和腿会在什么**位置**。
![](https://pic1.zhimg.com/v2-bd0ca467d46c29523d9366741aee2d4c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)
还是那个栗子，机器人突然从右边被人捅了一下，而脚已经着地了。

这时候，就需要一个往左的力，假设这个力是100牛，算法就要判断机器人半秒之后的命运了。

计算频率要跟上，每条腿**每50毫秒**计算一次，甚至**每20毫秒**计算一次。
![](https://pic2.zhimg.com/v2-826022ff0aa780a2076aec4eeb803d3d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='408' height='230'></svg>)
 两个算法搭配食用，闭眼机器人便能在**高速**的同时注意**安全**。

## **天黑请闭眼**

天黑，还闭眼？

在下不是机器人，这样走不了路。
![](https://pic3.zhimg.com/v2-9081547d13f12997a6f5fe5cfe39c6c6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
狼人，也要睁眼的。

—**完**—

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


