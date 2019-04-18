# Python入门--2--继续学习 - 默槑 - 博客园







# [Python入门--2--继续学习](https://www.cnblogs.com/modaidai/p/6724767.html)





　　　　　　　　　　　　　　　　　　　　　　　　继续学习小甲鱼

一、python比较操作符

    == ：判断左边是否等于右边

   ！= ： 判断左边是否不能右边

二、 if while判断语句

栗子：

temp = input ("sha shu zi :")
guess = int (temp)
while guess != 8 :
    　　temp = input ("you are wrong,please do it agagin:")    
    　　guess = int (temp)
    　　if guess == 8 :
        　　　　print ("你是小甲鱼肚子里的蛔虫吗")
        　　　　print ("猜中了也没有奖励 哼")
    　　else:
        　　　　if guess > 8 :
            　　　　　　print ("大了大了")
        　　　　else:
            　　　　　　print ("小了小了")
print ("游戏结束 不玩了")



三、小甲鱼的例子 

　　1、（10<cost） and  (50>cost)  等价于 10<cost<50

　　2、用分号 在一行写多个语句

　　　　　　　　print ("456") ; print("123")

　　3、一个语句分成多行：

　　　　　一、　3>4 and \

　　　　　　　　　　1<2                ##用反斜线

　　　　　二、(3>4 and

　　　　　　　　　　  1<2 )　　　　##用括号

四、python的数据类型

1、科学计数法

 输入15e10          ##150000000000.0　　

2、布尔型

输入 TRUE ＋ＴＲＵＥ　　　　　＃输出2

输入 TRUE + FLASE　　　　　　#输出1



之间转换：

1、变为整数：int()

a = 5.99

c = int(a)              ##此时的c 为5



2、变为浮点型：float()

b = float (c)           ##此时的b为 5.0



3、变为字符创： str()

q = str(b)   　　　　##此时的q为 '5.0'



查看数据是什么类型：type()

 type(a)   type(b)  type(c)



isinstance(a,str)  ##查看a是不是str类型  返回值是TRUE

isinstance(320,int)  ##查看320是不是int类型 返回值是TRUE

isinstance(320, bool) ##查看320是不是布尔型 返回值是FLASE












