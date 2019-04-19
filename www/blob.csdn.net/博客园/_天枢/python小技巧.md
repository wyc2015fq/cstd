# python小技巧 - _天枢 - 博客园
## [python小技巧](https://www.cnblogs.com/yhleng/p/9590530.html)
2018-09-05 10:18 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9590530)
平时开发中，一些使用的技巧，记录一下，持续更新…………
**1.当函数默认参数为可变对象时，每次调用函数，是不会重新创建对象的**
示例代码1：
```
def ddd(a,b=[]):
    b.append(a)
    return b
print(ddd(1))
print(ddd(2,['a','b','c']))
print(ddd(3))
```
输出结果：
```
[1]  
['a','b','c',2]   
[1，3]
```
你对此，可能有些疑问，不对呀，看上去第3个输出，应该输出[3]呢，为什么是[1,3]呢？？？？
这个问题问得很好，这涉及到python对象作为参数时，每次调用并不会创建一个新的对象。就这么简单。
那么你可能又问啦，那为什么第2个，不输出[1，2]这是因为第二个调用函数的时候，传的参数是一个对象，将原来的对象改变了。
下面我们看一下，参数变化过程，内存id变化，很明显第一次和第三次b参数内存地址是同一个。
这说明**函数调用，是不会创建新对象的。除非外在改变，也就是第二个调用**
```
D:\Python27\python.exe D:/untitled1/temp5.py
49089672
[1]
49089480
['a', 'b', 'c', 2]
49089672
[1, 3]
```
**2.字符串转成列表元素，以及列表转成字符串：**
```
a = 'helloworld'
print(list(a)) #字符串转成，列表
print(''.join(list(a))) #列表转成字符串
```
输出结果：
```
['h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd']
helloworld
```
**3.元序字典排序**
```
#字典排序
from collections import OrderedDict
dic = {'c':1,'b':2}
print(OrderedDict(sorted(dic.iteritems(),key=lambda a:a[0])))
#输出结果:OrderedDict([('b', 2), ('c', 1)])
#这里要说一下，看输出大家可能有点晕，你只把这个输出当原字典用就可以了。
```
**4.序列中出现次数最多的元素**
```
#序列中出现次数最多的元素
from collections import Counter
cnt = Counter()
lis = [1,2,3,4,2,3,5,6,2]
for i in lis:
    cnt[i] +=1
print(cnt) #输出所有元素排名
print(cnt.most_common(3)) #输出排名前三的元素
#结果:
# Counter({2: 3, 3: 2, 1: 1, 4: 1, 5: 1, 6: 1})  2出现3次
# [(2, 3), (3, 2), (1, 1)]
```
**5.列表清空**
```
a=[1,2,3]
del a[:]  #py3还可以a.clear()
print(a)
```
**6.生成列表**
```
#6.生成10个元素的列表
print(range(1,10)) #输出结果：[1, 2, 3, 4, 5, 6, 7, 8, 9]
```
**7.两个列表连接**
```
#7.两个列表连接
a=[1,2,3]
a.extend([4,5,6])
print(a) #输出结果:[1, 2, 3, 4, 5, 6]
```
```
a=[1,2,3]
b =[4,5,6]
print(a+b)
```
**8.python执行js脚本**
execjs包导入需要,先安装pyExecjs包
```
pip install pyExecjs
```
```
#python 执行js脚本
#如果是js文件的话,需要先把js文件所有内容读出来
#放在compile里进行编译
import execjs
ctx = execjs.compile('''function add(a,b){return a+b;}''')
print(ctx.call("add",1,2))
```
输出结果:
```
D:\python36\python3.exe D:/untitled4/temp7.py
3
Process finished with exit code 0
```
**9.cmp(x,y),比较函数**
python2.7中cmp是一个内建函数,直接比较两个值,x>y返回1,x==y返回0 ,x<y返回-1
在python3.0+ 中取消了这个函数,使用(x>y)-(x<y)表达式,替代.
示例:python3
```
def cmp(x,y):
    return (x>y)-(x<y)
print(cmp(2,2))
print(cmp(1,2))
print(cmp(3,2))
# 输出:
# 0
# -1
# 1
```
**10.所有序列随机排序**
```
import random
```
```
#将序列的所有元素随机排序
a = [1,2,3,4]
random.shuffle(a)
print(a)
#输出[1, 3, 2, 4]
```
**11.随机从序列中选择一个值**
```
#从序列的元素中随机挑选一个元素，比如random.choice(range(10))，从0到9中随机挑选一个整数。
print(random.choice([1,4,7,9]))
```
**12.两个列表,合并,生成字典**
```
list1 = ['a','b','c','d']
list2 = [5,6,7,8]
print(dict(zip(list1,list2)))
```
结果:
```
D:\Python27\python.exe D:/untitled1/temp5.py
{'a': 5, 'c': 7, 'b': 6, 'd': 8}
```
注意python2.7和python3x的区别:
python3需要在zip函数外加个dict或list进行转换,从面得到合并的list和dict
python2.7是不需要加的,这个默认就是list,字典的话需要用dict关键字转换
```
list1 = ['a','b','c','d']
list2 = [5,6,7,8]
print(list(zip(list1,list2)))
```
结果:
```
D:\python36\python3.exe D:/untitled4/temp7.py
[('a', 5), ('b', 6), ('c', 7), ('d', 8)]
```
**13.python2 与python3 json decode方法**
**能过解码,来解决,中文unicode输出编码\e5x\a5x**
python2:
```
json.dumps(a).decode('unicode-escape')
```
python3:
```
json.dumps(a,ensure_ascii=False)
```
**14.元组变量与列表变量前加*一个星；字典变量前加**两个星的作用**
```
#变量单星号实例与，双星号实例区别
def add(a, b):
    return a + b
#把列表元素，分析成单个元素4 3进行传递
data = [4, 3]
print(add(*data))
#把字典分解成a=4,b=3两个参数传递
data = {'a': 4, 'b': 3}
print(add(**data))
#把元组分解成1,2,3
up = (1,2,3)
print(*up)
```
**15.return 与 global关键字**
1>一般我们是这样使用return的
```
def demo():
    stra = 'abc'
    return stra
print(demo())
```
那么在接下来看看global能不能达到return同样的作用；
```
def demo():
    global stra
    stra = 'abc'
    
demo()
print(stra)
```
经过实实验，以上代码达到了return同样的效果。因为关键字global将变量stra设置为了全局变量，当调用demo函数时，stra被赋值了。
**不建议使用global关键字。容易造成代码混乱，可读性差，等多种不利因素。
2>函数要返回多个值怎么办??可以这样。
```
def demo():
    stra = 'abc'
    strb = 'def'
    return stra,strb
a,b = demo()
print(a,b)
```
推荐以上这样写法，当然你也可以反多个返回值存储在元组，列表，字典等里。
**16.小技七节省内存40%-50%   __slots__**
在Python中，每个类都有实例属性。默认情况下Python用一个字典来保存一个对象的实例属性。这非常有用，因为它允许我们在运行时去设置任意的新属性。
然而，对于有着已知属性的小类来说，它可能是个瓶颈。这个字典浪费了很多内存。Python不能在对象创建时直接分配一个固定量的内存来保存所有的属性。因此如果你创建许多对象（我指的是成千上万个），它会消耗掉很多内存。
不过还是有一个方法来规避这个问题。这个方法需要使用`__slots__`来告诉Python不要使用字典，而且只给一个固定集合的属性分配空间。
示例代码2：未加__slots__
```
class memory(object):
    #__slots__应加在此处
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def demo(self):
        print(self.name)
memory('yhleng','27')
```
示例代码2：加__slots__
```
class memory(object):
    __slots__ = ['name', 'age']
    def __init__(self,name,age):
        self.name = name
        self.age = age
    def demo(self):
        print(self.name)
memory('yhleng','27')
```
小技巧，示例代码2，要节省内存40%以上。
**17.行式构造器**
采用行式构造器，可以避免__init__初始化大量参数繁琐写法
self.__dict__.update({k: v for k, v in locals().items() if k != 'self'})
以下两个示例，效果等同，写法不同，很明显采用构造器，更简便
示例1：采用构造器
```
#行式构造器，避免大量初始化
# 示例1：采用构造器
class A(object):
    def __init__(self, a, b, c, d, e, f):
        self.__dict__.update({k: v for k, v in locals().items() if k != 'self'})
    def demo(self):
        print(self.b)
c = A(1,2,3,4,5,6)
c.demo()
# 输出结果：
# D:\python36\python3.exe D:/untitled4/temp7.py
# 2
#
# Process finished with exit code 0
```
示例2：未采用构造器
```
class A(object):
    def __init__(self, a, b, c, d, e, f):
        self.a = a
        self.b = b
        self.c = c
        self.d = d
        self.e = e
        self.f = f
    def demo(self):
        print(self.b)
c = A(1,2,3,4,5,6)
c.demo()
# 输出结果：等同示例1
# D:\python36\python3.exe D:/untitled4/temp7.py
# 2
#
# Process finished with exit code 0
```
**18.字符串首字母大写**
```
str1 = 'apple banana'
print(str1.capitalize())
```
结果：
```
D:\python36\python3.exe D:/untitled4/temp7.py
Apple banana
Process finished with exit code 0
```
**19.with语句块作用**
相当于c#中的used语句块，该语句块结束对象即销毁
```
with open('photo.jpg', 'r+') as f:
    jpgdata = f.read()
```
例如，以上代码，打开了一个文件，with语句结束销毁其对象，不需要人为的f.close()
如果人为的写f.close()还需要考虑到，如果open失败的情况，否则f.close()会出错，因为不存在f对象
20.三元运算
通常三元运算会简化代码，很有必要。
示例1：
```
def demo():
　　is_fat =1
    if is_fat != 1:
        return "not"
    return "ok"
```
以上代码，如果用三元运算通常可以这样写。
```
is_fat = 1
stat = "ok" if is_fat==1 else  "not"
print(stat)
```
示例2:
还有一种写法
```
a = ('abc','def')[False]
print(a)
```
[]中为True时a等于def。为False时a等于abc
G
M
T
||Detect languageAfrikaansAlbanianAmharicArabicArmenianAzerbaijaniBasqueBelarusianBengaliBosnianBulgarianCatalanCebuanoChichewaChinese (Simplified)Chinese (Traditional)CorsicanCroatianCzechDanishDutchEnglishEsperantoEstonianFilipinoFinnishFrenchFrisianGalicianGeorgianGermanGreekGujaratiHaitian CreoleHausaHawaiianHebrewHindiHmongHungarianIcelandicIgboIndonesianIrishItalianJapaneseJavaneseKannadaKazakhKhmerKoreanKurdishKyrgyzLaoLatinLatvianLithuanianLuxembourgishMacedonianMalagasyMalayMalayalamMalteseMaoriMarathiMongolianMyanmar (Burmese)NepaliNorwegianPashtoPersianPolishPortuguesePunjabiRomanianRussianSamoanScots GaelicSerbianSesothoShonaSindhiSinhalaSlovakSlovenianSomaliSpanishSundaneseSwahiliSwedishTajikTamilTeluguThaiTurkishUkrainianUrduUzbekVietnameseWelshXhosaYiddishYorubaZulu||AfrikaansAlbanianAmharicArabicArmenianAzerbaijaniBasqueBelarusianBengaliBosnianBulgarianCatalanCebuanoChichewaChinese (Simplified)Chinese (Traditional)CorsicanCroatianCzechDanishDutchEnglishEsperantoEstonianFilipinoFinnishFrenchFrisianGalicianGeorgianGermanGreekGujaratiHaitian CreoleHausaHawaiianHebrewHindiHmongHungarianIcelandicIgboIndonesianIrishItalianJapaneseJavaneseKannadaKazakhKhmerKoreanKurdishKyrgyzLaoLatinLatvianLithuanianLuxembourgishMacedonianMalagasyMalayMalayalamMalteseMaoriMarathiMongolianMyanmar (Burmese)NepaliNorwegianPashtoPersianPolishPortuguesePunjabiRomanianRussianSamoanScots GaelicSerbianSesothoShonaSindhiSinhalaSlovakSlovenianSomaliSpanishSundaneseSwahiliSwedishTajikTamilTeluguThaiTurkishUkrainianUrduUzbekVietnameseWelshXhosaYiddishYorubaZulu||||||
Text-to-speech function is limited to 200 characters
|||[Options](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?bbl) : [History](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?hist) : [Feedback](moz-extension://906716d0-bcce-4beb-897e-60cd1b012154/content/html/options/options.html?feed) : [Donate](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=GD9D8CPW8HFA2)|Close|

