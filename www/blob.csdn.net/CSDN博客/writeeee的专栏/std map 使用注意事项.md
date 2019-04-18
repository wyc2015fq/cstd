# std::map 使用注意事项 - writeeee的专栏 - CSDN博客
2016年05月27日 15:27:35[writeeee](https://me.csdn.net/writeeee)阅读数：572
对于下面的场景，存在key就使用，否则返回null，有下面两种写法：
```
if(m.count(key)>0)
 {
     return m[key];
 }
 return null;
```
```
iter = m.find(key);
 if(iter!=m.end())
 {
     return iter->second;
 }
 return null;
```
1.这里需要注意：前一种方法很直观，但是效率差很多。因为前面的方法，需要执行两次查找。因此，推荐使用后一种方法。
2.不管用哪种都需要判断，是否存在，否则会默认增加一个指定的key值元素，对应的Second项则不可预知。
