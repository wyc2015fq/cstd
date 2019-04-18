# C++ STL之vector详解 - L_Andy的专栏 - CSDN博客

2014年09月04日 12:47:24[卡哥](https://me.csdn.net/L_Andy)阅读数：13095


                
Vectors

   vector是C++标准模板库中的部分内容，它是一个多功能的，能够操作多种数据结构和算法的模板类和函数库。vector之所以被认为是一个容器，是因为它能够像容器一样存放各种类型的对象，简单地说，vector是一个能够存放任意类型的动态数组，能够增加和压缩数据。为了可以使用vector，必须在你的头文件中包含下面的代码：#include <vector>

构造函数。

  Vectors 包含着一系列连续存储的元素,其行为和数组类似。访问Vector中的任意元素或从末尾添加元素都可以在常量级时间复杂度内完成，而查找特定值的元素所处的位置或是在Vector中插入元素则是线性时间复杂度。

函数列表如下:

Constructors 构造函数 

Operators 对vector进行赋值或比较 

assign() 对Vector中的元素赋值 

at() 返回指定位置的元素 

back() 返回最末一个元素 

begin() 返回第一个元素的迭代器 

capacity() 返回vector所能容纳的元素数量(在不重新分配内存的情况下） 

clear() 清空所有元素 

empty() 判断Vector是否为空（返回true时为空） 

end() 返回最末元素的迭代器(译注:实指向最末元素的下一个位置) 

erase() 删除指定元素 

front() 返回第一个元素 

get_allocator() 返回vector的内存分配器 

insert() 插入元素到Vector中 

max_size() 返回Vector所能容纳元素的最大数量（上限） 

pop_back() 移除最后一个元素 

push_back() 在Vector最后添加一个元素 

rbegin() 返回Vector尾部的逆迭代器 

rend() 返回Vector起始的逆迭代器 

reserve() 设置Vector最小的元素容纳数量 

resize() 改变Vector元素数量的大小 

size() 返回Vector元素数量的大小 

swap() 交换两个Vector

////////////////////////////////////////////////////////////////////////////

                               函数详细说明

构造函数

语法: 

vector();

vector( size_type num, const TYPE &val );

vector( const vector &from );

vector( input_iterator start, input_iterator end );

C++ Vectors可以使用以下任意一种参数方式构造: 

无参数 - 构造一个空的vector, 

数量(num)和值(val) - 构造一个初始放入num个值为val的元素的Vector 

vector(from) - 构造一个与vector from 相同的vector 

迭代器(start)和迭代器(end) - 构造一个初始值为[start,end)区间元素的Vector(注:半开区间). 

举例,下面这个实例构造了一个包含5个值为42的元素的Vector

vector<int> v1( 5, 42 );

运算符

语法: 

v1 == v2

v1 != v2

v1 <= v2

v1 >= v2

v1 < v2

v1 > v2 

v[]

C++ Vectors能够使用标准运算符: ==, !=, <=, >=, <, 和 >. 要访问vector中的某特定位置的元素可以使用 [] 操作符. 

两个vectors被认为是相等的,如果: 

它们具有相同的容量 

所有相同位置的元素相等. 

vectors之间大小的比较是按照词典规则. 

assign函数 

语法: 

void assign( input_iterator start, input_iterator end );

void assign( size_type num, const TYPE &val );

assign() 函数要么将区间[start, end)的元素赋到当前vector,或者赋num个值为val的元素到vector中.这个函数将会清除掉为vector赋值以前的内容.

at函数 

语法: 

TYPE at( size_type loc );

at() 函数 返回当前Vector指定位置loc的元素的引用. at() 函数 比 [] 运算符更加安全, 因为它不会让你去访问到Vector内越界的元素. 例如, 考虑下面的代码:

vector<int> v( 5, 1 );

for( int i = 0; i < 10; i++ ) {

cout << "Element " << i << " is " << v[i] << endl;

}

这段代码访问了vector末尾以后的元素,这将可能导致很危险的结果.以下的代码将更加安全: 

vector<int> v( 5, 1 );

for( int i = 0; i < 10; i++ ) {

cout << "Element " << i << " is " << v.at(i) << endl;

}

取代试图访问内存里非法值的作法,at() 函数能够辨别出访问是否越界并在越界的时候抛出一个异常out_of_range.

back 函数 

语法: 

TYPE back();

back() 函数返回当前vector最末一个元素的引用.例如: 

vector<int> v;

for( int i = 0; i < 5; i++ ) {

v.push_back(i);

}

cout << "The first element is " << v.front() 

     << " and the last element is " << v.back() << endl;

这段代码产生如下结果:

The first element is 0 and the last element is 4

begin 函数 

语法: 

iterator begin();

begin()函数返回一个指向当前vector起始元素的迭代器.例如,下面这段使用了一个迭代器来显示出vector中的所有元素:

vector<int> v1( 5, 789 );

vector<int>::iterator it;

for( it = v1.begin(); it != v1.end(); it++ )

cout << *it << endl;

capacity 函数 

语法: 

size_type capacity();

capacity() 函数 返回当前vector在重新进行内存分配以前所能容纳的元素数量.

clear 函数 

语法: 

void clear();

clear()函数删除当前vector中的所有元素.

empty 函数 

语法: 

bool empty();

如果当前vector没有容纳任何元素,则empty()函数返回true,否则返回false.例如,以下代码清空一个vector,并按照逆序显示所有的元素:

vector<int> v;

for( int i = 0; i < 5; i++ ) {

    v.push_back(i);

}

while( !v.empty() ) {

    cout << v.back() << endl;

    v.pop_back();

}

end 函数 

语法: 

iterator end();

end() 函数返回一个指向当前vector末尾元素的下一位置的迭代器.注意,如果你要访问末尾元素,需要先将此迭代器自减1.

erase 函数 

语法: 

iterator erase( iterator loc );

iterator erase( iterator start, iterator end );

erase函数要么删作指定位置loc的元素,要么删除区间[start, end)的所有元素.返回值是指向删除的最后一个元素的下一位置的迭代器.例如:

// 创建一个vector,置入字母表的前十个字符

vector<char> alphaVector;

for( int i=0; i < 10; i++ )

    alphaVector.push_back( i + 65 );

int size = alphaVector.size();

vector<char>::iterator startIterator;

vector<char>::iterator tempIterator;

for( int i=0; i < size; i++ )

{

    tartIterator = alphaVector.begin();

    alphaVector.erase( startIterator );

    // Display the vector

    for( tempIterator = alphaVector.begin(); tempIterator != alphaVector.end(); tempIterator++ )

    cout << *tempIterator;

    cout << endl;

} 

这段代码将会显示如下输出:

BCDEFGHIJ

CDEFGHIJ

DEFGHIJ

EFGHIJ

FGHIJ

GHIJ

HIJ

IJ

J

front 函数 

语法: 

TYPE front();

front()函数返回当前vector起始元素的引用

get_allocator 函数 

语法: 

allocator_type get_allocator();

get_allocator() 函数返回当前vector的内存分配器.在STL里面一般不会调用new或者alloc来分配内存,而且通过一个allocator对象的相关方法来分配.

示例:vector<int>v3( 3, 1, v2.get_allocator( ));//把V2的内存分配器作为一个参数参与构造V3。这样，它们两个用一个内存分配器了。

insert 函数 

语法: 

iterator insert( iterator loc, const TYPE &val );

void insert( iterator loc, size_type num, const TYPE &val );

void insert( iterator loc, input_iterator start, input_iterator end );

insert() 函数有以下三种用法: 

在指定位置loc前插入值为val的元素,返回指向这个元素的迭代器, 

在指定位置loc前插入num个值为val的元素 

在指定位置loc前插入区间[start, end)的所有元素 . 

举例: 

//创建一个vector,置入字母表的前十个字符

vector<char> alphaVector;

for( int i=0; i < 10; i++ )

alphaVector.push_back( i + 65 );

//插入四个C到vector中

vector<char>::iterator theIterator = alphaVector.begin();

alphaVector.insert( theIterator, 4, 'C' );

//显示vector的内容

for( theIterator = alphaVector.begin(); theIterator != alphaVector.end(); theIterator++ )

cout << *theIterator;

这段代码将显示: 

CCCCABCDEFGHIJ

max_size 函数 

语法: 

size_type max_size();

max_size() 函数返回当前vector所能容纳元素数量的最大值(译注:包括可重新分配内存). 

pop_back 

语法: 

void pop_back();

pop_back()函数删除当前vector最末的一个元素,例如:

vector<char> alphaVector;

for( int i=0; i < 10; i++ )

    alphaVector.push_back( i + 65 );

int size = alphaVector.size();

vector<char>::iterator theIterator;

for( int i=0; i < size; i++ ) {

alphaVector.pop_back();

for( theIterator = alphaVector.begin(); theIterator != alphaVector.end(); theIterator++ )

      cout << *theIterator;

cout << endl;

}

这段代码将显示以下输出:

ABCDEFGHI

ABCDEFGH

ABCDEFG

ABCDEF

ABCDE

ABCD

ABC

AB

A

push_back 函数 

语法: 

void push_back( const TYPE &val );

push_back()添加值为val的元素到当前vector末尾

rbegin 函数 

语法: 

reverse_iterator rbegin();

rbegin函数返回指向当前vector末尾的逆迭代器.(译注:实际指向末尾的下一位置,而其内容为末尾元素的值,详见逆代器相关内容)

示例:

vector<int>v1;

for(int i=1;i<=5;i++)

{

    v1.push_back(i);

}

vector<int>::reverse_iterator pos;

pos=v1.rbegin();

cout<<*pos<<" ";

pos++;

cout<<*pos<<endl;

输出结果为:5 4

rend 函数 

语法: 

reverse_iterator rend();

rend()函数返回指向当前vector起始位置的逆迭代器. 

示例:

vector<int>v1;

for(int i=1;i<=5;i++)

{

    v1.push_back(i);

}

vector<int>::reverse_iterator pos;

pos=v1.rend();

pos--;

cout<<*pos<<" ";

pos--;

cout<<*pos<<endl;

输出结果为:1 2

reserve 函数 

语法: 

void reserve( size_type size );

reserve()函数为当前vector预留至少共容纳size个元素的空间.(译注:实际空间可能大于size)

resize 函数 

语法: 

void resize( size_type size, TYPE val );

resize() 函数改变当前vector的大小为size,且对新创建的元素赋值val

resize 与reserve的区别

    reserve是容器预留空间，但并不真正创建元素对象，在创建对象之前，不能引用容器内的元素，因此当加入新的元素时，需要用push_back()/insert()函数。

    resize是改变容器的大小，并且创建对象，因此，调用这个函数之后，就可以引用容器内的对象了，因此当加入新的元素时，用operator[]操作符，或者用迭代器来引用元素对象。再者，两个函数的形式是有区别的，reserve函数之后一个参数，即需要预留的容器的空间；resize函数可以有两个参数，第一个参数是容器新的大小，第二个参数是要加入容器中的新元素，如果这个参数被省略，那么就调用元素对象的默认构造函数。

初次接触这两个接口也许会混淆，其实接口的命名就是对功能的绝佳描述，resize就是重新分配大小，reserve就是预留一定的空间。这两个接口即存在差别，也有共同点。下面就它们的细节进行分析。

     为实现resize的语义，resize接口做了两个保证：

            一是保证区间[0, new_size)范围内数据有效，如果下标index在此区间内，vector[indext]是合法的。

             二是保证区间[0, new_size)范围以外数据无效，如果下标index在区间外，vector[indext]是非法的。

     reserve只是保证vector的空间大小(capacity)最少达到它的参数所指定的大小n。在区间[0, n)范围内，如果下标是index，vector[index]这种访问有可能是合法的，也有可能是非法的，视具体情况而定。

     resize和reserve接口的共同点是它们都保证了vector的空间大小(capacity)最少达到它的参数所指定的大小。

因两接口的源代码相当精简，以至于可以在这里贴上它们：

void resize(size_type new_size) { resize(new_size, T()); }

void resize(size_type new_size, const T& x) {

    if (new_size < oldsize) 

      erase(oldbegin + new_size, oldend); // erase区间范围以外的数据，确保区间以外的数据无效

    else

      insert(oldend, new_size - oldsize, x); // 填补区间范围内空缺的数据，确保区间内的数据有效

示例:

#include<iostream>

#include<vector>

using namespace std;

void main()

{

    vector<int>v1;

for(int i=1;i<=3;i++)

{

    v1.push_back(i);

}

v1.resize(5,8);//多出的两个空间都初始化为8，

for(i=0;i<v1.size();i++)//resize与reserver并不会删除原先的元素以释放空间

{

     cout<<v1[i]<<" ";

}

cout<<endl;

v1.reserve(7);// 新元素还没有构造,

    for(i=0;i<7;i++)

{

     cout<<v1[i]<<" ";//当i>4,此时不能用[]访问元素 

}

cout<<endl;

cout<<v1.size()<<endl;

cout<<v1.capacity()<<endl;

}

输出结果为:

1 2 3 8 8

1 2 3 8 8 -842150451 -842150451

5

7

size 函数 

语法: 

size_type size();

size() 函数返回当前vector所容纳元素的数目 

swap 函数 

语法: 

void swap( vector &from );

swap()函数交换当前vector与vector from的元素

示例:

     vector<int>v1,v2;

     for(int i=1;i<=3;i++)

     {

v1.push_back(i);

        v2.push_back(i);

     }

v2.push_back(4);

     v2.push_back(5);

     v1.swap(v2);

     for(int j=0;j<v1.size();j++)

     {

cout<<v1[j]<<" ";

     }

     cout<<endl;

     for(int k=0;k<v2.size();k++)

     {

cout<<v2[k]<<" ";

     }

     cout<<endl;

输出结果为:

1 2 3 4 5

1 2 3
            

