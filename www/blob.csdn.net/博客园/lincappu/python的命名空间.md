# python的命名空间 - lincappu - 博客园







# [python的命名空间](https://www.cnblogs.com/lincappu/p/8204287.html)





Python的命名空间是Python程序猿必须了解的内容，对Python命名空间的学习，将使我们在本质上掌握一些Python中的琐碎的规则。

接下来我将分四部分揭示Python命名空间的本质：一、命名空间的定义；二、命名空间的查找顺序；三、命名空间的生命周期；四、通过locals()和globals() BIF访问命名空间

重点是第四部分，我们将在此部分观察命名空间的内容。

**一、命名空间**


Python使用叫做命名空间的东西来记录变量的轨迹。命名空间是一个 字典（dictionary） ，它的键就是变量名，它的值就是那些变量的值。

A *namespace* is a mapping from names to objects. Most namespaces are currently implemented as Python dictionaries。



在一个 Python 程序中的任何一个地方，都存在几个可用的命名空间。

     1、每个函数都有着自已的命名空间，叫做局部命名空间，它记录了函数的变量，包括函数的参数和局部定义的变量。

     2、每个模块拥有它自已的命名空间，叫做全局命名空间，它记录了模块的变量，包括函数、类、其它导入的模块、模块级的变量和常量。

     3、还有就是内置命名空间，任何模块均可访问它，它存放着内置的函数和异常。



**二、命名空间查找顺序**

当一行代码要使用变量 x 的值时，Python 会到所有可用的名字空间去查找变量，按照如下顺序：

     1、局部命名空间：特指当前函数或类的方法。如果函数定义了一个局部变量 x，或一个参数 x，Python 将使用它，然后停止搜索。

     2、全局命名空间：特指当前的模块。如果模块定义了一个名为 x 的变量，函数或类，Python 将使用它然后停止搜索。

     3、内置命名空间：对每个模块都是全局的。作为最后的尝试，Python 将假设 x 是内置函数或变量。

     4、如果 Python 在这些名字空间找不到 x，它将放弃查找并引发一个 NameError 异常，如，NameError: name 'aa' is not defined。



嵌套函数的情况：

     1、先在当前 (嵌套的或 lambda) 函数的命名空间中搜索

     2、然后是在父函数的命名空间中搜索

     3、接着是模块命名空间中搜索

     4、最后在内置命名空间中搜索


！！！不同命名空间中命名没有任何联系！！！




示例：




```
1 info = "Adress : "
 2 def func_father(country):
 3     def func_son(area):
 4         city= "Shanghai " #此处的city变量，覆盖了父函数的city变量
 5         print(info + country + city + area)
 6     city = " Beijing "
 7     #调用内部函数
 8     func_son("ChaoYang ");
 9  
10 func_father("China ")
```

输出：Adress : China Shanghai ChaoYang




以上示例中，info在全局命名空间中，country在父函数的命名空间中，city、area在自己函数的命名空间中





**三、命名空间的生命周期**

不同的命名空间在不同的时刻创建，有不同的生存期。

     1、内置命名空间在 Python 解释器启动时创建，会一直保留，不被删除。

     2、模块的全局命名空间在模块定义被读入时创建，通常模块命名空间也会一直保存到解释器退出。

     3、当函数被调用时创建一个局部命名空间，当函数返回结果 或 抛出异常时，被删除。每一个递归调用的函数都拥有自己的命名空间。



如果一个命名声明为全局的，那么对它的所有引用和赋值会直接搜索包含这个模块全局命名的作用域。否则，在最里面作用域之外找到的所有变量都是只读的（对这样的变量赋值会在最里面的作用域创建一个***新*** 的局部变量，外部具有相同命名的那个变量不会改变）。

Python 的一个特别之处在于其赋值操作总是在最里层的作用域。赋值不会复制数据——只是将命名绑定到对象。删除也是如此："del y" 只是从局部作用域的命名空间中删除命名 y 。事实上，所有引入新命名的操作都作用于局部作用域。



示例：


```
i=1
def func2():
    i=i+1
 
func2();
#错误：UnboundLocalError: local variable 'i' referenced before assignment
```

由于创建命名空间时，python会检查代码并填充局部命名空间。在python运行那行代码之前，就发现了对i的赋值，并把它添加到局部命名空间中。当函数执行时，python解释器认为i在局部命名空间中但没有值，所以会产生错误。



