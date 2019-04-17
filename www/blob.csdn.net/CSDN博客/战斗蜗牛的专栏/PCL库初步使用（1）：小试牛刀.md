# PCL库初步使用（1）：小试牛刀 - 战斗蜗牛的专栏 - CSDN博客





2012年08月01日 20:23:00[vbskj](https://me.csdn.net/vbskj)阅读数：23049










本次试验的目的是利用PCL库来重建地形点云数据，并进行显示。总体流程是1）把DEM数据导入PCL的数据格式.pcd文件中；2）进行法向量估计；3）重建曲面；4）三维显示。

    1）写入pcd文件

首先打开C:\Program Files\PCL1.6.0\share\doc\pcl-1.6\tutorials\sources
下面的pcd_write工程，对立面的程序进行修改，把DEM数据（x,y,z）导入  pcl::PointCloud<pcl::PointXYZ>cloud成员变量中。

至此会产生一个test.pcd文件。

然后，可以利用同目录下的cloud_viewer编译后的程序来观看test.pcd文件，或者利用pcl安装目录bin目录下的pcd_viewer进行查看。有关pcd_viewer的用法可以再窗口按“h”，然后在命令行中进行查看。主要用的就是“r”：重置视角，“g”出现坐标轴，“alt+-/+”进行缩放等等。

![](https://img-my.csdn.net/uploads/201208/01/1343822485_4775.jpg)




2)利用安装目录bin下的pcl_normal_estimation.exe来估计点阵的法向量，指令如下：pcl_normal_estimation_release.exetest.pcd
 test1.pcd –k 3。后面的-k是指采用多少个k邻域的元素。

估计结束后，利用pcd_viewer进行查看，此时看没有区别。但是利用utraledit进行查看，发现test1.pcd后面多了几列，分别代表向量的xyz数值和曲率。




3） 曲面重建

利用安装目录bin下的pcl_poisson_reconstruction_release.exe对test1.pcd进行曲面重建，这时能从命令行看到该pcd文件中的xyz，曲率和法向量信息，指令如下：pcl_poisson_reconstruction_release.exe test1.pcd test1.vtk




4)从网上下载paraview软件，来查看test1.vtk即可。（首先，点击paraview左下“properties”标签的apply按钮，然后经过后面两个标签进行颜色、大小、显示方式设置即可；可以同时打开多个vtk文件进行查看）

![](https://img-my.csdn.net/uploads/201208/01/1343824393_4326.jpg)




