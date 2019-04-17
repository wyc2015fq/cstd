# matlab中.mat文件的读取 - 家家的专栏 - CSDN博客





2011年03月04日 16:42:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：10859








在matlab命令行中，输入
>> load('heart_scale.mat')

就可以载入文件了。。

再次输入whos，就可以看到里面的结构，和数据个数了。。。我试过的，有问题，请留言。
>> whos
  Name                    Size                    Bytes  Class

  A                       1x1                         8  double array
  ans                     1x1                         8  double array
  heart_scale_inst      270x13                    28080  double array
  heart_scale_label     270x1                      2160  double array
  i                       1x1                         8  double array
  tline                   1x788                    1576  char array
  x                       1x9                        72  double array

Grand total is 4580 elements using 31912 bytes



