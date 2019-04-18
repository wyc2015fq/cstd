# std::map find 陷阱 - writeeee的专栏 - CSDN博客
2016年05月17日 11:46:07[writeeee](https://me.csdn.net/writeeee)阅读数：3144
正确做法
```cpp
std::map<int,int*>::iterator iter = mapTemp.find(i);
if( mapTemp.end() != iter )
    return iter->second;
return NULL;
```
1.注意判断空,如果不判断是否为mapTemp.end(),如果用mapTemp[x].second方式获取，系统会自己偷偷插入一个查询的键。    因此直接 return mapTemp[x].second 有很大风险。
2.尽量用iterator 而不是[].因为[] 运算符内部有一次查询，最好就是调用一次find，返回iterator，根据iterator获取值。
