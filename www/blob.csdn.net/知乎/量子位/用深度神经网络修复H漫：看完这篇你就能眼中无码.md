# 用深度神经网络修复H漫：看完这篇你就能眼中无码 - 知乎
# 



## 晓查 发自 凹非寺量子位 出品 | 公众号 QbitAI



![](https://pic3.zhimg.com/v2-8fe61f713d245defd7f2fb88feb473ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='675'></svg>)



AI“脑补”能力一流，现在甚至已经能画出艺术品。热爱H漫的死宅们灵光一闪，AI是否也可以把马赛克阻挡的内容也画出来呢？

果然，原始动物本能是第一科技生产力。最近就有人在GitHub上发布了一个**DeepCreamPy**项目，能帮你把H漫中羞羞的画面补上。



![](https://pic2.zhimg.com/v2-b0ca4c886467bd644f8f0e35ec6a11d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='868'></svg>)



该项目使用深度完全卷积神经网络（deep fully convolutional neural network），参照了英伟达在今年4月前发布的一篇论文。当然，英伟达原文的目的可不是用来做羞羞的事情，而是为了复原画面被单色条带遮挡的问题。




![](https://pic3.zhimg.com/v2-065f0515fe26e95382094d67b04cfc0e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='898' height='701'></svg>)



从实际效果来看，复原后的图片涂抹痕迹仍然比较明显，不过处理线条比较简单的漫画可以说是绰绰有余。


接下来，就是让你“眼中无码”的DIY教程啦！

## **适用范围**

DeepCreamPy仅适用于**薄码**，如果马赛克太大太厚，去码可能会失效。另外，它**对真人图片无效**。如果你非要尝试，可以看一下强行使用的效果：



![](https://pic1.zhimg.com/v2-55934dcb76a21d785a740548e23dc228_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='566' height='193'></svg>)



而且DeepCreamPy目前的版本还不能完全自动处理图片，需要用Photoshop首先对马赛克部分进行手动预处理。


如果你十分介意这些缺点，那么后面的内容可以跳过了。如果你学(wu)习(ma)的意愿很强烈，请接着往下看。

## **使用方法**

## **安装软件**

64位Windows用户可以直接下载exe程序，资源地址请见附录。

如果你使用的不是Windows系统，需要自己编译，程序中已经包含训练模型，请从压缩包中提取。

编译代码需要一下组件：
- Python 3.6
- TensorFlow 1.10
- Keras 2.2.4
- Pillow
- h5py

Tensorflow、Keras、Pillow、和h5py可以使用pip安装：

`$ pip install -r requirements.txt`

请注意软件版本，Windows上的TensorFlow不兼容Python 2，也不兼容Python 3.7。

## **手动处理黑条遮挡和马赛克遮挡**

首先打开**Photoshop**或者**GIMP**。对于单色条遮住人物敏感部位的情况，使用**纯绿色**(色号#00FF00#)预处理图片，以绿条取代图片中的黑条。



![](https://pic2.zhimg.com/v2-69bcb66af00a180182357e03e952faed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)



强烈建议使用铅笔而不是毛刷工具，如果使用毛刷，请**一定要关闭抗锯齿功能**。或者用魔棒选中马赛克区域，再用油漆桶上色。


最后将处理的文件以PNG格式存储在软件的”decensor_input”文件夹中。
如果敏感部位不是黑条，而是马赛克，还需要将未上色的原始图片放入”decensor_input_original” 文件夹中，并确保其文件名和放在”decensor_input”中的预处理图片文件名相同。

## **运行去马赛克软件**

**A. 使用exe直接处理**

双击软件即可

**B. 命令行处理**

对于黑条遮挡的图片，输入以下命令：

`$ python decensor.py`

对于马赛克遮挡的图片，输入以下命令：

`$ python decensor.py —is_mosaic=True`

## **注意事项**

如果你图片处理后成了这样：



![](https://pic2.zhimg.com/v2-ef1800df7924c24ef75dd7904e9ae021_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='512'></svg>)



一定是你处理的姿势不对，请注意不要犯以下两种错误：




![](https://pic3.zhimg.com/v2-c62b3a8e932d16be3844028e5ad1eb9e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='503'></svg>)



第一幅图中，图片马赛克区域没有完全涂满；第二幅图中，由于开启了抗锯齿功能，导致马赛克边缘区域不是**纯绿色**填充，请**关闭抗锯齿**功能！！


最后给伸手党们附上软件地址，请自行下载，低调使用。Enjoy it！

## **资源地址**

GitHub项目地址：
[https://github.com/deeppomf/DeepCreamPy#dependencies-for-running-the-code-yourself](https://link.zhihu.com/?target=https%3A//github.com/deeppomf/DeepCreamPy%23dependencies-for-running-the-code-yourself)

软件下载地址：
[https://github.com/deeppomf/DeepCreamPy/releases/tag/v1.2.1-beta](https://link.zhihu.com/?target=https%3A//github.com/deeppomf/DeepCreamPy/releases/tag/v1.2.1-beta)

英伟达论文地址：
[https://arxiv.org/pdf/1804.07723.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1804.07723.pdf)


