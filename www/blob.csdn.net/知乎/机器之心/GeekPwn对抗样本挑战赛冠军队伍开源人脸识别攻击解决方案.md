# GeekPwn对抗样本挑战赛冠军队伍开源人脸识别攻击解决方案 - 知乎
# 



机器之心编辑，参与：路、王淑婷。

> 10 月 24 日，2018 GeekPwn 国际安全极客大赛在上海展开，由 FAIR 研究工程师吴育昕、约翰霍普金斯大学在读博士谢慈航组成的团队获得了令人瞩目的 [CAAD CTF 冠军](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650750713%26idx%3D2%26sn%3D5e08bf9a142320102872ef61a17f9629%26chksm%3D871afa87b06d739198e0073c23917158086ca8fd0fe0e04eeb73a8b52553b469daaf706f38ee%26scene%3D21%23wechat_redirect)。近日，该冠军队伍开源了其在人脸识别攻击项目中的解决方案。

2018 Geekpwn CAAD（对抗样本挑战赛）继承了 NIPS CAAD 2017 比赛的形式，但同时也添加了一些新的挑战。2018 年 10 月，吴育昕和谢慈航受邀参加 Geekpwn CAAD CTF，这是一场展示不同类型对抗样本攻防的现场比赛。

比赛上半场中，赛会要求所有选手进行非定向图片（将飞行器识别为任何其他物体）、定向图片（将武器识别为特定的其他物品）以及亚马逊名人鉴别系统（将大赛主持人蒋昌建的照片识别为施瓦辛格）共计三种图像的对抗样本攻击。由于在比赛前选手并不知晓题目模型所采用的算法，所以此类攻击也被称为「黑盒攻击」，其中第三个挑战在赛前更是被评委视为「无法完成的任务」。

第三项挑战是一项新挑战，要求参赛队伍攻击未知的人脸识别系统。在限时 30 分钟的上半场比赛中，参赛者拿到大赛主持人蒋昌建的照片。他们需要对照片做一些小的修改，以欺骗人脸识别系统，让它把照片中的人识别为施瓦辛格。比赛结束后，大家才知道该人脸识别系统是亚马逊名人鉴别系统。

由吴育昕与谢慈航组成的「IYSWIM」战队在限时 30 分钟的比赛中，首先于 21 分钟破解了亚马逊名人鉴别系统 Celebrity Recognition，并随后在定向图片的对抗样本攻击上破解成功，取得了领先。

近日，吴育昕在博客中简单介绍了他们对人脸识别的攻击，代码也放到了 GitHub 上。吴育昕称他们对比赛其他类型的攻击使用的是类似的算法，只是换了不同的 CNN。而关于比赛下半段防御相关内容的 paper 将稍后放出，代码和模型将于明年开源。




**攻击方法**

吴育昕在博客中称其黑盒攻击是很传统的方法，即用已知模型做梯度下降。




**1. Model：**

搜了搜 GitHub 上的人脸识别代码，找了个 5 分钟内能跑起来的 facenet。毕竟 GitHub 上大部分代码质量堪忧，有不少项目完全是靠 README 骗 star 的。能否短时间内复现 evaluation 结果是评判质量的重要 metric。更何况，我这次只需要能 evaluation 就够了。

facenet 的人脸识别流程很标准：对每张脸 I 输出一个 vector f(I)，然后通过比较不同脸的 f(I) 之间的 cosine distance 进行人脸识别。

**2. Objective：**

