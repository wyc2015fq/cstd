# C++ unordered_map remove 实现哈希表移除 - Grandyang - 博客园







# [C++ unordered_map remove 实现哈希表移除](https://www.cnblogs.com/grandyang/p/4871307.html)







使用C++的unordered_map类型时，我们经常要根据关键字查找，并移除一组映射，在Java中直接用remove即可，而STL中居然没有实现remove这个函数，还要自己写循环来查找要删除项，然后用erase来清除，我也是醉了，参见下面代码：



```
vector<A*> v1;
// remove A *a1
for (vector<A*>::iterator it = v1.begin(); it != v1.end(); ++it) {
    if (*it == a1) v1.erase(it);
}

unordered_map<int, B*> m;
// remove B *b1
for (unordered_map<int, B*>::iterator it = m.begin(); it != m.end(); ++it) {
    if (it->second == b1) m.erase(it);
}
```



再加上之前那篇博客[C++ Split string into vector<string> by space](http://www.cnblogs.com/grandyang/p/4858559.html)里面提到的C++的STL也没有实现split函数，如此这般，我感觉改攻Java很有必要啊~~












