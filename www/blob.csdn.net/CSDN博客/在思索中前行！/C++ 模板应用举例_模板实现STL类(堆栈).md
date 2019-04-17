# C++ 模板应用举例_模板实现STL类(堆栈) - 在思索中前行！ - CSDN博客





2015年03月30日 22:37:00[_Tham](https://me.csdn.net/txl16211)阅读数：1267标签：[堆栈																[模板实现堆栈](https://so.csdn.net/so/search/s.do?q=模板实现堆栈&t=blog)](https://so.csdn.net/so/search/s.do?q=堆栈&t=blog)
个人分类：[数据结构																[C/C++](https://blog.csdn.net/txl16211/article/category/2284665)](https://blog.csdn.net/txl16211/article/category/2409063)







```cpp
//stack集合类是一个简单的堆栈的实现。  
    //这里有两个模板参数，T和size，指定堆栈中的元素类型和堆栈中项数的最大值。  
    //push 和 pop成员函数添加和删除堆栈中的项，并在堆栈底部增加。  
    #include <iostream>  
    #include <algorithm>  
    #include <string>  
    using namespace std;  
    template <class T,int size> //出错点  
    class stack  
    {  
    private:  
        T items[size];  
        int top;  
        const int MaxSize;  
    public:  
        stack():MaxSize(size)  
        {  
            top=-1;//指向栈底  
        }  
        bool IsFull();  
        bool IsEmpty();  
        void push(const T item);  
        T pop();  
    };  
      
    template <class T,int size>  
    bool stack<T,size>::IsFull()  
    {  
        if(top==MaxSize-1)  
        {  
            return true;  
        }  
        else  
        {  
            return false;  
        }  
    }  
      
    template <class T,int size>  
    bool stack<T,size>::IsEmpty()  
    {  
        if (top==-1)  
        {  
              
            return true;  
        }  
        else  
        {  
            return false;  
        }  
    }  
      
    template <class T,int size>//出错点  
    void stack<T,size>::push(const T item)  
    {  
        if (IsFull())  
        {  
            cout<<"stack is full"<<endl;  
            return;  
        }  
        else  
        {  
            items[++top]=item;  
        }  
    }  
      
    template <class T,int size>  
    T stack<T,size>::pop()  
    {  
        if (!IsEmpty())  
        {  
            return items[top--];  
        }  
        else  
        {  
            cout<<"栈空"<<endl;  
            return 0;  
        }  
    }  
      
    void main()  
    {  
        //stack<string,5> s;//栈中存放字符串  
        stack<int,5> s;   //栈中存放整型数据  
        while(!s.IsFull())  
        {  
            //s.push("123");//字符串入栈  
            s.push(1);//整型数据入栈  
        }  
        while(!s.IsEmpty())  
        {  
            cout<<s.pop()<<endl;  
        }  
        system("pause");  
    }
```






