# C++Boost序列化（Serialization）库教程 - fanyun的博客 - CSDN博客
2016年09月30日 09:22:33[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3794
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
[一个非常简单的情形](http://dozb.bokee.com/1692310.html#simplecase)
[非侵入的版本](http://dozb.bokee.com/1692310.html#nonintrusiveversion)
[可序列化的成员](http://dozb.bokee.com/1692310.html#serializablemembers)
[派生类](http://dozb.bokee.com/1692310.html#derivedclasses)
[指针](http://dozb.bokee.com/1692310.html#pointers)
[数组](http://dozb.bokee.com/1692310.html#arrays)
[STL容器](http://dozb.bokee.com/1692310.html#stl)
[类的版本](http://dozb.bokee.com/1692310.html#versioning)
[把serialize拆分成save/load](http://dozb.bokee.com/1692310.html#splitting)
[档案](http://dozb.bokee.com/1692310.html#archives)
输出档案(archive)类似于输出数据流(stream)。数据能通过<< 或 & 操作符存储到档案(archive)中:
ar << data;
ar & data;
输入档案(archive)类似于输入数据流(stream)。数据能通过>> 或 & 操作符从档案(archive)中装载。
ar >> data;
ar & data;
对于原始数据类型，当这些操作调用的时候，数据是简单的“被存储/被装载”“到/从”档案(archive)。对于类(class)数据类型，类的serialize 函数被调用。对上面的操作，每个serialize 函数用来“存储/装载”其数据成员。这个处理采用递归的方式，直到所有包含在类中的数据“被存储/被装载”。
一个非常简单的情形
通常用serialize 函数来存储和装载类的数据成员。
这个库包含一个叫 [demo.cpp](http://www.boost.org/libs/serialization/example/demo.cpp) 的程序，用于介绍如何用这个库。下面，我们从这个demo摘录代码，来介绍这个库应用的最简单情形。
#include <fstream>
// include headers that implement a archivein simple text format
#include<boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simpletype
//
class gps_position
{
private:
   friend class boost::serialization::access;
    //When the class Archive corresponds to an output archive, the
   // & operator is defined similar to <<.  Likewise, when the class Archive
   // is a type of input archive the & operator is defined similar to>>.
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
    {
       ar & degrees;
       ar & minutes;
       ar & seconds;
    }
   int degrees;
   int minutes;
   float seconds;
public:
   gps_position(){};
   gps_position(int d, int m, float s) :
        degrees(d), minutes(m), seconds(s)
   {}
};
int main() {
   // create and open a character archive for output
   std::ofstream ofs("filename");
   boost::archive::text_oarchive oa(ofs);
   // create class instance
    constgps_position g(35, 59, 24.567f);
   // write class instance to archive
   oa << g;
   // close archive
   ofs.close();
   // ... some time later restore the class instance to its orginal state
   // create and open an archive for input
   std::ifstream ifs("filename", std::ios::binary);
   boost::archive::text_iarchive ia(ifs);
   // read class state from archive
   gps_position newg;
   ia >> newg;
   // close archive
   ifs.close();
   return 0;
}
对于每个通过序列化“被存储”的类，必须存在一个函数去实现“存储”其所有状态数据。对于每个通过序列化“被装载”的类，必须存在一个函数来实现“装载”其所有状态数据。在上面的例子中，这些函数是模板成员函数serialize。
非侵入的版本
在上例是侵入的设计。类是需要由其实例来序列化，来改变。这在某些情形是困难的。一个等价的可选的设计如下：
#include<boost/archive/text_oarchive.hpp>
#include<boost/archive/text_iarchive.hpp>
class gps_position
{
public:
    intdegrees;
   int minutes;
   float seconds;
   gps_position(){};
   gps_position(int d, int m, float s) :
       degrees(d), minutes(m), seconds(s)
   {}
};
namespace boost {
namespace serialization {
template<class Archive>
void serialize(Archive & ar,gps_position & g, const unsigned int version)
{
   ar & g.degrees;
   ar & g.minutes;
   ar & g.seconds;
}
} // namespace serialization
} // namespace boost
这种情况生成的serialize 函数不是gps_position类的成员函数。这有异曲同工之妙。
非侵入序列化主要应用在不改变类定义就可实现类的序列化。为实现这种可能，类必须提供足够的信息来更新类状态。在这个例子中，我们假设类有public成员。仅当提供足够信息来存储和装载的类，才能不改变类自身，在外部来序列化类状态。
可序列化的成员
一个可序列化的类，可拥有可序列化的成员，例如：
class bus_stop
{
   friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
    {
       ar & latitude;
       ar & longitude;
    }
   gps_position latitude;
   gps_position longitude;
protected:
   bus_stop(const gps_position & lat_, const gps_position & long_):
    latitude(lat_),longitude(long_)
   {}
public:
   bus_stop(){}
   // See item # 14 in Effective C++ by Scott Meyers.
   // re non-virtual destructors in base classes.
   virtual ~bus_stop(){}
};
这里，类类型的成员被序列化，恰如原始类型被序列化一样。
注意，类bus_stop的实例“存储”时，其归档(archive)操作符将调用latitude 和 longitude的serialize 函数。这将依次调用定义在gps_position中的serialize 来被“存储”。这种手法中，通过bus_stop的归档(archive)操作符,整个数据结构被存储,bus_stop是它的根条目。
派生类
派生类应包含其基类的序列化。
#include<boost/serialization/base_object.hpp>
class bus_stop_corner : public bus_stop
{
    friend class boost::serialization::access;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
    {
       // serialize base class information
       ar & boost::serialization::base_object<bus_stop>(*this);
       ar & street1;
       ar & street2;
    }
   std::string street1;
   std::string street2;
   virtual std::string description() const
    {
       return street1 + " and " + street2;
    }
public:
   bus_stop_corner(){}
   bus_stop_corner(const gps_position & lat_, const gps_position &long_,
       const std::string & s1_, const std::string & s2_
    ):
       bus_stop(lat_, long_), street1(s1_), street2(s2_)
   {}
};
注意在派生类中不要直接调用其基类的序列化函数。这样做看似工作，实际上绕过跟踪实例用于存储来消除冗余的代码。它也绕过写到档案中类的版本信息的代码。因此，总是声明serialize 作为私有函数。声明friend boost::serialization::access 将运行序列化库存取私有变量和函数。
指针
假设我们定义了bus route包含一组bus stops。假定：
我们可以有几种bus stop的类型（记住bus_stop是一个基类）。
一个所给的 bus_stop可以展现多于一个的路线。
一个bus route 用一组指向bus_stop的指针来描述是方便的。
class bus_route
{
    friendclass boost::serialization::access;
   bus_stop * stops[10];
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
    {
       int i;
       for(i = 0; i < 10; ++i)
           ar & stops[i];
    }
public:
    bus_route(){}
};
数组stops 的每个成员将被序列化。但是，记住每个成员是个指针。 - 实际含义是什么？序列化整个对象是要求在另一个地方和时间重新构造原始数据结构。用指针为了完成这些，存储指针的值是不够的，指针指向的对象必须存储。当成员最后被装载，一个新的对象被创建，新的指针被装载到类的成员中。
所有这一切是由序列化库自动完成的。通过指针关联的对象，上述代码能完成存储和装载。
数组
事实上上述方案比较复杂。序列化库能检测出被序列化的对象是一个数组，将产生上述等价的代码。因此上述代码能更短的写为：
class bus_route
{
   friend class boost::serialization::access;
   bus_stop * stops[10];
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
    {
       ar & stops;
    }
public:
   bus_route(){}
};
STL容器
上面的例子用数组成员。更多的如此的一个应用用STL容器为如此的目的。序列化库包含为所有STL容器序列化的代码。因此，下种方案正如我们所预期的样子工作。
#include<boost/serialization/list.hpp>
class bus_route
{
   friend class boost::serialization::access;
   std::list<bus_stop *> stops;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
    {
       ar & stops;
    }
public:
   bus_route(){}
};
类的版本
假设我们对bus_route类满意，在产品中使用它。一段时间后，发觉bus_route 类需要包含线路驾驶员的名字。因此新版本如下：
#include <boost/serialization/list.hpp>
#include<boost/serialization/string.hpp>
class bus_route
{
   friend class boost::serialization::access;
   std::list<bus_stop *> stops;
   std::string driver_name;
   template<class Archive>
   void serialize(Archive & ar, const unsigned int version)
    {
       ar & driver_name;
       ar & stops;
    }
public:
   bus_route(){}
};
好，完毕！异常...会发生在读取旧版本所生成的数据文件时。如何考虑版本问题？
通常，序列化库为每个被序列化的类在档案中存储版本号。缺省值是0。当档案装载时，存储的版本号可被读出。上述代码可修改如下：
#include <boost/serialization/list.hpp>
#include<boost/serialization/string.hpp>
#include<boost/serialization/version.hpp>
class bus_route
{
   friend class boost::serialization::access;
   std::list<bus_stop *> stops;
   std::string driver_name;
   template<class Archive>
    voidserialize(Archive & ar, const unsigned int version)
    {
       // only save/load driver_name for newer archives
       if(version > 0)
           ar & driver_name;
       ar & stops;
    }
public:
   bus_route(){}
};
BOOST_CLASS_VERSION(bus_route, 1)
对每个类通过应用的版本，没有必要维护一个版本文件。一个文件版本是所有它组成的类的版本的联合。系统允许程序和以前版本的程序创建的档案向下兼容。
把serialize拆分成save/load
serialize函数是简单，简洁，并且保证类成员按同样的顺序（序列化系统的key）被存储/被装载。可是有像这里例子一样，装载和存储不一致的情形。例如，一个类有多个版本的情况发生。上述情形能重写为：
#include<boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include<boost/serialization/version.hpp>
#include<boost/serialization/split_member.hpp>
class bus_route
{
   friend class boost::serialization::access;
   std::list<bus_stop *> stops;
   std::string driver_name;
    template<classArchive>
   void save(Archive & ar, const unsigned int version) const
    {
       // note, version is always the latest when saving
       ar  & driver_name;
       ar  & stops;
    }
   template<class Archive>
   void load(Archive & ar, const unsigned int version)
    {
       if(version > 0)
           ar & driver_name;
       ar  & stops;
    }
   BOOST_SERIALIZATION_SPLIT_MEMBER()
public:
   bus_route(){}
};
BOOST_CLASS_VERSION(bus_route, 1)
BOOST_SERIALIZATION_SPLIT_MEMBER() 宏生成调用 save 或 load的代码，依赖于是否档案被用于“存储”或“装载”。
档案
我们这里讨论将聚焦到类的序列化能力上。被序列化的数据的实际编码实现于档案(archive)类中。被序列化的数据流是所选档案(archive)类的序列化的产物。(键)key设计决定这两个组件的独立性。允许任何序列化的规范可用于任何档案(archive)。
在这篇指南中，我们用了一个档案类-用于存储的text_oarchive和用于装载的text_iarchive类。在库中其他档案类的接口完全一致。一旦类的序列化已经被定义，类能被序列化到任何档案类型。
假如当前的档案集不能提供某个属性，格式，或行为需要特化的应用。要么创建一个新的要么从已有的里面衍生一个。将在后继文档中描述。
注意我们的例子save和load程序数据在一个程序中，这是为了讨论方便而已。通常，被装载的档案或许在或许不在同一个程序中。
T完整的演示程序 - [demo.cpp](http://www.boost.org/libs/serialization/example/demo.cpp) 包括：
创建各种类别的 stops, routes 和 schedules
显示它
序列化到一个名叫 "testfile.txt"的文件中 
还原到另一个结构中
显示被存储的结构
[这个程序的输出](http://www.boost.org/libs/serialization/example/demo_output.txt) 分证实了对序列化系统所有的要求，都在这个系统中体现了。对序列化文件是ASCII文本的[档案文件的内容](http://www.boost.org/libs/serialization/example/demofile.txt) 能被显示。