```
# 1.只是取值：
i=1
def func():
    print(i)
func()
print(i)

# 2.赋值：
# i=1
# def func1():
#     i+=1
#     print(i)
#
# func1()
# print(i)
如果内部函数有引用外部函数的同名变量或者全局变量,并且对这个变量有修改.那么python会认为它是一个局部变量,又因为函数中没有gcount的定义和赋值，所以报错

# 3.申明全局变量：global
i=1
def func2():
    global i
    i+=1
    print(i)

func2()
print(i)
```

结论：在函数内部进行读值与赋值是不同的：

读值：变量按照查找顺序查找值，直到站到为止，找不到就报错，

赋值:赋值操作都是发生在函数的最内层空间，就是局部作用域，赋值只是将命名绑定到对象，这时候其实是看不到函数的外部变量的，只有加上 global 才能找到函数外面的变量。但读取就不需要。



**四、命名空间的访问**


**1、局部命名空间可以 locals()  BIF来访问。**

locals 返回一个名字/值对的 dictionary。这个 dictionary 的键是字符串形式的变量名字，dictionary 的值是变量的实际值。

示例：


```
def func1(i, str ):
    x = 12345
    print(locals())
 
func1(1 , "first")
```

输出：{'str': 'first', 'x': 12345, 'i': 1}




**2、全局 (模块级别)命名空间可以通过 globals() BIF来访问。**

示例：


```
'''Created on 2013-5-26'''
 
import copy
from copy import deepcopy
 
gstr = "global string"
 
def func1(i, info):
    x = 12345
    print(locals())
 
func1(1 , "first")
 
if __name__ == "__main__":
    print("the current scope's global variables:")
    dictionary=globals()
    print(dictionary)
```

输出：（我自己给人为的换行、更换了顺序，加颜色的语句下面重点说明）


{

'__name__': '__main__',

'__doc__': 'Created on 2013-5-26',  


'__package__': None, 

'__cached__': None, 

'__file__': 'E:\\WorkspaceP\\Test1\\src\\base\\test1.py', 


'__loader__': <_frozen_importlib.SourceFileLoader object at 0x01C702D0>, 

'copy': <module 'copy' from 'D:\\Python33\\lib\\copy.py'>, 

'__builtins__': <module 'builtins' (built-in)>, 

'gstr': 'global string', 

'dictionary': {...}, 

'func1': <function func1 at 0x01C6C540>, 

'deepcopy': <function deepcopy at 0x01DB28A0>

}



总结

　　1、模块的名字空间不仅仅包含模块级的变量和常量，还包括所有在模块中定义的函数和类。除此以外，它还包括了任何被导入到模块中的东西。

　　2、我们看到，内置命名也同样被包含在一个模块中，它被称作 __builtin__。

　　3、回想一下 from module import 和 import module 之间的不同。

　　　　使用 import module，模块自身被导入，但是它保持着自已的名字空间，这就是为什么您需要使用模块名来访问它的函数或属性：module.function 的原因。

　　　　但是使用 from module import function，实际上是从另一个模块中将指定的函数和属性导入到您自己的名字空间，这就是为什么您可以直接访问它们却不需要引用它们所来源的模块。使用 globals 函数，您会真切地看到这一切的发生，见上面的红色输出语句。




**3、 locals 与 globals 之间的一个重要的区别**

locals 是只读的，globals 不是

示例：



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

def func1(i, info):
    x = 12345
    print(locals())
    locals()["x"]= 6789
    print("x=",x)
 
y=54321
func1(1 , "first")
globals()["y"]= 9876
print( "y=",y)

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

输出：


{'i': 1, 'x': 12345, 'info': 'first'}

x= 12345

y= 9876

解释：

　　locals 实际上没有返回局部名字空间，它返回的是一个拷贝。所以对它进行改变对局部名字空间中的变量值并无影响。

　　globals 返回实际的全局名字空间，而不是一个拷贝。所以对 globals 所返回的 dictionary 的任何的改动都会直接影响到全局变量。



**五、nonlocal关键字用来在函数或其他作用域中使用外层(非全局)变量。**

**在Python 2.x中，闭包只能读外部函数的变量，而不能改写它。**

**python3中只要在闭包内用nonlocal声明变量，就可以让解释器在外层函数中查找变量名了**

```
def f1():
    x=1
    def f2():
        # nonlocal  x
        x+=1
        print(x)

    print(x)
    return f2


f=f1()
f()
```





























