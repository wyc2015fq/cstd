# python基础知识小结-运维笔记 - weixin_33985507的博客 - CSDN博客
2017年02月14日 16:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
接触python已有一段时间了，下面针对python基础知识的使用做一完整梳理：
1）避免‘\n’等特殊字符的两种方式：
    a）利用转义字符‘\’
    b）利用原始字符‘r’ print r'c:\now'
2）单行注释，使用一个#，如：
#hello Python
多行注释，使用三个单引号（或三个双引号），如：
'''hello python
hello world'''
或
"""hello python
hello world"""
另外跨越多行的字符串。也可以使用三个单引号或三个双引号，如：
'''......'''
或者
"""......"""
3）字符串中嵌入双引号等特殊符号
    a）利用转义字符‘\’
    b）使用单引号括起这个字符串。print ('i l"o"ve fis.com')
4）条件分支：
    if condition:
          条件为真执行的操作
    else:
          条件为假执行的操作
    if condition:
         action
   elif condition:
         action
   else:
        action
python可以有效避免“悬挂else”（if else对应关系出错）
条件表达式（三元操作符）
       small = x if x<y else y
       如果x<y ,small=x.否则small=y
断言assert：当这个关键字后面的条件为假，程序自动崩溃并抛出异常
      assert 3>4
      可以利用他置入检查点
5）while条件:
条件为真执行的操作
for 目标 in 表达式：
循环体
  例：favorite='fishc'
       for i in favorite:
              print(i,end='')
range([start,] stop[,step=1])
生成一个从start参数的值到stop参数值的数字序列
break：终止当前循环体。跳到外层程序
continue:终止本轮循环，开始下一轮循环（if condition true）
6）and逻辑操作符可以将任意表达式连接在一起，并得到一个布尔类型值
7）引入外援:
    a）random模块
    b）randint(),返回一个随机的整数
    import random 或 from random import randint()
    secret=random.randint(1,10)
8）python数据类型
    a）数值类型:整型、布尔类型、浮点型、e记法（1.5e10）
    b）类型转换：
         int()转换为整数
         str()转换为字符串
         float()转换为浮点数
    c）获取关于类型的信息：
         type()函数
            a=520
            type(a)
         isinstance()函数
            a=12
            isinstance(a,int) --->返回true
            isinstance(a,str) -->返回false
9）Python值常用操作符
         + - * / % **（幂运算） //(地板除法,结果偏小)
    比较操作符 > < >= <= 
    逻辑操作符 and or not
         优先级：
             幂运算**
             正负号 + -
             算术操作符 * / // 
                      + -
             比较操作符 < > =
             逻辑擦作福 not and or
10）列表-->可以把整数、浮点数、字符串等打包在一起。数组却不能
      创建一个普通列表：
           member = ['小甲鱼','小布丁','黑夜']
      创建一个混合列表：
           mix=[1,'小甲鱼'，3.12，[1,2,3]]
      创建空列表：
           empty=[]
      向列表添加元素：
           append(): member.append('福禄娃')-->只能添加一个。末尾添加
           extend(): member.extend(['test','test1'])-->只能以列表形式添加.末尾添加
           insert(): member.insert(1,'牡丹')-->第一位插入牡丹
      列表中获取元素：使用索引index。        mix[1]
      列表中删除元素：使用remove()。         mix.remove('小甲鱼')
                            使用del。                  del mix[3]/mix
                            使用pop()。               mix.pop()/mix.pop(1)
      列表切片：使用slice。 mix[1:4]/mix[1:]/mix[:4]
      列表操作符：>,and,+,*,in/not in
      列表的小伙伴：dir(list)
                         mix.count('小甲鱼')
                         mix.index('小甲鱼') 
      列表逆序：使用reverse。     mix.reverse()
      列表排序：使用sort。         mix.sort()
                    mix.sort(func,key)
                    mix.sort(reverse=True)
11）元组--->不可改变的列表
      和列表的主要不同点：
     a）创建和访问一个元组： 大部分用（）/, ；列表用[]
     b）元组不可修改数值
     c）更新和删除一个元组：temp = temp[:2] + ('test3',) + temp[2:]
        del temp
     d）IN/NOT IN，关系操作符，逻辑操作符，乘号操作符，连接操作符
