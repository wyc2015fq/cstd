# c++的坑--字符串内存拷贝注意事项 - writeeee的专栏 - CSDN博客
2016年01月18日 23:42:24[writeeee](https://me.csdn.net/writeeee)阅读数：209
个人分类：[c++](https://blog.csdn.net/writeeee/article/category/5634855)
写的程序一运行立马崩溃，通过dmp文件可以看到崩溃语句为
```cpp
memcpy(desStr,sourStr,MAX_PATH);
```
开始没有注意有什么问题，不就是一条简单的复制嘛，于是猜想可能由于其他地方越界了吧。  但就是这条语句出现错误。
sourStr有多长就复制多长，不要复制多余的，因为字符串以为的内存具有不确定性，可能导致你的字符串不正常结束 char *没有加\0就导致出错。
