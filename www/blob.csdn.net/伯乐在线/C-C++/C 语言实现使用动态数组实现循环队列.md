# C 语言实现使用动态数组实现循环队列 - 文章 - 伯乐在线
原文出处： [乞力马扎罗的雪](http://blog.csdn.net/chenyufeng1991/article/details/50816920)
我在上一篇博客《[C语言实现使用静态数组实现循环队列](http://blog.jobbole.com/106212/)》中实现了使用静态数组来模拟队列的操作。由于数组的大小已经被指定，无法动态的扩展。所以在这篇博客中，我换成动态数组来实现。动态数组可以不断开辟内存空间，只是会在数组的初始化时有所不同，其他对数组的操作都是一样的。代码上传至 [https://github.com/chenyufeng1991/Queue_DynamicArray](https://github.com/chenyufeng1991/Queue_DynamicArray) 。
### （1）声明变量


```
static int *queue;//声明数组
static int maxSize;//数组大小
static int head;//指向头部
static int tail;//指向尾部
```
### （2）初始化队列


```
//初始化队列  
void InitQueue(int size){  
  
    maxSize = size;  
    queue = (int *)malloc(size * sizeof(int));  
    head = 0;  
    tail = 0;  
}
```
### （3）进队列


```
//进队列  
void EnQueue(int value){  
    //先判断是否已满  
    if ((tail + 1) % maxSize == head) {  
        printf("队列已满，无法插入\n");  
    }else{  
        //未满  
        queue[tail] = value;  
        tail = (tail + 1) % maxSize;  
    }  
}
```
### （4）出队列


```
//出队列  
int DeQueue(){  
  
    int temp;  
    //先判断是否为空  
    if (head == tail) {  
        printf("队列为空，出列失败\n");  
    }else{  
  
        temp = head;  
        head = (head + 1) % maxSize;  
    }  
  
    return temp;  
}
```
### （5）判断队列是否为空


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
### （6）判断队列是否已满


```
//判断队列是否已满  
int IsFull(){  
  
    if ((tail + 1)% maxSize == head) {  
        printf("队列已满\n");  
        return 1;  
    }  
  
    printf("队列未满\n");  
    return 0;  
}
```
### （7）打印队列元素


```
//打印队列元素  
void PrintQueue(){  
    for (int i = head; i < tail; i++) {  
        printf("%d ",queue[i]);  
    }  
    printf("\n");  
}
```
### （8）测试代码


```
int main(int argc, const char * argv[]) {  
  
    InitQueue(5);  
  
    EnQueue(2);EnQueue(4);EnQueue(8);EnQueue(1);EnQueue(5);  
    PrintQueue();  
  
    DeQueue();DeQueue();DeQueue();  
    PrintQueue();  
  
    IsEmpty();  
    IsFull();  
  
    return 0;  
}
```
