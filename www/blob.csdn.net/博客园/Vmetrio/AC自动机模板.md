# AC自动机模板 - Vmetrio - 博客园







# [AC自动机模板](https://www.cnblogs.com/wangmengmeng/p/5484512.html)





学习链接:

http://www.cppblog.com/mythit/archive/2009/04/21/80633.html

http://blog.csdn.net/niushuai666/article/details/7002736

http://www.cnblogs.com/kuangbin/category/402395.html



hdu 2222 代码:

```
1 #include <vector>
  2 #include <map>
  3 #include <set>
  4 #include <algorithm>
  5 #include <iostream>
  6 #include <cstdio>
  7 #include <cmath>
  8 #include <cstdlib>
  9 #include <string>
 10 #include <cstring>
 11 #include <queue>
 12 #include <stack>
 13 using namespace std;
 14 
 15 #define N 1000010
 16 
 17 char str[N],key[111];
 18 int head,tail;
 19 
 20 struct node
 21 {
 22     node *fail;
 23     node *next[26];
 24     int count;
 25     node(){
 26         fail=NULL;
 27         count=0;
 28         for(int i=0; i<26; i++)
 29             next[i]=NULL;
 30     }
 31 }*q[N];
 32 
 33 node *root;
 34 
 35 void insert(char *str)  //建立Trie  
 36 {
 37     int temp,len;
 38     node *p=root;
 39     len=strlen(str);
 40     for(int i=0; i<len; i++){
 41         temp=str[i]-'a';
 42         if(p->next[temp] == NULL)
 43             p->next[temp]=new node();
 44         p=p->next[temp];
 45     }
 46     p->count++;
 47 }
 48 
 49 void build_ac()   //初始化fail指针，BFS  
 50 {
 51     q[tail++]=root;
 52     while(head != tail){
 53         node *p=q[head++];  //弹出队头  
 54         node *temp=NULL;
 55         for(int i=0; i<26; i++){
 56             if(p->next[i] != NULL){
 57                 if(p==root)  //第一个元素fail必指向根  
 58                     p->next[i]->fail=root;
 59                 else{
 60                     temp=p->fail;   //失败指针
 61                     while(temp!=NULL){   //2种情况结束：匹配为空or找到匹配
 62                         if(temp->next[i]!=NULL){    //找到匹配
 63                             p->next[i]->fail=temp->next[i];
 64                             break;
 65                         }
 66                         temp=temp->fail;
 67                     }
 68                     if(temp==NULL)   //为空则从头匹配 
 69                         p->next[i]->fail=root;
 70                 }
 71                 q[tail++]=p->next[i];   //入队 
 72             }
 73         }
 74     }
 75 }
 76 
 77 int query()    //扫描
 78 {
 79     int index,len,ans=0;
 80     node *p=root;   //Tire入口
 81     len=strlen(str);
 82     for(int i=0; i<len; i++){
 83         index=str[i]-'a';
 84         while(p->next[index]==NULL && p!=root)  //跳转失败指针 
 85             p=p->fail;
 86         p=p->next[index];
 87         if(p==NULL)
 88             p=root;
 89         node *temp=p;   //p不动，temp计算后缀串 
 90         while(temp!=root && temp->count!=-1){
 91             ans+=temp->count;
 92             temp->count=-1;
 93             temp=temp->fail;
 94         }
 95     }
 96     return ans;
 97 }
 98 
 99 int main()
100 {
101     int t,num;
102     scanf("%d",&t);
103     while(t--){
104         head=tail=0;
105         root=new node();
106         scanf("%d",&num);
107         //getchar();
108         for(int i=0; i<num; i++){
109             scanf("%s",key);
110             insert(key);
111         }
112         build_ac();
113         scanf("%s",str);
114         printf("%d\n",query());
115     }
116 }
```














