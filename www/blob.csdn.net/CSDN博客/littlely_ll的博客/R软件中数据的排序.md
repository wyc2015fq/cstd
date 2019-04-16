# R软件中数据的排序 - littlely_ll的博客 - CSDN博客





2017年02月04日 21:26:26[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：1974








y<-data.frame(number=c(2,7,2,5,2,6),age=c(22,31,23,34,22,41),weight=c(128,170,165,133,152,110))
y
  number age weight
1      2  22    128
2      7  31    170
3      2  23    165
4      5  34    133
5      2  22    152
6      6  41    110

#第一列升序排列
y[order(y[,1]),]
  number age weight
1      2  22    128
3      2  23    165
5      2  22    152
4      5  34    133
6      6  41    110
2      7  31    170
#第一列和第二列升序排列y[order(y[,1],y[,2]),]
  number age weight
1      2  22    128
5      2  22    152
3      2  23    165
4      5  34    133
6      6  41    110
2      7  31    170
#第一列升序，第二列降序排列y[order(y[,1],-y[,2]),]
  number age weight
3      2  23    165
1      2  22    128
5      2  22    152
4      5  34    133
6      6  41    110
2      7  31    170

矩阵同理。




