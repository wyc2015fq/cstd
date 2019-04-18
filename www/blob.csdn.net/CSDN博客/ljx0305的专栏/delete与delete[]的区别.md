# delete与delete[]的区别 - ljx0305的专栏 - CSDN博客
2009年09月20日 19:14:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：730标签：[delete																[汇编																[string																[编程																[object																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=汇编&t=blog)](https://so.csdn.net/so/search/s.do?q=delete&t=blog)
个人分类：[C++																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/380566)
原文：
   operator new 和 operator delete函数有两个重载版本，每个版本支持相关的new表达式和delete表达式：
   void* operator new (size_t);        // allocate an object
   void* operator new [] (size_t);     // allocate an array
   void operator delete (void*);       // free an oject
   void operator delete [] (void*);    // free an array
熟悉C的朋友看到这里可能会很奇怪：
在c中释放内存用free(void *)【注意这里只有一个参数void *】
为什么到了C++里会出现两个！按理说delete 会调用free释放内存的啊？
另外delete []是如何知道删除的对象个数的?
另外一般的教材比如《高质量C++编程指南》都会这么说：
在用delete 释放对象数组时，留意不要丢了符号‘[]’。例如
delete []objects; // 正确的用法
delete objects; // 错误的用法
后者相当于delete objects[0]，漏掉了另外99 个对象
这样的描述当然是错误的，而且会误导观众
为了解决问题，打开vc6，输入以下代码：
class A
...{
private:
   int i;
  string s;
public:
 ~A() ...{  printf("hi"); }
};
void d(A *);
int   main(int   argc,   char*   argv[]) 
...{
 A *p = new A[1];
 d(p);
 return   0; 
}
void d(A *p)
...{
 delete p;
}
运行结果：debug assertion failed!
咦，不是说等同于delete p[0]吗？
为了看看究竟，只好动用那多年以前就忘光了的汇编
经过一番折腾，最后连猜带蒙得出下面的观点：
1 如果对象无析构函数（包括不需要合成析构函数，比如注释掉~A和string s两行代码）
delete会直接调用operator delete并直接调用free释放内存
这个时候的new=new [](仅在数量上有差异)，delete=delete[]
2 如果对象存在析构函数（包括合成析构函数），则【这个才是重点】：
new []返回的地址会后移4个字节，并用那4个存放数组的大小!而new不用后移这四个字节
delete[]根据那个4个字节的值，调用指定次数的析构函数 ，同样delete也不需要那四个字节
结果就是在不恰当的使用delete 和delete []调用free的时候会造成4个字节的错位，最终导致debug assertion failed！
再回到《高质量C++编程指南》：
delete []objects; // 正确的用法
delete objects;  // 错误的用法
后者相当于delete objects[0]，漏掉了另外99 个对象
严格应该这样说：后者相当于仅调用了objects[0]的析构函数，漏掉了调用另外99 个对象的析构函数，并且在调用之后释放内存时导致异常（如果存在析构函数的话），如果对象无析构函数该语句与delete []objects相同
2008-01-09补充
相应的汇编代码说明：
NEW []：
00401298   push        5                                           //申请大小为5(4+1，既4个字节存放数组大小，一个存放对象大小，0字节对象大小为1)
0040129A   call        operator new (00408660)  //获取分配的内存地址，存放入AX
0040129F   add         esp,4
004012A2   mov         dword ptr [ebp-8],eax 
004012A5   cmp         dword ptr [ebp-8],0         //判断是否=0,既==NULL
004012A9   je          main+3Fh (004012bf)
004012AB   mov         eax,dword ptr [ebp-8]
004012AE   mov         dword ptr [eax],1            //!=0，则用前四个自己存放数组大小，测试对象数组大小为1
004012B4   mov         ecx,dword ptr [ebp-8]
004012B7   add         ecx,4                                //地址值加4
004012BA   mov         dword ptr [ebp-14h],ecx
004012BD   jmp         main+46h (004012c6)
004012BF   mov         dword ptr [ebp-14h],0 //分配失败，这是为0
004012C6   mov         edx,dword ptr [ebp-14h] 
004012C9   mov         dword ptr [ebp-4],edx
delete []：
00401388   mov         edx,dword ptr [ebp-4]
0040138B   sub         edx,4   //与delete 相比，先前移指针然后释放空间
0040138E   push        edx
0040138F   call        operator delete (004063e0)
delete:
0040134F   mov         ecx,dword ptr [ebp-4]
00401352   push        ecx
00401353   call        operator delete (00406370)
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/flyingscv/archive/2008/01/07/2029509.aspx](http://blog.csdn.net/flyingscv/archive/2008/01/07/2029509.aspx)
