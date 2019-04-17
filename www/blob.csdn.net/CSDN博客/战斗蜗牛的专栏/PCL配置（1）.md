# PCL配置（1） - 战斗蜗牛的专栏 - CSDN博客





2012年05月11日 10:17:45[vbskj](https://me.csdn.net/vbskj)阅读数：4831









### [开源三维点云平台——PCL(Point Cloud Library)](http://blog.csdn.net/giantchen547792075/article/details/7409303)


分类： [PCL](http://blog.csdn.net/giantchen547792075/article/category/1112381)[点云](http://blog.csdn.net/giantchen547792075/article/category/1112382)[三维GIS](http://blog.csdn.net/giantchen547792075/article/category/1112383)2012-03-29
 23:07408人阅读[评论](http://blog.csdn.net/giantchen547792075/article/details/7409303#comments)(0)[收藏](http://blog.csdn.net/giantchen547792075/article/details/7409303)[举报](http://blog.csdn.net/giantchen547792075/article/details/7409303#report)


# 如何在Windows環境中搭建PCL——Point Cloud Library開發環境
- 下載PCL的[ALL_IN_ONE安裝包](http://pointclouds.org/downloads/windows.html)


- 使用CMake構建簡單實例

打開CMake界面程序,在文件夾下將CMakelist.txt文件直接拖到界面程序上運行即可

- 注意事項

在make的過程中要注意編譯器的版本,像我在Windows 7 64位 SP1 操作系統上裝了Visual Studio 2010，在configure時要選擇MSVC_10.0之類的選項,避免選擇MSVC_9.0等（如果你用的是Visual Studio 2008，用msvc_9.0是沒有錯的


# 如何快速構建PCL應用程序

## 使用CMake
- 編寫PCL代碼 包含PCL的頭文件，使用PCL的函數和數據結構構建簡易的界面實現可示化。
- 運行CMake_GUI

直接使用Configure 與Generate工具，你可以在界面上找到相應的按鈕。

- 行Visual Studio 調試程序

在使用CMake 編譯完成之后就生成了一個 Visual Studio 2010 解決方案當中有三個項目，其中兩個是輔助工程。

- 注意事項

## 使用Visual Studio
- 路徑構建
- 庫文件查找
- 調試運行
- 改正錯誤

## 庫文件與動態庫的配置
- 文件夾
- Windows尋址方式

# 代碼剖析

## PCL_Write

頭文件包含：iostream，pcd_io.h，point_type.h

數據類型： pcl父類，PointCloud模板類，PointXYZRGB點數據類。。。。 細節處理：

- 初始化點云結構

pcl::PointCloud cloud;

按照對應的數據要求填充cloud數據，像width，height，is_dense（是否稠密），通過resize函數應用相應的參數對數据進行更新。

cloud是一個PointCloud數據模板，可以通過更新其x，y，z，rgb屬性來更新殿云數據

- 數據文件輸出

最後用戶通過pcl的io接口中的savePCDFileASCII將數據以ASCII碼的形式存儲在pcd文件中








## cloud_Viewer

這是一個查看有PCL_writer保存的pcd數據的Console程序從頭文件中我們就可以看到許多都包含了可眎化的元素，如visualization/cloud_viwer.h,pcl/io/io.h,pcl/io/pcd_io.h
- 
### pcl::visualization::PCLVisualizer



- 
### pcl::PointCloud::Ptr



- 
### pcl::io::loadPCDFile(string name, PointXYZRGB *cloud)



- 
### pcl::visualization::CloudViewer



- 
### showCloud(PointXYZRGB *cloud)方法



- 
### runOnVisualizationThread( void function() )


友情链接:(这两位讲得更细致)

[http://blog.csdn.net/lynnandwei/article/details/6913538](http://blog.csdn.net/lynnandwei/article/details/6913538)

[http://blog.csdn.net/halfwet/article/details/6927682](http://blog.csdn.net/halfwet/article/details/6927682)

[这里](http://download.csdn.net/detail/giantchen547792075/4194896)有我编译好的可执行程序....只是一个console,见笑了....![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)




