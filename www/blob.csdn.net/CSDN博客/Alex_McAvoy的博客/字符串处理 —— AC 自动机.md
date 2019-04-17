# 字符串处理 —— AC 自动机 - Alex_McAvoy的博客 - CSDN博客





2019年03月06日 13:43:17[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：74
个人分类：[—————字符串处理—————																[字符串处理——AC自动机](https://blog.csdn.net/u011815404/article/category/8727853)](https://blog.csdn.net/u011815404/article/category/8650815)








# 【概述】

KMP 算法用于解决长文本的单模板匹配问题，字典树用于解决单个单词（短文本）多模板匹配问题，而 AC 自动机用于解决的是长文本的多模板匹配问题，其是以 trie 树的结构为基础，结合 KMP 的思想建立的。

长文本的多模式匹配就是给出多个模式串 P1,P2,P3...，Pm，求出所有这些模式串在连续文本 T1....n 中的所有可能出现的位置、出现的个数、出现的单词等等。

例如：给出模式集合：{"nihao","hao","hs","hsr"} 与指定文本："sdmfhsgnshejfgnihaofhsrnihao"，求模式集合在文本中所有可能出现的位置。

其运行原理是**通过字典树来构建字典图实现自动跳转，构建失配指针实现多模式匹配**。

# 【预处理】

建立一个 AC 自动机进行查询前，通常需要两个步骤：
- 基础的 Trie 树结构：将所有的模式串构成一棵 Trie 树
- KMP 的思想：对 Trie 树上所有的结点构造失配指针。

## 1.Trie 树的构建

AC 自动机中 Trie 树个构建与单纯的 Trie 树中的 insert 操作一样，只需要利用 Trie 树的结构，将模式串存入即可。

```cpp
int tot=0;//编号
int trie[N][26];//字典树
int val[N];//字符串结尾标记
void insert(char * s){//插入模式串
    int root=0;//字典树上当前匹配到的结点
    for(int i=0;s[i];i++){
        int id=s[i]-'a';//子节点编号
        if(trie[root][id]==0)//若之前没有从root到id的前缀
            trie[root][id]=++tot;//插入
        root=trie[root][id];//顺着字典树往下走
    }
    val[root]++;
}
```

## 2.构造失配指针

### 1）失配指针

AC 自动机的失配指针与 KMP 的 next 数组，两者都是在失配的时候用于跳转的指针，不同的是，KMP 要求的是最长相同前后缀，AC 自动机要求的是相同后缀。

由于 KMP 只对一个模式串做匹配，AC 自动机要对多个模式串做匹配，因此，有可能 fail 指针指向的结点对应着另一个模式串，两个模式串的前缀不同，也就是说，AC 自动机在对匹配串做逐位匹配时，同一位上可能匹配多个模式串，因此，fail 指针会在字典树上来回穿梭，而不是像 KMP 的 next 数组在线性结构上跳转。

### 2）构造

失配指针（fail）的构造与 KMP 中 next 数组的构造相似，即利用部分已经求出的 fail 指针的结点推导出当前结点的 fail，具体使用 bfs 来实现：

首先考虑字典树中当前节点 u，u 的父节点是 p，p 通过字符 ch 的边指向 u，那么假设深度小于 u 的所有节点的 fail 指针都已求得，那么 p 的 fail 指针也可求得.

对于跳转到 p 的 fail 指针指向的节点 fail[p]，有：
- ​​​​​​若结点 fail[p] 通过字符 ch 连接到的子结点 w 存在：

	则让 u 的 fail 指针指向这个结点 w，相当于在 p 和 fail[p] 后加了一个字符 ch，即：fail[u]=w
- ​​​​​​若结点 fail[p] 通过字符 ch 连接到的子结点 w 不存在：

	则继续找 fail[fail[p]] 所指向的结点，重复上述过程，一直跳转 fail 指针直到根节点，若到达根节点时也不存在，那么就令 fail[u]=root

按照如上步骤，即完成了 fail 指针的构建。

如下图，对于字典树 {i,he,his,she,hers} 构建 fail 指针，黄色结点表示当前节点，绿色节点表示已经 bfs 遍历完毕的节点，橙色的边表示完成的 fail 指针，红色的边表示当前节点所指向的 fail 指针。

![](https://img-blog.csdnimg.cn/20190306162733894.gif)

**注：2 号结点的指针画错了，应为 fail[2]=0**

**![](https://img-blog.csdnimg.cn/20190306164654839.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)**

如上图，以节点 6 为例分析 fail 指针的构建：
- 找到节点 6 的父节点 5，5 的 fail 指针指向 10，而节点 1 没有字符 s 连出边
- 跳到 10 的 fail 指针指向的节点 0，发现节点 0 有字符 s 连出的边，指向节点 7
- 因此 fail[6]=7

### **3）字典树与字典图**

由于 fail 指针跳转的路径需要跳转很多次，因此将 fail 指针跳转的路径进行压缩（类似并查集的路径压缩），使得本来需要跳很多次的 fail 指针只跳一次。

在进行 bfs 时，若将根节点入队，则在第一次 bfs 的时，会将根节点的子节点的 fail 指针标记为本身，因此选择将根节点的子节点入队，由于 fail 指针初始化为 0，因此并不影响算法的正确性。

根节点的子节点入队后，每次取出队首元素 k，由于其 fail 指针已经求得，因此只需要求节点 k 的子节点的 fail 指针，则：
- 当字符 i 对应的子节点存在时，将这个子节点 fail 指针赋给 fail[k] 的字符 i 对应的节点
- 当字符 i 对应的子节点不存在时，将 fail[k] 的子节点直接赋成 k 的子节点

将上面的图改一下，蓝色结点表示 bfs 遍历到的结点 k，蓝色、黑色的边表示执行完路径压缩连出字典树的边，可以发现，众多交错的黑边将字典树转为了字典图（图中省略了连向根节点的边）。

![](https://img-blog.csdnimg.cn/20190306164536964.gif)

在构建 fail 指针过程中得到的字典图，在查询时也会起到关键作用。

![](https://img-blog.csdnimg.cn/20190306164706223.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

如上图，以节点 5 为例分析遍历时的情况：
- 如上图，本来应该跳 2 次才能找到 7 号节点，但是通过 10 号节点的黑色边直接通过字符 s 就找到了 7 号节点
- 因此，在路径压缩后，就能在 O(1) 的时间内对单个节点构造 fail 指针

### 4）实现

```cpp
void build(){//构建fail指针域建立字典图
    memset(fail,0,sizeof(fail));
    queue<int>q;
    for(int i=0;i<26;i++)//将根节点的子节点入队
        if(trie[0][i])
            q.push(trie[0][i]);
    
    while(!q.empty()){
        int k=q.front();//对于队首节点k，其fail指针已求得，现在要求的是他子节点的fail指针
        q.pop();
        for(int i=0;i<26;i++){//遍历字符集
            if(trie[k][i]){//若字符i对应的子节点存在
                fail[trie[k][i]]=trie[fail[k]][i];//将这个子节点fail指针赋给fail[k]的字符i对应的节点
                q.push(trie[k][i]);
            }
            else 
                trie[k][i]=trie[fail[k]][i];//将fail[k]的子节点直接赋成k的子节点
        }
    }
}
```

#  【多模式匹配】

fail 指针是在匹配串同一个位置失败时的跳转指针，因此可以利用 fail 指针在同一个位置上进行多模式匹配，匹配完了，就在字典图上自动跳转到下一位置。

以下图为例，红色结点表示当前匹配到的结点 root，粉色箭头表示 root 在字典图上的跳转，蓝色的边表示成功匹配的模式串，蓝色的结点表示跳 fail 指针时的结点。其中的部分跳转，利用的就是新构建的字典图上的边，它也满足后缀相同，所以自动跳转到下一个位置。

![](https://img-blog.csdnimg.cn/20190306170631260.gif)

```cpp
int query(char *t){//对文本串进行匹配
    int res=0;//存储结果
    int root=0;//字典树上当前匹配到的结点
    for(int i=0;t[i];i++){//对文本串进行遍历
        int id=t[i]-'a';//子节点编号
        root=trie[root][id];//在字典图中不断穿梭跳动
        int j=root;
        while(j&&val[j]!=-1){//利用fail指针找出所有匹配的模式串
            res+=val[j];//累加到答案中
            val[j]=-1;
            j=fail[j];//fail指针跳转
        }
    }
    return res;
}
```

# 【模版】

## 1.文本串中模版串总个数

```cpp
int tot=0;//编号
int trie[N][26];//字典树
int val[N];//字符串结尾标记
int fail[N];//失配指针
void insert(char * s){//插入模式串
    int root=0;//字典树上当前匹配到的结点
    for(int i=0;s[i];i++){
        int id=s[i]-'a';//子节点编号
        if(trie[root][id]==0)//若之前没有从root到id的前缀
            trie[root][id]=++tot;//插入
        root=trie[root][id];//顺着字典树往下走
    }
    val[root]++;
}
void build(){//构建fail指针域建立字典图
    memset(fail,0,sizeof(fail));
    queue<int>q;
    for(int i=0;i<26;i++)//将根节点的子节点入队
        if(trie[0][i])
            q.push(trie[0][i]);
    
    while(!q.empty()){
        int k=q.front();//对于队首节点k，其fail指针已求得，现在要求的是他子节点的fail指针
        q.pop();
        for(int i=0;i<26;i++){//遍历字符集
            if(trie[k][i]){//若字符i对应的子节点存在
                fail[trie[k][i]]=trie[fail[k]][i];//将这个子节点fail指针赋给fail[k]的字符i对应的节点
                q.push(trie[k][i]);
            }
            else 
                trie[k][i]=trie[fail[k]][i];//将fail[k]的子节点直接赋成k的子节点
        }
    }
}
int query(char *t){//对文本串进行匹配
    int res=0;//存储结果
    int root=0;//字典树上当前匹配到的结点
    for(int i=0;t[i];i++){//对文本串进行遍历
        int id=t[i]-'a';//子节点编号
        root=trie[root][id];//在字典图中不断穿梭跳动
        int j=root;
        while(j&&val[j]!=-1){//利用fail指针找出所有匹配的模式串
            res+=val[j];//累加到答案中
            val[j]=-1;
            j=fail[j];//fail指针跳转
        }
    }
    return res;
}
char P[N];
char T[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        memset(trie,0,sizeof(trie));
        memset(val,0,sizeof(val));
        memset(fail,0,sizeof(fail));
        tot=0;

        int n;//模式串个数
        scanf("%d",&n);
        while(n--){
            scanf("%s",P);//输入模式串
            insert(P);//插入字典树中
        }
        build();//构建失配指针与字典图

        scanf("%s",T);//输入文本串
        int res=query(T);
        printf("%d\n",res);
    }
    return 0;
}
```

##  2.文本串中单个模版串个数

```cpp
int res[N];
struct AC_Automata{
    int tire[N][26];//字典树
    int val[N];//字符串结尾标记
    int fail[N];//失配指针
    int last[N];//last[i]=j表j节点表示的单词是i节点单词的后缀，且j节点是单词节点
    int tot;//编号

    void init(){//初始化0号点
        tot=1;
        val[0]=0;
        last[0]=0;
        fail[0]=0;
        memset(tire[0],0,sizeof(tire[0]));
    }

    void insert(char *s,int v){//构造trie与val数组，v需非0，表示一个单词节点
        int len=strlen(s);
        int root=0;
        for(int i=0;i<len;i++){
            int id=s[i]-'a';
            if(tire[root][id]==0){
                tire[root][id]=tot;
                memset(tire[tot],0,sizeof(tire[tot]));
                val[tot++]=0;
            }
            root=tire[root][id];
        }
        val[root]=v;
    }

    void build(){//构造fail与last
        queue<int> q;
        for(int i=0;i<26;i++){
            int root=tire[0][i];
            if(root!=0){
                fail[root]=0;
                last[root]=0;
                q.push(root);
            }
        }

        while(!q.empty()){//bfs求fail
            int k=q.front();
            q.pop();
            for(int i=0;i<26; i++){
                int u=tire[k][i];
                if(u==0)
                    continue;
                q.push(u);

                int v=fail[k];
                while(v && tire[v][i]==0)
                    v=fail[v];
                fail[u]=tire[v][i];
                last[u]=val[fail[u]]?fail[u]:last[fail[u]];
            }
        }
    }

    void print(int i){//递归打印与结点i后缀相同的前缀节点编号
        if(val[i]){
            res[val[i]]++;
            print(last[i]);
        }
    }

    void query(char *s){//匹配
        int len=strlen(s);
        int j=0;
        for(int i=0;i<len;i++){
            int id=s[i]-'a';
            while(j && tire[j][id]==0)
                j=fail[j];
            j=tire[j][id];
            if(val[j])
                print(j);
            else if(last[j])
                print(last[j]);
        }
    }
}ac;
char P[1000][1000];
char T[N];
int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&n){
        memset(res,0,sizeof(res));
        ac.init();

        for(int i=1;i<=n;i++){
            scanf("%s",P[i]);
            ac.insert(P[i],i);
        }
        ac.build();

        scanf("%s",T);
        ac.query(T);
        for(int i=1;i<=n;i++)
            if(res[i])
                printf("%s: %d\n",P[i],res[i]);
    }
    return 0;
}
```

# 【例题】
- Keywords Search（HDU-2222）**(文本串中模版串总个数)**：[点击这里](https://blog.csdn.net/u011815404/article/details/88245190)
- 病毒侵袭持续中（HDU-3065）**(文本串中单个模版串个数)**：[点击这里](https://blog.csdn.net/u011815404/article/details/88250169)
- 病毒侵袭（HDU-2896）**(每个文本串中单个模版串个数+set)**：[点击这里](https://blog.csdn.net/u011815404/article/details/88250053)
- Searching the String（ZOJ-3228）**(可重复的模版串)**：[点击这里](https://blog.csdn.net/u011815404/article/details/88364406)




