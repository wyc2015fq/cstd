# Postgresql 启动could not create listen socket for "localhost"错误的解决 - =朝晖= - 博客园
# [Postgresql 启动could not create listen socket for "localhost"错误的解决](https://www.cnblogs.com/dhcn/p/7115378.html)
    新装的postgresql在第一次启动时可能会遇到错误，日志中的记录是：
```
could not create listen socket for "localhost"
```
  到/etc/hosts文件中添加一行：
```
0.0.0.0         localhost
```
   然后尝试再次启动postgresql。

