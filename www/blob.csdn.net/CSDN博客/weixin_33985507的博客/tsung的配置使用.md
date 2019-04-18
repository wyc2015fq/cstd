# tsung的配置使用 - weixin_33985507的博客 - CSDN博客
2018年10月17日 21:26:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7

1、在root下新建.tsung文件，在.tsung文件新建log文件夹。.tsung文件用于存放log和xml文件
2、复制/usr/local/tsung/share/doc/tsung/examples/目录下的xml文件到.tsung文件下（http压力测试http_simple.xml，mqtt压力测试mqtt.xml,mysql压力测试mysql.xml）,并重命名为tsung.xml
3、运行默认执行脚本~/.tsung/tsung.xml配置，修改tsung.xml文件，如下
4、clients：
![](https://img2018.cnblogs.com/blog/692143/201810/692143-20181017212058297-986297568.png)
可指定多个client，若指定多个client，则要保证各个client之间可以ssh无密码登录，并且erlang版本需一致。host为本地ip指定的域名（此处不能用ip地址），use_controller_vm为true时，当用户量增加到maxusers时，就是终止程序，当为false时，当用户量增加到maxusers时，会重新创建一个虚拟机，然后继续在新虚拟机上运行脚本 。（若主机为单核，请设置use_controller_vm为true）
注：host大多数情况下为localhost，若运行tsung时出现“Host key verfication failed”,可对/etc/hosts进行查看
![](https://img2018.cnblogs.com/blog/692143/201810/692143-20181017212452975-1018838567.jpg)
可以添加localhost字段，如上图，或者在xml文件中host后的域名改为"iZ23netdpp7Z"。
5、servers mqtt服务器的ip地址，端口号，默认协议为tcp
6、monitor 用了监控服务器信息（国内各大服务器厂商都要自己的监控工具，如阿里的云监控控制台，因此这里可以不进行设置）
7、load 加载用户信息，加载用户分为两种方式。1、静态加载用户；2、随机加载用户
     1、静态加载用户：
         <user session="session_name" start_time="10" unit="second"></user>
        产生一个用户，该用户执行名为session_name的session，10s后执行。
    2、随机加载用户
      <arrivalphase phase="1" duration="3" unit="second">
         <users maxnumber="5" arrivalrate="1" unit="second"/>
      </arrivalphase>
       其中产生用户速度配置有两种：1、arrivalrate:每s产生多少用户
                                                   2、interarrival:每多少s产生一个用户
       注：可配置多个arrivalphase，按照phase排序来顺序执行。

