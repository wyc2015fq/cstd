# C++操作符重载专题(C++)  - ljx0305的专栏 - CSDN博客
2008年04月14日 22:47:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1018
一、为什么使用操作符重载？
　　对于系统的所有操作符，一般情况下，只支持基本数据类型和标准库中提供的class，对于用户自己定义的class，如果想支持基本操作，比如比较大小，判断是否相等，等等，则需要用户自己来定义关于这个操作符的具体实现。比如，判断两个人是否一样大，我们默认的规则是按照其年龄来比较，所以，在设计person 这个class的时候，我们需要考虑操作符==，而且，根据刚才的分析，比较的依据应该是age。那么为什么叫重载呢？这是因为，在编译器实现的时候，已经为我们提供了这个操作符的基本数据类型实现版本，但是现在他的操作数变成了用户定义的数据类型class，所以，需要用户自己来提供该参数版本的实现。
　　二、如何声明一个重载的操作符？
A:操作符重载实现为类成员函数
　　重载的操作符在类体中被声明，声明方式如同普通成员函数一样，只不过他的名字包含关键字operator，以及紧跟其后的一个c++预定义的操作符。
　　可以用如下的方式来声明一个预定义的==操作符:
　　class person{
　　private:
　　int age;
　　 public:
　　 person(int a){
　　this->age=a;
　　}
　　inline bool operator ==(const person &ps) const;
　　};
　　实现方式如下：
　　inline bool person::operator==(const person &ps) const
　　{ if (this->age==ps.age)
　　 return true;
　　return false;
　　}
　　调用方式如下：
　　 #include
　　using namespace std;
　　int main()
　　{ person p1(10);
　　person p2(20);
　　if(p1==p2) cout<<”the age is equal!”<　　return 0; 
　　}
　　这里，因为operator ==是class person的一个成员函数，所以对象p1,p2都可以调用该函数，上面的if语句中，相当于p1调用函数==，把p2作为该函数的一个参数传递给该函数，从而实现了两个对象的比较。
　　考虑如下的if语句：
　　 if(10==p1) cout<<”the age is equal!”<　　是否回正确执行呢？
　　答案是不会的，因为只有左操作数是该类类型的对象的时，才会考虑作为类成员重载操作符。因为10不是person类型的对象，所以，不能调用classperson的操作符==。
　　考虑如下if语句：
　　if(person(10)==person(11))
　　 cout<<"ok"<　　是否能够正确执行呢？答案是可以，因为操作符两边均是无名对象。
　　重载的操作符并不要求两个操作数的类型一定相同。例如：我们可以为class person定义下标操作符，以表示该person和电话的对应关系：
　　/*实现下标操作符*/
　　#include
　　#include
　　using namespace std;
　　class person
　　{ private:
　　 int tel;
　　public:
　　int & operator[](string const & nm) 
　　{
　　 return tel;
　　}
　　int GetTel()
　　{
　　return tel;
　　}
　　};
　　int main()
　　{
　　person p1;
　　p1["suo"]=110;
　　person p2;
　　p2["rose"]=120;
　　cout<　　cout<　　return 0;
　　}
　　对于重载为成员函数方式的操作符，隐式的this指针被作为该函数的第一个参数,来代表左操作数。
　B:操作符重载实现为非类成员函数(全局函数)
　　对于全局重载操作符，代表左操作数的参数必须被显式指定。例如：
　　#include
　　#include
　　using namespace std;
　　class person
　　{
　　public:
　　 int age;
　　public:
　　};
　　/*在类的外部，不能访问该类私有数据，所以，要把
　　age设置为public*/
　　bool operator==(person const &p1 ,person const & p2)
　　{
　　 if(p1.age==p2.age)
　　 return true;
　　 return false;
　　}
　　int main()
　　{
　　person rose;
　　person jack;
　　rose.age=18;
　　jack.age=23;
　　if(rose==jack)/*两个对象分别代表左右操作数*/ 
　　cout<<"ok"<　　return 0;
　　}
C:如何决定把一个操作符重载为类成员函数还是全局名字空间的成员呢？
　　①如果一个重载操作符是类成员，那么只有当与他一起使用的左操作数是该类的对象时，该操作符才会被调用。如果该操作符的左操作数必须是其他的类型，则操作符必须被重载为全局名字空间的成员。
　　②C++要求赋值=，下标[]，调用()， 和成员指向-> 操作符必须被定义为类成员操作符。任何把这些操作符定义为名字空间成员的定义都会被标记为编译时刻错误。
　　③如果有一个操作数是类类型如string类的情形那么对于对称操作符比如等于操作符最好定义为全局名字空间成员。
D:操作符重载为友元函数方式
　　 如果把操作符重载为友元函数方式，则在该函数的内部，可以直接访问授权类的私有数据成员，这是友
转自:http://yousee-me.blog.sohu.com/46436263.html
