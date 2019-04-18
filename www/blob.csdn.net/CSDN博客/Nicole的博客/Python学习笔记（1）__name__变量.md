# Python学习笔记（1）__name__变量 - Nicole的博客 - CSDN博客
2018年04月09日 10:55:27[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：55标签：[__name__																[模块](https://so.csdn.net/so/search/s.do?q=模块&t=blog)](https://so.csdn.net/so/search/s.do?q=__name__&t=blog)
个人分类：[python 数据处理](https://blog.csdn.net/weixin_39541558/article/category/7563702)
Python使用缩进对齐组织代码的执行，所有没有缩进的代码，都会在载入时自动执行。每个文件（模块）都可以任意写一些没有缩进的代码，并在载入时自动执行。为了区分 主执行代码和被调用文件，Python引入了变量：__name__。
1）当文件是被调用时，__name__的值为模块名；
2）当文件被执行时，__name__的值为 ‘__main__’。
基于此特性，为测试驱动开发提供了很好的支持，我们可以在每个模块中写上测试代码，这些测试代码仅当模块被Python直接执行时才会运行，代码和测试完美的结合在一起。
### 1、典型的Python文件结构：
![](https://img-blog.csdn.net/20161207123709117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTUxMTYwMQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 2、Python中的__name__举例
```python
#hello.py  
def sayHello():  
    str="hello"  
    print(str);  
  
if __name__ == "__main__":  
    print ('This is main of module "hello.py"')  
    sayHello()
```
python作为一种脚本语言，我们用python写的各个module都可以包含以上那么一个类似c中的main函数，只不过python中的这种__main__与c中有一些区别，主要体现在：
1、当单独执行该module时，比如单独执行以上hello.py： python hello.py，则输出
```python
import hello  
hello.__name__  
'hello'
```
    可以理解为"if __name__=="__main__":" 这一句与c中的main()函数所表述的是一致的，即作为入口；
2、当该module被其它module 引入使用时，其中的"if __name__=="__main__":"所表示的Block不会被执行,这是因为此时module被其它module引用时，其__name__的值将发生变化，__name__的值将会是module的名字。比如在python shell中import hello后，查看hello.__name__：
```python
>>> import hello  
>>> hello.__name__  
'hello'  
>>>
```
3、因此，在python中，当一个module作为整体被执行时,moduel.__name__的值将是"__main__"；而当一个module被其它module引用时，module.__name__将是module自己的名字，当然一个module被其它module引用时，其本身并不需要一个可执行的入口main了。
