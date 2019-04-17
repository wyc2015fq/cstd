# tensorflow学习笔记(十七):name&variable scope - Keith - CSDN博客





2016年11月08日 13:49:58[ke1th](https://me.csdn.net/u012436149)阅读数：36168
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









水平有限,如有错误,请指正!

**在tensorflow中,有两个scope, 一个是name_scope一个是variable_scope,这两个scope到底有什么区别呢?**

先看第一个程序:

```python
with tf.name_scope("hello") as name_scope:
    arr1 = tf.get_variable("arr1", shape=[2,10],dtype=tf.float32)

    print name_scope
    print arr1.name
    print "scope_name:%s " % tf.get_variable_scope().original_name_scope
```

输出为:

hello/

arr1:0

scope_name:
可以看出:
- tf.name_scope() 返回的是 一个string,“hello/”
- 在name_scope使用 `get_variable()` 中定义的 variable 的 name 并没有 "hello/"前缀
- tf.get_variable_scope()的original_name_scope 是空

第二个程序:

```python
with tf.variable_scope("hello") as variable_scope:
    arr1 = tf.get_variable("arr1", shape=[2, 10], dtype=tf.float32)

    print variable_scope
    print variable_scope.name #打印出变量空间名字
    print arr1.name
    print tf.get_variable_scope().original_name_scope
    #tf.get_variable_scope() 获取的就是variable_scope

    with tf.variable_scope("xixi") as v_scope2:
        print tf.get_variable_scope().original_name_scope
        #tf.get_variable_scope() 获取的就是v _scope2
```

输出为:

```
<tensorflow.python.ops.variable_scope.VariableScope object at 0x7fbc09959210>
hello
hello/arr1:0
hello/
hello/xixi/
```

可以看出:
- tf.variable_scope() 返回的是一个 `VariableScope` 对象
- variable_scope使用 `get_variable` 定义的variable 的name加上了"hello/"前缀
- tf.get_variable_scope()的original_name_scope 是 嵌套后的scope name

第三个程序:

```python
with tf.name_scope("name1"):
    with tf.variable_scope("var1"):
        w = tf.get_variable("w",shape=[2])
        res = tf.add(w,[3])

print w.name
print res.name
# 输出
var1/w:0
name1/var1/Add:0
```

可以看出：
- `variable scope`和`name scope`都会给`op`的`name`加上前缀
- 这实际上是因为 创建 `variable_scope` 时内部会创建一个同名的 `name_scope`


**对比三个个程序可以看出:**
- `name_scope` 返回的是 string, 而 `variable_scope` 返回的是对象. 这也可以感觉到, `variable_scope` 能干的事情比 `name_scope` 要多.
- name_scope对 get_variable()创建的变量 的名字不会有任何影响,而创建的`op`会被加上前缀.
- tf.get_variable_scope() 返回的只是 variable_scope,不管 name_scope. 所以以后我们在使用tf.get_variable_scope().reuse_variables() 时可以无视name_scope

## 其它

```python
with tf.name_scope("scope1") as scope1:
	with tf.name_scope("scope2") as scope2:
		print scope2
#输出：scope1/scope2/
```

```python
import tensorflow as tf
with tf.variable_scope("scope1") as scope1:
	with tf.variable_scope("scope2") as scope2:
		print scope2.name
#输出:scope1/scope2
```

## name_scope可以用来干什么

典型的 TensorFlow 可以有数以千计的节点，如此多而难以一下全部看到，甚至无法使用标准图表工具来展示。为简单起见，我们为`op/tensor`名划定范围，并且可视化把该信息用于在图表中的节点上定义一个层级。默认情况下， 只有顶层节点会显示。下面这个例子使用tf.name_scope在hidden命名域下定义了三个操作:

```python
import tensorflow as tf

with tf.name_scope('hidden') as scope:
  a = tf.constant(5, name='alpha')
  W = tf.Variable(tf.random_uniform([1, 2], -1.0, 1.0), name='weights')
  b = tf.Variable(tf.zeros([1]), name='biases')
  print a.name
  print W.name
  print b.name
```

结果是得到了下面三个操作名:

hidden/alpha

hidden/weights

hidden/biases
**name_scope 是给op_name加前缀, variable_scope是给get_variable()创建的变量的名字加前缀。**
**tf.variable_scope有时也会处理命名冲突**

```python
import tensorflow as tf
def test(name=None):
    with tf.variable_scope(name, default_name="scope") as scope:
        w = tf.get_variable("w", shape=[2, 10])

test()
test()
ws = tf.trainable_variables()
for w in ws:
    print(w.name)

#scope/w:0
#scope_1/w:0
#可以看出，如果只是使用default_name这个属性来创建variable_scope
#的时候，会通过 处理variable_scope的名字 以处理命名冲突
```

## 其它
- tf.name_scope(None) 有清除name scope的作用

```python
import tensorflow as tf
with tf.name_scope("hehe"):
    w1 = tf.Variable(1.0)
    with tf.name_scope(None):
        w2 = tf.Variable(2.0)
print(w1.name)
print(w2.name)

#hehe/Variable:0
#Variable:0
```

## variable_scope可以用来干什么

variable_scope 用来管理 variable 详见[variable_scope](http://blog.csdn.net/u012436149/article/details/53018924)

## 总结

简单来看
- 使用`tf.Variable()`的时候，`tf.name_scope()`和`tf.variable_scope()` 都会给 `Variable` 和 `op` 的 `name`属性加上前缀。
- 使用`tf.get_variable()`的时候，`tf.name_scope()`就不会给 `tf.get_variable()`创建出来的`Variable`加前缀。但是 `tf.Variable()` 创建出来的就会受到 `name_scope` 的影响.





