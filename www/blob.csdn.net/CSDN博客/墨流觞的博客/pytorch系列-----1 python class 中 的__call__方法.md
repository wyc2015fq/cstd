# pytorch系列-----1 python class 中 的__call__方法 - 墨流觞的博客 - CSDN博客





2018年11月05日 15:46:23[墨氲](https://me.csdn.net/dss_dssssd)阅读数：612
所属专栏：[pytorch系列教程](https://blog.csdn.net/column/details/29549.html)[python3 pytorch0.4系列教程](https://blog.csdn.net/column/details/29714.html)









鉴于pytorch在科学论文领域用的越来越多，想写一个系列文章，讲述pytorch的用法。

要学习pytorch，一个前提是 知道python calss中的`__call__`和`__init__`方法.

简单的说就是:
- `__init__`: 类的初始化函数，类似于c++的构造函数
- `__call___`: 使得类对象具有类似函数的功能。

`__init__`比较好理解，现在主要看一下 `__call__`的功能示例：

```python
class A():
    def __call__(self):
        print('i can be called like a function')


a = A()
a()
```

out:

> 
i can be called like a function


让我们在调用时传入参数如何？

```python
class A():
    def __call__(self, param):
        
        print('i can called like a function')
        print('掺入参数的类型是：', type(param))


a = A()


a('i')
```

out:

> 
i can called like a function

掺入参数的类型是： <class ‘str’>


发现对象a的表现完全类似一个**函数**。

那当然也可以在`__call__`里调用其他的函数啊,

在`__call__`函数中调用`forward`函数，并且返回调用的结果

```python
class A():
    def __call__(self, param):
        
        print('i can called like a function')
        print('传入参数的类型是：{}   值为： {}'.format(type(param), param))

        res = self.forward(param)
        return res

    def forward(self, input_):
        print('forward 函数被调用了')

        print('in  forward, 传入参数类型是：{}  值为: {}'.format( type(input_), input_))
        return input_

a = A()


input_param = a('i')
print("对象a传入的参数是：", input_param)
```

out:

> 
i can called like a function

传入参数的类型是：<class ‘str’>   值为： i

forward 函数被调用了

in  forward, 传入参数类型是：<class ‘str’>  值为: i

对象a传入的参数是： i
现在我们将初始化函数`__init__`也加上，来看一下：

在对象初始化时确定初始年龄，通过调用`a(2)`为对象年龄增加2岁，

```python
class A():
    def __init__(self, init_age):
        super().__init__()
        print('我年龄是:',init_age)
        self.age = init_age

    def __call__(self, added_age):
        

        res = self.forward(added_age)
        return res

    def forward(self, input_):
        print('forward 函数被调用了')
        
        return input_ + self.age
print('对象初始化。。。。')
a = A(10)


input_param = a(2)
print("我现在的年龄是：", input_param)
```

out:

> 
对象初始化。。。。

我年龄是: 10

forward 函数被调用了

我现在的年龄是： 12
**pytorch主要也是按照`__call__`, `__init__`,`forward`三个函数实现网络层之间的架构的**

这博客讲述了pytorch中具体实现：[https://blog.csdn.net/dss_dssssd/article/details/82977170](https://blog.csdn.net/dss_dssssd/article/details/82977170)







