# 这梦一般的街景，全是AI伪造的（把GAN秒成渣渣）｜Paper+Code - 知乎
# 



> 李杉 李林 编译整理
量子位 报道 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-eb70a84b1532e9acb7e4783f4575700e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='427'></svg>)
**△** 『凡所有相，皆是虚妄』

上面这张德国街道图片，乍一看像是行车记录仪拍的，又好像谷歌街景照片加了复古滤镜。

实际上，这是一张合成图片。

在谷歌地图上，根本找不到这样的街道。一个神经网络，根据自己在训练过程中见过的真实街道，生成了它。
![](https://pic4.zhimg.com/v2-335b1407754cc7eb330a0d8aa82bda2b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
## 比GAN更强

生成这张照片的算法出自英特尔实验室的新论文**Photographic Image Synthesis with Cascaded Refinement Networks**，用级联优化网络生成照片。

在论文中，斯坦福大学博士陈启峰和英特尔实验室视觉组主管Vladlen Koltun展示了一种根据语义布局来合成照片的方法。所谓语义布局，就是像这样，在图片上标出各种物体的位置：
![](https://pic3.zhimg.com/v2-92c758f852305232c3b80bcb04a104f6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='325'></svg>)
而他们的算法可以看作一个渲染引擎，输入上面的图片，就相当于告诉它图片中间是道路、路上有车、上半部分有树、有交通灯、右上有一些行人，然后按照图上的布局，输出一张逼真的照片。

陈启峰在自己的博客中说，“好比机器想象出来的画面。”

经过3000张德国街道照片的训练，遇到输入的布局上标注成“car”的部分，这个见过很多车的算法就会自己生成一辆车，填在那里。

现在一提到生成、或者合成图像，可能大部分人都会立刻想到对抗生成网络（GAN），不过，陈启峰等的算法并没有用到对抗式训练，而只用了一个端到端训练过的前馈网络。

和各种往往只能生成小尺寸图片的GAN不同，他们提出的方法可以“无缝扩展”到高分辨率，1024×2014像素的图像也能搞定。

当然，他们在论文里也展示了自己的算法如何将各种基于GAN的生成模型秒成渣渣：
![](https://pic4.zhimg.com/v2-fbcfc6a90b17fb164cb559edd552b2ef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='275'></svg>)


![](https://pic2.zhimg.com/v2-99d075b133e87131adb46fd47181b9fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='245'></svg>)
这篇论文入选了今年10月在威尼斯举办的国际计算机视觉大会ICCV 2017，要进行口头报告，相应的源代码在GitHub上已经收获了402个星，月初还登顶了GitHub热门项目Python排行榜。
![](https://pic2.zhimg.com/v2-85e23cfa4bb6bbc0a2781d4af01cae61_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='349'></svg>)
## 现实，梦境

康奈尔大学计算机系副教授Noah Snavely深受打动，他说，创建逼真的人工场景非常困难，即便是当今最好的方法也无法做到，而陈奇峰的系统生成出了他见过的最大、最详细的人工场景。

他认为，这项技术可以让人们描述一个世界，然后让人工智能在虚拟现实中将其打造出来。“如果能够仅凭大声描述就能像魔法一样召唤出逼真的场景，那就太好了。”他说。

陈启峰认为，这种技术前途大好，最终可以用于创造真正模拟现实世界的游戏场景。他说：“使用深度学习渲染视频游戏可能成为未来的趋势。”现在，他已经开始尝试利用这套算法来替换《侠盗猎车手5》里的游戏场景。

不过，他对这个系统还有一点不满：合成的画面不够逼真。陈启·峰表示，因为这套神经网络渲染细节的能力还无法达到我们的预期，所以目前生成的图片比较模糊，有点像梦境。他正在开发一个更大的版本，希望能够提升系统功能。

在这一点上，Snavely比作者还乐观。他说，具体到构建虚拟现实场景，这种梦境般的效果未必是坏事，我们也不见得需要完全写实的效果。

他更期待的，是让这个系统能生成道路和室内布局之外的场景。Snavely说，要真正实现该系统的潜力，还是需要一套能够真正捕捉现实世界多样性的数据集。

但说起来容易做起来难，想用这个系统生成真是世界中的各种场景，需要借助大量的人力，对训练中使用的图片进行详细标记。

## 关于陈启峰

这篇论文的作者陈启峰，经历也颇为传奇。陈启峰生于1989年6月，成长于广东省中山市。小学时，陈启峰先后获得全国作文竞赛二等奖和奥数竞赛一等奖。

2006年，17岁的陈启峰发表论文，提出数据结构Size Balanced Tree（SBT）。2007年，陈启峰在IOI上获得金牌（第八名）；2008年，陈启峰以中学生身份获得中山“十大杰出青年”称号。

也是在2008年，陈启峰放弃保送清华的资格，进入香港科技大学就读，并获得54万港币全额奖学金。期间全部22门计算机课程都获得A+。2010年，陈启峰获得百度之星编程大赛第三名。

2011年，陈启峰在ACM国际大学生程序设计竞赛获得金牌（全球第二，北美冠军）。同年还获得香港大学生编程比赛冠军。

2012年，斯坦福、哈佛、麻省理工、普林斯顿、加州大学伯克利分校、哥伦比亚、康奈尔、加州大学洛杉矶分校、密歇根大学安娜堡分校九所高校，同时以全额奖学金录取陈启峰攻读研究生学位，最终他入读斯坦福。

最后，放一张陈启峰6月的博士毕业照。
![](https://pic2.zhimg.com/v2-88b7e3535e0f4f436b27cb627f7e3771_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='518'></svg>)
## 相关链接

项目主页：

[http://cqf.io/ImageSynthesis](https://link.zhihu.com/?target=http%3A//cqf.io/ImageSynthesis)

论文：

[http://web.stanford.edu/~cqf/papers/Photographic_Image_Synthesis_ICCV2017.pdf](https://link.zhihu.com/?target=http%3A//web.stanford.edu/~cqf/papers/Photographic_Image_Synthesis_ICCV2017.pdf)

代码：

[https://github.com/CQFIO/PhotographicImageSynthesis](https://link.zhihu.com/?target=https%3A//github.com/CQFIO/PhotographicImageSynthesis)

—— **完** ——

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)

վ'ᴗ' ի 追踪AI技术和产品新动态