12）字符串的各种内置方法
      str1='i love fishc.com'
      a=str1[:6] + '插入的字符串'+str1[6:]
      capitalize()： str2.capitalize()
      casefold()--->全部小写 str2.casefold()
      center(width)-->居中，不足空格填充
      count(sub[,start[,end]])-->返回sub在string中出现的次数
      endswith(sub[,start[,end]])-->以sub结束？
      startswith(prefix[,start[,end]])-->以prefix开头
      expandtabs([tabsize=8])-->将tab键转换为空格
      find(sub[,start[,end]])-->sub是否在字符串中出现
      rfind(sub)...
      index(sub[,start[,end]])-->跟sub一样，不过会产生异常
      rindex(sub..).....
      istitle()/isupper()/ljust(width)/lower()/strip()/title()/lower()
      join(sub):以字符串做分隔符，分割sub
      partion(sub):找到子字符串sub，把字符串分成一个3元组
      replace(old,new[,count])
      split(sep=none,maxsplit=-1)-->不带参数以空格做分隔符
      swapcase()-->字符串大小写翻转
      zfill(width)-->返回长度为width的字符串，不够补充空格
13）字符串格式化 replacement
     "{0} love {1}.{2:.2f}".format("i","fishc",3.1424)
     "{a} love {b}.{c}".format(a="i",b="fishc",c="com")
     "{0} love {b}.{c}".format("i",b="fishc",c="com")
     格式化符号含义：
         %c:格式化字符及其ASCII码
         '%c %c %c' % (97,98,99)
         %s:格式化字符串
         %d:格式化整数
         %o:格式化无符号八进制数
         %x:格式化无符号十六进制数 %X:...(大写)
         %f:格式化定点数，可指定小数点后的精度
         %e:用科学技术发格式化定点数===%E
         %g:根据值的大小决定使用%f或%e===%G
     格式化操作符辅助命令：
         m.n ：m是显示的最小总宽度，n是小数位精度
         - ：用于左对齐
         + ：在正数面前添加正号
         # ：在八进制面前显示0，在十六进制面前显示0x
         0 ：空格用0填充
     字符串转义字符
         \a:发出系统响铃声
         \b、\t、\n
14）序列
列表、元组和字符串的共同点：
    a）都可以通过索引
    b）索引值从零开始
内置方法：
     list（）-->help-->转换为序列
          list()
             a=list()
          list(iterable)
             b='i love fishc.com'
             b=list(b)
     tuple([iterable])-->把一个可迭代对象转换为元组
             b=tuple(b)
     str(obj)-->把obj对象转换为字符串
     len(obj)-->返回obj的长度
     max(序列/元组) / min(序列/元组)
     sum(iterable[,start=0])-->返回序列iterable。。的总和
     sorted(序列/元组)-->排序
     reversed(序列/元组)-->返回一个迭代器对象
     list(reversed(序列/元组))-->返回序列 
     enumerate(序列/元组)-->返回一个迭代器对象
     list(enumerate(序列/元组))-->返回数组形式列表
     zip（a,b）-->合并成以元组形式的列表
     list(zip(a,b))
15）函数
    定义：def Myfunction():
          print('this is my first function')
    调用：Myfunction()
    函数的参数：
         def Myfunction(name,age):
         print(name+age+'test')
         Myfunction('gncao'，‘age’)
    函数的返回值:
         return value
    形参（parameter）：函数定义时的参数
    实参（argument）：实际传递的参数
    函数文档：在函数中实现
    在函数体中用 '' 或 #
    查看函数文档: a） functionname.__doc__ （四个下划线）
                       b） help(functionname)
    关键字参数：避免参数出乱
         def Myfunction(words,name):
         ......
         Myfunction(words='words123',name='name123')
     默认参数:
         def Myfunction(name='name123',words='words123')
         ......
     收集参数：在参数前面加上*号
        def test(*params):
        print('参数的长度是:',len(params))
        print('第二个参数是:',params[1])
        test(1,'小甲鱼',2,4,5,6,7)
        def test(*params,exp):
        print('参数的长度是:',len(params),exp)
        print('第二个参数是:',params[1])
        test(1,'小甲鱼',23,4,2,5,7,exp=0)
16）函数有返回值，过程没有返回值
17）函数变量作用域（可见性）
局部：local-->函数内部定义的变量，局部可用
全局：global-->全局可访问
当在函数内部试图修改全局变量时，则会在函数内部新建一个跟 全局变量名相同的局部变量
18）内嵌函数和闭包
global关键字：
     def myfun():
          global count ->>>全局变量
          count=10
          print(count)
