# C++ vector容器类型的用法及注意 - xcw0754 - 博客园
# [C++ vector容器类型的用法及注意](https://www.cnblogs.com/xcw0754/p/4224193.html)
转自http://www.cnblogs.com/charley_yang/archive/2010/12/11/1903040.html
vector类为内置数组提供了一种替代表示，与string类一样 vector 类是随标准 C++引入的标准库的一部分 ，为了使用vector 我们必须包含相关的头文件  ：
**#include <vector>**
使用vector有两种不同的形式，即所谓的数组习惯和 STL习惯。
# 一、数组习惯用法
1. 定义一个已知长度的 vector ：
vector< int > vec( 10 );  //类似数组定义int a[ 10 ];
可以通过vec[索引号] 来访问元素
使用 if ( vec.empty() ) 判断是否是空，vec.size()判断元素个数。
2. vector的元素被初始化为与其类型相关的缺省值：算术和指针类型的缺省值是 0，对于class 类型，缺省值可通过调用这类的缺省构造函数获得，我们还可以为每个元素提供一个显式的初始值来完成初始化，例如  
vector< int > vec( 10, -1 ); 
定义了 vec 它包含十个int型的元素 每个元素都被初始化为-1 
对于内置数组 我们可以显式地把数组的元素初始化为一组常量值，例如 ： 
int a[ 6 ] = { -2, -1, 0, 1, 2, 1024 };
我们不能用同样的方法显式地初始化 vector ，但是可以将 vector 初始化为一个已有数组的全部或一部分，只需指定希望被用来初始化 vector 的数组的开始地址以及数组最末元的下一位置来实现，例如：  
// 把 ia 的 6 个元素拷贝到 ivec 中 
vector< int > vec( a, a+6 );
被传递给vec 的两个指针标记了用来初始化对象的值的范围，第二个指针总是指向要拷贝的末元素的下一位置，标记出来的元素范围也可以是数组的一个子集，例如 :
// 拷贝 3 个元素 a[2],a[3],a[4] 
vector< int > vec( &a[ 2 ], &a[ 5 ] );
3. 与内置数组不同 vector 可以被另一个 vector 初始化 或被赋给另一个 vector 例如  
vector< string > svec; 
void init_and_assign() 
{ 
    // 用另一个 vector 初始化一个 vector 
    vector< string > user_names( svec ); 
    // ... 
    // 把一个 vector 拷贝给另一个 vector 
    svec = user_names; 
}
# 二、STL习惯用法
在 STL9中对vector 的习惯用法完全不同。我们不是定义一个已知大小的 vector，而是定义一个空 vector  
vector< string > text;
1. 我们向 vector 中插入元素，而不再是索引元素，以及向元素赋值，例如 push_back()操作，就是在 vector 的后面插入一个元素下面的 while 循环从标准输入读入一个字符串序列并每次将一个字符串插入到 vector 中  
string word; 
while ( cin >> word ) { 
text.push_back( word ); 
// ... 
}
虽然我们仍可以用下标操作符来迭代访问元素  
　　cout << "words read are: \n"; 
　　for ( int ix = 0; ix < text.size(); ++ix ) 
 　　     cout << text[ ix ] << ' '; 
　　cout << endl; 
但是 更典型的做法是使用 vector 操作集中的begin()和 end()所返回的迭代器iterator
对 ：
　　cout << "words read are: \n"; 
　　for ( vector<string>::iterator it = text.begin(); 
 　　   it != text.end(); ++it ) 
           cout << *it << ' '; 
　　cout << endl 
iterator 是标准库中的类，它具有指针的功能 
　　*t; 
　　对迭代器解引用，并访问其指向的实际对象  
　　++t;
　　向前移动迭代器 it 使其指向下一个元素  
2. 注意 不要混用这两种习惯用法， 例如，下面的定义  
　　vector< int > vec; 
　　定义了一个空vector 再写这样的语句  
　　vec[ 0 ] = 1024; 
　　就是错误的 ，因为 vec 还没有第一个元素，我们只能索引 vector 中已经存在的元素 size()操作返回 vector 包含的元素的个数 。
3. 类似地 当我们用一个给定的大小定义一个 vector 时，例如  ：
　　vector<int> a( 10 );
　　任何一个插入操作都将增加vector 的大小，而不是覆盖掉某个现有的元素，这看起来好像是很显然的，但是 下面的错误在初学者中并不少见 ：
　　const int size = 7; 
　　int a[ size ] = { 0, 1, 1, 2, 3, 5, 8 }; 
　　vector< int > vec( size ); 
　　for ( int x = 0; x < size; ++x ) 
   　　 vec.push_back( a[ ix ]); 
程序结束时ivec 包含 14 个元素, ia 的元素从第八个元素开始插入。

