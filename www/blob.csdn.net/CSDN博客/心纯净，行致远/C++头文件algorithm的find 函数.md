# C++头文件algorithm的find 函数 - 心纯净，行致远 - CSDN博客





2018年11月24日 23:21:23[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：379








# Find()

很多语言都内置了非常方便的查找函数方便我们调用，其实C++中也有find函数：

## vector中

使用find()函数需要#include<algorithm>，例如：

```cpp
vector<string> ar1;
    ar1.push_back("aaa");
    ar1.push_back("bbb");
    ar1.push_back("ccc");
    ar1.push_back("ddd");
    if(find(ar1.begin(), ar1.end(), "bbb") != ar1.end()){
        cout<<find(ar1.begin(), ar1.end(), "bbb")-ar1.begin()<<endl;
    //看到find函数的返回结果与vector的end比较可以看出其实这是一个指针，那么如果我们想要获得索引
    //那么将返回结果与begin做差即可
    }
    else
        cout<<"NO"<<endl;
```

## string中

我是觉得find可以使用到string中真的是非常方便，例如：

```cpp
string str = "hello world";
    char ch = 'l';
    if(str.find(ch)!=string::npos){ //查找单个字符
        cout<<str.find(ch)<<endl;
    }
    else
        cout<<"NO"<<endl;
```

我们也可以看出这样返回的结果是匹配到的字符的第一个位置，如果有多个字符都可以匹配，那么只会返回第一个了，另外，关于**string:npos** ，如果我们故意没有匹配到，并输出结果，可以看到string:npos其实是在变化的，有些编译器等于4294967295，我用的DEVc++的输出等于18446744073709551615。通常使用这个值作为没有匹配到内容的结果判定。

```cpp
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

int main(){
	string s1="hello world",s2="you are right";
	for(int i=0;i<s2.length() ;i++){
			cout<<s1.find(s2[i])<<endl; 		
	}	
	return 0;
}
```

![](https://img-blog.csdnimg.cn/20181123221413450.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5zaGVuMTEy,size_16,color_FFFFFF,t_70)

一般的常用方法是：

```cpp
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;

int main(){
	string s1="hello world",s2="you are right";
	for(int i=0;i<s2.length() ;i++){
		if(s1.find(s2[i])!=string::npos )
			cout<<s1.find(s2[i])<<endl; 
		else	cout<<"NONONO"<<endl;			
	}	
	return 0;
}
```

if语句的意思是在s1字符串中查找s2中的字符元素，if(s1.find(s2[i])!=string::npos )的意思是能够查找到这样的元素。查得到则输出，反之，输出“NONONO”。

如果我们想要获取到所有的匹配字符的位置怎么办呢？其实我们find函数是有很多方式的，我们还可以通过第二个参数来控制开始查找的位置。参考：string find()

```cpp
int p = 0;
while(str.find(ch, p)!=string::npos){
            p = str.find(ch, p);
            cout<<p<<endl;
            p = p + 1;
        }
```

这样通过一个位置p不断的更新上一次的匹配位置，我们可以不断地更新开始位置，这样就能得到所有地匹配位置了。

同样地，string地find函数也可以匹配到字符串：

```cpp
string ps;
    while(cin>>ps){
        if(str.find(ps)!=string::npos){
            cout<<str.find(ps)<<endl;
        }
        else
            cout<<"NO"<<endl;
    }
```





