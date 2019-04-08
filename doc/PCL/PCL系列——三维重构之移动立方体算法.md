# PCL系列——三维重构之移动立方体算法

2016年04月01日 09:54:37

 

xuezhisdc

 

阅读数：8319

更多

所属专栏： [PCL](https://blog.csdn.net/column/details/pointcloudlibrary.html)



 版权声明：本文为博主原创文章，引用时请附上链接。	https://blog.csdn.net/xuezhisdc/article/details/51034359

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

- 如果通过移动立方体算法进行三维点云重构。
- 程序支持两种文件格式：`*.pcd`和`*.ply`。
- 程序先读取点云文件；然后计算法向量，并将法向量和点云坐标放在一起；接着使用移动立方体算法进行重构，最后显示结果。

## 操作

- 在VS2010 中新建一个文件 `recon_marchingCubes.cpp`，然后将下面的代码复制到文件中。
- 参照之前的文章，配置项目的属性。设置包含目录和库目录和附加依赖项。

```cpp
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/ply_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/marching_cubes_hoppe.h>
#include <pcl/surface/marching_cubes_rbf.h>
#include <pcl/surface/gp3.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>

int main (int argc, char** argv)
{
    // 确定文件格式
    char tmpStr[100];
    strcpy(tmpStr,argv[1]);
    char* pext = strrchr(tmpStr, '.');
    std::string extply("ply");
    std::string extpcd("pcd");
    if(pext){
        *pext='\0';
        pext++;
    }
    std::string ext(pext);
    //如果不支持文件格式，退出程序
    if (!((ext == extply)||(ext == extpcd))){
        std::cout << "文件格式不支持!" << std::endl;
        std::cout << "支持文件格式：*.pcd和*.ply！" << std::endl;
        return(-1);
    }

    //根据文件格式选择输入方式
  pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>) ; //创建点云对象指针，用于存储输入
    if (ext == extply){
        if (pcl::io::loadPLYFile(argv[1] , *cloud) == -1){
            PCL_ERROR("Could not read ply file!\n") ;
            return -1;
        }
    }
    else{
        if (pcl::io::loadPCDFile(argv[1] , *cloud) == -1){
            PCL_ERROR("Could not read pcd file!\n") ;
            return -1;
        }
    }

  // 估计法向量
  pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
  pcl::PointCloud<pcl::Normal>::Ptr normals (new pcl::PointCloud<pcl::Normal>);
  pcl::search::KdTree<pcl::PointXYZ>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZ>);
  tree->setInputCloud(cloud);
  n.setInputCloud(cloud);
  n.setSearchMethod(tree);
  n.setKSearch(20);
  n.compute (*normals); //计算法线，结果存储在normals中
  //* normals 不能同时包含点的法向量和表面的曲率

  //将点云和法线放到一起
  pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals (new pcl::PointCloud<pcl::PointNormal>);
  pcl::concatenateFields (*cloud, *normals, *cloud_with_normals);
  //* cloud_with_normals = cloud + normals


  //创建搜索树
  pcl::search::KdTree<pcl::PointNormal>::Ptr tree2 (new pcl::search::KdTree<pcl::PointNormal>);
  tree2->setInputCloud (cloud_with_normals);

  //初始化MarchingCubes对象，并设置参数
    pcl::MarchingCubes<pcl::PointNormal> *mc;
    mc = new pcl::MarchingCubesHoppe<pcl::PointNormal> ();
    /*
  if (hoppe_or_rbf == 0)
    mc = new pcl::MarchingCubesHoppe<pcl::PointNormal> ();
  else
  {
    mc = new pcl::MarchingCubesRBF<pcl::PointNormal> ();
    (reinterpret_cast<pcl::MarchingCubesRBF<pcl::PointNormal>*> (mc))->setOffSurfaceDisplacement (off_surface_displacement);
  }
    */

    //创建多变形网格，用于存储结果
  pcl::PolygonMesh mesh;

  //设置MarchingCubes对象的参数
  mc->setIsoLevel (0.0f);
  mc->setGridResolution (50, 50, 50);
  mc->setPercentageExtendGrid (0.0f);

  //设置搜索方法
  mc->setInputCloud (cloud_with_normals);

    //执行重构，结果保存在mesh中
    mc->reconstruct (mesh);

    //保存网格图
    pcl::io::savePLYFile("result.ply", mesh);

    // 显示结果图
  boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
  viewer->setBackgroundColor (0, 0, 0); //设置背景
  viewer->addPolygonMesh(mesh,"my"); //设置显示的网格
  viewer->addCoordinateSystem (1.0); //设置坐标系
  viewer->initCameraParameters ();
  while (!viewer->wasStopped ()){
    viewer->spinOnce (100);
    boost::this_thread::sleep (boost::posix_time::microseconds (100000));
  }

  return (0);
}
```

- 重新生成项目。
- 到改项目的Debug目录下，按住Shift，同时[点击鼠标](https://www.baidu.com/s?wd=%E7%82%B9%E5%87%BB%E9%BC%A0%E6%A0%87&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)右键，在当前窗口打开CMD窗口。
- 在命令行中输入`recon_marchingCubes.exe bunny.points.ply`，执行程序。得到如下图所示的结果。 
  ![移动立方体算法的结果](https://img-blog.csdn.net/20160401095126535)