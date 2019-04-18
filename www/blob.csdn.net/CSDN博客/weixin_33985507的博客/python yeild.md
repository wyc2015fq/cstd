# python yeild - weixin_33985507的博客 - CSDN博客
2016年10月13日 16:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
Yield的用法和关键字return差不多,下面的函数将会返回一个生成器:
```
n [65]: def createGenerator():
   ....:     mylist = range(3)
   ....:     for i in mylist:
   ....:         yield i*i
   ....:         
In [66]: mygenerator = cre
createGenerator  credits          
In [66]: mygenerator = createGenerator()
In [67]: for i in mygenerator:
   ....:     print i
   ....:     
0
1
4
```
