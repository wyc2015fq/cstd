# Linux细节 - Linux 常用工作管理命令 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月19日 19:47:32[initphp](https://me.csdn.net/initphp)阅读数：924








1. 将命令放到后台执行，&，工作完成后，会提示，也会有工作进程提


`cp  aaa.log bbb.log &`

2. 查看在后台的进程


`jobs -l `

3. 将前台进程转到后台，停止状态


`ctrl+z`

4. 后台放到前台来执行


`fg 1（进程号）`

5. 后台停止的命令，在后台执行


`bg 1 (进程)`

6. 脱机管理



```
nohup  命令
nohup  命令  &
```





