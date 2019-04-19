# django模型(增删改查等) - _天枢 - 博客园
## [django模型(增删改查等)](https://www.cnblogs.com/yhleng/p/8716026.html)
2018-04-04 10:48 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8716026)
**Django提供的模型操作起来数据库非常方便**
**以自定义模型名字`Demo为例：`**
**获取所有数据:**all()****
```
demo.objects.all()  #返回queryset对象
#这个语句和Select * from demo还是有一定区别的
demo.objects.all()  是直接获取所有列，速度上比select要快
```
**单个精确查找数据：**get()****
```
demo.objects.get(name='qc')
#一般精确找一条数据用get，需要注意的是，如果查找的数据不存在，或者，有多个数据会报异常。
```
**过滤，精确查找:filter()**
相对来说，filter的用法就比较多了，排序，精确查找等
例1：条件
```
demo.objects.filter(name='qc')  #返回的是一个list对象
#相当于sql语句
select * from demo whert name='qc';
```
例2：多个条件组合
```
demo.objects.filter(name='qc',id=3)
#相当于sql语句
select * from demo where name='qc' and id =3;
```
例3：__双下划线
```
demo.objects.filter(id__lte=3) #注意这里两个下划线__lte
相当于sql语句：
select * from demo where id <= 3;
demo.objects.filter(name__contains='qc')
相当于sql语句:
select * from demo where name like '%qc%'
```
例4：排序
```
demo.objects.order_by("name")
相当于sql语句：
select * from demo order by name; #默认正向排序
也可以降序
demo.objects.order_by("-name") #注意这里有一个减号
相当于sql语句：
select * from demo order by name desc;
```
```
#在filter基础上排序
demo.objects.filter(name='qc').order_by('name')
相当于sql语句：
select * from demo where name='qc' order by name asc;
```
例5：切片数据
```
demo.objects.order_by('name')[0] #只取第一条数据
相当于sql语句：
select * from demo order by name limit 1；
demo.objects.order_by('name')[0:2] #只取2条数据
相当于sql语句：
select * from demo order by name offset 0 limit 2；
```
