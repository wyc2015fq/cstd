# Python入门--11--自定义函数 - 默槑 - 博客园







# [Python入门--11--自定义函数](https://www.cnblogs.com/modaidai/p/6867584.html)





使用def定义自定义函数

举个栗子：

def myfristFunction():

　　print "we are 伐木累！"　　　　　　#输入myfristFunction() 　　会输出：we are 伐木累！



def mysecondFunction():

　　print "we are world"

　　return

　　print "ni gun dan "　　　　　　　　#函数不会输出第二个print 只会输出第一个



def mythirdFunction((x,y),(e,t)):

　　print (x,y,e,t)　　　　　　　　　　　　#函数的参数不能使列表、元祖

举个绕一点的 栗子

def gcd(x, y):
    　　while y:
        　　t = x % y
        　　x = y
        　　y = t

    　　return x
print(gcd(4, 6))　　　　　　#利用欧几里得算法求最大公约数

#####################################################################

一、函数文档在哪？

在自定义函数的过程中，我们需要在里面加入我们的帮助文档，以便后人使用

def mybuzhijigeFunction:

　　'这是啥用都没有的函数，你不用看我写的任何话'　　　　　　#这就是帮助文档

　　print 'zhe li shi zhong guo '　　　　　　　　

二、查看帮助文档

mybuzhijigeFunction.__doc__　　　　　　#会输出我上面写的那句话

help(mybuzhijigeFunction)　　　　　　　　#也可以

不光自定义的、还有BIF函数也可以

print.__doc__　　　　　　　　　　　　　　　#会有帮助文档



三、关键字参数

mybuzhijigeFunction(name,word)

　　print (name + "->" + word)　　　　　　

#使用这个函数的时候可以mybuzhijigeFunction(name='daidai',word='we are famliy')

#这就是关键字参数



四、默认参数

def mybuzhijigeFunction(name='daidai'.word='we are famliy')

#使用的时候可以不带参数

五、收集参数



def mybuzhijigeFunction(*aaa)

　　print ('参数的长度是：' , len(aaa))

　　print ('第二个参数是：',aaa[1])

输入mybuzhijigeFunction(456,65,'gr',66)　　会有4 和 65输出来

蛋是要注意：如果你想在收集参数后面继续加参数，那就必须是默认参数和关键字参数，系统会把所有不是后两者的参数默认为要收集的参数

print就是一个有收集参数的函数



六、多学点没坏处

//：是地板除，只取整数部分

%：取余数都知道，蛋是，当前面的数比后面的小的时候，取这个前面的数为结果

　　　　4%5 =4  　　5%333333=5












