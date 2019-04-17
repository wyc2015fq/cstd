# strtol 函数用法 - Grandyang - 博客园







# [strtol 函数用法](https://www.cnblogs.com/grandyang/p/4966132.html)







strtol是一个C语言函数，作用就是将一个字符串转换为长整型long，其函数原型为：

```
long int strtol (const char* str, char** endptr, int base);
```



下面我们来看下每个参数的意义：

str是要转换的字符

enptr是指向第一个不可转换的字符位置的指针

base的基数，表示转换成为几进制的数



两点注意：
- 当 base 的值为 0 时，默认采用 10 进制转换，但如果遇到 '0x' / '0X' 前置字符则会使用 16 进制转换，遇到 '0' 前置字符则会使用 8 进制转换。
- 若endptr 不为NULL，则会将遇到的不符合条件而终止的字符指针由 endptr 传回；若 endptr 为 NULL，则表示该参数无效，或不使用该参数。



```
string v = "1.10.2";
char *c = (char*)v.c_str();

long d = strtol(c, &c, 10);
printf("%d\n", d);
printf("%s\n", c);
```



上面这段代码先定义了一个值为"1.10.2"的字符串c，然后调用strtol后，此时的d为1，字符串c变成了".10.2"。



参考资料：

[http://c.biancheng.net/cpp/html/129.html](http://c.biancheng.net/cpp/html/129.html)














