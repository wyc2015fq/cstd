# C++中的Observer模式 - 电影旅行敲代码 - CSDN博客
2015年11月26日 22:01:59[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：760
# 引子
什么时候Observer模式呢？
- 对一个对象状态的更新，需要其他对象同步更新，而且其他对象的数量动态可变
- 对象仅需要将自己的更新通知给其他对象而不需要知道其他对象细节
许多时候，我们都需要当我们应用的一部分状态更新时，其他部分的状态能够相应的更新。有一种方式就是让接收者不断地去检查发送者的状态是否更新，这种方式有两种问题，**首先会占用大量的CPU时间，然后就是接受者的状态不会立即的更新，总会有一定的时间间隔**。
Observer模式有很多用途，比如我是数学老师，班里有30个学生，作为一个希望孩子们快乐成长的老师，我会在假期期间不定时的布置数学作业。
当我布置作业时，怎么通知孩子们让其立即写作业呢，有一种方式就是我把布置作业的通知写在校门外面的布告栏上，同学假期期间时不时来学校看一看数学老师有没有布置作业（心好累），如下图所示。这样做显然效率很低，最好的方式就是通知到每个孩子，让其立即写作业。
![这里写图片描述](https://img-blog.csdn.net/20151127101953490)
怎么样做呢，比如我可以把每一个孩子的电话号码记录到电话簿上，我不需要知道电话号码后面的学生是男是女，叫什么，家住哪里，我只需要挨个打电话过去通知他们写作业。这样做效率就高了很多，缺点就是我这里必须记录学生们的信息，以便能够通知到他们。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151127102611612)
其实Observer模式就是采用的这种方式，Subject对象存放了**一系列被通知的对象指针。然后通过对象指针来切实的通知到每一个对象**。
在实际设计中，为了考虑扩展，通常把**Subject(信息发送类)和Observer(接受类)设计成抽象基类**。抽象出最共同的特点，然后利用派生类实现具体的细节，例如将Subject将哪些通知发送过去，或者Observer接受到消息后，该执行哪些行为。例如有了这个电话簿，语文老师也可以利用这个电话簿作业发送平台，发送语文作业，学生也可以去做语文作业。如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20151127103804666)
重点是将Subject(老师)和Observer(学生)联系起来。 
代码如下，有一个抽象Teacher类作为Subject（信息发送者），有一个抽象Student类作为Observer（信息接收者）。Observer模式类图如下：
![这里写图片描述](https://img-blog.csdn.net/20151127150251001)
通过顶层建立好的联系，底层使用此接口即可。这种模式Subject作为主动方，主动去attach新的Observer，当然也可以在Observer中存储欲接受信息的对象(Subject* sub)，Subject无需去控制Observer的添加与删除，Observer去自行attach和detach即可。
```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;
class Teacher;
class Student
{
    public:
    virtual ~Student() {};
    virtual void DoHomeWork(string str) = 0;
    protected:
    // 设为保护，既防止实例化，派生类又可以调用
    Student() {}
};
class Teacher
{
    public:
    virtual ~Teacher() {}
    // 向电话簿添加学生
    virtual void Attach(Student* O) {m_Students.push_back(O);}
    // 从电话簿删除学生
    virtual void Detach(Student* O) 
    {
        m_Students.erase(remove(m_Students.begin(), m_Students.end(), O), m_Students.end());
    }
    // 通知学生们写作业
    virtual void Notify(string msg)
    {
        // 循环通知学生们去写作业
        for (vector<Student*>::const_iterator iter = m_Students.begin(); 
        iter != m_Students.end(); 
        ++iter)
        {
            if (*iter != 0)
            {
                (*iter)->DoHomeWork(msg);
            }
        }
    }
    protected:
    // 设为保护，既防止实例化，派生类又可以调用
    Teacher() {}
    private:
    vector<Student*> m_Students;
};
// 定义数学老师类，继承Teacher
class MathTeacher : public Teacher
{
    public:
    MathTeacher() {}
    // 数学老师布置作业
    void AssignHomework(string msg) { Notify(msg); }
};
// 定义语文老师类，继承Teacher
class ChineseTeacher : public Teacher
{
    public:
    ChineseTeacher() {}
    // 数学老师布置作业
    void AssignHomework(string msg) { Notify(msg); }
};
class FreshStudent : public Student
{
    public:
    FreshStudent() {}
    virtual ~FreshStudent() {}
    virtual void DoHomeWork(string str) { cout << "I have done "<< str << endl;};
};
int main()
{
    MathTeacher me;
    ChineseTeacher other_t;
    FreshStudent FS_1;
    FreshStudent FS_2;
    // 两位老师互相不可见，但是是同一批学生
    // 每个老师布置作业虽然不多，但是在学生来看，作业还是很繁重滴
    me.Attach(&FS_1);
    me.Attach(&FS_1);
    other_t.Attach(&FS_1);
    other_t.Attach(&FS_1);
    me.AssignHomework("math homework");
    other_t.AssignHomework("chinese homework");
    return 0;
}
```
注：参考了[这里](http://blog.csdn.net/fly542/article/details/6716825)
