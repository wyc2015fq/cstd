# PCL系列——如何使用迭代最近点法（ICP）配准

2016年03月31日 09:15:34

 

xuezhisdc

 

阅读数：12049

更多

所属专栏： [PCL](https://blog.csdn.net/column/details/pointcloudlibrary.html)



 版权声明：本文为博主原创文章，引用时请附上链接。	https://blog.csdn.net/xuezhisdc/article/details/51023251

> **博客新址:** [http://blog.xuezhisd.top](http://blog.xuezhisd.top/) 
> 邮箱：xuezhisd@126.com

------

**地平线机器人——嵌入式人工智能领导者** 长期招聘**计算机视觉/深度学习**等方向的工程师或实习生。感兴趣的话，请发送邮件，可以**内推**。

------

## PCL系列

- [PCL系列——读入PCD格式文件操作](http://blog.csdn.net/xuezhisdc/article/details/51012300)
- [PCL系列——将点云数据写入PCD格式文件](http://blog.csdn.net/xuezhisdc/article/details/51012463)
- [PCL系列——拼接两个点云](http://blog.csdn.net/xuezhisdc/article/details/51014388)
- [PCL系列——从深度图像(RangeImage)中提取NARF关键点](http://blog.csdn.net/xuezhisdc/article/details/51018872)
- [PCL系列——如何可视化深度图像](http://blog.csdn.net/xuezhisdc/article/details/51019461)
- [PCL系列——如何使用迭代最近点法（ICP）配准](http://blog.csdn.net/xuezhisdc/article/details/51023251)
- [PCL系列——如何逐渐地配准一对点云](http://blog.csdn.net/xuezhisdc/article/details/51030943)
- [PCL系列——三维重构之泊松重构](http://blog.csdn.net/xuezhisdc/article/details/51034189)
- [PCL系列——三维重构之贪婪三角投影算法](http://blog.csdn.net/xuezhisdc/article/details/51034272)
- [PCL系列——三维重构之移动立方体算法](http://blog.csdn.net/xuezhisdc/article/details/51034359)

## 说明

通过本教程，我们将会学会：

- 如何使用迭代最近点法（Iterative Closest Point）判断一个点云是否是另一个点云的刚体变换
- 使用的方法是：最小化两个点云中对应点之间的距离，并刚性变换他们。
- 代码说明：先生成输入点云（待变换的点云）和目标点云（变换参照），然后创建ICP对象，设置该对象的输入点[云和](https://www.baidu.com/s?wd=%E4%BA%91%E5%92%8C&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)目标点云，然后进行配准，并显示ICP配准信息和变换矩阵。

## 操作

- 在VS2010 中新建一个文件 iterative_closest_point.cpp，然后将下面的代码复制到文件中。
- 参照之前的文章，配置项目的属性。设置包含目录和库目录和附加依赖项。

```cpp
#include <iostream> //标准输入/输出
#include <pcl/io/pcd_io.h> //pcd文件输入/输出
#include <pcl/point_types.h> //各种点类型
#include <pcl/registration/icp.h> //ICP(iterative closest point)配准

int main (int argc, char** argv)
{
  //创建点云指针
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in (new pcl::PointCloud<pcl::PointXYZ>); //创建输入点云（指针）
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out (new pcl::PointCloud<pcl::PointXYZ>); //创建输出/目标点云（指针）

  //生成并填充点云cloud_in
  cloud_in->width    = 5;
  cloud_in->height   = 1;
  cloud_in->is_dense = false;
  cloud_in->points.resize (cloud_in->width * cloud_in->height); //变形，无序
  for (size_t i = 0; i < cloud_in->points.size (); ++i) //随机数初始化点的坐标
  {
    cloud_in->points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud_in->points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud_in->points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
  }
  std::cout << "Saved " << cloud_in->points.size () << " data points to input:"
      << std::endl;
  //打印点云cloud_in中所有点的坐标信息
  for (size_t i = 0; i < cloud_in->points.size (); ++i) std::cout << "    " <<
      cloud_in->points[i].x << " " << cloud_in->points[i].y << " " <<
      cloud_in->points[i].z << std::endl;

  // 填充点云cloud_out
  *cloud_out = *cloud_in; //初始化cloud_out
  std::cout << "size:" << cloud_out->points.size() << std::endl;
  for (size_t i = 0; i < cloud_in->points.size (); ++i)
    cloud_out->points[i].x = cloud_in->points[i].x + 0.7f; //平移cloud_in得到cloud_out
  std::cout << "Transformed " << cloud_in->points.size () << " data points:"
      << std::endl;
  //打印点云cloud_out中所有点的坐标信息（每一行对应一个点的xyz坐标）
  for (size_t i = 0; i < cloud_out->points.size (); ++i)
    std::cout << "    " << cloud_out->points[i].x << " " <<
      cloud_out->points[i].y << " " << cloud_out->points[i].z << std::endl;
  //*********************************
  // ICP配准
  //*********************************
  pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp; //创建ICP对象，用于ICP配准
  icp.setInputCloud(cloud_in); //设置输入点云
  icp.setInputTarget(cloud_out); //设置目标点云（输入点云进行仿射变换，得到目标点云）
  pcl::PointCloud<pcl::PointXYZ> Final; //存储结果
  //进行配准，结果存储在Final中
  icp.align(Final); 
  //输出ICP配准的信息（是否收敛，拟合度）
  std::cout << "has converged:" << icp.hasConverged() << " score: " <<
  icp.getFitnessScore() << std::endl;
  //输出最终的变换矩阵（4x4）
  std::cout << icp.getFinalTransformation() << std::endl;

 return (0);
}123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657
```

- 重新生成项目。
- 到改项目的[Debug](https://www.baidu.com/s?wd=Debug&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)目录下，按住Shift，同时[点击鼠标](https://www.baidu.com/s?wd=%E7%82%B9%E5%87%BB%E9%BC%A0%E6%A0%87&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)右键，在当前窗口打开CMD窗口。
- 在命令行中输入`iterative_closest_point.exe`，可以得到下图所示结果。 
  ![这里写图片描述](https://img-blog.csdn.net/20160331091512850)

## 参考

- [How to use iterative closest point](http://pointclouds.org/documentation/tutorials/iterative_closest_point.php#iterative-closest-point)