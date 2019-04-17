# PCL 1.6.0 VS2010 Configuration - Grandyang - 博客园







# [PCL 1.6.0 VS2010 Configuration](https://www.cnblogs.com/grandyang/p/4464560.html)







**Open VS2010, create a new project, then open Property Manager, double-click Microsoft.Cpp.win32.user, open the Property Pages. Then go to VC++ Directories, add in the Include Directories:**
C:\Program Files (x86)\PCL 1.6.0\3rdParty\Boost\include
C:\Program Files (x86)\PCL 1.6.0\3rdParty\Eigen\include
C:\Program Files (x86)\PCL 1.6.0\3rdParty\flann\include
C:\Program Files (x86)\PCL 1.6.0\3rdParty\qhull\include
C:\Program Files (x86)\PCL 1.6.0\3rdParty\VTK\include\vtk-5.8
C:\Program Files (x86)\PCL 1.6.0\include\pcl-1.6
C:\Program Files (x86)\OpenNI\Include

**Add in the Library Directories:**
C:\Program Files (x86)\PCL 1.6.0\3rdParty\Boost\lib
C:\Program Files (x86)\PCL 1.6.0\3rdParty\flann\lib
C:\Program Files (x86)\PCL 1.6.0\3rdParty\qhull\lib
C:\Program Files (x86)\PCL 1.6.0\3rdParty\VTK\lib\vtk-5.8
C:\Program Files (x86)\PCL 1.6.0\lib
C:\Program Files (x86)\OpenNI\Lib

**Go to Linker -> Input -> Additional Dependencies:**

****If you build the project under DEBUG mode, add the following:****

opengl32.lib

pcl_kdtree_debug.lib

pcl_io_debug.lib

pcl_search_debug.lib

pcl_segmentation_debug.lib

pcl_apps_debug.lib

pcl_features_debug.lib

pcl_filters_debug.lib

pcl_visualization_debug.lib

pcl_common_debug.lib

pcl_io_ply_debug.lib

pcl_keypoints_debug.lib

pcl_octree_debug.lib

pcl_registration_debug.lib

pcl_sample_consensus_debug.lib

pcl_surface_debug.lib

pcl_tracking_debug.lib

flann_cpp_s-gd.lib

flann_cuda_s-gd.lib

flann_s-gd.lib

flann-gd.lib

libboost_system-vc100-mt-gd-1_47.lib

libboost_filesystem-vc100-mt-gd-1_47.lib

libboost_thread-vc100-mt-gd-1_47.lib

libboost_date_time-vc100-mt-gd-1_47.lib

libboost_iostreams-vc100-mt-gd-1_47.lib

vtkalglib-gd.lib

vtkCharts-gd.lib

vtkCommon-gd.lib

vtkDICOMParser-gd.lib

vtkexoIIc-gd.lib

vtkexpat-gd.lib

vtkFiltering-gd.lib

vtkfreetype-gd.lib

vtkftgl-gd.lib

vtkGenericFiltering-gd.lib

vtkGeovis-gd.lib

vtkGraphics-gd.lib

vtkhdf5-gd.lib

vtkHybrid-gd.lib

vtkImaging-gd.lib

vtkInfovis-gd.lib

vtkIO-gd.lib

vtkjpeg-gd.lib

vtklibxml2-gd.lib

vtkmetaio-gd.lib

vtkNetCDF-gd.lib

vtkNetCDF_cxx-gd.lib

vtkpng-gd.lib

vtkproj4-gd.lib

vtkRendering-gd.lib

vtksqlite-gd.lib

vtksys-gd.lib

vtktiff-gd.lib

vtkverdict.lib

vtkViews.lib

vtkVolumeRendering.lib

vtkWidgets.lib

vtkzlib.lib



**If you build the project under RELEASE mode, add the following:**

opengl32.lib

pcl_kdtree_release.lib

pcl_io_release.lib

pcl_search_release.lib

pcl_segmentation_release.lib

pcl_apps_release.lib

pcl_features_release.lib

pcl_filters_release.lib

pcl_visualization_release.lib

pcl_common_release.lib

flann_cpp_s.lib

flann.lib

flann_cuda_s.lib

flann_s.lib

libboost_system-vc100-mt-1_47.lib

libboost_filesystem-vc100-mt-1_47.lib

libboost_thread-vc100-mt-1_47.lib

libboost_date_time-vc100-mt-1_47.lib

libboost_iostreams-vc100-mt-1_47.lib

vtkalglib.lib

vtkCharts.lib

vtkCommon.lib

vtkDICOMParser.lib

vtkexoIIc.lib

vtkexpat.lib

vtkFiltering.lib

vtkfreetype.lib

vtkftgl.lib

vtkGenericFiltering.lib

vtkGeovis.lib

vtkGraphics.lib

vtkhdf5.lib

vtkHybrid.lib

vtkImaging.lib

vtkInfovis.lib

vtkIO.lib

vtkjpeg.lib

vtklibxml2.lib

vtkmetaio.lib

vtkNetCDF.lib

vtkNetCDF_cxx.lib

vtkpng.lib

vtkproj4.lib

vtkRendering.lib

vtksqlite.lib

vtksys.lib

vtktiff.lib

vtkverdict.lib

vtkViews.lib

vtkVolumeRendering.lib

vtkWidgets.lib

vtkzlib.lib



**If want to use with Qt, also include:**

QVTK.lib



**Add following in the path:**
C:\Program Files (x86)\PCL 1.6.0\bin
C:\Program Files (x86)\PCL 1.6.0\3rdParty\FLANN\bin
C:\Program Files (x86)\PCL 1.6.0\3rdParty\Qhull\bin



**Add following in the CPP file:**

#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>














