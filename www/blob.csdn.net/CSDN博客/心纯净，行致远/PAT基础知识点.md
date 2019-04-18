# PAT基础知识点 - 心纯净，行致远 - CSDN博客





2018年12月20日 10:42:07[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：98








**D**ev C++支持C++11语法新特性：



Tool-> Compiler Option-> Add: -std=c++11

历年真题与题解 [http://blog.csdn.net/liuchuo/article/details/52510687](http://blog.csdn.net/liuchuo/article/details/52510687)

# **#include <string>**

[http://blog.csdn.net/fenxinzi557/article/details/51457829](http://blog.csdn.net/fenxinzi557/article/details/51457829)

**string赋值 **string str = “content”; equals string str (“content”);

**string尾部添加 **string.append(“”); .append(a,index0,string::npos);//把字符串a的首字母到尾字母加上

**string中间插入 **sting.insert(index,”content”);

注：单个字符串(比如数字)的插入

```cpp
string str;
int num = 7;
str+=(char)(num%7+'0');
```

**string删除 **string.erase(index,//number);

**string替换 **string.replace(index,number,”content”);

**string字符个数 **string.size() equals string.length(); //常用于循环

                         for(int 1=0; i<string.size(); i++){}

**string的比较 **string.compare(//index,//number,“content”,//index,//number);

**string的判空 **string.empty();

**string的提取 **

string[index] equals string.at(index); 单个提取

string.substr(); 返回全部内容

string.substr(index,//number);

**string的复制 **string str2 (str1);

**string的分割** string.substr(index,number) ;

**string的查找 **string.find(“contex”,//index,//number);

//从index开始找

//有，返回index，它的类型是string::size_type; 没有，返回string::npos;

find_first_of      查找包含子串中的任何字符，返回第一个位置

find_first_not_of      查找不包含子串中的任何字符，返回第一个位置

find_last_of      查找包含子串中的任何字符，返回最后一个位置

find_last_not_of      查找不包含子串中的任何字符，返回最后一个位置

```cpp
//查找子串出现的所有位置

    string substr = "i";
    position = 0;
    int i = 1;
    while((position = str.find_first_of(substr,position)) != string::npos)
    {
        cout<<"position "<<i++<<position<<endl;
        position++;
    }
```

**字符串反转**

```cpp
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
int main()
{
  string s = "hello";
  reverse(s.begin(),s.end());
  cout<<s<<endl;
  return 0;
}
```

**string的转换（****1002****）**

[http://greatverve.cnblogs.com/archive/2012/10/24/cpp-int-string.html](http://greatverve.cnblogs.com/archive/2012/10/24/cpp-int-string.html)
- int,float,double等转换为string
- string = to_string(number)
- string转换为int,float
- 首先要包含头文件#include <algorithm>
- 调用函数 atoi(string.**c_str()**); atof(string.**c_str()**)

**int转string**

```cpp
int a,b;
    cin>>a>>b;
    int c;
    c = a+b;
    char Cchar[10];
    string Cstring = itoa(c,Cchar,10);
```

也可以直接使用tostring()函数，该函数包含在<string>中

**string转int**

```cpp
std::string str = "123";
int n = atoi(str.c_str());
OR
int result = stoi(a) - stoi(b);
```

**string的分割 ****1009**

C/C++中符号的分割是很困难的，如果不能使用如下方法，那么只能组合使用string.find();和string.substr();的方法了。如下方法在PAT考试中很实用：

**string串的输入 ****1042**

string串（e.g： I love u）的输入主要要解决两个问题，string串的大小未定，不能用一个string解决（因为含有一个“ ”作为分割）
- 因为输入时就是以空格为分割，所以可以边输入边存储在vector中，但是要注意去cin.get();吸收空格，回车。当然遵循下面代码的原则就可以啦。

```cpp
vector<string> ans;
while(1){
    string str;
    cin >> str;
    ans.push_back(str);//把每个单词都加入字符串数组     
    char c = cin.get(); //用一个字符c来吸收空格
    //作用同getchar();
    if (c == '\n'){
      break;
    }     
  }
```

**Tips：**
- npos的含义: string::npos的类型是string::size_type,所以，一旦需要把一个索引与npos相比，这个索引值必须是string::size)type类型的，更多的情况下，我们可以直接把函数和npos进行比较
- 如：if(s.find(“jia”)== string::npos)  此时string::size_type position; 而不是 int position;
- string[N]是一个char类型

**C/C++中string、char *、char[]的转换**

[https://www.cnblogs.com/Pillar/p/4206452.html](https://www.cnblogs.com/Pillar/p/4206452.html)



# **#include <cmath>**

[http://blog.csdn.net/mary19920410/article/details/63696380](http://blog.csdn.net/mary19920410/article/details/63696380)

int abs 绝对值，返回整型int

**double fabs 绝对值.，返回double**

double pow(double x, double y); 返回x^y eg:PATB1012

double log(double x); 返回logeX

double log10(double x); 返回lgX;

int ceil(double x) 向上取整

int floor(double y) 向下取整

**Notes：**

使用时，直接使用函数即可。例如：计算一个数得平方根

sqrt(int a) 而不是int a=4; a=a.sqrt();



# #include<algorithm>

[http://blog.csdn.net/qq_28479381/article/details/53994972](http://blog.csdn.net/qq_28479381/article/details/53994972)

[http://blog.csdn.net/RRicky_/article/details/76696422](http://blog.csdn.net/RRicky_/article/details/76696422)

**sort()函数最为常用，但要注意：**
- sort(buffer,buffer+n,//less<data-type>()||greater<data-type>()); **buffer为待排序数组的首地址**，buffer+n为待排序数组的最后一个数据的地址。less<int>(),升序;greater<int>(),降序。默认是升序排列，也可以通过指定函数cmp，如sort(begin,end,cmp)来指定排列方式。

**reverse(myvector.begin(),myvector.end()); **翻转一个vector 两个参数，一个首地址，一个尾地址；string的操作类似。



# **#include<vector>**

[http://blog.csdn.net/duan19920101/article/details/50617190/](http://blog.csdn.net/duan19920101/article/details/50617190/)

**建立vector**

#include <vector>

vector<data-type> name;



**使用迭代器访问元素**

vector<int>::iterator it;

for(it=vec.begin();it!=vec.end();it++)

    cout<<*it<<endl;



**向vector添加元素**

尾插：name.push_back(element); 这样name[0]=element;

任意插：name.insert(vec.begin()+i,a); 在i个元素前面插入a;

```cpp
#include<vector>    
#include<iostream>    
using namespace std;    
int main()    
{    
    vector<int> v(3);    
    v[0]=2; //v[0]是第0个元素   
    v[1]=7;    
    v[2]=9;    
    v.insert(v.begin(),8);//在最前面插入新元素。    
    v.insert(v.begin()+2,1);//在迭代器中第二个元素前插入新元素    
    v.insert(v.end(),3);//在向量末尾追加新元素。    
    v.insert(v.end(),4,1);//在尾部插入4个1  
    int a[] = {1,2,3,4};  
    v.insert(v.end(),a[1],a[3]);//在尾部插入a[1]个a[3]  
    vector<int>::iterator it;    
    for(it=v.begin(); it!=v.end();it++)    
    {    
        cout<<*it<<" ";    
    }    
    cout<<endl;  
    return 0;  
}    

//8 2 1 7 9 3 1 1 1 1 4 4  
//请按任意键继续. . .
```

**删除vector内元素**

name.erase(name.begin()+i); 删除第i+1个元素

vec.erase(vec.begin()+i,vec.end()+j);删除区间[i,j-1];区间从0开始



**vector大小 **name.size();



**vector初始化**

//初始化size,但每个元素值为默认值

vector<int> abc(10); //初始化了10个默认值为0的元素

//初始化size,并且设置初始值

vector<int> cde(10，1); //初始化了10个值为1的元素



**vector查询**

```cpp
#include <algorithm>
if(find(vector.begin(), vector.end(), val) != vector.end()){
    //找到
}else{
    //没找到
}
```



**vector的排序**

```cpp
#include <algorithm>
sort(vector.begin(),vector.end());
```

(默认是按升序排列,即从小到大).

可以通过重写排序比较函数按照降序比较，如下：

定义排序比较函数：

```cpp
bool Comp(const int &a,const int &b)
{
    return a>b;
}
```

调用时:sort(vec.begin(),vec.end(),Comp)，这样就降序排序。



**vector的翻转 #include <algorithm> **reverse(vec.begin(),vec.end());



**vector二维数组 **vector< vector<type> > name //注意<<不能连写，得有空格



**vector二维数组的赋值**

```cpp
vector<vector<string> > v;
    for(int i = 0; i < 3; i++) {
        string s;
        getline(cin, s);
        vector<string> row;
        int j = 0, k = 0;
        while(j < s.length()) {
            if(s[j] == '[') {
                while(k++ < s.length()) {
                    if(s[k] == ']') {
                       row.push_back(s.substr(j+1, k-j-1));
                        break;
                    }
                }
            }
            j++;
        }
        v.push_back(row);
    }
```

**Tips:**

vector相当于一个size可变的数组。相比于数组，vector会消耗更多的内存以有效的动态增长。vector能更快的索引元素，而且能相对高效的在尾部插入和删除元素。

常和#include<algorithm>同用

reverse(name.begin(),name.end()); 逆序排列

sort(name.begin(),name.end()) ; 升序排列

**特别注意：**
- 如果你要表示的向量长度较长（需要为向量内部保存很多数），容易导致内存泄漏，而且效率会很低；

# #include <cctype>

[http://blog.csdn.net/bat67/article/details/52021803](http://blog.csdn.net/bat67/article/details/52021803)

int tolower(char)  若char是大写字母('A'-'Z')返回相应的小写字母('a'-'z')的ASCII码

int toupper(char)  若char是小写字母('a'-'z')返回相应的大写字母('A'-'Z')的ASCII码

int isupper(int ch) 若ch是大写字母('A'-'Z')返回非0值,否则返回0

int islower(int ch) 若ch是小写字母('a'-'z')返回非0值,否则返回0

int isalpha(int ch) 若ch是字母('A'-'Z','a'-'z')返回非0值,否则返回0

int isdigit(int ch) 若ch是数字('0'-'9')返回非0值,否则返回0

int isalnum(int ch) 若ch是字母('A'-'Z','a'-'z')或数字('0'-'9') 返回非0值,否则返回0



**sample：**

```cpp
#include<ctype.h>  
#include<stdio.h>  
int main()  
{  
    char str[]="I love ShangHai";  
    //将字符串中的小写字母转化为大写字母  
    int i=0;  
    char c;  
    while (str[i])  
    {  
        c=str[i];  
        if (islower(c)) str[i]=toupper(c);         
        i++;      
    }  
    printf("%s\n",str);  
}
```



1033     [1033 旧键盘打字——C/C++实现](https://blog.csdn.net/zhanshen112/article/details/84502911)

1014[1014 福尔摩斯的约会——C/C++实现](https://blog.csdn.net/zhanshen112/article/details/84331171)



# #inlcude <map>

自动建立Key － value的对应。key 和 value可以是任意你需要的类型。



**map的定义 **

> 
**map<int, string****//typename****> name**


map的访问

数组访问：map[0] map['a''] map["ans"]...



**数据的构造，插入**

```cpp
#include<map>
int main(){
    map<int,string>mapStudent;
    mapStudent.insert(pair<int,string>(1,"student_one"));
    return 0;
}


int main(){
    map<int,string>mapStudent;
    mapStudent.insert(map<int,string>::value_type(1,"student_one"));
return 0;
}


//遍历：
map<string,CAgent>::iterator iter;
 for(iter = m_AgentClients.begin(); iter != m_AgentClients.end(); ++iter)
 {
  if(iter->first=="8001")  {
   this->SendMsg(iter->second.pSocket,strMsg);//iter->first
  }
 }

//查找：

map<string,CAgent>::iterator iter=m_AgentClients.find(strAgentName);
 if(iter!=m_AgentClients.end())//有重名的 {
 }
 else //没有{
 }

//元素的个数

if (m_AgentClients.size()==0)
```

1069

[1069 微博转发抽奖——c++实现](https://blog.csdn.net/zhanshen112/article/details/84037036)



# #include <set>

[http://blog.csdn.net/yas12345678/article/details/52601454](http://blog.csdn.net/yas12345678/article/details/52601454)

set主要用于自动去重并按升序排序

set<int> test;



**set插入数值**： test.insert(0);

//不是使用push_back()哦



**set删除一个特定数值 **: test.erase(test.find(0));



**清空set ** test.clear();



**注意****set的遍历**

set<int>::iterator = it;

for(it = test.begin();it!=test.end();it++){

XXXXX

}
- **反向迭代器：**

set<int>::reverse_iterator = it;

it.rbegin() 返回一个逆序迭代器，它指向容器it的最后一个元素

it.rend() 返回一个逆序迭代器，它指向容器it的第一个元素前面的位置



# C/C++的输入

[http://blog.csdn.net/JIEJINQUANIL/article/details/50802902](http://blog.csdn.net/JIEJINQUANIL/article/details/50802902)

10521058

getchar(); 吸收诸如'()’、'【】'

[http://blog.csdn.net/zcmuczx/article/details/53926178](http://blog.csdn.net/zcmuczx/article/details/53926178)

[http://blog.csdn.net/zcmuczx/article/details/53926352](http://blog.csdn.net/zcmuczx/article/details/53926352)

1.**cin**最常见，不再多赘述，注意它在遇“空格”、“TAB”、“回车”都结束

2.**cin.get();** 用来接收字符，可以接受“空格”

```cpp
char c;
while(1){
  c = cin.get();
  cout<<c;
  if(c=='\n'){
    break;
  } 
}
```
- **(chat c = cin.get()) != EOF**

```cpp
#include <iostream>
using namespace std;
int main() {
 int map[128] = {0}, c;
 while ((c = cin.get()) != EOF) map[c]++;
 while (map['P'] > 0 || map['A'] > 0 || map['T'] > 0 || map['e'] > 0 || map['s'] > 0 || map['t'] > 0) {
  if (map['P']-- > 0) cout << 'P';
  if (map['A']-- > 0) cout << 'A';
  if (map['T']-- > 0) cout << 'T';
  if (map['e']-- > 0) cout << 'e';
  if (map['s']-- > 0) cout << 's';
  if (map['t']-- > 0) cout << 't';
 }
 return 0;
}
```

3.**cin.getline();** 接受一个字符串，可以接受“空格”并输出

```cpp
#include <iostream>
using namespace std;
void main ()
{
  char m[20];
  cin.getline(m,5);
  cout<<m<<endl;
}
//cin.getline()实际上有三个参数，cin.getline(接受字符串的首地址,接受个数5,结束字符) 
//当第三个参数省略时，系统默认为’\0’
```

4.**getline()** 接受一个字符串，可以接收空格并输出，需包含“#include <string>”
- 和cin.getline()类似，但是cin.getline()属于istream流，而getline()属于string流，是不一样的两个函数。但两者好像可以实现相同的功能，只是所需要的头文件不同。参考：[C++中cin、cin.get()、cin.getline()、getline()、gets()等函数的用法](https://blog.csdn.net/zhanshen112/article/details/85113031)

```cpp
string str;
  getline(cin,str);
  //不可以和cin连用，但可以和scanf连用
  //错误示范
  /*
  cin>>n;
  //应该修改为：
  //scanf("%d\n",&n); 
  for(int i=0;i<n;i++){
     string str;
     getline(cin,str); 
  }
  */
```



**5.特别的输入**

```cpp
#include <iostream>
using namespace std;
int main() {
  int a, b, flag = 0;
  while (cin >> a >> b) {
    if (flag == 0 && b == 0) cout << "0 0";
    if (b != 0) {
      if (flag == 1) cout << " ";
      cout << a * b << " " << b - 1;
      flag = 1;
    }
  }
  return 0;
}
```

**6.格式化输入**

# C/C++输出格式控制

C++在输出格式控制方面过于繁琐，PAT中遇到特定格式输出，建议使用C。

但是要注意，一旦想选择C，所有输入输出都用C（即scanf("%d",&m); printf("%03d",m);）

[http://blog.csdn.net/u010879456/article/details/53404757](http://blog.csdn.net/u010879456/article/details/53404757)



**C语言的基本输出格式：**

[https://www.cnblogs.com/renxinyuan/p/3862772.html](https://www.cnblogs.com/renxinyuan/p/3862772.html)

**基本格式：**

%c 字符

%d 整形

%f float或者double

%s 字符串

\n 换行



**对齐**

"-" 有则表示左对齐输出，如省略表示右对齐输出

“0” 有则表示空位填0.如省略表示指定空位不填 "

"%03d" 用于控制00X式的输出

printf("%.2f",a); 如果a是-0.04则输出-0.00 这边的负号需要考虑到

m.n m指域宽，n指的精度，一般默认n=6 用于控制.XXX的输出

“%.Xf” 用于控制0.XXXXX式的输出，结果自动四舍五入



# 结构体

**结构体排序**

```cpp
#include <iostream>
#include <algorithm>
#include <string>
using namespace std;
struct Person{
    string name;
    int age;
};

bool cmp(Person a,Person b){  //名字从小到大，同名情况下年龄从大到小
    if(a.name!=b.name){
        return a.name<b.name;
    } else {
        return a.age>b.age;
    }
}

int main(){
    Person p[3];
    int i,j;
    for(i=0;i<3;i++){
        cin>>p[i].name>>p[i].age;
    }
    //排序
    sort(p,p+3,cmp);
    //依次输出
    for(i=0;i<3;i++){
        cout<<p[i].name<<' '<<p[i].age<<endl;
    }
}
```



# 链表
- 静态链表与动态链表

1、静态链表是用类似于数组方法实现的，是顺序的存储结构，在物理地址上是连续的，而且**需要预先分配地址空间大小**。所以静态链表的初始**长度一般是固定的**，在做插入和删除操作时不需要移动元素，仅需修改指针。

2、动态链表是用内存申请函数（malloc/new）动态申请内存的，所以在链表的长度上没有限制。动态链表因为是动态申请内存的，所以**每个节点的物理地址不连续**，要通过指针来顺序访问。



# Hash
- 字符串Hash

参考：[胡凡《算法笔记》总结2——Hash散列](https://blog.csdn.net/zhanshen112/article/details/84951513)



# 经验
- 用数组前，请三思，这个数据是不是一定要存在(二维)数组中，如果不用请考虑，边输入边处理输出。 (1046,1061,1066)
- 一定要按输出格式打印出空格(" ")和换行符（\n）
- 一对象多属性，**结构体**，了解一下
- **输出做一个二选一的决定：**cout<<(a+b<c?"true":"fale");
- int/int 不要妄想得到一个float型的数字，要想得到 (float)int/int; 强制类型转换
- 同时也要注意int/int，被除数不能是0,不然会得到一个很大的数
- C语言输入数组 scanf(" % data-type",&a[i])
- 如果觉得用if(){}else{}处理很low 请尝试switch(); case 0: XXXX,XXXXX;break;
- 怎么求素数心里要有X数，此外求闰年（是4但不是10的倍数，是400的倍数）也是

```cpp
#include <iostream>
#include <vector>
using namespace std;
bool isprime(int a) {
    for (int i = 2; i * i <= a; i++)
        if(a % i == 0) return false;
    return true;
}

int main() {
    int M, N, num = 2, cnt = 0;
    cin >> M >> N;
    vector<int> v;
    while (cnt < N) {
        if (isprime(num)) {
            cnt++;
            if (cnt >= M) v.push_back(num);
        }
        num++;
    }

    cnt = 0;
    for (int i = 0; i < v.size(); i++) {
        cnt++;
        if (cnt % 10 != 1) printf(" ");
        printf("%d", v[i]);
        if (cnt % 10 == 0) printf("\n");
    }
    return 0;
}
```
- 关注数据结构的基本排序
- 是int还是long long int呢？
- int范围：-2147483648~21474836473 题目中一般提示：不超过十位数
- long long int： -9223372036854775808~9223372036854775807 给定区间[-2^31, 2^31]
- 输入案例要尽可能考虑周到，尤其是考虑到特殊情况
- 数组的初始化为一个具体的数，int a[10]={0}; //数组的元素全部初始化为0
- 数组的查找

```cpp
int temp[100]={3,4,2，1}
find(temp,temp+100,1)
//返回找到的内存地址，取* 可以重新赋值如 temp[3]=1;*find(temp,temp+100,1)=2;
//此时temp[3]=2;//如找不到 *find(temp,temp+100,1)==0
```
- 对于模板化输入，例如XX-XX XX可以使用scanf("%d-%d %d",&a,&b,&c);
- 在可能键值对操作(hash散列)的时候 Or 要先存再sort的时候，请三思是不是值得，换成数组（数字就用下标表示，字母-‘a’得到一个0~26的数字，亦转换成了数组下表形式的散列）是不是更便利。(求最大值这类问题，最大数组下标先用一个变量表示) 1032 1047
- 考前把进制转换了解一下： [https://www.cnblogs.com/Nastukashii/p/4394289.html](https://www.cnblogs.com/Nastukashii/p/4394289.html)
- 十进制转二进制
- 方法为：十进制数除2取余法，即十进制数除2，余数为权位上的数，得到的商值继续除2，依此步骤继续向下运算直到商为0为止。



![](https://img-blog.csdnimg.cn/20181220104208150)


- 输出：“@\/@”的’\’是转义字符，想要输出’\’就要用’\\’表示
- 运行超时要注意减少循环次数。如果运行超时，赶紧进入下一题，往往解决超时，伴随着方法的更变（代码的重构/写）
- 最小公倍数与最大公约数

[http://blog.jobbole.com/106315/](http://blog.jobbole.com/106315/)

辗转相除法：**两个正整数a和b（a>b）**，它们的最大公约数等于a除以b的余数c和b之间的最大公约数。

![](https://img-blog.csdnimg.cn/20181220104208178)



最大公因子求出来了，那么最小公倍数呢？

其实只要在运算函数里面返回两个数的积与最大公因子的比值即可,既：a*b/qcd;

不需要考虑ab的大小关系

```cpp
int gcd(int a,int b){
    return b==0?a:gcd(b,a%b);
}
```





