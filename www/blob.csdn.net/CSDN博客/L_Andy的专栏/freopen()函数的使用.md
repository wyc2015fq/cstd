# freopen()函数的使用 - L_Andy的专栏 - CSDN博客

2015年12月24日 19:19:08[卡哥](https://me.csdn.net/L_Andy)阅读数：639
个人分类：[C/C++编程](https://blog.csdn.net/L_Andy/article/category/1925955)


当我们求解acm题目时，通常在设计好算法和程序后，要在调试环境（例如VC等）中运行程序，输入测试数据，当能得到正确运行结果后，才将程序提交到oj中。但由于调试往往不能一次成功，每次运行时，都要重新输入一遍测试数据，对于有大量输入数据的题目，输入数据需要花费大量时间。 
使用freopen函数可以解决测试数据输入问题，避免重复输入，不失为一种简单而有效的解决方法。 函数名：freopen 
声明：FILE *freopen( const char *path, const char *mode, FILE *stream ); 
所在文件： stdio.h 
参数说明： 
path: 文件名，用于存储输入输出的自定义文件名。 
mode: 文件打开的模式。和fopen中的模式（如r-只读, w-写）相同。 
stream: 一个文件，通常使用标准流文件。 
返回值：成功，则返回一个path所指定文件的指针；失败，返回NULL。（一般可以不使用它的返回值） 
功能：实现重定向，把预定义的标准流文件定向到由path指定的文件中。标准流文件具体是指stdin、stdout和stderr。其中stdin是标准输入流，默认为键盘；stdout是标准输出流，默认为屏幕；stderr是标准错误流，一般把屏幕设为默认。 

        下面以在VC下调试“计算a+b”的程序举例。 
C语法： 
#include <stdio.h> 
int main() 
{ 
int a,b; 
freopen("debug\\in.txt","r",stdin); //输入重定向，输入数据将从in.txt文件中读取 
freopen("debug\\out.txt","w",stdout); //输出重定向，输出数据将保存在out.txt文件中 
while(scanf("%d %d",&a,&b)!=EOF) 
printf("%d\n",a+b); 
fclose(stdin);//关闭文件 
fclose(stdout);//关闭文件 
return 0; 
} 

C++语法 
#include <stdio.h> 
#include <iostream.h> 
int main() 
{ 
int a,b; 
freopen("debug\\in.txt","r",stdin); //输入重定向，输入数据将从in.txt文件中读取 
freopen("debug\\out.txt","w",stdout); //输出重定向，输出数据将保存在out.txt文件中 
while(cin>>a>>b) 
cout<<a+b<<endl; // 注意使用endl 
fclose(stdin);//关闭文件 
fclose(stdout);//关闭文件 
return 0; 
} 
        freopen("debug\\in.txt","r",stdin)的作用就是把标准输入流stdin重定向到debug\\in.txt文件中，这样在用scanf或是用cin输入时便不会从标准输入流读取数据,而是从in.txt文件中获取输入。只要把输入数据事先粘贴到in.txt，调试时就方便多了。 
类似的，freopen("debug\\out.txt","w",stdout)的作用就是把stdout重定向到debug\\out.txt文件中，这样输出结果需要打开out.txt文件查看。 

        需要说明的是： 
        1. 在freopen("debug\\in.txt","r",stdin)中，将输入文件in.txt放在文件夹debug中，文件夹debug是在VC中建立工程文件时自动生成的调试文件夹。如果改成freopen("in.txt","r",stdin)，则in.txt文件将放在所建立的工程文件夹下。in.txt文件也可以放在其他的文件夹下，所在路径写正确即可。 
        2. 可以不使用输出重定向，仍然在控制台查看输出。 
        3. 程序调试成功后，提交到oj时不要忘记把与重定向有关的语句删除。</span></span></span>

