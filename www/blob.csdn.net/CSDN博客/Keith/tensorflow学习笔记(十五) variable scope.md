# tensorflow学习笔记(十五): variable scope - Keith - CSDN博客





2016年11月03日 10:25:10[ke1th](https://me.csdn.net/u012436149)阅读数：25304
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# variable scope

**tensorflow 为了更好的管理变量,提供了variable scope机制**
**官方解释:**

Variable scope object to carry defaults to provide to get_variable.
Many of the arguments we need for get_variable in a variable store are most easily handled with a context. This object is used for the defaults.

Attributes:
- name: name of the current scope, used as prefix in get_variable.
- initializer: 传给get_variable的默认initializer.如果get_variable的时候指定了initializer,那么将覆盖这个默认的initializer.
- regularizer: 传给get_variable的默认regulizer.
- reuse: Boolean or None, setting the reuse in get_variable.
- caching_device: string, callable, or None: the caching device passed to get_variable.
- partitioner: callable or None: the partitioner passed to get_variable.
- custom_getter: default custom getter passed to get_variable.
- name_scope: The name passed to tf.name_scope.
- dtype: default type passed to get_variable (defaults to DT_FLOAT).

`regularizer`参数的作用是给在本`variable_scope`下创建的`weights`加上正则项.这样我们就可以不同`variable_scope`下的参数加不同的正则项了.

**可以看出,用variable scope管理get_varibale是很方便的**

## 如何确定 get_variable 的 prefixed name

首先, variable scope是可以嵌套的:

```python
with variable_scope.variable_scope("tet1"):
    var3 = tf.get_variable("var3",shape=[2],dtype=tf.float32)
    print var3.name
    with variable_scope.variable_scope("tet2"):
        var4 = tf.get_variable("var4",shape=[2],dtype=tf.float32)
        print var4.name
#输出为****************
#tet1/var3:0
#tet1/tet2/var4:0
#*********************
```

get_varibale.name 以创建变量的 `scope` 作为名字的prefix

```python
def te2():
    with variable_scope.variable_scope("te2"):
        var2 = tf.get_variable("var2",shape=[2], dtype=tf.float32)
        print var2.name
        def te1():
            with variable_scope.variable_scope("te1"):
                var1 = tf.get_variable("var1", shape=[2], dtype=tf.float32)
            return var1
        return te1() #在scope te2 内调用的
res = te2()
print res.name
#输出*********************
#te2/var2:0
#te2/te1/var1:0
#************************
```

观察和上个程序的不同

```python
def te2():
    with variable_scope.variable_scope("te2"):
        var2 = tf.get_variable("var2",shape=[2], dtype=tf.float32)
        print var2.name
        def te1():
            with variable_scope.variable_scope("te1"):
                var1 = tf.get_variable("var1", shape=[2], dtype=tf.float32)
            return var1
    return te1()  #在scope te2外面调用的
res = te2()
print res.name
#输出*********************
#te2/var2:0
#te1/var1:0
#************************
```

**还有需要注意一点的是`tf.variable_scope("name")` 与 `tf.variable_scope(scope)`的区别，看下面代码**

代码1

```python
import tensorflow as tf
with tf.variable_scope("scope"):
    tf.get_variable("w",shape=[1])#这个变量的name是 scope/w
    with tf.variable_scope("scope"):
        tf.get_variable("w", shape=[1]) #这个变量的name是 scope/scope/w
# 这两个变量的名字是不一样的，所以不会产生冲突
```

代码2

```python
import tensorflow as tf
with tf.variable_scope("yin"):
    tf.get_variable("w",shape=[1])
    scope = tf.get_variable_scope()#这个变量的name是 scope/w
    with tf.variable_scope(scope):#这种方式设置的scope，是用的外部的scope
        tf.get_variable("w", shape=[1])#这个变量的name也是 scope/w
# 两个变量的名字一样，会报错
```

## 共享变量

共享变量的前提是，变量的名字是一样的，变量的名字是由`变量名`和其`scope`前缀一起构成， `tf.get_variable_scope().reuse_variables()` 是允许共享当前`scope`下的所有变量。`reused variables`可以看同一个节点

```python
with tf.variable_scope("level1"):
    tf.get_variable("w",shape=[1])
    scope = tf.get_variable_scope()
    with tf.variable_scope("level2"):
        tf.get_variable("w", shape=[1])

with tf.variable_scope("level1", reuse=True): #即使嵌套的variable_scope也会被reuse
    tf.get_variable("w",shape=[1])
    scope = tf.get_variable_scope()
    with tf.variable_scope("level2"):
        tf.get_variable("w", shape=[1])
```

## 其它

`tf.get_variable_scope()` :获取当前scope 
`tf.get_variable_scope().reuse_variables()` 共享变量




