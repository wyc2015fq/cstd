# PCL：PCL可视化显示点云 - wishchinYang的专栏 - CSDN博客
2013年10月21日 15:45:22[wishchin](https://me.csdn.net/wishchin)阅读数：6835
（1）：引用：仅仅是简单的显示点云，可以使用CloudViewer类。这个类非常简单易用。但要注意，它不是线程安全的。如果要用于多线程，还要参考PCLVisualizer。
需要注意的是，PointCloud的数据类型要和PCD文件中或者代码中的PointT一致！
并且：CloudViewer除了显示什么也不能干.
 显示代码为：
```cpp
pcl::visualization::CloudViewer viewer ("Cluster viewer");
 viewer.showCloud(colored_cloud);
  while (!viewer.wasStopped ())
  {
  }
```
（2）：PCLVisualizer详细使用规则
 简单函数解释：
```cpp
boost::shared_ptr<pcl::visualization::PCLVisualizer> simpleVis (pcl::PointCloud<pcl::PointXYZ>::ConstPtr cloud)
{
  // --------------------------------------------
  // -----Open 3D viewer and add point cloud-----
  // --------------------------------------------
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));  
  viewer->setBackgroundColor (0, 0, 0);  
  viewer->addPointCloud<pcl::PointXYZ> (cloud, "sample cloud");  
  viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");  
  viewer->addCoordinateSystem (1.0);  
  viewer->initCameraParameters ();
  return (viewer);
}
鼠标事件：
```
```cpp
void mouseEventOccurred (const pcl::visualization::MouseEvent &event,
                     void* viewer_void)
{
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *> (viewer_void);
  if (event.getButton () == pcl::visualization::MouseEvent::LeftButton && event.getType () == pcl::visualization::MouseEvent::MouseButtonRelease)
  {
    std::cout << "Left mouse button released at position (" << event.getX () << ", " << event.getY () << ")" << std::endl;
    char str[512];
    sprintf (str, "text#%03d", text_id ++);
    viewer->addText ("clicked here", event.getX (), event.getY (), str);
  }
}
```
键盘交互：
```cpp
void keyboardEventOccurred (const pcl::visualization::KeyboardEvent &event,
                        void* viewer_void)
{
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer = *static_cast<boost::shared_ptr<pcl::visualization::PCLVisualizer> *> (viewer_void);
  if (event.getKeySym () == "r" && event.keyDown ())
  {
    std::cout << "r was pressed => removing all text" << std::endl;
    char str[512];
    for (unsigned int i = 0; i < text_id; ++i)
    {
      sprintf (str, "text#%03d", i);
      viewer->removeShape (str);
    }
    text_id = 0;
  }
}
```
调用函数为：
```cpp
viewer->registerKeyboardCallback (keyboardEventOccurred, (void*)&viewer);
viewer->registerMouseCallback (mouseEventOccurred, (void*)&viewer);
```
摄像头初始化函数：
```cpp
boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
viewer->initCameraParameters ();
```
后记：
       具体的使用方法还有很多，参见官方文档。
