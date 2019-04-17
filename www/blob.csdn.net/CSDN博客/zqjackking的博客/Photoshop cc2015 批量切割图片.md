# Photoshop cc2015 批量切割图片 - zqjackking的博客 - CSDN博客





2017年07月18日 21:48:27[zqjackking](https://me.csdn.net/zqjackking)阅读数：1965








```
因为要制作数据集，要把很多大图（160*192）切割成小图（32*32），每个大图要切割成30份。
```

1 首先新建动作  窗口—-动作，创建新动作，修改名称（假设为split），其他默认。 
![这里写图片描述](https://img-blog.csdn.net/20170718214725585?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170718213809068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

开始记录动作 
![这里写图片描述](https://img-blog.csdn.net/20170718214552613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

“文件”-“打开”任意一张图片 

点击左侧工具栏中的，右键“裁剪工具”，点击“切片选择工具” 
![这里写图片描述](https://img-blog.csdn.net/20170718214203783?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170718214225453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

右击图片，选择“划分切片” 
![这里写图片描述](https://img-blog.csdn.net/20170718214244124?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

自己调整参数： 
![这里写图片描述](https://img-blog.csdn.net/20170718214301695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

划分后的效果预览： 
![这里写图片描述](https://img-blog.csdn.net/20170718214326955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

“文件”-导出-“存储为Web和设备所有格式” 

可以逐块调整参数（注意把格式改成JPEG）： 
![这里写图片描述](https://img-blog.csdn.net/20170718214412613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

选择“存储”，选择你的文件夹（路径必须是英文的） 

Ps自动在你的文件夹里新建“images”的文件夹，并把分割图片放在里面。 

停止记录动作 
![这里写图片描述](https://img-blog.csdn.net/20170718214528250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2、批处理切片  

“文件”-“自动”-“批处理” 
![这里写图片描述](https://img-blog.csdn.net/20170718214634452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenFqYWNra2luZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

A.修改“动作”为“split”
B.“源”的“选择”你的需要分割的图片所在的目录

C.勾选“覆盖动作中的“打开”命令（否则不断读取同一张图片） 

D.“目标”的“选择”你需要存储分割后图片的目录，并在该目录下新建一个文件夹“images” 

E.勾选覆盖动作中的“存储为”命令 

确认后，Ps开始批量分割图片




