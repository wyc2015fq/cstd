# C++容器 bitset - 紫芝的博客 - CSDN博客





2018年05月12日 12:22:57[紫芝](https://me.csdn.net/qq_40507857)阅读数：52
个人分类：[C/C++基础知识](https://blog.csdn.net/qq_40507857/article/category/7565821)











C++语言的一个类库，用来方便地管理一系列的bit位而不用程序员自己来写代码。

要使用bitset 类我们必须包含相关的头文件    #include <bitset>



bitset除了可以访问指定下标的bit位以外，还可以把它们作为一个整数来进行某些统计。

可以如下声明一个该类型变量：

bitset<N>varm (M)

其中varm为变量名。

N表示该类型在内存中占的位数，是二进制。

M表示变量varm的初始值。


![](https://gss0.bdstatic.com/-4o3dSag_xI4khGkpoWK1HF6hhy/baike/c0%3Dbaike92%2C5%2C5%2C92%2C30/sign=57fd63aafc039245b5b8e95de6fdcfa7/503d269759ee3d6dc1c812ac43166d224f4ade4f.jpg)




bitset 有三种声明方式。在缺省定义中，我们只需简单地指明位向量的长度。例如：

bitset< 32 > bitvec;

声明了一个含有32 个位的bitset，对象位的顺序从0 到31。缺省情况下所有的位都被初始化为0 。



```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);

    bitset<10>first;//empty bitset
    cout<<first<<endl;//输出10个0

    bitset<10>second(120ul);//initlize from unsigned long
    //初始化为无符号型整数120

    bitset<10>a(5);
    cout<<a<<endl;//5的二进制数
    printf("%d\n",a);//输出5

    bitset<32>b(-5);
    cout<<b<<endl;//-5的二进制数（补码表示）
    printf("%d\n",b);//输出-5

    bitset<10>c(-5);
    cout<<c<<endl;//-5的二进制数（补码表示）
    printf("%d\n",c);//输出10位二进制数转化为十进制的结果

    bitset<10>d(string("000101"));//initalize from string
    cout<<d<<endl;
    printf("%d\n",d);//将01字符串转化为十进制正整数

    bitset<10>e(string("1111111011"));//initalize from string
    cout<<e<<endl;
    printf("%d\n",e);//将01字符串转化为十进制正整数
	return 0;
}
```




## ***相关函数***：



### 1.*any()*：


为了测试bitset 对象是否含有被设置为1的位，我们可以使用any()操作

当bitset对象的一位或多个位被设置为1 时any()返回true

例如，对于bitvec ，如下测试

bool is_set = bitvec.any();

它的结果当然是false。



```cpp
#include<iostream>
#include<bitset>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);

    bitset<32>mybits;
	cin>>mybits;
	if(mybits.any())//任何一位被设置就返回true
        cout<<"mybits has "<<(int)mybits.count()<<"bits set\n";
    else
        cout<<"No bit set\n";
	return 0;
}
```



### 2.*none()*：


相反，如果bitset 对象的所有位都被设置为0 ,则none()操作返回true

例如，对于bitvec 测试

bool is_not_set = bitvec.none();

结果为true



```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);

    bitset<32>mybits;
	cin>>mybits;
	if(mybits.none())//没有任何一位被设置就返回true
        cout<<"No bit set\n";
    else
        cout<<"mybits has "<<(int)mybits.count()<<"bits set\n";
	return 0;
}
```



### 3.*count()*：


count()操作返回被设置为1的位的个数.

int bits_set = bitvec.count();



```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);
    bitset<8> myset(string("10110011"));
    cout<<int(myset.count())<<" ones\n";
    cout<<int(myset.size()-myset.count())<<" zeros\n";
	return 0;
}
```


### 4.*set()*：


我们可以用set()操作或者下标操作符来设置某个单独的位

例如，下面的for循环把下标为偶数的设置为1.

for ( int index = 0; index < 32; ++ index )

if ( index % 2 == 0 )

bitvec[ index ] = 1;



```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);

    bitset<4> mybits;
    cout<<mybits.set()<<endl;//1111
    cout<<mybits.set(2,0)<<endl;//1011
    cout<<mybits.set(2)<<endl;//1111
	return 0;
}
```



### 5.*test()*：


测试某个单独的位的状态 




```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);
    string bitval("01011");
    bitset<5> mybits(bitval);
    cout<<boolalpha;
    for(size_t i=0;i<mybits.size();++i)
    cout<<mybits.test(i)<<endl;//返回指定位的状态
	return 0;
}
```



### 6.*reset()*：


要将某个单独的位设置为0 ,我们可以用reset()或下标操作符

下列两个操作都将bitvec的第一位设为0.

// 两者等价都把第一位设置为0

bitvec.reset( 0 );

bitvec[ 0 ] = 0;

我们也可以用set()和reset()操作将整个bitset 对象的所有位设为1 或0 ,只要**调用相应的操作而不必传递位置参数，**我们就可以做到这一点.例如:

bitvec.reset();// 把所有的位设置为0


```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);

    bitset<4> mybits(string("1011"));
    cout<<mybits.reset(2)<<endl;//1001
    cout<<mybits.reset()<<endl;//0000
	return 0;
}
```



### 7.*flip()*：


flip()操作翻转整个bitset 对象或一个独立的位

bitvec.flip( 0 ); // 翻转第一位

bitvec[0].flip(); // 也是翻转第一位

bitvec.flip(); // 翻转所有的位的值




```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);
    bitset<4> mybits(string("0001"));
    cout<<mybits.flip(2)<<endl;//0101
    cout<<mybits[2].flip()<<endl;//0
    cout<<mybits.flip()<<endl;//1010
	return 0;
}
```

### 8.to_ulong();

返回bitset的整数表示

```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);

    //string bitval("01011");
    string bitval="01011";
    bitset<5> mybits(bitval);
    cout<<mybits.to_ulong()<<endl;//返回bitset的整数表示


	return 0;
}
```

### 9.to_string():

返回bitset的字符串表示

```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);

    bitset<5> mybits;//0000
    mybits.set();//1111
    string s=mybits.to_string();
    cout<<s<<endl;//返回bitset的字符串表示

	return 0;
}
```

### 10.operator[]()

返回第x位的引用

```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);

     bitset<5> mybits;//0000
    mybits[1]=1;
    mybits[2]=mybits[1];
    cout<<mybits<<endl;

	return 0;
}
```


### 11.operatorss()
比较和赋值运算


```cpp
#include<iostream>
#include<bitset>
#include<cstdio>
using namespace std;

int main()
{
	ios::sync_with_stdio(0);
    bitset<4>first(string("1001"));
    bitset<4>second(string("0011"));

    cout<<(first^=second)<<endl;//1010
    cout<<(first&=second)<<endl;//0010
    cout<<(first|=second)<<endl;//0011
    cout<<endl;

    cout<<(first<<=2)<<endl;//1100
    cout<<(first>>=1)<<endl;//0110
    cout<<endl;

    cout<<(~second)<<endl;//1100
    cout<<(second<<1)<<endl;//0110
    cout<<(second>>1)<<endl;//0001
    cout<<endl;


    cout<<(first==second)<<endl;//false
    cout<<(first!=second)<<endl;//true
    cout<<endl;

    cout<<(first&second)<<endl;//0010
    cout<<(first|second)<<endl;//0111
    cout<<(first^second)<<endl;//0101
    cout<<endl;
	return 0;
}
```





