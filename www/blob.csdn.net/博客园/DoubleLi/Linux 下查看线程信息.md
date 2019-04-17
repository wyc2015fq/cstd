# Linux 下查看线程信息 - DoubleLi - 博客园






1. 使用 pstree -p PID

ps aux | grep firefox | grep -v grep
charles  26058  0.0  0.0   4908  1152 ?        S    19:17   0:00 /bin/sh /usr/lib/firefox-3.5.4/run-mozilla.sh /usr/lib/firefox-3.5.4/firefox



charles  26073  7.6  3.4 284264 70164 ?        Sl   19:17   4:36 /usr/lib/firefox-3.5.4/firefox

pstree -p 26058



run-mozilla.sh(26058)───firefox(26073)─┬─{firefox}(26074)
                                                                             ├─{firefox}(26075)
                                                                             ├─{firefox}(26076)
                                                                             ├─{firefox}(26085)
                                                                             ├─{firefox}(26086)
                                                                             ├─{firefox}(26087)
                                                                             ├─{firefox}(26092)
                                                                             └─{firefox}(26403)
  2.使用 /proc

cd /proc/26073/task

ls
26073  26074  26075  26076  26085  26086  26087  26092  26403



3. 使用 top 命令：

top -p 26073



然后press shift + h:



Tasks:   9 total,   0 running,   9 sleeping,   0 stopped,   0 zombie
Cpu(s): 10.3%us,  5.3%sy,  0.0%ni, 84.4%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
Mem:   2053032k total,   689696k used,  1363336k free,    37212k buffers
Swap:  2097140k total,   121804k used,  1975336k free,   314864k cached

  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND                                                   
26073 charles   20   0  277m  67m  24m S  1.0  3.4   4:47.01 firefox                                                   
26074 charles   20   0  277m  67m  24m S  0.0  3.4   0:01.41 firefox                                                   
26075 charles   20   0  277m  67m  24m S  0.0  3.4   0:00.12 firefox                                                   
26076 charles   20   0  277m  67m  24m S  0.0  3.4   0:14.60 firefox                                                   
26085 charles   20   0  277m  67m  24m S  0.0  3.4   0:00.00 firefox                                                   
26086 charles   20   0  277m  67m  24m S  0.0  3.4   0:00.00 firefox                                                   
26087 charles   20   0  277m  67m  24m S  0.0  3.4   0:03.51 firefox                                                   
26092 charles   20   0  277m  67m  24m S  0.0  3.4   0:00.06 firefox                                                   
26403 charles   20   0  277m  67m  24m S  0.0  3.4   0:00.00 firefox



4. 使用 ps -eLf



5. 检查线程占用的内存

cat /proc/statm



6. 使用命令：


ps -Lo pid,ppid,pgid,nlwp,lwp,stat,command -p PID


**[plain]**[view plain](http://blog.csdn.net/caspiansea/article/details/6216416)[copy](http://blog.csdn.net/caspiansea/article/details/6216416)

[print](http://blog.csdn.net/caspiansea/article/details/6216416)[?](http://blog.csdn.net/caspiansea/article/details/6216416)

- $ ps -Lo pid,ppid,pgid,nlwp,lwp,stat,command -p 2053  
-   PID  PPID  PGID NLWP   LWP STAT COMMAND  
-  2053     1  1830    2  2053 Sl   /usr/lib/zeitgeist/zeitgeist-fts  
-  2053     1  1830    2  2054 Sl   /usr/lib/zeitgeist/zeitgeist-fts  



7.自己编写一个脚本：







**[plain]**[view plain](http://blog.csdn.net/caspiansea/article/details/6216416)[copy](http://blog.csdn.net/caspiansea/article/details/6216416)

[print](http://blog.csdn.net/caspiansea/article/details/6216416)[?](http://blog.csdn.net/caspiansea/article/details/6216416)

- $ cat /home/charles/listThread.sh  
- #!/bin/bash  
- cd /proc/$1/task;  
- list=$(ls);  
- #echo $list;  
- printf "%-15s %-5s %7s %7s\n" Name State Pid Tid  
- for task in $list; do  
- list=$(cat $task/status | awk 'NR <= 4 {printf "%s\n", $2}');  
- list2=$(echo $list | tr '\n' ' ');  
- echo $list2 | awk '{printf "%-15s %-5s %7s %7s\n", $1, $2, $3, $4}'  
- done  




**[plain]**[view plain](http://blog.csdn.net/caspiansea/article/details/6216416)[copy](http://blog.csdn.net/caspiansea/article/details/6216416)

[print](http://blog.csdn.net/caspiansea/article/details/6216416)[?](http://blog.csdn.net/caspiansea/article/details/6216416)

- $ /home/charles/listThread.sh 2053  
- Name            State     Pid     Tid  
- zeitgeist-fts   S        2053    2053  
- gdbus           S        2053    2054  










