# Python入门之类与面向对象（一） - Machine Learning with Peppa - CSDN博客





2018年01月13日 22:50:36[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：137
所属专栏：[用Python玩转数据](https://blog.csdn.net/column/details/18811.html)









最近重拾了Python的学习，在复习语法和数据结构的同时，我也重新学习了Python中最重要的面向对象应用。

从前看Python的面向对象模块时并未完全领悟到类与对象的关系和底层逻辑。后来学了C语言，我试着用C语言的ADT的方式去理解类以及实例化的类，觉得两者之间有很多共性。

比如，Python中的四大特性：继承，多态，封装以及抽象，是在ADT中的抽象和封装基础上发展而来，其构造更加完善，定义更加普遍。此时此刻，我才理解了Python的发明者具有多神奇的创造能力。

作为强大的工具，多加练习和应用才是最好的办法，因此自己动手实现了一个小小的程序，通过类简单实现了一个有理数运算的对象操作，包含两个属性两个方法，供大家复习和参考



```python
# -*- coding: utf-8 -*-
"""
Created on Sat Jan 13 22:04:53 2018

@author: Administrator
"""

#practice for Class method and ADT
#抽象数据类型之：类
#类作用：将实例化的对象赋为有理数并求和，约分

class rational:
    @staticmethod       #使用修饰符将此函数局部化
    def _gcd(m , n):          #函数名前加下划线是约束此函数于CLASS内部
        if n == 0:
            m, n = n, m
        while m != 0:
            m, n = n % m, m         #此函数用来实现最大公约数
        return n
    
    def __init__(self, num, den = 1):       #构造函数，用于初始化
        if not isinstance(num, int) or not isinstance(den, int):
            raise TypeError
        if den == 0:
            raise ZeroDivisionError
        sign = 1
        if num < 0:                         
            num , sign = -num, -sign
        if den < 0:
            den, sign = -den, -sign
            
        g = rational._gcd(num, den)
        #call function，利用之前定义的GCD函数赋值给G
        
        self._num = sign * (num//g)
        self._den = den//g   #整除分子，分母
        
    #define a method Plus
        
    def plus(self, another):          
        _den = self._den * another._den
        _num = (self._num * another._den) + (self._den * another._num)
        
        return rational(_num, _den)
        
    def printf(self):
        print(str(self._num) + "/" + str(self._den))
        
if __name__ == '__main__':  #将文件作用域约束在此文件中
    r1 = rational(3, 1)
    r2 = r1.plus(rational(7,15))
    r2.printf()
```


输出结果：

![](https://img-blog.csdn.net/20180113225330497?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






