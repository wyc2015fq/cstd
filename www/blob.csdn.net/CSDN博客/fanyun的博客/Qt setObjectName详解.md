# Qt setObjectName详解 - fanyun的博客 - CSDN博客
2017年12月26日 22:26:44[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：4507标签：[qt](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[Qt](https://blog.csdn.net/fanyun_01/article/category/6429937)
使用findChild ()按名称(和类型)查找对象。您可以使用findchilds ()查找一组对象。
```cpp
qDebug("MyClass::setPrecision(): (%s) invalid precision %f",
       qPrintable(objectName()), newPrecision);
```
setObjectName(const QString & name)
销毁对象，删除其所有子对象。