内嵌函数:
     def fun1():
          print('fun1()正在被调用...')
          def fun2():
                print('fun2()正在被调用')
          fun2()
只能通过fun1()访问fun2()
闭包:在一个内部函数中，对外部函数的变量的引用。成内部函数为闭包
     def funx(x):
         def funy(y):
              return x * y
         return funy
调用方式:
     i=funx(8)
     i(5)
     或
     funx(4)(5)
通过关键字nonlocal可以使得内部函数调用外部函数变量。
     def fun1():
         x=5
         def fun2():
              nonlocal x
              x*=x
              return x
     return fun2()
19，递归：
recursion（）
                def fac(n):
                            if n==1:
                                        return 1
                            else:
                                        return n*fac(n-1)
                number=int(input('请输入一个整数:'))
                result=fac(number)
                print('%d 的阶乘是：%d' % (number,result))
迭代方法：
def fab(n):
    n1=1
    n2=1
    n3=1
    if n <1:
        print('输入有错')
        return -1
    while ( n-2>0 ):
        n3=n2+n1
        n1=n2
        n2=n3
        n-=1
    return n3
result=fab(20)
if result != -1:
    print('总共有%d对小兔子诞生：' % result)
递归方法：
def fab(n):
    if n < 1:
        print('error')
        return -1
    if n==1 or n==2:
        return 1
    else:
        return fab(n-1) + fab(n-2)
result=fab(20)
print('总共有%d对兔子诞生' % result)
但是递归的效率比较低        
20）字典（key-value）
映射/序列
例1：
dict1={'李宁':'一切皆有可能','耐克':'just do it','阿迪达斯':'impossible is nothing'}
print('李宁的口号是:',dict1['李宁'])
例2:
dict3=dict((('f',70),('i',105)))
例3:
dict4=dict(小甲鱼='编程改变世界',test='test')
dict4['小甲鱼']='修改小甲鱼对应的value'     -->如果没有该KEY，则会自动添加一个KEY
字典的内建方法:
a）
dict2['key']-->访问字典的元素
b）
fromkeys(s[,v]) -->创建或查询key
dict1.fromkeys((1,2,3))
{1: None, 2: None, 3: None}
dict1.fromkeys((1,2,3),'number')
{1: 'number', 2: 'number', 3: 'number'}
c）
keys()-->dict.keys() -->打印出dict的所有key
values()-->dict.values() -->打印出dict的所有value
items()-->dict.items() -->打印出dict所有(key,value)
get()--> dict.get(key) -->打印key对应的value
dict.get(key,'text')-->打印key对应的value，如果不存在，则打印text
in操作符 --> key in dict2
clear() -->dict.clear() -->清空dict的数据
copy() -->b=a.copy()    -->拷贝字典
id(a)-->查看id
pop(key) --> dict.pop(key) -->弹出key
popitem() -->dict.popitem()  -->随机弹出key
setdefault() -->dict.setdefault(key)  -->新建key
update()   -->dict.update(dict)   -->更新字典
21）集合 --->唯一性
num={1,2,3,4,5}
set()-->set1=set(列表/元组/字符串)
不支持索引
访问集合中的值:
        使用for循环一一查找        
        使用IN 或者 NOT IN
add()-->set1.add(value)
remove()-->set1.remove(value)
不可变集合:
num3=frozenset(元组/列表)
22）文件
输入-->处理-->输出
内存--->磁盘
open()打开文件:
open('filename/path',mode='rwxabt+U')
文件对象方法:
f.close() -->关闭文件
f.read(size=-1)  -->从文件读取size个字符
f.readline() -->以写入模式打开，如果文件存在，则在末尾添加
f.write(str) -->将str写入文件
f.writelines(seq)  ->向文件写入seq序列。seq应该是一个返回字符串序列
f.tell() -->返回当前的位置。书签
f.seek(offset,from)  -->在文件中移动文件指针，从from偏移offset字节
for each in f:    ---->浏览整个文件
print(each)
23）文件系统
模块：打包的文件系统
os模块：
常用方法：
os.getcwd():返回工作目录
os.chdir(path):改变工作目录
os.listdir(path=''):列出文件目录
os.mkdir(path):创建目录
os.makedirs(path):创建递归目录
os.remove(path):删除文件
os.removedirs(path):递归删除
os.rename(old,new):文件重命名
os.system(command):运行系统的shell命令
os.curdir:指代当前目录.等价于‘。’
os.pardir:指代上一级目录
os.sep:输出操作系统的路径分隔符
os.linesep:当前平台使用的行终止符
os.name:指出当前使用的操作系统
24）永久存储
存放：pickling
读取:unpickling
首先要导入模块pickle
import pickle
>>> my_list=[1,2,3,'test',[23,43]]
>>> pickle_file=open('my_list.pkl','wb')  --》打开一个pickle文件
>>> pickle.dump(my_list,pickle_file) --》把my_list导入到pickle_file
>>>pickle_file.close()
>>> pickle_file=open('my_list.pkl','wb')
>>> my_list2=pickle.load(pickle_file)  -->把pickle_file导入到my_list2
25）异常处理
常见标准异常:
AssertionErron/AttributeError/EOFError/IndexError/KeyError
/NameError/OSError/OverflowError/SyntaxError/TypeError/ZeroDivisionError
捕获异常：
try:
        检测范围
