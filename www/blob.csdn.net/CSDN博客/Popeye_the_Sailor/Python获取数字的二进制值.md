# Python获取数字的二进制值 - Popeye_the_Sailor - CSDN博客
2018年06月18日 20:36:15[_Sailor_](https://me.csdn.net/lz0499)阅读数：1366
# [Python获取数字的二进制值](https://www.cnblogs.com/cocowool/p/8037773.html)
### 目标
想要获取一个整形数字的二进制表示
### bin 内置函数
看一下官方的解释
Convert an integer number to a binary string prefixed with “0b”. The result is a valid Python expression. If x is not a Python int object, it has to define an **index**() method that returns an integer. Some examples。
```python
```python
>>> bin(3)
'0b11'
>>> bin(-10)
'-0b1010'
```
```
If prefix “0b” is desired or not, you can use either of the following ways.
```python
```python
>>> format(14, '#b'), format(14, 'b')
('0b1110', '1110')
>>> f'{14:#b}',f'{14:b}'
('0b1110', '1110')
```
```
可以看到bin函数返回二进制数字表示的形式是采用了负号，而不是补码的形式。那么如何获得补码形式的二进制表示呢，很简单只需要对数值进行与操作就可以。
```python
```python
>>> bin(-27 & 0b1111111111111111)
'0b1111111111100101'
```
```
这个例子手工指定了位数，也可以用下面带参数的形式
```python
```python
def bindigits(n, bits):
    s = bin(n & int("1"*bits, 2))[2:]
    return ("{0:0>%s}" % (bits)).format(s)
>>> print bindigits(-31337, 24)
111111111000010110010111
```
```
参考资料：
1、[Python bin](https://docs.python.org/3/library/functions.html#bin)
2、[Two's Complement Binary in Python?](https://stackoverflow.com/questions/12946116/twos-complement-binary-in-python)
3、[integers](http://docs.python.org/reference/lexical_analysis.html#integers)
转载于：[Python获取数字的二进制值](https://www.cnblogs.com/cocowool/p/8037773.html)
十进制到二进制：
def dec2bin(num):
    l =[]if num <0:return'-'+ dec2bin(abs(num))whileTrue:
        num, remainder = divmod(num,2)
        l.append(str(remainder))if num ==0:return''.join(l[::-1])
十进制到八进制：
def dec2oct(num):
    l =[]if num <0:return'-'+ dec2oct(abs(num))whileTrue:
        num, remainder = divmod(num,8)
        l.append(str(remainder))if num ==0:return''.join(l[::-1])
十进制到十六进制：
base=[str(x)for x in range(10)]+[ chr(x)for x in range(ord('A'),ord('A')+6)]def dec2hex(num):
    l =[]if num <0:return'-'+ dec2hex(abs(num))whileTrue:
        num,rem = divmod(num,16)
        l.append(base[rem])if num ==0:return''.join(l[::-1])
