# 自然数序列，找出任意连续之和等于n的所有子序列 - Arthur的随笔 - CSDN博客
2012年02月02日 10:24:09[largetalk](https://me.csdn.net/largetalk)阅读数：3165
```python
lst = [0,2,4,5,3,1,8,6,4,7,9,3,2]
total_sum = 9
def sum_seq(lst):
    seq = []
    ct = 0
    for x in lst:
        seq.append(x)
        ct += x
        if ct == total_sum:
            print seq
            continue
        if ct < total_sum:
            continue
        if ct > total_sum:
            seq_len = len(seq)
            for i in range(seq_len):
                tr = seq.pop(0)
                ct -= tr
                if ct < total_sum:
                    break
                if ct == total_sum:
                    print seq
sum_seq(lst)
```
