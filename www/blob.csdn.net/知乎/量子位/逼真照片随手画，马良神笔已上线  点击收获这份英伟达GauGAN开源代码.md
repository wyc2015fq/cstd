# 逼真照片随手画，马良神笔已上线 | 点击收获这份英伟达GauGAN开源代码 - 知乎
# 



> 鱼羊 发自 纽凹非寺
量子位 报道 | 公众号 QbitAI

有一种神奇的力场，它能凭空造物：
![](https://pic4.zhimg.com/v2-fabf09cc35e9481803164b0af97101eb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='367'></svg>)
还能扭转时间：
![](https://pic3.zhimg.com/v2-d0239d8f64ca22fcfba2dc2f95b5678a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1183' height='678'></svg>)
它专注于山和大海：
![](https://pic2.zhimg.com/v2-31db09888c8cb8427fc3bc220b3d2b09_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='367'></svg>)
你也可以试着造一造人潮人海。

你是否还记得它的名字？

没错，就是英伟达在上个月公布的，致敬印象派大师高更的GauGAN。

现在，这个能让普通人秒变神笔马良的神器终于开源啦！
![](https://pic3.zhimg.com/v2-2711ecae34af2a35620eb718395289f2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='540'></svg>)
## **怎么用**

首先克隆这个repo：


```
1 git clone https://github.com/NVlabs/SPADE.git
2 cd SPADE/
```


然后备好PyTorch 1.0和python 3+，安装requirements.txt：

`1 pip install -r requirements.txt`

还需要：


```
1 cd models/networks/ git clone https://github.com/vacancy/Synchronized-BatchNorm-PyTorch 
2 cp Synchronized-BatchNorm-PyTorch/sync_batchnorm . -rf 
3 cd ../../
```


## **别忘了数据集和预训练模型**

这一次随代码公布的还有数据集和预训练模型。

在下载数据集时，记得下载train2017.zip，val2017.zip，stuffthingmaps_trainval2017.zip和annotations_trainval2017.zip。
![](https://pic3.zhimg.com/v2-e1eefb0adb661595993c4ffcab82e4ea_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1285' height='503'></svg>)
要特别注意的是英伟达使用了一个生成实例的脚本datasets/coco_generate_instance_map.py。

准备好了数据集，就可以用预训练模型生成图像了。

从Google Drive Folder下载预训练模型，存成‘/checkpoints’：


```
1 cd checkpoints 
2 tar xvf checkpoints.tar.gz 
3 cd ../
```


开始生成图像：

`1 python test.py --name [type]_pretrained --dataset_mode [dataset] --dataroot [path_to_dataset]`

输出的图像默认存储在 ./results/[type]_pretrained/

当然你也可以训练新的模型：


```
1 # To train on the Facades or COCO dataset, for example.
2 python train.py —name [experiment_name] —dataset_mode facades —dataroot [path_to_facades_dataset] 
4 python train.py —name [experiment_name] —dataset_mode coco —dataroot [path_to_coco_dataset] 
5
6 # To train on your own custom dataset 
7 python train.py —name [experiment_name] —dataset_mode custom —label_dir [path_to_labels] — image_dir [path_to_images] —label_nc [num_labels]
```


不过想要真正利用GauGAN成为一名“造物主”，你还需要一台配备8个V100 GPU的NVIDIA DGX1。此外，该代码仅供学术研究使用。

## **为何与众不同**

GauGAN合成的图像明显要比此前的图像合成方法逼真得多，NVIDIA应用深度学习研究副总裁Bryan Catanzaro就说，这项技术不是简单地将其他图像拼接在一起，它是在合成新的图像——像个艺术家那样。
![](https://pic4.zhimg.com/v2-7db62b4f100c03e315c50c816ff7722f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='367'></svg>)
而GauGAN之所以能有如此神奇的手笔，当归功于空间适应标准化（SPADE）算法。

SPADE不单单使用随机的输入图像，它还采用了被称作分割图的图像，在分割图中，每一个像素都会被归类。这样一来，SPADE就能生成更接近于真实的图像，这种模式被称作“图对图翻译”。
![](https://pic1.zhimg.com/v2-25683c7a278de460632ed89a75e02694_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='403'></svg>)
并且SPADE不再将分割图输入到第一层，而是使用它的下采样来调整每一层的输出，这样就不会在生成图像中丢失语义信息。
![](https://pic1.zhimg.com/v2-958cfc5287a0d177503ebb5687d38ccc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='756' height='592'></svg>)



在这样的训练之下，生成器不会将草放在天空的位置，因为鉴别器能判断出这样的图像是假的。

也就是说，SPADE解决了对pix2pixHD来说最大的难题。

## **传送门**

GitHub地址：[https://github.com/NVlabs/SPADE](https://link.zhihu.com/?target=https%3A//github.com/NVlabs/SPADE)
项目地址：[https://nvlabs.github.io/SPADE/](https://link.zhihu.com/?target=https%3A//nvlabs.github.io/SPADE/)
论文地址：[https://arxiv.org/abs/1903.07291](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1903.07291)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


