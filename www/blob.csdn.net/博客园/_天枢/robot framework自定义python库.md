# robot framework自定义python库 - _天枢 - 博客园
## [robot framework自定义python库](https://www.cnblogs.com/yhleng/p/8857059.html)
2018-04-16 17:15 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8857059)
**自定义python库的好处:**
robot framework填表式,将python的灵活性弄没了,但是不要担心,RF早就想到了解决办法,就是扩充自己的库.
1.在python应用程序包目录下创建一个新包:D:\Python27\Lib\site-packages\**NewLibrary    **
**   这里注意文件夹NewLibrary就是你包的名称,这里顺便说一下包的概念,python区分包和普通文件夹就在于文件夹里是否有一个__init__.py的文件**
**回到正题**
**NewLibrary这个包就是之后RF Library要导入的名称**
2.创建一个类文件,用来存储你自定义函数的,这里写了个myclass.py的文件(这个名子可以自定义)
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180416170702235-1028391915.png)
3.分别编辑myclass.py与__init__.py的内容
__init__.py内容:
```
from myclass import MyClass #从myclass.py导入类名为MyClass的类
class NewLibrary(MyClass): #从MyClass断承
    ROBOT_LIBRARY_SCOPE = 'GLOBAL' #设置RF选项
```
myclass.py内容:
```
class MyClass(object):
    def __init__(self):
        pass
 
    def printMsg(self,msg):
        print "hello "+msg
```
以上写完,打开RF
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180416171052619-1741344125.png)
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180416171117578-1907314972.png)
看一下关键字里,是不是已经有了我们定义的类文件中的函数
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180416171205648-1789936475.png)
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180416171227249-740579625.png)
在RF中调用我们自定义的函数
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180416171245127-1802777877.png)
另外:输入部分函数名如:print按ctrl+alt+space(空格键),会调出提示
![](https://images2018.cnblogs.com/blog/1149221/201804/1149221-20180416171423419-1927714366.png)
