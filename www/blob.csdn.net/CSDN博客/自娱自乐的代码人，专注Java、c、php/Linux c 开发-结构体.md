# Linux c 开发 - 结构体 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年10月18日 15:52:30[initphp](https://me.csdn.net/initphp)阅读数：1311标签：[c																[语言																[struct																[float																[数据结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)](https://so.csdn.net/so/search/s.do?q=float&t=blog)




实际的项目开发中，有比较复杂的数据结构。例如学生，有姓名、年龄等属性参数。这个时候使用传统的类型就无法满足我们的需要。C语言的结构体就可以解决这个问题。

### 1. 定义结构体

结构体都是通过关键字 **struct** 来定义。例如我们定义一个结构体，包含学生的姓名和年龄：



```cpp
struct student {  
    char *username;  
    int age;  
};
```


struct为结构体的关键字。student为结构体的名称。






### 2. 结构体变量

结构体变量有多种方式定义。

1. 直接在结构体上定义。这种方式比较常用。



```cpp
//定义一个结构体  
struct student {  
    char *username;  
    int age;  
} s1, s2;
```


2. 直接说明结构变量。这种方法省了student，只能这几个变量用这个结构体，其它人就无法使用了。





```cpp
//定义一个结构体  
struct {  
    char *username;  
    int age;  
} s1, s2;
```


3. 先定义结构类型，再说明结构变量。





```cpp
//定义一个结构体  
struct student {  
    char *username;  
    int age;  
};
struct  student s1;
struct  student s2;
```




### 3. 结构体中带结构体



```cpp
//定义一个结构体 
struct date  {  
    int month;  
    int day;  
    int year;  
};   
struct student {  
    char *username;  
    int age;  
    struct date birthday;
};
struct  student s1;
```
我们先定义了一个date的结构体，主要用来存储时间信息。



在student的结构体中，定义了date结构体的一个参数。这种情况就是结构体中带结构体，比较适合复杂的数据结构类型。




### 4. 结构体结构变量使用

结构体中的结构变量使用，一般通过. 点号。{结构体变量名称}.{结构变量}

例如：



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h>

//定义一个结构体  
struct student {  
    char *username;  
    int age;  
};


int main(void) {
    struct  student s1;
    s1.username = "test";
    s1.age = 100;
    printf("结构体数据：用户名%s 用户年龄%d", s1.username, s1.age);
}
```






### 5. 结构体变量初始化



```cpp
//初始化s1
struct student {  
    char *username;  
    int age;  
} s1 = {"test", 100};


int main(void) {
    printf("结构体数据：用户名%s 用户年龄%d", s1.username, s1.age);
}
```




### 6. 结构体数组



```cpp
//初始化s1
struct student {  
    char *username;  
    int age;  
};

int main(void) {
    struct  student s1[2]; //定义一个结构体数组
    s1[0].username = "test";
    s1[0].age = 100;
    s1[1].username = "test2";
    s1[1].age = 200;
    printf("结构体数据：用户名%s 用户年龄%d\n", s1[0].username, s1[0].age);
    printf("结构体数据：用户名%s 用户年龄%d", s1[1].username, s1[1].age);
}
```




### 7. 结构体指针



```cpp
#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>   
#include <string.h>  
  
//定义一个结构体    
struct student {    
    char *username;    
    int age;    
};    
  
int main(void) {  
    struct student *p = NULL; //定义一个结构体指针  
    p = (struct student *) malloc(sizeof(struct student)); //分配一个结构体内存，p指向这个内存地址  
    p->username = "zhuli";   
    p->age = 100;  
    printf("结构体数据：用户名%s 用户年龄%d", p->username, p->age);  
}
```




### 8. 使用typedef来定义结构体

使用**typedef**来定义结构体的好处就是：

1. 不需要每次都写struct关键词

2. 更加形象的表达数据结构类型




```cpp
typedef struct student STU;
struct student {  
    char *username;  
    int age;  
};

int main(void) {  
    STU *p = NULL; //定义一个结构体指针  
    p = (STU *) malloc(sizeof(STU)); //分配一个结构体内存，p指向这个内存地址  
    p->username = "zhuli";   
    p->age = 100;  
    printf("结构体数据：用户名%s 用户年龄%d", p->username, p->age);  
}
```](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)




