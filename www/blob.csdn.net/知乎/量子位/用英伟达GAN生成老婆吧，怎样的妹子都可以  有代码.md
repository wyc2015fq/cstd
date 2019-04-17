# 用英伟达GAN生成老婆吧，怎样的妹子都可以 | 有代码 - 知乎
# 



> 栗子 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-1512431ad5790028545e847b3d850086_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)△ StyleGAN生成物
对一只GAN来说，次元壁什么的，根本不存在吧。

你看英伟达的[StyleGAN](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/y9h6xkK9Keq4UrdpGJS-4Q)，本来是以生成**逼真人脸**闻名于世。
![](https://pic4.zhimg.com/v2-01d0b641ebb4cc6d8c2d3e15f3f044ff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='460' height='448'></svg>)
不过，自从官方把算法**[开了源](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/byEa0rnfBU3WP0KzOMZBVw)**，拥有大胆想法的勇士们，便开始用自己的力量支配StyleGAN，顺道拯救世界。

以前，你的老婆可能是绫波丽，可能是真白，可能是漩涡鸣子 (误) 。

而今，推特名叫**roadrunner01**的程序猿，给StyleGAN喂食了大量二次元女子图像。

然后，生成了从萝莉、到乙女、到御姐的 (各种) 演变过程。**每一帧**，都可以是你的选择：
![](https://pic2.zhimg.com/v2-f3c6f878070de892eb123beb93d52909_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)
发色和发型，眼睛里的神色，都在一刻不停地变化。

但任它千变万化，妹子依旧浑然天成。不论色彩，还是五官之间的配合，都有骄人的默契。

如果，你喜欢收到两颊泛红的讯号：
![](https://pic3.zhimg.com/v2-cc0ed33aa6cdfe6a9cc73214fe8d7cd6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)
或者，你只喜欢清新的邻家少女：
![](https://pic1.zhimg.com/v2-21293e7c1804c5fd0ee810d501211bec_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)
还是，性别特征不甚明显的幼女呢：
![](https://pic3.zhimg.com/v2-fa26f306d30f75d545523d81120a54f2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)
不管怎样，红瞳才是正义。
![](https://pic1.zhimg.com/v2-84cdd941874d5b40c9da8c55963fb238_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)
Reddit网友发出了客观中肯的赞叹：
![](https://pic3.zhimg.com/v2-e6e5f3f18371838f3a00d484ac063112_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='612' height='164'></svg>)△ 注释：waifu=wife
相比之下，推特网友就更加直白一些：
![](https://pic4.zhimg.com/v2-8bcb05bbae77504f724425dc04d1e797_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='299'></svg>)△ 刹不住车
## **要用怎样的数据集？**

虽然，roadrunner01并没有透露，他的StyleGAN到底吃了怎样的数据集。

不过，另有一位叫做**gwern**的程序猿，也用二次元数据训练了StyleGAN，并且**公开了自己的数据集**。

鉴于他的训练成果同样秀色可餐：
![](https://pic3.zhimg.com/v2-ffd0908cef2ffe131f4194f08bad762a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)△ 来自gwern的StyleGAN
我们就来观察一下，这只AI到底吃过怎样的数据。

数据集名叫**DANBOORU2018**，包含超过**330万**张图，超过**9970万**个标签：
![](https://pic2.zhimg.com/v2-714afd68904537c906925f33530a46fd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='867'></svg>)△ 为了安全，只截取了比较安全的部分
是个百无禁忌的StyleGAN了。

回头再看roadrunner01的作品，可能真的用了这个数据集：许多妹子打扮得十分客气，可以看到曼妙的锁骨，甚至香肩。
![](https://pic2.zhimg.com/v2-77583be676e4b9ea81944f68429d6b89_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='400'></svg>)△ 来自roadrunner01的StyleGAN
想要数据集的同学，请使用**文底**的传送门。

除了数据集之外，gwern还分享了**自己训练好的模型**，链接也在**文底**。

顺便观赏一下，StyleGAN帮他生成的老婆，明日香：
![](https://pic2.zhimg.com/v2-3fffd4e8d8e16d7da045de536f6d0e95_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='836' height='1042'></svg>)△ 来自gwern的StyleGAN
## **StyleGAN的原理是什么？**

数据集有了，就来看看StyleGAN是怎样工作的吧。

它之所以获得“GAN 2.0”的盛赞，就是因为**生成器**和普通的GAN不一样。

这里的生成器，是用**风格迁移**的思路重新发明的。
![](https://pic4.zhimg.com/v2-d4c874cbcda268ac11fbd0df34f034df_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)△ 把B的风格，迁到A头上
向AI输入两张图，图A决定人物的年龄、性别、头发长度和姿势；图B决定一切其他因素：肤色、发色、衣服颜色等等。

这样一来，图B的画风，就可以自然地转移给图A。

更重要的是，StyleGAN可以从**粗糙**、**中等**、**精细**三种尺度上调节图像的生成：
![](https://pic4.zhimg.com/v2-0a82e90d39ce409c5fc79ebb8bb1a767_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)![](https://pic4.zhimg.com/v2-c07dba5bcc1cd153c6601c7376acf3ff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)△ 上为粗糙，下为中等
**粗糙尺度**，是规模最大的调整，人脸的朝向、脸型和发型，都在这里调整；**中等尺度**，调整只会涉及脸部特征、发色发量了。
![](https://pic3.zhimg.com/v2-f1b7eeaccbe4ed3de2b2afcab582de56_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)△ 精细尺度
精细尺度，改变的是图像的**配色**，几乎不会给任务变脸了。

三者组合在一起，才有最终的生成效果。

不过，私以为**配色**才是最重要的：
![](https://pic2.zhimg.com/v2-f1dd7c8ee46a0edbac680c05e78cccc5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='556' height='370'></svg>)
你看，StyleGAN生成的蓝妹子多可爱：
![](https://pic3.zhimg.com/v2-84d376cd72bd01f2101f97864cb0432a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='272'></svg>)△ 来自roadrunner01的StyleGAN
这是一个美好的季节，StyleGAN算法开源了，330万张的数据集也有了。

想要生成老婆的话，可以开始训练了。

不想训练的话，用gwern训练好的模型直接生成也行啊。
![](https://pic3.zhimg.com/v2-b72d604e7ebb7c6e5e4330b3a1f94a56_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='360'></svg>)△ 来自roadrunner01的StyleGAN
悄悄告诉你，如果你不想生成老婆，却想生成**猫片**，可以直接用官方提供的预训练模型哟。
![](https://pic3.zhimg.com/v2-21bebf24b45f5d96a6d426dc61ee9daa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='809'></svg>)△ 这是一群假猫，不是数据集
DANBOORU2018二次元妹子数据集：
[https://www.gwern.net/Danbooru2018](https://link.zhihu.com/?target=https%3A//www.gwern.net/Danbooru2018)

英伟达StyleGAN官方实现：
[https://github.com/NVlabs/stylegan](https://link.zhihu.com/?target=https%3A//github.com/NVlabs/stylegan)

gwern训练好的二次元StyleGAN模型：
[https://drive.google.com/file/d/1z8N_-xZW9AU45rHYGj1_tDHkIkbnMW-R/view](https://link.zhihu.com/?target=https%3A//drive.google.com/file/d/1z8N_-xZW9AU45rHYGj1_tDHkIkbnMW-R/view)

StyleGAN论文传送门：
[https://arxiv.org/abs/1812.04948](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1812.04948)

—**完**—

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


