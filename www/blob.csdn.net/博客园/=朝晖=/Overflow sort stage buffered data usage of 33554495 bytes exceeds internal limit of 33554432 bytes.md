# Overflow sort stage buffered data usage of 33554495 bytes exceeds internal limit of 33554432 bytes - =朝晖= - 博客园
# [Overflow sort stage buffered data usage of 33554495 bytes exceeds internal limit of 33554432 bytes](https://www.cnblogs.com/dhcn/p/9037013.html)
     MongoDB执行错误：
```
Overflow sort stage buffered data usage of 33554495 bytes exceeds internal limit of 33554432 bytes
```
    错误原因，排序状态数超出最大限制32M。
    两种解决方法，一个是把相关排序字段索引化，一个是扩大这个最大限制，要想从质变的角度解决这个问题，第一个方法比较靠谱一点。
```
db.collectionName.createIndex({field_name:1})
```
    如果你在应用中是对这个字段做反向排序，就把上面那个1改成-1

