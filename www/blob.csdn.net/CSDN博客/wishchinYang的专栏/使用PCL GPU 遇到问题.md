# 使用PCL::GPU::遇到问题 - wishchinYang的专栏 - CSDN博客
2014年04月23日 15:38:48[wishchin](https://me.csdn.net/wishchin)阅读数：3747
**一：**使用GPU进行点云分割，理论上可以极大地加快分割速度；
      于是对PCL1.7.1进行了编译，回到32位系统，重设QT，编译成功(时间好漫长，一定要配置仔细，否则编译一次又一次浪费更多时间)；
使用时：
#include < pcl/gpu/segmentation/gpu_extract_cluster.h>或者
#include < pcl/gpu/segmentation/gpu_extract_cluster.h>
函数里面添加代码：
// Get params from function in
const boost::shared_ptr<pcl::PointCloud<pcl::PointXYZ> >  host_cloud_ =CloudSrc;
float                                                       tolerance =m_DistanceThre;
unsigned int                                      min_pts_per_cluster =MaxCloudNum;
unsigned int                                      max_pts_per_cluster =MinCloudNum;
std::vector<pcl::PointIndices>                                     cluster_indices;
//define the extractor
pcl::gpu::EuclideanClusterExtraction ec;
pcl::gpu::DeviceArray<pcl::PointXYZ>   DevicePointArray(CloudSrc->size());   // allocation the new GPU memory！
pcl::gpu::EuclideanClusterExtraction::GPUTreePtr treeGPU(new pcl::gpu::EuclideanClusterExtraction::GPUTree);
treeGPU->setCloud(DevicePointArray);
//Set paramS！
ec.setSearchMethod (treeGPU);
ec.setClusterTolerance (tolerance);
ec.setMinClusterSize (MinCloudNum);//ec.setMinClusterSize (100)
ec.setMaxClusterSize (MaxCloudNum);//ec.setMaxClusterSize (25000)
ec.setHostCloud (CloudSrc);
//ec.setInput (DevicePointArray);
ec.extract(cluster_indices);
由此转入函数调用  ，编译出现问题  工程出现重定义，反复修改几次
设置 extract 函数里面调用的函数为全局函数，消除了两个重定义；不过extract本身不能消除，也无法设置为静态成员函数；
**二：**没有办法，注释掉gpu_extract_cluster.cpp里面的HPP包含，
重新加入库，编译库，最后还是没有成功，出现同样问题。发现时源代码的问题
**三：**把gpu_extract_cluster.hpp 和gpu_extract_cluster.h从源代码里面抽离出来，
gpu_extract_cluster.hpp 修改为gpu_extract_cluster.cpp
把gpu_extract_cluster.cpp 和gpu_extract_cluster.h添加到项目里面
程序编译成功。
**四：**编译成功可以运行，
待程序运行到gpu_extract_cluster.cpp 的extract函数里面，
if( !tree_->isBuilt() )
{
tree_->build();//
}
建立八叉树的时候，显卡驱动出现崩溃！瞬间我快要心碎的哭了，这可如何是好啊！
错误提示：
       AviTest.exe 中的 0x74a71ee9 处有未经处理的异常: Microsoft C++ 异常: 内存位置 0x035ded58 处的 thrust::system::system_error。
这可到头了，trust虽然可以像C++库一样被使用，我可，没有时间再去调试这个东西了,一股蛋蛋的淡疼的感觉。
最终定位到 afxmem.cpp，定位到323下一句要执行的语句：
320#if !defined(_AFX_NO_DEBUG_CRT) && defined(_DEBUG)
321pResult = _malloc_dbg(nSize, _NORMAL_BLOCK, NULL, 0);
322#else
323pResult = malloc(nSize);
324#endif
（tree_->build()行提示 treesize =4915200 702000个pointxyz ，
而 pResult = malloc(nSize);提示为nSize=4294967295，坑死人的大空间，怎么分配啊！）
这难道是显卡驱动有问题？还是CUDA有问题，我只能孤独地在黑夜里提心吊胆地摸索着前行，一步一磕绊，痛苦不堪.....
路慢慢其修远兮，不想探索了！！！先放下，哪天想起来或者发现自己的错误在哪里了，再回头解决.
