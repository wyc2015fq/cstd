# PCL系列——将点云数据写入PCD格式文件

2016年03月30日 10:53:42

 

xuezhisdc

 

阅读数：14350

 

标签： [PCL](http://so.csdn.net/so/search/s.do?q=PCL&t=blog)[pcd](http://so.csdn.net/so/search/s.do?q=pcd&t=blog) 更多

所属专栏： [PCL](https://blog.csdn.net/column/details/pointcloudlibrary.html)



版权声明：本文为博主原创文章，引用时请附上链接。	https://blog.csdn.net/xuezhisdc/article/details/51012463

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

## 操作

- 在VS2010 中新建一个文件pcd_write.cpp，然后将下面的代码复制到文件中。
- 参照之前的文章，配置项目的属性。设置包含目录和库目录和附加依赖项。

```c
#include <iostream> //标准输入输出流
#include <pcl/io/pcd_io.h> //PCL的PCD格式文件的输入输出头文件
#include <pcl/point_types.h> //PCL对各种格式的点的支持头文件

int  main (int argc, char** argv)
{
  pcl::PointCloud<pcl::PointXYZ> cloud; // 创建点云（不是指针）

  //填充点云数据
  cloud.width    = 5; //设置点云宽度
  cloud.height   = 1; //设置点云高度
  cloud.is_dense = false; //非密集型
  cloud.points.resize (cloud.width * cloud.height); //变形，无序
    //设置这些点的坐标
  for (size_t i = 0; i < cloud.points.size (); ++i)
  {
    cloud.points[i].x = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud.points[i].y = 1024 * rand () / (RAND_MAX + 1.0f);
    cloud.points[i].z = 1024 * rand () / (RAND_MAX + 1.0f);
  }
    //保存到PCD文件
  pcl::io::savePCDFileASCII ("test_pcd.pcd", cloud); //将点云保存到PCD文件中
  std::cerr << "Saved " << cloud.points.size () << " data points to test_pcd.pcd." << std::endl;
    //显示点云数据
  for (size_t i = 0; i < cloud.points.size (); ++i)
    std::cerr << "    " << cloud.points[i].x << " " << cloud.points[i].y << " " << cloud.points[i].z << std::endl;

  return (0);
}1234567891011121314151617181920212223242526272829
```

- 运行结束后，在项目目录下生成test_pcd.pcd文件，内容如下图所示。 
  ![这里写图片描述](https://img-blog.csdn.net/20160330105303294)

## 参考

[Writing Point Cloud data to PCD files](http://pointclouds.org/documentation/tutorials/writing_pcd.php#writing-pcd)