# ORACLE 操作空间字段 报ORA-29877/ORA-29875错误的处理 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月15日 18:14:50[左直拳](https://me.csdn.net/leftfist)阅读数：45
个人分类：[oracle																[gis](https://blog.csdn.net/leftfist/article/category/6685585)](https://blog.csdn.net/leftfist/article/category/5734215)
oracle里，对用arcgis创建的空间字段进行赋值，
```
update SDE.PE_BASE set SHAPE=sde.ST_PointFromText('point(118.25 103.21)',0) where objectid=203;
```
报错：
```
ORA-29877: 无法执行 ODCIINDEXUPDATE 例行程序
ORA-20085: Insert Spatial Reference SRID 0 does not match SDE.PE_BASE.SHAPE registered Spatial Reference SRID 4326
ORA-06512: 在 "SDE.ST_DOMAIN_METHODS", line 2014
ORA-06512: 在 "SDE.ST_DOMAIN_METHODS", line 2386
```
从错误提示可以得知，这是坐标系不匹配问题。其实sde.ST_PointFromText是一个函数，第二个参数就是让我们指定坐标系的。我执行的那条SQL语句，给出的坐标点，采用的是常用的空间地理坐标系，编号为4326，大家一定不会陌生。因此，将SQL修改一下即可：
```
update SDE.PE_BASE set SHAPE=sde.ST_PointFromText('point(118.25 103.21)',4326) where objectid=203;
```
oracle并不天然支持空间数据库，需要引用第三方库。arcgis就提供了这样的库。看上去很神秘，但其实，sde.ST_PointFromText不就是一个函数嘛，是一个专门的函数而已。
有关坐标系的一些心得，可看拙作：
[arcgis for jsapi开发：坐标系、经纬度与平面坐标的互换](https://leftfist.blog.csdn.net/article/details/88119372)
