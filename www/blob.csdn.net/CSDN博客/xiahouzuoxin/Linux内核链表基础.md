# Linux内核链表基础 - xiahouzuoxin - CSDN博客





2012年08月28日 21:50:41[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：3270标签：[linux内核																[list																[struct																[module																[structure																[makefile](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)
个人分类：[Linux](https://blog.csdn.net/xiahouzuoxin/article/category/1286747)





**1、内核链表的定义在include/linux/list.h**

struct list_head {

 struct list_head *next, *prev;

};

容易看出，Linux内核链表为双向链表。



**2、Linux链表与普通链表区别**

我们通常定义的链表是在链表节点中嵌入元素,比如

struct MyList

{

 int StudentID;       /* 被嵌入的元素 */

 struct MyList *prev;

 struct MyList *next;

}

而LInux为了移植方便性和通用性，在元素结构体中嵌入链表节点

strcut MyList

{

 int StudentID;

 struct list_head head;  /* 链表节点作为结构体元素 */

}



**3、Linux内核链表中提供的操作链表函数****(1)初始化**

static inline void INIT_LIST_HEAD(struct list_head *list)

{

 list->next = list;     /* 下一个节点指向自己 */

 list->prev = list;     /* 前一个节点指向自己 */

}
**(2)添加链表节点**

/**

 * list_add - add a new entry

 * @new: new entry to be added

 * @head: list head to add it after

 *

 * Insert a new entry after the specified head.

 * This is good for implementing stacks.

 */

static inline void list_add(struct list_head *new, struct list_head *head)

{

 __list_add(new, head, head->next);  /* 节点插入到head和head->next之间 */

}

而__list_add函数如下

static inline void __list_add(struct list_head *new,

         struct list_head *prev, struct list_head *next)

{

 next->prev = new;

 new->next = next;

 new->prev = prev;

 prev->next = new;

}
**(3)删除节点**

方法一：

/**

 * list_del - deletes entry from list.

 * @entry: the element to delete from the list.

 * Note: list_empty() on entry does not return true after this, the entry is

 * in an undefined state.

 */

static inline void list_del(struct list_head *entry)

{

 __list_del(entry->prev, entry->next);

 entry->next = (void *)0xDEADBEEF;      /* 将指针指向2个不可访问的位置 */

 entry->prev = (void *)0xBEEFDEAD;

}

其中调用的__list_del函数如下，

static inline void __list_del(struct list_head *prev, struct list_head *next)

{

 next->prev = prev;   /* */

 prev->next = next;

}

注意list_del函数中的最后两条语句，类似于free()的作用。

当用户打算访问地址0xDEADBEEF或0xBEEFDEAD时，将产生页中断。

方法二：

为了更安全的删除节点，可使用list_del_init

/**

 * list_del_init - deletes entry from list and reinitialize it.

 * @entry: the element to delete from the list.

 */

static inline void list_del_init(struct list_head *entry)

{

 __list_del(entry->prev, entry->next);

 INIT_LIST_HEAD(entry);

}



**(4)提取结构的数据信息**

按通常的方式使用链表很容易获取数据信息，

但使用Linux内核链表要访问数据则比较困难，

关键是如何求取链表节点地址和数据地址的偏移量。

注意list_entry传递的参数！type指传递的是类型，不是变量。

/**

 * list_entry - get the struct for this entry

 * @ptr: the &struct list_head pointer.

 * @type: the type of the struct this is embedded in.

 * @member: the name of the list_struct within the struct.

 */

#define list_entry(ptr, type, member) \

 container_of(ptr, type, member)

container_of定义在include/linux/kernel.h中，

/**

 * container_of - cast a member of a structure out to the containing structure

 * @ptr: the pointer to the member.

 * @type: the type of the container struct this is embedded in.

 * @member: the name of the member within the struct.

 *

 */

#define container_of(ptr, type, member) ({   \

 const typeof(((type *)0)->member) * __mptr = (ptr); \

 (type *)((char *)__mptr - offsetof(type, member)); })

而其中，

[a]#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

[b]typeof()是gcc的扩展，和sizeof()类似



**(5)、遍历链表**/**

 * list_for_each - iterate over a list

 * @pos: the &struct list_head to use as a loop cursor.

 * @head: the head for your list.

 */

#define list_for_each(pos, head) \

 for (pos = (head)->next; prefetch(pos->next), pos != (head); \

  pos = pos->next)

遍历链表也是一个宏定义，且是一个没加{}的for循环，

因此调用此函数时一定要加上{}。

使用时注意参数pos是指针类型，我们要定义一个list_head类型的

结构体变量指针传送给这个函数。


 4、链表使用实例



```cpp
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("monkeyzx");
MODULE_DESCRIPTION("Mylist Module");
MODULE_VERSION("V1.0");

struct student
{
	char name[100];
	int id;
	struct list_head list;
};

struct student *pstudent;
struct student *tmp_student;
struct list_head student_list;
struct list_head *pos;

int __init mylist_init()
{
	int i = 0;

	INIT_LIST_HEAD(&student_list);

	pstudent = kmalloc(sizeof(struct student)*5, GFP_KERNEL);
	memset(pstudent, 0, sizeof(struct student)*5);

	for(i=0; i<5; i++)
	{
		sprintf(pstudent[i].name, "Student %d", i+1);
		pstudent[i].id = i + 1;
		list_add(&(pstudent[i].list), &student_list);
	}

	list_for_each(pos, &student_list)
	{
		tmp_student = list_entry(pos, struct student, list);
		printk("<0>student %d name: %s\n", tmp_student->id, tmp_student->name);

	}
}

void __exit mylist_exit()
{
	int i = 0;

	for(i=0; i<5; i++)
	{
		list_del(&(pstudent[i].list));
	}

	kfree(pstudent);
}

module_init(mylist_init);
module_exit(mylist_exit);
```



makefile文件如下

#######################################################################################

```cpp
ifneq ($(KERNELRELEASE),)

obj-m := mylist.o

else
	
KDIR := /lib/modules/2.6.18-53.el5/build
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers

endif
```

########################################################################################](https://so.csdn.net/so/search/s.do?q=structure&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)




