# SQL SERVER读书笔记：JOIN - 左直拳的马桶_日用桶 - CSDN博客
2013年09月12日 00:05:16[左直拳](https://me.csdn.net/leftfist)阅读数：1521
nested loop join：适用于小数据集，有索引的情况。不占用内存，不用tempdb。
merge join：大数据，要排序，多对多，用tempdb；
hash join：对大数据集，少用户使用，尤其是数据仓库最为适用。消耗资源厉害，占用大量内存，使用tempdb。
