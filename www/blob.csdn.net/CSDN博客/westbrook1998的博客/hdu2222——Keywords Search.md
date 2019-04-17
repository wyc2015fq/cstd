# hdu2222——Keywords Search - westbrook1998的博客 - CSDN博客





2018年08月26日 19:40:37[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：63标签：[ac自动机																[字符串](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)](https://so.csdn.net/so/search/s.do?q=ac自动机&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
In the modern time, Search engine came into the life of everybody like Google, Baidu, etc.  

  Wiskey also wants to bring this feature to his image retrieval system.  

  Every image have a long description, when users type some keywords to find the image, the system will match the keywords with description of image and show the image which the most keywords be matched.  

  To simplify the problem, giving you a description of image, and some keywords, you should tell me how many keywords will be match.  

  Input 

  First line will contain one integer means how many cases will follow by.  

  Each case will contain two integers N means the number of keywords and N keywords follow. (N <= 10000)  

  Each keyword will only contains characters ‘a’-‘z’, and the length will be not longer than 50.  

  The last line is the description, and the length will be not longer than 1000000.  

  Output 

  Print how many keywords are contained in the description. 

  Sample Input 

  1 

  5 

  she 

  he 

  say 

  shr 

  her 

  yasherhs 

  Sample Output 

  3
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
#include <cstring>
#include <queue>
using namespace std;
const int N=10050;
char k[55];
char s[1000005];
struct Node{
    //表示这个节点做为结尾的单词个数
    int cnt;
    Node* Next[26];
    //失配指针
    Node* fail;
    Node(){
        cnt=0;
        fail=NULL;
        for(int i=0;i<26;i++){
            Next[i]=NULL;
        }
    }
};
queue<Node*> q;
void Insert(Node* &root,char* str){
    int len=strlen(str);
    int val;
    Node *p=root;
    for(int i=0;i<len;i++){
        val=str[i]-'a';
        if(p->Next[val]==NULL){
            p->Next[val]=new Node();
        }
        p=p->Next[val];
    }
    p->cnt++;
}
void getFail(Node* &root){
    while(!q.empty()){
        q.pop();
    }
    for(int i=0;i<26;i++){
        if(root->Next[i]!=NULL){
            //初始化把失配指针指向root
            root->Next[i]->fail=root;
            //第一层所有节点加入队列
            q.push(root->Next[i]);
        }
    }
    //BFS求失配指针
    while(!q.empty()){
        Node* t=q.front();
        q.pop();
        //对当前出队节点的操作是找到其子节点！的fail指针，而不是本身
        for(int i=0;i<26;i++){
            if(t->Next[i]!=NULL){
                //当前节点的fail指针
                Node* p=t->fail;
                while(p!=NULL){
                    //如果fail指针也有这个子节点对应的字符节点，那就将子节点的fail指针指向它
                    if(p->Next[i]!=NULL){
                        t->Next[i]->fail=p->Next[i];
                        break;
                    }
                    //再往上回退，类似于kmp中的j=Next[j]
                    p=p->fail;
                }
                //指向root
                if(p==NULL){
                    t->Next[i]->fail=root;
                }
                q.push(t->Next[i]);
            }
        }
    }
}
int Search(Node* &root,char* str){
    int ans=0;
    if(root==NULL){
        return false;
    }
    int len=strlen(str);
    Node *p=root;
    int val;
    for(int i=0;i<len;i++){
        val=str[i]-'a';
        while(!p->Next[val] && p!=root){
            p=p->fail;
        }
        p=p->Next[val];
        if(!p){
            p=root;
        }
        Node* tmp=p;
        while(tmp!=root){
            if(tmp->cnt>=0){
                ans+=tmp->cnt;
                tmp->cnt=-1;
            }
            else{
                break;
            }
            tmp=tmp->fail;
        }
    }
    return ans;
}
void clear(Node* root){
    if(root==NULL){
        return;
    }
    for(int i=0;i<26;i++){
        clear(root->Next[i]);
    }
    root->fail=NULL;
    delete(root);
}
int main(void){
    //freopen("data.txt","r",stdin);
    Node* root;
    int t,n;
    scanf("%d",&t);
    while(t--){
        root=new Node();
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%s",k);
            Insert(root,k);
        }
        getFail(root);
        scanf("%s",s);
        int ans=Search(root,s);
        printf("%d\n",ans);
        clear(root);
    }
    return 0;
}
```






