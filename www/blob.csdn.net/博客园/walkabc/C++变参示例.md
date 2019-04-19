# C++变参示例 - walkabc - 博客园
# [C++变参示例](https://www.cnblogs.com/hitfire/articles/3193061.html)
```
#include <iostream>
using namespace std;
#include <stdio.h>
#include <stdarg.h>
int printMax(int value, ...)
{
    va_list args;
    va_start(args, value);
    int val = value;
    while (val)
    {
        cout<<val<<endl;
        val = va_arg(args, int);
    }
    return 0;
}
int main()
{
    printMax(1, 2, 3, 0);
    return 0;
}
```

