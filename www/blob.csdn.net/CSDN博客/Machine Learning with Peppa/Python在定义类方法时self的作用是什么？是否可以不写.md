# Python在定义类方法时self的作用是什么？是否可以不写 - Machine Learning with Peppa - CSDN博客





2018年03月31日 21:31:54[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：5015








今天在写代码的时候发现某个类的方法:def xx() 中没有加上self，运行后提示的错误很有意思，大意是提供了一个参数但却定义了0个参数。后来查了下资料大概了解了self的作用，下面一起来看看：

### self的作用

不加self是本地变量，只在这个方法里有效；加self则是实例变量。因此，类里没有加self.的属性是全局的，所有的实例化出来的对象都访问到同一个

来看一个简单例子：

class A: 
    def fun(): 
        pass 
# 这种就是静态函数，一般用装饰器 @staticmethod 修饰 
class B: 
    def fun1(self): 
        pass 
# 这种就是成员函数，类实例可以直接调用 
@classmethod 
    def fun(): 
        pass 


# 这种是类函数，要求第一个参数表示类（ cls ）。也就是说，除了装饰器 @classmethod 外，还要求它的定义如 def fun(cls,...)。这是通过类直接调用的，如 A1.fun(...) 






### 那么，self可以不写吗
在Python的解释器内部，当我们调用X.fun()时，实际上Python解释成X.fun(self)，也就是说把self替换成类的实例。有兴趣的可以把上面的一行改写一下，运行后的实际结果完全相同。实际上已经部分说明了self在定义时不可以省略，如果非要试一下，那么请看下面：

```python
class Test:
    def prt():
        print(self)

t = Test()
t.prt()
1
2
3
4
5
6
class Test:
    def prt():
        print(self)
 
t = Test()
t.prt()
```

运行时提醒错误如下：prt在定义时没有参数，但是我们运行时强行传了一个参数。由于上面解释过了t.prt()等同于Test.prt(t)，所以程序提醒我们多传了一个参数t。


```python
Traceback (most recent call last):
  File "h.py", line 6, in <module>
    t.prt()
TypeError: prt() takes 0 positional arguments but 1 was given
1
2
3
4
Traceback (most recent call last):
  File "h.py", line 6, in <module>
    t.prt()
TypeError: prt() takes 0 positional arguments but 1 was given
```




