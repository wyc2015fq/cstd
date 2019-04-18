# goahead登录提示框中服务器信息的修改 - ljx0305的专栏 - CSDN博客
2013年04月18日 10:37:08[ljx0305](https://me.csdn.net/ljx0305)阅读数：3379
一般情况下一个web服务器都会有个登录验证的过程，在验证框里面包含了服务器的信息
如下图的goahead服务器，360IE提示
![](https://img-my.csdn.net/uploads/201304/18/1366252948_2025.png)
谷歌提示
![](https://img-my.csdn.net/uploads/201304/18/1366252965_4723.jpg)
有时候你不想让这个goahead出现在这，就应该修改源码了
在goahead源码中有webs.c是定义了realm name
```cpp
```
70 static char_t   websRealm[64] = T("nihao");  /* Realm name */
这样修改后的提示框就是你的自定义的字符串了！
