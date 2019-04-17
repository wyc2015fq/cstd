# TensorFlow可以“预装”数据集了，新功能Datasets出炉 - 知乎
# 



> 郭一璞 发自 凹非寺
量子位 报道 | 公众号 QbitAI

训练机器学习模型的时候，需要先找数据集、下载、装数据集……太麻烦了，比如MNIST这种全世界都在用的数据集，能不能来个一键装载啥的？

Google也这么想。
![](https://pic1.zhimg.com/v2-49b38c721e09d37f89fd352dbd8db534_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='1041'></svg>)
今天，TensorFlow推出了一个新的功能，叫做**TensorFlow Datasets**，可以以tf.data和NumPy的格式将公共数据集装载到TensorFlow里。

目前已经有29个数据集可以通过TensorFlow Datasets装载：

> **音频类**
nsynth
**图像类**
cats_vs_dogs
celeb_a
celeb_a_hq
cifar10
cifar100
coco2014
colorectal_histology
colorectal_histology_large
diabetic_retinopathy_detection
fashion_mnist
image_label_folder
imagenet2012
lsun
mnist
omniglot
open_images_v4
quickdraw_bitmap
svhn_cropped
tf_flowers
**结构化数据集**
titanic
**文本类**
imdb_reviews
lm1b
squad
**翻译类**
wmt_translate_ende
wmt_translate_enfr
**视频类**
bair_robot_pushing_small
moving_mnist
starcraft_video

未来还会增加更多数据集，你也可以自己添加数据集。

## **具体怎么装**

必须是TensorFlow1.12以上版本才可以安装，某些数据集需要额外的库。


```python
pip install tensorflow-datasets

# Requires TF 1.12+ to be installed.
# Some datasets require additional libraries; see setup.py extras_require
pip install tensorflow
# or:
pip install tensorflow-gpu
```


每个数据集都作为DatasetBuilder公开，已知：

1.从哪里下载数据集，如何提取数据并写入标准格式；

2.如何从disk加载；

3.各类要素名称、类型等信息。

这些DatasetBuilder都能直接实例化或者用tfds.builder字符串读取：


```
1import tensorflow_datasets as tfds
 2
 3# Fetch the dataset directly
 4mnist = tfds.image.MNIST()
 5# or by string name
 6mnist = tfds.builder('mnist')
 7
 8# Describe the dataset with DatasetInfo
 9assert mnist.info.features['image'].shape == (28, 28, 1)
10assert mnist.info.features['label'].num_classes == 10
11assert mnist.info.splits['train'].num_examples == 60000
12
13# Download the data, prepare it, and write it to disk
14mnist.download_and_prepare()
15
16# Load data from disk as tf.data.Datasets
17datasets = mnist.as_dataset()
18train_dataset, test_dataset = datasets['train'], datasets['test']
19assert isinstance(train_dataset, tf.data.Dataset)
20
21# And convert the Dataset to NumPy arrays if you'd like
22for example in tfds.as_numpy(train_dataset):
23  image, label = example['image'], example['label']
24  assert isinstance(image, np.array)
```


你也可以用tfds.load执行一系列的批量示例、转换操作，然后再调用。


```
1import tensorflow_datasets as tfds
2
3datasets = tfds.load("mnist")
4train_dataset, test_dataset = datasets["train"], datasets["test"]
5assert isinstance(train_dataset, tf.data.Dataset)
```


## **数据集版本控制**

当数据集自身版本更新时，已经开始训练的数据不会变化，TensorFlow官方会采取增加新版本的方式把新的数据集放上来。

## **具体配置**

有不同变体的数据集用BuilderConfigs进行配置，比如大型电影评论数据集（Large Movie Review Dataset），可以对输入文本进行不同的编码。

内置配置与数据集文档一起列出，可以通过字符串进行寻址。


```
1# See the built-in configs
 2configs = tfds.text.IMDBReviews.builder_configs
 3assert "bytes" in configs
 4
 5# Address a built-in config with tfds.builder
 6imdb = tfds.builder("imdb_reviews/bytes")
 7# or when constructing the builder directly
 8imdb = tfds.text.IMDBReviews(config="bytes")
 9# or use your own custom configuration
10my_encoder = tfds.features.text.ByteTextEncoder(additional_tokens=['hello'])
11my_config = tfds.text.IMDBReviewsConfig(
12    name="my_config",
13    version="1.0.0",
14    text_encoder_config=tfds.features.text.TextEncoderConfig(encoder=my_encoder),
15)
16imdb = tfds.text.IMDBReviews(config=my_config)
```


也可以用你自己的配置，通过tfds.core.BuilderConfigs，进行以下步骤：

1.把你自己的配置对象定义为的子类 tfds.core.BuilderConfig。比如叫“MyDatasetConfig”；

2.在数据集公开的列表中定义BUILDER_CONFIGS类成员，比如“MyDatasetMyDatasetConfig”；

3.使用self.builder_config在MyDataset配置数据生成，可能包括在_info()或更改下载数据访问中设置不同的值。

## **关于文本数据集**

平常遇到文本数据集都比较难搞，但是有了TensorFlow Datasets就会好办一些，包含很多文本任务，三种文本编码器：

1.ByteTextEncoder，用于字节/字符级编码；

2.TokenTextEncoder，用于基于词汇文件的单词级编码；

3.SubwordTextEncoder，用于子词级编码，具有字节级回退，以使其完全可逆，比如可以把“hello world”分为[“he”，“llo”，“”，“wor”，“ld”]，然后进行整数编码。

以上这些都支持Unicode。

编码器和词库可以这样访问：


```
1imdb = tfds.builder("imdb_reviews/subwords8k")
 2
 3# Get the TextEncoder from DatasetInfo
 4encoder = imdb.info.features["text"].encoder
 5assert isinstance(encoder, tfds.features.text.SubwordTextEncoder)
 6
 7# Encode, decode
 8ids = encoder.encode("Hello world")
 9assert encoder.decode(ids) == "Hello world"
10
11# Get the vocabulary size
```


TensorFlow官方明确表示，TensorFlow和TensorFlow Datasets在文本支持方面将会进一步改进。

## **传送门**

最后，下面是TensorFlow官方提供的各类文档资料教程链接：

TensorFlow博客原文

[https://medium.com/tensorflow/introducing-tensorflow-datasets-c7f01f7e19f3](https://link.zhihu.com/?target=https%3A//medium.com/tensorflow/introducing-tensorflow-datasets-c7f01f7e19f3)

TensorFlow官方文档

[https://www.tensorflow.org/datasets](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/datasets)

GitHub

[https://github.com/tensorflow/datasets](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/datasets)

Colab教程

[https://colab.research.google.com/github/tensorflow/datasets/blob/master/docs/overview.ipynb](https://link.zhihu.com/?target=https%3A//colab.research.google.com/github/tensorflow/datasets/blob/master/docs/overview.ipynb)

Enjoy yourself~

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


