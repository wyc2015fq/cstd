# Python读取多个txt文件并进行保存 - sinat_31425585的博客 - CSDN博客
2016年08月30日 18:16:00[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：9277
       记忆力不好的孩纸得勤做笔记！
       最近开始接触python，现在分享下写的第二个代码，好了，多的不说，直接上代码！操作系统为windons.
```python
#coding:utf-8
import matplotlib.pyplot as plt
import numpy as np
#距离
temp = 3.0
#定义字典相当于C++中宏
for i in range(1,16):
	path = { "path1":"data\\data" + str(i) + "\\result.txt",
			 "path2":"data\\data" + str(i) + "\\record.txt",
			 "path3":"result\\max.txt" }
	#读第一个文件
	x = []
	y = []
	f = open(path["path1"])
	line = f.readline()
	while line:
		#print line,
		a,b = line.split()
		x.append(b)
		y.append(a)
		
		line = f.readline()
		#print line	
	f.close()
	#string TO double
	x = [ int( x ) for x in x if x ]  
	y = [ float( y ) for y in y if y ] 
	#find the maxDefinition
	maxDef = np.max(y)
	Index = np.where(y == maxDef)
	id = x[Index[0]]
	print maxDef
	#print Index
	print id
	#读另外两个文件
	u = []
	v = []
	w = []
	f1 = open(path["path2"])
	line1 = f1.readline()
	while line1:
		#print line,
		c,d,e = line1.split()
		u.append(c)
		v.append(d)
		w.append(e)
		
		line1 = f1.readline()
		#print line	
	f1.close()
	#string TO double
	u = [ int( u ) for u in u if u ] 
	v = [ int( v ) for v in v if v ] 
	w = [ int( w ) for w in w if w ]  
	#Index1 = np.where(w == id)
	print u[id]
	print v[id]
	
	
	#Write data
	f2 = open(path["path3"], 'a')
	f2.write(str(u[id]) + ' ' + str(v[id]) +  ' ' + str(temp) + '\n')
	f2.close()
	temp += 0.5
```
```python
打完收工！！！简单的实现了对多个文本文件的读写操作~
```
