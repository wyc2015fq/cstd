# Cycle GAN: Unpaired Image-to-Image Translation using Cycle-Consistent Adversarial Networks - 家家的专栏 - CSDN博客





2018年12月20日 09:41:26[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：85








![](https://img-blog.csdnimg.cn/20181220093558489.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpaGFpemhpeWFu,size_16,color_FFFFFF,t_70)

1. Adversarial loss: 

![](https://img-blog.csdnimg.cn/20181220093701313.png)

2. Cycle consistency loss

![](https://img-blog.csdnimg.cn/20181220093741602.png)

3. Full objective function:

![](https://img-blog.csdnimg.cn/20181220093813303.png)

Cycle GAN, 不仅让生成的图像和真实图像对抗学习，同时要求生成的图像能够decode出来原图像，这样不至于生成的图像太失真。

文中有个例子，很好理解。就是不要要求中文翻译为英文，尽可能准确。还要求翻译出来的英文，能够反过来翻译为对应的中文。 有点拗口，但是很形象。







