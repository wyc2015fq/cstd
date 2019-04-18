# C++中cin、cin.get()、cin.getline()、getline()、gets()等函数的用法 - 心纯净，行致远 - CSDN博客





2018年12月20日 10:31:34[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：53








## 学C++的时候，这几个输入函数弄的有点迷糊；这里做个小结，为了自己复习，也希望对后来者能有所帮助，如果有差错的地方还请各位多多指教（本文所有程序均通过VC 6.0运行）

1、cin 

2、cin.get() 

3、cin.getline() 

4、getline() 

5、gets() 

6、getchar()

附:cin.ignore();cin.get()//跳过一个字符,例如不想要的回车,空格等字符

***1、cin>>         ***

用法1：最基本，也是最常用的用法，输入一个数字：

#include <iostream> 

using namespace std; 

main () 

{ 

int a,b; 

cin>>a>>b; 

cout<<a+b<<endl; 

}

输入：2[回车]3[回车] 

输出：5

注意:>> 是会过滤掉不可见字符（如 空格 回车，TAB 等） 

cin>>noskipws>>input[j];//不想略过空白字符，那就使用 noskipws 流控制

用法2：接受一个字符串，遇“空格”、“TAB”、“回车”都结束

#include <iostream> 

using namespace std; 

main () 

{ 

char a[20]; 

cin>>a; 

cout<<a<<endl; 

}

输入：jkljkljkl 

输出：jkljkljkl

输入：jkljkl jkljkl       //遇空格结束 

输出：jkljkl

***2、cin.get()***

用法1： cin.get(字符变量名)可以用来接收字符

#include <iostream> 

using namespace std; 

main () 

{ 

char ch; 

ch=cin.get();               //或者cin.get(ch); 

cout<<ch<<endl; 

}

输入：jljkljkl 

输出：j

用法2：cin.get(字符数组名,接收字符数目)用来接收一行字符串,可以接收空格

#include <iostream> 

using namespace std; 

main () 

{ 

char a[20]; 

cin.get(a,20); 

cout<<a<<endl; 

}

输入：jkl jkl jkl 

输出：jkl jkl jkl

输入：abcdeabcdeabcdeabcdeabcde （输入25个字符） 

输出：abcdeabcdeabcdeabcd              （接收19个字符+1个'\0'）

用法3：cin.get(无参数)没有参数主要是用于舍弃输入流中的不需要的字符,或者舍弃回车,弥补cin.get(字符数组名,接收字符数目)的不足.

这个我还不知道怎么用，知道的前辈请赐教；

***3、cin.getline()   // 接受一个字符串，可以接收空格并输出***

#include <iostream> 

using namespace std; 

main () 

{ 

char m[20]; 

cin.getline(m,5); 

cout<<m<<endl; 

}

输入：jkljkljkl 

输出：jklj

接受5个字符到m中，其中最后一个为'\0'，所以只看到4个字符输出；

如果把5改成20： 

输入：jkljkljkl 

输出：jkljkljkl

输入：jklf fjlsjf fjsdklf 

输出：jklf fjlsjf fjsdklf

//延伸： 

//cin.getline()实际上有三个参数，cin.getline(接受字符串的看哦那间m,接受个数5,结束字符) 

//当第三个参数省略时，系统默认为'\0' 

//如果将例子中cin.getline()改为cin.getline(m,5,'a');当输入jlkjkljkl时输出jklj，输入jkaljkljkl时，输出jk

当用在多维数组中的时候，也可以用cin.getline(m[i],20)之类的用法：

#include<iostream> 

#include<string> 

using namespace std;

main () 

{ 

char m[3][20]; 

for(int i=0;i<3;i++) 

{ 

cout<<"\n请输入第"<<i+1<<"个字符串："<<endl; 

cin.getline(m[i],20); 

}

cout<<endl; 

for(int j=0;j<3;j++) 

cout<<"输出m["<<j<<"]的值:"<<m[j]<<endl;

}

请输入第1个字符串： 

kskr1

请输入第2个字符串： 

kskr2

请输入第3个字符串： 

kskr3

输出m[0]的值:kskr1 

输出m[1]的值:kskr2 

输出m[2]的值:kskr3

***4、getline()     // 接受一个字符串，可以接收空格并输出，需包含“#include<string>”***

#include<iostream> 

#include<string> 

using namespace std; 

main () 

{ 

string str; 

getline(cin,str); 

cout<<str<<endl; 

}

输入：jkljkljkl 

输出：jkljkljkl

输入：jkl jfksldfj jklsjfl 

输出：jkl jfksldfj jklsjfl

和cin.getline()类似，但是cin.getline()属于istream流，而getline()属于string流，是不一样的两个函数

转载自：[C++中cin、cin.get()、cin.getline()、getline()、gets()等函数的用法](https://www.cnblogs.com/luolizhi/p/5746775.html)



