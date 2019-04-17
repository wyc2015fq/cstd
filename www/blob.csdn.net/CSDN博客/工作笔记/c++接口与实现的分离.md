# c++接口与实现的分离 - 工作笔记 - CSDN博客





2012年04月23日 10:33:03[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5566








由于C++没有明确的将接口和实现分离，文件之间的编译依赖关系很大，如果有一个文件代码发生变化，则可能影响其他文件，乃至整个项目。因此，将对象实现细目隐藏于一个指针背后的目的，我们可以设计一个接口类。一个实现类，负责接口的实现。如下：

class PersonImpl;

class Date;

class Address;

class Person

{

public:

 Person(const std::string& name, const Date& birthday, const Address& addr);

 std::string name() const;

 std::string birthDate() const;

 std::string address() const;

private:

 boost::shared_ptr<PersonImpl> pImpl; //智能指针，指向实现物,即PIMPL手法

};

实质是申明依存性替换定义依存性，正是编译依存性最小化的本质:现实中让头文件尽可能自我满足，万一做不到，则让它与其他文件内的声明式(而非定义式)相依。其他每一件事都源自于这个简单的设计策略:

如果对象指针或者对象引用可以完成任务，就不要用对象去做。

如果可以，尽量用类的申明去替换类的定义式。

为声明式和定义式提供不同的头文件。



