# Modern C++(二)Range-based for loop(基于范围的for循环) - xcyl的口袋 - CSDN博客
2016年10月29日 00:03:13[xcyl](https://me.csdn.net/cai6811376)阅读数：375
上篇文章我们介绍了auto关键字，最后举了个示例说明auto在改善遍历容器方面的作用。在C++11标准中我们有了Range-based for loop，就是基于范围的for循环，这让我们遍历容器更加的容易。我们先看使用示例。
```cpp
std::map<string, string> testMap;
for (auto& item : testMap)
{
    cout << item.first << "-----" << item.second << endl;
}
```
是不是很简洁~
这里面auto会自动推导为`std::pair<string, string>`，所以auto推导出来的是type，不是迭代器。
但是，基于范围的for只是for循环遍历的语法糖，所以在遍历容器时修改容器会造成迭代器失效。
什么样的容器支持？
只需要容器支持begin和end迭代器，所以自定义容器需要实现begin和end迭代器。
支持数组
```cpp
int intArray[5] = { 1, 2, 3, 4, 5 };
for (auto& item : intArray)
{
    cout << item << endl;
}
```
支持初始化列表
```
for (auto& item : { 1, 2, 3, 4, 5 })
{
    cout << item << endl;
}
```
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.easydarwin.org](http://www.easydarwin.org)
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
