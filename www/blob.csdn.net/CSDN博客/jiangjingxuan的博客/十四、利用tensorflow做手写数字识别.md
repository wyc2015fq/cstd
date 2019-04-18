# 十四、利用tensorflow做手写数字识别 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:25:36[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2804












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_255_default_big.jpeg)



模式识别领域应用机器学习的场景非常多，手写识别就是其中一种，最简单的数字识别是一个多类分类问题，我们借这个多类分类问题来介绍一下google最新开源的tensorflow框架，后面深度学习的内容都会基于tensorflow来介绍和演示

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 什么是tensorflow

tensor意思是张量，flow是流。

张量原本是力学里的术语，表示弹性介质中各点应力状态。在数学中，张量表示的是一种广义的“数量”，0阶张量就是标量(比如：0、1、2……)，1阶张量就是向量(比如：(1,3,4))，2阶张量就是矩阵，本来这几种形式是不相关的，但是都归为张量，是因为他们同时满足一些特性：1）可以用坐标系表示；2）在坐标变换中遵守同样的变换法则；3）有着相同的基本运算(如：加、减、乘、除、缩放、点积、对称……)

那么tensorflow可以理解为通过“流”的形式来处理张量的一种框架，是由google开发并开源，已经应用于google大脑项目开发



## tensorflow安装
`sudo pip install https://storage.googleapis.com/tensorflow/mac/tensorflow-0.9.0-py2-none-any.whl`
不同平台找对应的whl包

可能遇到的问题：

发现无法import tensorflow，问题在于protobuf版本不对，必须先卸载掉，再安装tensorflow，这样会自动安装3.0版本的protobuf

```bash
sudo pip uninstall protobuf
sudo brew remove protobuf260
sudo pip install --upgrade https://storage.googleapis.com/tensorflow/mac/tensorflow-0.9.0-py2-none-any.whl
```



## 手写数字数据集获取

在[http://yann.lecun.com/exdb/mnist/](http://yann.lecun.com/exdb/mnist/)可以下载手写数据集，http://yann.lecun.com/exdb/mnist/train-images-idx3-ubyte.gz和http://yann.lecun.com/exdb/mnist/train-labels-idx1-ubyte.gz，下载解压后发现不是图片格式，而是自己特定的格式，为了说明这是什么样的数据，我写了一段程序来显示这些数字：

```cpp
/************************
 * author: SharEDITor
 * date:   2016-08-02
 * brief:  read MNIST data
 ************************/
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

unsigned char *lables = NULL;

/**
 * All the integers in the files are stored in the MSB first (high endian) format
 */
void copy_int(uint32_t *target, unsigned char *src)
{
    *(((unsigned char*)target)+0) = src[3];
    *(((unsigned char*)target)+1) = src[2];
    *(((unsigned char*)target)+2) = src[1];
    *(((unsigned char*)target)+3) = src[0];
}

int read_lables()
{
    FILE *fp = fopen("./train-labels-idx1-ubyte", "r");
    if (NULL == fp)
    {
        return -1;
    }
    unsigned char head[8];
    fread(head, sizeof(unsigned char), 8, fp);
    uint32_t magic_number = 0;
    uint32_t item_num = 0;
    copy_int(&magic_number, &head[0]);
    // magic number check
    assert(magic_number == 2049);
    copy_int(&item_num, &head[4]);

    uint64_t values_size = sizeof(unsigned char) * item_num;
    lables = (unsigned char*)malloc(values_size);
    fread(lables, sizeof(unsigned char), values_size, fp);

    fclose(fp);
    return 0;
}

int read_images()
{
    FILE *fp = fopen("./train-images-idx3-ubyte", "r");
    if (NULL == fp)
    {
        return -1;
    }
    unsigned char head[16];
    fread(head, sizeof(unsigned char), 16, fp);
    uint32_t magic_number = 0;
    uint32_t images_num = 0;
    uint32_t rows = 0;
    uint32_t cols = 0;
    copy_int(&magic_number, &head[0]);
    // magic number check
    assert(magic_number == 2051);
    copy_int(&images_num, &head[4]);
    copy_int(&rows, &head[8]);
    copy_int(&cols, &head[12]);

    uint64_t image_size = rows * cols;
    uint64_t values_size = sizeof(unsigned char) * images_num * rows * cols;
    unsigned char *values = (unsigned char*)malloc(values_size);
    fread(values, sizeof(unsigned char), values_size, fp);

    for (int image_index = 0; image_index < images_num; image_index++)
    {
        // print the label
        printf("=========================================  %d  ======================================\n", lables[image_index]);
        for (int row_index = 0; row_index < rows; row_index++)
        {
            for (int col_index = 0; col_index < cols; col_index++)
            {
                // print the pixels of image
                printf("%3d", values[image_index*image_size+row_index*cols+col_index]);
            }
            printf("\n");
        }
        printf("\n");
    }

    free(values);
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[])
{
    if (-1 == read_lables())
    {
        return -1;
    }
    if (-1 == read_images())
    {
        return -1;
    }
    return 0;
}
```

下载并解压出数据集文件train-images-idx3-ubyte和train-labels-idx1-ubyte放到源代码所在目录后，编译并执行：

```bash
gcc -o read_images read_images.c
./read_images
```



展示出来的效果如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/afdcfd5c157985d8b0a6b34f07ded24f2db062a0.png)



一共有60000个图片，从代码可以看出数据集里存储的实际就是图片的像素

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址



## softmax模型

