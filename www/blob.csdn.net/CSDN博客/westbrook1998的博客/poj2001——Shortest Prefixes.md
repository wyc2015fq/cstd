# poj2001——Shortest Prefixes - westbrook1998的博客 - CSDN博客





2018年08月23日 18:26:24[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：23








> 
A prefix of a string is a substring starting at the beginning of the given string. The prefixes of “carbon” are: “c”, “ca”, “car”, “carb”, “carbo”, and “carbon”. Note that the empty string is not considered a prefix in this problem, but every non-empty string is considered to be a prefix of itself. In everyday language, we tend to abbreviate words by prefixes. For example, “carbohydrate” is commonly abbreviated by “carb”. In this problem, given a set of words, you will find for each word the shortest prefix that uniquely identifies the word it represents.  

  In the sample input below, “carbohydrate” can be abbreviated to “carboh”, but it cannot be abbreviated to “carbo” (or anything shorter) because there are other words in the list that begin with “carbo”.  

  An exact match will override a prefix match. For example, the prefix “car” matches the given word “car” exactly. Therefore, it is understood without ambiguity that “car” is an abbreviation for “car” , not for “carriage” or any of the other words in the list that begins with “car”.  

  Input 

  The input contains at least two, but no more than 1000 lines. Each line contains one word consisting of 1 to 20 lower case letters. 

  Output 

  The output contains the same number of lines as the input. Each line of the output contains the word from the corresponding line of the input, followed by one blank space, and the shortest prefix that uniquely (without ambiguity) identifies this word. 

  Sample Input 

  carbohydrate 

  cart 

  carburetor 

  caramel 

  caribou 

  carbonic 

  cartilage 

  carbon 

  carriage 

  carton 

  car 

  carbonate 

  Sample Output 

  carbohydrate carboh 

  cart cart 

  carburetor carbu 

  caramel cara 

  caribou cari 

  carbonic carboni 

  cartilage carti 

  carbon carbon 

  carriage carr 

  carton carto 

  car car 

  carbonate carbona
题意是给出一些字符串，找出每个字符串对应的唯一能表示该字符串的最短前缀（不会和其他字符串冲突） 

用字典树节点维护一个经过该节点的字符串个数，然后find的时候从开始输出字符，直到cnt==1，即经过这个节点只有一个字符串，那就是唯一标识了，return

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <map>
using namespace std;
char s[30];
char t[1005][30];
int k;
struct Node{
    int cnt;
    Node* Next[26];
    Node(int x):cnt(x){
        for(int i=0;i<26;i++){
            Next[i]=NULL;
        }
    }
};
void insert(Node* &rt,char* s){
    int l=strlen(s);
    int val;
    Node* p=rt;
    for(int i=0;i<l;i++){
        val=s[i]-'a';
        if(p->Next[val]==NULL){
            p->Next[val]=new Node(0);
        }
        p=p->Next[val];
        p->cnt++;
    }
}
void find(Node* &rt,char* s){
    int l=strlen(s);
    Node* p=rt;
    int val;
    for(int i=0;i<l;i++){
        val=s[i]-'a';
        if(p->Next[val]==NULL){
            return;
        }
        p=p->Next[val];
        printf("%c",s[i]);
        if(p->cnt==1){
            return;
        }
    }
}
void clear(Node* a){
    if(a==NULL){
        return;
    }
    else{
        for(int i=0;i<26;i++){
            clear(a->Next[i]);
        }
    }
    delete(a);
}
int main(void){
    //freopen("data.txt","r",stdin);
    Node* root=new Node(0);
    while(~scanf("%s",s)){
        insert(root,s);
        strcpy(t[k++],s);
    }
    for(int i=0;i<k;i++){
        printf("%s ",t[i]);
        find(root,t[i]);
        printf("\n");
    }
    return 0;
}
```





