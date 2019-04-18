# 利用string 字符串拷贝 - wishchinYang的专栏 - CSDN博客
2013年07月17日 11:03:11[wishchin](https://me.csdn.net/wishchin)阅读数：774
 序言：对于laws的代码，完全从Matlab中转来。其中用到了字符串复制和对比的函数。
C++要求：
           输入字符串，根据字符串，来确定选择数组，用于下一过程
MatLab代码：
 （1）.文件calLaws.m
```cpp
function [y,h_v,h_h]=calLaws(x,id,LocalEnergy)
   for dim=1:2
       if dim==1
          FilterId=upper(id(1:2)); % Input argument "id" is undefined.
    else
         FilterId=upper(id(3:4));
    end
    if strcmp(FilterId,'L5'), h(dim,1:5)=[ 1  4  6  4  1];
        elseif strcmp(FilterId,'E5'), h(dim,1:5)=[-1 -2  0  2  1];
        elseif strcmp(FilterId,'S5'), h(dim,1:5)=[-1  0  2  0 -1];
        elseif strcmp(FilterId,'W5'), h(dim,1:5)=[-1  2  0 -2  1];
        elseif strcmp(FilterId,'R5'), h(dim,1:5)=[ 1 -4  6 -4  1];
        elseif strcmp(FilterId,'__'), h(dim,1:5)=[ 0  0  1  0  0];
     end
  end
  h_v=h(1,:);
  h_h=h(2,:);
    y=filter2(h_v,filter2(h_h,x));
    if exist('LocalEnergy')==1 
    y=avgsmth(abs(y),LocalEnergy);%LocalEnergyz作为整数出现！
   end
```
 （2）.文件avgsmth.m
```python
function y=avgsmth(x,M,N)
    if nargin<3,%输入参数小于3
        N=M;
    end
    w=ones(M,1)/M;
    y=filter2(w,x);
    w=ones(1,N)/N;
   y=filter2(w,y);
```
（3）.C++代码实现:使用String对象实现
```cpp
void CGrayCal::calLaws(string id1,string id2)
{
    int numL,numR;
    int i,j;
    int k;
    int KerNum;
    
    i=0;
    j=0;
    k=0;
    numL=id1.size();
    numR=id2.size();  
    
    char FilterL[5];
    char FilterR[5];
    strcpy(FilterL,id1.c_str());
    strcpy(FilterR,id2.c_str());*/
//生成卷积核，利用字符串拷贝    
    KerNum =numL*numR;
    ConvKer=new ConvKernel[KerNum];//声明卷积核，内部数据结构，里面包含5*5矩阵
    int L5[]  = { 1 , 4,  6 , 4 , 1};
    int E5[]  = {-1, -2 , 0 , 2 , 1};
    int S5[]  = {-1 , 0,  2,  0, -1};
    int W5[]  = {-1 , 2 , 0 ,-2 , 1};
    int R5[]  = { 1, -4,  6 ,-4,  1};
    int __[]  = { 0 , 0 , 1 , 0,  0}; //(all pass)
    int TempL[5];
    int TempR[5];
    for(i=0;i<numL;++i){
        for(j=0;j<numR;++j){
    
            if ( id1.substr(i,1)=="L")//获得子串进行比较！！！
                memcpy(TempL,L5,sizeof(int)*5);
            if ( id1.substr(i,1)=="E")
                memcpy(TempL,E5,sizeof(int)*5);
            if ( id1.substr(i,1)=="S")
                memcpy(TempL,S5,sizeof(int)*5);
            if ( id1.substr(i,1)=="W")
                memcpy(TempL,W5,sizeof(int)*5);
            if ( id1.substr(i,1)=="R")
                memcpy(TempL,R5,sizeof(int)*5);
            if ( id2.substr(i,1)=="L")
                memcpy(TempR,L5,sizeof(int)*5);
            if ( id2.substr(i,1)=="E")
                memcpy(TempR,E5,sizeof(int)*5);
            if ( id2.substr(i,1)=="S")
                memcpy(TempR,S5,sizeof(int)*5);
            if ( id2.substr(i,1)=="W")
                memcpy(TempR,W5,sizeof(int)*5);
            if ( id2.substr(i,1)=="R")
                memcpy(TempR,R5,sizeof(int)*5);
            memcpy(ConvKer[k].vector1,TempL,sizeof(int)*5);
            memcpy(ConvKer[k].vector2,TempR,sizeof(int)*5);
            vectorPlus(TempL,TempR,ConvKer[k].ConvMat);
            ++k;
        }
    }
```
此段代码实现了输入指定字符串（大写），根据字符串来确定使用所指定的数组，进行左右相乘，得到必要的卷积核。
