# 带头尾指针的list的C实现 - maopig的专栏 - CSDN博客
2012年02月22日 23:30:18[maopig](https://me.csdn.net/maopig)阅读数：3193标签：[list																[c																[delete																[struct																[insert																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)
个人分类：[c/C++](https://blog.csdn.net/maopig/article/category/847675)
## 一、缘起
很早写了一个带头尾指针的list，该list支持从尾部插入元素，在任意位置删除元素，最近用这个list时发现一个bug，修正了，并加了几个接口函数。贴出来，希望对C的初学者有用。
## 二、基本说明
### 2.1、数据结构
l         listnode
typedef struct listnode
{
    int data;           //you can add any type data here 
    int id;
    struct listnode* next;
} *listnode, _listnode;
其中data、id用于测试程序，可换成任意数据类型。
l         list
typedef struct list
{
    struct listnode* head;
    struct listnode* tail;
    int count;           //0 --empty
    int current_id;
} *list, _list;
head、tail均指向实际元素，list为空时，指向NULL，current_id用于测试程序，可去。
### 2.2、函数说明
l         void list_init(list)
初始化list
l         void list_insert(list, listnode)
list的尾部插入元素
l         int list_delete(list, listnode)
从list上删除指定元素，删除成功返回1，删除失败返回0
l         void list_poll(list myroot)
遍历整个pool，测试用
l         listnode new_listnode(const int id, int data)
构建新的元素，并为其分配内存
l         void delete_listnode(listnode mylistnode)
删除指定元素，并释放其内存
l         listnode find_node_by_data(list myroot, const int data)
根据data在list上查找匹配的元素
l         listnode find_node_by_id(list myroot, const int id)
根据id在list上查找匹配的元素
## 三、代码
### 3.1、list.h：
/* list.h
** Copyright 2004 Coon Xu.
** Author: Coon Xu
** Date: 02 Feb 2005
*/
#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
typedef struct listnode
{
    int data;           //you can add any type data here 
    int id;
    struct listnode* next;
} *listnode, _listnode;
typedef struct list
{
    struct listnode* head;
    struct listnode* tail;
    int count;           //0 --empty
    int current_id;
} *list, _list;
void list_init(list);
void list_insert(list, listnode);
int list_delete(list, listnode);
void list_poll(list myroot);
listnode new_listnode(const int id, int data);
void delete_listnode(listnode mylistnode);
listnode find_node_by_data(list myroot, const int data);
listnode find_node_by_id(list myroot, const int id);
#endif
### 3.2、list.c：
/* list.c
** Copyright 2004 Coon Xu.
** Author: Coon Xu
** Date: 02 Feb 2005
*/
#include "list.h"
void list_init(list myroot)
{
    myroot->count = 0;
    myroot->head = NULL;
    myroot->tail = NULL;
    myroot->current_id = 1;
}
void list_poll(list myroot)
{
    printf("-------------------------------------------------------/n");
    listnode p_listnode = myroot->head;
    while(p_listnode != NULL)
    {
        printf("id: %d/t data: %d/n", p_listnode->id, p_listnode->data);
        p_listnode = p_listnode->next;
    }
}
//insert node at the tail
void list_insert(list myroot, listnode mylistnode)
{
    myroot->count++;
    mylistnode->next = NULL;
    if(myroot->head == NULL)
    {
        myroot->head = mylistnode;
        myroot->tail = mylistnode;
    }
    else
    {
        myroot->tail->next = mylistnode;
        myroot->tail = mylistnode;
    }    
    printf("[insert]:/tlist.cout:/t%d/n", myroot->count);
}
int list_delete(list myroot, listnode mylistnode)
{
    struct listnode* p_listnode = myroot->head;
    struct listnode* pre_listnode;
    //myroot is empty 
    if(p_listnode == NULL)
    {
        return 0;
    }
    //delete head node    
    if(p_listnode == mylistnode)
    {
        myroot->count--;
        //myroot has only one node
        if(myroot->tail == mylistnode)
        {
            myroot->head = NULL;
            myroot->tail = NULL;
        }
        else
        {
            myroot->head = p_listnode->next;
        }
        printf("[delete]:/tlist.cout:/t%d/n", myroot->count);
        return 1;
    }
    while(p_listnode != NULL)
    {
        if(p_listnode == mylistnode)
        {
            break;
        }
        pre_listnode = p_listnode;
        p_listnode = p_listnode->next;
    }
    if(p_listnode == NULL)
    {
        printf("can not find the node.../n");
        return 0;
    }
    else
    {
        pre_listnode->next = p_listnode->next;
        if(myroot->tail == p_listnode)
        {
            myroot->tail = pre_listnode;
        }
        myroot->count--;
        printf("[delete]:/tlist.cout:/t%d/n", myroot->count);
        return 1;
    }  
}
listnode new_listnode(const int id, int data)
{
    listnode p_listnode = malloc(sizeof(_listnode));
    p_listnode->id = id;
    p_listnode->data = data;
    return p_listnode;
}
void delete_listnode(listnode mylistnode)
{
    mylistnode->next = NULL;
    free(mylistnode);
    mylistnode = NULL;
}
listnode find_node_by_data(list myroot, const int data)
{
    listnode p_listnode = myroot->head;
    while(p_listnode != NULL)
    {
        if( p_listnode->data == data )
        {
            printf("find the node for data: %d/n", data);
            break;
        }
        p_listnode = p_listnode->next;
    }
    return p_listnode;
}
listnode find_node_by_id(list myroot, const int id)
{
    listnode p_listnode = myroot->head;
    while(p_listnode != NULL)
    {
        if( p_listnode->id == id )
        {
            printf("find the node for id: %d/n", id);
            break;
        }
        p_listnode = p_listnode->next;
    }
    return p_listnode;
}
### 3.3、附一个测试的主程序
main.c：
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
int main(int argc, char *argv[])
{
    list mylist = malloc(sizeof(_list));
    list_init(mylist);
    int ix = 0;
    for(ix = 0; ix < 10; ix++)
    {
        listnode my_listnode = new_listnode( (mylist->current_id)++, ix);
        list_insert(mylist, my_listnode);
    }
    list_poll(mylist);
    //delete head node and test
    listnode my_listnode = find_node_by_id(mylist, 1);
    list_delete(mylist, my_listnode);
    delete_listnode(my_listnode);
    list_poll(mylist);
    //insert a node and test
    my_listnode = new_listnode( (mylist->current_id)++, 100);
    list_insert(mylist, my_listnode);
    list_poll(mylist);
    //delete tail node and test
    my_listnode = find_node_by_id(mylist, 10);
    list_delete(mylist, my_listnode);
    delete_listnode(my_listnode);
    list_poll(mylist);
    //insert a node and test
    my_listnode = new_listnode( (mylist->current_id)++, 200);
    list_insert(mylist, my_listnode);
    list_poll(mylist);
    //delete a normal node and test    
    my_listnode = find_node_by_data(mylist, 6);
    list_delete(mylist, my_listnode);
    delete_listnode(my_listnode);
    list_poll(mylist);
    //insert a node and test
    my_listnode = new_listnode( (mylist->current_id)++, 300);
    list_insert(mylist, my_listnode);
    list_poll(mylist);
    //delete head node again and test    
    my_listnode = find_node_by_id(mylist, 2);
    list_delete(mylist, my_listnode);
    delete_listnode(my_listnode);
    list_poll(mylist);
    //insert a node and test  
    my_listnode = new_listnode( (mylist->current_id)++, 400);
    list_insert(mylist, my_listnode);
    list_poll(mylist);
  system("PAUSE");  
  return 0;
}
