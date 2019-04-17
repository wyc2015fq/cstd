# hdu1305——Immediate Decodability - westbrook1998的博客 - CSDN博客





2018年08月23日 19:01:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：35标签：[字符串																[字典树](https://so.csdn.net/so/search/s.do?q=字典树&t=blog)](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
An encoding of a set of symbols is said to be immediately decodable if no code for one symbol is the prefix of a code for another symbol. We will assume for this problem that all codes are in binary, that no two codes within a set of codes are the same, that each code has at least one bit and no more than ten bits, and that each set has at least two codes and no more than eight.  

  Examples: Assume an alphabet that has symbols {A, B, C, D}  

  The following code is immediately decodable:  

  A:01 B:10 C:0010 D:0000  

  but this one is not:  

  A:01 B:10 C:010 D:0000 (Note that A is a prefix of C)  

  Input 

  Write a program that accepts as input a series of groups of records from input. Each record in a group contains a collection of zeroes and ones representing a binary code for a different symbol. Each group is followed by a single separator record containing a single 9; the separator records are not part of the group. Each group is independent of other groups; the codes in one group are not related to codes in any other group (that is, each group is to be processed independently).  

  Output 

  For each group, your program should determine whether the codes in that group are immediately decodable, and should print a single output line giving the group number and stating whether the group is, or is not, immediately decodable.  

  Sample Input 

  01 

  10 

  0010 

  0000 

  9 

  01 

  10 

  010 

  0000 

  9 

  Sample Output 

  Set 1 is immediately decodable 

  Set 2 is not immediately decodable
也是找出字符串中是否有一个字符串是另一个字符串的前缀

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
bool flag;
char ph[20];
struct Node{
    //以该点结尾的单词数
    int c;
    int f;
    Node* Next[2];
    Node(){
        c=0;
        f=0;
        for(int i=0;i<2;i++){
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
    for(int i=0;i<2;i++){
        clear(root->Next[i]);
    }
    delete(root);
}
int main(void){
    int c=1;
    Node* root=new Node();
    while(~scanf("%s",ph)){
        if(ph[0]!='9'){
            insert(root,ph);
        }
        else{
            if(flag){
                printf("Set %d is not immediately decodable\n",c++);
            }
            else{
                printf("Set %d is immediately decodable\n",c++);
            }
            flag=false;
            clear(root);
            root=new Node();
        }
    }
    return 0;
}
```






