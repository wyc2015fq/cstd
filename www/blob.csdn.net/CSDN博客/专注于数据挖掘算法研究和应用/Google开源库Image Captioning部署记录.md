# Google开源库Image Captioning部署记录 - 专注于数据挖掘算法研究和应用 - CSDN博客
2018年12月25日 10:54:12[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：189
Github库：[https://github.com/tensorflow/models/tree/master/research/im2txt](https://github.com/tensorflow/models/tree/master/research/im2txt)
场景：给一张图片，描述图片内容。如下图：
![](https://img-blog.csdnimg.cn/2018122510382126.jpg)
环境：ubuntu，记住用户必须对目录有权限。
1、下载开源包：git clone [https://github.com/tensorflow/models.git](https://github.com/tensorflow/models.git)
      进入Im2txt子项目：cd research/im2txt
2、安装依赖包：
First ensure that you have installed the following required packages:
- **Bazel** ([instructions](http://bazel.io/docs/install.html))
- **Python 2.7**
- **TensorFlow** 1.0 or greater ([instructions](https://www.tensorflow.org/install/))
- **NumPy** ([instructions](http://www.scipy.org/install.html))
- **Natural Language Toolkit (NLTK)**:
	- First install NLTK ([instructions](http://www.nltk.org/install.html))
- Then install the NLTK data package "punkt" ([instructions](http://www.nltk.org/data.html))
- **Unzip**
1）Bazel： 我选择Installing using binary installer方式安装。
     参考网址：[https://docs.bazel.build/versions/master/install-ubuntu.html](https://docs.bazel.build/versions/master/install-ubuntu.html)
2）NLTK：sudo pip2 install -U nltk
     数据包安装：进入python2
```
>>>import nltk
>>> nltk.download("punkt")
```
3、准备训练数据
3.1  MSCOCO数据集
`bazel build //im2txt:download_and_preprocess_mscoco``bazel-bin/im2txt/download_and_preprocess_mscoco "im2txt/data/mscoco"`
准备成功如下图：
![](https://img-blog.csdnimg.cn/20181225104835405.png)
3.2 已训练CNN模型Inception v3 Checkpoint
```
wget "http://download.tensorflow.org/models/inception_v3_2016_08_28.tar.gz"
tar -xvf "inception_v3_2016_08_28.tar.gz" -C im2txt/data
rm "inception_v3_2016_08_28.tar.gz"
```
4、训练模型
`bazel build -c opt //im2txt/...`
nohup bazel-bin/im2txt/train \
  --input_file_pattern="im2txt/data/mscoco/train-?????-of-00256" \
  --inception_checkpoint_file="im2txt/data/inception_v3.ckpt" \
  --train_dir="im2txt/model/train" \
  --train_inception=false \
  --number_of_steps=1000000 &
evaluation script和Fine Tune the Inception v3 Model我这里没有去运行。有需要可参照官方介绍进行。训练完成如下图。
![](https://img-blog.csdnimg.cn/20181227124228887.png)
5、图片生成文本
`bazel build -c opt //im2txt:run_inference`
```
bazel-bin/im2txt/run_inference \
  --checkpoint_path=im2txt/model/train \
  --vocab_file=im2txt/data/mscoco/word_counts.txt \
  --input_files=im2txt/data/mscoco/raw-data/val2014/COCO_val2014_000000051008.jpg
```
 结果：
```
Captions for image COCO_val2014_000000051008.jpg:
  0) a cat laying on top of a laptop computer . (p=0.010717)
  1) a cat laying on top of a laptop keyboard . (p=0.001352)
  2) a cat laying on top of a laptop on a bed . (p=0.000764)
```
![](https://img-blog.csdnimg.cn/20181227124302670.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Zqc3NoYXJwc3dvcmQ=,size_16,color_FFFFFF,t_70)
