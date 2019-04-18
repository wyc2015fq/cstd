# C++模板类 容器之map 及 对象副本 深/浅拷贝 等问题 - ljx0305的专栏 - CSDN博客
2013年01月29日 10:44:03[ljx0305](https://me.csdn.net/ljx0305)阅读数：658
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
                
一般地当我们只想知道一个值是否存在时,set 最有用处;希望存储也可能修改一个相关的值时,map 最为有用.
**   map**提供一个键值对容器，在map(也叫关联数组)中我们提供一个键/值对,键用来索引,而值用作被存储和检索的数据.
   在使用map和set时两个最主要的动作是向里面放入元素以及查询元素是否存在.
**首先要包含头文件**
#include <map>
**定义并生成map**
为定义map对象我们至少要指明键和值的类型例如
map<string, int>   word_count;//定义名为word_count的map，键值为string类型，value为int类型
class employee;
map<int, employee*>    personnel;//定义名为personnel的map，键值为int，value为employee指针
或者使用类型定义
typedef     map <string,   int>   MY_MAP;
MY_MAP  
word_count；
**定义了map 以后下一步工作就是加入键/值元素对**
方法一：
word_count[ string("Anna") ] = 1;
1一个未命名的临时string 对象被构造并传递给与map 类相关联的下标操作符，这个对象用Anna
 初始化，
2 在word_count 中查找Anna 项，没有找到
3 一个新的键/值对被插入到word_count 中。当然键是一个string对象持有“Anna”。但是值不是1
 而是0
4 插入完成接着值被赋为1
用下标操作符把map 初始化至一组元素集合，会使每个值都被初始化为缺省值。然后再被赋值为显式的值，如果元素是类对象而且它的缺省初始化和赋值的运算量都很大。就会影响程序的性能
方法二：
word_count.insert(map<string,int>::value_type( string("Anna"), 1 ));
其作用是创建一个pair对象，接着将其直接插入map
typedef  map<string,int>::value_type    valType;
word_count.insert( valType( string("Anna"), 1 ));
方法三：
my_Map.insert(pair <string,int> ( "c ",3));
方法四：
my_Map.insert(make_pair( "d ",4));
方法五：
为插入一定范围内的键/值元素，我们可以用另一个版本的insert()方法。它用一对iterator作为参数。
map< string, int > word_count;
// ... fill it up
map< string, int > word_count_two;
// 插入所有键/值对
word_count_two.insert(word_count.begin(),word_count.end());
**查找并获取map 中的元素。**
下标操作符给出了获取一个值的最简单方法例如
// map<string,int> word_count;
int count = word_count[ "wrinkles" ];
但是只有当map 中存在这样一个键的实例时该代码才会表现正常如果不存在这样的实例使用下标操作符会引起插入一个实例在本例中键/值对
string( "wrinkles" ), 0
被插入到word_count 中count 被初始化为0
有两个map 操作能够发现一个键元素是否存在而且在键元素不存在时也不会引起插入实例
1   count(keyValue)
count()返回map 中keyValue 出现的次数当然对于map
而言返回值只能是0 或1 如果返回值非0 我们就可以安全地使用下标操作符例如
int count = 0;
if ( word_count.count( "wrinkles" ))
count = word_count[ "wrinkles" ];
2   find(keyValue)
如果实例存在则find()返回指向该实例的iterator 如果不存在则返回等于end()的iterator
 例如
int count = 0;
map<string,int>::iterator it = word_count.find( "wrinkles" );
if ( it != word_count.end() )
count = (*it).second;
指向map中元素的iterator指向一个pair对象（下文有对pair的介绍）其中first拥有键，second拥有值
**对map 进行迭代**
我们可以通过对由begin()和end()两个迭代器标记的所有元素进行迭代来做到这一点。
typedef map<string,int> tmap;
tmap::iterator iter = text_map->begin(),
iter_end = text_map->end();
while ( iter != iter_end )
{ cout << "word: " << (*iter).first << " ("<<(*iter).second<<")";
**对map****中的元素进行删除**
从map 中删除元素的erase()操作有三种变化形式。
为了删除一个独立的元素我们传递给erase()一个键值或iterator（删除的是当前迭代到的那个元素）, 为了删除一列元素我们传递给erase()一对lieator。
一个map 只能拥有一个键值的惟一实例,为了给同一个键提供多个项我们必须使用multimap。
**对map的其他操作**
my_Map.size()        返回元素数目
my_Map.empty()       判断是否为空
my_Map.clear()       清空所有元素
**PS:一些问题解惑：**
**Q：**以下两种定义方式，在进行map.clear（）的时候，内存释放上有啥不同？
     typedef map<CString,CFileAttribute> MAPStr2FileAttr;
     typedef map<CString,CFileAttribute *> MAPStr2FileAttr;
**A：**
   clear()相当于earse(m.begin(),   m.end());
若定义的map的存储对象是一个类对象：
拷贝是整个STL（Standard Template Library，标准模板库）工作的方式，所以容器中保存的是副本，而不是要添加的对象本身。对象原件在副本拷贝存放结束后便可以结束生命，而在使用clear（）的时候，对象副本会去走到析构函数，进行对象内部的内存释放。clear()后，对象拷贝被析构，剩下的只是raw   memory，即没有被初始化的内存，它们将被归还到stl的内存分配器alloc里的（记得吗，vector/list等所有容器都有一个alloc参数的，就是这东西），容器本身已经不再拥有这块内存了。内存归还了，只不过不是归还入系统堆而已。（除了vector不能（自动）释放内存，其它STL容器都会在每一个erase动作后释放一块内存。）
若定义的map存储对象是一个类对象的指针：
   这时一般不能采用clear()函数，因为clear()不会自动释放原来对象所占用的内存。这时可以使用erase()辅助delete语句进行元素删除和内存释放。
上面这句话我是这样理解的，因为存入的是指针，这个指针指向一块区域（new出来的，eg：class A， A *a = new A（）），但是毕竟map里面的value值是个指针，就是一个地址而已，因此在clear的时候只是把指针清除掉了，而指针指向的内容依旧存在。因此一般要在clear之前先释放掉这些个指针指向的空间。
   另外使用的是类对象指针时，还需要维护这个指针不受到破坏。
**小结：**
如果用容器存副本，则容器销毁的时候，副本也会自动被删除。
   如果用容器存指针，则容器销毁的时候，不会删除这些指针所指向的对象，因此必须先手工删除完毕之后，再销毁容器。
**Q:**对由key得到的value对象，没有办法改变其中的数值吗？？
例如下代码：
    //srcfileAttribute.nIndex初始为0
    CFileAttribute srcfileAttribute = m_mapKeyVsFile[“abc”];
    srcfileAttribute.nIndex++；
    但是再次
    CFileAttribute srcfileAttribute = m_mapKeyVsFile[“abc”];
    发现这个srcfileAttribute.nIndex还是0；并没有变成1，
    这是为什么呢？？
    难道说我不能这样直接改map里面的value值吗？必须删除重新insert一个？？
**A:**
srcfileAttribute = m_mapKeyVsFile[“abc”];此时srcfileAttrbute是通过map中值的拷贝构造函数构造的一个新的对象，这个副本的改变不影响map中的值，要改变map中的值可以直接m_mapKeyVsFile[“abc”].nIndex++;
如果用容器存副本，其存入、取出的过程是通过拷贝**构造函数** 和
**赋值操作符**来进行的。。。
**Q:**
我采用map<CString,CFileAttribute> 这种方式，既map的value值为CFileAttribute对象，
     但是在进行insert的时候程序却报错...
     例如以下代码：
     typedef map<CString,CFileAttribute> MAPStr2FileAttr;
     MAPStr2FileAttr m_mapKeyVsFile;
     szFilePath = "abc";
     CFileAttribute FileAttr;
     FileAttr.m_str = "nidfjasdkljfsdk";
     m_mapKeyVsFile.insert(pair <CString,CFileAttribute> (szFilePath, FileAttr));
     在执行insert的时候提示 _BLOCK_TYPE_IS_VALID 的错误，从网上查了下发现把CFileAttribute 的析构函数弄掉就OK了...
这是为什么呢？？不是存入map的是原对象的副本吗，怎么还会涉及到析构函数？？
**A：**
如果用容器存副本，其存入、取出的过程是**通过拷贝构造函数和赋值操作符**（详解查看operator操作符重载）来进行的。
  如果你没有显式地提供这两者，则使用缺省的拷贝构造函数和赋值操作符，其实现方式为：内存复制。例如：假若你没有提供CFileAttribute::operator=（重载赋值操作符），那么语句fileAttribute1 = fileAttribute2就相当于：memcpy(&fileAttribute1,
 &fileAttribute2, sizeof(CFileAttribute))。如果你的CFileAttribute仅包含简单变量，例如：
class CFileAttribute
{
  int i;
  double d;
};
那么memcpy没什么不妥。但假若你的CFileAttribute中包含（或者嵌套包含）指针，那么就可能有问题了，因为指针的复制并不等于内存块的复制，因此你必须显示地提供一个CFileAttribute::operator=，并且在其中把指针所对应的内存块也复制一遍，这才是真正的副本（此时这两个副本内的指针反而是不相等的，因为分别指向不同的内存块），其中任何一个副本的销毁（一般会在析构函数中把其指针所指向的内存块同时销毁）都不会影响到另一个副本的独立存在，既采用的是深拷贝（详解参看
**深拷贝与浅拷贝**）。
你的CFileAttribute::m_str显然是CString类型，而CString内部当然是一个指针，因此缺省的、memcpy方式的拷贝构造函数以及赋值操作符一定会出问题。你必须显式提供自己的拷贝构造函数以及赋值操作符。
PS：我在使用map模板类时碰到的一个问题：
使用map时vc6居然跳出了上百个warning C4786，上网查找才知道这个是vc的bug，由于stl里用的字符串过长，vc搞不定，呵呵，虽然可以不去管，但是如果程序出错误，要在上百条的warning找到错误信息还是很困难的。有人说用#pragma
 warning(disable: 4786)可以去掉，但是实验了一下居然不行，后来发现一个老外说要把这句话放到所有stl头的include之前，于是照办，果然可以，^_^老外就是敬业~~
******************************************************************************************
**pair**
pair同 map、set、mulitmap、multiset一样是关联容器
1 pair的应用
pair是将2个数据组合成一个数据，当需要这样的需求时就可以使用pair，如stl中的map就是将key和value放在一起来保存。另一个应用是，当一个函数需要返回2个数据的时候，可以选择pair。 pair的实现是一个结构体，主要的两个成员变量是first second 因为是使用struct不是class，所以可以直接使用pair的成员变量。
std::pair模型
template   <class T1,  class T2>  
  struct   pair  
  {  
      T1   first;  
      T2   second;  
  };   
2 make_pair函数
template pair make_pair(T1 a, T2 b) { return pair(a, b); }
很明显，我们可以使用pair的构造函数也可以使用make_pair来生成我们需要的pair。一般make_pair都使用在需要pair做参数的位置，可以直接调用make_pair生成pair对象很方便，代码也很清晰。另一个使用的方面就是pair可以接受隐式的类型转换，这样可以获得更高的灵活度。灵活度也带来了一些问题如：
std::pair<int, float>(1, 1.1);
std::make_pair(1, 1.1);
是不同的，第一个就是float，而第2个会自己匹配成double。
