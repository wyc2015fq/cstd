# Django 2.0 Release note阅读简记 - =朝晖= - 博客园
# [Django 2.0 Release note阅读简记](https://www.cnblogs.com/dhcn/p/7617069.html)
     最前面就是大家都知道的这个版本开始只支持py3.4+，而且下一个大版本就不支持3.4，再就是建议所有插件开始放弃1.11
     1、最惊艳的变化，就是URL配置正则表达式的简化，旧的：
```
url(r'^articles/(?P<year>[0-9]{4})/$', views.year_archive),
```
     简化后的：
```
path('articles/<int:year>/', views.year_archive),
```
    2、Admin对移动端的友好
    3、SQL查询对Windows表达式的支持，就是SQL里面的over从句，这个具体看SQL语言去吧。
    向前不兼容的变化里面可以说的就是Form field和Model index的两个配置定义函数的可选参数不再支持顺序传参，必须写参数名。
     再就是查询结果slice以后就不能再调用last()和reverse()

