# C++ 队列（queue）堆栈（stack）实现基础 - 在思索中前行！ - CSDN博客





2014年11月16日 20:22:16[_Tham](https://me.csdn.net/txl16211)阅读数：2117标签：[C++队列																[C++堆栈																[queue																[stack](https://so.csdn.net/so/search/s.do?q=stack&t=blog)
个人分类：[C/C++																[数据结构](https://blog.csdn.net/txl16211/article/category/2409063)](https://blog.csdn.net/txl16211/article/category/2284665)





## Queue


在C++中只要#include<queue>即可使用队列类，其中在面试或笔试中常用的成员函数如下（按照最常用到不常用的顺序）


1. push


2. pop


3. size


4. empty


5. front


6. back


接下来逐一举例说明：


1. push


队列中由于是先进先出，push即在队尾插入一个元素，如：

```
1 queue<string> q;
2 q.push("Hello World!");
3 q.push("China");
4 cout<<q.front()<<endl;
```


可以输出：Hello World！


2. pop


将队列中最靠前位置的元素拿掉，是没有返回值的void函数。如：

```
1 queue<string> q;
2 q.push("Hello World!");
3 q.push("China");
4 q.pop();
5 cout<<q.front()<<endl;
```


可以输出：China


原因是Hello World！已经被除掉了。


3. size


返回队列中元素的个数，返回值类型为unsigned int。如：

```
queue<string> q;
cout<<q.size()<<endl;
q.push("Hello World!");
q.push("China");
cout<<q.size()<<endl;
```


输出两行，分别为0和2，即队列中元素的个数。


4. empty


判断队列是否为空的，如果为空则返回true。如：

```
1 queue<string> q;
2 cout<<q.empty()<<endl;
3 q.push("Hello World!");
4 q.push("China");
5 cout<<q.empty()<<endl;
```


输出为两行，分别是1和0。因为一开始队列是空的，后来插入了两个元素。


5. front


返回值为队列中的第一个元素，也就是最早、最先进入队列的元素。注意这里只是返回最早进入的元素，并没有把它剔除出队列。如：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

1 queue<string> q;
2 q.push("Hello World!");
3 q.push("China");
4 cout<<q.front()<<endl;
5 q.pop();
6 cout<<q.front()<<endl;

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


输出值为两行，分别是Hello World！和China。只有在使用了pop以后，队列中的最早进入元素才会被剔除。


6. back


返回队列中最后一个元素，也就是最晚进去的元素。如：

```
1 queue<string> q;
2 q.push("Hello World!");
3 q.push("China");
4 cout<<q.back()<<endl;
```


输出值为China，因为它是最后进去的。这里back仅仅是返回最后一个元素，也并没有将该元素从队列剔除掉。


其他的方法不是很常用，就不再研究了。


接下来我们使用链表，自己将queue类写出来，将其所有方法都实现。代码都是自己写的，最后随便写了点main函数小测一下，没有发现问题，如果有不足还望能指正。如下：



```cpp
#include<iostream>
#include<string>
using namespace std;

template <typename T>
struct Node{
    T value;
    Node<T> *next;
    Node<T>(){next = NULL;}
};
 
template <typename T>
class MyQueue{
private:
    unsigned int num;
    Node<T> *first;
    Node<T> *last;
    
public:
    MyQueue();
    ~MyQueue();
    unsigned int size();
    void push(T element);
    void pop();
    bool empty();
    T back();
    T front();
};

template <typename T>
MyQueue<T>::MyQueue(){
    num = 0;
    first = NULL;
    last = NULL;
}

template <typename T>
MyQueue<T>::~MyQueue(){
    while(!empty()){
        pop();
    }
}

template <typename T>
unsigned int MyQueue<T>::size(){
    return num;
}

template <typename T>
bool MyQueue<T>::empty(){
    return (0==num);
}

template <typename T>
void MyQueue<T>::push(T element){
    Node<T> *temp = new Node<T>;
    temp->next = NULL;
    temp->value = element;
    if (0 == this->num){
        first = temp;
        last = temp;
    }else{
        last->next = temp;
        last = temp;
    }
    (this->num)++;
}

template <typename T>
void MyQueue<T>::pop(){
    if (0==this->num){
        cout<<"No elements in the queue!"<<endl;
    }else if(1 == this->num){
        delete first;
        first = NULL;
        last = NULL;
        this->num = 0;
    }else{
        Node<T> *temp = first;
        first = first->next;
        delete temp;
        (this->num)--;
    }
}

template <typename T>
T MyQueue<T>::back(){
    if (0==this->num){
        cout<<"No elements in the queue!"<<endl;
        return NULL;
    }
    return last->value;
}

template <typename T>
T MyQueue<T>::front(){
    if(0== this->num){
        cout<<"No elements in the queue!"<<endl;
        return NULL;
    }
    return first->value;
}


int main(){
    MyQueue<string> q;
    q.push("Hello world");
    q.push("China");
    cout<<q.front()<<endl;
    cout<<q.size()<<endl;
    cout<<q.back()<<endl;
    q.pop();
    cout<<q.empty()<<endl;
    cout<<q.back()<<endl;
    cout<<q.front()<<endl;
    q.pop();
    cout<<q.size()<<endl;
    cout<<q.empty()<<endl;
    system("pause");
    return 0;
}

队列实现
```

## Stack


## [进栈出栈顺序问题](http://www.cnblogs.com/xuning/p/3368264.html)


题目：两个数组，长度相同，都为n，两个数组分别为inseq和outseq，求出如果以inseq为入栈顺序，那么outseq可不可能是它的一个出栈顺序，可能则返回true


样例：


inseq = {1,2,3,4,5}  outseq={5,4,3,2,1} 返回true；


inseq = {1,2,3,4,5}  outseq={4,3,2,1,5}，返回true；


inseq = {1,2,3,4,5} outseq={2,3,5,1,4}，返回false。

解题思路：模拟整个过程，挨个把inseq的数据放入栈中。直到栈顶元素和出栈序列outseq所指的元素相同，则一直出栈，并将outseq指针后移，直到栈顶元素和outseq指针所指的元素不一样了，则又开始进栈。每次循环中，要么进栈，要么出栈，总要有一个动作在执行，如果既没出栈也没进栈，一定出了什么问题，直接跳出循环，最后进行判断。代码如下：

```cpp
#include <iostream>
#include <stack>
using namespace std;

bool islegal(int *inseq, int *outseq, int n){
    if(n==0) return true;
    if(n==1) return inseq[0]==outseq[0];
    stack<int> st;
    
    int i=0,j=0;
    
    bool flag = false; //用于确定每一个最外层while循环中有操作在执行，没有操作可以执行，则必然有违反的情况
    while(j<n){
        if((st.empty() || (st.top()!= outseq[j])) && i<n){//如果栈为空或者第一个元素不等于出栈序列第一个元素,且i<n，则进栈
            st.push(inseq[i]);
            i++;
            flag = true;
        }
        if(!st.empty() && st.top()== outseq[j] ){//如果栈不为空，并且第一个元素等于出栈序列第一个元素，则出栈
            st.pop();
            j++;
            flag = true;
        }
        if (!flag)
            break;
        else
            flag = false;
    }
    if(st.empty() && j==n && i==n)
        return true;
    
    return false; 
}

int main(){
    int a[] = {1,2,3,4,5};
    int b[] = {4,3,5,2,1};
    cout<<islegal(a,b,5);
    system("pause");
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=queue&t=blog)](https://so.csdn.net/so/search/s.do?q=C++堆栈&t=blog)](https://so.csdn.net/so/search/s.do?q=C++队列&t=blog)




