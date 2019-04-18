# python入门基础--数据类型--循环控制--函数--OOP - wsp_1138886114的博客 - CSDN博客





2018年05月17日 08:34:21[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：364
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









## 一、Python基本数据类型
                    集合，数字，字符串，列表，元组，字典（集合几人捏圆子）

                    python基本数据类型：number, None,  bool, str
                    python容器类型：    list,   tuple, dict, set


### 二、print格式化输出
                print 连接符： + ，
                print ''.format()

                eg:    


```python
print('他的身高是%-8.2f厘米' % 180.222)
             print('%s的身高是%-8.2f厘米' % ('her',180222))
                    
             print('{}的身高是{}厘米'.format('her',180222))
             print('{0}的身高是{1}厘米'.format('her',180222))
             print('{1}的身高是{0}厘米'.format(180222,'her'))
```
    占位符：             %d整数        %s字符串        %f浮点型
    占位符宽度：      %10d    占10个位置的整数，    默认居右
                             %-10d    占10个位置的整数，    默认居左

    format:            '{:20}{:24}{:28}'.format('str1','str2','str3')
                            '{:^20}{:^24}{:^28}'.format('str1','str2','str3')#占位符居中

    连接符：        print('',sep = '-')

    输出：            raw_input()        得到字符串        #只能在python2.x用
                          input()            得到处理后的结果，一般是number类型


### 三、中文输出问题：
                    import json

                    print(json.dumps(tup/dict).decode("unicode-escape"))
                    print 格式化字符串  %d(整型)    %s（字符串）    %f（浮点型）


### 四、数据类型
    list:            list[]        可以存放number    bool    str1    list    tuple
                     list()        将str转换成list

                    append()    在list结尾添加新元素
                    del()           删除元素，一个，区间元素，删除整个列表
                                      del list1[2:]        del list2[:5]
                    读取list        list[1]    list[-1]    list[1,3]

    tuple()元组         tuple1 = (,)    tuple2 = "a","b","c"    
                                    注意：a = ('hello')不是元组
                                              a = ('hello',)才是

                                    不可更改元素里面的值
    dict{}        创建字典(几种方法)：                    
```python
dict1 = {}                                         #1.创建空字典
            dict1['name3'] = 'wangmazi'  
    
            dict2{'name1':'zhangsan",'name2':'lisi'}           #2.直接赋值创建
    
            tup1 = (['name4','daaizi'],['name5','dsssaaizi'])  #长度只能是两个
            dict3 = dict(tup1)                                 #3.通过二元组列表创建
    
            dict4 = dict(zip('abc', [1, 2, 3]))                #4.dict和zip结合创建
    
            dict5 = {i:2*i for i in range(3)}                  #5.通过字典推导式创建
  
            {}.fromkeys()                                      #6.通过dict.fromkeys()创建
            dict6 = {}.fromkeys(('key1','key2'),value1)
```
                    读取字典：print dict1['name1']
                    更改字典：dict2['name2']
                    删除字典：del dict1['name4']    
                                      dict2.clear
                                      del dict3 
                    获取字典： key,value            
                                      dict.keys()
                                      dict.values()      


    set()        创建集合set()： 


                   set是一个无序且不重复的元素集合             #通过set() 函数创建 


```python
s1 = set("python")                          #把字符串变为集合
                print(s1)

                s2 = set([123,"python","face","book","face"])#把列表变为集合    
                print(s2)

                s3 = set({"face",123})                       #把字典变为集合
                s4 = set{'name1':'zhangsan','age':[12,34,56]}
                print(s3,s4)
```
                  将有重复元素的列表转为集合时，将无序排列，且自动去重
```python
set1={11,22,33}
            set2={22,55}
            difference()方法
                   set1.difference(set2)            #找到set1中存在，set2中不存在的集合，返回新的集合
                   set1.difference_updata(set2)     #找到set1中存在，set2中不存在的集合，覆盖set1
            intersection()方法
                   set1.intersection(set2)          #取交集，返回新的集合
                   set1.intersection_updata(set2)   #取交集并更新自己
            判断：isdisjoint()、issubset()、issuperset()
                   set1.isdisjoint(set2)            #判断是否不存在交集
                   set1.issubset(set2)              #判断set1是否时set2的子集合
                   set1.issuperset(set2)            #判断set1是否时set2的父集合
            合并：symmertic_difference()
                   set1.symmertic_difference(set2)           #取并集，并返回新的集合
                   set1.symmertic_difference_update(set2)    #取并集，并更新自己
                   set1.updae(iterable)
```

### 五、循环控制  

     -->for循环与while循环
        for ...  :        while  ... :
        else:            else:
    -->for循环遍历两个列表：zip()函数
    zip()函数:


               zip()函数，将对象(可迭代)打包成一个个元组。然后返回由这些元组列表，则返回的列表长度由最短的对象决定。利用"*"操作符，可以将元组解压为列表。在python3.x版本中，将不再支持打印zip()函数的内容，返回值为一个内存地址

```python
list1=[1,2,3,4]
        list2=['a','b','c']
        zip(list1,list2)    -> [(1,'a'),(2,'b'),(3,'c')]

        for a,b in zip(list1,list2):
        print (a,'is', b)
```

     --> enumerate()


```python
list1=['a','b','c','d','e','f','g']
            for index,letter in enumerate(list1):    #可用于同时获取索引和元素
            print (index,'位是：',letter)
```

    -->循环控制
    break    #一旦条件满足跳出整个循环
    continue    #一旦条件满足，跳过本次循环
    pass    #仅为保持结构完整性




    多值 互换:        a,b = b,a        #将b赋给a,将b赋给a

                          a,b,c = c,a,b    #将c赋给a,将a赋给b,将b赋给c    


判断输入的是否是number:            
                    .strip()    默认去掉开头结尾的空格
                    .strip('-')    指定去掉('-')符号
                    .isdigit() 判断输入的是否是number:
                    用法：if a.strip().isdigit()

    三元运算取最大值：

                            a = x if x>y else y
    range()函数      range(10)           #取0-9                     
                             range(2,9)          #取2-8            
                            range(3,100,3)    #取3-99，步长为3

    升级python（有多个版本的时候）                            
    win+r 输入cmd 执行命令            py -2 -m pip install --upgrade pip
                                                       py -3 -m pip install --upgrade pip
    bool值为false：None,     (),     0,

    执行 .py文件    cmd> pyton name1.py

    输出1-100平方    print [x**2 for x in range(1,11)]
                               b = [x**2 for x in range(1,11)]
                               print (b)

    在python中，操作文件路径："./" :表示当前层，"../" :表示上一层 

    推导式：        list = [item for i in range(iterable)]

```python
print [x for x in range(1,20,2)]                               #输出0-20的奇数
        print [x for x in range(20) if x%2 ==0]                        #输出0-20的偶数，有判断的
        print [index,item for index,item in enumerate(list(str))]      #遍历输出列表

        print [x for x in list[]]                                      #直接遍历列表
```
    python 参数传递    
                    Tuples string number 值传递不可更改
                    list         等参数传递可以改变

### 六、函数        
    定义:            def functionname(arg1,arg2,*args,**kwargs):
                        执行语句
                        return                #本身结束函数，后面的所有语句都不会执行
                                                  #返回值
                                                  #可用于闭包，返回函数
                    执行函数：

                    functionname(arg1,arg2,a =2,c='33')                       


```python
def functionname1(num1,num2,*args,**kwargs):
				sum1 = num1+num2+sum(args)
				for i in args:
					pass
				return sum1
			functionname1(1,2,3,4,5,a = '12',b = '23')
```

    参数传递   必要参数    def funcA(str):
                    关键参数    def funcB(str = ''):
                    缺省参数    def funcC(x,y,z = 15):
                                                    funcC(20,10)
                    不定长参数    def funcD(str1,str2,*args,**args):
                                        *args    元组形式(a,b,c,d)
                                        **args    字典形式(a = 'b',c = 'd')
                    ----------------------------------------------------------------------------------                    
                    值传递不可更改    str  number    tuple
                     引用传递        list [:]    dict    #list[:]，复制时不会修改原list

    调用函数        functionname()    里面放参数，否则报错

    变量            全局变量    total
                       局部变量    def funcA():内的变量
                                若想使用全局变量 global total

                    模拟range()、sum()方法
                        return 返回值
                        每个函数执行的结果都是默认的 return
                        return 结果
                        return 固定的
                        return None
                        return 结束函数


####     lambda(匿名函数):

```python
filter(functionname,list[])
                list1 = [1,2,3,4,5,6,7,8,9,10]
                print(list(filter(lambda x:x%2==0,list1)))
    
            map(functionname,list[])
                list1 = range(10)
                list2 = map(lambda y:y**2,list1)
                print list2        #[0, 1, 4, 9, 16, 25, 36, 49, 64, 81]

                list3 = map(lambda x,y:y+x,[1,2,3],[4,5,6])
                print list3        #[5, 7, 9]
                
            reduce(functionname,list[])         #reduce读取前两个list元素
                list2 = reduce(lambda x,y:x+y,[1,2,3,4,5])
                print list2

            list3 = reduce(lambda x,y:x+y,[2,3,5,2],1)
                print list3
```

####     闭包函数        

```python
def funA(x):
                aa = 10
                def funR():
                return 2*aa+20
             return funR()
```

```python
print('------------利用闭包生成函数--------------')
            def line_corf(a,b):
                def line(x,y,z = 5):
                    return a*x+b*y+z
                return line
            line1 = line_corf(1,2)(2,4,1)    #举例运行
            print (line1)                    #运行结果x+2y+z;11
            line2 = line_corf(5,2)           #生成函数
```
    对函数添加属性

```python
print('----------对函数添加属性-----------')
            def funcF(x, y, z):
                print(x**2, y**2, z**2)
            funcF.alert = "hello python"
            funcF.hobby = ['play', 'eat']
            funcF.pp = lambda: "hello word"
            funcF(1, 2, 3)                      #运行结果: 1,4,9
            print (funcF.alert)                 #运行结果: hello python
            print (funcF.hobby)                 #运行结果: ['play', 'eat']
            print (funcF.pp())  # 匿名函数加括号  #运行结果: hello word
```

####  deractor（装饰器）                
                    扩展已有函数功能
                    装饰器：python函数熟练使用的分水岭            

```python
def funA(funC):
                    def wrapper():
                        1.
                        f()
                        2.
                        3.
                        return f
                    return wrapper
                @funA
                def selfFun():
                    pass
        约等于  funA(selfFun)
#实例:
            def deco(func):
                def wrapper(a,b):
                    startTime = time.time()
                    func(a,b)
                    time.sleep(1)
                    endTime = time.time()
                    msecs = (endTime - startTime)*1000
                    print("运行时间： %f ms" %msecs)
                return wrapper
            @deco
            def func(c,d):
                print("运行结果是： %d" %(c+d))
            f=func
            f(3,4)
```

### 七、OOP(面向对象)               

```python
class Meinv:
                count = 0                                #类的属性
                sex = female
                __pricvate_value = '秘密'
                def __init__(self,name,sex,age，aim):    #每次实例化都会运行
                    self.name = name
                    self.age = age
                    self.aim = __pricvate_value.aim
                    Meinv.count+=1
                def goodOpera():
                    print '我会qingqishuhua'
                def showCount():
                    print '已经有',Meinv.count,'beautiful girl'
                    
            #实例化对象
            girl = meinv(diaocan,while)
            girl.goodOpera()
            Meinv.goodOpera()
            
            gir2 = meinv(wangzhanjun,while)
            gir2.goodOpera()
            Meinv.showCount()
            Meinv.goodOpera.name
```

私有变量特征：  __pricvate_value    外部不能访问
    公有变量特征：  public                    外部可以访问
                    getattr(object，'name',...)         获取属性
                    setattr(object,'name',value)      设置属性
                    hasattr(object,'name')              是否有这个属性
                    delattr(object,'name')              删除属性

    函数类可以使用外部函数
                    def aaa():
                        range()
                    不要轻易的让
                        a = aaa()    这不是函数赋值，这是函数的结果

                        a = aaa     这才是函数

### 八、继承与多重继承

    定义类并继承：

```python
实例1：
        class Student(object):             #这是一个学生类
            schoo1='清华大学'               #类的属性，所有类的对象共享
            count=0                        #用于计数
            def __init__(self,name,age,score):    #类的构造函数或初始化方法，
                self.name=name            #实例化对象的时候，__init__()方法都会默认被运行
                self.age=age
                self.score=score
                self.count+=1               #对象计数，对象自己的属性
                Student.count+=1            #每有对象产生时+1，类的属性
            def counter(self):              #定义计数方法
                print (self.name+'是'+self.schoo1+'第'+str(self.count)+'个学生！')
                print('目前学校有',Student.count,'个学生！')
                
        stu1=Student('张三',20,98.5)            #实例化对象
        stu2=Student('李四',21,89.5)
        stu3=Student('王五',21,89.5)
        stu1.counter()        # 张三是清华大学第1个学生！\n目前学校有 3 个学生！
        stu2.counter()        # 李四是清华大学第2个学生！\n目前学校有 3 个学生！
        stu3.counter()        # 王五是清华大学第3个学生！\n目前学校有 3 个学生！

    实例2  继承实例1:
        class HighSchool(Student):
            schoo1='清华附中'                                #修改派生类的类属性
            def __init__(self,name,age,score,uniform):      #派生类构造方法
                super(HighSchool, self).__init__(name,age,score) #继承自基类固定写法 #表示继承基类的name,age,score属性(须全部写一遍)
                self.uniform=uniform            #继承类新增的属性
        
        stu1=Student('张三',20,98.5)            #基类对象1
        stu2=Student('李四',21,89.5)            #基类对象2
        stu1.counter()                    #基类对象调用基类counter()方法
        stu2.counter()
        hs1=HighSchool('小明',12,98.5,'L')      #派生类对象1
        hs2=HighSchool('小红','13',98.5,'M')    #派生类对象2
        hs1.counter()                     #派生类没有覆写基类方法，故调用基类方法
        
        # 输出结果：
        # 张三是清华大学第1个学生！
        # 目前学校有 2 个学生！
        # 李四是清华大学第2个学生！
        # 目前学校有 2 个学生！
        # 小明是清华附中第3个学生！
        # 目前学校有 4 个学生！
```

    类的属性

        操作类的属性
            getattr(obj,'name'[,default])    访问对象的属性
            hasattr(obj,'name')                检查是否存在一个属性
            setattr(obj,'name')                设置一个属性，如果属性不存在，会创建一个新属性
            delattr(obj,'name')                删除属性
        内置类属性
            __dict__         类的属性（包含一个字典，由类的数据属性组成） 
            __doc__         类的文档字符串 
            __name__         类名 
            __module__         类定义所在的模块 
            __bases__          类的所有父类构成元素（包含了一个由所有父类组成的元组
        类的私有属性和私有方法:
            类的私有属性 __private_attrs：
                两个下划线开头，声明该属性为私有，不能在类的外部被使用或 直接访问。
                在类内部的方法中使用时 self.__private_attrs。 
            类的方法: 
                在类的内部，使用 def 关键字可以为类定义一个方法，
                与一般函数定义不同，类方法必须包含参数 self,且为第一个参数 
            类的私有方法__private_method：
                两个下划线开头，声明该方法为私有方法，不能在类地外部 调用。
                在类的内部调用 self.__private_methods


附加：python中不支持多态



