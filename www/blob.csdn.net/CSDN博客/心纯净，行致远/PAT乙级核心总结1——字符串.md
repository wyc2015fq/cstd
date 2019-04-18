# PAT乙级核心总结1——字符串 - 心纯净，行致远 - CSDN博客





2018年11月22日 12:47:18[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：174








对于字符串的考察时PAT乙级的必考点之一，相应的字符串操作函数是重难点，梳理如下。

# 1、字符串完全反转

**Q：将字符串“123456789  asd”反转为“dsa 987654321”**

字符串的反转总结了以下两种方法。**1、利用字符串反转函数reverse（s.begin()，s.end()）；2、自定义函数：依次将字符串逆序赋值输出;3、使用栈；4、string的反向迭代器**

### 1、利用字符串反转函数reverse（s.begin()，s.end()）

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
int main()
{
    string s="123456";
    reverse(s.begin(),s.end());	//reverse()头文件是<algorithm>
    cout<<s<<endl;
    return 0;
}

/*
output:
654321
*/
```

### 2、自定义函数：依次将字符串逆序赋值输出。

```cpp
string reverse(string str){	//字符串反转 
	string rev;
	rev=str;
	for(int i=0;i<str.length();i++)
		rev[i] = str[str.length()-1-i];
	return rev;
}
```

### 3、使用栈（debug）

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main(){
    stack<char> s;
    char c;
    while(cin>>c) s.push_back(c)
    while(!s.empty()){
		cout<<" "<<s.top();
		s.pop();
    }
}



#include <iostream>
#include <stack>
using namespace std;

int main(){
    stack<char> s;
    char c;
    while(scanf("%c",&c)!=EOF)	s.push (c);
	cout<<s.top();
	s.pop();
    while(!s.empty ()){
		cout<<s.top();
		s.pop();
    }
}
```

### 4、string的反向迭代器

```cpp
#include <iostream>
#include <string>
using namespace std; 
int main(){
	string s="123456";
	s.assign(s.rbegin() ,s.rend()) ;
	cout<<s;
}

/*
output:
654321
*/


#include <iostream>
#include <string>
using namespace std; 
int main(){
	string s="123456";
	s=string(s.rbegin() ,s.rend() );
	cout<<s;
}

/*
output:
654321
*/
```

# 2、字符串段间反转，段内不反转

**Q：将字符串“how are you”反转为“you are how”**

## **实例：**

> 
1009 说反话 （20 分）

给定一句英语，要求你编写程序，将句中所有单词的顺序颠倒输出。

### 输入格式：

测试输入包含一个测试用例，在一行内给出总长度不超过 80 的字符串。字符串由若干单词和若干空格组成，其中单词是由英文字母（大小写有区分）组成的字符串，单词之间用 1 个空格分开，输入保证句子末尾没有多余的空格。

### 输出格式：

每个测试用例的输出占一行，输出倒序后的句子。

### 输入样例：

```
Hello World Here I Come
```

### 输出样例：

```
Come I Here World Hello
```


### 算法

使用栈这种数据结构极其方便。针对这种逆序输出的情况，栈天生适合。

### 代码

```cpp
#include <iostream>
#include <stack>
using namespace std;

int main(){
	stack<string> v;
	string s;
	while(cin>>s) v.push(s);
	cout<<v.top();
	v.pop();
	while(!v.empty()){
		cout<<" "<<v.top();
		v.pop();
	}
	return 0;
}
```





