# "is" vs. "==" - weixin_33985507的博客 - CSDN博客
2018年10月16日 07:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
```
# "is" vs "=="
>>> a = [1, 2, 3]
>>> b = a
>>> a is b
True
>>> a == b
True
>>> c = list(a)
>>> a == c
True
>>> a is c
False
# • "is" expressions evaluate to True if two 
#   variables point to the same object
# • "==" evaluates to True if the objects 
#   referred to by the variables are equal
```
