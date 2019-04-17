# ODB——基于c++的ORM映射框架尝试(使用) - DoubleLi - 博客园






摘要： 2、使用 首先，需要定义一个对象，用来和数据库字段对应： [cce lang=”cpp”] #ifndef VOLUME_H #define VOLUME_H #include #include #pragma db object cl


2、使用
首先，需要定义一个对象，用来和数据库字段对应：
[cce lang=”cpp”]
#ifndef VOLUME_H
#define VOLUME_H

#include <string>
#include <odb/core.hxx>

#pragma db object
class Volume
{
public:
Volume(const std::string &name, const std::string &location, const std::string &cover_path, int trackCount)
: _name(name), _location(location), _cover_path(cover_path), _trackerCount(trackCount)
{}

unsigned long long id() { return _id; }
void id(unsigned long long i) { _id = i;}

const std::string &name() {return _name;}
void name(const std::string &n) {_name = n;}

const std::string &location() {return _location;}
void location(const std::string &l) {_location = l;}

const std::string &cover_path() {return _cover_path;}
void cover_path(const std::string &c) {_cover_path = c;}

int trackCount() {return _trackerCount;}
void trackCount(int c) {_trackerCount = c;}

private:
friend class odb::access;
Volume () {}

#pragma db id auto
unsigned long long _id;
std::string _name;
std::string _location;
std::string _cover_path;
int _trackerCount;
};
[/cce]
首先是引入core.hxx这个头文件，包含access这个类。在类上面添加#pragma db object宏，标识这是个数据库对象。在主键上增加宏#pragma db id auto，标识这个是主键，并且自增。这两个宏都是提供信息给odb，用来生成最终c++代码的。因为数据库对应字段都是私有类型，所以需要将odb::access声明为友元。

为了方便，这里连接数据库都使用sqlite，因此，需要引入sqlite相关的包。创建数据库连接(对sqlite来说，就是打开数据库文件)：
[cce lang=”cpp”]
std::shared_ptr<odb::database> sqliteDB(new odb::sqlite::database("mycppweb.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));
[/cce]
注：这里需要引入头文件odb/sqlite/database.hxx，创建数据库连接的第一个参数（只针对sqlite）是数据库文件名，后面是打开的flag，这里主要是提示如果数据库文件不存在，默认创建。另外，这里使用了c++11的shared_ptr，g++需要添加参数-std=c++0x

插入对象：
[cce lang=”cpp”]
{
odb::transaction t(db->begin());
volumeId = db->persist(volume);
t.commit();
}
[/cce]
插入的时候，使用了事务，需要引入odb/transaction.hxx头文件，并且尽量减少transaction的生命周期。

通过odb命令生成对应的代码：
[cce lang=”bash”]
odb –database sqlite \
–hxx-suffix .hpp –ixx-suffix .ipp –cxx-suffix .cpp \
–output-dir /tmp \
–generate-query –generate-schema –schema-format embedded volume.h
[/cce]
这里指定输出的数据库是sqlite，创建schema的语句嵌入到代码里面。
执行之后，会生成volume-odb.hpp、volume-odb.cpp和volume-odb.ipp三个文件。查看volume-odb.cpp就会发现，里面包含了volume.h中指定的类和数据库表的关系。如果希望通过代码来创建数据库（貌似需要自己来判断是否已经存在，否则第二次运行又会重新创建表，导致数据丢失），可以通过：
[cce lang=”cpp”]
{
odb::transaction t (sqliteDB->begin ());
odb::schema_catalog::create_schema (*sqliteDB);
t.commit ();
}
[/cce]

odb的查询，还没有去尝试，具体文档在[http://www.codesynthesis.com/products/odb/doc/manual.xhtml](http://www.codesynthesis.com/products/odb/doc/manual.xhtml)










