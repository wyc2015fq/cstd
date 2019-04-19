# linux重定向：2>&1 - Koma Hub - CSDN博客
2019年01月13日 21:41:52[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：94
**目录**
[基本符号含义](#%E5%9F%BA%E6%9C%AC%E7%AC%A6%E5%8F%B7%E5%90%AB%E4%B9%89)
[/dev/null](#%2Fdev%2Fnull)
[重定向到普通文件](#%E9%87%8D%E5%AE%9A%E5%90%91%E5%88%B0%E6%99%AE%E9%80%9A%E6%96%87%E4%BB%B6)
[多个重定向复用：1>f1 2>f2](#%E5%A4%9A%E4%B8%AA%E9%87%8D%E5%AE%9A%E5%90%91%E5%A4%8D%E7%94%A8%EF%BC%9A1%3Ef1%202%3Ef2)
[多个重定向到一个文件:1>f1 2>1](#%E5%A4%9A%E4%B8%AA%E9%87%8D%E5%AE%9A%E5%90%91%E5%88%B0%E4%B8%80%E4%B8%AA%E6%96%87%E4%BB%B6%3A1%3Ef1%202%3E1)
[多个重定向到一个文件](#%E5%A4%9A%E4%B8%AA%E9%87%8D%E5%AE%9A%E5%90%91%E5%88%B0%E4%B8%80%E4%B8%AA%E6%96%87%E4%BB%B6)
[1>f1 2>&1](#1%3Ef1%202%3E%261)
[2>&1 1>f1](#2%3E%261%201%3Ef1)
[参考：](#%E5%8F%82%E8%80%83%EF%BC%9A)
# 基本符号含义
- ">"：定向去处
- "/dev/null"：空设备文件
- "0"：stdin
- "1"：stdout
- "2"：stderr
# /dev/null
重定向到空文件中，也就是说啥也不干了，重定向到此的stdin、stdout和stderr都会罢工。
```bash
$ ls
$ touch 1 2 3
$ ls
1  2  3
$ ls >/dev/null
$ ls 1>/dev/null
$ ls 2>/dev/null
1  2  3
```
# 重定向到普通文件
```bash
$ ls
$ touch f1 f2 f3
$ ls
f1  f2  f3
$ cat f1
$ ls >f1
$ cat f1
f1
f2
f3
$ ls 1>f2
$ cat f2
f1
f2
f3
$ ls 2>f3
f1  f2  f3
$ cat f3
```
# 多个重定向复用：1>f1 2>f2
```cpp
#include <stdio.h>
int main()
{
        fprintf(stdout, "hello1\n");
        fprintf(stderr, "hello2\n");
        return 0;
}
```
编译一下：然后运行（我的环境是cygwin）：
```bash
$ gcc main.c -o main
$ ./main.exe
hello1
hello2
$ ./main.exe 1>f1 2>f2
$ cat f1
hello1
$ cat f2
hello2
```
# 多个重定向到一个文件:1>f1 2>1
```bash
$ ./main.exe 1>f1 2>f1
$ cat f1
hello1
```
这个因为文件被打开了两次，然后之写入了第一次写入的内容，试着用下面的方法。
# 多个重定向到一个文件
## 1>f1 2>&1
```bash
$ ./main.exe 1>f1 2>&1
$ cat f1
hello2
hello1
```
## 2>&1 1>f1
```bash
$ ./main.exe 2>&1 1>f1
hello2
$ cat f1
hello1
```
就是这样。。。。
# 参考：
[https://blog.csdn.net/GGxiaobai/article/details/53507530](https://blog.csdn.net/GGxiaobai/article/details/53507530)
[Linux里的2>&1究竟是什么，这篇文章告诉你](https://mp.weixin.qq.com/s?__biz=MzA4Nzg5Nzc5OA==&mid=2651675544&idx=1&sn=c62ca1948b6decf127752a2b88b2cf6d&chksm=8bcb9831bcbc112788ca2666bed7d310222f616a876e5d562afeb8c33b6483f6e3ed2d5c3194&mpshare=1&scene=1&srcid=0112qMoaozdiDNg44f926yr0#rd)