except Exception[as reason]:
        出现异常后的处理代码 print('代码')
except Exception[as reason]；
        出现异常后的处理代码 print('daimai'+ str(reason))        
except (Error1,Error2):
        处理异常的代码
try:
        检测范围
except  Exception[as reason]:
        处理异常的代码
finally:
        无论如何都会处理的代码
raise语句抛出异常
raise Exception('指示代码')
26）丰富的else语句和简洁的with语句
else配合其他语句产生更多的功能
with语句：缩小工作量:
没有使用with前:
try:
   f=open('data.txt','r')
   for each in f:
       print(each)
except OSError as reason:
    print('出错啦:'+str(reason))
finally:
    f.close()
使用with后:
try:
    with open('data.txt','w') as f:
        for each in f:
            print(each)
except OSError as reason:
    print('出错啦:'+str(reason))
27）图形用户界面编程:EasyGui
导入模块三种方式:
a）import   easygui
easygui.msgbox('test')
b）from easygui import *
msgbox('test')
c）import easygui as g
   g.msgbox('test')
建议不要再IDLE上运行EASYGUI
28）类和对象
关键词class
class Turtle:
        #属性
        color='green'
        weight=10
        #方法:
        def climb(self)
                print('climb tree')
调用:
tt=Turtle() -->创建对象
tt.climb() -->调用方法
oo=面向对象
oo的特征：
1，封装
2，继承
        class mylist(list):
                pass --->表示只继承父类，不做其他任何改动        
        list2=mylist()
3，多态
self-->相当于c++的this指针
>>> class ball:
        def setname(self,name):
                self.name=name
        def kick(self):
                print('i am %s,who kicked me????' % self.name)
a=ball()
a.setname('test')
4，_init_(self) --->构造方法
>>> class ball:
        def __init__(self,name):
                self.name=name
        def kick(self):
                print('i am %s,who kicked me????' % self.name)
b=ball('test')
5,公有和私有
类属性和方法默认都是公有的
name mangling --->名字改编，名字重整
私有变量:在变量名或函数名前加上'__'双下划线          
访问私有变量方法:
1,在类内定义方法，间接访问私有变量
2,._类名__变量名
6，继承
class derivedclassname(basename):
        ....
如果子类中出现和父类相同的方法，则覆盖掉父类的方法
不想覆盖掉父类的方法：
1，调用未绑定的父类方法
          def __init__(self):
                fish.__init__(self)   ----》先调用父类同名的方法
                       self.hungry=True
2，使用super函数
          def __init__(self):
                        super().__init__()
                 self.hungry=True        
7，多重继承
class derivedclass(base1,base2,base3):
        ......
8，拾遗 
Mix-in编程机制
类，类对象，实例对象,实例属性（static）
如果属性的名字和方法的名字相同，则属性会覆盖掉方法
绑定:
class bb:
        def printbb():
                print('no zuo no die')
b1=bb()
b1.printbb() ---->会出错误
9，一些相关的BIF：
issubclass(class,classinfo)
1,一个类被认为是自己的子类
2，classinfo可以是类对象组成的元祖，只要class与其中任何一个候选类的子类，则返回TRUE
isinstance(object,classinfo)
检查对象是否属于classinfo类
1，如果第一个参数不是对象，则永远返回fasle
2，如果第二个不是类，则会抛出typeerror的异常
hasattr(object,name)  -->测定object中是否有’name‘属性
hasattr(c1,'x')
getattr(object,name[,default])  -->如果有属性返回1，否则返回default
setattr(object,name,value)  -->给object中的name属性赋值vlalue
delattr(object,name)  -->删除object中的name属性
property(fget=none,fset=none,fdel=none,doc=none)设置属性，设置定义好的属性
获取属性的方法，设置属性的方法，删除属性的方法
class c:
        def __init__(self,size=10):
                self.size=size
        def getsize(self):
                return self.size
        def setsize(self,value):
                self.size=value
        def delsize(self):
                del self.size
        x=property(getsize,setsize,delsize)
