# hdu1247——Hat’s Words - westbrook1998的博客 - CSDN博客





2018年08月23日 18:57:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：30








> 
A hat’s word is a word in the dictionary that is the concatenation of exactly two other words in the dictionary.  

  You are to find all the hat’s words in a dictionary.  

  Input 

  Standard input consists of a number of lowercase words, one per line, in alphabetical order. There will be no more than 50,000 words.  

  Only one case.  

  Output 

  Your output should contain all the hat’s words, one per line, in alphabetical order. 

  Sample Input 

  a 

  ahat 

  hat 

  hatword 

  hziee 

  word 

  Sample Output 

  ahat 

  hatword
这题是有没有字符串是由其他两个字符串组成的 

WA了很久的一个点 就是在枚举分割点，找到一个符合条件的，应该**`break`** 不然可能会重复输出一个字符串多次

代码：

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
using namespace std;
string s[50005];
vector<string> res;
bool cmp(string a,string b){
    return a<b;
}
struct Node{
    int cnt;
    Node* Next[26];
    Node(){
        cnt=0;
        for(int i=0;i<26;i++){
            Next[i]=NULL;
        }
    }
};
Node* root;
void insert(string s){
    Node* p=root;
    int val;
    int l=s.size();
    for(int i=0;i<l;i++){
        val=s[i]-'a';
        if(p->Next[val]==NULL){
            p->Next[val]=new Node();
        }
        p=p->Next[val];
    }
    p->cnt++;
}
bool find(string s){
    int l=s.size();
    Node* p=root;
    int val;
    for(int i=0;i<l;i++){
        val=s[i]-'a';
        if(p->Next[val]==NULL){
            return false;
        }
        p=p->Next[val];
    }
    if(p->cnt>0){
        return true;
    }
    else{
        return false;
    }
}
int main(void){
    //freopen("data.txt","r",stdin);
    int c=0;
    root=new Node();
    while(cin >> s[c]){
        insert(s[c]);
        c++;
    }
    for(int i=0;i<c;i++){
        //枚举分解点
        int len=s[i].size();
        for(int k=1;k<len;k++){
            string s1=s[i].substr(0,k);
            string s2=s[i].substr(k,len-k+1);
            //cout << s1 <<" "<<s2 << endl;
            if(find(s1) && find(s2)){
                res.push_back(s[i]);
                break;
            }
        }
    }
    sort(res.begin(),res.end(),cmp);
    int l=res.size();
    for(int i=0;i<l;i++){
        cout << res[i] << endl;
    }
    return 0;
}
```





