# new/delete与malloc/free的区别 - Ms.Fox的专栏 - CSDN博客
2014年10月21日 14:26:42[lulu-lu](https://me.csdn.net/smbluesky)阅读数：286
个人分类：[C++：注意事项](https://blog.csdn.net/smbluesky/article/category/2641013)
**new/delete与malloc/free的区别**
1、new/delete在管理内存的同时会调用类的构造函数和析构函数。
      malloc/free仅仅实现了内存的分配和释放，没有调用类的构造函数和析构函数。
2、new/delete是C++语言的运算符关键字。
       malloc/free是C/C++语言的标准库函数。——在使用的时候需要头文件库函数支持。
3、通过new创建的对象具有类型，而malloc的返回值为void*，需要进行强制类型转换。
  ClassA   *a= new ClassA   ;
   ClassA   *a=**( ClassA   *a) **malloc(sizeof(  ClassA ));
4、new申请内存失败时，会调用new_handler处理函数。
   而malloc申请内存失败时，仅仅返回NULL，不会进行任何善后处理。
new的三种形态
  ClassA   *a= new ClassA   ;
这个语句，完成了三个操作：
       1、通过operator new 申请内存
        2、使用placement new调用构造函数（简单类型如int等 ，忽略此步）
       3、返回内存指针
## **注意点：**
在编程时，new的表达式中使用[]，则在delete表达式中也使用[];
在new的表达式中没有使用[]，则在delete表达式中一定不要使用[]。
但是在遇到typedef类型对象时，要清楚知道该用哪一种delete。
比如：typedef int  arriMatrix[4];——————定义了int 型大小为4的一个数组的别名 ：arriMatrix；
int * parriMatrix=new arriMatrix;
delelte []arriMatrix;

