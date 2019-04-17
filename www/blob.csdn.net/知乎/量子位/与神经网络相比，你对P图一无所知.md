# 与神经网络相比，你对P图一无所知 - 知乎
# 



> 栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI 
![](https://pic2.zhimg.com/v2-38cb3678fd38a142e95ae9c759a96839_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='506'></svg>)
“大神，可以帮我把羊蜜街拍P成我的脸么？哦对，不光是这样，还要像羊蜜哟…”

从一张照片里取出某个元素，再把它神不知鬼不觉地混入另一张图片里，似乎是一件很有难度的事情。毕竟，分分钟就会产生魔性的拼贴效果，不管是手动操作，还是AI的计算。

而如果“另一张图片”是指人类的画作，就更加不容易了。

不过，康奈尔大学和啊逗比的大神们生产了一种算法，能把各种物体融合进画作里，且毫无PS痕迹。

大量艺术家的心血，甚至艺术家自己，都惨遭它的毒手：
![](https://pic1.zhimg.com/v2-dea947dfecbdcc660733d2b123a5add0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='448'></svg>)要啥自行车![](https://pic1.zhimg.com/v2-3ed05901b21f3388e01028bbe213cb80_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='533'></svg>)蒙娜憨莎·豆![](https://pic3.zhimg.com/v2-5e44a1f760a2e3379a8f166f1205089e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='455'></svg>)毕加炳 ![](https://pic1.zhimg.com/v2-0b120caac8987e16a77bcfc37c74c050_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='457'></svg>)给你买了个biao![](https://pic3.zhimg.com/v2-b5fa71cb70c9d1f6bd0059e7c52eaf26_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='464'></svg>)樊 · 蔡康永 · 高
是什么算法，如此丧心病狂？

记住这个名字：Deep Painterly Harmonization。

在康奈尔和啊逗比的P图大神们看来，现存的全局风格化算法都太弱了。

它们确实能搞定整张照片的风格迁移。把画作和被画传染的图片摆在一起，可能不会看出太大的问题。
![](https://pic3.zhimg.com/v2-aa9fa3a3820d2447990bc1499a66c7ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1062' height='432'></svg>)介绍一下，这是我的新装备
但要像上图这样，把美队的盾牌融合到意大利画家Onofrio Bramante的作品里，微小的差异看上去就会很明显。全局风格迁移的表现也比较谦虚 (如左三) 。

无论是去除边界线、匹配色彩还是细化质地，都很难让粘贴部分拥有画作的原始风格。

## **我们不一样**

于是，这群大神觉得，他们需要搭建局部风格化的神经网络。

大的方略就是，把画作相关部分 (神经元响应) 的特征统计，迁移到外来物体的对应位置上——关键在于选择，哪些东西才是应该迁移的。
![](https://pic2.zhimg.com/v2-eb15538d6dd436387853815f66117311_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='719'></svg>)Gatys有只几何猫
小伙伴们以Gatys团队发表的风格迁移技术为基础，用VGG搭建了一个two-pass算法，还额外引入了一些风格重建损失 (Reconstruction Losses) 来优化结果。

来来来，算法详情要一步一步看。

## **第一步 (First Pass)：粗略图像协调 (单一尺度)**

大致调整外来元素的色彩和质地，和画中语义相似的部分对应。要在神经网络的每一层，分别找到相邻最近的神经补丁，和粘贴部分的神经元响应匹配起来。
![](https://pic2.zhimg.com/v2-2f69457ebc0d374e887dbedf44d97339_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='645' height='503'></svg>)卷福丽莎，迷人的眼神
退一步，海阔凭鱼跃。先不需要太纠缠图像的质量，因为一定程度上牺牲品质，团队就可以设计出一个强大的算法，适应多种多样的画风。

用Gram矩阵来计算风格重建损失，便可以将粗略协调的版本优化一下。
![](https://pic4.zhimg.com/v2-f64986b296c96bf2352b1e7e88d6892f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='553' height='254'></svg>)不计风格损失的后果
这里得到的是一个中间产物，不过风格已经和原画作很相似了。

## **第二步 (Second Pass)：高品质细化 (多尺度)**

有了第一步的基础，此时开始对图像质量提出严格要求，便是顺理成章。

这一步，先要在一个负责捕捉质地属性的中间层集中火力，生成一个对应关系图 (correspondence map) ，来剔除空间异常值 (spatial outliers)。
![](https://pic2.zhimg.com/v2-16266e31275e550e5a8089958acad109_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='649' height='454'></svg>)你沉在，我昏黄的画布里
然后，对拥有空间一致性的对应图进行上采样 (upsample) ，进入神经网络更加精细的层次里。这样可以保证，对每一个输出位置来说，所有尺度上的神经元响应，都来自画中的同一位置。

如此一来，画面便能拥有更加连贯的质感，看上去自然许多。

## **后处理**

经过Two-Pass算法的洗礼，中大尺度上看，图像质量几乎无懈可击；但在精细尺度上，可能还有不够准确的情况存在。也就是说，可远观而不可__焉 (填空题：10分)。

于是，后处理也要两步走：

## **色度降噪**

高频失真的现象，主要作用在色度通道，对亮度并没有太大影响。

发现了这一特点，团队将图像转换成CIELAB色彩空间；再在Guided Filter里，以亮度通道作为向导，来过滤ab色度通道。
![](https://pic1.zhimg.com/v2-6e918f14fd38cca47894db8f97ebd140_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='655' height='489'></svg>)工作还是生活，这是一个问题
这个方法有效改善了高频色彩失真的情况，但有可能捅出更大的漏洞。然后…

## **补丁合成**

就有了第二步，来确保输出的每个补丁都出现在画面中。用PatchMatch给每一个补丁找到相似的补丁。再为所有画风重叠的补丁取平均值，以此重构output，就可以保证画面里不要生出新的内容。

不过，这里的副作用是会柔化细节，于是又要请出Guided Filter把图像分成底层 (Base Layer) 和细节层 (Detail Layer) ，来削弱柔化效果。
![](https://pic2.zhimg.com/v2-77ab7a5305aa728bb41d20750c035209_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1059' height='397'></svg>)画风虐我千百遍，我，选择狗带
## **局部迁移疗效显著**

实验结果表明，转移一些区域特征统计的集合，要比转移很多独立位置的特征统计，效果要好得多。神经网络反应的对应关系图帮了大忙。

局部风格迁移的算法，保障了统计学意义上的空间一致性和跨尺度一致性。得到天衣无缝的拼图质量，这两个“一致性”功绩斐然。
![](https://pic4.zhimg.com/v2-95eb5282e8b9bc11c3a67a82467f750b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='639'></svg>)金拱门的八度人生
以后，拼图少年们大概可以抛弃全局风格迁移算法了。放眼局部，你的脑洞可以绽放得惨无人道。

不过，比起难以察觉的入侵，我还是醉心于这种360度全死角的魔性p图。
![](https://pic4.zhimg.com/v2-d7a465d4f29f87846fa4eae7cc470097_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='369' height='191'></svg>)万能的拷贝，万能的粘贴
更酸爽的**论文**传送门：

[http://www.cs.cornell.edu/~fujun/files/painting-arxiv18/deeppainting.pdf](https://link.zhihu.com/?target=http%3A//www.cs.cornell.edu/~fujun/files/painting-arxiv18/deeppainting.pdf)

这篇论文的作者包括：Fujun Luan（栾福军）、Sylvain Paris、Eli Shechtman、Kavita Bala等人。他们还把**代码**开源了，还能看到更多触目惊心的例子：

[https://github.com/luanfujun/deep-painterly-harmonization](https://link.zhihu.com/?target=https%3A//github.com/luanfujun/deep-painterly-harmonization)

顺便提一下，论文一作栾福军，2015年本科毕业于清华，目前在康纳尔大学攻读博士学位。曾在Facebook、Face++、Adobe等公司实习。
![](https://pic1.zhimg.com/v2-bfad1ff05ffc84ea0fd0b58b8ca33304_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='393'></svg>)
艺术照小哥哥一直在钻研视觉的玄妙。

比方说曾经被誉为“下一代PS”的[Deep Photo Style Transfer](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247487956%26idx%3D5%26sn%3De4af59c62f69d1298030216f4a1923df%26scene%3D21%23wechat_redirect)，也是栾福军的研究成果。Deep Photo能实现像素级的风格迁移。

也是非常酷炫。
![](https://pic3.zhimg.com/v2-eb68060719ff8d0364746c42e038d216_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='218'></svg>)
## **One More Thing**

如果你暂时不方便去GitHub上围观，我们还搞了一个小型AI高手P图展。在量子位公众号（ID:QbitAI）对话界面，回复“**高手**”二字，即可开始参观。

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


