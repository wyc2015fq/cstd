# 如何用 TensorFlow 实现 OCR - zj360202的专栏 - CSDN博客





2018年01月17日 17:21:59[zj360202](https://me.csdn.net/zj360202)阅读数：4139









OCR（Optical Character Recognition，光学字符识别）是指电子设备（例如扫描仪或数码相机）检查纸上打印的字符，通过检测暗、亮的模式确定其形状，然后用字符识别方法将形状翻译成计算机文字的过程；即，针对印刷体字符，采用光学的方式将纸质文档中的文字转换成为黑白点阵的图像文件，并通过识别软件将图像中的文字转换成文本格式，供文字处理软件进一步编辑加工的技术。如何除错或利用辅助信息提高识别正确率，是OCR最重要的课题。





近年来，随着深度学习技术的进步，对 OCR 正确率的提升起到积极促进的作用。一些成熟的产品级别 OCR 系统能同时识别汉字、字母、数字、特殊字符并进行版面恢复，可以应用于身份证实名认证中的人证识别、名片识别、印刷文本识别等具体场景。





下面我们看如何利用 TensorFlow 实现英文字符识别，读者需要在机器上安装 TensorFlow 和 Keras，具体安装步骤可以参考上一篇文章《[利用
 TensorFlow 高级 API Keras 实现 MLP，CNN，LSTM](http://mp.weixin.qq.com/s?__biz=MzI2MzYwNzUyNg==&mid=2247483843&idx=1&sn=e297ce1ca83eb65825fe21c095ec5ba4&chksm=eab8044addcf8d5c7b544119125e1f23a5e5b990eb58fd38d09d9d9b44f916cd8936a7e5bc38&scene=21#wechat_redirect)》。OCR 例程位于 keras/examples/image_ocr.py。





为了运行该例程，需要安装以下两个包：




# pip install cairocffi

# pip install editdistance





根据运行时报错情况，可能还需要安装以下包：




# pip install matplotlib

# pip install scipy

# pip install numpy

# pip install h5py





这个例子使用卷积网络 + 递归网络 + CTC 对数损失函数来实现生成文本图像上的 OCR 功能。目前未验证是否真正学到了文本的通用形状，还是只能识别训练集里面的字体。





起初，先识别 4 字母的单词。前 12 个 epoch，难度递增。


使用 TextImageGenerator 类作为训练数据、测试数据生成器。该类也是 Keras 回调类。


经过 20 个 epoch，将更长的序列作为输入，重新构建能处理宽图像的模型，重建单词列表，包括两个由空格分离的单词。














1. 准备数据：


原来的代码中数据下载部分有 bug，需要做如下修改。


找到这两行：

    fdir = os.path.dirname(get_file('wordlists.tgz',

                                    origin='http://www.isosemi.com/datasets/wordlists.tgz', untar=True))





删掉，改为：




    fdir = '/tmp/'





从网盘（关注公众号，后台回复“20170220”获得下载链接）下载 wordlists.tgz，解压到 /tmp/，包括这两个文件：


wordlist_bi_clean.txt  wordlist_mono_clean.txt








2. 运行训练：


# python image_ocr.py





训练过程如下：




![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAGtv95rawYia7LButGMIfxH40Wiaor17LovhiaC90icREeAUeZPhUGvhTfnW5HUV2ZibxHFEnq0Jd7k6jw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAGtv95rawYia7LButGMIfxH4ickuE7lpA4Oy13n5yOkQVdicyKAIjrzsWLJF4dYgJkeu9Uf4rgQfIkoQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)





中间结果输出目录位于 examples/image_ocr/





epoch 0：

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAGtv95rawYia7LButGMIfxH4hLz8ApOwItMogJTriacsCKSXCERFI10TAf2ibcJp1ZHN2z9ZU3St9Tfw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


epoch 10:

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAGtv95rawYia7LButGMIfxH4ymco3uUaicJGIydAZWoC370k6RD7IHxX3A6wGCyzXtDVEibygWnLHTow/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


epoch 20:

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAGtv95rawYia7LButGMIfxH4OgI45ibH0JbTiaRr1mZYeqv6iaSbLBBQYvreHoicaaDZ24yILfS4JO4LOg/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)


epoch 24：

![](http://mmbiz.qpic.cn/mmbiz_png/a9UoojghtAGtv95rawYia7LButGMIfxH4A6f5Gu94gKsWlVPzbt1fohrwbtqqcJEcZLBI2V78TvuqOmQmp5Nriaw/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1)



