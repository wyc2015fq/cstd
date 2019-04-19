# Python学习之路三--（lambda应用） - BlueSky - CSDN博客
2015年10月28日 14:39:51[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：455标签：[python																[lambda																[应用](https://so.csdn.net/so/search/s.do?q=应用&t=blog)](https://so.csdn.net/so/search/s.do?q=lambda&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python学习](https://blog.csdn.net/ASIA_kobe/article/category/5896995)
## lambda函数简介
lambda函数也叫匿名函数，即，函数没有具体的名称，而用def创建的方法是有名称的。 
使用lambda函数还有一些注意事项： 
lambda 函数可以接收任意多个参数 (包括可选参数) 并且返回单个表达式的值。 
lambda 函数不能包含命令，包含的表达式不能超过一个。 比如:
```python
def foo():   #命名的foo函数
    return 'Hello world!'
lambda:'Hello world!'   #用lambda关键字创建的匿名函数
```
lambda语句中，冒号前是参数，可以有多个，用逗号隔开，冒号右边的为返回值。lambda语句构建的是一个函数对象。例如：
```python
lambda arg1,arg2,....argN: expression
```
**参数有无**
如果没有参数，lambda冒号前面就没有，如：`lambda：'Hello'`
如果有参数，则冒号前面为相应的参数，如：`lambda x,y:x+y`
## lambda函数排序
![这里写图片描述](https://img-blog.csdn.net/20151028110816958)
运行结果：   #后面再详细介绍下sort函数 
Before sort: 
Name:Kobe    Score:98 
Name:James   Score:78 
Name:Yao     Score:68
After ascending sort: 
Name:Yao     Score:68 
Name:James   Score:78 
Name:Kobe    Score:98
After descending sort: 
Name:Kobe    Score:98 
Name:James   Score:78 
Name:Yao     Score:68
## def与lambda的区别
它们的主要不同点是**python def 是语句而python lambda是表达式** ，理解这点对你了解它们很重要。 下面看看他们的应用吧。  
首先在python 里面语句是可以嵌套的，比如你需要根据某个条件来定义方法话，那只能用def了。你用lambda就会报错了。
```python
a = 2 
if a > 1 : 
    def info (): 
        print 'haha' 
else : 
    def test (): 
        print 'test'
```
而有的时候你需要在python 表达式里操作的时候，那需要用到表达式嵌套，这个时候python def就不能得到你想要的结果，那只能用python lambda  
下面给个例子： 
```python
g = lambda x : x+2
info = [g(x) for x in range(10)] 
print info
```
## sort()函数
