# Struct结构体的使用学习 - BlueSky - CSDN博客
2015年10月09日 09:04:21[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：289标签：[C																[结构体																[Struct](https://so.csdn.net/so/search/s.do?q=Struct&t=blog)](https://so.csdn.net/so/search/s.do?q=结构体&t=blog)](https://so.csdn.net/so/search/s.do?q=C&t=blog)
## Struct结构体的使用
结构体的声明
```
struct _Person
{
    char *name;
    int age;
    double height;  
};
```
struct  _Person  p1; 
每次使用结构体的时候都要struct _Person ，比如struct _Person p1={0};sizeof(struct _Person ); 太麻烦。 
2、typedef int zhengshu;可以给int取一个别名zhengshu。
因此也可以用typedef struct _Person Person;把Person定义为struct _Person 的别名
3、当然最常用的简化写法：
```
typedef struct _Person
{
int age;
char* name;
} Person;
```
4、结构体的赋值 
Person p1={0}; 
p1.name = “全智贤”; 
p1.age=30; 
Person p2 = p1; 
printf(“%d,%d\n”,&p1,&p2); 
printf(“%s的年龄是%d\n”,p1.name,p1.age); 
p1和p2的地址不一样，结构体赋值是“复制一份”
5、为了避免内存占用，一般只用一份，因此通过结构体指针传递：
Person *p2 = &p1;
printf(“%s的年龄是%d\n”,p2->name,p2->age);
6、注意：对于结构体指针，取成员要使用->，而不是“.”
```
#include <stdio.h>
typedef struct _Person
{
    char *name;
    int age;
} Person;
int main(int argc, char *argv[])
{
    Person p1 ={0};
    p1.name= "aaaa";
    p1.age= 30;
    printf("%s,%d\n",p1.name,p1.age);
    Person p2 = p1;//创建一个Person结构体内存
        //复制p1的内容
    p1.age = 300;
    printf("%s,%d\n",p2.name,p2.age);
    printf("%d,%d\n",&p1,&p2);
    Person* p3 = &p1;
    printf("%d,%d\n",p3,&p1);
    printf("%s,%d\n",p3->name,p3->age);//对于结构体指针
//取成员的值要使用->
    p1.age= 100;
    printf("%s,%d\n",p3->name,p3->age);
    return 0;
}
```
