# 三十六、深入理解tensorflow的session和graph - jiangjingxuan的博客 - CSDN博客





2017年01月25日 14:01:07[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：10936












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_451_default_big.png)



tensorflow作为一个基于图结构的深度学习框架，内部通过session实现图和计算内核的交互，那么这个图是什么样的结构，session的工作原理又是什么样的呢？我们通过几段代码来深入理解一下

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## tensorflow中的基本数学运算用法

```
import tensorflow as tf

sess = tf.Session()

a = tf.placeholder("float")
b = tf.placeholder("float")
c = tf.constant(6.0)
d = tf.mul(a, b)
y = tf.mul(d, c)
print sess.run(y, feed_dict={a: 3, b: 3})

A = [[1.1,2.3],[3.4,4.1]]
Y = tf.matrix_inverse(A)
print sess.run(Y)
sess.close()
```



主要数字运算还包括：

```cpp
tf.add
tf.sub
tf.mul
tf.div
tf.mod
tf.abs
tf.neg
tf.sign
tf.inv
tf.square
tf.round
tf.sqrt
tf.pow
tf.exp
tf.log
tf.maximum
tf.minimum
tf.cos
tf.sin
```

主要矩阵运算还包括：

```
tf.diag生成对角阵
tf.transpose
tf.matmul
tf.matrix_determinant计算行列式的值
tf.matrix_inverse计算矩阵的逆
```



## 插播小甜点：tensorboard使用

tensorflow因为代码执行过程是先构建图，然后在执行，所以对中间过程的调试不太方便，所以提供了一个tensorboard工具来便于调试，用法如下：

在训练时会提示写入事件文件到哪个目录(比如：/tmp/tflearn_logs/11U8M4/)

执行如下命令并打开http://192.168.1.101:6006就能看到tensorboard的界面
`tensorboard --logdir=/tmp/tflearn_logs/11U8M4/`


## 什么是Graph和Session

为了步入正题，我们通过一段代码来展示Graph和Session的使用

```python
import tensorflow as tf

with tf.Graph().as_default() as g:
    with g.name_scope("myscope") as scope: # 有了这个scope，下面的op的name都是类似myscope/Placeholder这样的前缀
        sess = tf.Session(target='', graph = g, config=None) # target表示要连接的tf执行引擎
        print "graph version:", g.version # 0
        a = tf.placeholder("float")
        print a.op # 输出整个operation信息，跟下面g.get_operations返回结果一样
        print "graph version:", g.version # 1
        b = tf.placeholder("float")
        print "graph version:", g.version # 2
        c = tf.placeholder("float")
        print "graph version:", g.version # 3
        y1 = tf.mul(a, b) # 也可以写成a * b
        print "graph version:", g.version # 4
        y2 = tf.mul(y1, c) # 也可以写成y1 * c
        print "graph version:", g.version # 5
        operations = g.get_operations()
        for (i, op) in enumerate(operations):
            print "============ operation", i+1, "==========="
            print op # 一个结构，包括：name、op、attr、input等,不同op不一样
        assert y1.graph is g
        assert sess.graph is g
        print "================ graph object address ================"
        print sess.graph
        print "================ graph define ================"
        print sess.graph_def
        print "================ sess str ================"
        print sess.sess_str
        print sess.run(y1, feed_dict={a: 3, b: 3}) # 9.0 feed_dictgraph中的元素和值的映射
        print sess.run(fetches=[b,y1], feed_dict={a: 3, b: 3}, options=None, run_metadata=None) # 传入的feches和返回值的shape相同
        print sess.run({'ret_name':y1}, feed_dict={a: 3, b: 3}) # {'ret_name': 9.0} 传入的feches和返回值的shape相同

        assert tf.get_default_session() is not sess
        with sess.as_default(): # 把sess作为默认的session，那么tf.get_default_session就是sess, 否则不是
            assert tf.get_default_session() is sess

        h = sess.partial_run_setup([y1, y2], [a, b, c]) # 分阶段运行，参数指明了feches和feed_dict列表
        res = sess.partial_run(h, y1, feed_dict={a: 3, b: 4}) # 12 运行第一阶段
        res = sess.partial_run(h, y2, feed_dict={c: res}) # 144.0 运行第二阶段，其中使用了第一阶段的执行结果
        print "partial_run res:", res
        sess.close()
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

输出如下：

```
graph version: 0
name: "myscope/Placeholder"
op: "Placeholder"
attr {
  key: "dtype"
  value {
    type: DT_FLOAT
  }
}
attr {
  key: "shape"
  value {
    shape {
    }
  }
}

