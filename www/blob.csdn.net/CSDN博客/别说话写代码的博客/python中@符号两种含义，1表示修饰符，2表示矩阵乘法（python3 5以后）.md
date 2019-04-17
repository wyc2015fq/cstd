# python中@符号两种含义，1表示修饰符，2表示矩阵乘法（python3.5以后） - 别说话写代码的博客 - CSDN博客





2018年12月14日 12:35:33[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：738








## 第一种，大家所熟知，表示修饰符，可以在模块或者类的定义层内对函数进行修饰。出现在函数定义的前一行，不允许和函数定义在同一行。在下面这种情况

```python
def funcA(A):
    print("function A")
    print(A)

def funcB(B):
    print(B(2))
    print("function B")

@funcA
@funcB
def func(c):
    print("function C")
    return c**2
```

执行结果为

![](https://img-blog.csdnimg.cn/20181214123343224.png)

```python
def makebold(fn):
    def warp():
        return "<a>"+fn()+"<a>"
    return warp
def makeitalic(fn):
    def warp():
        return "<b>"+fn()+"<b>"
    return warp
@makebold         #这句话相当于makebold(test1),也就是把当前函数传过去
def test1():
    return "test1"

@makeitalic
def test2():
    return "test2"
    
@makebold
@makeitalic
def test3():     #函数和装饰器是倒着执行的，从下往上
    return "test3"
    
print(test1())
print(test2())
print(test3())
```

执行结果为

![](https://img-blog.csdnimg.cn/20181214123432143.png)

## 第二种，不常用，表示矩阵乘法

![](https://img-blog.csdnimg.cn/2018121412071095.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)



