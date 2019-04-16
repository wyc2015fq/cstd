# TensorFlow中数据的feed与fetch - 关注微信公众号【OpenCV学堂】 - CSDN博客





2018年03月30日 16:42:29[gloomyfish](https://me.csdn.net/jia20003)阅读数：3506








# TensorFlow中数据的feed与fetch

## 一：占位符(placeholder)与feed

当我们构建一个模型的时候，有时候我们需要在运行时候输入一些初始数据，这个时候定义模型数据输入在tensorflow中就是用placeholder（占位符）来完成。它的定义如下：

```python
def placeholder(dtype, shape=None, name=None):
```

其中dtype表示数据类型，shape表示维度，name表示名称。它支持单个数值与任意维度的数组输入。

### 1. 单个数值占位符定义

```
a = tf.placeholder(tf.float32)
b = tf.placeholder(tf.float32)
c = tf.add(a, b)
```

当我们需要执行得到c的运行结果时候我们就需要在会话运行时候，通过feed来插入a与b对应的值，代码演示如下：

```
with tf.Session() as sess:
    result = sess.run(c, feed_dict={a:3, b:4})
    print(result)
```

其中 feed_dict就是完成了feed数据功能，feed中文有喂饭的意思，这里还是很形象的，对定义的模型来说，数据就是最好的食物，所以就通过feed_dict来实现。

### 2. 多维数据

同样对于模型需要多维数据的情况下通过feed一样可以完成，定义二维数据的占位符，然后相加，代码如下：

```
_x = tf.placeholder(shape=[None, 2], dtype=tf.float32, name="x")
_y = tf.placeholder(shape=[None, 2], dtype=tf.float32, name="y")
z = tf.add(_x, _y);
```

运行时候需要feed二维数组，实现如下：

```
with tf.Session() as sess:
    result = sess.run(z, feed_dict={_x:[[3, 4], [1, 2]], _y:[[8, 8],[9, 9]]})
    print(result)
```

## 二：fetch用法

会话运行完成之后，如果我们想查看会话运行的结果，就需要使用fetch来实现，feed，fetch同样可以fetch单个或者多个值。 

1. fetch单个值 

矩阵a与b相乘之后输出结果，通过会话运行接受到值c_res这个就是fetch单个值，fetch这个单词在数据库编程中比较常见，这里称为fetch也比较形象。代码演示如下：
```
import tensorflow as tf

a = tf.Variable(tf.random_normal([3, 3], stddev=3.0), dtype=tf.float32)
b = tf.Variable(tf.random_normal([3, 3], stddev=3.0), dtype=tf.float32)
c = tf.matmul(a, b);
init = tf.global_variables_initializer()

with tf.Session() as sess:
    sess.run(init)
    c_res = sess.run(c)
    print(c_res)
```
- fetch多个值 

还是以feed中代码为例，我们把feed与fetch整合在一起，实现feed与fetch多个值，代码演示如下：

```
import tensorflow as tf

_x = tf.placeholder(shape=[None, 2], dtype=tf.float32, name="x")
_y = tf.placeholder(shape=[None, 2], dtype=tf.float32, name="y")
z = tf.add(_x, _y);
data = tf.random_normal([2, 2], stddev=5.0)
Y = tf.add(data, z)

with tf.Session() as sess:

    z_res, Y_res = sess.run((z, Y), feed_dict={_x:[[3, 4], [1, 2]], _y:[[8, 8],[9, 9]]})
    print(z_res)
    print(Y_res)
```

上述代码我们就fetch了两个值，这个就是feed与fetch的基本用法。下面我们就集合图像来通过feed与fetch实现一些图像ROI截取操作。代码演示如下：

```
import tensorflow as tf
import cv2 as cv

src = cv.imread("D:/javaopencv/test.png")
cv.imshow("input", src)
_image = tf.placeholder(shape=[None, None, 3], dtype=tf.uint8, name="image")
roi_image = tf.slice(_image, [40, 130, 0], [180, 180, -1])

with tf.Session() as sess:
    slice = sess.run(roi_image, feed_dict={_image:src})
    print(slice.shape)
    cv.imshow("roi", slice)
    cv.waitKey(0)
    cv.destroyAllWindows()
```

原图 
![这里写图片描述](https://img-blog.csdn.net/20180330164021296?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYTIwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

ROI图像 
![这里写图片描述](https://img-blog.csdn.net/20180330164030761?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYTIwMDAz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**欢迎关注本博客以及微信公众号【OpenCV学堂】获取第一手资料！**





