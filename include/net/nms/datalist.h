#ifndef DataList_H
#define DataList_H

//#include <sys/ipc.h>
#include <pthread.h>
#include <stdlib.h>
//#include <sys/time.h>
/*定义节点类型*/

typedef struct _ListNode {
  void* data;    /*数据域*/
  time_t systime;
  struct _ListNode* prev;     /*指向前驱*/
  struct _ListNode* next;     /*指向后继*/
} ListNode;

/*定义链表类型*/
typedef struct  _SList {
  ListNode* head;     /*指向头节点*/
  ListNode* tail;     /*指向尾节点*/
  pthread_mutex_t locker;
  int   size;
} DList;

/*构造链表*/
DList* listInit();

/*摧毁链表*/
void listDestroy(DList* plist);

/*将链表清空*/
void listClear(DList* plist);

/*添加数据到链表的前面*/
ListNode* listPushFront(DList* plist, void* pdata, int32_t len);

/*添加数据到链表的后面*/
ListNode* listPushEnd(DList* plist, void* pdata, int32_t len);

/*将链表中的节点pnode删除*/
ListNode* listDelNote(DList* plist, ListNode* pnode);

/*清理超时节点*/
int listDelTimeOutNote(DList* plist, int timeout_s);

/*检测节点是否存在*/
int listCheckNote(DList* plist, ListNode* pnode);

#include "datalist.c"
#endif

