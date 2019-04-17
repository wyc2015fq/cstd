# hxy系列2-图像分类与KNN - 战斗蜗牛的专栏 - CSDN博客





2016年09月29日 15:09:20[vbskj](https://me.csdn.net/vbskj)阅读数：482











作者： [寒小阳](http://blog.csdn.net/han_xiaoyang?viewmode=contents)

时间：2015年11月。 

出处：[http://blog.csdn.net/han_xiaoyang/article/details/49949535](http://blog.csdn.net/han_xiaoyang/article/details/49949535)

声明：版权所有，转载请注明出处，谢谢

## 1. 图像分类问题


这是人每天自然而然会做的事情，普通到大部分时候，我们都感知不到我们在完成一个个这样的任务。早晨起床洗漱，你要看看洗漱台一堆东西中哪个是杯子，哪个是你的牙刷；吃早餐的时候你要分辨食物和碗碟… 

抽象一下，对于一张输入的图片，要判定它属于给定的一些标签/类别中的哪一个。看似很简单的一个问题，这么多年却一直是计算机视觉的一个核心问题，应用场景也很多。它的重要性还体现在，其实其他的一些计算机视觉的问题(比如说物体定位和识别、图像内容分割等)都可以基于它去完成。


咱们举个例子从[机器学习](http://lib.csdn.net/base/2)的角度描述一下这个问题^_^


计算机拿到一张图片(如下图所示)，然后需要给出它对应{猫，狗，帽子，杯子}4类的概率。人类是灰常牛逼的生物，我们一瞥就知道这货是猫。然而对计算机而言，他们是没办法像人一样『看』到整张图片的。对它而言，这是一个3维的大矩阵，包含248*400个像素点，每个像素点又有红绿蓝(RGB)3个颜色通道的值，每个值在0(黑)-255(白)之间，计算机就需要根据这248*400*3=297600个数值去判定这货是『猫』

![猫图像=>矩阵](http://cs231n.github.io/assets/classify.png)

### 1.1 图像识别的难点


图像识别看似很直接。但实际上包含很多挑战，人类可是经过数亿年的进化才获得如此强大的大脑，对于各种物体有着精准的视觉理解力。总体而言，我们想『教』会计算机去认识一类图，会有下面这样一些困难：
- 视角不同，每个事物旋转或者侧视最后的构图都完全不同
- 尺寸大小不统一，相同内容的图片也可大可小
- 变形，很多东西处于特殊的情形下，会有特殊的摆放和形状
- 光影等干扰/幻象
- 背景干扰
- 同类内的差异(比如椅子有靠椅/吧椅/餐椅/躺椅…)

![图像面临的挑战](http://cs231n.github.io/assets/challenges.jpeg)

### 1.2 识别的途径


首先，大家想想就知道，这个算法并不像『对一个数组排序』或者『求有向图的最短路径』，我们没办法提前制定一个流程和规则去解决。定义『猫』这种动物本身就是一件很难的事情了，更不要说去定义一只猫在图像上的固定表现形式。所以我们寄希望于机器学习，使用
```
『Data-driven
 approach/数据驱动法』
```
来做做尝试。简单说来，就是对于每个类别，我们都找一定量的图片数据，『喂』给计算机，让它自己去『学习和总结』每一类的图片的特点。对了，这个过程和小盆友学习新鲜事物是一样一样的。『喂』给计算机学习的图片数据就和下图的猫/狗/杯子/帽子一样：

![Data-driven approach](http://cs231n.github.io/assets/trainset.jpg)

### 1.3 机器学习解决图像分类的流程/Pipeline


整体的流程和普通机器学习完全一致，简单说来，也就下面三步：
- 输入：我们的给定K个类别的N张图片，作为计算机学习的训练集
- 学习：让计算机逐张图片地『观察』和『学习』
- 评估：就像我们上学学了东西要考试检测一样，我们也得考考计算机学得如何，于是我们给定一些计算机不知道类别的图片让它判别，然后再比对我们已知的正确答案。

## 2. 最近邻分类器(Nearest Neighbor Classifier)


先从简单的方法开始说，先提一提最近邻分类器/Nearest Neighbor Classifier，不过事先申明，它和深度学习中的卷积神经网/Convolutional Neural Networks其实一点关系都没有，我们只是从基础到前沿一点一点推进，最近邻是图像识别一个相对简单和基础的实现方式。

### 2.1 CIFAR-10

[CIFAR-10](http://www.cs.toronto.edu/~kriz/cifar.html)是一个非常常用的图像分类数据集。数据集包含60000张32*32像素的小图片，每张图片都有一个类别标注(总共有10类)，分成了50000张的训练集和10000张的测试集。如下是一些图片示例：

![CIFAR-10例子](http://cs231n.github.io/assets/nn.jpg)


上图中左边是十个类别和对应的一些示例图片，右边是给定一张图片后，根据像素距离计算出来的，最近的10张图片。

### 2.2 基于最近邻的简单图像类别判定


假如现在用CIFAR-10数据集做训练集，判断一张未知的图片属于CIFAR-10中的哪一类，应该怎么做呢。一个很直观的想法就是，既然我们现在有每个像素点的值，那我们就根据输入图片的这些值，计算和训练集中的图片距离，找最近的图片的类别，作为它的类别，不就行了吗。


恩，想法很直接，这就是『最近邻』的思想。偷偷说一句，这种直接的做法在图像识别中，其实效果并不是特别好。比如上图是按照这个思想找的最近邻，其实只有3个图片的最近邻是正确的类目。


即使这样，作为最基础的方法，还是得掌握，我们来简单实现一下吧。我们需要一个图像距离评定准则，比如最简单的方式就是，比对两个图像像素向量之间的l1距离(也叫曼哈顿距离/cityblock距离)，公式如下：








其实就是计算了所有像素点之间的差值，然后做了加法，直观的理解如下图：

![矩阵的l1距离](http://cs231n.github.io/assets/nneg.jpeg)


我们先把数据集读进内存：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#! /usr/bin/env python</span>
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;">#coding=utf-8</span>
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">import</span> os
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">import</span> sys
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">import</span> numpy <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">as</span> np

<span class="hljs-function" style="box-sizing: border-box;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">def</span> <span class="hljs-title" style="box-sizing: border-box;">load_CIFAR_batch</span><span class="hljs-params" style="color: rgb(102, 0, 102); box-sizing: border-box;">(filename)</span>:</span>
    <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"""
    cifar-10数据集是分batch存储的，这是载入单个batch

    @参数 filename: cifar文件名
    @r返回值: X, Y: cifar batch中的 data 和 labels
    """</span>

    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">with</span> open(filename, <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'r'</span>) <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">as</span> f:
        datadict=pickle.load(f)

        X=datadict[<span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'data'</span>]
        Y=datadict[<span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'labels'</span>]

        X=X.reshape(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">10000</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">3</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">32</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">32</span>).transpose(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>,<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">2</span>,<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">3</span>,<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>).astype(<span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"float"</span>)
        Y=np.array(Y)

        <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">return</span> X, Y


<span class="hljs-function" style="box-sizing: border-box;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">def</span> <span class="hljs-title" style="box-sizing: border-box;">load_CIFAR10</span><span class="hljs-params" style="color: rgb(102, 0, 102); box-sizing: border-box;">(ROOT)</span>:</span>
    <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"""
    读取载入整个 CIFAR-10 数据集

    @参数 ROOT: 根目录名
    @return: X_train, Y_train: 训练集 data 和 labels
             X_test, Y_test: 测试集 data 和 labels
    """</span>

    xs=[]
    ys=[]

    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">for</span> b <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">in</span> range(<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>,<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">6</span>):
        f=os.path.join(ROOT, <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"data_batch_%d"</span> % (b, ))
        X, Y=load_CIFAR_batch(f)
        xs.append(X)
        ys.append(Y)

    X_train=np.concatenate(xs)
    Y_train=np.concatenate(ys)

    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">del</span> X, Y

    X_test, Y_test=load_CIFAR_batch(os.path.join(ROOT, <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">"test_batch"</span>))

    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">return</span> X_train, Y_train, X_test, Y_test

<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 载入训练和测试数据集</span>
X_train, Y_train, X_test, Y_test = load_CIFAR10(<span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'data/cifar10/'</span>) 
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 把32*32*3的多维数组展平</span>
Xtr_rows = X_train.reshape(X_train.shape[<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>], <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">32</span> * <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">32</span> * <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">3</span>) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># Xtr_rows : 50000 x 3072</span>
Xte_rows = X_test.reshape(X_test.shape[<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>], <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">32</span> * <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">32</span> * <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">3</span>) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># Xte_rows : 10000 x 3072</span></code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li><li style="box-sizing: border-box; padding: 0px 5px;">44</li><li style="box-sizing: border-box; padding: 0px 5px;">45</li><li style="box-sizing: border-box; padding: 0px 5px;">46</li><li style="box-sizing: border-box; padding: 0px 5px;">47</li><li style="box-sizing: border-box; padding: 0px 5px;">48</li><li style="box-sizing: border-box; padding: 0px 5px;">49</li><li style="box-sizing: border-box; padding: 0px 5px;">50</li><li style="box-sizing: border-box; padding: 0px 5px;">51</li><li style="box-sizing: border-box; padding: 0px 5px;">52</li><li style="box-sizing: border-box; padding: 0px 5px;">53</li><li style="box-sizing: border-box; padding: 0px 5px;">54</li><li style="box-sizing: border-box; padding: 0px 5px;">55</li><li style="box-sizing: border-box; padding: 0px 5px;">56</li><li style="box-sizing: border-box; padding: 0px 5px;">57</li><li style="box-sizing: border-box; padding: 0px 5px;">58</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li><li style="box-sizing: border-box; padding: 0px 5px;">35</li><li style="box-sizing: border-box; padding: 0px 5px;">36</li><li style="box-sizing: border-box; padding: 0px 5px;">37</li><li style="box-sizing: border-box; padding: 0px 5px;">38</li><li style="box-sizing: border-box; padding: 0px 5px;">39</li><li style="box-sizing: border-box; padding: 0px 5px;">40</li><li style="box-sizing: border-box; padding: 0px 5px;">41</li><li style="box-sizing: border-box; padding: 0px 5px;">42</li><li style="box-sizing: border-box; padding: 0px 5px;">43</li><li style="box-sizing: border-box; padding: 0px 5px;">44</li><li style="box-sizing: border-box; padding: 0px 5px;">45</li><li style="box-sizing: border-box; padding: 0px 5px;">46</li><li style="box-sizing: border-box; padding: 0px 5px;">47</li><li style="box-sizing: border-box; padding: 0px 5px;">48</li><li style="box-sizing: border-box; padding: 0px 5px;">49</li><li style="box-sizing: border-box; padding: 0px 5px;">50</li><li style="box-sizing: border-box; padding: 0px 5px;">51</li><li style="box-sizing: border-box; padding: 0px 5px;">52</li><li style="box-sizing: border-box; padding: 0px 5px;">53</li><li style="box-sizing: border-box; padding: 0px 5px;">54</li><li style="box-sizing: border-box; padding: 0px 5px;">55</li><li style="box-sizing: border-box; padding: 0px 5px;">56</li><li style="box-sizing: border-box; padding: 0px 5px;">57</li><li style="box-sizing: border-box; padding: 0px 5px;">58</li></ul>

下面我们实现最近邻的思路：
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-class" style="box-sizing: border-box;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">class</span> <span class="hljs-title" style="box-sizing: border-box; color: rgb(102, 0, 102);">NearestNeighbor</span>:</span>
  <span class="hljs-function" style="box-sizing: border-box;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">def</span> <span class="hljs-title" style="box-sizing: border-box;">__init__</span><span class="hljs-params" style="color: rgb(102, 0, 102); box-sizing: border-box;">(self)</span>:</span>
    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">pass</span>

  <span class="hljs-function" style="box-sizing: border-box;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">def</span> <span class="hljs-title" style="box-sizing: border-box;">train</span><span class="hljs-params" style="color: rgb(102, 0, 102); box-sizing: border-box;">(self, X, y)</span>:</span>
    <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">""" 
    这个地方的训练其实就是把所有的已有图片读取进来 -_-||
    """</span>
    <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># the nearest neighbor classifier simply remembers all the training data</span>
    self.Xtr = X
    self.ytr = y

  <span class="hljs-function" style="box-sizing: border-box;"><span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">def</span> <span class="hljs-title" style="box-sizing: border-box;">predict</span><span class="hljs-params" style="color: rgb(102, 0, 102); box-sizing: border-box;">(self, X)</span>:</span>
    <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">""" 
    所谓的预测过程其实就是扫描所有训练集中的图片，计算距离，取最小的距离对应图片的类目
    """</span>
    num_test = X.shape[<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">0</span>]
    <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 要保证维度一致哦</span>
    Ypred = np.zeros(num_test, dtype = self.ytr.dtype)

    <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 把训练集扫一遍 -_-||</span>
    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">for</span> i <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">in</span> xrange(num_test):
      <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 计算l1距离，并找到最近的图片</span>
      distances = np.sum(np.abs(self.Xtr - X[i,:]), axis = <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>)
      min_index = np.argmin(distances) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 取最近图片的下标</span>
      Ypred[i] = self.ytr[min_index] <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 记录下label</span>

    <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">return</span> Ypred

nn = NearestNeighbor() <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 初始化一个最近邻对象</span>
nn.train(Xtr_rows, Y_train) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 训练...其实就是读取训练集</span>
Yte_predict = nn.predict(Xte_rows) <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 预测</span>
<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 比对标准答案，计算准确率</span>
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">print</span> <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'accuracy: %f'</span> % ( np.mean(Yte_predict == Y_test) )</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li><li style="box-sizing: border-box; padding: 0px 5px;">21</li><li style="box-sizing: border-box; padding: 0px 5px;">22</li><li style="box-sizing: border-box; padding: 0px 5px;">23</li><li style="box-sizing: border-box; padding: 0px 5px;">24</li><li style="box-sizing: border-box; padding: 0px 5px;">25</li><li style="box-sizing: border-box; padding: 0px 5px;">26</li><li style="box-sizing: border-box; padding: 0px 5px;">27</li><li style="box-sizing: border-box; padding: 0px 5px;">28</li><li style="box-sizing: border-box; padding: 0px 5px;">29</li><li style="box-sizing: border-box; padding: 0px 5px;">30</li><li style="box-sizing: border-box; padding: 0px 5px;">31</li><li style="box-sizing: border-box; padding: 0px 5px;">32</li><li style="box-sizing: border-box; padding: 0px 5px;">33</li><li style="box-sizing: border-box; padding: 0px 5px;">34</li></ul>

最近邻的思想在CIFAR上得到的准确度为38.6%，我们知道10各类别，我们随机猜测的话准确率差不多是1/10=10%，所以说还是有识别效果的，但是显然这距离人的识别准确率(94%)实在是低太多了，不那么实用。

### 2.3 关于最近邻的距离准则


我们这里用的距离准则是l1距离，实际上除掉l1距离，我们还有很多其他的距离准则。
- 比如说l2距离(也就是大家熟知的欧氏距离)的计算准则如下：






- 比如余弦距离计算准则如下：








更多的距离准则可以参见[scipy相关计算页面](http://docs.scipy.org/doc/scipy-0.16.0/reference/generated/scipy.spatial.distance.pdist.html).

## 3. K最近邻分类器(K Nearest Neighbor Classifier)


这是对最近邻的思想的一个调整。其实我们在使用最近邻分类器分类，扫描CIFAR训练集的时候，会发现，有时候不一定距离最近的和当前图片是同类，但是最近的一些里有很多和当前图片是同类。所以我们自然而然想到，把最近邻扩展为最近的N个临近点，然后统计一下这些点的类目分布，取最多的那个类目作为自己的类别。


恩，这就是KNN的思想。


KNN其实是一种特别常用的分类算法。但是有个问题，我们的K值应该取多少呢。换句话说，我们找多少邻居来投票，比较靠谱呢？

### 3.1 交叉验证与参数选择


在现在的场景下，假如我们确定使用KNN来完成图片类别识别问题。我们发现有一些参数是肯定会影响最后的识别结果的，比如：
- 距离的选择(l1,l2,cos等等)
- 近邻个数K的取值。


每组参数下其实都能产生一个新的model，所以这可以视为一个模型选择/model selection问题。而对于模型选择问题，最常用的办法就是在[交叉验证](https://en.wikipedia.org/wiki/Cross-validation_%28statistics%29)集上实验。


数据总量就那么多，如果我们在test data上做模型参数选择，又用它做效果评估，显然不是那么合理（因为我们的模型参数很有可能是在test data上过拟合的，不能很公正地评估结果）。所以我们通常会把训练数据分为两个部分，一大部分作为训练用，另外一部分就是所谓的cross validation数据集，用来进行模型参数选择的。比如说我们有50000训练图片，我们可以把它分为49000的训练集和1000的交叉验证集。
<code class="language-python hljs  has-numbering" style="display: block; padding: 0px; color: inherit; box-sizing: border-box; font-family: 'Source Code Pro', monospace;font-size:undefined; white-space: pre; border-radius: 0px; word-wrap: normal; background: transparent;"><span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 假定已经有Xtr_rows, Ytr, Xte_rows, Yte了，其中Xtr_rows为50000*3072 矩阵</span>
Xval_rows = Xtr_rows[:<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1000</span>, :] <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 构建1000的交叉验证集</span>
Yval = Ytr[:<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1000</span>]
Xtr_rows = Xtr_rows[<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1000</span>:, :] <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 保留49000的训练集</span>
Ytr = Ytr[<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1000</span>:]

<span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 设置一些k值，用于试验</span>
validation_accuracies = []
<span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">for</span> k <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">in</span> [<span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">1</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">3</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">5</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">7</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">10</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">20</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">50</span>, <span class="hljs-number" style="color: rgb(0, 102, 102); box-sizing: border-box;">100</span>]:

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 初始化对象</span>
  nn = NearestNeighbor()
  nn.train(Xtr_rows, Ytr)
  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 修改一下predict函数，接受 k 作为参数</span>
  Yval_predict = nn.predict(Xval_rows, k = k)
  acc = np.mean(Yval_predict == Yval)
  <span class="hljs-keyword" style="color: rgb(0, 0, 136); box-sizing: border-box;">print</span> <span class="hljs-string" style="color: rgb(0, 136, 0); box-sizing: border-box;">'accuracy: %f'</span> % (acc,)

  <span class="hljs-comment" style="color: rgb(136, 0, 0); box-sizing: border-box;"># 输出结果</span>
  validation_accuracies.append((k, acc))</code><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li></ul><ul class="pre-numbering" style="box-sizing: border-box; position: absolute; width: 50px; top: 0px; left: 0px; margin: 0px; padding: 6px 0px 40px; border-right-width: 1px; border-right-style: solid; border-right-color: rgb(221, 221, 221); list-style: none; text-align: right; background-color: rgb(238, 238, 238);"><li style="box-sizing: border-box; padding: 0px 5px;">1</li><li style="box-sizing: border-box; padding: 0px 5px;">2</li><li style="box-sizing: border-box; padding: 0px 5px;">3</li><li style="box-sizing: border-box; padding: 0px 5px;">4</li><li style="box-sizing: border-box; padding: 0px 5px;">5</li><li style="box-sizing: border-box; padding: 0px 5px;">6</li><li style="box-sizing: border-box; padding: 0px 5px;">7</li><li style="box-sizing: border-box; padding: 0px 5px;">8</li><li style="box-sizing: border-box; padding: 0px 5px;">9</li><li style="box-sizing: border-box; padding: 0px 5px;">10</li><li style="box-sizing: border-box; padding: 0px 5px;">11</li><li style="box-sizing: border-box; padding: 0px 5px;">12</li><li style="box-sizing: border-box; padding: 0px 5px;">13</li><li style="box-sizing: border-box; padding: 0px 5px;">14</li><li style="box-sizing: border-box; padding: 0px 5px;">15</li><li style="box-sizing: border-box; padding: 0px 5px;">16</li><li style="box-sizing: border-box; padding: 0px 5px;">17</li><li style="box-sizing: border-box; padding: 0px 5px;">18</li><li style="box-sizing: border-box; padding: 0px 5px;">19</li><li style="box-sizing: border-box; padding: 0px 5px;">20</li></ul>

这里提一个在很多地方会看到的概念，叫做k-fold cross-validation，意思其实就是把原始数据分成k份，轮流使用其中k-1份作为训练数据，而剩余的1份作为交叉验证数据(因此其实对于k-fold cross-validation我们会得到k个accuracy)。以下是5-fold cross-validation的一个示例：

![k-fold 交叉验证](http://cs231n.github.io/assets/crossval.jpeg)


以下是我们使用5-fold cross-validation，取不同的k值时，得到的accuracy曲线(补充一下，因为是5-fold cross-validation，所以在每个k值上有5个取值，我们取其均值作为此时的准确度)

![5-fold 交叉验证](http://cs231n.github.io/assets/cvplot.png)


可以看出大概在k=7左右有最佳的准确度。

### 3.2 最近邻方法的优缺点


K最近邻的优点大家都看出来了，思路非常简单清晰，而且完全不需要训练…不过也正因为如此，最后的predict过程非常耗时，因为要和全部训练集中的图片比对一遍。


实际应用中，我们其实更加关心实施predict所消耗的时间，如果有一个图像识别app返回结果要半小时一小时，你一定第一时间把它卸了。我们反倒不那么在乎训练时长，训练时间稍微长一点没关系，只要最后应用的时候识别速度快效果好，就很赞。后面会提到的深度神经网络就是这样，深度神经网络解决图像问题时训练是一个相对耗时间的过程，但是识别的过程非常快。


另外，不得不多说一句的是，优化计算K最近邻时间问题，实际上依旧到现在都是一个非常热门的问题。Approximate Nearest Neighbor (ANN)算法是牺牲掉一小部分的准确度，而提高很大程度的速度，能比较快地找到近似的K最近邻，现在已经有很多这样的库，比如说[FLANN](http://www.cs.ubc.ca/research/flann/).


最后，我们用一张图来说明一下，用图片像素级别的距离来实现图像类别识别，有其不足之处，我们用一个叫做[t-SNE](http://lvdmaaten.github.io/tsne/)的技术把CIFAR-10的所有图片按两个维度平铺出来，靠得越近的图片表示其像素级别的距离越接近。然而我们瞄一眼，发现，其实靠得最近的并不一定是同类别的。

![像素级别图像距离排列](http://cs231n.github.io/assets/pixels_embed_cifar10.jpg)


其实观察一下，你就会发现，像素级别接近的图片，在整张图的颜色分布上，有很大的共性，然而在图像内容上，有时候也只能无奈地呵呵嗒，毕竟颜色分布相同的不同物体也是非常多的。

## 参考资料与原文

[cs231n 图像分类与KNN](http://cs231n.github.io/classification/)



[](http://blog.csdn.net/han_xiaoyang/article/details/49949535#)[](http://blog.csdn.net/han_xiaoyang/article/details/49949535#)[](http://blog.csdn.net/han_xiaoyang/article/details/49949535#)[](http://blog.csdn.net/han_xiaoyang/article/details/49949535#)[](http://blog.csdn.net/han_xiaoyang/article/details/49949535#)[](http://blog.csdn.net/han_xiaoyang/article/details/49949535#)