对于分类器的 target/untarget attack，没什么好说的，就是 minimize/maximize 一个 cross entropy loss。而对于人脸，我们首先收集 target 人物的 N 张人脸图片，运行模型得到 N 个 embedding vector v_i。我们的 objective 就是 minimize 输入图片的 embedding 到这 N 个 embedding 的平均距离：
![](https://pic2.zhimg.com/v2-a4fe8d5431570586942f95908dcec109_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='288' height='67'></svg>)
**3. Attack：**

我们在最原始的 PGD (Projected Gradient Descent) 攻击上加了点 trick。原始的 PGD 就是

 1）算 objective 对输入图片的梯度 ΔI
 2）更新输入 I = I−sign(ΔI)
 3）clip 以免输入超出允许范围：I←clip(I,I_orig−ϵ,I_orig+ϵ)
 4）回到第一步循环

这里有不少 trick 可以提高黑盒攻击的 generalization 能力：

 1）对梯度 normalize 一下，然后更新时用 0.9 的 momentum
 2）在循环每一步给图片随机加点小 noise
 3）在循环每一步对输入图片做些乱七八糟的随机 resize 变换
 4）对梯度做 spatial smoothing

这些 trick 从去年的 NIPS 比赛之后就陆续被大家用了。这次的人脸攻击用了 1,2,3。第三条是我比赛时发现代码不 work 临时加上的。事后验证发现，这一点尤其重要。如果不加的话攻击很难成功。

在相关的 GitHub repo 中，我们可以看到该团队的攻击代码和结果：




**结果**

比赛期间，吴育昕团队成功地攻击了 AWS 名人鉴别系统，让它把蒋昌建识别为了施瓦辛格。
![](https://pic2.zhimg.com/v2-68482903236029a10e7139bf385c6d9d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='579'></svg>)
下面这个挑战比较困难（不同随机种子的成功率较低），可能是因为：1）原人物和目标人物都是美国名人。2）性别不同。
![](https://pic3.zhimg.com/v2-ff17a8324aa0399e939a5820c7af346e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='486'></svg>)
很明显，Azure Vision API 和 Clarifai Celebrity Recognition 系统都可以被欺骗：
![](https://pic1.zhimg.com/v2-8179e4c364cfc2f133cb8c3bbc1c20d4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='478'></svg>)![](https://pic2.zhimg.com/v2-76ac7fd1e0529ef802763c0ede3ee4ed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='590'></svg>)
以上图像的原版本和对抗版本都可以从以下地址找到，若有兴趣你可以下载并在公开 API 上进行测试：[https://github.com/ppwwyyxx/Adversarial-Face-Attack/blob/master/images](https://link.zhihu.com/?target=https%3A//github.com/ppwwyyxx/Adversarial-Face-Attack/blob/master/images)




**使用代码的准备工作**

1. 安装 TensorFlow ≥ 1.7

2. 按照 facenet wiki 中的步骤 1 - 4 设置 facenet （[https://github.com/davidsandberg/facenet/wiki/Validate-on-LFW](https://link.zhihu.com/?target=https%3A//github.com/davidsandberg/facenet/wiki/Validate-on-LFW)）

3. 复制此 repo 并解压内部的预训练模型：


```
git clone https://github.com/ppwwyyxx/Adversarial-Face-Attack
cd Adversarial-Face-Attack
wget https://github.com/ppwwyyxx/Adversarial-Face-Attack/releases/download/v0.1/model-20180402-114759.tar.bz2
tar xjvf model-20180402-114759.tar.bz2
```


你也可以从 facenet 下载模型（[https://github.com/davidsandberg/facenet#pre-trained-models](https://link.zhihu.com/?target=https%3A//github.com/davidsandberg/facenet%23pre-trained-models)）。

4. 验证模型和数据集：


```
./face_attack.py --data /path/to/lfw_mtcnnpy_160 --validate-lfw
# /path/to/lfw_mtcnnpy_160 is obtained above in step #4 in facenet wiki.
```


它应该在 LFW 数据集上有高准确率，如：


```
Accuracy: 0.99517+-0.00361
Validation rate: 0.97467+-0.01454 @ FAR=0.00067
```





**执行攻击**


```
./face_attack.py --data /path/to/lfw_mtcnnpy_160 \
    --attack images/clean-JCJ.png \
    --target Arnold_Schwarzenegger \
    --output JCJ-to-Schwarzenegger.png
```


--target 必须是在 LFW 数据集中有很多图像（越多越好）的人。可以通过以下方式找到这种人：


```
find /path/to/lfw_mtcnnpy_160/ -type f -printf "%h\0" | \
    xargs -0 -L1 basename | sort | uniq -c | sort -k1 -n
```


你可以在 LFW 目录中添加新的名人作为 victim，或者为 LFW 中现有的名人添加图像。添加新图像之后，你需要重复 facenet wiki 中的步骤 4，以裁剪和对齐添加的新图像。

请注意，攻击包含随机性：你不会每次都得到相同的输出。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*- 博客地址：[http://ppwwyyxx.com/2018/Geekpwn-CAAD-CTF/](https://link.zhihu.com/?target=http%3A//ppwwyyxx.com/2018/Geekpwn-CAAD-CTF/)
- GitHub 地址：[https://github.com/ppwwyyxx/Adversarial-Face-Attack](https://link.zhihu.com/?target=https%3A//github.com/ppwwyyxx/Adversarial-Face-Attack)


