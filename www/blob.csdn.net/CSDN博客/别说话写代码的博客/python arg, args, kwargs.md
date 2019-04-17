# python arg,*args,**kwargs - 别说话写代码的博客 - CSDN博客





2019年01月07日 10:55:12[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：20标签：[python可变列表																[python *args																[python **kwargs																[**kwargs](https://so.csdn.net/so/search/s.do?q=**kwargs&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=python **kwargs&t=blog)




 python中 arg,*args,**kwargs（当然这几个变量名换成其他无所谓），主要是(没有*)变量名，*变量名，**变量名。 这几个相对顺序不能变。分别表示 变量，可变参数列表，参数及对应值列表

```python
#python中 arg,*args,**kwargs,当然这几个变量名换成其他无所谓，主要是(没有*)变量名，*变量名，**变量名。 这几个相对顺序不能变
#arg表示某个参数
def func1(arg):  #一个变量
    print(arg)
func1(1)

def func2(*args):  #可变参数列表,*args 用来将参数打包成tuple给函数体调用,输出结果以元组的形式展示
    print(args,type(args))
func2(1,2,3)

def func3(arg1,arg2,*args):  
    print('arg1:',arg1)
    print('arg2:',arg2)
    print('args:',args)
func3(1,2,3,4,5,6,7)
    
def func4(**kwargs):   # **kwargs 打包关键字参数成dict给函数体调用,输出结果以dict形式展示
    print(kwargs,type(kwargs))
func4(a=1,b=2,c=3,d=4)

def func5(arg,*args,**kwargs):
    print(arg,args,kwargs)
func5(1,2,3,4,a=5,b=6,c=7)
```

![](https://img-blog.csdnimg.cn/20190107105212659.png)](https://so.csdn.net/so/search/s.do?q=python *args&t=blog)](https://so.csdn.net/so/search/s.do?q=python可变列表&t=blog)




