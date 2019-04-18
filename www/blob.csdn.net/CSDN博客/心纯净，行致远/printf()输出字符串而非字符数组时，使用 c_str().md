# printf()输出字符串而非字符数组时，使用.c_str() - 心纯净，行致远 - CSDN博客





2018年12月02日 16:40:38[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：966








**printf()输出字符串而非字符数组时，使用.c_str()。c_str()函数是包含在<string>中的，使用的时候最好也将其头文件带上。**

**1、使用printf()输出字符串数组，显然没有问题，只需要使用字符串首地址即可。如：**

```cpp
#include <iostream>
using namespace std;
int main(){
	char s[20]={"hello"};
	printf("%s",s);
}
```

**2、如果使用string，再直接使用printf()就会出现问题。这时候需要使用.c_str()函数。**

```cpp
#include <iostream>
using namespace std;
int main(){
	string s="hello";
	printf("%s",s.c_str() );
}
```

这里，字符串也可以加括号，下面也是对的。不过字符串不是数组形式时，不加括号也是可以的。下面的同样没问题。

```cpp
#include <iostream>
using namespace std;
int main(){
	string s={"hello"};
	printf("%s",s.c_str() );
}
```





