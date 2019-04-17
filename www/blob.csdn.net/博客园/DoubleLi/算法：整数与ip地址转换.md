# 算法：整数与ip地址转换 - DoubleLi - 博客园






直接上代码(不要直接拷贝，中间少了一行啊)：






- 



#include <string>


- 



#include <iostream>


- 



using namespace std;


- 



int shu[4]={0};


- 



int IPtoINT(string s){


- 



int i,a=0,c=0,p=0;


- 



for(i=0;i<s.length();i++)


- 



{


- 



if(s[i]!='.')


- 



{


- 



a=a*10+(s[i]-'0');


- 



}else{


- 



if(a>=0&&a<=255)


- 



{


- 



shu[p++]=a;


- 



c++;


- 



}else


- 



{//cout<<"NO"<<endl;


- 



return 0;}


- 




- 



}


- 



}


- 



if(c!=3)


- 



{//cout<<"NO"<<endl;


- 



return 0;}


- 



else


- 



{if(a>=0&&a<=255)


- 



{


- 



shu[p++]=a;


- 



}else


- 



{//cout<<"NO"<<endl;


- 



return 0;}


- 




- 



}


- 



//cout<<"YES"<<endl;


- 



return 1;


- 



}


- 



void INTtoIP(long s)


- 



{


- 



shu[0]=s&255;


- 



shu[1]=(s&(255*256))/256;


- 



shu[2]=(s&(255*256*256))/(256*256);


- 



shu[3]=(s&(255*256*256*256))/(256*256*256);


- 



}


- 



void main(){


- 



string ips;


- 



long ipint,stoint=0;


- 



int i;


- 



cin>>ips;


- 



cin>>ipint;


- 



if(IPtoINT(ips))


- 



{


- 



for(i=0;i<4;i++)


- 



stoint=stoint*256+shu[i];


- 



cout<<stoint<<endl;


- 



}


- 



INTtoIP(ipint);


- 



if(shu[0]<256&&shu[1]<256&&shu[2]<256&&shu[3]<256)


- 



{


- 



cout<<shu[3]<<'.'<<shu[2]<<'.'<<shu[1]<<'.'<<shu[0]<<endl;


- 



}


- 



}