我们在《[机器学习教程 十三-用scikit-learn做逻辑回归](http://www.shareditor.com/blogshow/?blogId=93)》中介绍了逻辑回归模型。逻辑回归是用于解决二类分类问题(使用sigmoid函数)，而softmax模型是逻辑回归模型的扩展，用来解决多类分类问题。

softmax意为柔和的最大值，也就是如果某个zj大于其他z，那么这个映射的分量就逼近于1，其他的分量就逼近于0，从而将其归为此分类，多个分量对应的就是多分类，数学形式和sigmoid不同，如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/d6aa64b86d625770a3a7492c6d285cbae85e6ad4.png)

它的特点是，所有的softmax加和为1，其实它表示的是一种概率，即x属于某个分类的概率。

在做样本训练时，这里的xi计算方法是：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/d04deffd60c4527f0b643443148154cbf7819d7c.png)

其中W是样本特征的权重，xj是样本的特征值，bi是偏置量。

详细来说就是：假设某个模型训练中我们设计两个特征，他们的值分别是f1和f2，他们对于第i类的权重分别是0.2和0.8，偏置量是1，那么

xi=f1*0.2+f2*0.8+1

如果所有的类别都计算出x的值，如果是一个训练好的模型，那么应该是所属的那个类别对应的softmax值最大

softmax回归算法也正是基于这个原理，通过大量样本来训练这里的W和b，从而用于分类的



## tensorflow的优点

tensorflow会使用外部语言计算复杂运算来提高效率，但是不同语言之间的切换和不同计算资源之间的数据传输耗费很多资源，因此它使用图来描述一系列计算操作，然后一起传给外部计算，最后结果只传回一次，这样传输代价最低，计算效率最高

举个例子：

```python
import tensorflow as tf
x = tf.placeholder(tf.float32, [None, 784])
```

这里的x不是一个实际的x，而是一个占位符，也就是一个描述，描述成了二维浮点型，后面需要用实际的值来填充，这就类似于printf("%d", 10)中的占位符%d，其中第一维是None表示可无限扩张，第二维是784个浮点型变量

如果想定义可修改的张量，可以这样定义：

```python
W = tf.Variable(tf.zeros([784,10]))
b = tf.Variable(tf.zeros([10]))
```

其中W的维度是[784, 10]，b的形状是[10]

有了这三个变量，我们可以定义我们的softmax模型：

```python
y = tf.nn.softmax(tf.matmul(x,W) + b)
```

这虽然定义，但是没有真正的进行计算，因为这只是先用图来描述计算操作

其中matmul是矩阵乘法，因为x的维度是[None, 784]，W的维度是[784, 10]，所以矩阵乘法得出的是[None, 10]，这样可以和向量b相加

softmax函数会计算出10维分量的概率值，也就是y的形状是[10]



## 数字识别模型实现

基于上面定义的x、W、b，和我们定义的模型：

```python
y = tf.nn.softmax(tf.matmul(x,W) + b)
```

我们需要定义我们的目标函数，我们以交叉熵(衡量预测用于描述真相的低效性)为目标函数，让它达到最小：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/12c891ad70fde8bcc7f71266e7429b596a9d4f05.png)

其中y'是实际分布，y是预测的分布，即：

```python
y_ = tf.placeholder("float", [None,10])
cross_entropy = -tf.reduce_sum(y_*tf.log(y))
```

利用梯度下降法优化上面定义的Variable：

```python
train_step = tf.train.GradientDescentOptimizer(0.01).minimize(cross_entropy)
```

其中0.01是学习速率，也就是每次对变量做多大的修正



按照上面的思路，最终实现的代码digital_recognition.py如下：



```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from tensorflow.examples.tutorials.mnist import input_data
import tensorflow as tf

flags = tf.app.flags
FLAGS = flags.FLAGS
flags.DEFINE_string('data_dir', './', 'Directory for storing data')

mnist = input_data.read_data_sets(FLAGS.data_dir, one_hot=True)


x = tf.placeholder(tf.float32, [None, 784])
W = tf.Variable(tf.zeros([784,10]))
b = tf.Variable(tf.zeros([10]))
y = tf.nn.softmax(tf.matmul(x,W) + b)
y_ = tf.placeholder("float", [None,10])
cross_entropy = -tf.reduce_sum(y_*tf.log(y))
train_step = tf.train.GradientDescentOptimizer(0.01).minimize(cross_entropy)

init = tf.initialize_all_variables()
sess = tf.InteractiveSession()
sess.run(init)
for i in range(1000):
    batch_xs, batch_ys = mnist.train.next_batch(100)
    sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})

correct_prediction = tf.equal(tf.argmax(y, 1), tf.argmax(y_, 1))
accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
print(accuracy.eval({x: mnist.test.images, y_: mnist.test.labels}))
```

运行效果如下：

```
[root@mymac $] python digital_recognition.py
Extracting ./train-images-idx3-ubyte.gz
Extracting ./train-labels-idx1-ubyte.gz
Extracting ./t10k-images-idx3-ubyte.gz
Extracting ./t10k-labels-idx1-ubyte.gz
0.9039
```



解释一下

```python
flags.DEFINE_string('data_dir', './', 'Directory for storing data')
```

表示我们用当前目录作为训练数据的存储目录，如果我们没有提前下好训练数据和测试数据，程序会自动帮我们下载到./

```python
mnist = input_data.read_data_sets(FLAGS.data_dir, one_hot=True)
```

这句直接用库里帮我们实现好的读取训练数据的方法，无需自行解析

```python
for i in range(1000):
    batch_xs, batch_ys = mnist.train.next_batch(100)
    sess.run(train_step, feed_dict={x: batch_xs, y_: batch_ys})
```

这几行表示我们循环1000次，每次从训练样本里选取100个样本来做训练，这样我们可以修改配置来观察运行速度

最后几行打印预测精度，当调整循环次数时可以发现总训练的样本数越多，精度就越高




