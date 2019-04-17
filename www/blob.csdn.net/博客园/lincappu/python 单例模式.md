# python  单例模式 - lincappu - 博客园







# [python  单例模式](https://www.cnblogs.com/lincappu/p/8303961.html)





## 1.定义：确保一个类只有一个实例，而且自行实例化并向整个系统提供这个实例。

　　　　以上定义虽然只说了一句话，但是包含了以下三点：　　
- 某个类只能有一个实例
- 它必须创建这个实例
- 它必须自行向整个系统提供这个实例

## 2.类图

![](https://images2015.cnblogs.com/blog/1136072/201707/1136072-20170705150024675-2117329263.png)

## 3.单例模式类型

**demo1:以下例子看样子没有什么毛病，但是类名本身也是可以实例化的，如obj3 = Foo()，这不符合单例模式的要求。**

```
class Foo:
    __instance = None
    def __init__(self):
        self.ip = '1.2.3.4'
        self.port = 3360
        self.pwd = '123456'
        self.username = 'xxxx'
        self.conn_list = [1,2,3,4,5,6,7,8]

    @classmethod  # 调用类的方法来创建一个单例
    def get_connetion(cls):
        if cls.__instance:
            return cls.__instance
        else:
            cls.__instance = Foo()
            return cls.__instance

obj1 = Foo.get_connetion()
print(obj1)
obj2 = Foo.get_connetion()
print(obj2)
```

**demo2：以下同过__new__来创建一个单例**
```
class Singleton(object):
    __instance = None
    def __init__(self):
        pass

    def __new__(cls, *args, **kwargs):
        if not Singleton.__instance:
            Singleton.__instance = object.__new__(cls,*args, **kwargs)
        return Singleton.__instance

obj1 = Singleton()
obj2 = Singleton()

if obj1 == obj2:
    print('两个是一样的')
else:
    print('两个不是一样的')
```


## 4.应用场景

　　需要频繁实例化然后销毁的对象。

　　创建对象时耗时过多或者耗资源过多，但又经常用到的对象。
　　有状态的工具类对象。
　　频繁访问数据库或文件的对象。

　　生成全局惟一的序列号

　　访问全局复用的惟一资源，如磁盘、总线等

　　单个对象占用的资源过多，如数据库等

　　系统全局统一管理，如Windows下的Task Manager

　　网站计数器

## 5.单例优缺点

优点：

　　在内存中只有一个对象，节省内存空间。
　　避免频繁的创建销毁对象，可以提高性能。
　　可以全局访问。

　　全局只有一个接入点，可以更好地进行数据同步控制，避免多重占用

缺点：

**开销**

*　　虽然数量很少，但如果每次对象请求引用时都要检查是否存在类的实例，将仍然需要一些开销。可以通过使用静态初始化解决此问题。*

**　　可能的开发混淆**

　　使用单例对象（尤其在类库中定义的对象）时，开发人员必须记住自己不能使用 **new**关键字实例化对象。因为可能无法访问库源代码，因此应用程序开发人员可能会意外发现自己无法直接实例化此类。

**　　对象生存期**

　　不能解决删除单个对象的问题。在提供内存管理的语言中（例如基于.NET Framework的语言），只有单例类能够导致实例被取消分配，因为它包含对该实例的私有引用。在某些语言中（如 C++），其他类可以删除对象实例，但这样会导致单例类中出现悬浮引用。

## 6.单例模式注意事项

　　只能使用单例类提供的方法得到单例对象，不要使用反射，否则将会实例化一个新对象。
　　不要做断开单例类对象与类中静态引用的危险操作。
　　多线程使用单例使用共享资源时，注意线程安全问题.

##  7.实例

　　Demo1:web界面简单应用

```
#!/usr/bin/env python
#coding:utf-8
from wsgiref.simple_server import make_server

# ########### 单例类定义 ###########
class DbHelper(object):

    __instance = None

    def __init__(self):
        self.hostname = '1.1.1.1'
        self.port = 3306
        self.password = 'pwd'
        self.username = 'root'

    @staticmethod
    def singleton():
        if DbHelper.__instance:
            return DbHelper.__instance
        else:
            DbHelper.__instance = DbHelper()
            return DbHelper.__instance

    def fetch(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def create(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def remove(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass

    def modify(self):
        # 连接数据库
        # 拼接sql语句
        # 操作
        pass


class Handler(object):

    def index(self):
        obj =  DbHelper.singleton()
        print id(single)
        obj.create()
        return 'index'

    def news(self):
        return 'news'


def RunServer(environ, start_response):
    start_response('200 OK', [('Content-Type', 'text/html')])
    url = environ['PATH_INFO']
    temp = url.split('/')[1]
    obj = Handler()
    is_exist = hasattr(obj, temp)
    if is_exist:
        func = getattr(obj, temp)
        ret = func()
        return ret
    else:
        return '404 not found'

if __name__ == '__main__':
    httpd = make_server('', 8001, RunServer)
    print "Serving HTTP on port 8001..."
    httpd.serve_forever()
```

 　　Demo2:某中央处理器（CPU）通过某种协议总线与一个信号灯相连，信号灯有64种颜色可以设置，中央处理器上运行着三个线程，都可以对这个信号灯进行控制，并且可以独立设置该信号灯的颜色。抽象掉协议细节（用打印表示），如何实现线程对信号等的控制逻辑。

```
import threading
import time


class Singleton(object):
    def __new__(cls, *args, **kwargs):
        if not hasattr(cls, '_instance'):
            orig = super(Singleton,cls)
            cls._instance = orig.__new__(cls, *args, **kwargs)
        return cls._instance


class Bus(Singleton):  # 总线
    lock = threading.RLock()

    def sendData(self, data):
        self.lock.acquire()
        time.sleep(3)
        print('Sending signal Data....', data)  # 模拟使用总线
        self.lock.release()


class Visit(threading.Thread):
    my_bus = ''
    name = ''

    def getname(self):
        return self.name

    def setName(self, name):
        self.name = name

    def run(self):
        self.my_bus = Bus()
        self.my_bus.sendData(self.name)

if __name__ == '__main__':
    for i in range(3):
        print("Entity %s begin to run..." %i)
        my_entity = Visit()
        my_entity.setName("Entity_"+str(i))
        my_entity.start()
```












