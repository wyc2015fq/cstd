# python之__name__和__main__ - nosmatch的专栏 - CSDN博客
2011年07月14日 19:55:08[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：378标签：[python																[module																[import																[shell																[脚本																[语言](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[脚本](https://blog.csdn.net/HDUTigerkin/article/category/845589)
[python中 __name__及__main()__的妙处](http://www.cnblogs.com/liqilei/archive/2010/08/11/1797715.html)
```
#hello.py
def sayHello():
    str="hello"
    print(str);
if __name__ == "__main__":
    print ('This is main of module "hello.py"')
    sayHello()
python作为一种脚本语言，我们用python写的各个module都可以包含以上那么一个累死c中的main函数，只不过python中的这种__main__与c中有一些区别，主要体现在：
1、当单独执行该module时，比如单独执行以上hello.py： python hello.py，则输出
     
```
This is main of module "hello.py"
hello
```
   可以理解为"if __name__=="__main__":" 这一句与c中的main()函数所表述的是一致的，即作为入口；
2、当该module被其它module 引入使用时，其中的"if __name__=="__main__":"所表示的Block不会被执行,这是因为此时module被其它module引用时，其__name__的值将发生变化，__name__的值将会是module的名字。比如在python shell中import hello后，查看hello.__name__：
```
>>> import hello
>>> hello.__name__
'hello'
>>>
```
3、因此，在python中，当一个module作为整体被执行时,moduel.__name__的值将是"__main__"；而当一个module被其它module引用时，module.__name__将是module自己的名字，当然一个module被其它module引用时，其本身并不需要一个可执行的入口main了。可以说python中的这种用法很灵活啊。
```
