# C++变长数组 - fanyun的博客 - CSDN博客
2017年08月31日 22:09:59[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3251
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
什么是变长数组？   
变长数组是指在编译时不能确定数组长度，程序在运行时需要动态分配内存空间的数组。
1.普通方式实现一维变长数组：
```cpp
#include<iostream>  
 using   namespace   std;    
 int   main()  
 {  
   int   len;  
   cin>>len;  
   //用指针p指向new动态分配的长度为len*sizeof(int)的内存空间  
   int   *p=new   int[len];  
/*注意int   *p=new  int[len];这一句，你不能这样做：  
 int   p[len];  
  C++编译器会报错说len的大小不能确定，因为用这种形式声明数组，数组的大小需要在编译时确定。而且这样也不行：  
 int   p[]=new   int[len];  
  编译器会说不能把int*型转化为int[]型，因为用new开辟了一段内存空间后会返回这段内存的首地址，所以要把这个地址赋给一个指针，所以要用： 
int  *p=new   int[len]*/
........... delete[] p;//注意要注销指针p，使程序释放用new开辟的内存空间
}
```
2.使用C++标准模版库（STL）中的vector（向量）也可以实现变长数组：
```cpp
#include<iostream>  
 #include<vector>  
 using  namespace   std;  
   
 int  main()  
 {  
   int  len;  
   cin>>len;  
   vector<int>   array(len);//声明变长数组  
   
   for(int  i=0;i<len;i++)  
   {  
     array[i]=i;  
    cout<<array[i]<<"\t";  
   }  
   return  0;  
 }
```
使用完vector后调用~vector()析构函数释放内存。
3.用vector实现变长二维数组
```cpp
#include <iostream>  
 #include  <vector>  
 #include  <iomanip>  
 using  namespace   std;  
 int  main()  
 {  
   int  i,  
           j, 
           m,  //行数  
           n;  //列数  
   
   cout  <<   "input  value  for   m,n:";  
   cin>>m>>n;  
      
   //注意下面这一行：vector<int后两个">"之间要有空格！否则会被认为是重载">>"。  
   vector<vector<int>  >  vecInt(m,   vector<int>(n));      
   for  (i   =  0;  i   <  m;  i++)  
     for  (j   =  0;  j   <  n;  j++)  
       vecInt[i][j]   =  i*j;    
         
    for (i   =   0; i   <   m; i++)  
   {  
     for  (j   =  0;  j   <  n;  j++)  
      cout<<setw(5)<<vecInt[i][j]<<":"<<setw(9)<<&vecInt[i][j];  
     cout<<endl;  
   }     
   return  0;  
 }
```
4.实现变长n维数组
首先介绍一下双指针方法，在这里双指针就是指像指针的指针，比如你可以这样声明一个数组：  
```cpp
int  **p   =  new  int*[num1]; 而对每一个*p（一共num1个*p）申请一组内存空间：  
 for(int  i=0;   i<num1;  ++i) 
   p[i]  =   new  int[num2]; 
其中，num1是行数，num2是数组的列数。
#include  <iostream>  
 #include  <iomanip>  
  using  namespace  std;  
   
 int  main()  
 {  
   int  num1,//行数  
           num2;//列数    
   cout<<"Please  enter  the   number  for  row   and  column:  "<<endl; 
   cin  >>   num1  >>  num2;  
   //为二维数组开辟空间  
   int  **p   =   new  int*[num1];  
   for(int  i=0;   i<num1;  ++i) 
     p[i]  =   new  int[num2]; 
   
   for(int  j=0;j<num1;j++)  
   {  
     for(int  k=0;k<num2;k++)  
     { 
       p[j][k]=(j+1)*(k+1);  
       cout<<setw(6)<<p[j][k]<<':'<<setw(8)<<&p[j][k];  
     } 
     cout<<endl;  
   }   
   //释放二维数组占用的空间  
   for(int  m=0;m<num1;m++)  
     delete[]  p[m];  
   delete[]  p;  
   
   return  0;  
 }
```
   由于数组空间是动态分配的，数组行之间的地址空间是不连续的，因为不同行的数组元素的地址空间是用不同的new来分配的。而每一行之中列之间的地址空间是连续的。
