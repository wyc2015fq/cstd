# nginx封装的数据结构 - LC900730的博客 - CSDN博客
2017年11月03日 15:23:27[lc900730](https://me.csdn.net/LC900730)阅读数：113
### ngx_string:宏函数，只能用于赋值初始化
`#define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }`
```
ngx_string与ngx_null_string是“{，}”格式的，故只能用于赋值时初始化，如：
ngx_str_t str = ngx_string("hello world");
ngx_str_t str1 = ngx_null_string;
```
如果像下面这样使用，就会有问题
```
ngx_str_t str, str1;
str = ngx_string("hello world");    // 编译出错
str1 = ngx_null_string;                // 编译出错
```
这种情况，可以调用ngx_str_set与ngx_str_null这两个函数来做:
```
ngx_str_t str, str1;
ngx_str_set(&str, "hello world");
ngx_str_null(&str1);
```
```bash
另外要注意的是，ngx_string与ngx_str_set在调用时，传进去的字符串一定是常量字符串，否则会得到意想不到的错误(因为ngx_str_set内部使用了sizeof()，如果传入的是u_char*，那么计算的是这个指针的长度，而不是字符串的长度)。如：
ngx_str_t str;
u_char *a = "hello world";
ngx_str_set(&str, a);
```
值得注意的是，由于ngx_str_set与ngx_str_null实际上是两行语句，故在if/for/while等语句中单独使用需要用花括号括起来，例如
