# 让screen帮助你协同工作 - 在思索中前行！ - CSDN博客





2014年09月19日 13:49:15[_Tham](https://me.csdn.net/txl16211)阅读数：617







Screen是系统管理员手中的一件利器，下面我把它介绍给你，相信你会和我一样，认可这个非常棒的软件
一，什么情况下会用到screen?
  比如说，我们在运行一个非常费时间的程序，注意：可能我们是在通过ssh远程连接到托管在机房中的机器上，
  如果这个期间，网络出现中断（这个可以说是很正常的情况，别不信，我讲一件我亲历的例子），
  则我们正在运行的程序就会中止（为什么会这样？我稍后解答）
  如何保证当网络连接断开后，程序仍然在继续运行？而且可以随时的取回我们的屏幕？
  答案就是screen
  说明：1，我们做技术工作，要把各种极限情况考虑在内：有一个例子：有一次我在做 reiserfsck
       （我通过ssh远程连接到托管在机房中的机器上）
         那个分区的容量是2T，在进行了一半多（5个多小时过去了）我们的网络管理员不小心碰到了
         路由器的电源线，完了，所有的一切都要重新开始，当时我还不会使用screen,
         否则可以避免这场灾难。
         2，网络中断后，我们远程运行的程序会自动中止，这个是为什么？
            很简单,实验一下，我们现在就ssh到一台机器上，
            我们运行这个命令： ps auxfww
         注意：    f参数： forest  就是以树结构来显示进程
root   756  0.0  0.0  3508  336 ?      S    2008  11:20  /usr/sbin/sshd    
       root    8933  0.0  0.1  6776 1820 ?       S   23:52   0:00  \_/usr/sbin/sshd   
       root    8935  0.0  0.1  5616 1512 pts/3   S   23:52   0:00   \_ -bash   
       root    8977  0.0  0.0  2900  952 pts/3   R   23:53   0:00   \_ ps auxfww 
     可以看到当前正在执行的 ps auxfww 这个命令的位置，它的父进程是 -bash,而 -bash则是建立在pts/3这个tty上面。当网络中断时，pts/3上面所建立的连接会终止，当8935这个进程停止时，我们在其下面运行的命令自然也会停止。
      3,多问一个问题，会nohup不可以吗？
      这个问题问得好，用nohup当然可以，但注意：nohup需要把进程扔到后台去运行，仍以上面的reiserfsck为例，运行此命令时需要和程序进行交互，这种情况下用nohup就没办法输入了。而且screen有更多更强大的功能，当然更值得学习

## 二， 安装screen
      screen不是标配的命令，但是centos的yum源中包含，用
      yum install screen即可实现安装。
      它的官网：http://www.gnu.org/software/screen/

## 三，使用screen的例子：
    1,运行screen
    screen -S down_video
    直接运行screen就可以启动一个session(会话)，但是还是给它做一下命名比较好，
    这样可以避免和其他的session混淆，更可以一目了然的知道这个session的功能
    说明 : -S  给session命名

    2,如何列出系统中已在运行的screen session?
        screen -ls
        或着 screen -list
        例子：
        [root@dev ~]# screen -ls
      There is a screen on:
               4083.down_video (Attached)
      1 Socket in /var/run/screen/S-root.
      说明（Attached和Detached是screen session的两种状态，分别表示：正在被连接 和 已经被断开）
**  3,取回我们的screen**
      进入screen之后，此时我们可以进行我们的工作，
      比如我要用wget 下载一个大型的视频 或用php写入大量的数据等，注意此时我们不用再担心我们到此远程机器的连接断开,可以在此时直接关闭窗口，然后我们重新ssh连接到此远程机器。
      登录后用screen -ls进行检查 
     [root@dev ~]# screen -ls
     There is a screen on:
        4083.down_video (Detached)
     1 Socket in /var/run/screen/S-root.
    状态显示 down_video这个screen session已经被 断开(detach)
    此时我们用screen -r可以取回进行，
**说明： -r :取回一个已被断开的screen session**
    例：   [root@dev ~]# screen -r down_video
    可以看到我们的程序仍在运行，中间未曾停止，而且此时我们又可以控制它了。
4，如何中断一个 screen session?
    看例子：
[root@dev ~]# screen -ls
    There is a screen on:
        4083.down_video (Attached)
    1 Socket in /var/run/screen/S-root.

    请注意：**当前看到的session : down_video 正在被连接使用中，****这时我想使用此session**
[root@dev ~]# screen -r down_video
    There is a screen on:
        4083.down_video (Attached)
**There is no screen to be resumed matching down_video.****此时用 -r 不能取到此 screen session****    这时只能用 -d 参数来强制中断它**
    [root@dev ~]# screen -d down_video
    [4083.down_video detached.]
    [root@dev ~]# screen -r down_video
    再用 -r 取回 screen就成功了
    注意：原来的被我们中断的连接会显示 ：
    [root@dev ~]# screen -r down_video
    [remote detached]

**   5、如何切换到一个screen**

[root@dev ~]# screen -ls
     There is a screen on:
        4083.down_video (Attached)
    1 Socket in /var/run/screen/S-root.
**screen -x down_video #就能进入该Screen的工作任务中了**
    四：使用screen进行远程协作：
    1，什么情况下会用到这个远程协作？
    比如：你需要给在远方的同事在一台服务器上面演示一项功能的操作步骤，又如：你和同事需要连接到同一台服务器上面协作完成一项工作，每个人执行其中的一部分
2,步骤：
   需要有一个人先登录到服务器，比如你的伙伴先登录，
   登录后执行:
   [root@dev ~]# screen -S cooperate
下面我们以同一帐号登录到同一台机器上面:
   先查看情况：
   [root@dev ~]# screen -ls
   There is a screen on:
        4195.cooperate  (Attached)
   1 Socket in /var/run/screen/S-root.
   我们可以看到cooperate这个session是正在被连接的，
   下面我们连接进入此session,即： 我们和伙伴使用同一个帐号进入同一个screen session
** [root@dev ~]# screen -x cooperate**
**  说明： -x:Attach to a not detached screen session. (Multi display mode)**
** 就是连接 到一个未被断开的screen session,****注意此时两边的显示内容是完全相同，**
**注意：此时我们和伙伴都可以输入命令了，大家在一个session开始协同工作，     比如:咱们在这边执行： ls     伙伴的屏幕上会有相同的显示信息**
   3,从会话中退出：
   如果只有一个人想退出，那么关闭终端的窗口即可
   如果不想关闭窗口，可以在screen中输入 ctrl-a d  (按住ctrl不放，再按a,放开两者后按d)，如果两个均想退出，任一方在screen中输入exit即可

下面我再说一句：

screen的功能可不止于此，它还有很多巨强大的功能，我的一个系统管理员朋友说:"我在工作中简直离不开screen了",其他的功能，我会专门再写一篇内容进行介绍    

## **四、Screen的使用操作**

screen 能够支持多个终端窗口，这样能给我们在终端的操作带来大量的便利。比如：我们可以在一个window中编译程序，在另一个windows中写代码，在第三个window中下载软件，在第四个window中做yum升级，互不干扰，而不必打开多个putty到服务器的连接。而且它还具有锁定复制等大量能给我们提高效率的功能，开始学习吧!


### 一，如何查看screen的帮助?
   ctrl-a ?
   说明：这个快捷键如何操作？
   首先是在终端启动screen
   (在这里说一下操作screen的终端，gnome terminal,putty,sshsecureclient这几个均可)
   命令:
   [lhd@localhost ~]$ screen -S testwindow
   此时可以使用快捷键了,按住ctrl不放，按a,然后放开此两者，输入一个?(需要按住shift键才可以打出来)

                           Command key:  ^A   Literal ^A:  a

break     ^B b      fit       F         lastmsg    ^M m      next     ^@ ^N   sp n readbuf <          silence    _        width     W         clear     
 C         flow     ^F f    license    ,          number     N       redisplay  ^L l      split      S         windows   ^W w      
colon      :          focus      ^I         lockscreen ^X x       only       Q       remove X       suspend    ^Z z       wrap       ^R r      
。。。。。。       
^]  paste .
"   windowlist -b
-   select -
0   select 0
1   select 1
2   select 2
3   select 3
4   select 4
5   select 5
6   select 6
7   select 7
8   select 8
9   select 9
I   login on
O   login off
]   paste .


