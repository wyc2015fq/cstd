# C++ ORM ODB入门 - DoubleLi - 博客园






# 1.ORM



ORM, Object Relational Mapping, 对象关系映射，用来将基于对象的数据结构映射到SQL的数据结构中。即将基于对象的数据映射到关系表中的字段，然后我们可以通过对象提供的接口来操作数据库，而无需写sql语句来操作数据库。
一般一张关系表对应两个类，一个实体类和一个操作类。
ORM是一种框架，而不是一种实现。




# 2.C++ 的ORM的实现ODB


1)一个简单的例子
#include <odb/core.hxx>  //(1) 


#pragma db object//(2) 


class person
{
public:
 person (const std::string& first,
 const std::string& last,
 unsigned short age);
 const std::string& first () const;
 const std::string& last () const;
 unsigned short age () const;
 void age (unsigned short);
private:
 person(); //(3)

 friend class odb::access;//(4)

 #pragma db id auto//(5)
 unsigned long id_;//(5)
 std::string first_;
 std::string email_;
 std::string last_;
 unsigned short age_;
};


Note:
(1) headers to provide class like 'access'


(2) 告诉编译器这是一个persistent class,所以我们要将#pragma db object放在类的定义前。将一个类声明为persistent class并不意味着这个类的所有对象都会被存储到数据库中。


(3)这个构造函数在实例化一个persistent class时会用到。


(4)让默认的构造函数和其它类成员能够访问access类。


(5)每个persistent object通常都要有一个独一无二的id.如果persistent object没有定义id，那么数据库的操作就会受限。
#pragma db id auto 用来指定这个对象id，auto表明这个id的值是有数据库自动分配的。
这个id可以使这个类本身就具有的成员，也可以是自己添加仅作为id来标识不同的persistent对象。上面的例子中的id_就是仅仅用来区别persistent object.


上面的例子我们也可以写成：
#include <odb/core.hxx>  //(1) 


#pragma db object//(2) 
class person
{
public:
 person();
 person (const std::string& first,
 const std::string& last,
 unsigned short age);
 const std::string& first () const;
 const std::string& last () const;
 unsigned short age () const;
 void age (unsigned short);
private:
 #pragma db id auto//(5)
 unsigned long id_;//(5)
 std::string first_;
 std::string email_;
 std::string last_;
 unsigned short age_;
};
//用来指定persistent 的id
#pragma db object(person)
#pragma db member(person::email_) id




# 3.用ODB编译器生成代码


将上面的代码保存为person.hxx,然后使用如下的命令来编译：
odb -d mysql --generate-query person.hxx
-d 用来指定我们使用的数据库。odb支持很多不同的数据库，我们以mysql为例。


这个命令会让odb的编译器生成三个文件：person-odb.hxx, person-odb.ixx, person-odb.cxx.


--generate-query用来指定编译器生成数据库支持的代码。如果我们使用--generate-shema, 那么还将生成person-odb.sql文件。


ok, 现在我们已经有persistent class(person.cxx)和database support code(person-odb.hxx, person-odb.ixx, person-odb.cxx)了。




# 4.编译和运行


假如我们的main文件为diver.cxx, 我们应该先编译main文件和上面生成的cxx文件:
c++ -c driver.cxx
c++ -c person-odb.cxx -Iyou_dir_to_odb_headers
c++ -o driver driver.o person-odb.o -Lyou_dir_to_odb_lib


在运行数据库前，记得先启动mysql并在库中使用指定表格。
mysql --user=user_name --database=you_db\




# 5.一个更加实用的例子


// driver.cxx
//
#include <memory> // std::auto_ptr
#include <iostream>
#include <odb/database.hxx> //定义database类
#include <odb/transaction.hxx> //定义transaction类
#include <odb/mysql/database.hxx> //实现database类的接口，同样要include
#include "person.hxx"
#include "person-odb.hxx"
using namespace std;
using namespace odb::core;//relative namespace.下面直接使用odb::batabase和odb::transaction就可以了，不用写成odb::core::database和odb::core::transaction了。


