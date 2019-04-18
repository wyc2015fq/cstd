# C++11make_pair问题 - Likes的博客 - CSDN博客
2018年10月26日 14:17:38[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：340
[https://stackoverflow.com/questions/32443181/stdmake-pair-with-c-11](https://stackoverflow.com/questions/32443181/stdmake-pair-with-c-11)
在C++11中使用make_pair一定不要显式给出类型参数，不然会报错
```cpp
#include <utility>
using namespace std;
int main()
{
  int n=0,m=0;
  auto x=make_pair<int,int>(m,n);//wrong
  auto x=make_pair(m,n);//right
 
  return 0;
}
```
使用C++11编译将会出错：
$ g++ -std=c++11 testpair.cc 
testpair.cc: In function ‘int main()’:
testpair.cc:6:32: error: no matching function for call to ‘make_pair(int&, int&)’
   auto x=make_pair<int,int>(m,n);
                                ^
testpair.cc:6:32: note: candidate is:
In file included from /usr/local/include/c++/4.8.5/utility:70:0,
                 from testpair.cc:1:
/usr/local/include/c++/4.8.5/bits/stl_pair.h:276:5: note: template<class _T1, class _T2> constexpr std::pair<typename std::__decay_and_strip<_Tp>::__type, typename std::__decay_and_strip<_T2>::__type> std::make_pair(_T1&&, _T2&&)
     make_pair(_T1&& __x, _T2&& __y)
     ^
在C++11之前的版本显式给出类型参数是不会有问题的。
其实make_pair作为模板函数，本来就应该尽可能使用自动的参数推断，而不是画蛇添足地显式给出类型参数。