c1=c()
c1.x       /    c1.x=19     /c1.size
29）魔法方法（构造和析构）
特点：
1，魔法方法总是被双下划线包围，例如__init__
2，魔法方法是面向对象python的一切
3，魔法方法的魔力体现在他们总能够在适当的时候被自动调用
构造方法：
__init__(self[,...])  -->返回值一定是NONE
用在实例初始化
__new__(cls[,...])    --->第一个初始化的方法
当继承一个不可修改属性的类时，则调用此方法修改属性
class capstr(str):
        def __new__(cls,string):
                string=string.upper()
                return str.__new__(cls,string)
析构方法：
__del__(self)   当数据不适用时，调用此方法
只有在调用了该对象的所有实例都消失时才会产生此方法
30）魔法方法：算术运算
__add__(self,other):定义加法的行为'+'
例子：
>>> class new_int(int):
        def __add__(self,other):
                return int.__sub__(self,other)
        def __sub__(self,other):
                return int.__add__(self,other)
        
>>> a=new_int(3)
>>> b=new_int(8)
>>> a+b   ---->此时a是self，b是other
-5
__sub__(sub,other):减法
__mul__(sub,other):乘法
truediv/floordiv/mod/divmod/pow/lshift/rshift/and/xor/or
divmod(a,b)返回的值是一个元祖：(a//b,a%b)
31）网络socket
socket提供较为底层的网络连接及数据传输功能
tcp socket/udp socket/unix socket
整个通信过程三步走：
a）客户端连接服务器：
导入模块：
import socket
创建tcp类型的socket:
c=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
建立到指定ip地址、端口的tcp连接
c.connect(('211.121.12.43',80))
在系统上使用netstat命令查看新建的连接:
关闭连接：
c.close()
b）服务器端监听：
import socket
s=socket.socket(socket.AF_INET,socket.sock.SOCK_STREAM)
s.bind(('127.0.0.1',80))
s.listen(1)
while True:
        cs,ca=s.accept()  -->创建socket和客户端通信
        cs.sendall('replay')
        cs.close()
c）客户端收发数据:
import socket
c=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
c.connect(('211.121.12.43',80))
向服务端发送‘hello’
c.send('hello')
读取服务端回复数据:
c.recv(1024)
c.close()
32）HTTP库实现HTTP协议
导入httplib模块：
import httplib
创建http实例，指定连接主机名和端口：
http=httplib.HTTPConnection('itercast.com',80)
指定要获取的URI:
http.request('GET','/ask')  -->get方法获取指定数据，ask表示要访问的页面
输出返回的网页body内容：
print http.getresponse().read()
关闭连接：
http.close()
更简单的urllib库：
导入urllib模块
import urllib
创建一个opener的实例
opener=urllib.build_opener()
打开指定的url
f=opener.open('http://www.baidu.com/ask')
读取返回内容
f.read()
33）python连接mysql的模块
import MySQLdb
conn=MySQLdb.connect(user='root',passwd='',host='127.0.0.1') --->连接mysql，默认是localhost
创建游标，通过游标发送sql指令
cur=conn.cursor()
conn.select_db('database-name')  --》连接数据库,本例用week
cur.execute('insert into userinfo(name,age) value('milo',20)') --》执行sql语句。insert
简单化操作方式:
sqli='insert into userinfo(name,age,gender) value(%s,%s,%s)'
cur.execute(sqli,('a',37,'male'))
cur.executemany(sqli,[('test',34,'male'),('test2',36,'female')])
cur.execute('delete from userinfo where id=20') -->删除数据
cur.execute('select * from userinfo') -->查询数据，但是不能直接显示，使用以下方法可以查看
cur.fetchone()-->在python上显示一行数据
cur.scroll(0,'absolute')-->移动指标,此为绝对方式
cur.fetchmany(15)-->在python上显示15条数据.需要输入数据。必须先查询
cur.fetchmany(cur.execute('select * from userinfo')) -->通过一条命令显示数据
cur.close() -->关闭游标的连接
conn.close()   --->关闭数据库连接
