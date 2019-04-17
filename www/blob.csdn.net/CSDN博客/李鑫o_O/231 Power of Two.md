# 231. Power of Two - 李鑫o_O - CSDN博客





2016年03月19日 19:43:51[hustlx](https://me.csdn.net/HUSTLX)阅读数：193










```cpp
bool isPowerOfTwo(int n) {
    while (n>1) {
        if (n % 2) return false;
        n /= 2;
    }
    return n == 1;
}
```


以上是我自己想的版本，但是论坛里一种更巧妙的办法，因为2的次方的二进制只有一位是1，所以必有 n&(n-1)==0





```cpp
bool isPowerOfTwo(int n) {
        if(n<=0) return false;
        return !(n&(n-1));
    }
```


















































