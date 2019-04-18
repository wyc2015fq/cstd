# C中的setjmp与longjmp - xcw0754 - 博客园
# [C中的setjmp与longjmp](https://www.cnblogs.com/xcw0754/p/4966152.html)
setjmp与longjmp是属于C语言中的，当然，C++也会有这两个函数了。他们的原型如下：
***int setjmp( jmp_buf env );***
**作用：第一次调佣时，将寄存器的当前状态信息全部存入到env中，并返回0。如果在某处调用了longjmp(env,x)，且x!=0，则setjmp的返回值将设为x。而若x==0，则setjmp返回1**。
**void longjmp( jmp_buf env,int value );**
**作用：重新存储当前寄存器的状态信息，并将setjmp的返回值设为value。**
　　简单概括之，如果你有月光宝盒，那么需要将月光宝盒在某处定一个时间点，比如2015年11月11日11点11分，后来，我在2016年感觉不爽，就直接使用月光宝盒回到2015年11月11日11点11分，当时所有的东西仍然没有变化，仅仅只有我知道我穿越了。那么setjmp就是用于设置时间点，会将必须的东西记录下来，保存在一个jmp_buf类型的变量中，此后，如果有longjmp想回到这个时间点，就可以直接跳回去。值得注意的是，这样不就一直在无限循环吗？当然还是有变化的，第一次调用setjmp时，返回的是0。当调用longjmp跳回去时，setjmp的返回值变成了非0值，这样就可以区分开来了。
　　下面是个例子：
```
1 void test()
2 {
3     jmp_buf jb;
4     int ret=setjmp(jb);    //设置返回点,返回0.
5     if(ret!=0)    cout<<"123"<<endl;    //这仅被输出1次。
6     if(ret==0)    longjmp(jb,7);//回到jb处，并将ret设为非零。
7 }
```
　　那么setjmp与longjmp在C++中的适用范围是什么？setjmp与longjmp如果跳出了当前函数的话，当前函数中声明的对象可是不会被析构的（不会调用析构函数），那么就存在了一定的危险，比如对象里还有动态申请的内存怎么办，然而C中就不存在这个问题了。并不是说不能用，只是必须了解它的利弊，才能更好地使用他。
　　例子：
```
1 class Rainbow
 2 {
 3 public:
 4     Rainbow() {cout<<"Rainbow()"<<endl;}
 5     ~Rainbow() {cout<<"~Rainbow()"<<endl;}
 6 };
 7 jmp_buf jmpBuf;
 8 void Summer()
 9 {
10     Rainbow rb;
11     for(int i=3;i>0;--i)
12         cout<<"Happy Chinese Valentine's Day!"<<endl;
13     longjmp(jmpBuf,2);
14     
15     //下面这句被忽略
16     cout<<"Oh no.I am being ignore."<<endl;
17 }
18 
19 int main()  
20 {  
21     test();
22     if(!setjmp(jmpBuf))
23     {
24         cout<<"setjmp() return a zero value!"<<endl;
25         Summer();
26     }
27     else
28     {
29         cout<<"setjmp() get a non-zero value!"<<endl;
30     }
31     return 0;  
32 }
```
　　经测试，G++下并不会调用该析构函数。当然，你需要考虑编译器的智能程度，可能帮你做优化了。
　　这种方法看起来与goto相似，但是是有区别的，区别如下：
　　（1）goto语句不能跳出C语言当前的函数。
　　（2）用longjmp只能跳回曾经到过的地方。在执行setjmp的地方仍留有一个过程活动记录。从这个角度上讲，longjmp更象是“从何处来”，而不是“要往哪去”。另外，longjmp接受一个额外的整形参数并返回它的值，这可以知道是由longjmp转移到这里的还是从上一条语句执行后自然执行到这里的。
　　参考：http://blog.csdn.net/cscmaker/article/details/7584433

