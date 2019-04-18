# C++中的字符/字符串的I/O - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 15:51:15[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：477








**cin**


- 最基本的应用，输入一个数字：cin>>a>>b; cout<<a+b<<endl;
- 输入的是一个字符串，那么cin遇到空格、TAB、回车都会结束。char a[20]; cin>>a; cout<<a<<endl; 如果输入ab c, 那么输出ab;

**cin.get()**



- cin.get(字符变量名)  可以接收字符。







- cin.get(字符数组名, 需要接收字符的数目)  可以接收一行字符串，可以接收空格，只要不超过字符数目，都不会停止。







- cin.get()  无参数，用于舍弃输入流中的不需要的字符，或者舍弃回车。





**cin.getline()**

- 可以接收一个字符串，可以接收空格












**getline()**

- 接收一个字符串，可以接收空格，不过需要包含头文件：#include<string>.








**gets()**

- 接收一个字符串，可以接收空格，需要包含头文件：#include<string>.










**getchar()**

- 接收一个字符，需要包含头文件：#include<string>.












