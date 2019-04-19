# python partial function - Arthur的随笔 - CSDN博客
2011年07月17日 16:47:46[largetalk](https://me.csdn.net/largetalk)阅读数：2684
刚看到haskell的higher order function 偏函数那一节， 想到python也有这东西， 知道但没看过，就看看
大致的实现是这样的：
```python
def partial(func, *args, **keywords):
    def newfunc(*fargs, **fkeywords):
        newkeywords = keywords.copy()
        newkeywords.update(fkeywords)
        return func(*(args + fargs), **newkeywords)
    newfunc.func = func
    newfunc.args = args
    newfunc.keywords = keywords
    return newfunc
```
要使用也很简单的
```python
>>> from functools import partial
>>> basetwo = partial(int, base=2)
>>> basetwo.__doc__ = 'Convert base 2 string to an int.'
>>> basetwo('10010')
18
```
haskell的则是更好用了，不需要库
```
multThree :: (Num a) => a -> a -> a -> a  
multThree x y z = x * y * z  
ghci> let multTwoWithNine = multThree 9  
ghci> multTwoWithNine 2 3  
54  
ghci> let multWithEighteen = multTwoWithNine 2  
ghci> multWithEighteen 10  
180
```
