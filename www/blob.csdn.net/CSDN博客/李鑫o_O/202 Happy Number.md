# 202. Happy Number - 李鑫o_O - CSDN博客





2016年03月17日 14:52:07[hustlx](https://me.csdn.net/HUSTLX)阅读数：151








```cpp
bool isHappy(int n) {
    int sum = 0;
    while (1) {
        while (n > 0) {
            sum += pow(n % 10, 2);
            n /= 10;
        }
        if (sum == 1) return true;
        else if (sum < 10) return false;
        n = sum;
        sum = 0;
    } 
}
```