int
main (int argc, char* argv[])
{
 try
 {
 auto_ptr<database> db (new odb::mysql::database (argc, argv));//(1)
 unsigned long john_id, jane_id, joe_id;
 // Create a few persistent person objects.
 //
 {
 person john ("John", "Doe", 33);
 person jane ("Jane", "Doe", 32);
 person joe ("Joe", "Dirt", 30);
 transaction t (db->begin ());//(1)
 // Make objects persistent and save their ids for later use.
 //
 john_id = db->persist (john);//(2)
 jane_id = db->persist (jane);//(2)
 joe_id = db->persist (joe);//(2)
 t.commit ();//(3)
 }
 }
 catch (const odb::exception& e)
 {
 cerr << e.what () << endl;
 return 1;
 }
}


Note:
(1) 首先，我们要先创建一个database对象和transaction对象。
odb的实现中，所有database的操作都必须在一个transaction内进行。transaction是我们操作数据库的工作量的一个原子单位。一个transaction内包含有很过个database操作。如果一次transaction成功，则这个transaction内的数据库操作都会成功。如果一次transaction失败，这个transaction内的所有操作都会失败。


(2)我们在new了很多person对象后，使用persist选择奖哪些对象存储到数据库。


(3)提交一次transaction。如果在commit之前程序崩溃，所有的将要进行保存的数据库都不会被存储。


编译程序，然后开启数据库，然后执行程序。查看SQL的记录，我们可看到如下记录：
INSERT INTO ‘person‘ (‘id‘,‘first‘,‘last‘,‘age‘) VALUES (?,?,?,?)
INSERT INTO ‘person‘ (‘id‘,‘first‘,‘last‘,‘age‘) VALUES (?,?,?,?)
INSERT INTO ‘person‘ (‘id‘,‘first‘,‘last‘,‘age‘) VALUES (?,?,?,?)




# 6.查询数据库


 typedef odb::query<person> query;
 typedef odb::result<person> result;
 // Say hello to those over 30.
 //
 {
 transaction t (db->begin ());
 result r (db->query<person> (query::age > 30));
 for (result::iterator i (r.begin ()); i != r.end (); ++i)
 {
 cout << "Hello, " << i->first () << "!" << endl;
 }
 t.commit ();
 }
 }


#  7.更新数据库


 (1)//使用object id
 unsigned long john_id, jane_id, joe_id;
 // Create a few persistent person objects.
 //
 {
 ...
 // Save object ids for later use.
//
 john_id = john.id ();
 jane_id = jane.id ();
 joe_id = joe.id ();
 }
 // Joe Dirt just had a birthday, so update his age.
 //
 {
 transaction t (db->begin ());
 auto_ptr<person> joe (db->load<person> (joe_id));
 joe->age (joe->age () + 1);
 db->update (*joe);
 t.commit ();
 }


 (2)不使用object id
  // Joe Dirt just had a birthday, so update his age. An
 // alternative implementation without using the object id.
 //
 {
 transaction t (db->begin ());
 result r (db->query<person> (query::first == "Joe" &&
 query::last == "Dirt"));
 result::iterator i (r.begin ());
 if (i != r.end ())
 {
 auto_ptr<person> joe (i.load ());
 joe->age (joe->age () + 1);
 db->update (*joe);
 }
 t.commit ();
 }

# 8.删除persistent object


删除一个object要使用到object 的id 
 // John Doe is no longer in our database.
 //
 {
 transaction t (db->begin ());
 db->erase<person> (john_id);
 t.commit ();
 }


 当然，也可以不用object id 
  // John Doe is no longer in our database. An alternative
 // implementation without using the object id.
 //
 {
 transaction t (db->begin ());
 result r (db->query<person> (query::first == "John" &&
 query::last == "Doe"));
 result::iterator i (r.begin ());
 if (i != r.end ())
 {
 auto_ptr<person> john (i.load ());
 db->erase (*john);
 }
 t.commit ();
 }









