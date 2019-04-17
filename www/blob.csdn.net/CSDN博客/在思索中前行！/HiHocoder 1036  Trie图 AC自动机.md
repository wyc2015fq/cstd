# HiHocoder 1036 : Trie图 AC自动机 - 在思索中前行！ - CSDN博客





2016年10月08日 21:29:16[_Tham](https://me.csdn.net/txl16211)阅读数：645








Trie图


 先看一个问题：给一个很长很长的母串 长度为n，然后给m个小的模式串。求这m个模式串里边有多少个是母串的字串。


最先想到的是暴力O(n*m*len(m)) len(m)表示这m个模式串的平均长度。。。


显然时间复杂度会很高。。。


再改进一些，用kmp让每一模式串与母串进行匹配呢？时间复杂度为O((n + len(m))*m)，还算可以。


可是还有没有更快的算法呢？


编译原理里边有一个很著名的思想：自动机。




这里就要用到确定性有限状态自动机（DFA）。可以对这m个模式串建立一个DFA，然后让母串在DFA上跑，遇到某个模式串的终结节点则表示这个模式串在母串上。





![](http://pic002.cnblogs.com/images/2012/307740/2012072414074035.jpg)


就像这个图，母串“nano”在上边跑就能到达终止节点。


上边说的是自动机的概念。。。还有一个要用到的是trie树，这个不解释了，网上资料一大堆。




这里步入正题：Trie图


trie图是一种DFA，可以由trie树为基础构造出来，

对于插入的每个模式串，其插入过程中使用的最后一个节点都作为DFA的一个终止节点。

如果要求一个母串包含哪些模式串，以用母串作为DFA的输入，在DFA 上行走，走到终止节点，就意味着匹配了相应的模式串。

ps: AC自动机是Trie的一种实现，也就是说AC自动机是构造Trie图的DFA的一种方法。还有别的构造DFA的方法... 


怎么建Trie图？


可以回想一下，在kmp算法中是如何避免母串在匹配过程种指针回溯的？也就是说指针做不必要的前移，浪费时间。


同样的，在trie图中也定义这样一个概念：前缀指针。


这个前缀指针，从根节点沿边到节点p我们可以得到一个字符串S，节点p的前缀指针定义为：指向树中出现过的S的最长的后缀。

![](http://pic002.cnblogs.com/images/2012/307740/2012072414181656.jpg)




构造前缀指针的步骤为：根据深度一一求出每一个节点的前缀指针。对于当前节点，设他的父节点与他的边上的字符为Ch，如果他的父节点的前缀指针所指向的节点的儿子中，有通过Ch字符指向的儿子，那么当前节点的前缀指针指向该儿子节点，否则通过当前节点的父节点的前缀指针所指向点的前缀指针，继续向上查找，直到到达根节点为止。

![](http://pic002.cnblogs.com/images/2012/307740/2012072414255524.jpg)




上图构造出所有节点的前缀指针。


相信原来的问题到这里基本已经解决了。可以再考虑一下它的时间复杂度，设M个串的总长度为LEN


所以算法总的时间复杂度为O(LEN + n)。比较好的效率。


模板，HDU 2222：



```cpp
/*
个人感觉这样写更清晰一点。（动态分配内存）
*/
class Node {
public:
    Node* fail;
    Node* next[26];
    int cnt;
    Node() {
        CL(next, 0);
        fail = NULL;
        cnt = 0;
    }
};

//Node* q[10000000];
class AC_automaton : public Node{
public:
    Node *root;
    int head, tail;

    void init() {
        root = new Node();
        head = tail = 0;
    }

    void insert(char* st) {
        Node* p = root;
        while(*st) {
            if(p->next[*st-'a'] == NULL) {
                p->next[*st-'a'] = new Node();
            }
            p = p->next[*st-'a'];
            st++;
        }
        p->cnt++;
    }

    void build() {
        root->fail = NULL;
        deque<Node* > q;
        q.push_back(root);

        while(!q.empty()) {
            Node* tmp = q.front();
            Node* p = NULL;
            q.pop_front();
            for(int i = 0; i < 26; ++i) {
                if(tmp->next[i] != NULL) {
                    if(tmp == root) tmp->next[i]->fail = root;
                    else {
                        p = tmp->fail;
                        while(p != NULL) {
                            if(p->next[i] != NULL) {
                                tmp->next[i]->fail = p->next[i];
                                break;
                            }
                            p = p->fail;
                        }
                        if(p == NULL)   tmp->next[i]->fail = root;
                    }
                    q.push_back(tmp->next[i]);
                }
            }
        }
    }

    int search(char* st) {
        int cnt = 0, t;
        Node* p = root;
        while(*st) {
            t = *st - 'a';
            while(p->next[t] == NULL && p != root) {
                p = p->fail;
            }
            p = p->next[t];
            if(p == NULL)   p = root;

            Node* tmp = p;
            while(tmp != root && tmp->cnt != -1) {
                cnt += tmp->cnt;
                tmp->cnt = -1;
                tmp = tmp->fail;
            }
            st++;
        }
        return cnt;
    }
}AC;
```
以上转载自：http://www.cnblogs.com/vongang/archive/2012/07/24/2606494.html




Trie图：[http://hihocoder.com/problemset/problem/1036](http://hihocoder.com/problemset/problem/1036)



[是](http://www.cnblogs.com/abclzr/p/5355499.html)[**上建立“前缀边”**](http://www.cnblogs.com/abclzr/p/5355499.html)，不用再像在上那样顺着一个一个往上跳了，省了不少时间。这种做法在 上时间排到了前三名。




```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
#define N 1000006
using namespace std;
int c[N][26], cnt = 0, fail[N], n, q[N], w[N];
inline void ins(char *s) {
    int len = strlen(s), now = 0;
    for(int i = 0; i < len; ++i) {
        int t = s[i] - 'a';
        if (!c[now][t]) c[now][t] = ++cnt;
        now = c[now][t];
    }
    w[now] = 1;
}
inline void BFS() {
    int now, head = -1, tail = -1;
    for(int t = 0; t < 26; ++t)
        if (c[0][t])
            q[++tail] = c[0][t];
    while (head != tail) {
        now = q[++head];
        for(int t = 0; t < 26; ++t)
            if (!c[now][t])
                c[now][t] = c[fail[now]][t]; //建立“前缀边”
            else {
                q[++tail] = c[now][t];
                int tmp = fail[now];
                while(tmp && !c[tmp][t])
                    tmp = fail[tmp];
                fail[c[now][t]] = c[tmp][t];
            }
    }
}
inline void AC(char *s) {
    int len = strlen(s), now = 0;
    for(int i = 0; i < len; ++i) {
        now = c[now][s[i] - 'a'];
        if (w[now]) {
            puts("YES");
            return;
        }
    }
    puts("NO");
}
int main() {
    scanf("%d\n", &n);
    char s[N];
    for(int i = 1; i <= n; ++i)
        scanf("%s", s), ins(s);
    BFS();
    scanf("%s", s);
    AC(s);
    return 0;
}
```
不要介意“前缀边”这个名字起得多么牵强，可以理解为记录最终跳到的点，直接指过去就行了。gty学长讲课时也讲过这种优化。

## 法一：Trie图
讲的很详细，又是已经会了手动操作，变成代码还是有点困难，按照郭老师那个模版敲了一个差不多的，但是感觉和本题所讲写的不一样，让我再研究一下





```cpp
#include <cstdio>  
#include <cstring>  
#include <queue>  
  
using namespace std;  
  
int n;  
char s[1000005];  
  
struct Node {  
    bool isend;  
    Node *nxt[26],*pre;  
  
    Node():isend(false),pre(NULL) {  
        memset(nxt,NULL,sizeof(nxt));  
    }  
}*root,*cur,*pre;  
  
void add(char *p) {//添加模式串，建立trie树  
    cur=root;  
    while(*p) {  
        if(cur->nxt[*p-'a']==NULL)  
            cur->nxt[*p-'a']=new Node();  
        cur=cur->nxt[*p-'a'];  
        ++p;  
    }  
    cur->isend=true;  
}  
  
void build() {//建立trie图  
    cur=root;  
    queue<Node*> q;  
    for(int i=0;i<26;++i)  
        if(root->nxt[i]) {//第一层结点的前缀指针指向根结点  
            cur->nxt[i]->pre=root;  
            q.push(cur->nxt[i]);  
        }  
    while(!q.empty()) {  
        cur=q.front();  
        q.pop();  
        for(int i=0;i<26;++i) {  
            if(cur->nxt[i]) {//如果当前结点存在i子结点  
                pre=cur->pre;  
                while(pre) {  
                    if(pre->nxt[i]) {//找到当前结点的有i子结点的前缀结点  
                        cur->nxt[i]->pre=pre->nxt[i];  
                        if(pre->nxt[i]->isend)//如果该前缀结点危险结点，则其i子结点也是危险结点  
                            cur->nxt[i]->isend=true;  
                        break;  
                    }  
                    pre=pre->pre;  
                }  
                if(cur->nxt[i]->pre==NULL)//如果未找到当前结点的有i子结点的前缀结点，则其i子结点的前缀结点是根节点  
                    cur->nxt[i]->pre=root;  
                q.push(cur->nxt[i]);  
            }  
        }  
    }  
}  
  
bool query(char *p) {  
    int i;  
    cur=root;  
    while(*p) {  
        i=*p-'a';  
        while(cur) {  
            if(cur->nxt[i]) {  
                cur=cur->nxt[i];  
                if(cur->isend==true)  
                    return true;  
                break;  
            }  
            cur=cur->pre;  
        }  
        if(cur==NULL)//若trie图中没有以*p开头的模式串，当前结点指向根结点  
            cur=root;  
        ++p;  
    }  
    return false;  
}  
  
int main() {  
    root=new Node();  
    scanf("%d",&n);  
    while(n--) {  
        scanf("%s",s);  
        add(s);  
    }  
    build();  
    scanf("%s",s);  
    printf("%s\n",query(s)?"YES":"NO");  
    return 0;  
}
```


## 法二：AC自动机



刚开始直接用没有修改的build函数和query函数，导致query每次还得查询当前词的后缀，引起TLE后来发现如果其后缀是河蟹词，将其标记为危险可以避免查询当前词的后缀


```cpp
#include <cstdio>  
#include <queue>  
using namespace std;  
  
const int MAXNODE=1000005;  
  
struct Trie {  
    int nxt[MAXNODE][26],fail[MAXNODE];  
    bool ed[MAXNODE];  
    int l;  
    const static int root=0;  
  
    Trie() {  
        clear();  
    }  
  
    int newNode() {  
        for(int i=0;i<26;++i)  
            nxt[l][i]=-1;  
        ed[l]=false;  
        return l++;  
    }  
  
    void insert(char *p) {  
        int cur=root;  
        while(*p) {  
            if(nxt[cur][*p-'a']==-1)  
                nxt[cur][*p-'a']=newNode();  
            cur=nxt[cur][*p-'a'];  
            ++p;  
        }  
        ed[cur]=true;  
    }  
  
    void build() {  
        int cur=root,i;  
        queue<int> q;  
        fail[root]=root;  
        for(i=0;i<26;++i) {  
            if(nxt[root][i]==-1)  
                nxt[root][i]=root;  
            else {  
                fail[nxt[root][i]]=root;  
                q.push(nxt[root][i]);  
            }  
        }  
  
        while(!q.empty()) {  
            cur=q.front();  
            q.pop();  
            for(i=0;i<26;++i) {  
                if(nxt[cur][i]==-1)  
                    nxt[cur][i]=nxt[fail[cur]][i];  
                else {  
                    fail[nxt[cur][i]]=nxt[fail[cur]][i];  
                    q.push(nxt[cur][i]);  
                    if(ed[fail[nxt[cur][i]]])//优化，与普通的AC自动机不同，因为只要有河蟹词就返回，所以有河蟹词后缀的也标记危险，去掉查询时通过while查询后缀  
                        ed[nxt[cur][i]]=true;  
                }  
            }  
        }  
    }  
  
    bool query(char *p) {  
        int cur=root;  
        while(*p) {  
            cur=nxt[cur][*p-'a'];  
            if(ed[cur])  
                return true;  
            ++p;  
        }  
        return false;  
    }  
  
    void clear() {  
        l=root;  
        newNode();  
    }  
}ac;  
  
int n;  
char s[MAXNODE];  
  
int main() {  
    scanf("%d",&n);  
    while(n--) {  
        scanf("%s",s);  
        ac.insert(s);  
    }  
    ac.build();  
    scanf("%s",s);  
    printf("%s\n",ac.query(s)?"YES":"NO");  
    return 0;  
}
```







