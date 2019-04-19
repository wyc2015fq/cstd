# go语言string、int、int64互相转换 - 三少GG - CSDN博客
2015年12月30日 11:29:55[三少GG](https://me.csdn.net/scut1135)阅读数：152420
个人分类：[golang](https://blog.csdn.net/scut1135/article/category/1449469)

```cpp
#string到int
int,err:=strconv.Atoi(string)
#string到int64
int64, err := strconv.ParseInt(string, 10, 64)
#int到string
string:=strconv.Itoa(int)
#int64到string
string:=strconv.FormatInt(int64,10)
```
