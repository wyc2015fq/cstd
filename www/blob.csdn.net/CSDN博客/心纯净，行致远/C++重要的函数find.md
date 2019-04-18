# C++重要的函数find - 心纯净，行致远 - CSDN博客





2018年12月09日 10:42:17[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：657








# C++ STL中有两种find

1、algorithm中的函数

其调用形式为

find（start,end,value）

start搜寻的起点，end搜寻的终点，要寻找的value值

容器的表示方法

find（a.begin(),a.end(),value）

数组的表示方法

find（a,a+length,val）

所有的返回，均是迭代器（容器）或指针（数组），而非是直观感觉上的索引下标。如果在查找范围内不存在，返回a.end()，这里需要注意的是，a.end()不在查找范围内。



我们可以进一步思考，为什么返回的不是索引下标？这是因为索引下标是在数组这种连续存储的数据结构里面常见的，是一种顺序索引；而很多数据结构的存储并非连续的，不能实现顺序索引，为了兼顾所有的数据结构，所以返回的仅仅是指针或是迭代器。



2、各个容器自己实现的成员函数

vector没有实现find函数，除此之外，常见容器都实现了自己的find函数。

String是这一种顺序存储结构，其find函数返回的是下标索引。set,map,multiset,multimap都不是顺序索引的数据结构，所以返回的是迭代器。



3、如果元素不在

对于返回迭代器的查找，通过判断find（a.begin(),a.end(),value）==a.end()，来判断元素是否存在

对于string，通过a.find(val)==string::npos判断


# Find()

### 很多语言都内置了非常方便的查找函数方便我们调用，其实C++中也有find函数：

### vector中

使用find()函数需要#include<algorithm>，例如：

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



### string中

我是觉得find可以使用到string中真的是非常方便，例如：

string str = "hello world";

    char ch = 'l';

    if(str.find(ch)!=string::npos){ //查找单个字符

        cout<<str.find(ch)<<endl;

    }

    else

        cout<<"NO"<<endl;

我们也可以看出这样返回的结果是匹配到的字符的第一个位置，如果有多个字符都可以匹配，那么只会返回第一个了，另外，关于string:npos ，如果我们故意没有匹配到，并输出结果，可以看到string:npos其实是等于4294967295的，通常使用这个值作为没有匹配到内容的结果判定。如果我们想要获取到所有的匹配字符的位置怎么办呢？其实我们find函数是有很多方式的，我们还可以通过第二个参数来控制开始查找的位置。参考：string find()

int p = 0;

while(str.find(ch, p)!=string::npos){

            p = str.find(ch, p);

            cout<<p<<endl;

            p = p + 1;

        }

这样通过一个位置p不断的更新上一次的匹配位置，我们可以不断地更新开始位置，这样就能得到所有地匹配位置了。

同样地，string地find函数也可以匹配到字符串：

    string ps;

    while(cin>>ps){

        if(str.find(ps)!=string::npos){

            cout<<str.find(ps)<<endl;

        }

        else

            cout<<"NO"<<endl;

    }



