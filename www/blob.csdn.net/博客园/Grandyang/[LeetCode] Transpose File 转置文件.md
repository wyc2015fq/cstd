# [LeetCode] Transpose File 转置文件 - Grandyang - 博客园







# [[LeetCode] Transpose File 转置文件](https://www.cnblogs.com/grandyang/p/5382166.html)







Given a text file `file.txt`, transpose its content.

You may assume that each row has the same number of columns and each field is separated by the `' '` character.

For example, if `file.txt` has the following content:
name age
alice 21
ryan 30



Output the following:
name alice ryan
age 21 30


这道题让我们转置一个文件，其实感觉就是把文本内容当做了一个矩阵，每个单词空格隔开看做是矩阵中的一个元素，然后将转置后的内容打印出来。那么我们先来看使用awk关键字的做法，关于awk的介绍可以参见[这个帖子](http://tanxin.blog.51cto.com/6114226/1222140)。其中NF表示当前记录中的字段个数，就是有多少列，NR表示已经读出的记录数，就是行号，从1开始。那么在这里NF是2，因为文本只有两列，这里面这个for循环还跟我们通常所熟悉for循环不太一样，通常我们以为i只能是1和2，然后循环就结束了，而这里的i实际上遍历的数字为1,2,1,2,1,2，我们可能看到实际上循环了3遍1和2，而行数正好是3，可能人家就是这个机制吧。知道了上面这些，那么下面的代码就不难理解了，遍历过程如下：

i = 1, s = [name]

i = 2, s = [name; age]

i = 1, s = [name alice; age]

i = 2, s = [name alice; age 21]

i = 1, s = [name alice ryan; age 21]

i = 2, s = [name alice ryan; age 21 30]

然后我们再将s中的各行打印出来即可，参见代码如下：



解法一：

```
awk '{
    for (i = 1; i <= NF; ++i) {
        if (NR == 1) s[i] = $i;
        else s[i] = s[i] " " $i;
    }
} END {
    for (i = 1; s[i] != ""; ++i) {
        print s[i];
    }
}' file.txt
```



下面这种方法和上面的思路完全一样，但是代码风格不一样，上面是C语言风格，而这个完全就是Bash脚本的风格了，我们用read关键字，我们可以查看read的用法read: usage: read [-ers] [-u fd] [-t timeout] [-p prompt] [-a array] [-n nchars] [-d delim] [name ...]。那么我们知道-a表示数组，将读出的每行内容存入数组line中，那么下一行for中的一堆特殊字符肯定让你头晕眼花，关于shell中的特殊变量可以参见[这个帖子](http://c.biancheng.net/cpp/view/2739.html)，其实我也不能算特别理解下面的代码，大概觉得跟上面的思路一样，求大神来具体给讲解下哈：



解法二：

```
while read -a line; do
    for ((i = 0; i < "${#line[@]}"; ++i)); do
        a[$i]="${a[$i]} ${line[$i]}"
    done
done < file.txt
for ((i = 0; i < ${#a[@]}; ++i)); do
    echo ${a[i]}
done
```



参考资料：

[https://leetcode.com/problems/transpose-file/](https://leetcode.com/problems/transpose-file/)

[https://leetcode.com/problems/transpose-file/discuss/55522/AC-Solution%3A-8-lines-only-in-pure-Bash](https://leetcode.com/problems/transpose-file/discuss/55522/AC-Solution%3A-8-lines-only-in-pure-Bash)

[https://leetcode.com/problems/transpose-file/discuss/55502/AC-solution-using-awk-and-statement-just-like-C.](https://leetcode.com/problems/transpose-file/discuss/55502/AC-solution-using-awk-and-statement-just-like-C.)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












