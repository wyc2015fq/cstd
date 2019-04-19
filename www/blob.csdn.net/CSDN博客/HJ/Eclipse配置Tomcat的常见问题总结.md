# Eclipse配置Tomcat的常见问题总结 - HJ - CSDN博客
2018年07月26日 10:09:52[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：134
一、项目中HttpServlet.java以及.jsp文件报错 
![这里写图片描述](https://img-blog.csdn.net/20180726100622294?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
原因分析：由于Libararies中未引入Apache Tomcat的JARS包 
![这里写图片描述](https://img-blog.csdn.net/20180726100629730?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
解决方法步骤：指定项目（这里是Chapter11），右键点击选入“Build Path”->选择“Configure Build Path”，进入下面界面，然后按图示完成配置即可。  
![这里写图片描述](https://img-blog.csdn.net/2018072610063735?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/2018072610064582?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180726100658442?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
二、服务器启动出现异常（例如点击starta.bat闪现或者从eclipse中启动不成功） 
原因分析：电脑中下载安装并不止一个tomcat服务器，并且已经有另外一个tomcat服务器正在开启，导致8080端口被占用。 
解决办法： 
    ①修改端口（不提倡，因为如果修改了端口值，那么在项目运行访问的时候，在localhost:后面就需要加上修改后的端口值，而不是默认的8080/80） 
    进入tomcat文件夹，按图示打开server.xml文件修改端口号； 
![这里写图片描述](https://img-blog.csdn.net/20180726100726263?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018072610073594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
如果没有其它编译软件（如notepad++或者sublime Text）就用记事本打开，找到如下一条语句，修改port对应引号内的8080值即可。 
![这里写图片描述](https://img-blog.csdn.net/20180726100834322?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
        ②杀死占用8080端口的进程 
win+R -> 输入“cmd”进入控制台，输入netstat –aov，找到本地地址8080结尾那一行所对应的PID 
![这里写图片描述](https://img-blog.csdn.net/20180726100841966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
然后进入任务管理器，查看进程（如果没有PID显示列，选择“查看”-> “选择列”，在PID前选择），找到对应的3804，杀死即可。 
![这里写图片描述](https://img-blog.csdn.net/20180726100855169?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180726100904729?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/2018072610091349?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
三、Eclipse引入tomcat并且成功运行，但是 tomcat的webapps文件夹中并没有运行的项目 
   原因分析：由于eclipse最开始在导入tomcat的时候并没有给tomcat进行设置环境路径 
解决方法：删除现有的tomcat，重新添加。在启动之前，在Modules中双击tomcat进入配置界面，按图示修改两处，然后启动服务器，即可看到webapps文件夹中映射过来的项目文件了。 
![这里写图片描述](https://img-blog.csdn.net/20180726100920731?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180726100927744?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ZlaXphb1NZVUFDTQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
