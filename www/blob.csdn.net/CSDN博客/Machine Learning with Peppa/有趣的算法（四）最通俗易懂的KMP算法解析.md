# 有趣的算法（四）最通俗易懂的KMP算法解析 - Machine Learning with Peppa - CSDN博客





2018年02月26日 21:01:50[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：405








## 最通俗易懂的KMP算法解析

### 1.1 算法定义

    Knuth-Morris-Pratt 字符串查找算法，简称为 “KMP算法”，常用于在一个文本串S内查找一个模式串P 的出现位置，这个算法由Donald Knuth、Vaughan Pratt、James H. Morris三人于1977年联合发表，故取这3人的姓氏命名此算法。





## 1.2 算法目标

kmp算法完成的任务是：给定两个字符串O和f，长度分别为n和m，判断f是否在O中出现，如果出现则返回出现的位置。常规方法是遍历a的每一个位置，然后从该位置开始和b进行匹配，但是这种方法的复杂度是O(nm)。kmp算法通过一个O(m)的预处理，使匹配的复杂度降为O(n+m)。



### 1.3 算法思想

我们首先用一个例子来描述kmp算法的思想。当匹配到位置i时两个字符串不相等，这时我们需要将字符串向前移动。常规方法是每次向前移动一位，但是它没有考虑前i-1位已经比较过这个事实，所以效率不高。事实上，如果我们提前计算某些信息，就有可能一次前移多位。

我们根据已经获得的信息知道可以前移k位，并得出一个结论：移动K位大大简化了时间复杂度，充分利用了目标字符串ptr的性质（比如里面部分字符串的重复性，即使不存在重复字段，在比较时，实现最大的移动量）。


KMP算法的核心，是一个被称为部分匹配表(Partial Match Table)的数组。我觉得理解KMP的最大障碍就是很多人在看了很多关于KMP的文章之后，仍然搞不懂PMT中的值代表了什么意思。这里我们抛开所有的枝枝蔓蔓，先来解释一下这个数据到底是什么。

对于字符串“abababca”，它的PMT如下表所示：

![](https://img-blog.csdn.net/20180226204728373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

就像例子中所示的，如果待匹配的模式字符串有8个字符，那么PMT就会有8个值。

我先解释一下字符串的前缀和后缀。如果字符串A和B，存在A=BS，其中S是任意的非空字符串，那就称B为A的前缀。例如，”Harry”的前缀包括{”H”, ”Ha”, ”Har”, ”Harr”}，我们把所有前缀组成的集合，称为字符串的前缀集合。同样可以定义后缀A=SB， 其中S是任意的非空字符串，那就称B为A的后缀，例如，”Potter”的后缀包括{”otter”, ”tter”, ”ter”, ”er”, ”r”}，然后把所有后缀组成的集合，称为字符串的后缀集合。要注意的是，字符串本身并不是自己的后缀。

有了这个定义，就可以说明PMT中的值的意义了。**PMT中的值是字符串的前缀集合与后缀集合的交集中最长元素的长度**。例如，对于”aba”，它的前缀集合为{”a”, ”ab”}，后缀 集合为{”ba”, ”a”}。两个集合的交集为{”a”}，那么长度最长的元素就是字符串”a”了，长 度为1，所以对于”aba”而言，它在PMT表中对应的值就是1。再比如，对于字符串”ababa”，它的前缀集合为{”a”, ”ab”, ”aba”, ”abab”}，它的后缀集合为{”baba”, ”aba”, ”ba”, ”a”}， 两个集合的交集为{”a”, ”aba”}，其中最长的元素为”aba”，长度为3。 

好了，解释清楚这个表是什么之后，我们再来看如何使用这个表来加速字符串的查找，以及这样用的道理是什么。这里先引用一个有趣的例子：



甲：abbaabbaaba


在里面寻找
乙：abbaaba
发现第 7 个字符不匹配。
这时候甲把乙叫走了，说你先一边玩去，我自己研究下。

然后甲想，自己已经知道乙的前 6 个字符就是自己的前 6 个字符，不妨先「自己与自己匹配一番」，即用 abbaab 去匹配 abbaab。

向右错 1 个位，发现第一个就不一样（不匹配）；向右错 2 个位，还是不匹配。
当错 3 个位的时候，甲发现匹配了一个 a，但是第二个 b 不匹配。
当错 4 个位的时候，匹配了两个，错 5 个位不匹配。后面的东西甲就不知道了，因为他只知道前 6 个字符。

随后，甲把乙叫了过来：
「我已经知道你下一次匹配开始的位置了，来，让你的头部对齐我的第 5 个字符，然后从你的第 3 个字符开始继续匹配我吧！」

关键的地方，在于不要让乙「前功尽弃」——已经匹配了 6 个了，还差一个就结束了，这时不匹配导致从 0 开始，多可惜啊！
现在我告诉你，在不匹配的情况下，你仍然已经匹配了 2 个（乙内心：还好不是 0），并且你可以继续从不匹配的地方开始比较，即用你的第 3 个字符与我继续匹配。
那，这个 2 你是怎么算的？
我在你来之前就算好啦！
**我先与自己进行匹配（预处理），对每个子字符串 [0...i]，找其「相匹配的前缀与后缀中，最长的那个字符串的长度」。**
UCCU，从你的第 6 个字符往左看，恰好 [ab] 与你的前缀 [ab] 匹配，但是我的第 7 个字符并不知道你的第 3 个字符是否与我一样，所以你直接从这里开始继续匹配我。





因此，回到图 1.12 所示，要在主字符串"ababababca"中查找模式字符串"abababca"。如果在 j 处字符不匹配，那么由于前边所说的模式字符串 PMT 的性质，主字符串中 i 指针之前的 PMT[j −1] 位就一定与模式字符串的第 0 位至第 PMT[j−1] 位是相同的。这是因为主字符串在 i 位失配，也就意味着主字符串从 i−j 到 i 这一段是与模式字符串的 0 到 j 这一段是完全相同的。而我们上面也解释了，模式字符串从 0 到 j−1 ，在这个例子中就是”ababab”，其前缀集合与后缀集合的交集的最长元素为”abab”， 长度为4。所以就可以断言，主字符串中i指针之前的 4 位一定与模式字符串的第0位至第 4 位是相同的，即长度为 4 的后缀与前缀相同。这样一来，我们就可以将这些字符段的比较省略掉。具体的做法是，保持i指针不动，然后将j指针指向模式字符串的PMT[j −1]位即可。

简言之，以图中的例子来说，在 i 处失配，那么主字符串和模式字符串的前边6位就是相同的。又因为模式字符串的前6位，它的前4位前缀和后4位后缀是相同的，所以我们推知主字符串i之前的4位和模式字符串开头的4位是相同的。就是图中的灰色部分。那这部分就不用再比较了。

![](https://img-blog.csdn.net/20180226204718923?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

有了上面的思路，我们就可以使用PMT加速字符串的查找了。我们看到如果是在 j 位 失配，那么影响 j 指针回溯的位置的其实是第 j −1 位的 PMT 值，所以为了编程的方便， 我们不直接使用PMT数组，而是将PMT数组向后偏移一位。我们把新得到的这个数组称为next数组。下面给出根据next数组进行字符串匹配加速的字符串匹配程序。其中要注意的一个技巧是，在把PMT进行向右偏移时，第0位的值，我们将其设成了-1，这只是为了编程的方便，并没有其他的意义。在本节的例子中，next数组如下表所示。







### 1.4 算法步骤


- ①寻找前缀后缀最长公共元素长度- 对于P = p0 p1 ...pj-1 pj，寻找模式串P中长度最大且相等的前缀和后缀。如果存在p0 p1 ...pk-1 pk = pj- k pj-k+1...pj-1 pj，那么在包含pj的模式串中有最大长度为k+1的相同前缀后缀。举个例子，如果给定的模式串为“abab”，那么它的各个子串的前缀后缀的公共元素的最大长度如下表格所示：



> 
![](https://img-blog.csdn.net/20140819173639786)

比如对于字符串aba来说，它有长度为1的相同前缀后缀a；而对于字符串abab来说，它有长度为2的相同前缀后缀ab（相同前缀后缀的长度为k + 1，k + 1 = 2）。


- ②求next数组- next 数组考虑的是当前字符之前的字符串前后缀的相似度，所以通过第①步骤求得各个前缀后缀的公共元素的最大长度后，只要稍作变形即可：将第①步骤中求得的值整体右移一位，然后初值赋为-1，如下表格所示：



> 
![](https://img-blog.csdn.net/20140819173649739)

比如对于aba来说，第3个字符a之前的字符串ab中有长度为0的相同前缀后缀，所以第3个字符a对应的next值为0；而对于abab来说，第4个字符b之前的字符串aba中有长度为1的相同前缀后缀a，所以第4个字符b对应的next值为1（相同前缀后缀的长度为k，k = 1）。


- ③根据next数组进行匹配- 匹配失配，j = next [j]，模式串向右移动的位数为：j - next[j]。换言之，当模式串的后缀pj-k pj-k+1, ..., pj-1 跟文本串si-k si-k+1, ..., si-1匹配成功，但pj 跟si匹配失败时，因为next[j] = k，相当于在不包含pj的模式串中有最大长度为k 的相同前缀后缀，即p0 p1 ...pk-1 = pj-k pj-k+1...pj-1，故令j = next[j]，从而让模式串右移j - next[j] 位，使得模式串的前缀p0 p1, ..., pk-1对应着文本串 si-k si-k+1, ..., si-1，而后让pk 跟si 继续匹配。如下图所示：（该图形象地解释了为什么j=next[j]，实质上就是模式串向右移动j-next[j]位.）



> 
> ![](https://img-blog.csdn.net/20140812223633281)





    综上，KMP的next 数组相当于告诉我们：当模式串中的某个字符跟文本串中的某个字符匹配失配时，模式串下一步应该跳到哪个位置。如模式串中在j 处的字符跟文本串在i 处的字符匹配失配时，下一步用next [j] 处的字符继续跟文本串i 处的字符匹配，相当于模式串向右移动 j - next[j] 位。










### 1.4 算法实现



JAVA

```java
int[] calc_max_match_lengths(String pattern) {
    int[] max_match_lengths = new int[pattern.length()];
    int max_length = 0;
    for (int i = 1; i < pattern.length(); i++) {
        while (max_length > 0 && pattern.charAt(max_length) != pattern.charAt(i)) {
            max_length = max_match_lengths[max_length - 1]; // ①
        }
        if (pattern.charAt(i) == pattern.charAt(max_length)) {
            max_length++; // ②
        }
        max_match_lengths[i] = max_length;
    }
    return max_match_lengths;
}
```

```java
// 在 text 中寻找 pattern，返回所有匹配的位置开头
List<Integer> search(String text, String pattern) {
    List<Integer> positions = new ArrayList<>();
    int[] max_match_lengths = calc_max_match_lengths(pattern);
    int count = 0;
    for (int i = 0; i < text.length(); i++) {
        while (count > 0 && pattern.charAt(count) != text.charAt(i)) {
            count = max_match_lengths[count - 1];
        }
        if (pattern.charAt(count) == text.charAt(i)) {
            count++;
        }
        if (count == pattern.length()) {
            positions.add(i - pattern.length() + 1);
            count = max_match_lengths[count - 1];
        }
    }
    return positions;
}
```




C/C++

```cpp
void cal_next(char *str, int *next, int len)
{
    next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
    int k = -1;//k初始化为-1
    for (int q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
        {
            k = next[k];//往前回溯
        }
        if (str[k + 1] == str[q])//如果相同，k++
        {
            k = k + 1;
        }
        next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
    }
}
```

```cpp
int KMP(char *str, int slen, char *ptr, int plen)
{
    int *next = new int[plen];
    cal_next(ptr, next, plen);//计算next数组
    int k = -1;
    for (int i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])//ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
            k = next[k];//往前回溯
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)//说明k移动到ptr的最末端
        {
            //cout << "在位置" << i-plen+1<< endl;
            //k = -1;//重新初始化，寻找下一个
            //i = i - plen + 1;//i定位到该位置，外层for循环i++可以继续找下一个（这里默认存在两个匹配字符串可以部分重叠），感谢评论中同学指出错误。
            return i-plen+1;//返回相应的位置
        }
    }
    return -1;  
}
```




参考文章：

1. [如何理解KMP算法](https://www.zhihu.com/question/21923021) 作者：海纳，[逍遥行](https://www.zhihu.com/people/endlesscheng)

2. [KMP算法最浅显易懂的解释](http://blog.csdn.net/starstar1992/article/details/54913261)

3. [KMP算法的优化与分析](https://www.cnblogs.com/cherryljr/p/6519748.html)

4. [KMP算法详解](http://blog.csdn.net/yutianzuijin/article/details/11954939)






