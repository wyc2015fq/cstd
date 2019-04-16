# Nginx源码分析 - 基础数据结构篇 - 字符串结构 ngx_string.c - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2016年02月17日 18:50:12[initphp](https://me.csdn.net/initphp)阅读数：2136
所属专栏：[Nginx源码阅读](https://blog.csdn.net/column/details/nginx-read.html)









Nginx的ngx_str_t字符串结构非常简单和好用。

nxg_string.c文件中包含了很多常用的字符串操作方法，具体我们就不讲解了，主要简单来看下Nginx的字符串数据结构


### 数据结构定义

#### ngx_str_t 字符串结构



```cpp
/**
 * 字符串结构
 */
typedef struct {
    size_t      len; //字符串长度
    u_char     *data; //具体的指针地址
} ngx_str_t;
```



#### ngx_keyval_t 字符串K V结构



```cpp
/**
 * 字符串的K V结构
 */
typedef struct {
    ngx_str_t   key;
    ngx_str_t   value;
} ngx_keyval_t;
```


### 数据结构图

![](https://img-blog.csdn.net/20160217185635792?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 具体函数实现

#### 初始化一个字符串 ngx_string



```cpp
//初始化一个字符串
#define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }
```




#### 设置字符串 



```cpp
//将一个字符串设置为NULL
#define ngx_null_string     { 0, NULL }
//设置一个字符串
#define ngx_str_set(str, text)                                               \
    (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = NULL
```









