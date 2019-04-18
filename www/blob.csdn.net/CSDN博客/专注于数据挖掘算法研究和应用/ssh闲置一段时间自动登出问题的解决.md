# ssh闲置一段时间自动登出问题的解决 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月09日 11:11:27[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4077








                
1、问题：ssh连接到服务器，没有操作一段时间就自动登出；

         等待输入超时：自动登出，这是闲置超时自动退出

方法一：

   1)定位用户目录，如etl用户

     /home/etl/.ssh

   2)在.ssh目录新建一个config文件，输入：

     ServerAliveInterval 60

     就是每隔60秒自动ssh客户端自动和服务通信一次，避免因一段时间没有操作而自动登出；

   3)ssh重连；

   没生效；



方法二：

直接在secureCRT上配置：选项---终端---反空闲中设置每隔60秒发送一个字符串，或者是NO-OP协议包；

没生效。



方法三：



  1）修改/etc/profile下tmout参数

     如：sudo vi /etc/profile

     tmout=300

     就是300秒后闲置自动退出；


  2）在未设置TMOUT或者设置TMOUT=0时，此闲置超时自动退出的功能禁用。



生效。
            


