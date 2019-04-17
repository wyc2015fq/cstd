# Learning with python: Chapter5~8 Fruitful functions, Iterations, Strings, Lists - xiahouzuoxin - CSDN博客





2013年08月13日 21:25:14[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1307










**Examples:**



```python
#  Fruitful functions
print "===========Fruitful functions============"
import math
def area(radius):
	return math.pi*(radius**2)
mianji=area(2);
print mianji

def fibonacci(n):
	if not isinstance(n,int):  # check parameters
	  print "Factorial is only for integer."
	  return -1;
	elif n < 0:
	  print "Factorial is only for n>=0."
	  return -1;
	elif n==0 or n==1:
	  return 1
	else:
	  return fibonacci(n-1)+fibonacci(n-2)

fib = fibonacci(10);
print fib
fib = fibonacci(1.5);
print fib
fib = fibonacci(-10);
print fib


# test while
print "=========test while========="
def countdown(n):
	while n > 0:
	  print n
	  n = n - 1
	print "Blastoff!"
countdown(4)

x=1.0
while x < 10.0:
  print x, '\t', math.log(x)
  x = x + 1.0

def printMultiples(n):
  i=1
  while	i<=6:
    print n*i,'\t',
    i = i + 1
  print

i=1
while i<=6:
  printMultiples(i)
  i = i + 1

# Strings
print "=======Strings======"
fruit="banana"
# i=0
# print isinstance(len(fruit),int);
# while i < len(fruit):
#	print fruit[i]
#	i = i + 1

for char in fruit:
	print char

prefixes = "JKLMNOPQ"
suffix = "ack"
for letter in prefixes:
	print letter + suffix

s = "Peter, Paul, and Mary"
print s[0:5]
print s[7:11]
print s[17:21]

import string
index = string.find(fruit, "a")
print index
index = string.find(fruit, "na")
print index

print string.lowercase
print string.uppercase
print string.digits

# Lists
num = range(0,1,10);
str = ["xiahou", "zuoxin"];
print num, str

print "'xiahou' int str:", 'xiahou' in str  # membership
print "'xiah' int str:", 'xiah' not in str  # membership
```





**Results:**

```
===========Fruitful functions============
12.5663706144
89
Factorial is only for integer.
-1
Factorial is only for n>=0.
-1
=========test while=========
4
3
2
1
Blastoff!
1.0 	0.0
2.0 	0.69314718056
3.0 	1.09861228867
4.0 	1.38629436112
5.0 	1.60943791243
6.0 	1.79175946923
7.0 	1.94591014906
8.0 	2.07944154168
9.0 	2.19722457734
1 	2 	3 	4 	5 	6 	
2 	4 	6 	8 	10 	12 	
3 	6 	9 	12 	15 	18 	
4 	8 	12 	16 	20 	24 	
5 	10 	15 	20 	25 	30 	
6 	12 	18 	24 	30 	36 	
=======Strings======
b
a
n
a
n
a
Jack
Kack
Lack
Mack
Nack
Oack
Pack
Qack
Peter
Paul
Mary
1
2
abcdefghijklmnopqrstuvwxyz
ABCDEFGHIJKLMNOPQRSTUVWXYZ
0123456789
[0] ['xiahou', 'zuoxin']
'xiahou' int str: True
'xiah' int str: True
```








