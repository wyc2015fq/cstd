
# Python的实例方法,类方法,静态方法之间的区别及调用关系 - jiahaowanhao的博客 - CSDN博客


2018年03月11日 20:23:14[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：703


[Python的实例方法,类方法,静态方法之间的区别及调用关系](http://cda.pinggu.org/view/24961.html)
![](http://cda.pinggu.org/uploadfile/image/20180311/20180311065517_42578.png)
如果只看这个图,很多人可能会看的一头雾水,特别是学过完全面向对象语言的同学, Python 是双面向的,既可以面向函数编程,也可以面向对象编程,所谓面向函数就是单独一个. py 文件,里面没有类,全是一些函数,调用的时候导入模块,通过模块名.函数名()即可调用,完全不需要类,那么你可能会问,那要类还有什么毛用? 类就是用来面向对象编程啦,类可以有自己的属性,类可以创建很多实例,每个实例可以有不同的属性,这也就保存了很多私有的数据,总之都有存在的必要.
再来看上面这张图,在类里面定义的函数就是方法,类方法需要@ classmethod 修饰并且有个隐藏参数 cls,实例方法必须有个参数 self, 静态方法必须有 @staticmethod修饰,类和实例都可以访问静态方法,实例可以访问实例方法也可以访问类方法,类可以访问类方法也可以访问实例方法,访问实例方法必须要带参数 self, 可以理解为类其实也是一个实例,类访问实例方法不带参数会报错的.例如：
[python] view plain copy
Traceback (most recent call last):
File "L:\Python\GetAllFiles\testRead.py", line 310, in <module>
sumRow , dimension = util.SumofRow(outputfilename)
TypeError: SumofRow() takes exactly 1 argument (2 given)
类本身可以访问函数,实例却不行.所以在定义实例方法时要加一个self的参数：
[python] view plain copy
def SumofRow(self,inputfile):
csv_input = pd.read_csv(inputfile,error_bad_lines=False)
return sum(np.asarray(csv_input).T[1:]),csv_input.shape[1]
从这个图片中学习到了，类方法，类静态方法，静态方法，注意它们是如何定义的，参数是怎么确定的，用的时候要注意。
说到这里，注意面向对象编程的思想，以及静态变量，静态方法，类，实例对象他们的生存期，知道生存期的差异就可以避免调用出错了。

