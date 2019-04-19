# python challenge第4题 - Arthur的随笔 - CSDN博客
2012年02月08日 00:26:09[largetalk](https://me.csdn.net/largetalk)阅读数：948标签：[python																[import](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/largetalk/article/category/715661)
```python
import urllib2, re
charnum_dic = {'one':1, 'two':2}
nothing=12345
#nothing=16044
#nothing=82682
#nothing=87890
for x in xrange(400):
    f = urllib2.urlopen('http://www.pythonchallenge.com/pc/def/linkedlist.php?nothing=%s'%nothing)
    data = f.read()
    try:
        nothing = re.findall('next\snothing.*is\s(\d+)', data)[0]
        print nothing
    except:
        data = data.lower()
        if data.find('divide') != -1:
            lst = data.split()
            nothing /= charnum_dic[lst[lst.index('by')+1]]
            print nothing
            continue
        print data
        break
```
