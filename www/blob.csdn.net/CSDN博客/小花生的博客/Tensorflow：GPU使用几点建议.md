
# Tensorflow：GPU使用几点建议 - 小花生的博客 - CSDN博客


2018年09月06日 21:48:26[Peanut_范](https://me.csdn.net/u013841196)阅读数：2334



## GPU运行Tensorflow的几点建议：
**1.在运行之前先查看GPU的使用情况：**
指令：nvidia-smi  备注：查看GPU此时的使用情况
或者
指令：watch nvidia-smi    备注：实时返回GPU使用情况
**2.指定GPU训练：**
方法一、在python程序中设置：
代码：os.environ[‘CUDA_VISIBLE_DEVICES’] = ‘0’   备注：使用 GPU 0
代码：os.environ[‘CUDA_VISIBLE_DEVICES’] = ‘0,1’   备注：使用 GPU 0，1
方法二、在执行python程序时候：
指令：CUDA_VISIBLE_DEVICES=2 python yourcode.py
指令：CUDA_VISIBLE_DEVICES=0,1 python yourcode.py
**备注：**‘=’的左右不允许有空格
---
**注：**TensorFlow会默认直接占满我们模型部署的GPU的存储资源，只允许一个小内存的程序也会占用所有GPU资源。因此有的时候我们通过nvidia-smi查看GPU状态的时候，会发现有些GPU的计算利用率很低或者计算利用率为0，但是存储被占满了，而这个时候其他人也不能使用这块GPU。但是现在公司的问题是模型多，卡不够用，所有只能“文明”使用GPU，如果设置为允许动态增长的话，这样这个GPU没有被占的存储还可以被其他人使用。
---
**3.两种限定GPU占用量的方法：**
方法一、设置定量的GPU显存使用量:
config = tf.ConfigProto()
config.gpu_options.per_process_gpu_memory_fraction = 0.4 \# 占用GPU40%的显存
session = tf.Session(config=config)
方法二、设置最小的GPU显存使用量，动态申请显存:（建议）
config = tf.ConfigProto()
config.gpu_options.allow_growth = True
session = tf.Session(config=config)
---
**注：**还有一个问题就是计算资源闲置，发现计算资源闲置的一个很简单的方法就是使用命令nvidia-smi查看GPU的使用状态，只要GPU的计算时间使用率没有达到80%-100%,，都是对GPU的极大浪费。
---
**4.**将数据的处理使用多线程的queue在CPU上进行，使得GPU随时都有充足的数据使用，更专注于训练，这样可以大幅度提升模型的训练速度。
**with tf.device(‘/cpu:0’):**
\# function to get and process images or data.
distorted_inputs = load_and_distort_images()
**5.**程序中跟训练无关的尽量不要sess.run()操作
**6.**训练代码的优化等，可参考：
[http://d0evi1.com/tensorflow/performance/](http://d0evi1.com/tensorflow/performance/)
---

###### 注：博众家之所长，集群英之荟萃。

