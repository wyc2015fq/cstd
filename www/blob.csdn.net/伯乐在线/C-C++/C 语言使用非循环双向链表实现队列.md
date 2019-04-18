# C 语言使用非循环双向链表实现队列 - 文章 - 伯乐在线
原文出处： [乞力马扎罗的雪](http://blog.csdn.net/chenyufeng1991/article/details/50839670)
在前面两篇博客中，我分别使用了[静态数组](http://blog.jobbole.com/106212/)和[动态数组](http://blog.jobbole.com/106216/)来模拟循环队列。但是线性表中和队列最神似的莫过于链表了。我在前面也使用了大量的篇幅来讲述了链表的各种操作。今天我们使用一种比较特殊的链表——非循环双向链表来实现队列。首先这里的说明的是构建的是普通的队列，而不是循环队列。当我们使用数组的时候创建循环队列是为了节省存储空间，而来到链表中时，每一个节点都是动态申请和释放的，不会造成空间的浪费，所以就不需要采用循环队列了。第二，大家在很多书上看到的是使用单链表实现队列，我这里将会使用带头结点尾结点的非循环双链表实现，虽然多维护了两个节点和指针域，但是在链表头尾进行插入删除的时候不需要遍历链表了，队列操作变得非常的方便。真正实现了只在头尾操作。代码上传至[https://github.com/chenyufeng1991/Queue_LinkedList](https://github.com/chenyufeng1991/Queue_LinkedList) 。
核心代码如下：
### （1）初始化队列


```
//初始化带头结点和尾结点的非循环双向链表  
void InitialQueue(Queue **pHead,Queue **pTail){  
  
    *pHead = (Queue *)malloc(sizeof(Queue));  
    *pTail = (Queue *)malloc(sizeof(Queue));  
  
    if (*pHead == NULL || *pTail == NULL) {  
        printf("%s函数执行，内存分配失败，初始化双链表失败\n",__FUNCTION__);  
    }else{  
        //这个里面是关键，也是判空的重要条件  
        (*pHead)->next = NULL;  
        (*pTail)->prior = NULL;  
  
        //链表为空的时候把头结点和尾结点连起来  
        (*pHead)->prior = *pTail;  
        (*pTail)->next = *pHead;  
  
        printf("%s函数执行，带头结点和尾节点的双向非循环链表初始化成功\n",__FUNCTION__);  
    }  
}
```
### （2）入队，在尾结点插入元素


```
//入队,也就是在链表的尾部插入节点  
void EnQueue(Queue *head,Queue *tail,int x){  
  
    Queue *pInsert;  
    pInsert = (Queue *)malloc(sizeof(Queue));  
    memset(pInsert, 0, sizeof(Queue));  
    pInsert->next = NULL;  
    pInsert->prior = NULL;  
    pInsert->element = x;  
  
    tail->next->prior = pInsert;  
    pInsert->next = tail->next;  
    tail->next = pInsert;  
    pInsert->prior = tail;  
}
```
### （3）出队，在头结点处删除节点


```
//出队,在队列头部删除元素  
void DeQueue(Queue *head,Queue *tail){  
  
    if (IsEmpty(head,tail)) {  
        printf("队列为空，出队列失败\n");  
    }else{  
  
        Queue *pFreeNode;  
        pFreeNode = head->prior;  
        head->prior->prior->next = head;  
        head->prior = head->prior->prior;  
  
        free(pFreeNode);  
        pFreeNode = NULL;  
    }  
}
```
### （4）打印所有节点


```
//打印出从队列头部到尾部的所有元素  
void PrintQueue(Queue *head,Queue *tail){  
  
    Queue *pMove;  
    pMove = head->prior;  
    printf("当前队列中的元素为(从头部开始):");  
    while (pMove != tail) {  
        printf("%d ",pMove->element);  
        pMove = pMove->prior;  
    }  
    printf("\n");  
}
```
### （5）判断队列是否为空


```
//判断队列是否为空,为空返回1，否则返回0  
int IsEmpty(Queue *head,Queue *tail){  
  
    if (head->prior == tail) {  
        return 1;  
    }  
  
    return 0;  
}
```
### （6）测试代码


```
int main(int argc, const char * argv[]) {  
  
    Queue *pHead;//头结点  
    Queue *pTail;//尾结点  
  
    InitialQueue(&pHead, &pTail);  
  
    EnQueue(pHead, pTail, 2);EnQueue(pHead, pTail, 1);  
    EnQueue(pHead, pTail, 9);EnQueue(pHead, pTail, 3);EnQueue(pHead, pTail, 4);  
    PrintQueue(pHead, pTail);  
  
    DeQueue(pHead,pTail);DeQueue(pHead,pTail);DeQueue(pHead,pTail);  
    PrintQueue(pHead, pTail);  
  
    return 0;  
}
```
