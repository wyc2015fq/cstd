# nginx初步 - Big Smile - CSDN博客
2016年09月18日 20:55:21[王啸tr1912](https://me.csdn.net/tr1912)阅读数：555
           nginx程序下载地址：[http://download.csdn.net/detail/tr1912/9629880](http://download.csdn.net/detail/tr1912/9629880)
这几天帮助评教捣鼓了一下nginx，有点小小的收获，来和大家分享一下！
# 一、初见Nginx
        其实最早的时候是在上个学期的期末考试的时候看到Nginx的，那个时候是叫负载均衡，就是用来转发请求用的，感觉特别的新鲜，特别的高大上，这肯定又是一个技术难点，可要好好的学习，但是后来发现在许多打不开的网站上面都会报出一个“nginx error”的错误，现在看来Nginx也是没有那么难（使用上），但是在配置上还有不少的需要发现的地方。
# 二、深入了解
        Nginx ("engine x") 是一个高性能的HTTP和反向代理服务器，也是一个IMAP/POP3/SMTP服务器。Nginx是由Igor Sysoev为俄罗斯访问量第二的Rambler.ru站点开发的，第一个公开版本0.1.0发布于2004年10月4日。其将源代码以类BSD许可证的形式发布，因它的稳定性、丰富的功能集、示例配置文件和低系统资源的消耗而闻名。2011年6月1日，nginx
 1.0.4发布。
        以上均来自百度，其实我对于Nginx的理解就是他作为一个请求的接收点，然后按照权重的不同，向配置好的服务器分发这些请求。
![](https://img-blog.csdn.net/20160918202942050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        其中我们可以用权重来控制给每个服务器的请求的量，如果有一台服务器宕机了的话还可以设置判断来选择转发。
在这其中我们还需要考虑不同服务器上的session的存贮问题还有对于静态变量的使用所造成的连锁问题的影响都是需要注意的。
# 三、配置文件的设置
        说到Nginx，其中最为重要的也就是写好配置文件了，只有写好配置文件，整个Nginx才可以很好的运行。配置文件的位置在："..\nginx-1.4.7\conf\nginx.conf"下面，比较好找。
注意：这个文件是不可以用windows自带的记事本打开的，因为记事本会向文件中写入一些二进制数值，导致文件不可被Nginx识别，所以只能用第三方的一些文件编辑器（notepa++，EditPlus等）来进行编辑。
 基本上里面所有的地方都是有汉语注释的（本博客提供的文件），其最重要的是两点：一个是监听的端口，一个是服务器集群的地址和权重。
```
#服务器的集群
    upstream  netitcast.com {  #服务器集群名字
  		#server   172.16.21.13:8081 weight=1;#服务器配置   weight是权重的意思，权重越大，分配的概率越大。		
		#server    192.168.22.243:8006  weight=1;
		server    192.168.21.125:8080  weight=1;
	}	
    #当前的Nginx的配置
    server {
        listen       8006;#监听80端口，可以改成其他端口
        server_name  192.168.22.238;##############	当前服务的域名
      
        #}
```
        注：上述配置文件中的#号为注释标志。
         可以看到，server后面加上的就是集群的IP地址，也就是接收请求的服务器的地址，后面的weight就是权重，1-5，越大分到的概率越大。
          下面的server根项就是对于搭载Nginx的服务器的配置，listen就是对于请求要传递的端口号的设置，也就是外面访问的地址的端口。后面的server_name就是Nginx服务器的域名或者是IP地址，这里可以自己定义，用户访问的时候就是通过这两个信息来进行访问的。
# 三、启动Nginx服务
        配置好了Nginx的转发和监听了之后就可以使用Nginx了，这个时候需要启动服务，需要win+r调出运行框，在运行中输入cmd进入命令提示符（最好是管理员模式），然后把操作地址用cd命令变为Nginx文件所在的根目录下，输入start nginx 就可以运行Nginx的服务了。
![](https://img-blog.csdn.net/20160918205406679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
          特别注意的是，一旦Nginx服务运行了之后他就是一个独立的个体了，我们还可以修改配置文件，启动新的Nginx服务，来进行不同端口的监听和集群的设置，并且不影响原来的使用（仅在windows中适用）。
**总结：**其实Nginx的使用也不是想象中的那么难，我们今后还需要再用一用这些这样的东西。
