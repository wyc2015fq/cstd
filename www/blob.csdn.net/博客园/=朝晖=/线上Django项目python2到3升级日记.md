# 线上Django项目python2到3升级日记 - =朝晖= - 博客园
# [线上Django项目python2到3升级日记](https://www.cnblogs.com/dhcn/p/7606210.html)
这两天干了一个几斤疯狂的事情，花不到一个工作日的时间把一个线上Django项目语言版本从python2升级到Python3
1、字典的一个语法变化
Python2.7：
```
if dict1.haskey(key1):
```
Python3：
```
if key1 in dict1.keys():
# or
if key1 in dict1 :
```
2、u"strcontent"这种格式在python3里面支持，但是定义正则的ur"regexcontent"已经不支持，需要改成r"regexcontent"
3、urllib2不再支持，代替方式：
```
import urllib.request
resp=urllib.request.urlopen('http://www.baidu.com')
html=resp.read()
print(html)
```
4、HTMLParser包路径发生变化：
```
from html.parser import HTMLParser
```
5、HTMLParser的继承定制方式发生变化，初始化函数必须加上超类的初始化语句，否则会error：
object has no attribute 'convert_charrefs'
```
def __init__(self):
        super().__init__()
        self.reset()
        self.images = []
```
6、StringIO新的import方式：
```
try:
    from StringIO import StringIO
except ImportError:
    from io import StringIO
```
7、Python3的异常类体系发生变化，主基类变为Exception，以前的StandardError下线，所以出错：NameError: name 'StandardError' is not defined
8、Python3里面没有long类型，所有都是int，还有以前的那种数字后面加L的long类型定义方式L得去掉，否则报错
9、urlparse包路径改为urllib.parse，用法基本没变
10、整除法的计算机制改变了，非整除会变成小数，不会直接floor了。
11、Ubuntu下uwsgi执行环境由python2升级为python3的主要操作是apt卸载*uwsgi-plugin-python，装上*uwsgi-plugin-python3，当然了，还有python3版本的切换和一堆依赖的安装。
12、md5的update参数值需要encode以下：
```
md5.update(str1.encode('utf-8'))
```
 13、属性和setter方法里面对实例值的定义名称不能和属性名称一样了，具体看下例中的下划线处理,如果不加那个处理，会导致stack overflow：
```
class Person(object):
    def __init__(self, name, birthday):
        self.name = name
        self.birthday = birthday
        
    @property
    def birthday(self):
        return self._birthday
    
    @birthday.setter
    def birthday(self, birthday):
        self._birthday = str_to_date(birthday)
    
    @property
    def age(self):
        return datetime.date.today() - self.birthday
    
    
    def __str__(self):
        return '%s,%s(%s)'%(self.name,self.birthday, self.age)
    
person = Person('Xiao Ming','1980-10-1')
```
14、urllib.basejoin已经废除，用urllib.parse.urljoin代替 
15、xrang全部统一成rang了
16、最后强调一下，所有的urllib.function的调用方式都已经废止，需要重新import相关新模块。
总结：修改点主要都在第三方不再维护的两个遗留库里面，特别廖雪峰的一个小lib，里面设计套路很深，用的都是传统的老api，最后实在移植成本太高，果断放弃，从pypi找了个同类的取代之。

