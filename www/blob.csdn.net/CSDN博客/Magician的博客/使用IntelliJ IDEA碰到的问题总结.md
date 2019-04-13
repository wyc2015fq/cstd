
# 使用IntelliJ IDEA碰到的问题总结 - Magician的博客 - CSDN博客


2018年09月25日 17:19:38[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：151



### 文章目录
[问题一：无法创建Java Class文件](#Java_Class_1)

# 问题一：无法创建Java Class文件
![在这里插入图片描述](https://img-blog.csdn.net/20180925171625582?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
今天打开项目时，发现右击新建不了java.class文件，于是尝试了以下方法：
（1）选择 File——>Project Structure——>Project Settings——>Modules：
![在这里插入图片描述](https://img-blog.csdn.net/20180925171742136?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（2）选择该项目的java存放的文件夹，将其设置为源文件，点击OK,apply即可
![在这里插入图片描述](https://img-blog.csdn.net/20180925171758662?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
（3）再次单击新建，就会发现可以新建java.class文件了
![在这里插入图片描述](https://img-blog.csdn.net/20180925171816362?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzE2NjMzNDA1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

