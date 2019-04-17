# Learning with python: Chapter3~4 Functions,Conditionals and Recursion - xiahouzuoxin - CSDN博客





2013年08月13日 21:09:24[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1331
个人分类：[ComputerLanuage](https://blog.csdn.net/xiahouzuoxin/article/category/1172320)












**1. Call Functions**
**>>> betty=type("32")   # call function @type>>> print betty**

<type 'str'>
**>>> id(betty)           # call function @id**

56091456
**>>> betty=3>>> id(betty)**

155577720
**>>> int("32")        # call function @int**

32
**>>> int(3.99)**

3
**>>> int(-2.3)**

-2
**>>> float(32)**

32.0
**>>> float("3.1415926")**

3.1415926000000001
**>>> str(3.1415926)       # call function @str**

'3.1415926'
**>>> minute=58>>> float(minute)/60**

0.96666666666666667
**>>> import math  # use math functions, should import math>>> decibel=math.log10(17.0)>>> decibel**

1.2304489213782739
**>>> angle=1.5>>> height=math.sin(angle)>>> height**

0.99749498660405445
**>>> degrees=45>>> angle=degrees*2*math.pi/360.0>>> math.sin(angle)**

0.70710678118654746
**>>> math.sqrt(2)/2.0**

0.70710678118654757
**>>> x=math.exp(math.log(10.0))>>> x**

10.000000000000002




**2.Define Functions**

The syntax for a function deﬁnition is 


```python
def NAME( LIST OF PARAMETERS ):
    STATEMENTS
```




**Examples：**



```python
# define functions
print "===========def func==========="
def newLine():  # def func
	print

print "First Line."
newLine();
print "Second Line"

newLine();
def odd_even(x):
	if x%2==0:
		print x, "is even"
	else:
		print x, "is odd"

odd_even(10);
odd_even(11);


def comp(x,y):
	if x < y:  
	  print x, "is less than ",y   # incdent 2 space is OK, not must TAB
	elif x > y:
	  print x, "is greater than ",y
	else:
	  print x, "and", y, "are equal"

comp(1,2);
comp(3,1);
comp(1,1);

x = -100;
if 0 < x < 200:
  print "x is between 0~200"
else:
  print "x is not in scale"

# recursion
def countdown(n):
	if n == 0:
	  print "Blastoff!"
	else:
	  print n
	  countdown(n-1)

countdown(5);

# keyboard input
print "===========input==========="
# print "Please input:"
inputthing = raw_input("What is your name?");
print inputthing
speed=input("What is the train speed?");  # integer
print speed
```



**Results：**



```
===========def func===========
First Line.

Second Line

10 is even
11 is odd
1 is less than  2
3 is greater than  1
1 and 1 are equal
x is not in scale
5
4
3
2
1
Blastoff!
===========input===========
What is your name?xiahouzuoxin
xiahouzuoxin
What is the train speed?100
100
```





