# Python入门--4--分之和循环 - 默槑 - 博客园







# [Python入门--4--分之和循环](https://www.cnblogs.com/modaidai/p/6746348.html)





1、用ELIF比较省CPU：

第一种方法，使用if

score = int(input('请输入你的分数：'))

if (score <= 100) and (score >= 90):
    　　print('A')
if (score <= 89) and (score >= 80):
    　　print ("B")
if (score <= 79) and (score >= 70):
    　　print ("C")
if (score <= 69) and (score >= 60):
    　　print ("D")
if (score <= 59) or (score >=101):
    　　print ("输入错误！")



第二种方法，使用ELIF

score = int(input('请输入你的分数：'))

if (score <= 100) and (score >= 90):
    print('A')
elif (score <= 89) and (score >= 80):
    print ("B")
elif (score <= 79) and (score >= 70):
    print ("C")
elif (score <= 69) and (score >= 60):
    print ("D")
else :
    print ("输入错误！")



使用ELIF比IF更省CPU



2、assert：当这个关键词后面的条件为假的时候，程序自动崩溃并抛出assertionError的异常

　　　　　　为了防止程序员找不到后面的异常，所以在前面的判断出现问题的时候就先跟他停止

　　　　　　以后会说：怎么对这个异常进行捕获，并处理

3、一个小技巧：

　　　　　　1、怎么让x、y、z这个三个值快速互换

　　　　　　　　　　x,y,z = y,z,x

　　　　　　2、成员资格运算符：in

　　　　　　　　　　　　　　xname= "北京天安门"
　　　　　　　　　　　　　　　'门' in xname 　　　　#返回TRUE

4、三元操作符：if       使用方法



x, y, z = 6, 5, 4　　　　#这个程序可以变为：

if x < y:　　　　　　　　　min_value = x if (x < y and x < z) else (y if y < z else z)

    　　small = x
    　　if z < small:
        　　　　small = z
elif y < z:
    　　small = y
else:
    　　small = z



5、 while 

　　用法：while 条件：

　　　　　　　　循环体　　　　　　#直到条件为假 否则一直循环 像 条件为1 = 1这种就没完没了的循环

6、for

　　用法：for 目标 in 表达式：　　　　　　

　　　　　循环体

举个栗子 　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　

name = 'xiaowangba'

for i in name:　　　　　　　　　　　　　

　　print i

再举个栗子

name = ['xiaowangba','123','456','we are coming']
for each in name:
    　　print (each,len(each))

7、range():内置函数 BIM 　　生成数字序列的函数   可以与for一起用

  　　　range(5) 　　　#生成从0到4这五个数

　　　　range(1,10)　　#生成从0到10这是个数

　　　　range(1,10,2)　　#范围从0到10 间隔为2

8、break：可以终止当前循环，不再往下循环，立刻跳出循环，执行循环外接下来的语句

9、continue：终止本轮循环，并开始下一轮循环












