# python元类metaclass - Orisun - 博客园







# [python元类metaclass](https://www.cnblogs.com/zhangchaoyang/articles/4984833.html)





## 使用type创建类

使用type创建类的语法为

```
type(类名, 父类的元组（针对继承的情况，可以为空），包含属性的字典（名称和值）)
```

举个例子

```
class A(object):
    name='orisun'

class B(object):
    city='BeiJing'

class C(A,B):
    country='CN'

if __name__ == '__main__':
    c=C()
    print c.__class__.__name__
    print type(c)
    print type(c.__class__)
    print dir(c)
    print 

    d=type('C',(A,B,),{'country':'CN'})
    print d.__name__
    print type(d)
    print dir(d)
```

输出

```
C
<class '__main__.C'>
<type 'type'>
['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'city', 'country', 'name']

C
<type 'type'>
['__class__', '__delattr__', '__dict__', '__doc__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', 'city', 'country', 'name']
```

在python中一切皆对象，类也是对象，从上面的例子我们看到type(obj)是一个class，但是只要在这个class上继续调用type()，就会发现它最终还是个type。可以说pyhton中的所有对象（包括类）都是type创建出来的。下文我们会讲到使用MetaClass可以创建类（注意是创建类，不是对象哦），而MetaClass内部实际上还是调用的type。

## 使用MetaClass创建类

```
class Foo(object):
    __metaclass__=something
```

当定义了一个类时，python会经历以下步骤来确定如何创建一个对象。
- 如果类中指定了__metaclass__，则使用__metaclass__来创建对象。否则进入第2步
- 延着继承链从基类中找__metaclass__，如果找到了就使用该__metaclass__来创建对象。否则进入第3步
- 使用内置的type来创建对象

归根到底，能够创建对象的只有type，所以我们要自定义一个__metaclass__只有2个办法：
- 调用type函数
- 自己实现一个type的子类

### 使用type函数创建__metaclass__

```
def upper_attr(class_name,base_classes,attr_dict):
    '''返回一个类对象，将属性都转化为大写
    '''
    #选择所有不以'__'开头的属性
    attrs=((name,value) for name,value in attr_dict.items() if not name.startswith('__'))
    #将它们转换为大写
    uppercase_attr=dict((name.upper(),value) for name,value in attrs)
    #通过type来创建类对象
    return type(class_name,base_classes,uppercase_attr)


class A(object):
    __metaclass__=upper_attr
    name='orisun'

if __name__ == '__main__':
    print hasattr(A,'name')         #输出False
    print hasattr(A,'NAME')         #输出True
```

### 创建type的子类来实现__metaclass__

```
class UpperMetaClass(type):
    def __new__(cls,class_name,base_classes,attr_dict):
        attrs=((name,value) for name,value in attr_dict.items() if not name.startswith('__'))
        uppercase_attr=dict((name.upper(),value) for name,value in attrs)
        return type.__new__(cls,class_name,base_classes,uppercase_attr)
        #等价于：
        #return super(UpperMetaClass,cls).__new__(cls,class_name,base_classes,uppercase_attr)
   
class A(object):
    __metaclass__=UpperMetaClass
    name='orisun'

if __name__ == '__main__':
    print hasattr(A,'name')         #输出False
    print hasattr(A,'NAME')         #输出True
```

 其实在MetaClass中定义__new__或__init__或__call__都可以实现对创建类的操控。具体原因参见：[python对象的创建和销毁](http://www.cnblogs.com/zhangchaoyang/articles/4984768.html#M)

在MetaClass中定义__getattribute__、__getattr__、__setattr__可以影响“[通过类对属性的访问和设置](http://www.cnblogs.com/zhangchaoyang/articles/4984074.html#M)”












