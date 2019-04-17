# linux 正则使用 - zj360202的专栏 - CSDN博客





2014年09月22日 17:38:55[zj360202](https://me.csdn.net/zj360202)阅读数：605标签：[linux																[rm																[快速删除																[删除文件																[正则](https://so.csdn.net/so/search/s.do?q=正则&t=blog)
个人分类：[linux](https://blog.csdn.net/zj360202/article/category/2463627)





1.rm(ls) ：快速删除一些连续的文件（ls和rm一样，这里就是列举了）




```java
[root@node2 test]# ls
1  12  13  2  3  4
[root@node2 test]# rm -rf 12
[root@node2 test]# ls
1  13  2  3  4
[root@node2 test]# touch 12
[root@node2 test]# ls
1  12  13  2  3  4
[root@node2 test]# rm -rf [12]   只删除了1,2相当于枚举
[root@node2 test]# ls
12  13  3  4
[root@node2 test]# touch 1
[root@node2 test]# touch 2
[root@node2 test]# ls
1  12  13  2  3  4
[root@node2 test]# rm -rf {12,[12]}  删除了12,1,2
[root@node2 test]# ls
13  3  4
[root@node2 test]# ls
1  12  13  2  3  4
[root@node2 test]# rm -rf 1[2-3] 效果和rm -rf 1[23];rm -rf 1[2,3]一样  删除12,13
[root@node2 test]# ls
1  2  3  4
```](https://so.csdn.net/so/search/s.do?q=删除文件&t=blog)](https://so.csdn.net/so/search/s.do?q=快速删除&t=blog)](https://so.csdn.net/so/search/s.do?q=rm&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




