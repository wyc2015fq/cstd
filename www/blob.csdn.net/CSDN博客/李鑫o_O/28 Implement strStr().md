# 28. Implement strStr() - 李鑫o_O - CSDN博客





2016年03月16日 10:57:27[hustlx](https://me.csdn.net/HUSTLX)阅读数：186








```cpp
int strStr(string haystack, string needle) {
    int sz = haystack.size() - needle.size();
    for (int i = 0; i <= sz;i++){
        int j = 0;
        while (j < needle.size() && (haystack[i + j] == needle[j])) {
            j++;
        }
        if (j == needle.size()) return i;
    }
    return -1;
}
```




