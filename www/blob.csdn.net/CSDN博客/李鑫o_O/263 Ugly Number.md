# 263. Ugly Number - 李鑫o_O - CSDN博客





2016年03月17日 14:58:20[hustlx](https://me.csdn.net/HUSTLX)阅读数：221








```cpp
bool isUgly(int num) {
    while (num != 0 && num % 5 == 0) num /= 5;
    while (num != 0 && num % 3 == 0) num /= 3;
    while (num != 0 && num % 2 == 0) num /= 2;
    return num == 1;
}
```




