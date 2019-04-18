# HDU3788 ZOJ问题 - 江南烟雨 - CSDN博客
2011年03月07日 21:15:00[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：2910
题目：[http://acm.hdu.edu.cn/showproblem.php?pid=3788](http://acm.hdu.edu.cn/showproblem.php?pid=3788)
代码：
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){
    char s[1001];
    int len,cnt,flag1,flag2,flag3,i;
    while(gets(s)!=NULL){
         len=strlen(s);
         cnt=0;
         flag1=flag2=-1;
         flag3=0;
         for(i=0;i<len;i++){
               if(s[i]!='z'&&s[i]!='o'&&s[i]!='j'){
                      flag3=1; //记录是否有非法字符； 
                      break;                            
               }             
         }
         for(i=0;i<len;i++){
              if(s[i]=='z'){
                    flag1=i;//z第一次出现； 
                    break;        
              }              
         }
         for(i=len-1;i>=0;i--){
              if(s[i]=='j'){
                    flag2=i;//j最后一次出现； 
                    break;        
              }                 
         }
         if(flag1>flag2||flag1==-1||flag2==-1||flag3==1){
              printf("Wrong Answer/n");           
         } 
         else{
              for(i=flag1+1;i<flag2;i++){
                    if(s[i]=='o'){
                          cnt++;        
                    }//统计z、j中间o的个数                     
              }
              //printf("cnt=%d/n",cnt);
              if(cnt!=flag2-flag1-1||cnt==0){//判断z、j中间是否出现除o以外的字符；cnt=0是zoj中间没有出现o，不合法； 
                     printf("Wrong Answer/n");                  
              }
              else{
                   if(flag1*cnt==len-flag2-1){
                       printf("Accepted/n");                       
                   }
                   else{
                        printf("Wrong Answer/n");
                   }
              }
         }               
    }
}
