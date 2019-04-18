# Django models中多对多字段的反查询 - 知乎
# 



The Django Book中这样写
![](https://pic3.zhimg.com/v2-97af562fc3062bd4d562e96c11fb2446_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='701' height='397'></svg>)
但我使用属性名后加_set会报错 

而直接用members = group.user_group_join.all() 就可以

可能因为我的MyUser类里有两个与MyGroup相关的字段


```
class MyUser(models.Model):
    group_join = models.ManyToManyField(MyGroup,related_name="user_group_join")
    group_manage = models.ManyToManyField(MyGroup,related_name="user_group_manage")
```





