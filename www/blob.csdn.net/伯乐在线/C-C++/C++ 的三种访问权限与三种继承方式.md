# C++ 的三种访问权限与三种继承方式 - 文章 - 伯乐在线
原文出处： [luoweifu](http://blog.csdn.net/luoweifu/article/details/46953343)
### 三种访问权限
我们知道C++中的类，有三种访问权限(也称作访问控制)，它们分别是public、protected、private。要理解它们其实也很容易，看下面了一个例子。
父类：


```
class Person
{
public:
    Person(const string& name, int age) : m_name(name), m_age(age)
    {
    }
    void ShowInfo()
    {
        cout << "姓名：" << m_name << endl;
        cout << "年龄：" << m_age << endl;
    }
protected:
    string  m_name;     //姓名
private:
    int     m_age;      //年龄
};
```
子类：


```
class Teacher : public Person
{
public:
    Teacher(const string& name, int age, const string& title)
        : Person(name, age), m_title(title)
    {
    }
    void ShowTeacherInfo()
    {
        ShowInfo();                             //正确，public属性子类可见
        cout << "姓名：" << m_name << endl;        //正确，protected属性子类可见
        cout << "年龄：" << m_age << endl;     //错误，private属性子类不可见
        cout << "职称：" << m_title << endl;   //正确，本类中可见自己的所有成员
    }
private:
    string  m_title;        //职称
};
```
调用方：


```
void test()
{
    Person person("张三", 22);
    person.ShowInfo();                  //public属性,对外部可见
    cout << person.m_name << endl;      //protected属性,对外部不可见
    cout << person.m_age << endl;       //private属性,对外部不可见
}
```
### 总结
我们对C++类三种方式控制权限总结如下，这与Java中的三种对应的访问权限是一样的。
![qq%e6%88%aa%e5%9b%be20161104113813](http://jbcdn2.b0.upaiyun.com/2016/11/af511dd385d4438091093f68b3fe3bcc.jpg)
### 三种继承方式
C++中继承的方式还有多种，也分别都用public、protected、private表示。这与Java不一样，Java只有继承的概念，默认是public继承的。
#### 1. 三种继承方式不影响子类对父类的访问权限，子类对父类只看父类的访问控制权**。**
如下面三种继承方式都能访问父类中的public和protected成员。


```
class Teacher : /*public*/ /*protected*/ private Person
{
public:
    Teacher(const string& name, int age, const string& title)
        : Person(name, age), m_title(title)
    {
    }
    void ShowTeacherInfo()
    {
        ShowInfo();                             //正确，public属性子类可见
        cout << "姓名：" << m_name << endl;        //正确，protected属性子类可见
        //cout << "年龄：" << m_age << endl;       //错误，private属性子类不可见
        cout << "职称：" << m_title << endl;   //正确，本类中可见自己的所有成员
    }
private:
    string  m_title;        //职称
};
```
#### 2. 继承方式是为了控制子类(也称派生类)的调用方(也叫用户)对父类(也称基类)的访问权限。
public继承


```
class Teacher : public Person
{
public:
    Teacher(const string& name, int age, const string& title)
        : Person(name, age), m_title(title)
    {
    }
    void ShowTeacherInfo()
    {
        ShowInfo();                             //正确，public属性子类可见
        cout << "职称：" << m_title << endl;   //正确，本类中可见自己的所有成员
    }
private:
    string  m_title;        //职称
};
```


```
void TestPublic()
{
    Teacher teacher("李四", 35, "副教授");
    teacher.ShowInfo();
    cout << endl;
    teacher.ShowTeacherInfo();
}
```
结果：
> 
姓名：李四
年龄：35
姓名：李四
年龄：35
职称：副教授
private继承：


```
class Teacher : private Person
{
public:
    Teacher(const string& name, int age, const string& title)
        : Person(name, age), m_title(title)
    {
    }
    void ShowTeacherInfo()
    {
        ShowInfo();                             //正确，public属性子类可见
        cout << "职称：" << m_title << endl;   //正确，本类中可见自己的所有成员
    }
private:
    string  m_title;        //职称
};
```


```
void TestPrivate()
{
    Teacher teacher("李四", 35, "副教授");
    teacher.ShowInfo();             //错误，因为Teacher采用了private的继承方式，外部不可访问。
    cout << endl;
    teacher.ShowTeacherInfo();
}
```
#### 3. public、protected、private三种继承方式，相当于把父类的public访问权限在子类中变成了对应的权限。
如protected继承，把父类中的public成员在本类中变成了protected的访问控制权限；private继承，把父类的public成员和protected成员在本类中变成了private访问控制权。
protected继承：


```
class Teacher : protected Person
{
public:
    Teacher(const string& name, int age, const string& title)
        : Person(name, age), m_title(title)
    {
    }
    void ShowTeacherInfo()
    {
        ShowInfo();                             //正确，public属性子类可见
        cout << "职称：" << m_title << endl;   //正确，本类中可见自己的所有成员
    }
private:
    string  m_title;        //职称
};
```


```
void TestProtected()
{
    Teacher teacher("李四", 35, "副教授");
    teacher.ShowInfo();         //错误，基类Person的ShowInfo此时对Teacher相当于protected的，外部不可以被访问
    cout << endl;
    teacher.ShowTeacherInfo();
}
```


```
class Leader : public Teacher
{
public:
    Leader(const string& name, int age, const string& title, string position)
        : Teacher(name, age, title), m_position(position)
    {
    }
    void ShowLeaderInfo()
    {
        ShowInfo();             //基类Person的ShowInfo此时相当于protected的，但子类仍可以访问
        ShowTeacherInfo();      //ShowTeacherInfo仍然是public的，可以访问
        cout << m_position << endl;
    }
private:
    string m_position;
};
```
