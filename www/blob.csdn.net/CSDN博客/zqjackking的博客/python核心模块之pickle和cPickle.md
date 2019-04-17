# python核心模块之pickle和cPickle - zqjackking的博客 - CSDN博客





2017年05月03日 12:07:36[zqjackking](https://me.csdn.net/zqjackking)阅读数：182标签：[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/zqjackking/article/category/6897674)









```
pickle模块的功能就是执行序列化和反序列化，分别是通过dump()函数和load()函数完成的。
dump()函数接受一个文件句柄和一个数据对象作为参数，把数据对象以特定的格式保存到给定的文件中。当我们使用load()函数从文件中取出已保存的对象时，pickle知道如何恢复这些对象到它们本来的格式。
cPickle是pickle得一个更快得C语言编译版本。
[这里写链接内容](http://blog.csdn.net/yucan1001/article/details/8478755)
```

```python
>>> a1 = 'apple'  
>>> b1 = {1: 'One', 2: 'Two', 3: 'Three'}  
>>> c1 = ['fee', 'fie', 'foe', 'fum']  
>>> f1 = file('temp.pkl', 'wb')  
>>> pickle.dump(a1, f1, True)  
>>> pickle.dump(b1, f1, True)  
>>> pickle.dump(c1, f1, True)  
>>> f1.close()  
>>> f2 = file('temp.pkl', 'rb')  
>>> a2 = pickle.load(f2)  
>>> a2  
'apple'  
>>> b2 = pickle.load(f2)  
>>> b2  
{1: 'One', 2: 'Two', 3: 'Three'}  
>>> c2 = pickle.load(f2)  
>>> c2  
['fee', 'fie', 'foe', 'fum']  
>>> f2.close()
```

pickle还有另一个函数对dumps()和loads()，dumps(object) 返回一个字符串，它包含一个 pickle 格式的对象； loads(string) 返回包含在 pickle 字符串中的对象

```python
>>> import cPickle as pickle  
>>> t1 = ('this is a string', 42, [1, 2, 3], None)  
>>> t1  
('this is a string', 42, [1, 2, 3], None)  
>>> p1 = pickle.dumps(t1)  
>>> p1  
"(S'this is a string'/nI42/n(lp1/nI1/naI2/naI3/naNtp2/n."  
>>> print p1  
(S'this is a string'  
I42  
(lp1  
I1  
aI2  
aI3  
aNtp2  
.  
>>> t2 = pickle.loads(p1)  
>>> t2  
('this is a string', 42, [1, 2, 3], None)  
>>> p2 = pickle.dumps(t1, True)  
>>> p2  
'(U/x10this is a stringK*]q/x01(K/x01K/x02K/x03eNtq/x02.'  
>>> t3 = pickle.loads(p2)  
>>> t3  
('this is a string', 42, [1, 2, 3], None)
```



