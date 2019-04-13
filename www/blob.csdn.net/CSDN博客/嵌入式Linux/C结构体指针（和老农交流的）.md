
# C结构体指针（和老农交流的） - 嵌入式Linux - CSDN博客

2014年03月12日 18:05:27[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：530个人分类：[C																](https://blog.csdn.net/weiqifa0/article/category/1389385)



```python
#include "stdio.h"
#include <stdlib.h>  
#include <string.h>  
struct student{
       int num;
       char *name;
       struct student *next;
};
int main(void)
{
    struct student a,b,c,*head,*p;
    
    a.name = "lilia";
    b.name = "lilib";
    c.name = "lilic";
    
    a.num = 1;
    b.num = 2;
    c.num = 3;
    
    head = &a;
    a.next = &b;
    b.next = &c;
    c.next = NULL;
    
    p = head;
	printf("a.num=%d a.name=%s\n",a.num,a.name);
	while(p!=NULL)
	{
		printf("%d %s\n",p->num,p->name);
		p=p->next;
	}
    return 1;
}
```
\#include "stdio.h"
struct student{
       int num;
       char name;
       struct student *next;
};
int main(void)
{
    struct student a,b,c,*head,*p;
    a.name = 'a';
    b.name = 'b';
    c.name = 'c';
    a.num = 1;
    b.num = 2;
    c.num = 3;
    head = &a;
    a.next = &b;
    b.next = &c;
    c.next = NULL;
    p = head;
	while(p!=NULL)  
    {  
        printf("%d %c\n",p->num,p->name);  
        p=p->next;  
    }  
	/*
    while(p != NULL){
         printf("d% s% \n",p->num,p->name);
         p = p->next;
    }*/
    return 1;
}


