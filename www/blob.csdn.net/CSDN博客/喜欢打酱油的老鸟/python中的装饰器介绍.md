
# python中的装饰器介绍 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月14日 15:27:59[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：124


[http://blog.itpub.net/31473948/viewspace-2157622/](http://blog.itpub.net/31473948/viewspace-2157622/)
在了解装饰器之前，我们需要知道什么**闭包**是什么鬼！
**闭包：**在一个函数内定义了一个函数f,并且这个函数f引用外部变量，在把这个函数f当做返回值返回。
上述说了闭包的三个条件：
**1 函数内定义了一个函数f**
**2 f函数引用了外部变量**
**3 f被当做返回值返回**
def t1():\#定义t1函数
    x=2def f():\#t1函数内部定义了f函数
        print(x)\#f函数引用了不属于自己内部的变量xreturnf \#f被当做返回值返回
**而装饰器有是什么鬼呢？其实闭包的一种运用。**
**装饰器：在不改变****函数****（当然还有其他的）****的代码和调用方式的前提下，为****函数增加新的功能!**
那么需求来了：**例如我定义了一个下载方法，我在想不改变下载方法的代码和调用方式，为其加一个需要登录后才能下载的功能**
defget(file):\#下载方法
    print('下载成功',file)那么如何实现呢？这里就要用到上面提到的闭包。
我们再来看看闭包，即是在一个函数内定义了一个函数f,并且这个函数f引用外部变量，在把这个函数f当做返回值返回
那么上面的需求我们可以先试着做**第一步，即是不改变下载方法的代码，为其加一个需要登录后才能下载的功能**
def get(file):\#下载方法
print('下载成功',file)
def auth(get,*args,**kwargs):
    print('登录方法')get(*args,**kwargs)
auth(get,'a.txt')
\#结果：
\#登录方法
\#下载成功 a.txt但是这样的话调用方式改变了，那么我们可以利用闭包：
def get(file):\#下载方法
print('下载成功',file)
def func(get):
    def auth():
        print('登录方法')get()returnauthget=func(get)get('dasd')这样的话，参数有传不进去，并且报错参数没有接收到：
我们分析一下上面代码 func(get)其实应该是auth这函数名，那么get=func(get)，就是把auth赋值给get变量，即是次数 get=auth,
那么 我们调用get('dasd') 即是auth('dasd')，即是我们需要修改auth函数
def get(file):\#下载方法
print('下载成功',file)
def func(get):
    def auth(file):
        print('登录方法')get(file)returnauthget=func(get)get('dasd')这样的话get('dasd')的时候就是调用auth('dasd'),那么会执行登录方法，并且执行get('dasd'),这里是真正的下载方法
优化一下就是这样的：
def auth(f):
    def wrapper(*args,**kwargs):
        print('其它功能或方法')
        f(*args,**kwargs)returnwrapper这样的要装饰get函数的时候，只需get=auth(get),这里第一个get是变量，第二个get是函数名,这样就实现get**不改变其的代码和调用方式，为其增加功能。**
**在python中 get=auth(get)，可以省略为：**
def auth(f):
def wrapper(*args,**kwargs):
        print('登录方法')
        f(*args,**kwargs)returnwrapper
@auth \# 相当于get=auth(get)
defget(file):\#下载方法
    print('下载成功',file)**当然上面是装饰器的无参装饰器，还有有参装饰器：**
\#有参装饰器 def authx(a): def auth(f): def wrapper(*args, **kwargs): if a=='a': print('登录方法') else: print('adad')
f(*args, **kwargs)returnwrapperreturnauth
@authx('adas')\#相当于get=auth(get) 当然 a的值也传入了defget(file):\#下载方法print('下载成功',file)
本文作者：孤月灵声
原文链接：https://www.cnblogs.com/wh-alan/p/9287434.html

