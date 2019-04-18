# 用python 4行搞定基数排序 - wishchinYang的专栏 - CSDN博客
2014年08月20日 20:12:36[wishchin](https://me.csdn.net/wishchin)阅读数：774
```python
from random import randint  
              
def main():                 
    A = [randint(1, 99999999) for _ in xrange(9999)]  
    for k in xrange(8):     
        S = [ [] for _ in xrange(10)]  
        for j in A:         
            S[j / (10 ** k) % 10].append(j)  
        A = [a for b in S for a in b]  
    for i in A:             
        print i
```