### 二，如何新建一个window?如何管理window?
   1,新建一个window
   ctrl-a c
   注意此时我们已新建了一个window
   那么新建此window之前，就是刚进入screen时的那个window去哪儿了？
   如何进行切换？

   2,列出我们所打开的window
   ctrl-a w
   注意: 已打开的window会显示在标题栏或终端的最下面一行
   0-$ bash 1*$ bash
   说明:
   0和1分别是两个window的id
   可以供我们对window进行切换使用

   -表示前一个window,我们之前使用过的那一个窗口
   *表示当前正在打开的window

   3,如何给窗口改名？
     ctrl-a A

     在终端最下面一行会显示:
     Set window's title to: compile
     为当前windows输入新名字后回车即可

   4,如何切换window?
     用快捷键

     ctrl-a 0   切换到第0个window
     ctrl-a 1   切换到第1个window
     ctrl-a 2   切换到第2个window
     ctrl-a 3   切换到第3个window
     以此类推

     用列表:
     ctrl-a "    (ctrl加a,然后按双引号)
     终端会显示
Num Name                                                                                                                                        Flags


 0 

bash                                   $
 1

compile                               
  $用上下箭头从中选择一个后回车即可
5,如何关闭一个不再使用的window ?

ctrl-a K ，请注意K是大写
此时终端会提示： Really kill this window [y/n]
输入y后回车即可，此时你可以查看window列表，可以发现我们关闭的window已不在列表之内了
6,如何锁定当前的window?
  这个功能很方便，锁定终端的同时不影响正在运行的程序，
  而且锁定之后只能输入密码来解锁，既安全又方便 ctrl-a x

  窗口会变成:Screen used by liuhongdi <lhd>.
             Password:


