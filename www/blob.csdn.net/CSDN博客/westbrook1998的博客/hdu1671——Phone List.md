# hdu1671——Phone List - westbrook1998的博客 - CSDN博客





2018年08月23日 18:47:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：26标签：[字符串																[字典树																[前缀](https://so.csdn.net/so/search/s.do?q=前缀&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
Given a list of phone numbers, determine if it is consistent in the sense that no number is the prefix of another. Let’s say the phone catalogue listed these numbers:  

  1. Emergency 911  

  2. Alice 97 625 999  

  3. Bob 91 12 54 26  

  In this case, it’s not possible to call Bob, because the central would direct your call to the emergency line as soon as you had dialled the first three digits of Bob’s phone number. So this list would not be consistent.  

  Input 

  The first line of input gives a single integer, 1 <= t <= 40, the number of test cases. Each test case starts with n, the number of phone numbers, on a separate line, 1 <= n <= 10000. Then follows n lines with one unique phone number on each line. A phone number is a sequence of at most ten digits. 

  Output 

  For each test case, output “YES” if the list is consistent, or “NO” otherwise. 

  Sample Input 

  2 

  3 

  911 

  97625999 

  91125426 

  5 

  113 

  12340 

  123440 

  12345 

  98346 

  Sample Output 

  NO 

  YES
给几个字符串 问其中是否有字符串是另一个字符串的前缀

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
bool flag;
char ph[15];
struct Node{
    int f;
    Node* Next[10];
    Node(){
        c=0;
        f=0;
        for(int i=0;i<10;i++){
            Next[i]=NULL;
        }
    }
};
void insert(Node* &rt,char* s){
    int l=strlen(s);
    int val;
    Node* p=rt;
    for(int i=0;i<l;i++){
        val=s[i]-'0';
        //考虑先插入长的 再插入短的 只要短的遍历完下一个节点还有数据即可，说明短的为前缀
        if(i==l-1 && p->Next[val]){
            flag=true;
            return;
        }
        if(p->Next[val]==NULL){
            p->Next[val]=new Node();
        }
        p=p->Next[val];
        if(p->f){
            flag=true;
        }
    }
    p->f=1;
}
void clear(Node* root){
    if(root==NULL){
        return;
    }
    for(int i=0;i<10;i++){
        clear(root->Next[i]);
    }
    delete(root);
}
int main(void){
    int t,n;
    scanf("%d",&t);
    while(t--){
        Node* root=new Node();
        scanf("%d",&n);
        flag=false;
        while(n--){
            scanf("%s",ph);
            if(flag){
                continue;
            }
            insert(root,ph);
        }
        if(flag){
            printf("NO\n");
        }
        else{
            printf("YES\n");
        }
        clear(root);
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=字典树&t=blog)](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)




