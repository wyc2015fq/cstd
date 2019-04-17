# 英伟达的最强人脸GAN开源了，它吃的高清数据集也开源了 - 知乎
# 



> 栗子 假装发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-a04f8cb0bc75c253feac80ae9b683adc_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='540' height='303'></svg>)
你大概还没忘记，[英伟达去年年底推出的GAN](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/y9h6xkK9Keq4UrdpGJS-4Q)，它合成的人脸甚至骗得过肉眼。

如今，它终于有了自己的名字，叫**StyleGAN**。顾名思义，GAN的生成器，是借用**风格迁移**的思路重新发明的，能在粗糙、中度、精细三个层面调节图像生成。

更重要的是，你现在也能自己养一只这样的GAN了：

官方实现的代码开源了，提供了许多预训练好的模型，自然也支持自己训练模型。
![](https://pic1.zhimg.com/v2-840b2708511342eaecddb6ecd1df2758_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
另外，Flicker人像照片的高清数据集 (FFHQ) 也开源了，包含70,000张高清人脸。

## **官方实现**

首先，送上来自英伟达的友情提示：

这个实现可以用Linux跑，也可以用Windows跑，但墙裂推荐大家用**Linux**跑，为性能和兼容性着想。

除此之外，必需品还有Python 3.6，和TensorFlow 1.10以上 (支持GPU) 。

## **预训练模型**

这里，用pretrained_example.py举个简易的栗子。执行的时候，脚本会从Google Drive下载一个预训练的StyleGAN生成器，然后用它来生成图像：


```python
> python pretrained_example.py
  Downloading https://drive.google.com/uc?id=MEGjdvVpUsu1jB4zrXZN7Y4kBBOzizDQ .... done
  
  Gs                              Params    OutputShape          WeightShape
  ---                             ---       ---                  ---
  latents_in                      -         (?, )             -
  ...
  images_out                      -         (?, , , )   -
  ---                             ---       ---                  ---
 Total                           
 
 > ls results
 example.png # https://drive.google.com/uc?id=1UDLT_zb-rof9kKH0GwiJW_bS9MoZi8oP
```

![](https://pic4.zhimg.com/v2-01d0b641ebb4cc6d8c2d3e15f3f044ff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='460' height='448'></svg>)
预训练的生成器有三种食用方法：

**一是**Gs.run()快速模式，这里的输入和输出都是numpy阵列：


```python
# Pick latent vector.
 rnd = np.random.RandomState()
 latents = rnd.randn(, Gs.input_shape[])
 
 # Generate image.
 fmt = dict(func=tflib.convert_images_to_uint8, nchw_to_nhwc=True)
 images = Gs.run(latents, None, truncation_psi=0.7, randomize_noise=True, output_transform=fmt)
```


**二是**用Gs.get_output_for()把生成器整合到一个更大的TensorFlow表达式里面：


```python
latents = tf.random_normal([self.minibatch_per_gpu] + Gs_clone.input_shape[:])
 images = Gs_clone.get_output_for(latents, None, is_validation=True, randomize_noise=True)
 images = tflib.convert_images_to_uint8(images)
 result_expr.append(inception_clone.get_output_for(images))
```


**三是**查找Gs.components.mapping和Gs.components.synthesis，访问生成器的各个子网络。与Gs相似，这鞋子网络也表示为dnnlib.tflib.Network的独立示例。


```python
src_latents = np.stack(np.random.RandomState(seed).randn(Gs.input_shape[]) for seed in src_seeds)
 src_dlatents = Gs.components.mapping.run(src_latents, None) # [seed, layer, component]
 src_images = Gs.components.synthesis.run(src_dlatents, randomize_noise=False, **synthesis_kwargs)
```


## **自己训练模型**

如果不满足于预训练的模型，想自己训练模型，就要先：

**一、处理好数据集**

把数据集储存为多重分辨率的TFRecords，训练和评估脚本都是在这上面跑：


```python
> python dataset_tool.py create_lsun datasets/lsun-bedroom-full ~/lsun/bedroom_lmdb --resolution 
 > python dataset_tool.py create_lsun_wide datasets/lsun-car-x384 ~/lsun/car_lmdb --width  --height 
 > python dataset_tool.py create_lsun datasets/lsun-cat-full ~/lsun/cat_lmdb --resolution 
 > python dataset_tool.py create_cifar10 datasets/cifar10 ~/cifar10
 > python dataset_tool.py create_from_images datasets/custom-dataset ~/custom-images
```


数据集表示为一个目录，里面的每张图像都有多种不同的分辨率，用于高效的streaming。每个分辨率都有一个自己的*.tfrecords文件。数据有标注的话，也是用一个分开的文件来储存的。
![](https://pic4.zhimg.com/v2-d4c874cbcda268ac11fbd0df34f034df_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
**二、训练吧**

官方提供的训练过程分四步：

> 1. 编辑train.py，通过取消注释或者修改某些行，来指定数据集和训练配置；
2. 用train.py来运行训练脚本；
3. 结果会写在一个新目录里，叫results/-；
4. 训练直至完成，几天时间可能是要的。

(最好有一台英伟达高端GPU，至少11GB的DRAM，再开始训练。有很多台的话，当然更好。)

至于训练好的模型该怎样**评估**，大家可以去项目页自行观察。

## **高清无码数据集**

和官方代码实现一起发布的，就是Flickr高清人脸数据集 (FFHQ) 了。

那些几可乱真的人脸，就是StyleGAN吃了这个数据集，才生成的。
![](https://pic2.zhimg.com/v2-7c5c6809e6e6342ee42ff85750774071_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1008' height='512'></svg>)
数据集里包含7万张**1024 x 1024**高清人像。英伟达说，这些照片在年龄、种族、以及图片背景上，都有很强的多样性。

并且，**眼镜**、**墨镜**、**帽子**这些元素，也是应有尽有。

团队说，图像是直接从Flickr上面扒下来，自动对齐自动裁剪而成。并且，数据集里收录的图片都有使用许可，无须担心。

## **还不去试试**

有大胆想法的同学们，可以去试试了。毕竟，不是只有生成人脸这一种功能，猫片，汽车，房间……
![](https://pic4.zhimg.com/v2-93adc0ff4be4e6f3a45820120529345f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
你还想生成一些什么？

代码实现传送门：
[https://github.com/NVlabs/stylegan](https://link.zhihu.com/?target=https%3A//github.com/NVlabs/stylegan)

FFHQ数据集传送门：
[https://github.com/NVlabs/ffhq-dataset](https://link.zhihu.com/?target=https%3A//github.com/NVlabs/ffhq-dataset)

论文传送门：
[https://arxiv.org/abs/1812.04948](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1812.04948)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


