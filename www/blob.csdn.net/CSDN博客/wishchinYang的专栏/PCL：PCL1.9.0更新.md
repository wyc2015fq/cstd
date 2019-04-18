# PCL：PCL1.9.0更新 - wishchinYang的专栏 - CSDN博客
2019年03月05日 11:07:02[wishchin](https://me.csdn.net/wishchin)阅读数：237
PCL1.9.0的开源安装包已经发布一段时间，重要的更新有，下载页.
下载：[https://github.com/PointCloudLibrary/pcl/releases](https://github.com/PointCloudLibrary/pcl/releases)
### `New Features:`
*Newly added functionalities.*
- **[common][visualization]** Add Viridis color LUT [[#2420]](https://github.com/PointCloudLibrary/pcl/pull/2420)
- **[octree]** Implementation of the iterator 'OctreeLeafNodeBreadthIterator'. [[#2204]](https://github.com/PointCloudLibrary/pcl/pull/2204)
- **[octree]** Implementation of the iterator 'OctreeFixedDepthIterator'. [[#1983]](https://github.com/PointCloudLibrary/pcl/pull/1983)
- **[ci]** Enable Global Tests on Windows CI [[#2137]](https://github.com/PointCloudLibrary/pcl/pull/2137)
- **[features]** Add GASD global point cloud descriptor [[#1652]](https://github.com/PointCloudLibrary/pcl/pull/1652)
- **[visualization]** Add overload to `PCLVisualizer::addText3D()` that allows specifying text orientation [[#2038]](https://github.com/PointCloudLibrary/pcl/pull/2038)
- **[features]** FLARELocalReferenceFrameEstimation class added [[#1571]](https://github.com/PointCloudLibrary/pcl/pull/1571)
- **[surface][tools]** Add new mls projection method. Deprecated `MovingLeastSquares::setPolynomialFit()`. [[#1960]](https://github.com/PointCloudLibrary/pcl/pull/1960)
### `Deprecated:`
*Deprecated code scheduled to be removed after two minor releases.*
- **[octree]** Implementation of the iterator 'OctreeLeafNodeBreadthIterator'. [[#2204]](https://github.com/PointCloudLibrary/pcl/pull/2204)
- **[common][segmentation]** Provide proper EuclideanClusterComparator method depreciation. New Pragma macro. New Deprecated type. [[#2096]](https://github.com/PointCloudLibrary/pcl/pull/2096)
- **[io]** Add support pcl::PointXYZRGBA to pcl::VLPGrabber. Deprecate rgb signatures. [[#2102]](https://github.com/PointCloudLibrary/pcl/pull/2102)
- **[surface][tools]** Add new mls projection method. Deprecated `MovingLeastSquares::setPolynomialFit()`. [[#1960]](https://github.com/PointCloudLibrary/pcl/pull/1960)
### `Removed:`
*Removal of deprecated code.*
- **[filters][io][surface][visualization]** Removal of deprecated code in filters, io, surface and visualization modules [[#2077]](https://github.com/PointCloudLibrary/pcl/pull/2077)
- **[common]** Remove deprecated ros headers [[#2075]](https://github.com/PointCloudLibrary/pcl/pull/2075)
- **[registration]** Remove registration module deprecated methods [[#2076]](https://github.com/PointCloudLibrary/pcl/pull/2076)
- **[sample_consensus]** Remove deprecated functions and variables from SAC module [[#2071]](https://github.com/PointCloudLibrary/pcl/pull/2071)
- **[common]** Removal of PCA deprecated constructor [[#2070]](https://github.com/PointCloudLibrary/pcl/pull/2070)
### `Behavioral changes:`
*Changes in the expected default behavior.*
- **[common]** PointCloudDepthAndRGBtoXYZRGBA: initialize with the default alpha value (fix [#2476](https://github.com/PointCloudLibrary/pcl/issues/2476)) [[#2533]](https://github.com/PointCloudLibrary/pcl/pull/2533)
- **[octree]** Reverse octree's depth first iterator order [[#2332]](https://github.com/PointCloudLibrary/pcl/pull/2332)
- **[common]**`PointXYZRGBL``label` field is now default constructed to 0 [[#2462]](https://github.com/PointCloudLibrary/pcl/pull/2462)
- **[io]** Fix PLYReader is_dense behavior [[#2133]](https://github.com/PointCloudLibrary/pcl/pull/2133)
### `API changes:`
*Changes to the API which didn't went through the proper deprecation and removal cycle.*
- **[octree]** Implementation of the iterator 'OctreeLeafNodeBreadthIterator'. [[#2204]](https://github.com/PointCloudLibrary/pcl/pull/2204)
- **[sample_consensus]** Const-qualify most of the methods in SAC model classes [[#2270]](https://github.com/PointCloudLibrary/pcl/pull/2270)
- **[simulation]** Use GLuint rather than size_t to represent OpenGL indices. [[#2238]](https://github.com/PointCloudLibrary/pcl/pull/2238)
- **[visualization]** Fix access specifier in `PointCloudColorHandlerRGBAField`[[#2226]](https://github.com/PointCloudLibrary/pcl/pull/2226)
- **[docs]** Misc. typos (cont.) [[#2215]](https://github.com/PointCloudLibrary/pcl/pull/2215)
- **[octree]** OctreeIterators special member revision [[#2108]](https://github.com/PointCloudLibrary/pcl/pull/2108)
- **[io]** Add support pcl::PointXYZRGBA to pcl::VLPGrabber. Deprecate rgb signatures. [[#2102]](https://github.com/PointCloudLibrary/pcl/pull/2102)
- **[surface][tools]** Add new mls projection method. Deprecated `MovingLeastSquares::setPolynomialFit()`. [[#1960]](https://github.com/PointCloudLibrary/pcl/pull/1960)
- **[surface]** Add ability to cache mls results [[#1952]](https://github.com/PointCloudLibrary/pcl/pull/1952)
### `ABI changes:`
*Changes that cause ABI incompatibility but are still API compatible.*
- **[surface]** Missing pcl::MovingLeastSquaresOMP declaration without /openmp [[#2324]](https://github.com/PointCloudLibrary/pcl/pull/2324)
- **[common][filters][surface]** Improved docstrings and error messages [[#2300]](https://github.com/PointCloudLibrary/pcl/pull/2300)
- **[common]** Modified `GlasbeyLUT` indexing type to `size_t`[[#2297]](https://github.com/PointCloudLibrary/pcl/pull/2297)
- **[octree]** Implementation of the iterator 'OctreeFixedDepthIterator'. [[#1983]](https://github.com/PointCloudLibrary/pcl/pull/1983)
- **[common][segmentation]** Provide proper EuclideanClusterComparator method depreciation. New Pragma macro. New Deprecated type. [[#2096]](https://github.com/PointCloudLibrary/pcl/pull/2096)
- **[gpu]** Allow specifying decimation step in convertToTsdfCloud [[#2099]](https://github.com/PointCloudLibrary/pcl/pull/2099)
- **[apps]** More warning suppression in pcl apps [[#2080]](https://github.com/PointCloudLibrary/pcl/pull/2080)
- **[io]** Removed unused member from ply_parser [[#2066]](https://github.com/PointCloudLibrary/pcl/pull/2066)
- **[filters]** Fixes remove_indices in UniformSampling [[#1902]](https://github.com/PointCloudLibrary/pcl/pull/1902)
- **[visualization]** Add accessor for current rendering framerate in PCLVisualizer [[#1974]](https://github.com/PointCloudLibrary/pcl/pull/1974)
- **[simulation]** Redo: Simulation: enable returning of organized point clouds [[#1687]](https://github.com/PointCloudLibrary/pcl/pull/1687)
- **[registration]** Added option to specify translation and rotation convergence deltas in ICP and NDT algorithms. [[#1724]](https://github.com/PointCloudLibrary/pcl/pull/1724)
### `Modules:`
`Uncategorized:`
- Change Log generation tool..........................................................
