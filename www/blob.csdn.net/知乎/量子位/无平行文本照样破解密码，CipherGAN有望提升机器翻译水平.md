# 无平行文本照样破解密码，CipherGAN有望提升机器翻译水平 - 知乎
# 



> Root 编译整理
量子位 出品 | 公众号 QbitAI

有一位97年的少年，本科期间就进了微软、谷歌大脑实习。

不仅如此，他还参与了去年刷屏级论文Attention is All You Need的和One Model to Learn Them All的研究工作。

他，就是Aidan N. Gomez。
![](https://pic4.zhimg.com/v2-c3ea9e6097b30aaa2847a2d6f3807a97_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='460' height='460'></svg>)
2月3号，Gomez作为一作和他在谷歌大脑的导师Kaiser往2018ICLR投的论文*Unsupervised Cipher Cracking Using Discrete GANs*被正式接收。

这篇论文介绍了非监督破译密码的模型CipherGAN，经非配对的明文密文训练后，可以高保真地破译凯撒移位密码或维吉尼亚密码。

## **加密 pk 解密**

简单介绍一下移位密码和维吉尼亚密码。

最早的加密方法，是公元前58年，凯撒出于军事需要发明的移位密码。

通过把明文的字母往右移三位，可得到外人看了不知所言的密文。
![](https://pic1.zhimg.com/v2-4d72bed533aa1e795edaafa5acf5afd4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='166'></svg>)
但这个加密方法保密性一般，只要破译的人用穷举法，就可以知道偏移量了。

好在，那时候的人没什么文化，移位密码在军事上用了好几个世纪。

直到9世纪博学家AlKindi引入了字频统计破译法，即分析字母出现的频率，移位密码才终于被拉下神坛。

字频统计，就是分析字母出现的频率。

比如说在英语里面，最常出现的字母是e(12.7%)，t(9.1%)，a(8.2%)，而q，x，z每一个频率都小于1%。还有同字母的双字母组，ss，ee，oo，都属于英文单词里常见的组合。

抓住语言结构特点，密码就迎刃而解。

移位密码被破之后，为了对抗字频统计破译法，维吉尼亚密码，最著名的多字母替换式密码应势而生。

维吉尼亚密码是有个密表的，相当于26个不同偏移量的凯撒密码叠在一起，以及以一个关键词不断重复的密钥。

只有拿到密钥的人对应着密表才能解出密文来。
![](https://pic2.zhimg.com/v2-f24438d1bb50737e7249d7644753c8b5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='450' height='450'></svg>)
比方说，明文是Holy high，密钥是wow。

那么对应着上面的密表，去w行找到h的密文是D，o行o的密文是C，以此类推，holy high的密文就是DCHU VECV。

## **破解密码的表现**

以往破解密码的模型，在密文长度有限的情况下，还是可以破解出密钥很短的密文来的，正确率不错。

但如果密钥的关键词很长的话，正确率就很低了。

而该论文的工作，训练数据是非配对的明文和密文，也**无需预置的人类语言学知识，就可以破译长词级的密钥。**如下表所示⤵️
![](https://pic1.zhimg.com/v2-dfc170546f11b7a3e3d501a6841335d0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='316'></svg>)
表中左边三列是CipherGAN的表现，表右边两列是以往模型的表现。

可以看出来，在解密三种词量长度（10，200，58）的密文时，CipherGAN解密移位密码的表现近乎完美（100%，98.7%，99.8%）。

**当密文有200个单词时：**

1）CipherGAN破解移位加密的正确率可达到98.7%，是以往模型的两倍（44.5%）；

2）维吉尼亚加密的密文以往模型的正确率完全不行，给了密钥才勉强能达到44.3%，而CipherGAN的正确率可达到75.7%。可以看出来CipherGAN比较擅长破译长词级密钥的密文。

该模型是基于图像风格转换模型CycleGAN（加我们自己的链接）作的修改，破译表现远高于以往频率分析的模型。

针对CipherGAN可以使用非平行文本作输入的特点，Gomez在接受Newsweek外媒采访的时候，也提到了，“**密码破译的模型思路也能迁移到非监督学习的翻译上。**”

因为语言翻译常面临的难题是，缺乏足够的平行语料。

正好和非配对明文密文的密码破译过程很相似。

Gomez希望Facebook和Google这种量级的巨头公司可以继续这个课题，把CipherGAN用在提高机器翻译水平上。

“我们提出的算法，对任何两种文本信息都是通用的。不一定非要纯文本或者暗号，也可以是英语和法语。”Gomez说。

## **OMT**

破解密码模型工作的，是整个**for.ai**研究团队。一共5个小伙子，都是多伦多大学计算机系的学生。
![](https://pic3.zhimg.com/v2-19d8d848be0e3b2b7085ba6acc5f17ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='720'></svg>)
Gomez文章开头介绍过就不多说了。

左一是还在念大三的Sheldon Huang，创办了多伦多大学机器智能学生社团UTMIST，是UTMIST主席。

左二是已经休学的Ivan Zhang，曾在Ranomics工作了两年，加入For.ai将近1年。

右二是Muhammad Osama，今年Gomez一起拿到了荣誉学士学位，于18年6月毕业。

最右边的是香港小伙子Bryan Li，曾在华为香港和AMD实习过。

除了专研AI课题，这几个小伙子还在自己的简历里谈到自己有不少兴趣爱好，有喜欢画画的，有喜欢摄影的，还有喜欢玩乐器的。

量子位前往个人的Ins上推上看了看，样样出手都鹅妹子嘤。大家感受一下⤵️
![](https://pic3.zhimg.com/v2-56b3fc194a5c622bccf4d9b7bf7fb6fe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='599' height='386'></svg>)
**△** The darker the night, the brighter the stars

by Bryan Li

最后，附论文地址，

[https://arxiv.org/abs/1801.04883](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1801.04883)

及高质的同行评议，
[https://openreview.net/forum?id=BkeqO7x0-](https://link.zhihu.com/?target=https%3A//openreview.net/forum%3Fid%3DBkeqO7x0-)

及开源代码的地址，
[https://github.com/for-ai/CipherGAN](https://link.zhihu.com/?target=https%3A//github.com/for-ai/CipherGAN)

[https://For.ai](https://link.zhihu.com/?target=https%3A//For.ai)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


