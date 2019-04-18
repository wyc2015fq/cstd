# 【Python】Lambda - YZXnuaa的博客 - CSDN博客
2018年02月06日 11:21:07[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：104
个人分类：[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)

# Lambda
## 初步认识
### 表达式及定义
> 
**lambda [ arg1 [arg2, arg3, … argN] ] : expression**
　　　　　　　　　　　　　　　　　　　　　　– 《Python核心编程》
从《Python核心编程》书本中包含了以上lambda表达式的表述。也就是说在lambda右侧的式子中，冒号左边的为参数值，右边的为计算表达式。
### 实例说明
**1.单层参数求和**
现在假设需要对两个数进行求和运算。对于正常的逻辑代码，不难写出如下代码：
```python
def sum(x, y):
    return x + y
```
- 1
- 2
而在lambda中，我们可以这样来写：
```python
p = lambda x, y: x + y
```
- 1
代码简洁了许多，可是因为缺少方法名对此步操作进行描述，也让我们对这个求和的Lambda表达式有一些疑问，也就是它在程序中目的是什么，我们难以揣测。 
![这里写图片描述](https://img-blog.csdn.net/20160308181139659)
**2.嵌套参数求和**
需求还是之前的需求，可是我们在对参数的传递发生了变化。什么叫作嵌套参数，我们可以参考如下代码： 
**注：下面的一段代码实际上是存在语法错误的，此代码的目的纯粹只是为了说明问题。请勿模仿**
```python
def sum_outer(x=0):
    def sum_inner(y):
        return x + y
```
- 1
- 2
- 3
采用lambda改写后的代码如下：
```python
def test_lamdba2(x=0):
    return lambda y: x + y
```
- 1
- 2
而两种代码逻辑的转换关系如下： 
![这里写图片描述](https://img-blog.csdn.net/20160308183728161)
**3.嵌套lambda**
前面假想了一种方法内部嵌套方法的情况。我们说Lambda就是理解成在方法内部嵌套了一个“表达式方法”。所以，还可以能上面进行一层Lambda的方法进行改写。将代码中的方法*test_lambda2*改写成lambda。改写后的描述如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20160309084907904)
将上图所示的转换过程与之前的转换进行一个分析，可以得到一个结论，在lambda的表达式中，前面的lambda为外层方法，后续lambda则为次外层方法，并以次由外向内递推。
## 变量作用域说明
关于变量在Lambda中的作用域从上面的几幅图中也可以看出一二来，主要可以做出以下几点的总结：
- 对局部变量可见
- 对全局变量可见
- 对当前层传入的参数可见
- 对上层函数传入的参数可见
- 对上层Lambda传入的参数可见
## Lambda评价
### 优点
- 在普通代码里几行的代码，在Lambda中只需要一行就可以解决。所以代码比以前更简洁了
- 可以在某一个方法内部定义，这样可以提高操作的便捷性
### 缺点
- Lambda是一个匿名函数，因为是匿名，所以可读性变差了
- 有时候有多个Lambda嵌套（就像实例中的第3点一样），让程序变得难以理解
# Ref
- 《Python核心编程》
- 《Python学习手册》
