# C中的继承和多态 - 文章 - 伯乐在线
原文出处： [CODE PROJECT](http://www.codeproject.com/Articles/108830/Inheritance-and-Polymorphism-in-C)   译文出处：[吴秦的博客](http://www.cnblogs.com/skynet/archive/2010/09/23/1833217.html)
### **1、引言**
继承和多态是面向对象语言最强大的功能。有了继承和多态，我们可以完成代码重用。在C中有许多技巧可以实现多态。本文的目的就是演示一种简单和容易的技术，在C中应用继承和多态。通过创建一个VTable（virtual table）和在基类和派生类对象之间提供正确的访问，我们能在C中实现继承和多态。VTable能通过维护一张函数表指针表来实现。为了提供基类和派生类对象之间的访问，我们可以在基类中维护派生类的引用和在派生类中维护基类的引用。
### **2、说明**
在C中实现继承和多态之前，我们应该知道类（Class）在C中如何表示。
**2.1、类在C中的表示**
考虑C++中的一个类”Person”。

C
```
//Person.h
class Person
{
private:
    char* pFirstName;
    char* pLastName;
public:
    Person(const char* pFirstName, const char* pLastName);    //constructor
    ~Person();    //destructor
    void displayInfo();
    void writeToFile(const char* pFileName);
};
```
在C中表示上面的类，我们可以使用结构体，并用操作结构体的函数表示成员函数。

C
```
//Person.h
typedef struct _Person
{
    char* pFirstName;
    char* pLastName;
}Person;
new_Person(const char* const pFirstName, const char* const pLastName);    //constructor
delete_Person(Person* const pPersonObj);    //destructor
void Person_DisplayInfo(Person* const pPersonObj);
void Person_WriteToFile(Person* const pPersonObj, const char* const pFileName);
```
这里，定义的操作结构体Person的函数没有封装。为了实现封装，即绑定数据、函数、函数指针。我们需要创建一个函数指针表。构造函数new_Person()将设置函数指针值以指向合适的函数。这个函数指针表将作为对象访问函数的接口。
下面我们重新定义C中实现类Person。

C
```
//Person.h
typedef struct _Person Person;
//declaration of pointers to functions
typedef void    (*fptrDisplayInfo)(Person*);
typedef void    (*fptrWriteToFile)( Person*, const char*);
typedef void    (*fptrDelete)( Person *) ;
//Note: In C all the members are by default public. We can achieve 
//the data hiding (private members), but that method is tricky. 
//For simplification of this article
// we are considering the data members     //public only.
typedef struct _Person 
{
    char* pFName;
    char* pLName;
    //interface for function
    fptrDisplayInfo   Display;
    fptrWriteToFile   WriteToFile;
    fptrDelete      Delete;
}Person;
person* new_Person(const char* const pFirstName, 
                   const char* const pLastName); //constructor
void delete_Person(Person* const pPersonObj);    //destructor
void Person_DisplayInfo(Person* const pPersonObj);
void Person_WriteToFile(Person* const pPersonObj, const char* pFileName);
```
new_Person()函数作为构造函数，它返回新创建的结构体实例。它初始化函数指针接口去访问其它成员函数。这里要注意的一点是，我们仅仅定义了那些允许公共访问的函数指针，并没有给定私有函数的接口。让我们看一下new_Person()函数或C中类Person的构造函数。

C
```
//Person.c
person* new_Person(const char* const pFirstName, const char* const pLastName)
{
    Person* pObj = NULL;
    //allocating memory
    pObj = (Person*)malloc(sizeof(Person));
    if (pObj == NULL)
    {
        return NULL;
    }
    pObj->pFirstName = malloc(sizeof(char)*(strlen(pFirstName)+1));
    if (pObj->pFirstName == NULL)
    {
        return NULL;
    }
    strcpy(pObj->pFirstName, pFirstName);
    pObj->pLastName = malloc(sizeof(char)*(strlen(pLastName)+1));
    if (pObj->pLastName == NULL)
    {
        return NULL;
    }
    strcpy(pObj->pLastName, pLastName);
    //Initializing interface for access to functions
    pObj->Delete = delete_Person;
    pObj->Display = Person_DisplayInfo;
    pObj->WriteToFile = Person_WriteToFile;
    return pObj;
}
```
创建完对象之后，我们能够访问它的数据成员和函数。

C
```
Person* pPersonObj = new_Person("Anjali", "Jaiswal");
//displaying person info
pPersonObj->Display(pPersonObj);
//writing person info in the persondata.txt file
pPersonObj->WriteToFile(pPersonObj, "persondata.txt");
//delete the person object
pPersonObj->Delete(pPersonObj);
pPersonObj = NULL;
```
注意：不像C++，在C中我们不能在函数中直接访问数据成员。在C++中，可以隐式通过“this”指针直接访问数据成员。我们知道C中是没有“this”指针的，通过显示地传递对象给成员函数。在C中为了访问类的数据成员，我们需要把调用对象作为函数参数传递。上面的例子中，我们把调用对象作为函数的第一个参数，通过这种方法，函数可以访问对象的数据成员。
### **3、在C中类的表现**
Person类的表示——检查初始化接口指向成员函数：
**3.1、继承和多态的简单例子**
继承-Employee类继承自Person类：
![](http://ww1.sinaimg.cn/mw690/5e4d414cgw1es1gs1slvtj208e0cnjs9.jpg)
在上面的例子中，类Employee继承类Person的属性。因为DisplayInfo()和WriteToFile()函数是virtual的，我们能够从Person的实例访问Employee对象中的同名函数。为了实现这个，我们创建Person实例的时候也初始化Employee类。多态使这成为可能。 在多态的情况下，去解析函数调用，C++使用VTable——即一张函数指针表。
前面我们在结构体中维护的指向函数的指针接口的作用类似于VTable。

C
```
//Polymorphism in C++
Person PersonObj("Anjali", "Jaiswal");
Employee EmployeeObj("Gauri", "Jaiswal", "HR", "TCS", 40000);
Person* ptrPersonObj = NULL;
//preson pointer pointing to person object
ptrPersonObj = &PersonObj;
//displaying person info
ptrPersonObj ->Display();
//writing person info in the persondata.txt file
ptrPersonObj ->WriteToFile("persondata.txt");
//preson pointer pointing to employee object
ptrPersonObj = &EmployeeObj;
//displaying employee info
ptrPersonObj ->Display();
//writing empolyee info in the employeedata.txt file
ptrPersonObj ->WriteToFile("employeedata.txt");
```
在C中，继承可以通过在派生类对象中维护一个基类对象的引用来完成。在基类实例的帮助下，women可以访问基类的数据成员和函数。然而，为了实现多态，街垒对象应该能够访问派生类对象的数据。为了实现这个，基类应该有访问派生类的数据成员的权限。
为了实现虚函数，派生类的函数签名应该和基类的函数指针类似。即派生类函数将以基类对象的一个实例为参数。我们在基类中维护一个派生类的引用。在函数实现上，我们可以从派生类的引用访问实际派生类的数据。
**3.2、在C中结构体中的等效表示**
C中的继承-Person和Employee结构体：
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1es1gs25uh8j20e804w3yr.jpg)
如图所示，我们在基类结构体中声明了一个指针保存派生类对像，并在派生类结构体中声明一个指针保存基类对象。
在基类对象中，函数指针指向自己的虚函数。在派生类对象的构造函数中，我们需要使基类的接口指向派生类的成员函数。这使我们可以通过基类对象（多态）灵活的调用派生类函数。更多细节，请检查Person和Employee对象的构造函数。
当我们讨论C++中的多态时，有一个对象销毁的问题。为了正确的清楚对象，它使用虚析构函数。在C中，这可以通过使基类的删除函数指针指向派生类的析构函数。派生类的析构函数清楚派生类的数据和基类的数据和对象。注意：检查例子的源码中，实现须构造函数和虚函数的实现细节。
**创建Person对象**

C
```
//Person.h
typedef struct _Person Person;
//pointers to function
typedef void    (*fptrDisplayInfo)(Person*);
typedef void    (*fptrWriteToFile)(Person*, const char*);
typedef void    (*fptrDelete)(Person*) ;
typedef struct _person
{
    void* pDerivedObj;
    char* pFirstName;
    char* pLastName;
    fptrDisplayInfo Display;
    fptrWriteToFile WriteToFile;
    fptrDelete        Delete;
}person;
Person* new_Person(const char* const pFristName, 
                   const char* const pLastName);    //constructor
void delete_Person(Person* const pPersonObj);    //destructor
void Person_DisplayInfo(Person* const pPersonObj);
void Person_WriteToFile(Person* const pPersonObj, const char* const pFileName);
//Person.c
//construction of Person object
Person* new_Person(const char* const pFirstName, const char* const pLastName)
{
    Person* pObj = NULL;
    //allocating memory
    pObj = (Person*)malloc(sizeof(Person));
    if (pObj == NULL)
    {
        return NULL;
    }
    //pointing to itself as we are creating base class object
    pObj->pDerivedObj = pObj;
    pObj->pFirstName = malloc(sizeof(char)*(strlen(pFirstName)+1));
    if (pObj->pFirstName == NULL)
    {
        return NULL;
    }
    strcpy(pObj->pFirstName, pFirstName);
    pObj->pLastName = malloc(sizeof(char)*(strlen(pLastName)+1));
    if (pObj->pLastName == NULL)
    {
        return NULL;
    }
    strcpy(pObj->pLastName, pLastName);
    //Initializing interface for access to functions
    //destructor pointing to destrutor of itself
    pObj->Delete = delete_Person;
    pObj->Display = Person_DisplayInfo;
    pObj->WriteToFile = Person_WriteToFile;
    return pObj;
}
```
**Person对象的结构**
![](http://ww3.sinaimg.cn/mw690/5e4d414cgw1es1gs2kirlj20dk0dg3zt.jpg)
**创建Employee对象**

C
```
//Employee.h
#include "Person.h"
typedef struct _Employee Employee;
//Note: interface for this class is in the base class
//object since all functions are virtual.
//If there is any additional functions in employee add
//interface for those functions in this structure 
typedef struct _Employee
{
    Person* pBaseObj;
    char* pDepartment;
    char* pCompany;
    int nSalary;
    //If there is any employee specific functions; add interface here.
}Employee;
Person* new_Employee(const char* const pFirstName, const char* const pLastName,
        const char* const pDepartment, const char* const pCompany, 
        int nSalary);    //constructor
void delete_Employee(Person* const pPersonObj);    //destructor
void Employee_DisplayInfo(Person* const pPersonObj);
void Employee_WriteToFile(Person* const pPersonObj, const char* const pFileName);
//Employee.c
Person* new_Employee(const char* const pFirstName, const char* const pLastName,
                     const char* const pDepartment, 
                     const char* const pCompany, int nSalary)
{
    Employee* pEmpObj;
    //calling base class construtor
    Person* pObj = new_Person(pFirstName, pLastName);
    //allocating memory
    pEmpObj = malloc(sizeof(Employee));
    if (pEmpObj == NULL)
    {
        pObj->Delete(pObj);
        return NULL;
    }
    pObj->pDerivedObj = pEmpObj; //pointing to derived object
    //initialising derived class members
    pEmpObj->pDepartment = malloc(sizeof(char)*(strlen(pDepartment)+1));
    if(pEmpObj->pDepartment == NULL)
    {
        return NULL;
    }
    strcpy(pEmpObj->pDepartment, pDepartment);
    pEmpObj->pCompany = malloc(sizeof(char)*(strlen(pCompany)+1));
    if(pEmpObj->pCompany== NULL)
    {
        return NULL;
    }
    strcpy(pEmpObj->pCompany, pCompany);
    pEmpObj->nSalary = nSalary;
    //Changing base class interface to access derived class functions
    //virtual destructor
    //person destructor pointing to destrutor of employee
    pObj->Delete = delete_Employee;
    pObj->Display = Employee_DisplayInfo;
    pObj->WriteToFile = Employee_WriteToFile;
    return pObj;
}
```
**Employee对象的结构**
![](http://ww2.sinaimg.cn/mw690/5e4d414cgw1es1gs336pdj20gs09ajsc.jpg)
注意：从基类函数到派生类函数改变了接口（VTable）中指针位置。现在我们可以从基类（多态）访问派生类函数。我们来看如何使用多态。

C
```
Person* PersonObj = new_Person("Anjali", "Jaiswal");
Person* EmployeeObj = new_Employee("Gauri", "Jaiswal","HR", "TCS", 40000);
//accessing person object
//displaying person info
PersonObj->Display(PersonObj);
//writing person info in the persondata.txt file
PersonObj->WriteToFile(PersonObj,"persondata.txt");
//calling destructor
PersonObj->Delete(PersonObj);
//accessing to employee object
//displaying employee info
EmployeeObj->Display(EmployeeObj);
//writing empolyee info in the employeedata.txt file
EmployeeObj->WriteToFile(EmployeeObj, "employeedata.txt");
//calling destrutor
EmployeeObj->Delete(EmployeeObj);
```
### **结论**
使用上面描述的简单的额外代码能是过程式C语言有多态和继承的特性。我们简单的使用函数指针创建一个VTable和在基类和派生类对象中交叉维护引用。用这些简单的步骤，我们在C中可以实现继承和多态。
例子代码下载：[http://files.cnblogs.com/skynet/PolymorphisminC.zip](http://files.cnblogs.com/skynet/PolymorphisminC.zip)
