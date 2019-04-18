# 【Python】‘dict’ object has no attribute 'has_key' - YZXnuaa的博客 - CSDN博客
2018年04月16日 16:36:24[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：705
‘dict’ object has no attribute 'has_key'
解决办法：
```
if adict.has_key(key1):  
改为
if key1 in adict:
```
