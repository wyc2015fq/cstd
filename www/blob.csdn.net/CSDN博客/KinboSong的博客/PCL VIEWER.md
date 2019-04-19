# PCL VIEWER - KinboSong的博客 - CSDN博客
2017年06月12日 16:05:40[songjinbo3](https://me.csdn.net/KinboSong)阅读数：2218
【原文：[https://segmentfault.com/a/1190000006685118](https://segmentfault.com/a/1190000006685118)】
## 可视化：一目了然
如题所示，可视化的重要性不必多说。在点云数据预处理中，要想知道点云的形状需要可视化; 要想了解精简/去噪/简化/压缩 的结果需要可视化; 配准中，对应点对的显示/对应点对的去除结果/配准变化的过程 需要可视化 …
## pcl_viewer
[Linux](http://lib.csdn.net/base/linux) 下可直接在命令行输入 `pcl_viewr path/to/.pcd或.vtk`可直接显示pcl中的点云文件。
`pcl_viewr`几个常用的命令：
`r`键: 重现视角。如果读入文件没有在主窗口显示，不妨按下键盘的`r`键一试。
`j`键：截图功能。
`g`键：显示/隐藏 坐标轴。 
`鼠标`：左键，使图像绕自身旋转; 滚轮, 按住滚轮不松，可移动图像，滚动滚轮，可放大/缩小 图像; 右键,`“原地” `放大/缩小。
`-/+`：-（减号）可缩小点; +(加号)，可放大点。
`pcl_viewe -bc r,g,b /path/to/.pcd`:可改变背景色.
`pcl_viewer`还可以用来直接显示pfh，fpfh（fast point feature histogram），vfh等直方图。
常用的`pcl_viewer` 好像就这些，其他未涉及到的功能可通过
```
pcl_viewer
 /path/.pcd
```
打开图像，按键盘`h`（获取帮助）的方式获得.
## 程序中的可视化
### 简单可视化类
所谓简单可视化类，是指直接在程序中使用，而且不支持多线程。
必须包含的头文件`#include<pcl/visualization/cloud_viewer.h>`，声明一个可视化类直接
```
pcl::visualization::CloudViewer
 viewer ("test");
```
即可，它的意思是说，我创建了一个CloudViewer的可视化类，这个可视化窗口的名字叫做`test`;
 显示用`viewer.showCloud(cloud)` , 要想让自己所创窗口一直显示，则加上 
```
while
 (!viewer.wasStopped()){ };
```
即可.
### “复杂的”可视化类
以一段程序为例：
```cpp
```
#include <pcl/visualization/pcl_visualizer.h> //包含基本可视化类
    #include <pcl/visualization/pcl_visualizer.h>
    //设置键盘交互函数,按下`space`键，某事发生
    void keyboardEvent(const pcl::visualization::KeyboardEvent &event,void *nothing)
    {
        if(event.getKeySym() == "space" && event.keyDown())
                next_iteration = true;
    }
    int main (int argc, char **argv)
    {
        1.  读入点云 source, target
        2.  处理读入的数据文件
          boost::shared_ptr<pcl::visualization::PCLVisualizer> view (new pcl::visualization::PCLVisualizer("test")); //创建可视化窗口，名字叫做`test`
          view->setBackgroundColor(0.0,0,0); //设置背景色为黑色
          viewer->addCoordinateSystem(1.0); //建立空间直角坐标系
      //  viewer->setCameraPosition(0,0,200); //设置坐标原点
          viewer->initCameraParameters();   //初始化相机参数
          ***`*显示的”处理的数据文件“的具体内容*`***
          view->registerKeyboardCallback(&keyboardEvent,(void*)NULL);  //设置键盘回吊函数
          while(!viewer->wasStopped())
          {
            viewer->spinOnce(100);  //显示
            boost::this_thread::sleep (boost::posix_time::microseconds (100000));   //随时间
          }
    }
```
```
在主程序`2`中，处理显示数据文件包含以下几种:
一. 计算并显示法向量，具体在自己的笔记[pcl法向量的计算与显示](https://segmentfault.com/n/1330000005761876)
点击预览
二. 我的笔记[画线与显示](https://segmentfault.com/n/1330000006645681)
点击预览,可用于配准计算中对应点对的显示，不过用画线的办法很不好.
三. 单纯的自定义的显示点云有如下常用函数：
- 
pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> sources_cloud_color(source,250,0,0); //这句话的意思是：对输入为`pcl::PointXYZ`类型的点云，着色为红色。其中，source表示真正处理的点云，sources_cloud_color表示处理结果.
- 
view->addPointCloud(source,sources_cloud_color,”sources_cloud_v1”,v1); //将点云source,处理结果sources_cloud_color,添加到视图中，其中,双引号中的sources_cloud_v1,表示该点云的”标签“，我们依然可以称之为”名字“，之所以设置各个处理点云的名字，是为了在后续处理中易于区分; v1表是添加到哪个视图窗口（pcl中可设置多窗口模式）
- 
view->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE,3,”sources_cloud_v1”); //设置点云属性. 其中`PCL_VISUALIZER_POINT_SIZE`表示设置点的大小为3,双引号中”sources_cloud_v1“,就是步骤`2`中所说的标签。
- 
view->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_OPACITY,1,”sources_cloud_v1”); //主要用来设置`标签`点云的不透明度，表示对标签名字为”sources_cloud_v1”的标签点云设置不透明度为1,也就是说透明度为0.
 默认情况下完全不透明。
四. 显示配准中的对应点对关系.
要想显示点对之间的对应关系, 首先必须计算出对应点对, pcl中对应点对的计算可通过
```
pcl::registration::CorrespondenceEstimation<pcl::PointT,pcl::PointT>
 correspond_est;
```
计算,计算出对应点对后, source 和 target对应点的索引会存储在`vector<int> A`或
```
pcl::Correspondences
 A
```
中.要想显示点对的对应关系,只需`view->addCorrespondences<pcl::PointXYZ>(source,target,A,"correspond",v1);` 其中，`pcl::PointXYZ`表示所添加对应点对的类型为`PointXYZ`类型的，参数中的前两个表示目标点云和源点云，`A` 存储从目标点云到源点云的对应点的索引，”correspond“依然是自定义的标签，v1表示添加到哪个窗口. 
为了使得对应点更加个性化，我们可以对它进行一下”定制“：
- 
view->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_LINE_WIDTH,2,”correspond”); //设置对应点连线的粗细.PCL_VISUALIZER_LINE_WIDTH,表示`线`操作,线段的宽度为2（提醒一下自己:
 线段的宽度最好不要超过自定义的点的大小）,”correspond”表示对 对应的标签 做处理.
- 
view->setShapeRenderingProperties(pcl::visualization::PCL_VISUALIZER_COLOR,0,0,1,”correspond”); //设置对应点连线的颜色，范围从0-1之间。
五. 多窗口及人机交互设置.
具体操作即设置请看[pcl之ICP实现](https://segmentfault.com/a/1190000005930422)
六. 如果用pcl的可视化类显示直方图，则可以这样做(以fpfh为例)：
```cpp
```
#include <pcl/visualization/histogram_visualizer.h> //直方图的可视化
#include <boost/thread/thread.hpp>
#include <pcl/visualization/pcl_plotter.h>
int main (int argc, char **argv)
{
    ....
     直方图计算
    ....
      pcl::visualization::PCLHistogramVisualizer view;
      view.setBackgroundColor(255,0,0);
      view.addFeatureHistogram<pcl::FPFHSignature33> (*fpfhs,"fpfh",1000);   //对下标为1000的元素可视化
      //view.spinOnce(10000);  //循环的次数
      view.spin();  //无限循环
    return 0;
}
```
```
也可以这样显示直方图不过需要在添加头文件`#include <pcl/visualization/pcl_plotter.h>`
```cpp
```
pcl::visualization::PCLPlotter plotter;
// We need to set the size of the descriptor beforehand.
plotter.addFeatureHistogram(*fpfhs, 300); //设置的很坐标长度，该值越大，则显示的越细致
plotter.plot();
```
```
![](https://segmentfault.com/img/bVCpg4)
