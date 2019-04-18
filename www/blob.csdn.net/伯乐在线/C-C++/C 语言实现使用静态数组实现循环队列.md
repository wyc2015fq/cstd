# C 语言实现使用静态数组实现循环队列 - 文章 - 伯乐在线
原文出处： [乞力马扎罗的雪](http://blog.csdn.net/chenyufeng1991/article/details/50815760)
队列是一种先进先出的的数据结构，我们同样可以使用数组、链表等来实现。我们可以在队列的尾部进行插入元素，在队列的头部取出元素。普通的队列由于空间利用率不高，所以我们一般都用循环队列。循环队列中最重要的的两个操作就是判断是否为空和是否已满。当head==tail时，表示队列为空。当(tail+1)%MAX_SIZE == head，表示队列已满。
我判断队满的方法：牺牲一个单元来区分对空和队满，入队时少用一个队列单元，相当于浪费一个存储空间。“队头指针的队尾指针的下一位置作为队满的标志”。代码上传至：[https://github.com/chenyufeng1991/Queue_Array](https://github.com/chenyufeng1991/Queue_Array)  。
### （1）进队列


```
//进队列  
void EnQueue(int value){  
  
    //要先判断队列是否已满  
    if ((tail + 1) % QUEUE_SIZE == head) {  
        printf("队列已满，无法从队尾插入元素\n");  
    }else{  
  
        queue[tail] = value;  
        tail = (tail + 1) % QUEUE_SIZE;  
    }  
}
```
### （2）出队列


```
//出队列  
int DeQueue(){  
  
    int temp;  
    if (tail == head) {  
        printf("队列为空，元素无法出队列\n");  
    }else{  
  
        temp = queue[head];  
        head = (head + 1) % QUEUE_SIZE;  
    }  
  
    printf("%d\n",temp);  
    return temp;  
}
```
### （3）判断队列是否为空


```
//判断队列是否为空  
int IsEmpty(){  
    if (head == tail) {  
        printf("队列为空\n");  
        return 1;  
    }  
  
    printf("队列不为空\n");  
    return 0;  
}
```
### （4）判断队列是否已满


```
//判断队列是否已满  
/** 
 *  我这里判断队满的的方法： 
 牺牲一个单元来区分队空和队满，入队时少用一个队列单元。如果数组的大小为Size,那么实际只能存放(Size-1)个元素。（这是比较常用的判满的方式） 
 * 
 */  
int IsFull(){  
  
    if ((tail + 1) % QUEUE_SIZE == head) {  
        printf("队列已满\n");  
        return 1;  
    }  
  
    printf("队列未满\n");  
    return 0;  
}
```
### （5）打印队列元素


```
//打印出队列元素  
void PrintQueue(){  
  
    for (int i = head; i < tail; i++) {  
        printf("%d ",queue[i]);  
    }  
    printf("\n");  
}
```
### （6）测试代码


```
int main(int argc, const char * argv[]) {  
  
    EnQueue(4);EnQueue(1);EnQueue(2);EnQueue(9);EnQueue(8);  
    PrintQueue();  
  
    DeQueue();DeQueue();  
    PrintQueue();  
      
    return 0;  
}
```
