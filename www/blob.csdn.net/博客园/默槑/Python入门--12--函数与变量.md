# Python入门--12--函数与变量 - 默槑 - 博客园







# [Python入门--12--函数与变量](https://www.cnblogs.com/modaidai/p/6869122.html)





python只有函数没有过程

函数有运行完了之后会有返回值、过程没有

def back():

　　return 1,'gg',2　　　　#会有返回值(1,'gg',2) #注意默认返回元祖

一、全局和局部变量

在函数里面想改变全局变量的值，需要加上：global XX列表或单一值

二、内嵌函数

def ee():

　　print "123"

　　def ww()

　　　　print "456"

　　ww()　　　　　　　　#最后在调用一下  你就可以在命令行输入ee()  会有123 456输出 

在外面调用ww() 是无法调用的 

三、闭包

函数式编程的重要语法结构，函数式编程是一种编程方式，其最经典的一个编程语言就是lisp 主要用于绘图与人工智能，天才程序员才使用的语言

闭包概念：如果在一个内部函数里，对外部作用域（不是在全局变量的引用），那么内部函数就被认为闭包

def Funx():

　　def FunY(y):

　　　　return x * y

　　return FunY

可以这样用：

i = FunX(8)

i(5)　　　　　　　　　　#先赋值x 再赋值y 最后输出40

在内部函数中只能调用外部函数的局部变量，但不能修改

 还有调用的注意，看下面的这个栗子：

x=10
def www():
    　　x *= x

　　return x　　　　　　　#发现会报错，因为x=10是全局变量，而自定义函数里面的x是局部变量的x，

print www()　　　　　　   #跟外面的x是不一样的



 同样下面的这种引用也会报错：

def Fun1():

　　x = 5 

　　def Fun2():

　　　　x *= x

　　　　return x

　　return Fun2()

现在输入Fun1()会报错，因为python会把Fun2()函数中的x当做内部函数的变量，不会引用外部函数的变量



避免这样的方法以前是这样做：

def Fun1():

　　x = [5] 

　　def Fun2():

　　　　x [0]*= x[0]

　　　　return x[0]

　　return Fun2()　　　　　　　　#把数据的容器变一下，变为列表

python3现在不用那么麻烦，直接价加一个东西就好了：

def Fun1():

　　x = 5 

　　def Fun2():

　　　　nonlocal x　　　　　　　　#变为非局部变量，就可以在内部函数里面使用外部函数的变量

　　　　x *= x

　　　　return x

　　return Fun2()
#在函数中修改全局变量的值，使用global xx变量

#在嵌套的函数中，想在内部函数中修改外部函数的局部变量 使用nonlocal

##################################################################

四、匿名函数：lambda

python写一些执行脚本时，使用lambda就可以省下定义函数的过程，使得代码更简单，还可以避免重复，简化代码可读性



def ww(x):

　　return x * x　　　　#两行代码

lambda x : x*x　　　　　#几个字就解决　　　还可以 g = lambda x : x*x  就可以使用 g(x)来替代这个式子



五、介绍两个BIF

1、filter（函数，可迭代的对象列表元祖什么的）  #有两个参数,其返回结果会是使用函数在对象中的每个元素进行迭代

　　　　　　　　　　　　　　　　　　　　　#并返回为TRUE的列表

list(filter(None,[1,0,Flase,True]))　　　　#会返回[1,True]



list(filter((lambda x: x %2),range(10)))　　#自定义一个函数：用x除以2 得到余数，范围是1到10

　　　　　　　　　　　　　　　　　　　　　　#用filter处理，并生成list



2、map()

map在语言里面是映射的意思。将序列的每一个元素作为函数的参数，并进行迭代加工

list(map(lambda x : x*2,range(10)))　　　　#生成0,2,4,6,8,10,12,14,16,18



六、补充

def funOut():
    　　def funIn():
        　　print('宾果！你成功访问到我啦！')
    return funIn

 上面的函数怎么能访问funIn？

第一种方法：funOut()()

第二种方法：go = funOut()

*　　　　　　go()*



下面介绍一个很好的方法，用于看有很多符号的字符串中的字母、数字

str1='''xxx'''　　　　　　　　　　#str1可以很长很长，有许多你看不懂的￥#！@%&***￥#，其中夹杂着字母数字
for each in str1:
    　　if each not in list1:
        　　　　list1.append(each)　　　　#输出的是单一的‘@’　　‘#’　　‘￥’　　‘%’　　‘……’　　‘&’　　‘&’　　‘*’
print (list1)　　　　　　　　　　　　#情况一目了然



可以继续改良一下：

**str1 = '''拷贝过来的字符串'''**
**list1 = []**

**for each in str1:**
**    　　if each not in list1:**
**        　　　　if each == '\n':**
**            　　　　　　print('\\n', str1.count(each))**
**        　　　　else:**
**            　　　　print(each, str1.count(each))**
**        list1.append(each)**












