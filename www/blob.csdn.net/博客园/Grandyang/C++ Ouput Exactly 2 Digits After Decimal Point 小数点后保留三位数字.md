# C++ Ouput Exactly 2 Digits After Decimal Point 小数点后保留三位数字 - Grandyang - 博客园







# [C++ Ouput Exactly 2 Digits After Decimal Point 小数点后保留三位数字](https://www.cnblogs.com/grandyang/p/4846001.html)







在C++编程中，有时候要求我们把数据保留小数点后几位，或是保留多少位有效数字等等，那么就要用到setiosflags和setprecision函数，记得要包含头文件#include <iomanip>，请参考下面的示例：



```
#include <iostream>
#include <iomanip>    // Need this 
using namespace std;

int main() {
    float a = 4, b = 3, c = 2;
    
    cout << a / b << endl;
    cout << b / c << endl;
    
    cout << setprecision(3) << a / b << endl;
    cout << setprecision(3) << b / c << endl;
    
    cout << setiosflags(ios::fixed) << setprecision(3) << a / b << endl;
    cout << setiosflags(ios::fixed) << setprecision(3) << b / c << endl;

    return 0;
}
```



输出为：

```
1.33333
1.5

1.33
1.5

1.333
1.500
```



如上面所示，默认情况最多保留6位有效数字，我们如果加上setprecision(3)的话，表明我们需要三位有效数字，如果我们想要小数点后三位有效数字，还需要在前面加上setiosflags(ios:：fixed).














