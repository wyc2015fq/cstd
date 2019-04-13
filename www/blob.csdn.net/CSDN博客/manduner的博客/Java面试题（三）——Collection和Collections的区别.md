
# Java面试题（三）——Collection和Collections的区别 - manduner的博客 - CSDN博客


2018年08月01日 16:33:09[Manduner_TJU](https://me.csdn.net/manduner)阅读数：233


# 1，区别
Collection，是单列集合的接口，有子接口List和Set
Collections，是针对集合操作的工具类，其中包含对集合进行排序和二分查找的方法
# 2，Collections常用方法
public static <T> void sort (List<T> list)：排序，默认情况下是自然排序
public static <T> int binarySearch (List<?> list, T key)：二分查找
public static <T> T max (Collection<?> coll)：最大值（最小值类似用法）
public static void reverse (List<?> list) ：顺序反转
public static void shuffle (List<?> list)：随机置换