graph version: 1
graph version: 2
graph version: 3
graph version: 4
graph version: 5
============ operation 1 ===========
name: "myscope/Placeholder"
op: "Placeholder"
attr {
  key: "dtype"
  value {
    type: DT_FLOAT
  }
}
attr {
  key: "shape"
  value {
    shape {
    }
  }
}

============ operation 2 ===========
name: "myscope/Placeholder_1"
op: "Placeholder"
attr {
  key: "dtype"
  value {
    type: DT_FLOAT
  }
}
attr {
  key: "shape"
  value {
    shape {
    }
  }
}

============ operation 3 ===========
name: "myscope/Placeholder_2"
op: "Placeholder"
attr {
  key: "dtype"
  value {
    type: DT_FLOAT
  }
}
attr {
  key: "shape"
  value {
    shape {
    }
  }
}

============ operation 4 ===========
name: "myscope/Mul"
op: "Mul"
input: "myscope/Placeholder"
input: "myscope/Placeholder_1"
attr {
  key: "T"
  value {
    type: DT_FLOAT
  }
}

============ operation 5 ===========
name: "myscope/Mul_1"
op: "Mul"
input: "myscope/Mul"
input: "myscope/Placeholder_2"
attr {
  key: "T"
  value {
    type: DT_FLOAT
  }
}

================ graph object address ================
<tensorflow.python.framework.ops.Graph object at 0x1138702d0>
================ graph define ================
node {
  name: "myscope/Placeholder"
  op: "Placeholder"
  attr {
    key: "dtype"
    value {
      type: DT_FLOAT
    }
  }
  attr {
    key: "shape"
    value {
      shape {
      }
    }
  }
}
node {
  name: "myscope/Placeholder_1"
  op: "Placeholder"
  attr {
    key: "dtype"
    value {
      type: DT_FLOAT
    }
  }
  attr {
    key: "shape"
    value {
      shape {
      }
    }
  }
}
node {
  name: "myscope/Placeholder_2"
  op: "Placeholder"
  attr {
    key: "dtype"
    value {
      type: DT_FLOAT
    }
  }
  attr {
    key: "shape"
    value {
      shape {
      }
    }
  }
}
node {
  name: "myscope/Mul"
  op: "Mul"
  input: "myscope/Placeholder"
  input: "myscope/Placeholder_1"
  attr {
    key: "T"
    value {
      type: DT_FLOAT
    }
  }
}
node {
  name: "myscope/Mul_1"
  op: "Mul"
  input: "myscope/Mul"
  input: "myscope/Placeholder_2"
  attr {
    key: "T"
    value {
      type: DT_FLOAT
    }
  }
}
versions {
  producer: 15
}

================ sess str ================

9.0
[array(3.0, dtype=float32), 9.0]
{'ret_name': 9.0}
partial_run res: 144.0
```



## tensorflow的Session是如何工作的

Session是Graph和执行者之间的媒介，Session.run()实际上将graph、fetches、feed_dict序列化到字节数组中，并调用tf_session.TF_Run（参见/usr/local/lib/python2.7/site-packages/tensorflow/python/client/session.py）

而这里的tf_session.TF_Run实际上调用了动态链接库_pywrap_tensorflow.so中实现的_pywrap_tensorflow.TF_Run接口(参见/usr/local/lib/python2.7/site-packages/tensorflow/python/pywrap_tensorflow.py)，这个动态链接库是tensorflow提供的诸多语言接口中python语言的接口

事实上这里的_pywrap_tensorflow.so和pywrap_tensorflow.py是通过SWIG工具自动生成，大家都知道tensorflow核心语言是c语言，这里是通过SWIG生成了各种脚本语言的接口




