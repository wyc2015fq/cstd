# python进阶(三) 内建函数getattr工厂模式 - _天枢 - 博客园
## [python进阶(三) 内建函数getattr工厂模式](https://www.cnblogs.com/yhleng/p/8340317.html)
2018-01-24 10:21 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8340317)
getattr()这个方法最主要的作用是实现反射机制。也就是说可以通过字符串获取方法实例。  传入不同的字符串，调用的方法不一样。
```
原型：getattr(对象，方法名)
```
举个栗子：
pyMethod类下定义了三个方法，getattr(pyMethod(),'out%s'%str)()   传入的方法名不同，调用不同的方法。些处方法名为字符串。
这样的话，想想是不是用途很多，我可以把方法名配置到文件中，读取时使用getattr动态去调用。
```
#coding=utf-8
class pyMethod(object):
    def outstr(self):
        print('this is string')
    def outint(self):
        print('this is number')
    def outdate(self):
        print('this is date')
if __name__=="__main__":
    str = 'int'
    getattr(pyMethod(),'out%s'%str)()     
    str = 'str'
    getattr(pyMethod(),'out%s'%str)()
    str = 'date'
    getattr(pyMethod(),'out%s'%str)()
```
```
getattr(pyMethod(),'out%s'%str)()  注意pyMethod()和最后的()   这里之所以这么写pyMethod()加括号是实例化类对象，最后的括号，因为getattr函数反射后，是一个方法对象。
```
运行结果：
```
C:\Python27\python.exe D:/weixin/python_getattr.py
this is number
this is string
this is date
Process finished with exit code 0
```
**Linux and python学习交流1,2群已满.**
**Linux and python学习交流3群新开,欢迎加入,一起学习.qq 3群:563227894**
不前进,不倒退,停止的状态是没有的.
一起进步,与君共勉,
