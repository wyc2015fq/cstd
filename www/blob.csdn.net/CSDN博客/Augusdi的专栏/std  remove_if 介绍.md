
# std::remove_if 介绍 - Augusdi的专栏 - CSDN博客


2017年03月16日 13:57:25[Augusdi](https://me.csdn.net/Augusdi)阅读数：1396个人分类：[STL标准模板库																](https://blog.csdn.net/Augusdi/article/category/719794)


remove_if  可以这样理解
**前两个参数：**
给他一个迭代的起始位置和这个起始位置所对应的停止位置。  例如下方函数中的  str.begin(),  str.end()
**最后一个参数：**
传入一个回调函数，如果 回调函数函数返回真，则将当前所指向的参数，移到尾部（不稳定的数据移动）例如 下方的 Lambda 表达式  如果  n == find_str 这条命题为真则执行操作。
**返回值：**
被移动区域的首个元素 iterator

这个函数不负责删除工作。所以他一般与 erase 成对出现
```python
// list::front
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
int main ()
{
  std::vector<std::string> str = { "Name", "1", "2", "3 ", "4", "5", "6", "7", "Name"};  
  for(auto& name : str) {
      std::cout << name << std::endl;
  }
  auto find_str = "Name";  
  std::cout << "============================" << std::endl;
#if 0  
  auto sd = std::remove_if(str.begin(), str.end(), [find_str](std::string n) { return n == find_str; });  
  str.erase(sd, str.end());  
#else  
  str.erase(std::remove_if(str.begin(), str.end(),   
      [find_str](std::string n) { return n == find_str; }),  
      str.end());  
#endif
  std::for_each(str.begin(), str.end(), [](std::string name) {std::cout << name << std::endl;});

  return 0;
}
```


