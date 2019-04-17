# 字符串处理 —— 回文串相关 —— Manacher 算法 - Alex_McAvoy的博客 - CSDN博客





2019年02月25日 18:08:15[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：65








# 【概述】

Manacher 算法又称马拉车算法，用于求最长回文子串。

对于最长回文子串传统的求法的求法是以每个字符为中心，向两边寻找回文子串，在遍历完整个数组后即可得到最长回文子串，其时间复杂度为 O(n^2)

而马拉车算法，将求最长回文子串的时间复杂度提升到了线性，其时间复杂度只有 O(n)

# 【算法流程】

## 1.预处理

由于字符串的长度分为奇偶两种，因此对于初始的字符串，在每一个字符的左右都加上一个未在串中出现过的字符，得到一个新的字符数组 newStr[]。

比如：
- 奇数字符串：bob --> #b#o#b#
- 偶数字符串：noon --> #n#o#o#n#

这样一来，无论原字符串是奇数个字符还是偶数个字符，处理后的字符串字符的个数都是奇数个，这样就无需分情况讨论了。

## 2.设置辅助数组

接下来，还需要定义一个辅助数组 p[]，其中 p[i] 表示以字符 newStr[i] 为中心的最长回文半径，若 p[i]=1，则该回文子串就是 newStr[i] 本身。

例如：以字符串 abbahopxp 为例
|i|0|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|
|newStr[i]|$|#|a|#|b|#|b|#|a|#|h|#|o|#|p|#|x|#|p|#|
|p[i]||1|2|1|2|5|2|1|2|1|2|1|2|1|2|1|4|1|2|1|

可以看出，p[i]-1 即为原字符串中最长回文串的长度。

## 3.求解辅助数组

首先新增两个变量 mx 与 id，其中 mx 代表以 id 为中心的最长回文串的右边界，即：mx=id+p[id]

假设求以 i 为中心的最长回文半径，即求解 p[i]，**当 i<mx 时，那么有 p[i] = min(p[2*id-i], mx-i)，否则 p[i]=1**，其中 2*id-i 为图中的 j 点，即 p[j] 是以 j 为中心的最长回文半径

# 【复杂度分析】

基于回文串的性质，辅助数组 p[i] 的值基于以下三种情况得出：

**1）j 的回文串有一部分在 id 的回文串之外**

![](https://img-blog.csdnimg.cn/20190225194515351.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

如上图，黑线为 id 的回文串，i 与 j 关于 id 对称，红线为 j 的回文，紫线为 i 的回文，虚线为以 id 为中心的回文右边界 mx，那么根据 p[i]=mx-i，即紫线的部分，那么可以得出，p[i] 无法得到更大的值

![](https://img-blog.csdnimg.cn/20190225194435626.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

如上图，假设右侧新增的紫色 d 部分是 p[i] 可增加的部分，那么根据回文的性质，也即是说 **id 的回文 = 黑线 + a + d**，矛盾，假设不成立，因此 p[i] 无法再增加，得到一个更大的值

**2）j 回文串全部在 i 内部**

![](https://img-blog.csdnimg.cn/20190225194526481.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

如上图，此时 p[i]=p[j]，那么可以得出，p[i] 无法得到更大的值

![](https://img-blog.csdnimg.cn/20190225194449392.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

如上图，假设右侧的红线 d 是 p[i] 可增加的部分，那么根据回文的性质，**j 的回文 = j 之前的回文 + a + b**，矛盾，假设不成立，因此 p[i] 无法再增加，得到一个更大的值

**3）i 的回文串右端与 id 的回文串右端重合**

![](https://img-blog.csdnimg.cn/20190225194502441.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=,size_16,color_FFFFFF,t_70)

如上图，此时 p[i]=p[j]，或 p[i]=mx-i，此时 p[i] 可以继续增加，那么有：

```cpp
while(newStr[i-p[i]]==newStr[i+p[i]]) 
    p[i]++;
```

根据（1）（2）（3），容易推出 Manacher 算法的最坏情况，即为字符串内全是相同字符的时候，平均访问每个字符 5 次，同理，也容易推出 Manacher 算法的最佳情况，即为字符串内字符各不相同的时候，平均每个字符访问 4 次。

综上，Manacher算法的时间复杂度为 O(n)。

# 【实现】

```cpp
char str[N];//原字符串
char newStr[N*2];//预处理后的字符串
int p[N*2];//辅助数组
int init(){//对原字符进行预处理
    newStr[0]='$';
    newStr[1]='#';
    
    int j=2;
    int len=strlen(str);
    for (int i=0;i<len;i++){
        newStr[j++]=str[i];
        newStr[j++]='#';
    }
    newStr[j] ='\0'; //字符串结束标记
    
    return j;//返回newStr的长度
}

int manacher(){
    int len=init();//取得新字符串长度并完成字符串的预处理
    int res=-1;//最长回文长度

    int id;
    int mx=0;
    for(int i=1;i<len;i++){
        int j=2*id-i;//与i相对称的位置
        if(i<mx)
            p[i]=min(p[j], mx-i);
        else
            p[i]=1;

        //由于左有'$',右有'\0'，不需边界判断
        while(newStr[i-p[i]] == newStr[i+p[i]])//p[i]的扩大
            p[i]++;

        if(mx<i+p[i]){//由于希望mx尽可能的远，因此要不断进行比较更新
            id=i;
            mx=i+p[i];
        }
        res=max(res,p[i]-1);
    }
    return res;
}

int main(){
    while (printf("请输入字符串：\n")){
        scanf("%s",str);
        printf("最长回文长度为 %d\n", manacher());
    }
    return 0;
}
```





