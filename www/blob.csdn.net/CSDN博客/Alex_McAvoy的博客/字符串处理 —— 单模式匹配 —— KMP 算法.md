# 字符串处理 —— 单模式匹配 —— KMP 算法 - Alex_McAvoy的博客 - CSDN博客





2019年02月26日 16:15:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：51
个人分类：[字符串处理——单模式匹配(KMP)](https://blog.csdn.net/u011815404/article/category/8698890)









# 【概述】

KMP 算法又称 Knuth-Morris-Pratt 字符串匹配算法，是由于 D.E.Knuth、J.H.Morris 和 V.R.Pratt 三人共同研究的，用于解决字符串匹配问题。

其核心思想是利用**已经部分匹配的有效信息，来保持文本串指针不回溯，通过修改模式串指针，让模式串尽量地移动到有效的位置**，因此，整个 KMP 的重点就在于当模式串中某个字符与文本串不匹配时，如何移动模式串指针。

KMP 算法的主要特点是：
- 需要对模式字符串做预处理；
- 预处理阶段需要额外的 O(m) 空间和复杂度；
- 匹配阶段与字符集的大小无关；
- 匹配阶段至多执行 2n-1 次字符比较；
- 对模式中字符的比较顺序时从左到右；

# 【算法原理】

如下图，以文本 T=ABACBCDHIJK，模式 P=ABAD 为例，前面三个字符是匹配的而 C 与 D 不匹配，可以利用已匹配的信息将模式指针 j 移到第 1 位上（指针从 0 开始），文本指针 i 位置不变

![](https://img-blog.csdnimg.cn/20190226162059920.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190226162112424.png)

同理，对于下图中的文本 T=ABCABCDHIJK 与模式 P=ABCABB，当 C 与 B 不匹配时，可利用已匹配信息，将模式指针 j 移动到第 2 位上，文本指针 i 位置不变

![](https://img-blog.csdnimg.cn/20190226162120212.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)![](https://img-blog.csdnimg.cn/20190226162128877.png)

通过上面两个例子，可以看出，当匹配失败时，文本指针 i 的位置不变，模式指针 j 要移动到位置 k，而位置 k 的性质满足：**模式串最前面 k 个字符与 j 之前的最后 k 个字符相同**，即：![P[0\sim k-1]=P[i-k\sim j-1]](https://private.codecogs.com/gif.latex?P%5B0%5Csim%20k-1%5D%3DP%5Bi-k%5Csim%20j-1%5D)

![](https://img-blog.csdnimg.cn/20190226162136125.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

当 ![T[i]!=P[j]](https://private.codecogs.com/gif.latex?T%5Bi%5D%21%3DP%5Bj%5D) 时，有：![T[i-j \sim i-1]=P[0 \sim j-1]](https://private.codecogs.com/gif.latex?T%5Bi-j%20%5Csim%20i-1%5D%3DP%5B0%20%5Csim%20j-1%5D)

根据![P[0\sim k-1]=P[i-k\sim j-1]](https://private.codecogs.com/gif.latex?P%5B0%5Csim%20k-1%5D%3DP%5Bi-k%5Csim%20j-1%5D)

可得：![T[i-k\sim i-1]=P[0\sim k-1]](https://private.codecogs.com/gif.latex?T%5Bi-k%5Csim%20i-1%5D%3DP%5B0%5Csim%20k-1%5D)，因此可以判断直接将 j 移动到 k 无须再比较前面的 k 个字符

至此，关键的部分是求得 k，由于在模式 P 的每一个位置都可能发生不匹配，也就是说需要计算每一个位置 j 对应的 k，故而可以用一个数组 next[] 来保存，即**令 next[j]=k 表示当 T[i]!=P[j] 时，j 指针的下一个位置**

以下图为例，可以发现，当 j 为 0 时就不匹配，这个时候 j 已经在最左边了，不可能进行移动，需要调整的是将指针 i 后移一位，因此要对 next[] 数组初始化，即 next[0]=-1

![](https://img-blog.csdnimg.cn/20190226162144220.png)

再以下图为例，当 j=1 时不匹配，因为他前面只有一个位置，那么显然指针 j 需要移到 0 位置

![](https://img-blog.csdnimg.cn/20190226162152840.png)

对比以下两个图，可以发现，当 ![P[k]=P[j]](https://private.codecogs.com/gif.latex?P%5Bk%5D%3DP%5Bj%5D) 时，有 **next[j+1]=next[j]+1**

证明：

由于 P[j] 之前已经有 ![P[0\sim k-1]=P[i-k\sim j-1]](https://private.codecogs.com/gif.latex?P%5B0%5Csim%20k-1%5D%3DP%5Bi-k%5Csim%20j-1%5D)，即 ![next[j]=k](https://private.codecogs.com/gif.latex?next%5Bj%5D%3Dk)

这个时候有 P[k]=P[j]，那么可以得到 ![P[0 \sim k-1] + P[k] = P[j-k\sim j-1] + P[j]。](https://private.codecogs.com/gif.latex?P%5B0%20%5Csim%20k-1%5D%20&plus;%20P%5Bk%5D%20%3D%20P%5Bj-k%5Csim%20j-1%5D%20&plus;%20P%5Bj%5D%25u3002)

即：![P[0 \sim k] = P[j-k \sim j]](https://private.codecogs.com/gif.latex?P%5B0%20%5Csim%20k%5D%20%3D%20P%5Bj-k%20%5Csim%20j%5D)，即：![next[j+1] = k + 1](https://private.codecogs.com/gif.latex?next%5Bj&plus;1%5D%20%3D%20k%20&plus;%201)，从而有：![next[j+1]=next[j]+1](https://private.codecogs.com/gif.latex?next%5Bj&plus;1%5D%3Dnext%5Bj%5D&plus;1)

![](https://img-blog.csdnimg.cn/20190226162159767.png)![](https://img-blog.csdnimg.cn/20190226162207460.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

而当 ![P[k] \:\:!=P[j]](https://private.codecogs.com/gif.latex?P%5Bk%5D%20%5C%3A%5C%3A%21%3DP%5Bj%5D) 时，对比以下两张图，可以发现只要令** k=next[k] **即可

![](https://img-blog.csdnimg.cn/20190226162233473.png)![](https://img-blog.csdnimg.cn/20190226162240214.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

此时，next 数组已经求出，但还存在一个缺陷

以下图为例，显然得到的 next 数组是 {-1,0,0,1}

![](https://img-blog.csdnimg.cn/20190226162214181.png)

如下图，当 C 与 B 不匹配时，应将模版指针 j 移动到第一个元素

![](https://img-blog.csdnimg.cn/20190226162224487.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

不难发现，由于后面的 B 已经不匹配了，那么前面的 B 也一定是不匹配的，因此这一步完全没有意义，同理，A 也一样

显然，发生问题的原因在于 P[j]=P[next[j]] ，因此加一个判断条件即可，值得注意的是，何时需要加上此条判断要根据实际情况

# 【应用】

KMP 算法的核心是求 next 数组，最大的应用除解决字符串匹配问题外，常用 next 数组来求循环节。

在未加判断的 next 数组中，其代表**当前字符之前的字符串中，最大前缀与后缀匹配数**，即 next[j]=k 代表 j 之前的字符串中有最大长度为 k 的相同前缀后缀。

例如：
|i|0|1|2|3|4|5|
|----|----|----|----|----|----|----|
|p[i]|a|b|c|b|a||
|next[i]|-1|0|0|0|0|1|

对于长度为 n 的模式串，由于 next[j]=k 表示 p[1...i-1] 最大前缀与后缀匹配数，那么模式串第 1 位到 next[n] 位与模式串第 n-next[n] 位到第 n 位是匹配的

因此当 next[i]>0 时，i-next[i] 为字符串匹配的时候移动的位数，故而当 **n%(n-next[n])=0** 时，说明模式串中存在重复连续的子串，其长度为** len=n-next[n]**，那么字符串的最小周期** res=n/len=n/(n-next[n])**

# 【算法实现】

## 1.求 next 数组

1）不加判断版

```cpp
int next[N];
void getNext(char p[]){
    next[0]=-1;//初始化

    int len=strlen(p);//模式串长度
    int j=0;//模式指针j
    int k=-1;//位置k

    while(j<len) {
        if(k==-1||p[j]==p[k]) {//next[j+1]=next[j]+1
            k++;//此前有next[j]=k
            j++;//指针后移
            next[j]=k;
        }else{
            k=next[k];
        }
    }
}
```

2）加判断版

```cpp
int next[N];
void getNext(char p[]){
    next[0]=-1;//初始化

    int len=strlen(p);//模式串长度
    int j=0;//模式指针j
    int k=-1;//位置k

    while(j<len) {
        if(k==-1||p[j]==p[k]) {//next[j+1]=next[j]+1
            k++;//此前有next[j]=k
            j++;//指针后移
            if(p[j]==p[k])//当两个字符相等时跳过
                next[j] = next[k];
            else
                next[j]=k;
        }else{
            k=next[k];
        }
    }
}
```

## 2.匹配

```cpp
int KMP(char t[],char p[]) {
    int tLen=strlen(t);//文本串长度
    int pLen=strlen(p);//模式串长度

    int i=0;//文本串指针
    int j=0;//模式串指针

    getNext();//获取next数组

    while(i<tLen&&j<pLen) {
       if (j==-1||t[i]==p[j]){//当j为-1时，要移动的是i，同样j也要归零
           i++;
           j++;
       }else{
           j=next[j];//j回到指定位置
       }
    }

    if(j==pLen)//最终当模式串的位置与模式串的长度相同时，说明匹配成功
       return i-j;
    else//匹配失败
       return -1;
}
```

## 3.求模式串出现次数

```cpp
int KMP(char t[],char p[]) {
    int tLen=strlen(t);
    int pLen=strlen(p);
    getNext(p);

    int res=0;
    
    int j=0;//初始化在模式串的第一个位置
    for(int i=0;i<tLen;i++){//遍历文本串
    	while(j&&p[j]!=t[i]){//顺着失配边走，直到可以匹配，最坏情况是j=0
        	j=Next[j];
    	}
    	if(p[j]==t[i]){//匹配成功，继续下一个位置
        	j++;
    	}
    	if(j==pLen){//计算模式串在文本串中出现的次数
        	res++;
            /*
                若可使用重复字符，j=0需注释掉
                若不可使用重复字符，j=0无需注释
                原因：是否需要初始化模式串位置
            */
            j=0;
        }
    }

    return res;
}
```

## 4.求最小循环节长度

```cpp
int next[N];
void getNext(char p[]){
    next[0]=-1;//初始化

    int len=strlen(p);//模式串长度
    int j=0;//模式指针j
    int k=-1;//位置k

    while(j<len) {
        if(k==-1||p[j]==p[k]) {//next[j+1]=next[j]+1
            k++;//此前有next[j]=k
            j++;//指针后移
            next[j]=k;
        }else{
            k=next[k];
        }
    }
}
int main(){
    scanf("%s",p)；
    int n=strlen(p);//模式串长度
    getNext();//获取next数组
    
    int len=n-next[n];//与前缀相同的后缀长度
    if(n%len==0){//存在循环节
        int res=n/len;//循环节长度
        printf("%d\n",res);
    }
    else//不存在循环节，长度为1
        printf("1\n");
    return 0;
}
```



