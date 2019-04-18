# Linux 返回上一条命令的返回值 - Likes的博客 - CSDN博客
2019年01月30日 11:57:54[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：638
`echo $?`
用法示例
我们常常写一个返回值为int的main函数，期望查看返回值为多少：
```cpp
int main()
{
    return -1;
}
```
运行程序后执行命令 echo $? 显示-1
