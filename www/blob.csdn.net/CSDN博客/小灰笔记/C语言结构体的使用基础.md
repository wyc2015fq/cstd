# C语言结构体的使用基础 - 小灰笔记 - CSDN博客





2017年03月22日 23:47:31[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：153
个人分类：[C](https://blog.csdn.net/grey_csdn/article/category/6639462)









    C语言中的结构体说起来接触的很多，但是工作中自己真正写的想想多少？0个？这确实是很有意思的事情，结构体可以说是构造数据结构的中心，不过这么多年的工作中我用到的确实是不多。不过，在PowerPC的驱动编写中，官方提供的那些数据结构可真是帮了大忙。

       这段时间一直在接触Java，接触面向对象编程，也接触UNIX以数据为中心的理论。回头一想，数据结构确实是值得去仔细研读一下。在后面的工作中，如有可能还是把这项技能使用一下。

       数据结构定义的基本语法：

       struct tag {成员列表}变量列表;

       其实，知道了定义方式再知道成员的访问方式，基本的技能就描述了一半了。其他剩下的应该都算得上是使用技巧。

       如果考虑软件功能的管理，最好还是把结构体定义到头文件中，同时给结构体加上一个tag。这样不仅利于管理，而且还使用方便。

       关于指针，指向结构体的指针，定义起来也是很简单的。方法更定义一个普通的方法差不多，只是把数据类型换成tag的名字即可。

       结构体比数组自由度高很多，结构体的成员可以相同也可以不同。而且，结构体的成员可以是任何类型（本身结构体除外，否则无限递归），也可以是其他的结构体。如果结构体中加入指向结构体的指针成员时，可以方便地构建出链、表、树等不同的数据结构。

       结构体的初始化跟数组的初始化差不多，只是成员肯能不像数组全都是同类型成员，有可能是不同类型的成员。

       针对结构体常用的功能总结，写代码如下：

#include"stdio.h"

#include"string.h"



structtag_demo_result

{

       char name[50];

       int age;

       char sex[10];

       int score;

};



typedef structtag_demo_result demo_t,*demo_p_t;

demo_tresult_person1,result_person2;



int main(void)

{

       demo_p_t demo_p = NULL;



       strcpy(result_person1.name,"ZhangSan");

       result_person1.age = 20;

       strcpy(result_person1.sex,"male");

       result_person1.score = 95;



       strcpy(result_person2.name,"LiSi");

       result_person2.age = 21;

       strcpy(result_person2.sex,"male");

       result_person2.score = 93;



       demo_p = &result_person1;



       printf("name of person1:%s\n",demo_p->name);

       printf("age of person1:%d\n",demo_p->age);

       printf("sex of person1:%s\n",demo_p->sex);

       printf("score of person1:%d\n",demo_p->score);



       demo_p = &result_person2;



       printf("name of person2:%s\n",(*demo_p).name);

       printf("age of person2:%d\n",(*demo_p).age);

       printf("sex of person2:%s\n",(*demo_p).sex);

       printf("score of person2:%d\n",(*demo_p).score);



       return 0;

}

       编译运行结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_06>gcc exp_06.c



E:\WorkSpace\01_编程语言\01_C语言\exp_06>a

name of person1:ZhangSan

age of person1: 20

sex of person1:male

score of person1:95

name of person2:LiSi

age of person2: 21

sex of person2:male

score of person2:93

       用到的主要功能如下：

       1，结构体的tag；

       2，定义结构体数据类型；

       3，定义执行结构体的指针数据类型；

       4，通过结构体变量访问结构体成员；

       5，通过结构体指针变量访问结构体成员；

       6，结构体指针变量与结构体变量等效使用。



       需要注意的一个小细节是，如果是使用了结构体的tag在定义结构体中指向本身结构体类型的指针时是可以成功的，但是如果没有tag直接定义一个数据类型将会导致错误。具体如下：

       代码1：

#include"stdio.h"



typedef struct{

       int a;

       demo_t *b;

} demo_t;



int main(void)

{

       return 0;

}

       上面的代码在编译的时候会报错，因为在定义结构体中指向结构体的成员时结构体的定义还没有完成。为了能够实现上面的功能，代码应该修改如下;

#include"stdio.h"



typedef structdemo_tag_t{

       int a;

       struct demo_tag_t *b;

} demo_t;



int main(void)

{

       return 0;

}

       编译结果：

E:\WorkSpace\01_编程语言\01_C语言\exp_06>gcc exp_061.c



E:\WorkSpace\01_编程语言\01_C语言\exp_06>

       而这种技术非但是合法的，而且如前面所说，是实现多种数据结构的基础。



