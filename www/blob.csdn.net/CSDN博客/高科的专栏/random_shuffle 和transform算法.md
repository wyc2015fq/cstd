
# random_shuffle 和transform算法 - 高科的专栏 - CSDN博客

2012年10月23日 16:45:48[高科](https://me.csdn.net/pbymw8iwm)阅读数：5854


1）STL中的函数random_shuffle()用来对一个元素序列进行重新排序（随机的），函数原型如下：

# std::
# random_shuffle
template <class RandomAccessIterator>
  void random_shuffle ( RandomAccessIterator first, RandomAccessIterator last );
    template <class RandomAccessIterator, class RandomNumberGenerator>
  void random_shuffle ( RandomAccessIterator first, RandomAccessIterator last,
                        RandomNumberGenerator& rand );`template``<``class``RandomAccessIterator,``class``RandomNumberGenerator>``void``random_shuffle ( RandomAccessIterator first, RandomAccessIterator last,
                        RandomNumberGenerator& rand )
{
  iterator_traits<RandomAccessIterator>::difference_type i, n;
  n = (last-first);``for``(i=n-1; i>0; --i) swap (first[i],first[rand(i+1)]);
}`
### Example
|1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38|// random_shuffle example|\#include <iostream>|\#include <algorithm>|\#include <functional>|\#include <vector>|\#include <ctime>|\#include <cstdlib>|using|namespace|std;|// random generator function:|ptrdiff_t myrandom (ptrdiff_t i) {|return|rand()%i;}|// pointer object to it:|ptrdiff_t (*p_myrandom)(ptrdiff_t) = myrandom;|int|main () {
  srand (|unsigned|( time (NULL) ) );
  vector<|int|> myvector;
  vector<|int|>::iterator it;|// set some values:|for|(|int|i=1; i<10; ++i) myvector.push_back(i);|// 1 2 3 4 5 6 7 8 9|// using built-in random generator:|random_shuffle ( myvector.begin(), myvector.end() );|// using myrandom:|random_shuffle ( myvector.begin(), myvector.end(), p_myrandom);|// print out content:|cout <<|"myvector contains:"|;|for|(it=myvector.begin(); it!=myvector.end(); ++it)
    cout <<|" "|<< *it;
  cout << endl;|return|0;
}|

A possible output:
|myvector contains: 3 4 1 6 8 9 2 7 5|


random_shuffle()是一个完全通用的算法，适用于内置数据类型和用户自定义类型。同时，由于STL算法不仅适用于容器，也适用于序列，因此，random_shuffle()算法可用于内置数组。
实例代码如下：
\#include <iostream>
\#include <vector>
\#include <algorithm>
\#include <string>
int main()
{
//用于内置数据类型
std::vector<int> vi;
for(int i=0; i<100; i++)
{
vi.push_back(i);
}
std::random_shuffle(vi.begin(), vi.end());
std::vector<int>::iterator it;
for(it=vi.begin(); it!=vi.end(); it++)
{
std::cout<<*it<<std::endl;
}
//用于用户自定义类型
std::vector<std::string> vs;
vs.push_back(std::string("Sunday"));
vs.push_back(std::string("Monday"));
vs.push_back(std::string("Tuesday"));
vs.push_back(std::string("Wednesday"));
vs.push_back(std::string("Thursday"));
vs.push_back(std::string("Friday"));
vs.push_back(std::string("Saturday"));
std::random_shuffle(vs.begin(), vs.end());
for(int i=0; i<7; i++)
{
std::cout<<vs[i]<<std::endl;
}
//用于数组
char arr[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
std::random_shuffle(arr, arr+6);
for(int i=0; i<6; i++)
{
std::cout<<arr[i]<<" ";
}
std::cout<<std::endl;
system("pause");
return 0;
}
2）STL中的函数transform()用来遍历一个容器里面指定范围的元素，并对这些元素执行指定的操作，函数原型如下：
template<class InputIterator, class OutputIterator, class UnaryFunction>
OutputIterator transform(
InputIterator _First1, //元素起始位置的输入迭代器
InputIterator _Last1, //元素结束位置的输入迭代器
OutputIterator _Result, //执行指定操作的元素的起始位置的输出迭代器
UnaryFunction _Func //执行的操作（函数）
);
template<class InputIterator1, class InputIterator2, class OutputIterator,
class BinaryFunction>
OutputIterator transform(
InputIterator1 _First1, //第一个操作范围的元素起始位置的输入迭代器
InputIterator1 _Last1, //第一个操作范围的元素结束位置的输入迭代器
InputIterator2 _First2, //第二个操作范围的元素起始位置的输入迭代器
OutputIterator _Result, //最终范围的元素的起始位置的输出迭代器
BinaryFunction _Func //执行的操作（函数）
);
上面第一个版本的算法对区间[_First1, _Last1]中的每个元素应用函数_Func，并将每次_Func返回的结果存储到_Result中；
第二个版本的算法以类似的方式运行，但它期望获得两个序列并逐次调用一个处理成对元素的二元函数
实例代码如下：
\#include <vector>
\#include <algorithm>
\#include <functional>
\#include <iostream>
// The function object multiplies an element by a Factor
template <typename T>
class MultiValue
{
private:
T Factor;   //The value to multiply by
public:
//Constructor initializes the value to multiply by
MultiValue(const T& _val) : Factor(_val)
{
}
//The function call for the element to be multiplied
T operator()(T& elem) const
{
return elem*Factor;
}
};
int main()
{
using namespace std;
vector<int> v1, v2(7), v3(7);
vector<int>::iterator it1, it2, it3;
//Constructing vector v1;
for(int i=-4; i<=2; i++)
{
v1.push_back(i);
}
cout<<"Original vector v1=(";
for(it1=v1.begin(); it1!= v1.end(); it1++)
{
cout<<*it1<<" ";
}
cout<<")."<<endl;
//Modifying the vector v1 in place
transform(v1.begin(), v1.end(), v1.begin(), MultiValue<int>(2));
cout<<"The elements of the vector v1 multiplied by 2 in place gives:"
<<"/n v1mod=(";
for(it1=v1.begin(); it1!=v1.end(); it1++)
{
cout<<*it1<<" ";
}
cout<<")."<<endl;
//using transform to multiply each element by a factor of 5
transform(v1.begin(), v1.end(), v2.begin(), MultiValue<int>(5));
cout<<"Multiplying the elements of the vector v1mod/n"
<<"by the factor 5 & copying to v2 gives:/n v2=(";
for(it2=v2.begin(); it2!=v2.end(); it2++)
{
cout<<*it2<<" ";
}
cout<<")."<<endl;
//The second version of transform used to multiply the
//elements of the vectors v1mod & v2 pairwise
transform(v1.begin(), v1.end(), v2.begin(), v3.begin(),
multiplies<int>());
cout<<"Multiplying elements of the vectors v1mod and v2 pairwise "
<<"gives:/n v3=( ";
for(it3=v3.begin(); it3!=v3.end(); it3++)
{
cout<<*it3<<" ";
}
cout<<")."<<endl;
system("pause");
return 0;
}
程序运行后输出如下：
Original vector  v1 = ( -4 -3 -2 -1 0 1 2 ).
The elements of the vector v1 multiplied by 2 in place gives:
v1mod = ( -8 -6 -4 -2 0 2 4 ).
Multiplying the elements of the vector v1mod
by the factor 5 & copying to v2 gives:
v2 = ( -40 -30 -20 -10 0 10 20 ).
Multiplying elements of the vectors v1mod and v2 pairwise gives:
v3 = ( 320 180 80 20 0 20 80 ).


