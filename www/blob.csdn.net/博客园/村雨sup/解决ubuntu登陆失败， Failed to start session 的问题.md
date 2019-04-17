# 解决ubuntu登陆失败，"Failed to start session"的问题 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





我是在虚拟机中安装了Ubuntu 14.04 系统，在Ubuntu 中执行 apt-get update  和  apt-get upgrade 命令后，然后重启系统。
    但是，在重启成功后，在登陆的界面进行登陆时，却报错误"Failed to start session" ,无法登陆。

        Ubuntu 从 图形界面 切换到 终端界面的 命令是：  CTRL + ALT + [F1 ~ F6]           // F1 到 F6中任意一个按键都可以。
       Ubuntu 从  终端界面 切换到 图形界面的 命令是：  CTRL + ALT + F7         

      把登陆界面 切换到 终端 界面，然后再终端中输入 账号和密码 进入 系统。
      在Ubuntu系统中执行以下命令：
            sudo apt-get install ubuntu-desktop  
       重启系统，然后就可以在图形界面登陆成功了。
--------------------- 
作者：西域孤梅 
来源：CSDN 
原文：https://blog.csdn.net/lhd435940424/article/details/78853066 
版权声明：本文为博主原创文章，转载请附上博文链接！











