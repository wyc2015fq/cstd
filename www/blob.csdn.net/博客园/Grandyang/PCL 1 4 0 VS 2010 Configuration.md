# PCL 1.4.0 VS 2010 Configuration - Grandyang - 博客园







# [PCL 1.4.0 VS 2010 Configuration](https://www.cnblogs.com/grandyang/p/4251820.html)






**Open VS2010, create a new project, then open Property Manager, double-click Microsoft.Cpp.win32.user, open the Property Pages. Then go to VC++ Directories, add in the Include Directories:**
C:\Program Files (x86)\PCL 1.4.0\3rdParty\Boost\include
C:\Program Files (x86)\PCL 1.4.0\3rdParty\Eigen\include
C:\Program Files (x86)\PCL 1.4.0\3rdParty\flann\include
C:\Program Files (x86)\PCL 1.4.0\3rdParty\qhull\include
C:\Program Files (x86)\PCL 1.4.0\3rdParty\VTK 5.8.0\include\vtk-5.8
C:\Program Files (x86)\PCL 1.4.0\include\pcl-1.4
C:\Program Files (x86)\OpenNI\Include

**Add in the Library Directories:**
C:\Program Files (x86)\PCL 1.4.0\3rdParty\Boost\lib
C:\Program Files (x86)\PCL 1.4.0\3rdParty\flann\lib
C:\Program Files (x86)\PCL 1.4.0\3rdParty\qhull\lib
C:\Program Files (x86)\PCL 1.4.0\3rdParty\VTK 5.8.0\lib\vtk-5.8
C:\Program Files (x86)\PCL 1.4.0\lib
C:\Program Files (x86)\OpenNI\Lib

**Go to Linker -> Input -> Additional Dependencies:**

****If you build the project under DEBUG mode, add the following:****

libboost_system-vc100-mt-gd-1_47.lib
libboost_filesystem-vc100-mt-gd-1_47.lib
libboost_thread-vc100-mt-gd-1_47.lib
libboost_date_time-vc100-mt-gd-1_47.lib
libboost_iostreams-vc100-mt-gd-1_47.lib
pcl_common-gd.lib
pcl_octree-gd.lib
openNI.lib
vtkCommon-gd.lib
vtkRendering-gd.lib
vtkHybrid-gd.lib
pcl_io-gd.lib
pcl_sample_consensus-gd.lib
flann_cpp_s-gd.lib
pcl_kdtree-gd.lib
pcl_search-gd.lib
pcl_filters-gd.lib
pcl_segmentation-gd.lib
pcl_range_image-gd.lib
pcl_visualization-gd.lib
pcl_features-gd.lib
qhullstatic_d.lib
pcl_surface-gd.lib
pcl_registration-gd.lib
pcl_keypoints-gd.lib
pcl_apps-gd.lib
vtkGraphics-gd.lib
vtkverdict-gd.lib
vtkImaging-gd.lib
vtkIO-gd.lib
vtkFiltering-gd.lib
vtkDICOMParser-gd.lib
vtkNetCDF_cxx-gd.lib
vtkmetaio-gd.lib
vtksys-gd.lib
ws2_32.lib
comctl32.lib
wsock32.lib
vtksqlite-gd.lib
vtkpng-gd.lib
vtktiff-gd.lib
vtkzlib-gd.lib
vtkjpeg-gd.lib
vtkexpat-gd.lib
vtkftgl-gd.lib
vtkfreetype-gd.lib
opengl32.lib
vtkexoIIc-gd.lib
vtkNetCDF-gd.lib
vfw32.lib



**If you build the project under RELEASE mode, add the following:**

libboost_system-vc100-mt-1_47.lib
libboost_filesystem-vc100-mt-1_47.lib
libboost_thread-vc100-mt-1_47.lib
libboost_date_time-vc100-mt-1_47.lib
libboost_iostreams-vc100-mt-1_47.lib
pcl_common.lib
pcl_octree.lib
openNI.lib
vtkCommon.lib
vtkRendering.lib
vtkHybrid.lib
pcl_io.lib
pcl_sample_consensus.lib
flann_cpp_s.lib
pcl_kdtree.lib
pcl_search.lib
pcl_filters.lib
pcl_segmentation.lib
pcl_range_image.lib
pcl_visualization.lib
pcl_features.lib
qhullstatic.lib
pcl_surface.lib
pcl_registration.lib
pcl_keypoints.lib
pcl_apps.lib
vtkGraphics.lib
vtkverdict.lib
vtkImaging.lib
vtkIO.lib
vtkFiltering.lib
vtkDICOMParser.lib
vtkNetCDF_cxx.lib
vtkmetaio.lib
vtksys.lib
ws2_32.lib
comctl32.lib
wsock32.lib
vtksqlite.lib
vtkpng.lib
vtktiff.lib
vtkzlib.lib
vtkjpeg.lib
vtkexpat.lib
vtkftgl.lib
vtkfreetype.lib
opengl32.lib
vtkexoIIc.lib
vtkNetCDF.lib
vfw32.lib



**Add following in the path:**
C:\Program Files (x86)\PCL 1.4.0\bin
C:\Program Files (x86)\PCL 1.4.0\3rdParty\flann\bin
C:\Program Files (x86)\PCL 1.4.0\3rdParty\qhull\bin



**Add following in the CPP file:**

#include <pcl/common/common_headers.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/pcl_visualizer.h>














