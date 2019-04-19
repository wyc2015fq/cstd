# svn "local edit, incoming delete upon update" problem fix method - Arthur的随笔 - CSDN博客
2011年11月21日 09:39:04[largetalk](https://me.csdn.net/largetalk)阅读数：3030
$ svn st
!  +  C foo
      >   local edit, incoming delete upon update
!  +  C bar
      >   local edit, incoming delete upon update
$ touch foo bar
$ svn revert foo bar
$ rm foo bar
我也不知道怎么导致的这个问题，估计是有人merge的时候冲突了。
ps. svn ingore 语法是 svn pe svn:ignore folder_name
