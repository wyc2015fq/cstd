# 把pcl的VTK显示融合到MFC（代码找原作者） - wishchinYang的专栏 - CSDN博客
2013年11月05日 20:48:57[wishchin](https://me.csdn.net/wishchin)阅读数：8666
转自PCL中国，原文链接：[http://www.pclcn.org/bbs/forum.php?mod=viewthread&tid=223&extra=page%3D1](http://www.pclcn.org/bbs/forum.php?mod=viewthread&tid=223&extra=page%3D1)
本人做了少量修改！
1：系统平台：XP/win7（我都做过，可以实现）
工具：visual studio 2010，PCL-1.6.0-AllInOne-msvc2010-win32，VTK5.10（[用Cmake编译一下，网上都有教程](http://blog.csdn.net/left_la/article/details/7069708)，注意：PCL-1.6.0-AllInOne-msvc2010-win32中提供的VTK5.8缺少关键文件vtkMFCWindow.h和vktMFC.lib，所以无法实现）
2 实现过程
2.1 建立基于对话框的工程：PCLDialog
（我的工程路径D:\Program\VS2010\Project\PCLDialog）图1所示
2.2将我提供pclvisualization_mfc文件夹（修改过的源代码）放到自己所建的工程中文件夹下(如有需要，请回复留下邮箱，我可以发给大家)
（D:\Program\VS2010\Project\PCLDialog\PCLDialog\pclvisualization_mfc）
其中源文件和头文件，如图1所示：
将源文件和头文件添加到工程中，如图2所示
**********************************************************************************************
2.3 具体实现
2.3.1 
为工程添加额外引用路径（根据自己程序安装路径而定）
```cpp
C:\Program Files\VTK5.10\include\vtk-5.10;
C:\Program Files\PCL 1.6.0\3rdParty\Boost\include;
C:\Program Files\PCL 1.6.0\include\pcl-1.6;
C:\Program Files\PCL 1.6.0\3rdParty\Eigen\include;
C:\Program Files\PCL 1.6.0\3rdParty\Flann\include;
D:\Program\VS2010\Project\PCLDialog\PCLDialog;
```
2.3.2 
添加附加库路径（根据自己程序安装路径而定）
```cpp
C:\Program Files\PCL 1.6.0\3rdParty\Qhull\lib;
C:\Program Files\VTK5.10\lib;
C:\Program Files\PCL 1.6.0\3rdParty\Boost\lib;
C:\Program Files\PCL 1.6.0\lib;
C:\Program Files\PCL 1.6.0\3rdParty\Flann\lib;
C:\Program Files\OpenNI\Lib;
C:\Program Files\PCL 1.6.0\3rdParty\Eigen\bin;
```
2.3.3 
为工程添加附加依赖项（我把所有需要的*.lib;文件都添加了，不一定都用的着）
附加项，见附件
2.3.4 
在工程属性字符集中选择UseMulti-Byte Character Set，如果用Unicode， PCL源文件有太多要改，后面编译通不过。（总之我试着解决了，总是出错，后来就放弃了，请高手指点）
2.3.5在对话框中插入picture control控件ID：IDC_PCDVIEWER。
2.3.6 
在PCLDialog.h中添加
头文件
```cpp
#include "pcl\point_cloud.h"
    #include "pcl\point_types.h"
    #include "pclvisualization_mfc\include\pcl_mfc_visualizer.h"
    #include "vtkRenderer.h"
    #include <pcl/io/pcd_io.h>
```
*复制代码*
新建私有变量
```cpp
private:
        //视图窗口
        pcl::mfc_visualization::PCLVisualizer *viewer;
        //vtkRenderer *pvtkRenderer;
        POINT  ptBorder;
        //数据点
        sensor_msgs::PointCloud2::Ptr binary_blob;
        //数据点句柄
        pcl::mfc_visualization::PointCloudGeometryHandlerXYZ<sensor_msgs::PointCloud2>::Ptr xyz_Handler;
        pcl::mfc_visualization::PointCloudColorHandlerRGBField<sensor_msgs::PointCloud2>::Ptr color_Handler;
        //传感器位置方向矩阵
        Eigen::Vector4f sensor_origin;
        Eigen::Quaternion<float> sensor_orientation;
```
*复制代码*
2.3.7 
初始化
```cpp
CPCLDialogDlg::CPCLDialogDlg(CWnd* pParent /*=NULL*/): CDialogEx(CPCLDialogDlg::IDD, pParent)
    {
            m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
            ///////////////////////////////////////////////////////////////////////////////////////////////////
            //begin tyz
            //初始化
            this->viewer = NULL;
        sensor_origin = Eigen::Vector4f::Zero();
            sensor_orientation = Eigen::Quaternionf::Identity ();
    }
```
*复制代码*
2.3.8 在BOOL CPCLDialogDlg::OnInitDialog()函数中添加
```cpp
////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //利用PictControl控件加载PCD窗口
            CWnd *viewer_pcWnd;
            viewer_pcWnd = this->GetDlgItem(IDC_PCDVIWER);
            this->viewer = new pcl::mfc_visualization::PCLVisualizer(viewer_pcWnd);
            CRect cRectPCL;
            this->viewer->GetClientRect(&cRectPCL);
            CRect cRectClient;
            GetClientRect(&cRectClient);
            this->ptBorder.x = cRectClient.Width()  - cRectPCL.Width();
            this->ptBorder.y = cRectClient.Height() - cRectPCL.Height();
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
```
2.3.9 
添加消息WM_SIZE响应函数OnSize，保证PCLVisualizer窗口viewer随对话框变化。
```cpp
void CPCLDialogDlg::OnSize(UINT nType, int cx, int cy)
{
        CDialogEx::OnSize(nType, cx, cy);
        // TODO: Add your message handler code here
        if (::IsWindow(this->GetSafeHwnd())) {
                if (this->viewer){
                        cx -= ptBorder.x;
                        cy -= ptBorder.y;
                        this->GetDlgItem(IDC_PCLVIWER)->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
                        this->viewer->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
                }
      }
}
```
*复制代码*
2.3.10 
新建button控件，caption：Open PCD，ID：IDC_OPENPCD，为控件添加响应函OnBnClickedOpenpcd()
```cpp
void CPCLDialogDlg::OnBnClickedOpenpcd()
    {
            this->viewer->removeAllPointClouds ();
            // TODO: Add your control notification handler code here
            static TCHAR BASED_CODE szFilter[] = _T("PCD (*.pcd )|*.pcd||");
            CFileDialog cFileDialog(true, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT| OFN_NOCHANGEDIR ,szFilter);
            if (cFileDialog.DoModal() == IDOK)
            {
                    /////////////////////////////////////////////////////////////////////////////
                    //文档名称
                    std::string filename;
                    filename = cFileDialog.GetFileName();
                    //reset data
                    this->binary_blob.reset();
                    binary_blob = sensor_msgs::PointCloud2::Ptr (new sensor_msgs::PointCloud2);
                    // read new data
                    //*.pcd文件
                    pcl::PCDReader pcd_reader;        
                    if (pcd_reader.read ((char*)_bstr_t(filename.c_str()), *binary_blob) != 0) //* load the file
                    {
                            MessageBox (_T("Couldn't read PCData file!"));
                            return;
                    }        
            }
            if (binary_blob == NULL)
            {
                    MessageBox("Please load PCD file firstly!");
                    return;
            }
            else
            {
                    //其他句柄
                    if (pcl::getFieldIndex(*binary_blob, "rgb") > 0)
                    {
                            color_Handler = pcl::mfc_visualization::PointCloudColorHandlerRGBField<sensor_msgs::PointCloud2>::Ptr
                                    (new pcl::mfc_visualization::PointCloudColorHandlerRGBField<sensor_msgs::PointCloud2> (binary_blob));
                            this->viewer->addPointCloud(binary_blob, color_Handler, sensor_origin, sensor_orientation);
                    }
                    else
                    {
                            xyz_Handler = pcl::mfc_visualization::PointCloudGeometryHandlerXYZ<sensor_msgs::PointCloud2>::Ptr
                                    (new pcl::mfc_visualization::PointCloudGeometryHandlerXYZ<sensor_msgs::PointCloud2> (binary_blob));
                            this->viewer->addPointCloud(binary_blob, xyz_Handler, sensor_origin, sensor_orientation);
                    }        
                    this->viewer->resetCamera();
            }
    }
<em>复制代码</em>
```
2.3.11 
：最终效果，图4所示
2.3.12：实现多项功能的程序效果，如图5所示
      由于级别不够所以附件大小限制，这里只给出图4和图5，图1~3不重要，所以不给出了。压缩文件是修改好的源代码(如有需要请回复留下邮箱，我可以发给大家)，按说明操作即可。
       修改源代码的使用请看[如何实现基于MFC对话框的PCL显示（4）——关于修改源代码的使用](http://www.pclcn.org/bbs/forum.php?mod=viewthread&tid=227&extra=)。
- 
[图5 实现多功能对话框的效果图.png](http://www.pclcn.org/bbs/forum.php?mod=attachment&aid=MTI4fDI3YWVmNzMwfDEzODM2NTU2ODZ8NDI0fDIyNQ%3D%3D&nothumb=yes)*(264.06 KB, 下载次数: 11)*
![图5 实现多功能对话框的效果图.png](http://www.pclcn.org/bbs/data/attachment/forum/201305/17/163311bidgaxexydix0dug.png)
- 
[图4 最终效果图.png](http://www.pclcn.org/bbs/forum.php?mod=attachment&aid=MTI5fGMwNWNhOTkxfDEzODM2NTU2ODZ8NDI0fDIyNQ%3D%3D&nothumb=yes)*(71.08 KB, 下载次数: 6)*
![图4 最终效果图.png](http://www.pclcn.org/bbs/data/attachment/forum/201305/17/163312rnxo7oamooo9px9a.png)
附加注释：
转载请注明出处（[http://www.pclcn.org/bbs/forum.php?mod=viewthread&tid=227&extra=](http://www.pclcn.org/bbs/forum.php?mod=viewthread&tid=227&extra=)）
       所提供的修改源代码（如有需要，回复留下邮箱，我发给大家）是基于PCL官网提供的源文件（[PCL-1.6.0-Source.tar.bz2](http://sourceforge.net/projects/pointclouds/files/1.6.0/PCL-1.6.0-Source.tar.bz2/download)）内的visualization文件夹下的相关源文件和头文件的基础上修改完成的。主要思想是基于vtkMFCWindow类实现的，将PCL显示窗口类pcl::visualization::PCLVisualizer由原来的基于vtkRenderWindow类改成基于vtkMFCWindow类的新类pcl::mfc_visualization::PCLVisualizer。而且新类需要指向所依托显示的子窗口句柄（有机会在慢慢解释给大家吧，还没太理顺，大概就是这么个思想）。
      修改源代码 相当于对原pcl::visualization Namespace 修改得pcl::mfc_visualization Namespace，所以在使用过程中，相关功能的实现可以参照官网教程，但是涉及到pcl::visualization Namespace的全部修改为pcl::mfc_visualization。
       例如前面在新建pcl::visualization::PCLVisualizer类时MFC对话框应用中需要新建pcl::mfc_visualization::PCLVisualizer类，其他的也类似，如数据点句柄（[参见如何实现基于MFC对话框的PCL显示
 2](http://www.pclcn.org/bbs/forum.php?mod=viewthread&tid=224&page=1&extra=#pid751)的新建私有变量代码）。
        对象pcl::mfc_visualization::PCLVisualizer *viewer的初始化，在构造对话框时
```cpp
this->viewer=NULL;
```
对话框初始化时将viewer指向Picture Control的窗口句柄
```cpp
CWnd *viewer_pcWnd;
            viewer_pcWnd = this->GetDlgItem(IDC_PCDVIWER);
            this->viewer = new pcl::mfc_visualization::PCLVisualizer(viewer_pcWnd);<em>复制代码</em>
```
         经过以上几步就可完成窗口的基本建设，剩下的都是MFC对话框的基本性质了。还有我在 
pcl::mfc_visualization Namespace中新加了几个函数，随后给大家解释和用法。
