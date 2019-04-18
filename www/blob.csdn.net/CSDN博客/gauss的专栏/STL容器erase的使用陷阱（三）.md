# STL容器erase的使用陷阱（三） - gauss的专栏 - CSDN博客
2013年01月08日 13:16:30[gauss](https://me.csdn.net/mathlmx)阅读数：246
个人分类：[C/C++/内存管理](https://blog.csdn.net/mathlmx/article/category/605906)

原文：[http://www.fengwanfeng.com/article.asp?id=42](http://www.fengwanfeng.com/article.asp?id=42)
三、List容器中删除元素的方法
对于list容器，由于list本身有remove和remove_if的成员函数，所以最好优先考虑list自己的算法，对于remove函数，比较简单，不再讨论，对于remove_if函数，本人发现在vc6.0中有重大问题。我试了多种函数对象，总是编译不过，通过查看源代码，才发现VC6.0中对remove_if()函数作了简化，只提供了一种比较函数，它只能删除不等于某值的元素，VC6.0种remove_if()函数的源码如下：
typedef binder2nd<not_equal_to<_Ty> > _Pr1;
       void remove_if(_Pr1 _Pr)
              {iterator _L = end();
              for (iterator _F = begin(); _F != _L; )
                     if (_Pr(*_F))
                            erase(_F++);
                     else
                            ++_F; }
从源码中可以看出，remove_if中_Pr1函数对象被固定为binder2nd<not_equal_to<_Ty> >一种格式。而在VC7.0中已经修改了这个bug，源码如下：
template<class _Pr1>
              void remove_if(_Pr1 _Pred)
              {     // erase each element satisfying _Pr1
              iterator _Last = end();
              for (iterator _First = begin(); _First != _Last; )
                     if (_Pred(*_First))
                            erase(_First++);
                     else
                            ++_First;
              }
在VC7.0中remove_if()是成员模板函数，可以用任何判断条件的函数对象。
例如：
例 8：
#include <iostream>
#include <string>
#include <list>
#include <algorithm>
using namespace std;
class CTest{
public:
       CTest( int i ) : m_iPrice ( i ) {     }
       int operator == ( const CTest& right ) const{
              return ( m_iPrice == right.m_iPrice ) ? 1 : 0;
       }
       int operator != ( const CTest& right ) const{
              return ( m_iPrice != right.m_iPrice ) ? 1 : 0;
       }
       int operator < ( const CTest& right ) const {
              return ( m_iPrice < right.m_iPrice ) ? 1 : 0;
       }
private:
       int m_iPrice;
       friend class CTestFunc;
};
class CTestFunc {            //       函数对象
public:
       int m_value;
       CTestFunc( int i ) : m_value( i ) {}
       bool operator () ( const CTest& clFirst ) {
              return ( clFirst.m_iPrice == m_value ) ? true : false;     }
};
void main() {
       list< CTest > listTest;
       for ( int i = 0; i < 5; i++ ) {
              CTest clTest( i );
              listTest.push_back( clTest );
       }
       cout << "remove before : " << listTest.size() << endl;
//     删除所有为2的元素
       listTest.remove_if( CTestFunc( 2 )  ); //       这条语句在vc6.0中不能编译通过,VC7.0中可以
       cout << "remove after : 2, size =  " << listTest.size() << endl;
       //     删除所以不等于2的元素，VC6.0中只能以这种方式调用remove_if()函数
       listTest.remove_if(  bind2nd( not_equal_to<CTest>(), 2 )    );
       cout << "remove after not equal to 2, size =  " << listTest.size() << endl;
       //     因为CTest类提供了==、< 成员函数，所以也可以用remove函数
       listTest.remove( 2 ); //       删除所有为2的元素
       cout << "remove after : 2, size =  " << listTest.size() << endl;
}
不知道在VC6.0中能否突破只能函数对象被固定为binder2nd<not_equal_to<_Ty> >一种格式的限制？欢迎诸位大虾不吝赐教。不过采用通用算法remove_if只是多了几次对象的赋值的负担，如果对象不是太大，用通用算法的性能也是可以接受的。
另外，这些天使用了VC7.0后，感觉非常棒，不仅几乎符合Standard C++规范，错误提示也更清晰，而编译速度和编译后的文件大小大大减小，如我原来的一个大量使用了模板的程序，用VC6.0编译后Release版的可执行文件大小为1.2M，用VC7.0编译后只有420K，我想可能VC7.0在代码优化和模板代码的膨胀等方面有了极大的改善；在STL的实现上也有了极大的改进，把原来的一些效率不好的地方都改进了，处理策略基本与SGI STL一致。
4．STL容器中元素为指针情况下的删除方法
对于容器中的元素为指针的删除方法。如果容器中的元素为指针则不能用上面介绍的用通过算法或成员函数的方法删除元素，因为那样做会导致内存泄露，容器中的元素为指针指向的内存没有释放，在这种情况下有以下方法解决：
1．  尽可能不用指针作为容器的元素。
2．  如果是因为要减少对象拷贝和赋值方面的负担，而要在容器中存放指针的话，可以考虑用boost库中的智能指针shared_ptr包装指针，达到容器中引用的语意。
3．  如果你不希望因为使用boost::shared_ptr增加引用计数的负担，认为引入智能指针不好理解，那么你用指针作为容器的元素要千万小心，这时你要自己管理内存。
例如：    
例 9：用boost库中的智能指针shared_ptr包装指针的例子：
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <boost/smart_ptr.hpp> // 要包含BOOST类库中智能指针的头文件
using namespace std;
class CTest {
public:
       CTest( const string& str, int iPrice ) : m_strName( str ), m_iPrice( iPrice ) { }
      void vPrint() { cout << "name=" << m_strName << " price = " << m_iPrice << endl;
       }
private:
       string m_strName;
       int   m_iPrice;
       friend class CStrFunc;
       friend class CIntFunc;
};
//     函数对象，根据string比较
class CStrFunc {
       string m_str;
public:
       CStrFunc( const string& str ) : m_str( str ) {
       }
       //     此处要改为boost::shared_ptr<CTest>&，因为vector容器中的元素为
//       boost::shared_ptr<CTest>
       bool operator() ( const boost::shared_ptr<CTest>& left ) {
              return ( m_str == (*left).m_strName ) ? true : false;
       }
};
//     函数对象，根据int比较
class CIntFunc {
       int m_iPrice;
public:
       CIntFunc( int iPrice ) : m_iPrice( iPrice ) {
       }
//     此处要改为boost::shared_ptr<CTest>&，因为vector容器中的元素为
//       boost::shared_ptr<CTest>
       bool operator() ( const boost::shared_ptr<CTest>& left ) {
              return ( m_iPrice == (*left).m_iPrice ) ? true : false;
       }
};
void main( ) {
       vector< boost::shared_ptr<CTest>  > vectTest;
       int i;
       for (  i = 0; i < 5 ; i++ ) {
              stringstream stream;
              stream << i;
              string str = stream.str();
              boost::shared_ptr<CTest>  ptrShare( new CTest( str, i ) );
              vectTest.push_back( ptrShare );
       }
      for (  i = 0 ; i < vectTest.size(); i++ )  {
              ( *vectTest[ i ] ).vPrint();
       }
       //     删除所有m_strName = "3"的元素
       vectTest.erase( remove_if( vectTest.begin(), vectTest.end(), CStrFunc( "3" ) ),
              vectTest.end() );
       cout << "delete 3 after : " << endl;
      for (  i = 0 ; i < vectTest.size(); i++ )  {
              ( *vectTest[ i ] ).vPrint();
       }
       //     删除所有m_iPrice = 2的元素
       vectTest.erase( remove_if( vectTest.begin(), vectTest.end(), CIntFunc( 2 ) ),
              vectTest.end() );
       cout << "delete 2 after : " << endl;
      for (  i = 0 ; i < vectTest.size(); i++ )  {
              ( *vectTest[ i ] ).vPrint();
       }
}
以上代码不会导致内存泄露。
例 10：自己编程删除容器中元素为指针的例子：
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
class CTest {
public:
       CTest( const string& str, int iPrice ) : m_strName( str ), m_iPrice( iPrice ) { }
      void vPrint() { cout << "name=" << m_strName << " price = " << m_iPrice << endl;
       }
private:
       string m_strName;
       int   m_iPrice;
       //     声明友员函数，因为vDeleteVector函数要访问CTest的private成员变量
       friend void vDeleteVector( vector< CTest*  >& vectTest, const string& str );
       friend void vDeleteVector( vector< CTest*  >& vectTest, int iPrice );
};
//     根据CTest类中m_strName比较
void vDeleteVector( vector< CTest*  >& vectTest, const string& str ) {
       vector< CTest* >::iterator itVect = vectTest.begin();
       for ( ; itVect != vectTest.end();; ) {
              if ( (*itVect)->m_strName == str ) {
                     //       删除vector容器中指针元素指向的内容，防止内存泄露
                     delete *itVect;
                     itVect = vectTest.erase( itVect );
              }
              else {
                     ++itVect;
              }
       }
}
//     根据CTest类中m_iPrice比较
void vDeleteVector( vector< CTest*  >& vectTest, int iPrice ) {
       vector< CTest* >::iterator itVect = vectTest.begin();
       for ( ; itVect != vectTest.end(); ) {
              if ( (*itVect)->m_iPrice == iPrice ) {
                     //       删除vector容器中指针元素指向的内容，防止内存泄露
                     delete *itVect;
                     itVect = vectTest.erase( itVect );
              }
              else {
                     ++itVect;
              }
       }
}
void main( ) {
       vector< CTest*  > vectTest;
       int i;
       for (  i = 0; i < 5 ; i++ ) {
              stringstream stream;
              stream << i;
              string str = stream.str();
              CTest* pclTest =  new CTest( str, i ) ;
              vectTest.push_back( pclTest );
       }
      for (  i = 0 ; i < vectTest.size(); i++ )  {
              vectTest[ i ]->vPrint();
       }
       //     删除所有m_strName = "5"的元素
       vDeleteVector( vectTest, "3" );
       cout << "delete 3 after : " << endl;
      for (  i = 0 ; i < vectTest.size(); i++ )  {
              vectTest[ i ]->vPrint();
       }
       //     删除所有m_iPrice = 2的元素
       vDeleteVector( vectTest, 2 );
       cout << "delete 2 after : " << endl;
      for (  i = 0 ; i < vectTest.size(); i++ )  {
              vectTest[ i ]->vPrint();
       }
}
原则：
1．  尽可能用通用算法。相信STL的算法要比自己的实现高效、优雅、安全。
2．  优先用容器自身的成员函数。 见《Effective STL》中 Item 44: Prefer member functions to algorithms with the same names
3．  尽可能熟悉函数对象。
4．  多看STL的源码，了解其实作。
5．   不成熟的优化是一切恶果的根源。编写代码，安全第一。
综上，在STL中删除容器中部分元素时要特别小心，不过通过使用通用算法或容器本身的删除函数，能大大减小重复代码和程序出错的机会，能够使代码得到优化，生成高效的代码。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/waterathena/archive/2008/09/17/2943715.aspx](http://blog.csdn.net/waterathena/archive/2008/09/17/2943715.aspx)
