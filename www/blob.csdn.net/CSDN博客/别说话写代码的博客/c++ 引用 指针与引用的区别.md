# c++ 引用 指针与引用的区别 - 别说话写代码的博客 - CSDN博客





2017年07月21日 09:29:46[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：218
个人分类：[c/c++](https://blog.csdn.net/qq_21997625/article/category/6988647)









引用：即某一变量的别名，对引用的操作与对变量直接操作完全一样  int a;  int &b=a;




1.&不是取地址，是标识作用

2.声明引用时必须进行初始化

3.类型标识符是指目标变量的类型

4.引用变量绑定后，不能再绑定到其他变量




swap(int &x,int &y) //引用作为函数参数

1.引用传递，效果与传递指针相同。被调函数形参成为主调函数实参变量的别名

2.引用传递函数参数，内存中并没有产生实参副本，他是直接对实参操作。而使用一般变量传递函数参数，当发生函数调用，需要给形参分配存储单元

3.指针作为函数参数虽然效果与引用传参相同，但也需要给形参分配空间。且在主函数调用点初，必须以变量地址作为实参

4.若既要利用引用传递提高效率，又要保护传递给函数的数据不在函数中被改变，应该用常引用。

     int a=10; const int &b=a;




//引用作为函数返回值

1.定义函数时，在函数名前加&

2.用引用返回一个函数值的好处：在内存中不产生被返回值的副本







规则：

1.不能返回局部变量的引用

2.不能返回函数内部new分配内存的引用

     例：被函数返回的引用只是作为一个临时变量出现，没有被赋予一个实际的变量，这个引用所指的空间（new分配）就无法释放，造成memory leak





#include <iostream>

using std::cout;

using std::endl;




void swap1(int &a,int &b)//引用传递，形参不会产生新的内存空间

{

     int temp=a;

     a=b;

     b=temp;

}




void swap(int a,int b)//值传递，本质：复制。形参在栈中重新生成内存空间

     //实参与形参无关，调用后并不能改变实参的值

{

     int temp=a;

     a=b;

     b=temp;

}




void swap(int *a,int *b)//地址传递，本质：复制。在栈中生成新的指针内存空间

{

     int temp=*a;

     *a=*b;

     *b=temp;

}




void refer(void)

{

     int a=10;

     int &b = a;//b是a的引用（别名），一旦绑定不可更改指向

     //类似于指针常量。操作引用相当于操作变量本身

     //int &b;//错误，引用必须进行初始化

     //

     cout << "a= " << a <<endl;

     cout << "b= " << b <<endl;




     b=8;//用引用改编其值




     cout << "a= " << a <<endl;

     cout << "b= " << b <<endl;




     int c=20;

     b=c;//通过引用改编其值




     cout << "a= " << a <<endl;

     cout << "b= " << b <<endl;

}




int main()

{

     refer();




     int a=3,b=5;

     cout << "a=" <<a<<",b=" <<b<<endl;

     swap(&a,&b);




     cout << "a=" <<a<<",b=" <<b<<endl;

     swap1(a,b);

     cout << "a=" <<a<<",b=" <<b<<endl;

     return 0;

}


运行结果






![](https://img-blog.csdn.net/20170721092958299?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)












#include <iostream>

using std::cout;

using std::endl;




int arr[5]={0,1,2,3,4};

//引用作为函数返回值，不会进行复制。要返回的变量生命周期一定要大于函数

//不然函数结束，空间被释放，无法返回一个引用

//




int & func(int i)

{

     return arr[i];//返回一个引用

}




//返回值的参数就是一个int类型，此时返回时会进行复制

int func2()

{

     int a=3;

     return a;

}




//不要返回一个局部变量的引用

int & func3()//会有警告

{

     int a=3;//局部变量，当函数执行结束，a生命周期结束，它已经被销毁

     return a;

}




//不要轻易返回一个堆空间变量的引用，除非你已经做好了内存回收的策略

int & func5()

{

     int *p=new int(1);//申请堆空间

     return *p;

}




int *func4()//返回的空间已经释放，有警告

{

     int a=3;

     return &a;//返回地址

}




int test()

{

     func(0)=10;//func(0)返回的是arr[0]的引用，将arr[0]的值改变为10

     cout<<"arr[0]= " << arr[0] <<endl;




     return 0;

}




int test1()//段错误

{

     cout << func3() << endl;

}




void test2()

{

     int &ref=func5();//新申请堆空间内容的引用

     cout << "ref = " << ref << endl;




     delete(&ref);//释放内存

}//若不释放内存情况如下




void test3()

{

     int a=3,b=4;

     int c=a+func5()+b;//

     cout << "c = " <<c << endl;

}//由于没有释放内存，导致内存泄漏




int main(void)

{

     test();

     //test1();//段错误

     test2();

     test3();




     return 0;

}





运行结果：




![](https://img-blog.csdn.net/20170721093006756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjE5OTc2MjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)









