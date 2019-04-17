# 9. Palindrome Number - 李鑫o_O - CSDN博客





2016年03月16日 19:26:10[hustlx](https://me.csdn.net/HUSTLX)阅读数：174








```cpp
bool isPalindrome(int x) {
    if (x<0 || (x != 0 && x % 10 == 0)) return false;
    int sum = 0;
    int temp = x;
    while (x > 0) {
        sum = sum * 10 + x % 10;
        x /= 10;
    }
    return sum == temp;
}
```




