# 论文阅读：《BLEU: a Method for Automatic Evaluation of Machine Translation》 - Soul Joy Hub - CSDN博客

2018年05月31日 17:09:23[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：349


[https://blog.csdn.net/qq_21190081/article/details/53115580](https://blog.csdn.net/qq_21190081/article/details/53115580)

论文地址：[http://xueshu.baidu.com/s?wd=paperuri%3A%2888a98dec5bea94cca9f474db30c36319%29&filter=sc_long_sign&tn=SE_xueshusource_2kduw22v&sc_vurl=http%3A%2F%2Fciteseer.ist.psu.edu%2Fviewdoc%2Fdownload%3Bjsessionid%3DF4B7103527B9E68CE036BB1F77EB78BD%3Fdoi%3D10.1.1.19.9416%26rep%3Drep1%26type%3Dpdf&ie=utf-8&sc_us=137105618768529979](http://xueshu.baidu.com/s?wd=paperuri%3A%2888a98dec5bea94cca9f474db30c36319%29&filter=sc_long_sign&tn=SE_xueshusource_2kduw22v&sc_vurl=http%3A%2F%2Fciteseer.ist.psu.edu%2Fviewdoc%2Fdownload%3Bjsessionid%3DF4B7103527B9E68CE036BB1F77EB78BD%3Fdoi%3D10.1.1.19.9416%26rep%3Drep1%26type%3Dpdf&ie=utf-8&sc_us=137105618768529979)

# 1. 引言

在牵涉到语句生成尤其是机器翻译的应用领域，如何衡量生成语句与参考语句之间的相似性是一个很重要的问题，而在2002年Kishore Papineni et al.就提出了一个经典的衡量标准Bleu，如今这篇文献已经引用量过万，因此是NLP领域必读文章之一。

# 2. 论文中使用的例子

论文中给出了四个例子来辅助解释算法，每个例子都有待评价(Candidate)语句和标准参考(Reference)语句。
- Example 1.

```
Candidate 1：It is a guide to action which ensures that the military always obeys the commands of the party.

Candidate 2: It is to insure the troops forever hearing the activity guidebook that party direct.

Reference 1: It is a guide to action that ensures that the military will forever heed Party commands.

Reference 2: It is the guiding principle which guarantees the military forces always being under the command of the Party.

Reference 3: It is the practical guide for the army always to heed the directions of the party .
```
- Example 2.

```
Candidate: the the the the the the the.

Reference 1: The cat is on the mat.

Reference 2: There is a cat on the mat.
```
- Example 3.

```
Candidate: of the

Reference 1: It is a guide to action that ensures that the military will forever heed Party commands.

Reference 2: It is the guiding principle which guarantees the military forces always being under the command of the Party.

Reference 3: It is the practical guide for the army always to heed the directions of the party.
```
- Example 4.

```
Candidate 1: I always invariably perpetually do.

Candidate 2: I always do.

Reference 1: I always do.

Reference 2: I invariably do.

Reference 3: I perpetually do.
```

# 3. Bleu方法使用的基本度量指标和概念

## 3.1 “n单位片段” (n-gram)

所谓n-gram就是指一个语句里面连续的n个单词组成的片段，一个18单词的语句有18个1-gram，每个单词都睡一个1-gram；有17个2-gram，这个很好理解。

## 3.2 精确度（Precision）和“修正的n-单位精确度”(modified n-gram recision)

**Precision是指Candidate语句里面的n-gram在所有Reference语句里面出现的概率。**

在Example 1.的Candidate 1 语句中，18个单词共有17个单词出现过，所以1-gram的precision是17/18,17个2-gram片段总共有10个出现过，所以2-gram的precision是10/17。同理有，Example 2.的Candidate 语句1-gram的Precision是7/7。

但是以上方法存在一个问题，就是可能Reference里面的单词会被重复利用，这是不合理的。所以有了“修正的n-单位精确度”(modified n-gram recision)，主要思路是Reference语句里面如果一个单词片段已经被匹配，那么这个片段就不能再次被匹配，并且一个单词片段只能取一个Reference语句中出现次数的最大值，比如7个the分别在Reference 1 和 2中出现2和1次，所以取2而不是两者相加的3。

利用以上方法，每一个句子都可以得到一个modified n-gram recision，一个句子不能代表文本翻译的水平高低，于是把一段话或者所有翻译句子的结果综合起来可以得到$p_n$
![](https://upload-images.jianshu.io/upload_images/1621805-4d2ab659e01115b4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

# 4. BP值(Brevity Penalty)和BLEU值的计算公式

上面我们已经介绍了modified n-gram precision，对于不同的长度n都会有一个pn，那么如何将不同n的pn结合起来得到最终的Bleu值。研究者们还考虑到一种情况，就是待测译文翻译不完全不完整的情况，这个问题在机器翻译中是不能忽略的，而简单的pn值不能反映这个问题，例如Example 3。

这个问题也不能用recall来解决，例如Example 4. 显然Candidate 1的回召率比Candidate 2要高，但是显然Candidate 1的翻译不如Candidate 2。所以recall并不能解决这个问题。

首先引入BP值，作者指定当待评价译文同任意一个参考译文长度相等或超过参考译文长度时，BP值为1，当待评价译文的长度较短时，则用一个算法得出BP值。以c来表示待评价译文的长度，r来表示参考译文的文字长度，则 
![](https://upload-images.jianshu.io/upload_images/1621805-86dbdc3fb038629a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)

通常这个N取4，$w_n=1/4$，这就是很多论文里面的一个经典指标Bleu4

