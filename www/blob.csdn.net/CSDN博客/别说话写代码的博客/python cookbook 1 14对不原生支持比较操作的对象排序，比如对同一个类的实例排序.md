# python cookbook 1.14对不原生支持比较操作的对象排序，比如对同一个类的实例排序 - 别说话写代码的博客 - CSDN博客





2019年01月14日 22:54:58[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：22标签：[python  cookbook																[python对类对象排序](https://so.csdn.net/so/search/s.do?q=python对类对象排序&t=blog)](https://so.csdn.net/so/search/s.do?q=python  cookbook&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)








```python
#1.14对不原生支持比较操作的对象排序，比如对同一个类的实例排序
#若想使用User_id属性对类的实例排序
class User:
    def __init__(self,user_id):
        self.user_id = user_id
    def __repr__(self):
        return 'User({})'.format(self.user_id)
    
users = [User(23),User(2),User(99)]
print(users)
usrsort=sorted(users,key=lambda u:u.user_id)   #可以通过sorted函数中的key参数指定
print(usrsort)
```

![](https://img-blog.csdnimg.cn/20190114225354441.png)

```python
from operator import attrgetter
sortusr = sorted(users,key=attrgetter('user_id'))   #可以使用这个函数.通常比lambda快一些
print(sortusr)

#by_name = sorted(users,key=attrgetter('last_name','first_name'))   #按照两个参数进行排序
print(min(users,key=attrgetter('user_id')))   #也可以用在min,max这样的函数
print(max(users,key=attrgetter('user_id')))
```

![](https://img-blog.csdnimg.cn/20190114225414391.png)






