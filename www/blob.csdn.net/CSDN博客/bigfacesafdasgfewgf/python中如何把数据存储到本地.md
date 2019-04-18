# python中如何把数据存储到本地 - bigfacesafdasgfewgf - CSDN博客





2015年04月09日 14:48:58[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：5735标签：[python																[数据存储																[pickle模块](https://so.csdn.net/so/search/s.do?q=pickle模块&t=blog)
个人分类：[Python](https://blog.csdn.net/puqutogether/article/category/2595125)





有很多时候，我们会在python的运行过程中得到一些重要的变量，比如一个数据量很庞大的dict。而且，后面的某些程序也会用到这个dict，那么我们就最好把它存储到本地来，然后下次调用的时候，先读取本地的文件，导入到字典类型中，调用即可。这样就免去了重新学习这个字典的过程。那么在python中如何把数据存储到本地呢？

我们用到的是python中的pickle模块。 

如下：

```python
import pickle

data1 = {'a': [1, 2.0, 3, 4+6j],
         'b': ('string', u'Unicode string'),
         'c': None}

selfref_list = [1, 2, 3]
selfref_list.append(selfref_list)

output = open('data.pkl', 'wb')

# Pickle dictionary using protocol 0.
pickle.dump(data1, output)

# Pickle the list using the highest protocol available.
pickle.dump(selfref_list, output, -1)

output.close()
```

总结：使用pickle.dump把程序中的变量存储到本地文件，然后用pickle.load从本地文件中导入数据到程序中的变量中，实现调用。

ps：pickle居然是泡菜的意思，dump是倾倒的意思，这样子记比较容易一些。

其实最主要的还是要记住pickle模块的用法啦~

参考链接： 
[http://www.cnblogs.com/pzxbc/archive/2012/03/18/2404715.html](http://www.cnblogs.com/pzxbc/archive/2012/03/18/2404715.html)](https://so.csdn.net/so/search/s.do?q=数据存储&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




