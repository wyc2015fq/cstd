# 关于编译PCL1.71 - wishchinYang的专栏 - CSDN博客
2013年11月06日 22:10:31[wishchin](https://me.csdn.net/wishchin)阅读数：3788
最近在编译PCL1.71时总会出现错误，
编译的时候就出现无法生成pcl_io_debug.lib
由于无法生成pcl_io_debug.lib,。
借鉴PCL中国的经验：
（1）：把io\include\pcl\io\png_io.h文件报错地方”template <“的前四行注释掉，重新编译io模块
//    PCL_DEPRECATED (template <typename T> void savePNGFile (const std::string& file_name, const pcl::PointCloud<T>& cloud),
//    "pcl::io::savePNGFile<typename T> (file_name, cloud) is deprecated, please use a new generic "
//    "function pcl::io::savePNGFile (file_name, cloud, field_name) with \"rgb\" as the field name."
//    );
（2）：不用去掉，改成
template <typename T> PCL_DEPRECATED ( void savePNGFile (const std::string& file_name, const pcl::PointCloud<T>& cloud),
    "pcl::io::savePNGFile<typename T> (file_name, cloud) is deprecated, please use a new generic "
    "function pcl::io::savePNGFile (file_name, cloud, field_name) with \"rgb\" as the field name."
    );
common/include/pcl/ros/conversions.h 同样改法。

