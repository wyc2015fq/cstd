# 一文详解Python中的*可变参数与**关键字参数 - Machine Learning with Peppa - CSDN博客





2018年01月15日 23:02:09[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2688
所属专栏：[用Python玩转数据](https://blog.csdn.net/column/details/18811.html)









在Python中可以定义可变参数，顾名思义，可变参数就是传入参数是可变的。可以是任意个，以一个简单的数学编程为例，计算

sum = a * a + b * b + .....z * z

函数定义可以如下：

```
def getsum(num) :
    sum = 0
    for n in num :
        sum = sum + n * n
    return sum
```

但是调用的时候必须定义一个元组或者列表。比如getsum([1,2,3])

如果利用可变参数调用函数的方式可以变成这样的话就方便的多比如getsum(1,2,3)

可以这么修改

```
def getsum(*num) :
    sum = 0
    for n in num :
        sum = sum + n * n
    return sum
```

如果已经有一个list或者tuple，要调用一个可变参数怎么办？可以这样做：

```
>>> nums = [1, 2, 3]
>>> calc(nums[0], nums[1], nums[2])
14
```

这种写法当然是可行的，问题是太繁琐，所以Python允许你在list或tuple前面加一个`*`号，把list或tuple的元素变成可变参数传进去：

```
>>> nums = [1, 2, 3]
>>> calc(*nums)
14
```


### 关键字参数

可变参数允许你传入0个或任意个参数，这些可变参数在函数调用时自动组装为一个tuple。而关键字参数允许你传入0个或任意个含参数名的参数，这些关键字参数在函数内部自动组装为一个dict。请看示例：

```
def person(name, age, **kw):
    print('name:', name, 'age:', age, 'other:', kw)
```



函数person除了必选参数name和age外，还接受关键字参数kw。调用该函数时可以只传入必选参数

```
>>> person('Michael', 30)
name: Michael age: 30 other: {}
```



```
>>> person('Bob', 35, city='Beijing')
name: Bob age: 35 other: {'city': 'Beijing'}
>>> person('Adam', 45, gender='M', job='Engineer')
name: Adam age: 45 other: {'gender': 'M', 'job': 'Engineer'}
```

关键字参数有什么用？它可以扩展函数的功能，比如在person函数里，我们保证能接收到必选参数，但是如果调用者愿意提供更多的参数，我们也能收到，

试想你正在做一个用户注册的功能，除了用户名和年龄是必须的以外，其他的都是可选项目，利用关键字参数就可以完成这个要求。

和可变参数类似可以先组装出一个字典dict，然后把dict传进去。

```
>>> extra = {'city': 'Beijing', 'job': 'Engineer'}
>>> person('Jack', 24, city=extra['city'], job=extra['job'])
name: Jack age: 24 other: {'city': 'Beijing', 'job': 'Engineer'
```

上面复杂的调用可以用简化的写法

```
>>> extra = {'city': 'Beijing', 'job': 'Engineer'}
>>> person('Jack', 24, **extra)
name: Jack age: 24 other: {'city': 'Beijing', 'job': 'Engineer'}
```

**extra表示把extra这个dict的所有key-value用关键字参数传入到函数的**kw参数，**kw将获得一个dict，注意kw获得的dict是extra的一个拷贝，对kw的改动不会影响到函数外的extra



命名关键字参数：

对于关键字参数，函数的调用者可以传入任意不受限制的关键字参数。至于到底传入了哪些，需要在函数内部对kw进行检查

仍然以上述函数为例，我们需要检查是否有city和job参数

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
def person(name, age, **kw):
    if 'city' in kw:
        # 有city参数
        pass
    if 'job' in kw:
        # 有job参数
        pass
    print('name:', name, 'age:', age, 'other:', kw)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

但是调用者仍可以传入不受限制的关键字参数：
`>>> person('Jack', 24, city='Beijing', addr='Chaoyang', zipcode=123456)`


如果要限制关键字参数的名字，就可以用命名关键字参数，例如，只接收`city`和`job`作为关键字参数。这种方式定义的函数如下：

```
def person(name, age, *, city, job):
    print(name, age, city, job)
```

和关键字参数`**kw`不同，命名关键字参数需要一个特殊分隔符`*`，`*`后面的参数被视为命名关键字参数。




