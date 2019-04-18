# PAT注意点梳理 - 心纯净，行致远 - CSDN博客





2018年11月26日 21:18:58[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：144








1、调试

自己在写程序的时候一定要学会用调试的，如果有特殊的需要，需要程序暂停的，可以写上

```cpp
getchar();//等待获取一个字符，此时程序会暂停等待
```

或

```cpp
system("pause");
```

2、debug时的输入输出

debug时数据可以是手动输入的，在数据多的时候也可以粘贴进去，右键就可以粘贴。如果这些都麻烦或不支持，可以使用读入数据的方式实现数据输入。

从文件中输入的话，可以用C类型的freopen，也可以用C++类型的fstream，不过前者更简单，而且提交的时候只用注释一句话即可，所以我个人更推荐前者。

```cpp
freopen("F://Temp/input.txt","r",stdin);//输入重定向，从指定文件中读取，当然了，你要在相应的目录下新建txt并且写入测试数据的输入，保存。如果是在和debug下的目录则可以不用绝对路径。
freopen("F://Temp/input.txt","w",stdout);//输出重定向
```

这里注意一下的是记得要把windows下的“\”地址符号改为“//”和“/”的。

再解释一下freopen函数：（这是应该是以前百度到的，没有记出处，凑合用上吧哈）

使用freopen函数可以解决测试数据输入问题，避免重复输入，不失为一种简单而有效的解决方法。

**函数名：freopen声明：FILE *freopen( const char *path, const char *mode, FILE *stream );所在文件： stdio.h**

参数说明：

path: 文件名，用于存储输入输出的自定义文件名。

mode: 文件打开的模式。和fopen中的模式（如r-只读, w-写）相同。

stream: 一个文件，通常使用标准流文件。

返回值：成功，则返回一个path所指定文件的指针；失败，返回NULL。（一般可以不使用它的返回值）

功能：实现重定向，把预定义的标准流文件定向到由path指定的文件中。标准流文件具体是指stdin、stdout和stderr。其中stdin是标准输入流，默认为键盘；stdout是标准输出流，默认为屏幕；stderr是标准错误流，一般把屏幕设为默认。

例如：

```cpp
//C语法： 
#include <stdio.h> 
int main() 
{ 
int a,b; 
freopen("D://2018年//PAT考试//Progam//in.txt","r",stdin); //输入重定向，输入数据将从in.txt文件中读取 
freopen("D://2018年//PAT考试//Progam//out.txt","w",stdout); //输出重定向，输出数据将保存在out.txt文件中 
while(scanf("%d %d",&a,&b)!=EOF) 
printf("%d\n",a+b); 
fclose(stdin);//关闭文件 
fclose(stdout);//关闭文件 
return 0; 
} 
 
//C++语法 
#include <stdio.h> 
#include <iostream.h> 
int main() 
{ 
int a,b; 
freopen("D://2018年//PAT考试//Progam//in.txt","r",stdin); //输入重定向，输入数据将从in.txt文件中读取 
freopen("D://2018年//PAT考试//Progam//out.txt","w",stdout); //输出重定向，输出数据将保存在out.txt文件中 
while(cin>>a>>b) 
cout<<a+b<<endl; // 注意使用endl 
fclose(stdin);//关闭文件 
fclose(stdout);//关闭文件 
return 0; 
}
```

freopen("debug\\in.txt","r",stdin)的作用就是把标准输入流stdin重定向到debug\\in.txt文件中，这样在用scanf或是用cin输入时便不会从标准输入流读取数据,而是从in.txt文件中获取输入。只要把输入数据事先粘贴到in.txt，调试时就方便多了。

类似的，freopen("debug\\out.txt","w",stdout)的作用就是把stdout重定向到debug\\out.txt文件中，这样输出结果需要打开out.txt文件查看。

需要说明的是：

1. 在freopen("debug\\in.txt","r",stdin)中，将输入文件in.txt放在文件夹debug中，文件夹debug是在VC中建立工程文件时自动生成的调试文件夹。如果改成freopen("in.txt","r",stdin)，则in.txt文件将放在所建立的工程文件夹下。in.txt文件也可以放在其他的文件夹下，所在路径写正确即可。

 2. 可以不使用输出重定向，仍然在控制台查看输出。

 3. 程序调试成功后，提交到oj时不要忘记把与重定向有关的语句删除。



---------------------

作者：陈小旭

来源：CSDN

原文：https://blog.csdn.net/Apie_CZX/article/details/45200921

版权声明：本文为博主原创文章，转载请附上博文链接！







