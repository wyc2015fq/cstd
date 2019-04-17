# 谷歌的AI能为黑白视频上色，没有人告诉它哪里是汽车，哪里是人脸 | 附论文 - 知乎
# 



> 五色栗 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-a558767c311076cedce242c7d92cff64_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='250' height='253'></svg>)![](https://pic2.zhimg.com/v2-6aba3a2f747d53bdc46af70fcf2948dd_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='250' height='253'></svg>)△ 输入黑白，生成色彩
谷歌培育了一只AI，是一只自动给视频**上色**的AI。

从彩色视频里，截取某一帧作参考。再把所有帧都变成黑白，AI仅仅依靠**参考帧**的颜色，便可以还原整段视频的色彩了。

把影像从**灰暗**变到**新鲜**，只用了下面这一张图。
![](https://pic4.zhimg.com/v2-0a824aed4574dc83631b13a467ca452f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='250' height='253'></svg>)
 并且，没有人告诉AI，哪里是人，哪里是柜子。

## **走到哪，都认识你**

这是一个**自我监督** (Self-Supervised) 的物体追踪模型。
![](https://pic1.zhimg.com/v2-91dac7358a939459aaa6e70c37173540_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='988' height='350'></svg>)
 所以，研究人员喂给神经网络的是，大量的**无标记**视频数据。

模型要学会**预测**，黑白视频中，物体的颜色。

最关键的步骤，就是**物体跟踪**。
![](https://pic3.zhimg.com/v2-fb7520c3a9293d74584b7ea8bc1e12a2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='374' height='164'></svg>)
 比如，AI看到了一辆汽车，没有人告诉它那是汽车。

但在黑白的图像里，它还是要准确找到这辆车所在的**区域**，把参考帧里相应区域的**红色**填过去。
![](https://pic4.zhimg.com/v2-0a300ed5fefb18ef4aa2856e8b046037_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='524' height='487'></svg>)
 虽然，模型没有用**参考标准**标记 (Ground Truth Labels) 训练过。但从结果上看，神经网络依然学成了人类喜闻乐见的样子。

面包上的奶油，即便涂好的样子不同了，AI也依然记得。
![](https://pic1.zhimg.com/v2-10233baf0461dfedf5780d8abfe6e130_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='522' height='487'></svg>)
## **姿势也可以追踪**

另外，研究人员发现，这个模型还可以用来追踪人类的姿势。

对于主角是人类的视频，神经网络只有了解他们的**一举一动**，才能更顺畅地为人类上色。

毕竟，他们动作比较多，还喜欢给自己加戏。你看——

你让我坐下？
![](https://pic4.zhimg.com/v2-2b91d6bcc911c9fc784f10081d6ac47f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='256' height='256'></svg>)
 我就不坐下。
![](https://pic3.zhimg.com/v2-aeca8fb9b5296b70646ef51f0b6a0d7a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='256' height='256'></svg>)
 我很厉害吧。
![](https://pic2.zhimg.com/v2-981e52c7aa2cc2b99634a7a077934c09_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='256' height='256'></svg>)
 再不鼓掌我…
![](https://pic2.zhimg.com/v2-3d3a1dfcb7234a19201fd6e76117d749_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='256' height='256'></svg>)
只要输入一帧带有骨架的图。
![](https://pic4.zhimg.com/v2-ab5e26c14d08e5a5825cd383aaa1fe63_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='250' height='251'></svg>)
AI就可以推断出，视频里**其他帧**的姿势了。
![](https://pic2.zhimg.com/v2-a906055d0079f7f63c4f0d3ae309a081_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='753' height='280'></svg>)
那么，追踪功夫姿势的话，一定会更燃吧。

然而，并没有，可能只会更佛系，因为——
![](https://pic4.zhimg.com/v2-4c0d6b8daf9845af19591585fac230b3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='421' height='203'></svg>)△ 你的黄色，不，是你的黄色
这是太极的上色效果。

注意，二者清晰度不同，不是AI的问题，是微信动图只支持2MB以内，的后果。

论文传送门：
[https://arxiv.org/pdf/1806.09594.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1806.09594.pdf)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