###    三,screen 的复制粘贴模式
1,复制:
      ctrl-a [
      此时可以移动光标,
      注意：如何开启复制?
      第一次按space键: 开始复制
      第二次按space键: 结束复制
      两次按space键之间反选的内容是已复制成功的内容
    2,粘贴:
      ctrl-a ]
      我们可以在当前window,也可以切换到另一window,
      打开vi,例如:
      vi testpaste.txt
      点击i进入insert模式,
      然后用  ctrl-a ]即可复制成功

###    四,screen的分割屏幕
   例如：
   我现在需要把屏幕分成上下两个部分:
   上半部分我需要用top来监视进程
   下半部分我可以随时输入命令来kill掉上面显示出的最费资源的进程，
   screen 可以帮我们做到这一点
   1,如何进行分割?
   ctrl-a S
   请注意: S是大写
   此时我们可以看到屏幕被用分隔线分离成上下两部分，screen称之为区域(region)
   而 ctrl-a S的作用就是把当前的region分割成两部分
   2,如何在分割后的区域间进行切换
     ctrl-a tab  
     用ctrl加a,然后按tab即可切换
   3,新切换到的区域(region)内无任何内容，如何处理？
     两个做法：
     ctrl-a c,新建一个窗口
    当然也可以切换到我们之前已打开的窗口,比如： ctrl-a 1
   3,如何关闭当前的region?
     ctrl-a X
     请注意: X大写

###    五,对screen session的管理:
   1,如何离开当前的screen session?
     请注意：是离开，不是结束会话，
           就是说我需要能随时回到当前会话中
     ctrl-a d
     终端会显示:
     [detached]
     [lhd@localhost ~]$
     象这样，我们可以随时输入 screen -r 返回到我们刚刚离开的 screen session
    [lhd@localhost ~]$ screen -r testwindow
   2,如何退出当前的screen session?
     注意退出后可不能再用screen -r 返回了
     在命令提示符后输入 exit即退出了
     [lhd@localhost ~]$ exit
     之后终端会显示:
     [screen is terminating]
     [lhd@localhost ~]$ 

###    六,screen的配置文件在哪里?
   screen的快捷键等均可进行设置，比如：你可能不适应一直 ctrl加a
   那么就编辑它的配置文件吧
   /etc/screenrc
   这里面的配置项很多，如果不是确有必要，不建议对它做改动
   把这里面的配置作为练习留给大家去自行研究




