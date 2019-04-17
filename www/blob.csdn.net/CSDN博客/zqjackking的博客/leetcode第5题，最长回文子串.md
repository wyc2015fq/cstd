# leetcode第5题，最长回文子串 - zqjackking的博客 - CSDN博客





2017年02月17日 19:04:02[zqjackking](https://me.csdn.net/zqjackking)阅读数：348








```
一开始用的穷举法，时间直接超了，后来稍作修改才被accept，但是只是打败了70%的人。
后来找到了大神写的代码，现在直接粘上如下：
```

```
if (s.size() < 2)
            return s;
        int len = s.size(), max_left = 0, max_len = 1, left, right;
        for (int start = 0; start < len && len - start > max_len/2;) 
        {
            left = right = start;
            while (right < len - 1 && s[right + 1] == s[right])
                ++right;
            start = right + 1;
            while (right < len - 1 && left > 0 && s[right + 1] == s[left - 1]) {
                ++right;
                --left;
            }
            if (max_len < right - left + 1) {
                max_left = left;
                max_len = right - left + 1;
            }
        }
        return s.substr(max_left, max_len);
```

start每次从上次的没有重复的right+1开始。 

while (right < len - 1 && s[right + 1] == s[right]) 

                ++right; 

      这个循环语句是考虑有连续字符的情况，比如“cbbd”，如果没有这个循环 那么最后找到的答案是”c”，而不是“bb”。right一直加到没有相等字符为止（不管是奇数个bbb还是偶数个bb）都是回文。 

     再次考虑这个while循环的意义，如果连续字符的个数是奇数，比如说“cbbbd”，没有这个循环的话倒也检测得出来(从start等于2开始检测，可以检出中间的bbb），可是如果连续字符的个数是偶数，比如说“cbbd”，那么没有这个循环就检测不出来了。如果是”cbbbbd”，那么只能检测出三个。 

    综上，这个循环是很重要的。然后start每次都要在right的基础上加1。 

    后半部分的代码则很好理解了。 

    需要注意的是,for循环里的条件,不能将len-start>maxlen/2换成len-start>maxlen.否则的话,“kanana”返回的结果是”ana”,因为在start=3的时候len-start=3,而maxlen也等于3。 

     其实可以删去len-start>maxlen/2这句话，还是可以accepted。只不过最后会浪费一些时间。 

     这里的len-start>maxlen/2，maxlen/2选取的也很有讲究。从start开始，剩下的字符串长度就是len-start,那么可能的回文串就是两种情况，要么是len-start(连续的字符），要么是2(len-start)-1,所以要保证len-start>maxlen,或者2(len-start)-1>maxlen。而这里实际取了比这两个条件还要宽松的条件。也就是len-start>maxlen/2。就是为了宁愿浪费时间也不要漏掉。



