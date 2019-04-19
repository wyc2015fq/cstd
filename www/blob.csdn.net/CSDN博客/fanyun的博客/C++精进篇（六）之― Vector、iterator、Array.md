# C++精进篇（六）之― Vector、iterator、Array - fanyun的博客 - CSDN博客
2016年03月26日 09:16:14[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2009
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
 Vector用法：
(1)vector< 类型 > 标识符 ;
(2)vector< 类型 > 标识符(最大容量) ；
(3)vector< 类型 > 标识符（最大容量，初始所有值）；
(4) int i[4] = {12,3,4,5};
    vector< 类型 > vi(i , i+2); //得到i索引值为3以后的值；  
(5)vector< vector >   //vi 定义2维的容器；记得一定要有空格，不然会报错
  vector< int > line   
  // 在使用的时候一定要首先将vi个行进行初始化;   
  for(int i = 0 ; i < 10 ; i ++)  
  {  
    vector.push_back(line);  
  }  
  /// 个人认为使用vector定义二维数组很好，因为是长度可以不预先确定。很好。 
(6)C++ Vector排序
  vector< int > vi ;   
  vi.push_back(1);  
  vi.push_back(3);  
  vi.push_back(0);  
  sort(vi.begin() , vi.end()); /// /小到大  
  reverse(vi.begin(),vi.end()) /// 从大道小 
(7)顺序访问
  vector < int > vi ;   
  for( int i = 0 ; i < 10 ; i ++)  
  {  
    vector.push_back(i);  
  } 
  for(int i = 0 ; i < 10 ; i ++) /// 第一种调用方法  
  {  
    cout <<<"="" "="";=""   <="" span=""style="word-wrap: break-word;">
  } 
   for(vector::iterator it = vi.begin() ; 
  it !=vi.end() ; it++) ///第二种调用方法  
  {  
    cout << *it << " " ;  
  }
(8)查找
vector < int > vi ;   
for( int i = 0 ; i < 10 ; i ++)  
{  
  vector.push_back(i);  
} 
vector < int >::interator it = find(vi.begin() , vi.end,3) ;  
cout << *it << endl ; ///返回容器内找到值的位置。 
(9)使用数组对C++ Vector进行初始化
int i[10] ={1,2,3,4,5,6,7,78,8} ;  
///第一种   
vector vi(i+1,i+3); ///从第2个元素到第三个元素  
for(vector ::interator it = vi.begin() ; it != vi.end() ; it++)  
{  
  cout << *it <<" " ;   
} 
(10) 结构体类型
struct temp  
{  
public :  
  string str ;   
public :  
  int id ;  
}tmp
int main()  
{  
  vector t ;   
  temp w1 ;   
  w1.str = "Hellowor" ;  
  w1.id = 1 ;   
  t.push_back(t1);  
  cout << w1.str << "," <<<=""span="" style="word-wrap: break-word;">
  return 0 ;   
} 
iterator迭代器用法：
1. 迭代器(iterator)是一中检查容器内元素并遍历元素的数据类型。
(1) 每种容器类型都定义了自己的迭代器类型，如vector:
vector<int>::iterator iter;这条语句定义了一个名为iter的变量，它的数据类型是由vector<int>定义的iterator类型。
(2) 使用迭代器读取vector中的每一个元素：
vector<int> ivec(10,1);
for(vector<int>::iterator iter=ivec.begin();iter!=ivec.end();++iter)
{
*iter=2; //使用 * 访问迭代器所指向的元素
}
const_iterator:
只能读取容器中的元素，而不能修改。
for(vector<int>::const_iteratorciter=ivec.begin();citer!=ivec.end();citer++)
{
cout<<*citer;
//*citer=3; error
}
vector<int>::const_iterator 和 constvector<int>::iterator的区别
const vector<int>::iterator newiter=ivec.begin();
*newiter=11; //可以修改指向容器的元素
//newiter++; //迭代器本身不能被修改 
(3) iterator的算术操作：
iterator除了进行++,- -操作，可以将iter+n,iter-n赋给一个新的iteraor对象。还可以使用一个iterator减去另外一个iterator.
const vector<int>::iterator newiter=ivec.begin();
vector<int>::iterator newiter2=ivec.end();
cout<<"\n"<<newiter2-newiter; 
一個很典型使用vector的STL程式:
#include <vector>
#include <iostream>
 using namespace std;
 int main() {
 vector<int> ivec;
 ivec.push_back(1);
 ivec.push_back(2);
 ivec.push_back(3);
 ivec.push_back(4);
 for(vector<int>::iterator iter =ivec.begin();1. iter != ivec.end(); ++iter)
 cout << *iter << endl;
 }
2. Iterator（迭代器）模式
一、概述
    Iterator（迭代器）模式又称Cursor（游标）模式，用于提供一种方法顺序访问一个聚合对象中各个元素, 而又不需暴露该对象的内部表示。或者这样说可能更容易理解：Iterator模式是运用于聚合对象的一种模式，通过运用该模式，使得我们可以在不知道对象内部表示的情况下，按照一定顺序（由iterator提供的方法）访问聚合对象中的各个元素。
    由于Iterator模式的以上特性：与聚合对象耦合，在一定程度上限制了它的广泛运用，一般仅用于底层聚合支持类，如STL的list、vector、stack等容器类及ostream_iterator等扩展iterator。
    根据STL中的分类，iterator包括：
Input Iterator：只能单步向前迭代元素，不允许修改由该类迭代器引用的元素。
Output Iterator：该类迭代器和Input Iterator极其相似，也只能单步向前迭代元素，不同的是该类迭代器对元素只有写的权力。
Forward Iterator：该类迭代器可以在一个正确的区间中进行读写操作，它拥有Input Iterator的所有特性，和Output Iterator的部分特性，以及单步向前迭代元素的能力。
Bidirectional Iterator：该类迭代器是在Forward Iterator的基础上提供了单步向后迭代元素的能力。
Random Access Iterator：该类迭代器能完成上面所有迭代器的工作，它自己独有的特性就是可以像指针那样进行算术计算，而不是仅仅只有单步向前或向后迭代。
    这五类迭代器的从属关系，如下图所示，其中箭头A→B表示，A是B的强化类型，这也说明了如果一个算法要求B，那么A也可以应用于其中。
input output
     \ /
forward
       |
bidirectional
       |
random access
 图1、五种迭代器之间的关系
    vector 和deque提供的是RandomAccessIterator，list提供的是BidirectionalIterator，set和map提供的 iterators是 ForwardIterator，关于STL中iterator迭代器的操作如下：
说明：每种迭代器均可进行包括表中前一种迭代器可进行的操作。
迭代器操作                     说明
(1)所有迭代器
p++：后置自增迭代器                           
++p：前置自增迭代器
(2)输入迭代器
*p：复引用迭代器，作为右值
p=p1： 将一个迭代器赋给另一个迭代器
p==p1：比较迭代器的相等性
p!=p1：比较迭代器的不等性
(3)输出迭代器
*p：复引用迭代器，作为左值
p=p1：将一个迭代器赋给另一个迭代器
(4)正向迭代器
提供输入输出迭代器的所有功能
(5)双向迭代器
--p： 前置自减迭代器                             
p--：后置自减迭代器                               
(6)随机迭代器
p+=i：将迭代器递增i位                             
p-=i：将迭代器递减i位                             
p+i：在p位加i位后的迭代器                               
p-i：在p位减i位后的迭代器                               
p[i]： 返回p位元素偏离i位的元素引用                             
p<p1：如果迭代器p的位置在p1前，返回true，否则返回false                            
p<=p1：p的位置在p1的前面或同一位置时返回true，否则返回false                          
p>p1：如果迭代器p的位置在p1后，返回true，否则返回false                            
p>=p1：p的位置在p1的后面或同一位置时返回true，否则返回false                          
    只有顺序容器和关联容器支持迭代器遍历，各容器支持的迭代器的类别如下：
容器                 支持的迭代器类别            容器               支持的迭代器类别            容器                 支持的迭代器类别
vector              随机访问                      deque              随机访问                       list                   双向
set                  双向                            multiset            双向                           map                 双向
multimap          双向                             stack                不支持                        queue              不支持
priority_queue   不支持
二、结构
Iterator模式的结构如下图所示：
![](https://img-blog.csdn.net/20160328180323536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                                                             图2、Iterator模式类图示意
三、应用
    Iterator模式有三个重要的作用：
1）它支持以不同的方式遍历一个聚合.复杂的聚合可用多种方式进行遍历，如二叉树的遍历，可以采用前序、中序或后序遍历。迭代器模式使得改变遍历算法变得很容易: 仅需用一个不同的迭代器的实例代替原先的实例即可，你也可以自己定义迭代器的子类以支持新的遍历，或者可以在遍历中增加一些逻辑，如有条件的遍历等。
2）迭代器简化了聚合的接口. 有了迭代器的遍历接口，聚合本身就不再需要类似的遍历接口了，这样就简化了聚合的接口。
3）在同一个聚合上可以有多个遍历每个迭代器保持它自己的遍历状态，因此你可以同时进行多个遍历。
4）此外，Iterator模式可以为遍历不同的聚合结构（需拥有相同的基类）提供一个统一的接口，即支持多态迭代。
    简单说来，迭代器模式也是Delegate原则的一个应用，它将对集合进行遍历的功能封装成独立的Iterator，不但简化了集合的接口，也使得修改、增加遍历方式变得简单。从这一点讲，该模式与Bridge模式、Strategy模式有一定的相似性，但Iterator模式所讨论的问题与集合密切相关，造成在Iterator在实现上具有一定的特殊性，具体将在示例部分进行讨论。
四、优缺点
     正如前面所说，与集合密切相关，限制了 Iterator模式的广泛使用，就个人而言，我不大认同将Iterator作为模式提出的观点，但它又确实符合模式“经常出现的特定问题的解决方案”的特质，以至于我又不得不承认它是个模式。在一般的底层集合支持类中，我们往往不愿“避轻就重”将集合设计成集合 + Iterator 的形式，而是将遍历的功能直接交由集合完成，以免犯了“过度设计”的诟病，但是，如果我们的集合类确实需要支持多种遍历方式（仅此一点仍不一定需要考虑 Iterator模式，直接交由集合完成往往更方便），或者，为了与系统提供或使用的其它机制，如STL算法，保持一致时，Iterator模式才值得考虑。
五、举例
    可以考虑使用两种方式来实现Iterator模式：内嵌类或者友元类。通常迭代类需访问集合类中的内部数据结构，为此，可在集合类中设置迭代类为friend class，但这不利于添加新的迭代类，因为需要修改集合类，添加friend class语句。也可以在抽象迭代类中定义protected型的存取集合类内部数据的函数，这样迭代子类就可以访问集合类数据了，这种方式比较容易添加新的迭代方式，但这种方式也存在明显的缺点：这些函数只能用于特定聚合类，并且，不可避免造成代码更加复杂。
    STL的list::iterator、deque::iterator、rbtree::iterator等采用的都是外部Iterator类的形式，虽然STL的集合类的iterator分散在各个集合类中，但由于各Iterator类具有相同的基类，保持了相同的对外的接口（包括一些traits及tags等，感兴趣者请认真阅读参考1、2），从而使得它们看起来仍然像一个整体，同时也使得应用algorithm成为可能。我们如果要扩展STL的iterator，也需要注意这一点，否则，我们扩展的iterator将可能无法应用于各algorithm。
Array用法：
    在c++中创建数组时,大量使用new和delete,很繁琐，但使用<array>很方便,用起来和STL<vector>一样的，而且执行效率比<vector>高,差不多和int myarray[5]效率一样：
但是要注意的是：
（1）     array 定义的时候必须定义数组的元素个数;而vector不需要；且只能包含整型字面值常量，枚举常量或者用常量表达式初始化的整型const对象，
非const变量以及需要到运行阶段才知道其值的const变量都不能用来定义数组的维度.
（2）     array 定义后的空间是固定的了，不能改变；而vector 要灵活得多，可再加或减.
（3）     vector有一系列的函数操作，非常方便使用.和vector不同，数组不提供push——back或者其他的操作在数组中添加新元素，数组一经定义就不允许添加新元素；
若需要则要充许分配新的内存空间，再将员数组的元素赋值到新的内存空间。
（4）     数组和vector不同，一个数组不能用另一个数组初始化，也不能将一个数组赋值给另一个数组；
示例：
#include <iostream>  
#include <array>     
using namespace std;  
  int main ()  
{  
 //--这是1维数组  
    array<int,5> myarray={1,2,3,4,5};  
  cout <<"myarray="<<endl;  
  for (size_t n=0; n<myarray.size(); n++){  
      cout << myarray[n] <<'\t';  
  }  
  cout << endl;  
   //当然也可以使用  
  cout <<"myarray="<<endl;  
  for (size_t n=0; n<myarray.size(); n++){  
      cout << myarray.at(n) << '\t';  
  }  
   cout << endl;  
//--这是2维数组,共3行2列  
  array<array<int,2>,3 > myarray2D={1,2,3,4,5,6};  
   cout <<"myarray2D="<<endl;  
  for (size_t m=0; m<myarray2D.size(); m++){  
    for (size_t n=0; n<myarray2D[m].size(); n++){      
        cout << myarray2D[m][n] <<'\t';  
    }   
   cout << endl;   
  }  
  cout << endl;  
  return 0;  
}  
C++智能指针处理Array对象示例：
//C++11的<memory>中有一整套智能指针，
//完全可以避免写手动的delete代码，
//但是它默认使用delete删除对象，
//如果是数组对象，需要指定自定义的删除方法，支持delete[]
std::shared_ptr<int> p(new int[10],
    [](int* p){
        delete[] p;
    });
//或者使用helper
std::shared_ptr<int> p(new int[10],std::default_delete<int[]>()); 
unique_ptr跟shared_ptr不一样，它直接支持持有数组对象
std::unique_ptr<int[]> p(new int[10]);//ok
std::shared_ptr<int[]> p(new int[10]);//error, does not compile
std::unique_ptr<int, void(*)(int*)> p(new int[10],
    [](int* p){
        delete[] p;
    });
