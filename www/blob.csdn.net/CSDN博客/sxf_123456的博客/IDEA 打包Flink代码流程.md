# IDEA 打包Flink代码流程 - sxf_123456的博客 - CSDN博客
2018年09月12日 21:12:34[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：945
- IDEA打包flink代码流程
1、点击图中红色方框
![](https://img-blog.csdn.net/20180912211022578?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2、点击图标+,再点击Maven
![](https://img-blog.csdn.net/20180912211050565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
3、(1)在Name后添加工程名，(2)Working directory 中选择项目的工程路径：D:\Code\Hydrogen\etl.worker.stbalarm (根据自己的工程选择) (3)在Command line 中输入打包命令clean package -Pbuild-jar -Dmaven.test.skip=true (4)点击OK
![](https://img-blog.csdn.net/20180912211112670?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
4、选择刚才打包的工程名，点击运行进行预编译，查看语法是否错误
![](https://img-blog.csdn.net/20180912211126633?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
5、打好的包在etl.worker.stbalarm下边
![](https://img-blog.csdn.net/20180912211144803?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3N4Zl8xMjM0NTY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
6、将打好的包，上传到139的/data/flink/jar.d
7、linux终端运行命令：
/usr/local/flink/bin/flink run -p 3 -c com.sctele.design.etl.hydrogen.worker.stbalarm.StbalarmWorker /data/flink/jar.d/stbalarm-apk-0.4.3-stable-18-gfad75c3-dirty.jar
注意：
-p 后边的数字代表进程数
-c 后边的是工程的主函数(操作点击本工程的主函数，右键选择Copy Reference)
