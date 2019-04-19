# Python3  if...else - Simple 专栏 - CSDN博客
2018年10月05日 19:52:46[Simple_Zz](https://me.csdn.net/love284969214)阅读数：330
决策结构计算多个表达式并产生 true 或 flase 作为结果并退出。您需要确定采取的行动，如果结果是 true 要执行哪些语句，或否则返回 false。
Python编程语言假定任何非零和非空值的结果值为 true，如果它是零或为null，则假定为 false 值。
Python编程语言提供了以下几种类型的决策声明。
- **if 语句**： if 语句包含一个布尔表达式后跟一个或多个语句。
- **if...else 语句**：if 语句可以跟一个可选的 else 语句，当布尔表达式为 false，else语句就会被执行。
- **嵌套 if 语句**：可以使用一个 if 或 else if 在另一个 if 或 else if语句中
### **if 语句 **语法
```python
if expression:
   statement(s)
```
如果布尔表达式的计算结果为 true，则语句块中的 if 语句被执行。在Python中，在语句块后的冒号 : 均匀缩进。如果布尔表达式为 false，那么块结束后的第一组代码块将被执行。
例如：
```python
#!/usr/bin/python3
var1 = 100
if var1:
   print ("1 - Got a true expression value")
   print (var1)
var2 = 0
if var2:
   print ("2 - Got a true expression value")
   print (var2)
print ("Good bye!")
```
当执行上面的代码，它产生以下结果：
```python
1 - Got a true expression value
100
Good bye!
```
### if...else 语句 语法
```python
if expression:
   statement(s)
else:
   statement(s)
```
例如：
```python
#!/usr/bin/python3
amount=int(input("Enter amount: "))
if amount<1000:
    discount=amount*0.05
    print ("Discount",discount)
else:
    discount=amount*0.10
    print ("Discount",discount)
    
print ("Net payable:",amount-discount)
```
 在上面的例子中，discount 计算是一个输入的数量。如果用量少于1000折扣率是5%，而高于10000折扣率是10%。 当执行上面的代码，它产生以下结果：
```python
Enter amount: 600
Discount 30.0
Net payable: 570.0
Enter amount: 1200
Discount 120.0
Net payable: 1080.0
```
### elif 语句
elif语句让你检查多个表达式是否 true，并且当条件之一的值为 true 则立即执行该代码块。
类似于else，elif语句是可选的。然而，不同于else，它至多有一个语句， if 下面可以有任意数的 elif 语句。
### 语法
```python
if expression1:
   statement(s)
elif expression2:
   statement(s)
elif expression3:
   statement(s)
else:
   statement(s)
```
Python核心编程不提供在其他语言中的switch或case语句，但我们可以用if..elif...语句来模拟 switch case 
例如：
```python
#!/usr/bin/python3
amount=int(input("Enter amount: "))
if amount<1000:
    discount=amount*0.05
    print ("Discount",discount)
elif amount<5000:
    discount=amount*0.10
    print ("Discount",discount)
else:
    discount=amount*0.15
    print ("Discount",discount)
    
print ("Net payable:",amount-discount)
```
当执行上面的代码，它产生以下结果：
```python
Enter amount: 600
Discount 30.0
Net payable: 570.0
Enter amount: 3000
Discount 300.0
Net payable: 2700.0
Enter amount: 6000
Discount 900.0
Net payable: 5100.0
```
### **嵌套 if 语句 **语法
嵌套的语法：if...elif...else 的结构可能是：
```python
if expression1:
   statement(s)
   if expression2:
      statement(s)
   elif expression3:
      statement(s)
   else
      statement(s)
elif expression4:
   statement(s)
else:
   statement(s)
```
例如：
```python
# !/usr/bin/python3
num=int(input("enter number"))
if num%2==0:
    if num%3==0:
        print ("Divisible by 3 and 2")
    else:
        print ("divisible by 2 not divisible by 3")
else:
    if num%3==0:
        print ("divisible by 3 not divisible by 2")
    else:
        print  ("not Divisible by 2 not divisible by 3")
```
当执行上面的代码，它产生以下结果：
```python
enter number8
divisible by 2 not divisible by 3
enter number15
divisible by 3 not divisible by 2
enter number12
Divisible by 3 and 2
enter number5
not Divisible by 2 not divisible by 3
```
