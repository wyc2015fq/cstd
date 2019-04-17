# C++ 字符数组的输出 - Alex_McAvoy的博客 - CSDN博客





2018年02月21日 23:33:57[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：246








# 【不涉及循环的cout】



    特点：遇'\0'结束



**[cpp]**[view plain](http://blog.csdn.net/u011815404/article/details/79344773#)[copy](http://blog.csdn.net/u011815404/article/details/79344773#)
- **char** str[10]="abc";  
- cout<<str;  

# 【涉及循环的cout】



    特点：与'\0'无关



**[cpp]**[view plain](http://blog.csdn.net/u011815404/article/details/79344773#)[copy](http://blog.csdn.net/u011815404/article/details/79344773#)
- **char** str[10]="abc";  
- **int** i;  
- **for**(i=0;i<10;i++)  
-     cout<<str[i];  

# 【puts()】



    特点：遇'\0'结束，输出完，自动换行



**[cpp]**[view plain](http://blog.csdn.net/u011815404/article/details/79344773#)[copy](http://blog.csdn.net/u011815404/article/details/79344773#)
- **char** str[10]="abc";  
- puts(str);//与cin>>str>>endl;等价



