# KMP Algorithm 字符串匹配算法KMP小结 - Grandyang - 博客园







# [KMP Algorithm 字符串匹配算法KMP小结](https://www.cnblogs.com/grandyang/p/6992403.html)







这篇小结主要是参考这篇帖子[从头到尾彻底理解KMP](http://blog.csdn.net/v_july_v/article/details/7041827)，不得不佩服原作者，写的真是太详尽了，让博主产生了一种读学术论文的错觉。后来发现原作者是写书的，不由得更加敬佩了。博主不才，尝试着简化一些原帖子的内容，希望能更通俗易懂一些。博主的帖子一贯秉持通俗易懂的风格，使得非CS专业的人士也能读懂，至少博主自己是这么认为的-.-|||

KMP算法，全称Knuth-Morris-Pratt算法，根据三个作者Donald Knuth、Vaughan Pratt、James H. Morris的姓氏的首字母拼接而成的。是一种字符串匹配的算法，用于在一个文本串S中查找模式串P的位置。在讲解KMP算法之前，我们先来看暴力破解法是如何运作的，假如我们有一个文本串S和一个模式串P如下：

文本串: BBC_ABCDAB_ABCDABCDABDE

模式串: ABCDABD

那么我们首先来找模式串的第一个字母A在文本串出现的位置：

```
BBC_ABCDAB_ABCDABCDABDE
    ABCDABD
```

找到后，再来一一比较后面的字母，比较到模式串的D的位置，发现不匹配:

```
BBC_ABCDAB_ABCDABCDABDE
    ABCDABD
```

暴力破解的下一步是将模式串后移一步，继续来匹配开头的A

```
BBC_ABCDAB_ABCDABCDABDE
     ABCDABD
```

直到找到下一个A，然后开始往后一一比较:

```
BBC_ABCDAB_ABCDABCDABDE
        ABCDABD
```

后面的步骤就不一一列举了，都是按这种方法来查找的，这种算法十分的不高效，时间复杂度是O(m*n)，其中m和n分别是文本串和模式串的长度。当m和n都很大的时候，运算速度就会很慢，那么此时就有请KMP算法闪亮登场！！

我们再回到暴力破解方法中的一一比较后面的字母那一步，比较到模式串的D的位置，发现不匹配:

```
BBC_ABCDAB_ABCDABCDABDE
    ABCDABD
```

此时KMP算法并不是将模式串向右移动一位，而是向后移动四位，直接到这一步：

```
BBC_ABCDAB_ABCDABCDABDE
        ABCDABD
```

这样文本串的遍历位置并不会移回去，而是'_'直接跟'C'匹配，是不是很神奇，它怎么知道要跟模式串上的哪个字符相比呢，实际上是从next数组中查的值，再讲解next数组之前，我们先来讲一下最大前缀后缀公共元素。

所谓最大前缀后缀公共元素，就是模式串中最大且相等的前缀和后缀，比如aba，有长度为1的相同前缀后缀a，再比如，字符串acdac有长度为2的相同前缀后缀ac，那么我们可以写出ABCDABD的每一位上的前缀后缀长度：

```
A   B   C   D   A   B   D
0   0   0   0   1   2   0
```

由于模式串的尾部可能有重复的字符，所以我们可以得出一个重要的结论：**失配时，模式串向右移动的距离 = 已匹配字符数 - 失配字符的上一位字符所对应的最大长度值**

我们之前是在字符'D'处失配的，上一位字符是'B'，对应的最大长度是2，此时已经成功匹配了6个字符，那么我们就将模式串向右移动6-2=4位，并继续匹配即可。

```
BBC_ABCDAB_ABCDABCDABDE
        ABCDABD
```

此时我们发现'_'和'C'不匹配，那么'C'的上一个字符'B'的最大长度为0，此时已经匹配了2个字符，所以模式串向右移动2-0=2位继续匹配，得到：

```
BBC_ABCDAB_ABCDABCDABDE
          ABCDABD
```

此时发现'_'和'A'不匹配，'A'已经是第一个了，不需要查表了，此时将模式串向右移动一位：

```
BBC_ABCDAB_ABCDABCDABDE
           ABCDABD
```

发现此时模式串的首字母'A'匹配上了，然后就按顺序一路往下匹配，直到最后一个'D'和'C'失配：

```
BBC_ABCDAB_ABCDABCDABDE
           ABCDABD
```

我们进行和之前相似的操作，上一位字符是'B'，对应的最大长度是2，此时已经成功匹配了6个字符，那么我们就将模式串向右移动6-2=4位，并继续匹配即可:

```
BBC_ABCDAB_ABCDABCDABDE
               ABCDABD
```

移动后发现模式串的首字母'A'匹配上了，然后就按顺序一路往下匹配，最终完成模式串的匹配:

```
BBC_ABCDAB_ABCDABCDABDE
               ABCDABD
```

我们发现文本串中的遍历位置始终没有退后，一直都是在向前的，这样使得其比暴力破解法节省了大量的时间，其时间复杂度为O(m+n)，简直碉堡了。读到这里是不是有疑问，怎么算法都结束了，还没next数组什么事呢，其实next数组和这里的最大前缀后缀公共元素长度数组是有关联的，上面的方法在失配时，要找失配字符前一个字符的最大前缀后缀公共元素长度值，那么如果我们将最大前缀后缀公共元素长度数组整体右移一位，形成next数组，如下所示：

```
A   B   C   D   A   B   D
0   0   0   0   1   2   0
-1  0   0   0   0   1   2
```

上面的中间那行是之前的最大前缀后缀公共元素长度数组，我们将其整体右移一位，多出的位置补上一个-1，就变成了下面的一行。那么我们此时就直接找失配字符的next值就行了。于是我们就得到了新的结论：**失配时，模式串向右移动的距离 = 失配字符所在位置 - 失配字符对应的next值。**

读到这里是不是对KMP算法的发明者佩服的五体投地，别着急，还剩最后一部分，就是用代码来递推计算next数组。对于next的数组的计算，可以采用递推来算。根据上面的分析，我们知道如果模式串当前位置j之前有k个相同的前缀后缀，那么可以表示为next[j] = k，所以如果当模式串的p[j]跟文本串失配后，我们可以用next[j]处的字符继续和文本串匹配，相当于模式串向右移动了j - next[j]位。那么问题就来了，如何求出next[j+1]的值呢，我们还是来看例子吧：

```
模式串：    A  B  C  D  A  B  C  E
next值：   -1  0  0  0  0  1  2  ?  
索引：             k           j
```

如上所示，模式串为"ABCDABCE"，且j=6, k = 2，我们有next[j] = k，这表示j位置上的字符C之前的最大前后缀长度为2，即AB。现在我们要求next[j+1]的值，因为p[k] == p[j]，所以next[j+1] = next[j] + 1 = k + 1 = 3。即字母E之前的最大前后缀长度为3，即ABC。

那么我们再来看p[k] != p[j]的情况下怎么处理，还是来看例子：

```
模式串：    A  B  C  D  A  B  D  E
next值：   -1  0  0  0  0  1  2  ?  
索引：             k           j
```

这个例子把上面例子中的第二个'C'换成了'D'，所以字符'E'前面的相同后缀就不再是3了，所以我们希望在k前面找出个k'位置，使得p[k']为D，这样next[j+1] = k' +1，但是这个例子中不存在这样的'D'，所以next[j+1] = 0。我们看一个能在前缀中找到'D'的例子：

