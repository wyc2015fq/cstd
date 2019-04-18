# shell学习1 - nosmatch的专栏 - CSDN博客
2011年08月26日 08:45:41[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：349标签：[shell																[脚本																[任务																[bash](https://so.csdn.net/so/search/s.do?q=bash&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)
                
  有的时候case能对参数进行过滤，如下例子：
- #!/bin/bash 
- 
- case "$1" in 
- "") echo "Usage: ${0#.} <filename>"; exit $E_PARAM;; 
- 
- # 注意：${0##*/} 是${var##pattern} 的一种替换形式。得到的结果是$0 
- 
- -*) FILENAME=./$1;;  #如果传递进来的文件名参数($1)以一个破折号开头 
-                      #那么用./$1来代替。这样后边的命令将不会将它作 
-                      #为一个选项来解释。 
- *) FILENAME=$1;;     #否则，$1. 
- 
- esac 
- echo $FILENAME 
- exit 0 
- [root@localhost shell]# cat 20 
- #!/bin/bash 
- 
- case "$1" in 
- "") echo "Usage: ${0#.} <filename>"; exit $E_PARAM;; 
- 
- # 注意：${0##*/} 是${var##pattern} 的一种替换形式。得到的结果是$0 
- 
- -*) FILENAME=./$1;;  #如果传递进来的文件名参数($1)以一个破折号开头 
-                      #那么用./$1来代替。这样后边的命令将不会将它作 
-                      #为一个选项来解释。 
- *) FILENAME=$1;;     #否则，$1. 
- 
- esac 
- echo $FILENAME 
- exit 0 
   select的用法：
- #!/bin/bash 
- 
- PS3='Choose your favorite vegetable: ' # 设置提示符字串. 
- 
- echo 
- 
- select vegetable in "beans" "carrots" "potatoes" "onions" "rutabagas" 
- do 
-   echo 
-   echo "Your favorite veggie is $vegetable." 
-   echo "Yuck!" 
-   echo 
-   break  # 如果这里没有 'break' 会发生什么? 
- done 
- 
- exit 0 
   运行会有如下输出：
- [max@localhost shell]$ ./21 
- 
- 1) beans 
- 2) carrots 
- 3) potatoes 
- 4) onions 
- 5) rutabagas 
- Choose your favorite vegetable: 1 #输入1 
- 
- Your favorite veggie is beans. 
- Yuck! 
也可以用函数参数的形式给select传递参数：
- #!/bin/bash 
- 
- PS3='Choose your favorite vegetable: '
- 
- echo 
- 
- choice_of() 
- { 
- select vegetable 
- # [in list]被忽略, 所以'select'使用传递给函数的参数. 
- do 
-   echo 
-   echo "Your favorite veggie is $vegetable." 
-   echo "Yuck!" 
-   echo 
-   break 
- done 
- } 
- 
- choice_of beans rice carrots radishes tomatoes spinach 
- #         $1    $2   $3      $4       $5       $6 
- #         传递给choice_of()的参数 
- 
- exit 0 
当一个命令或者是shell本身需要初始化(或者*创建*)一个新的子进程来执行一个任务的时候, 这种行为被称为*fork*. 这个新产生的进程被叫做*子进程*, 并且这个进程是从*父进程*中*fork*出来的. 当*子进程*执行它的任务时,
*父进程*也在运行.
注意: 当*父进程*获得了*子进程*的*进程ID*时, 父进程可以给子进程传递参数, 然而*反过来却不行*.
   如下例子：
- #!/bin/bash 
- #spawn.sh 
- 
- 
- PIDS=$(pidof sh $0) 
- P_array=( $PIDS ) 
- echo $PIDS 
- let "instances = ${#P_array[*]} - 1" 
- 
- echo "$instances instance(s) of this script running." 
- echo "[Hit Ctl-C to exit]"; echo 
- 
- 
- sleep 1 #等一下 
- sh $0 
- 
- exit 0  #没必要，脚本永远不会运行到这里。 
    通常情况下, 脚本中的Bash*内建命令*在运行的时候是不会fork出一个子进程的. 但是脚本中的外部或者过滤命令通常*会*fork出一个子进程.
[http://alkshao.blog.51cto.com/1953382/508418](http://alkshao.blog.51cto.com/1953382/508418)
