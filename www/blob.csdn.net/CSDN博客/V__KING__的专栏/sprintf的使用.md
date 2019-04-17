# sprintf的使用 - V__KING__的专栏 - CSDN博客





2016年11月07日 15:38:51[v__king__](https://me.csdn.net/V__KING__)阅读数：430标签：[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[c																[C++](https://blog.csdn.net/V__KING__/article/category/1680273)](https://blog.csdn.net/V__KING__/article/category/1711535)








下面的是会有问题的：



```cpp
char *temp ;
sprintf(temp,"client=> %s:%d   : %d\n",client_ip, client_port, client_st._socktid);
```






应该写成：





```cpp
char temp[100] ;
sprintf(temp,"client=> %s:%d   : %d\n",client_ip, client_port, client_st._socktid);
```












