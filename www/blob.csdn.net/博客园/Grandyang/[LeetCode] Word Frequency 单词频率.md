# [LeetCode] Word Frequency 单词频率 - Grandyang - 博客园







# [[LeetCode] Word Frequency 单词频率](https://www.cnblogs.com/grandyang/p/5386475.html)







Write a bash script to calculate the frequency of each word in a text file `words.txt`.

For simplicity sake, you may assume:
- `words.txt` contains only lowercase characters and space `' '` characters.
- Each word must consist of lowercase characters only.
- Words are separated by one or more whitespace characters.

For example, assume that `words.txt` has the following content:
the day is sunny the the
the sunny is is

Your script should output the following, sorted by descending frequency:
the 4
is 3
sunny 2
day 1

**Note:**

Don't worry about handling ties, it is guaranteed that each word's frequency count is unique.


[[show hint]](https://leetcode.com/problems/word-frequency/)

**Hint:**

Could you write it in one-line using [Unix pipes](http://tldp.org/HOWTO/Bash-Prog-Intro-HOWTO-4.html)?




这道题给了我们一个文本文件，让我们统计里面单词出现的个数，提示中让我们用管道Pipes来做，在之前那道[Tenth Line](http://www.cnblogs.com/grandyang/p/5376902.html)中，我们使用过管道命令，管道命令的讲解请参见[这个帖子](http://www.programgo.com/article/6462705832/;jsessionid=01A7179C41368EACBD963D21018878B2)。提示中让我们用管道连接各种命令，然后一行搞定，那么我们先来看第一种解法，乍一看啥都不明白，咋办？没关系，容我慢慢来讲解。首先用的关键字是grep命令，该命令一种强大的文本搜索工具，它能使用正则表达式搜索文本，并把匹配的行打印出来，详解请参见[这个帖子](http://www.cnblogs.com/peida/archive/2012/12/17/2821195.html)。后面紧跟的-oE '[a-z]+'参数表示原文本内容变成一个单词一行的存储方式，于是此时文本的内容就变成了：

the
day
is
sunny
the
the
the
sunny
is

下面的sort命令就是用来排序的，参见[这个帖子](http://blog.csdn.net/monkeyduck/article/details/10097829)。排完序的结果为：

day
is
is
is
sunny
sunny
the
the
the
the

后面的uniq命令是表示去除重复行命令(参见[这个帖子](http://blog.51yip.com/shell/1022.html))，后面的参数-c表示在每行前加上表示相应行目出现次数的前缀编号，得到结果如下： 

   1 day
   3 is
   2 sunny
   4 the

然后我们再sort一下，后面的参数-nr表示按数值进行降序排列，得到结果：

   4 the
   3 is
   2 sunny
   1 day  

而最后的awk命令就是将结果输出，两列颠倒位置即可：



解法一：

```
grep -oE '[a-z]+' words.txt | sort | uniq -c | sort -nr | awk '{print $2" "$1}'
```





下面这种方法用的关键字是tr命令，该命令主要用来进行字符替换或者大小写替换，详解请参见[这个帖子](http://www.cnblogs.com/leaven/archive/2011/01/05/1926194.html)。后面紧跟的-s参数表示如果发现连续的字符，就把它们缩减为1个，而后面的‘ ’和‘\n'就是空格和回车，意思是把所有的空格都换成回车，那么第一段命令tr -s ' ' '\n' < words.txt 就好理解了，将单词之间的空格都换成回车，跟上面的第一段实现的作用相同，后面就完全一样了，参见上面的讲解：



解法二：

```
tr -s ' ' '\n' < words.txt | sort | uniq -c | sort -nr | awk '{print $2, $1}'
```





下面这种方法就没有用管道命令进行一行写法了，而是使用了强大的文本分析工具awk进行写类C的代码来实现，这种写法在之前的那道[Transpose File](http://www.cnblogs.com/grandyang/p/5382166.html)已经讲解过了，这里就不多说了，最后要注意的是sort命令的参数-nr -k 2表示按第二列的降序数值排列：



解法三：

```
awk '{
    for (i = 1; i <= NF; ++i) ++s[$i];
} END {
    for (i in s) print i, s[i];
}' words.txt | sort -nr -k 2
```



参考资料：

[https://leetcode.com/discuss/33353/my-accepted-answer-using-tr-sort-uniq-and-awk](https://leetcode.com/discuss/33353/my-accepted-answer-using-tr-sort-uniq-and-awk)

[https://leetcode.com/discuss/46976/my-ac-solution-one-pipe-command-but-cost-20ms](https://leetcode.com/discuss/46976/my-ac-solution-one-pipe-command-but-cost-20ms)

[https://leetcode.com/discuss/29001/solution-using-awk-and-pipes-with-explaination](https://leetcode.com/discuss/29001/solution-using-awk-and-pipes-with-explaination)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












