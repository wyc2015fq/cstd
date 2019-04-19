# MAC OS安装Maven - 零度的博客专栏 - CSDN博客
2017年05月04日 14:56:35[零度anngle](https://me.csdn.net/zmx729618)阅读数：337
1、下载http://maven.apache.org/download.cgi
2、把下载到的压缩包解压到相应目录，我使用的目录结构是/usr/local/maven/maven-3.2.3
3、配置4个环境变量。看看~/下是否有.bash_profile文件，如果没有就创建一个，环境变量要配置在这个文件中。
创建文件：touch ~/.bash_profile
打开文件：open ~/.bash_profile，当然了你也可以使用vi命令直接编辑
在这个文件中配置如下四个环境变量：
①、maven所在的目录
export M2_HOME=/usr/local/maven/maven-3.2.3
②、maven bin所在的目录
export M2=$M2_HOME/bin
③、将maven bin加到PATH变量中
export PATH=$M2:$PATH
④、配置JAVA_HOME所在的目录，注意这个目录下应该有bin文件夹，bin下应该有java等命令
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.7.0_67.jdk/Contents/Home
4、在新的命令行窗口执行mvn -version，如果正常显示了maven的版本等信息，就是配置成功了。
