
# [Python学习] 专题一.函数的基础知识 - 杨秀璋的专栏 - CSDN博客

2014年09月08日 05:10:51[Eastmount](https://me.csdn.net/Eastmount)阅读数：5478标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)[基础知识																](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)[函数																](https://so.csdn.net/so/search/s.do?q=函数&t=blog)[网络简单爬虫																](https://so.csdn.net/so/search/s.do?q=网络简单爬虫&t=blog)[httplib2																](https://so.csdn.net/so/search/s.do?q=httplib2&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=网络简单爬虫&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=函数&t=blog)个人分类：[Python基础知识																](https://blog.csdn.net/Eastmount/article/category/2547623)
[
																					](https://so.csdn.net/so/search/s.do?q=函数&t=blog)所属专栏：[Python学习系列](https://blog.csdn.net/column/details/eastmount-python.html)[
							](https://so.csdn.net/so/search/s.do?q=函数&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=基础知识&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=Python&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=Python&t=blog)

**最近才开始学习Python语言,但就发现了它很多优势(如语言简洁、网络爬虫方面深有体会).我主要是通过《Python基础教程》和"51CTO学院 智普教育的python视频"学习,在看视频中老师讲述函数知识的时候觉得非常不错,所以就写了第一篇Python学习的文章分享给大家.主要内容:**
**1.Python安装与基本输入输出,print()函数和raw_input()函数简单用法.**
**2.我根据视频中学到的知识,讲解函数的基本知识:**
**(1).系统提供内部函数:字符串函数库、数学函数库、网络编程函数库、OS函数库**
**(2).第三方提供函数库:讲解如何安装httplib2第三方函数库,再做了个简单的网页爬虫例子**
**(3).用户自定义函数:讲解无返回类型、有形参、预设值参数等自定义函数**
**3.同时网络编程中与C\#以前学过的进行了简单对比,发现python确实有很多优点,而且很方便强大.**
**PS:文章中引用了很多视频中的知识、书籍知识和自己的知识,感谢那些作者和老师,希望文章对大家有所帮助,才开始学习python知识,如果文章中有错误或不足之处,还请海涵,也希望大家提出意见与君共勉.勿喷~**
## 一. Python安装及输入输出函数
**python解释器在Linux中可以内置使用安装,windows中需要去****www.python.org****官网downloads页面下载(如python-2.7.8.amd64.msi),安装****Python集成开发环境(Python
 Integrated Development Environment,IDLE)****即可.运行程序输入">>>print 'hello world'"则python解释器会打印输出"hello world"字符串. 如下：**
![](https://img-blog.csdn.net/20140908034033796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20140908034128343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**然后讲述Python程序的基本框架是"输入-处理-输出",而输入输出函数如下:**
**1.print()函数**
**函数用于输出或打印整型、浮点型、字符串数据至屏幕,如print(3)、print(12.5)、print('H').它输出变量格式"print(x)或print x",而且可以输出多个变量"print x,y,z".并且支持格式化输出数据,调用format()函数,其格式为:**
**print(format(val,format_modifier)) 其中val表示值,format_modifier表示格式字.**
```python
#简单输出
>>> print(12.5)
12.5
>>> print("eastmount")
eastmount
#输出"123.46",6.2表示输出6位,小数点后精度2位,输出最后一位6是四舍五入的结果
>>> print(format(123.45678,'6.2f'))
123.46
#输出"口口口123",采用右对齐补空格方式输出总共6位
>>> print(format(123.45678,'6.0f'))
   123
#输出"123.456780000"小数点后面9位,超出范围会自动补0
>>> print(format(123.45678,'6.9f'))
123.456780000
#输出"34.56%"表示打印百分率
>>> print(format(0.3456,'.2%'))
34.56%
```
**2.raw_input()函数**
**内建函数从sys.stdin接受输入,读取输入语句并返回string字符串.输入以换行符结束,****通过help(raw_input)可以查找帮助,****常见格式为:**
**s = raw_input([prompt]) 参数[prompt]可选,用于提示用户输入.**
```python
#输入函数
>>> str1 = raw_input("please input a string:")
please input a string:I love you
>>> print(str1)
I love you
#查看str1数据类型
>>> type(str1)
<type 'str'>
```
**注意raw_input()与input()的区别：(1).input支持合法python表格式"abc",字符串必须用引号括起,否则会报错"NameError: name 'abc' is not defined",而raw_input()任何类型输入都可接受; (2).raw_input()将所有输入作为字符串,返回string,而input()输入纯数字时具有自己的特性,返回输入的数字类型int或float.举例解释如下:**
```python
#SyntaxError语法错误
>>> str1 = input("input:")
input:abc
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "<string>", line 1, in <module>
NameError: name 'abc' is not defined
#正确输入输出
>>> str1 = input("input:")
input:"I love you"
>>> print str1
I love you
#input纯数字 数据类型
>>> weight = input("weight:")
weight:12.5
>>> type(weight)
<type 'float'>
#raw_input 数据类型均为str
>>> age = raw_input("age:")
age:20
>>> type(age)
<type 'str'>
```
## 二. 函数之系统提供内部函数
**python提供的系统内部库函数主要讲述一下四种类型:(引自视频,只简单介绍)**
**1.字符串函数库**
**通过help(str)可以查询字符串函数库,其中查询过程中"-More-"输入回车实现滚动信息,输出"q"退出帮助(Quit).字符串函数大家都比较熟悉,无论C\C++\C\#\Java都学过很多,而且基本大同小异.如:**
**islower()函数判断字符串是否大小写,一个大写返回False.前面使用的format()函数、求字符串长度len()函数均属于字符串函数库,help(str.replace)可以查询具体函数用法,该函数用于新字符串替换旧字符串.**
```python
#判断字符串是否小写
>>> str1 = "Eastmount"
>>> str1.islower()
False
#字符串替换
>>> str2 = 'adfababdfab'
>>> str2.replace('ab','AB')
'adfABABdfAB'
#字符串求长度
>>> print(len(str2))
11
>>>
```
**2.数学函数库**
**使用数学函数库时需要注意的是导入库"import math",该库中函数我们也非常熟悉,如sin()求正弦,cos()求余弦,pow(x,y)计算x的y次幂,如pow(3,4)=3*3*3*3,python中也可以使用3**4表示.help(math)中可以查看详细信息,而且库中定义了两个常数DATA:**
**e = 2.718281... pi = 3.14159265...**
```python
#导入math库
>>> import math
>>> print math.pi
3.14159265359
#计算sin30度
>>> val = math.sin(math.pi/6)
>>> print val
0.5
#pow函数
>>> math.pow(3,4)
81.0
>>> 3**4
81
>>> help(math.pow)
Help on built-in function pow in module math:
pow(...)
    pow(x, y)
    Return x**y (x to the power of y).
>>>
```
**3.网络编程库**
**系统提供内部库函数中网络编程库,我此处仅仅举个简单例子,socket(套接字网络编程)获取主机ip地址这是非常常见的运用,我将与C\#网络编程进行简单对比.后面的博文中将详细讲述python网络编程.**
```python
>>> import socket
>>> baiduip = socket.gethostbyname('www.baidu.com')
>>> print baiduip
61.135.169.125
```
**其中socket编程很常用,gethostbyname()返回指定主机ip,而C\#中获取百度网址的ip地址代码如下所示.代码中可能会出现"警告:Dns.GetHostByName()函数已过时",可替换为IPHostEntry myHost = Dns.GetHostEntry(****www.baidu.com****).输出：**
**61.135.169.121**
**61.134.169.125**
```python
//引用新命名空间
using System.Net;
namespace GetIp
{
    class Program
    {
        static void Main(string[] args)
        {
            //获取DNS主机名的DNS信息
            IPHostEntry myHost = Dns.GetHostByName("www.baidu.com");
            IPAddress[] address = myHost.AddressList;
            for (int index = 0; index < address.Length; index++)
            {
                Console.WriteLine(address[index]);
            }
            Console.ReadKey();    
        }
    }
}
```
**4.操作系统(OS)函数库**
**操作系统库函数引用"import os",举例获取当前工作路径和先死当前路径下的文件和目录.使用os.system("cls")可以实现清屏功能.安装python目录Lib文件夹下含有很多py库文件供使用.**
```python
>>> import os
#获取当前工作路径
>>> current = os.getcwd()
>>> print current
G:\software\Program software\Python\python insert
#获取当前路径下的文件和目录
>>> dir = os.listdir(current)
>>> print dir
['DLLs', 'Doc', 'h2.txt', 'include', 'Lib', 'libs', 'LICENSE.txt
', 'NEWS.txt', 'python.exe', 'pythonw.exe', 'README.txt', 'tcl', 'Tools']
>>>
```
## 三. 函数之第三方提供函数库及安装httplib2模块过程
**(一).安装第三方函数库httplib2过程**
**Python中第三方开源项目中提供函数库供我们使用,比如使用httplib2库函数.在Linux中直接使用"easy_install httplib2"搜索自动安装,Windows下python开发工具IDLE里安装httplib2模块的过程如下(其他模块类似).**
**1.下载httplib2模块"****https://code.google.com/p/httplib2/****"到指定目录,解压压缩包"httplib2_0.8.zip"到某目录下,如果该网址google访问失败,可以到此处下载:**
**2.配置python运行环境**
**右键"计算机"->"属性"->在"系统"中选择"高级系统设置"->在"系统属性"中"高级"选择"环境变量"**
![](https://img-blog.csdn.net/20140906033841800?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**在系统环境变量Path后添加python安装目录"G:\software\Program software\Python\python insert"**
![](https://img-blog.csdn.net/20140906033852734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**3.在dos下安装httpLib2**
**管理员模式运行cmd,利用cd命令进入httplib2_0.8.zip解压目录,输入"****python settup.py install****",如下图所示安装成功.**
![](https://img-blog.csdn.net/20140906034150859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**4.使用httplib2**
**如果httplib2库函数没有安装成功,"import httplib2"会提示错误"ImportError: No module named httplib2".否则安装成功,举例如下.看参考更多"****Python - 熟悉httplib2****"**
```python
import httplib2
#获取HTTP对象
h = httplib2.Http()
#发出同步请求并获取内容
resp, content = h.request("http://www.csdn.net/")
#显示返回头信息
print resp
#显示网页相关内容
print content
输出头信息:
{'status': '200', 'content-length': '108963', 'content-location': 'http://www.csdn.net/', .... 'Fri, 05 Sep 2014 20:07:24 GMT', 'content-type': 'text/html; charset=utf-8'}
```
**(二).简单网页爬虫示例**
**使用第三方函数库时的具体格式为module_name.method(parametes) 第三方函数名.方法(参数).**
**讲述一个引用web库,urllib库访问公网上网页,webbrowser库调用浏览器操作,下载csdn官网内容并通过浏览器显示的实例.**
```python
import urllib
import webbrowser as web
url = "http://www.soso.com"
content = urllib.urlopen(url).read()
open("soso.html","w").write(content)
web.open_new_tab("soso.html")
```
**它会输出True并在浏览器中打开下载的静态网页.引用import webbrowser as web使用web,也可以直接引用,使用时"****module_name.method****"即可.读者可以对比我的C\#文章"****C\#
 网络编程之网页简单下载实现****".**
**content = urllib.urlopen(url).read()表示打开url并读取赋值**
**open("soso.html","w").write(content)表示在python安装目录写静态soso.html文件**
**web.open_new_tab("soso.html")表示打开该静态文件新标签.**
**同样可以使用web.open_new_tab('http://www.soso.com')直接在浏览器打开动态网页.效果如下图所示:**
![](https://img-blog.csdn.net/20140906044133187)![](https://img-blog.csdn.net/20140906044453270)
## 四. 函数之自定义函数
**1.无返回值自定义函数**
**其基本语法格式如下:**
**def function_name([parameters]):****函数名([参数]),其中参数可有可无**
**(TAB) statement1**
**(TAB) statement2**
**...**
**注意:(1).自定义函数名后面的冒号(:)不能省略,否则报错"invalid syntax",而且无需像C语言一样加{};**
**(2).函数里每条语句前都有缩进TAB,否则会报错"There's an error in your programs:expected an indented block",它的功能相当于区分函数里的语句与函数外的语句.**
**举例:打开IDLE工具->点击栏"File"->New File新建文件->命名为test.py文件,在test文件里添加代码如下.**
```python
def fun1():
    print 'Hello world'
    print 'by eastmount csdn'
print 'output'
fun1()
def fun2(val1,val2):
    print val1
    print val2
fun2(8,15)
```
**保存,在test.py文件里点击Run->Run Module.输出结果如下图所示,其中fun1()函数无形参自定义函数,fun2(val1,val2)是有形参自定义函数,fun2(8,15)为函数的调用,实参8和15.**
![](https://img-blog.csdn.net/20140908040757171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.有返回值自定义函数**
**其基本语法格式如下:**
**def funtion_name([para1,para2...paraN])**
**statement1**
**statement2**
**....**
**return value1,value2...valueN**
**返回值支持一个或多个返回,需要注意的是自定义函数有返回值,主调函数就需要接受值(接受返回的结果).同时在定义变量时可能sum这些可能是关键字(注意颜色),最好使用不是关键字的变量.举例：**
```python
def fun3(n1,n2):
    print n1,n2
    n = n1 + n2
    m = n1 - n2
    p = n1 * n2
    q = n1 / n2
    e = n1 ** n2
    return n,m,p,q,e
a,b,c,d,e = fun3(2,10)
print 'the result are ',a,b,c,d,e
re = fun3(2,10)
print re
```
**输出结果如下,其中需要注意的是参数一一对应,在除法中2/10=0,**幂运算2的10次方=1024.而使用re=fun3(2,10)直接输出的结果成为元组,后面会详细讲述,(12,-8,20,0,1024)元组中re[0]存储12,re[1]存储-8,依次~**
```python
2 10
the result are  12 -8 20 0 1024
2 10
(12, -8, 20, 0, 1024)
```
**3.自定义函数参数含预定义**
**预设值的基本格式如下:**
**def function_name(para1,para2...parai=default1...paran=defaultn)**
**statement1**
**statement2**
**...**
**return value1,value2...valuen**
**其中需要注意的是预定义值得参数不能先于无预定义值参数,举个例子讲解.**
```python
def fun4(n1,n2,n3=10):
    print n1,n2,n3
    n = n1 + n2 + n3
    return n
re1 = fun4(2,3)
print 'result1 = ',re1
re2 = fun4(2,3,5)
print 'result2 = ',re2
```
**输出结果如下图所示,预定义的参数在调用时,实参可以省略,也可以替换默认定义的预定义值.**
```python
2 3 10
result1 =  15
2 3 5
result2 =  10
```
**其中如果函数定义为def fun4(n3=10,n2,n1)就会报错"non-default argument follows default argument"(没预定义的参数在预定义参数的后面),所以定义函数时需要注意该点.**
**同时需要注意函数调用时的赋值顺序问题,最好采用一对一复制,也可以函数调用中给出具体形参进行赋值,但需要注意的是在函数调用过程中(使用函数时),有预定义值的参数不能先于无预定义值参数被赋值.**
**如例子中自定义函数def fun4(n1,n2,n3=10),在调用时可以:**
**(1).s=fun4(2,3)省略预定义参数,它是一对一赋值,其中n1赋值2、n2赋值3、n3赋值10(预定义)**
**(2).s=fun4(4,n2=1,n3=12)它也是一对一赋值,而且预定义值n3替换为12**
**(3).s=fun4(n2=1,n1=4,n3=15)它的顺序与定义函数没有对应,但是调用时给出具体对应也行**
**下面的情况就会出现所述的"****有预定义值的参数先于无预定义值参数被赋值****"错误:**
**(1).s=fun4(n3=12,n2=1,4)此时会报错"non-keyword arg after keyword arg",它不能指定n1=4,就是没有指定预定值n1=4在有预定值n3=12,n2=1之后,如果改成s=fun4(4,n2=1,n3=12)或s=fun4(4,n3=12,n2=1)即可.**
**(2).s=fun4(4,n1=2)此时也会报错"TypeError: fun4() got multiple values for keyword argument 'n1'",它不能指定n1=2&n2=4,而是n1会赋值多个.**
**所以,最好还是一一对应的调用函数,平时写程序没有这样去刁难自己,对应即可**
**总结:****文章从系统提供的内部函数、第三方提供函数库+简单爬出代码及安装httplib2模块过程和用户自定函数三个方面进行讲述.文章中如果有错误或不足之处,海涵~最后感谢那个视频老师和上述博主、书籍老师及me.**
**Python 公开课视频链接****http://edu.51cto.com/course/course_id-527.html**
**最后祝大家中秋节快乐!全家团圆,聚聚玩玩.**
**(By:Eastmount 2014-9-8 夜5点 原创CSDN****http://blog.csdn.net/eastmount/****)**