```
模式串：    D  A  B  C  D  A  B  D  E
next值：   -1  0  0  0  0  1  2  3  ?  
索引：                k           j
```

这个例子上面例子的最前面加上了个'D'，此时j = 7, k = 3了，我们有next[j] = k，这表示j位置上的字符3之前的最大前后缀长度为3，即DAB。要求next[j+1]的值，我们发现此时p[k] != p[j]，然后我们让k = next[k] = 0，此时p[0]是D，那么next[j+1] = k + 1 = 1了，这说明字母E之前的最大前后缀长度为1，即D。综上所述，我们可以写出next的生成函数如下：



```
vector<int> getNext(string p) {
    int n = p.size(), k = -1, j = 0;
    vector<int> next(n, -1);
    while (j < n - 1) {
        if (k == -1 || p[j] == p[k]) {
            ++k; ++j;
            next[j] = k;
        } else {
            k = next[k];
        }
    }
    return next;
}
```



上面这种计算next数组的方式可以进一步的优化，可以优化的原因是因为上面的方法存在一个小小的问题，如果用这种方法求模式串ABAB，会得到next数组为[-1 0 0 1]，我们用这个模式串去匹配ABACABABC:

```
ABACABABC
ABAB
```

我们会发现C和B失配，那么根据上面的规则，我们要向右移动j - next[j] = 3 - 1 = 2位，于是有：

```
ABACABABC
  ABAB
```

我们右移两位后发现又是C和B失配了，而我们在上一步中，已知p[3] = B, s[3] = C，就已经失配了，让p[next[3]] = p[1] = B再去和s[3]比较，肯定还是失配。原因是当p[j] != s[i]时，下一步要用p[next[j]]和s[i]去匹配，而如果p[j] == p[next[j]]了，再用p[next[j]]和s[i]去匹配必然会失配。所以我们要避免出现p[j] == p[next[j]]的情况，一旦出现了这种情况，我们可以再次递归，next[j] = next[next[j]]，修改后的代码如下：



```
vector<int> getNext(string p) {
    int n = p.size(), k = -1, j = 0;
    vector<int> next(n, -1);
    while (j < n - 1) {
        if (k == -1 || p[j] == p[k]) {
            ++k; ++j;
            next[j] = (p[j] != p[k]) ? k : next[k];
        } else {
            k = next[k];
        }
    }
    return next;
}
```



讲到这里，KMP算法的内容就完全讲完了，原帖子中还有两个扩展方法，这里就不讲了，感觉能把上述内容吃透就很不容易了，下面贴上完整的KMP的代码仅供参考：



```
#include <iostream>
#include <vector>

using namespace std;

vector<int> getNext(string p) {
    int n = p.size(), k = -1, j = 0;
    vector<int> next(n, -1);
    while (j < n - 1) {
        if (k == -1 || p[j] == p[k]) {
            ++k; ++j;
            next[j] = (p[j] != p[k]) ? k : next[k];
        } else {
            k = next[k];
        }
    }
    return next;
}

int kmp(string s, string p) {
    int m = s.size(), n = p.size(), i = 0, j = 0;
    vector<int> next = getNext(p);
    while (i < m && j < n) {
        if (j == - 1 || s[i] == p[j]) {
            ++i; ++j;
        } else {
            j = next[j];
        }
    }
    return (j == n) ? i - j : -1;
}

int main() {
    cout << kmp("BBC_ABCDAB_ABCDABCDABDE", "ABCDABD") << endl; // Output: 15
}
```



参考资料：

[http://blog.csdn.net/v_july_v/article/details/7041827](http://blog.csdn.net/v_july_v/article/details/7041827)



转载请注明出处：来自Grandyang的博客园：[http://www.cnblogs.com/grandyang/p/6992403.html](http://www.cnblogs.com/grandyang/p/6992403.html)












