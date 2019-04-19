# python c++ 成员函数中的访问多态，与动态绑定_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
python本来就是动态解析，调用成员函数，都是传进去self，
等执行代码的时候，self动态解析变量，无论是函数还是变量。
class Foo(object):
 name = 'foo
name'
 age = 'foo
age'
 def
displayName(self):
print 'displayName in foo with',self.name
self.displayAge();
 def
displayAge(self):
print 'displayAge in Foo with',self.age
class Bar(Foo):
 name = 'Bar
name'
 age = 'Bar
age'
 def
displayAge(self):
print 'displayAge in Bar with',self.age
Foo().displayName() # displayName in foo with foo name #
displayAge in foo with foo age
Bar().displayName() # displayName in foo with Bar name # displayAge
in Bar with Bar age
c++是显式的定义变量类型，定义什么就是什么。
c++里面，子类赋值给父类变量。那么就是连虚表里面的虚函数地址也是要修改的，不然指向子类的虚函数，那你还是不是父类了。定义什么就是什么。
也只有指针和引用才引入了多态。一般来说多态是指调用派生类的成员。
成员函数里面访问类成员是转化成指针的形式，所以还是会出现多态。
成员函数里面访问虚函数的话，还是会实现多态机制。和在外部，指针访问成员是一样。
下面说明：指针访问成员函数时
displayname中访问name是访问foo的name，
但是访问displayage是访问bar里面的displayage，
displayage访问的是bar里面的age。
所以，除了虚函数配合指针，其它的定义什么就是什么，在哪里定义就访问哪里。
class Foo
{
public:
char * name = "foo name";
char* age = "foo age";
virtual void displayName(){
cout << "displayName in
foo with " << name << endl;
displayAge();
}
virtual void displayAge(){
cout << "displayAge in
foo with " << age << endl;
}
};
class Bar:public Foo
{
public:
char * name = "Bar name";
char * age = "Bar age";
virtual void displayAge(){
cout << "displayAge in
Bar with " << age << endl;
}
};
int _tmain(int argc, _TCHAR* argv[])
{
Foo foo = Bar();
foo.displayName(); // displayName in foo with foo
name # displayAge in foo with foo age
Foo * fp = &Bar(); // displayName in foo
with foo name # displayAge in bar with bar age
fp->displayName();
system("pause");
return 0;
}

