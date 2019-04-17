# PCL入门文档 - 战斗蜗牛的专栏 - CSDN博客





2012年05月11日 10:18:22[vbskj](https://me.csdn.net/vbskj)阅读数：10572









### [PCL Getting Start.PCL（Point Cloud Library）入门文档整理](http://blog.csdn.net/lynnandwei/article/details/6913538)


2011-10-28 10:51914人阅读[评论](http://blog.csdn.net/lynnandwei/article/details/6913538#comments)(3)[收藏](http://blog.csdn.net/lynnandwei/article/details/6913538)[举报](http://blog.csdn.net/lynnandwei/article/details/6913538#report)


PCL Getting Start.

-------------

Translator: wei 

Date: 2011.10.11 

-------------

此文档对点云库中所包含的库进行了简单介绍，用于查阅与学习。对应的图片，请参照以下原始文档链接。


http://pointclouds.org/documentation/


The pcl_filters library contains outlier and noise removal mechanisms for 3D point cloud data filtering applications.


An example of noise removal is presented in the figure below. Due to measurement errors, certain datasets present a large number of shadow points. This complicates the estimation of local point cloud 3D features. Some of these outliers can be filtered by performing
 a statistical analysis on each point's neighborhood, and trimming those which do not meet a certain criteria. The sparse outlier removal implementation in PCL is based on the computation of the distribution of point to neighbors distances in the input dataset.
 For each point, the mean distance from it to all its neighbors is computed. By assuming that the resulted distribution is Gaussian with a mean and a standard deviation, all points whose mean distances are outside an interval defined by the global distances
 mean and standard deviation can be considered as outliers and trimmed from the dataset.


pcl_filters库包含针对三维点云数据滤波应用的噪声去除机制。


下图中的噪声消除的一个例子。由于测量误差，数据集中会有大量的阴影点。这使得3D点云特征估计变得复杂。有些离群点可以通过对每个点的邻居进行了统计分析，并修剪那些不符合一定标准的点进行筛选。在PCL中，稀疏离群点的计算是基于输入数据集中点到邻居的距离的分布的。对于每一个点，计算它到所有邻居的平均距离。假设这是一个均值和标准差的高斯分布，每个中值距离大于全局平均距离均值和标准差的点，可以从数据集离群和修剪


The pcl_features library contains data structures and mechanisms for 3D feature estimation from point cloud data. 3D features are representations at a certain 3D point or position in space, which describe geometrical patterns based on the information available
 around the point. The data space selected around the query point is usually referred as the k-neighborhood.


The following figure shows a simple example of a selected query point, and its selected k-neighborhood.

features_normal.png


An example of two of the most widely used geometric point features are the underlying surface's estimated curvature and normal at a query point p. Both of them are considered local features, as they characterize a point using the information provided by its
 k closest point neighbors. For determining these neighbors efficienctly, the input dataset is usually split into smaller chunks using spatial decomposition techniques such as octrees or kD-trees (see the figure below - left: kD-tree, right: octree), and then
 closest point searches are performed in that space. Depending on the application one can opt for either determining a fixed number of k points in the vecinity of p, or all points which are found inside of a sphere of radius r centered at p. Unarguably, one
 the easiest methods for estimating the surface normals and curvature changes at a point p is to perform an eigendecomposition (i.e. compute the eigenvectors and eigenvalues) of the k-neighborhood point surface patch. Thus, the eigenvector corresponding to
 the smallest eigenvalue will approximate the surface normal n at point p, while the surface curvature change will be estimated from the eigenvalues as:

$\frac{\lambda_0}{\lambda_0 + \lambda_1 + \lambda_2}$, where $\lambda_0 < \lambda_1 < \lambda_2$.

features_bunny.png


pcl_features库包含了从点云数据的三维特征估计的数据结构和机制。 3D特征通过三维点或空间位置来描述，可以根据该点周围的有效信息来描述相应的几何图案。查询点周围的数据空间通常被称为K -邻域。


下图显示了一个简单的例子，一个选定的查询点和其选定的K -邻域。

features_normal.png


这个例子给出了两个使用最广泛的几何点特征：表面的曲面估计和一般的确定点p，他们都被认为是本地特征，因为它们通过其K最接近点的邻居提供的信息被认为是一个点的特征。为了有效的确定这些邻居，这些输入数据集通常是使用空间分解方法，如八叉树或KD -树（见下图 - 左：KD -树中，右键：八叉树）分割成较小的块，然后这个空间中最近点开始搜索。根据不同的应用，可以选择或者确定的P vecinity，或在第为中心的半径为r的领域内发现的所有的点固定数量的k点无可争议的，估计在P点是执行一个特征分解的K -邻域点表面修补（即特征向量和特征值计算）的表面法线和曲率变化的最简单的方法。因此，特征值对应的最小特征值将接近正常的N，P点的表面表面曲率的变化，同时将作为特征值估计：

$\frac{\lambda_0}{\lambda_0 + \lambda_1 + \lambda_2}$, where $\lambda_0 < \lambda_1 < \lambda_2$.


The pcl_keypoints library contains implementations of two point cloud keypoint detection algorithms. Keypoints (also referred to as interest points) are points in an image or point cloud that are stable, distinctive, and can be identified using a well-defined
 detection criterion. Typically, the number of interest points in a point cloud will be much smaller than the total number of points in the cloud, and when used in combination with local feature descriptors at each keypoint, the keypoints and descriptors can
 be used to form a compact—yet descriptive—representation of the original data.


pcl_keypoints库包含两个点云的关键点检测算法的实现。关键点（兴趣点“也被称为）是在图像或点云中是稳定，有特色，可以用一个定义良好的检测标准来确定的。通常情况下，点云中的兴趣点数量比云中的点的总数少得多，配合每个关键点的局部特征描述符使用时，关键点和描述符可以用来对原始数据进行紧凑并具有代表性的描述。


Combining several datasets into a global consistent model is usually performed using a technique called registration. The key idea is to identify corresponding points between the data sets and find a transformation that minimizes the distance (alignment error)
 between corresponding points. This process is repeated, since correspondence search is affected by the relative position and orientation of the data sets. Once the alignment errors fall below a given threshold, the registration is said to be complete.


The pcl_registration library implements a plethora of point cloud registration algorithms for both organized an unorganized (general purpose) datasets


将几个数据集融合成为一个全局一致的模型通常使用的一种技术称为注册。关键的思想是，确定数据集之间的对应点，找到一个转换方法最大限度地使得对应点之间的距离最小（对准误差）。重复这一过程，因为对应点的搜索受到数据集的相对位置和方向的影响。一旦对齐误差低于一个给定的的阈值范围，就称为完成注册。


pcl_registration库实现了大量针对有组织的和无组织（通用）数据集的点云注册算法。


The pcl_kdtree library provides the kd-tree data-structure, using FLANN, that allows for fast nearest neighbor searches.


A Kd-tree (k-dimensional tree) is a space-partitioning data structure that stores a set of k-dimensional points in a tree structure that enables efficient range searches and nearest neighbor searches. Nearest neighbor searches are a core operation when working
 with point cloud data and can be used to find correspondences between groups of points or feature descriptors or to define the local neighborhood around a point or points.


pcl_kdtree库提供KD -树的数据结构，使用FLANN，允许快速最近的邻居搜索。


一个KD -树（k维树）是一个空间分区中的数据结构存储在一个树状结构，使有效的范围搜索和近邻搜索的K -维点。点云数据工作时的核心操作是最近邻搜索，可以用来寻找点群或者特征描述符之间的关联，或围绕一个点或多个点来定义的它的局部邻居点。


The pcl_octree library provides efficient methods for creating a hierarchical tree data structure from point cloud data. This enables spatial partitioning, downsampling and search operations on the point data set. Each octree node the has either eight children
 or no children. The root node describes a cubic bounding box which encapsulates all points. At every tree level, this space becomes subdivided by a factor of 2 which results in an increased voxel resolution.


The pcl_octree implementation provides efficient nearest neighbor search routines, such as "Neighbors within Voxel Search”, “K Nearest Neighbor Search” and “Neighbors within Radius Search”. It automatically adjusts its dimension to the point data set. A set
 of leaf node classes provide additional functionality, such as spacial "occupancy" and "point density per voxel" checks. Functions for serialization and deserialization enable to efficiently encode the octree structure into a binary format. Furthermore, a
 memory pool implementation reduces expensive memory allocation and deallocation operations in scenarios where octrees needs to be created at high rate.


The following figure illustrates the voxel bounding boxes of an octree nodes at lowest tree level. The octree voxels are surrounding every 3D point from the bunny's surface. The red dots represent the point data. This image is create with the octree_viewer
 (visualization/tools/octree_viewer).


For examples how to use the pcl_octree library, please visit the pcl tutorial page.


pcl_octree库提供了有效的方法，创建一个层次结构树中的数据点云数据的结构。这使得在点数据集空间的分区，下采样和搜查行动。每个八叉树节点有八个子女或无子女。根节点，描述了一个封装了所有的点的立方体盒子。在树的每一层，分为四个部分，体素的分解度会增加。


pcl_octree的实现提供高效的最近邻搜索程序，如“Voxel邻居搜索”，“K近邻搜索”和“半径内邻居搜索”，它会根据点的数据集自动调整它的维数。它额外提供一个叶节点类的设置，如空间上的“占用”和“voxel点密度检查”。序列化和反序列化的功能能够有效地将八叉树结构​​编码成二进制格式，此外，内存池的实现在高效的建立八叉树时能降低内存分配和释放的代价。


下图说明了在最低树级别的八叉树节点的voxel的边界框。八叉树voxel是由兔子的表面每一个三维点组成。红点代表数据点。这个图像是由octree_viewer所创建（可视化/工具/ octree_viewer）。


对于如何使用pcl_octree库的例子，请访问PCL教程页面。



The pcl_segmentation library contains algorithms for segmenting a point cloud into distinct clusters. These algorithms are best suited to processing a point cloud that is composed of a number of spatially isolated regions. In such cases, clustering is often
 used to break the cloud down into its constituent parts, which can then be processed independently.

pcl_segmentation库包含点云分割成不同的簇（集群）的算法。这些算法最适合于处理空间由多个相对独立区域组成的一个点云。在这种情况下，集群通常用于分解成点云到的点云组成部分，这样就可以独立处理。



The pcl_sample_consensus library holds SAmple Consensus (SAC) methods like RANSAC and models like planes and cylinders. These can combined freely in order to detect specific models and their paramters in point clouds.


Some of the models implemented in this library include: lines, planes, cylinders, and spheres. Plane fitting is often applied to the task of detecting common indoor surfaces, such as walls, floors, and table tops. Other models can be used to detect and segment
 objects with common geometric structures (e.g., fitting a cylinder model to a mug).


pcl_sample_consensus库保存SAmple Consensus (SAC) 方法如 RANSAC和平面和圆柱模型等。这些可以自由组合来检测点云中特定的模型和它们的外观。


在这个库中实现的模型包括：线，面，柱面，和平面。平面拟合常常用于检测常见的室内表面，如墙壁，地面，桌面。其他模型可用于检测和段与常见的几何结构（如一个圆柱体模型拟合一大杯）对象。

    * SACMODEL_PLANE - used to determine plane models. The four coefficients of the plane are its Hessian Normal form: [normal_x normal_y normal_z d]

    * SACMODEL_LINE - used to determine line models. The six coefficients of the line are given by a point on the line and the direction of the line as: [point_on_line.x point_on_line.y point_on_line.z line_direction.x line_direction.y line_direction.z]

    * SACMODEL_CIRCLE2D - used to determine 2D circles in a plane. The circle's three coefficients are given by its center and radius as: [center.x center.y radius]

    * SACMODEL_CIRCLE3D - not implemented yet

    * SACMODEL_SPHERE - used to determine sphere models. The four coefficients of the sphere are given by its 3D center and radius as: [center.x center.y center.z radius]

    * SACMODEL_CYLINDER - used to determine cylinder models. The seven coefficients of the cylinder are given by a point on its axis, the axis direction, and a radius, as: [point_on_axis.x point_on_axis.y point_on_axis.z axis_direction.x axis_direction.y axis_direction.z
 radius]

    * SACMODEL_CONE - not implemented yet

    * SACMODEL_TORUS - not implemented yet

    * SACMODEL_PARALLEL_LINE - a model for determining a line parallel with a given axis, within a maximum specified angular deviation. The line coefficients are similar to SACMODEL_LINE.

    * SACMODEL_PERPENDICULAR_PLANE - a model for determining a plane perpendicular to an user-specified axis, within a maximum specified angular deviation. The plane coefficients are similar to SACMODEL_PLANE.

    * SACMODEL_PARALLEL_LINES - not implemented yet

    * SACMODEL_NORMAL_PLANE - a model for determining plane models using an additional constraint: the surface normals at each inlier point has to be parallel to the surface normal of the output plane, within a maximum specified angular deviation. The plane
 coefficients are similar to SACMODEL_PLANE.

    * SACMODEL_PARALLEL_PLANE - a model for determining a plane parallel to an user-specified axis, within a maximim specified angular deviation. SACMODEL_PLANE.

    * SACMODEL_NORMAL_PARALLEL_PLANE defines a model for 3D plane segmentation using additional surface normal constraints. The plane must lie parallel to a user-specified axis. SACMODEL_NORMAL_PARALLEL_PLANE therefore is equivallent to SACMODEL_NORMAL_PLANE
 + SACMODEL_PARALLEL_PLANE. The plane coefficients are similar to SACMODEL_PLANE.


The following list describes the robust sample consensus estimators implemented:


    * SAC_RANSAC - RANdom SAmple Consensus

    * SAC_LMEDS - Least Median of Squares

    * SAC_MSAC - M-Estimator SAmple Consensus

    * SAC_RRANSAC - Randomized RANSAC

    * SAC_RMSAC - Randomized MSAC

    * SAC_MLESAC - Maximum LikeLihood Estimation SAmple Consensus

    * SAC_PROSAC - PROgressive SAmple Consensus



* SACMODEL_PLANE - 用于确定平面模型。平面的四个系数是Hessian的正常形态：[normal_x normal_y normal_z D]

    * SACMODEL_LINE - 用于确定线模型。该行的6个系数是由一个点上线，并线的方向：[point_on_line.x point_on_line.y point_on_line.z line_direction.x line_direction.y line_direction.z]

    * SACMODEL_CIRCLE2D - 用于确定一个2d圆。其圆心和半径的圆的三个系数：[center.x center.y半径]

    * SACMODEL_CIRCLE3D - 尚未实现

    * SACMODEL_SPHERE - 用于确定球体。球体的四个系数由3D的圆心和半径为：[center.x center.y center.z半径]

    * SACMODEL_CYLINDER - 用于确定圆柱。圆柱模型的七个系数由点上的轴，轴的方向，半径，因为：[point_on_axis.x point_on_axis.y point_on_axis.z axis_direction.x axis_direction.y axis_direction.z半径]

    * SACMODEL_CONE - 尚未实现

    * SACMODEL_TORUS - 尚未实现

    * SACMODEL_PARALLEL_LINE - 确定一个给定轴的平行线的模型，在一个指定的最大角偏差。该生产线的系数类似SACMODEL_LINE。

    * SACMODEL_PERPENDICULAR_PLANE - 为确定用户指定的轴垂直的平面内一个指定的最大角偏差，的模式。平面系数类似SACMODEL_PLANE。

    * * * * * SACMODEL_PARALLEL_LINES - 尚未实现

    * SACMODEL_NORMAL_PLANE - 一个使用一个额外的约束来确定平面：在每个内点的表面法线在指定的最大角偏差，表面正常的输出平面平行。平面系数类似SACMODEL_PLANE。

    * SACMODEL_PARALLEL_PLANE - 一个maximim指定的角偏差，到用户指定的轴范围内确定一个平面平行的典范。 SACMODEL_PLANE。


    * SACMODEL_NORMAL_PARALLEL_PLANE 使用额外的一般平面约束来分割3D平面。平面必须平行与用户指定的轴。SACMODEL_NORMAL_PARALLEL_PLANE因此是equivallent SACMODEL_NORMAL_PLANE + SACMODEL_PARALLEL_PLANE。平面系数类似SACMODEL_PLANE。


The pcl_surface library deals with reconstructing the original surfaces from 3D scans. Depending on the task at hand, this can be for example the hull, a mesh representation or a smoothed/resampled surface with normals.


Smoothing and resampling can be important if the cloud is noisy, or if it is composed of multiple scans that are not aligned perfectly. The complexity of the surface estimation can be adjusted, and normals can be estimated in the same step if needed.

resampling_1.png


Meshing is a general way to create a surface out of points, and currently there are two algorithms provided: a very fast triangulation of the original points, and a slower meshing that does smoothing and hole filling as well.

surface_meshing.png


Creating a convex or concave hull is useful for example when there is a need for a simplified surface representation or when boundaries need to be extracted.

surface_hull.png



pcl_surface库处理从三维扫描重建原始表面。根据手头上的任务不同，可以采用外壳，网格表示，或表面法线的平滑/重采样。


如果云是带噪声的，或者如果它是完全不对齐的多个扫描线组成，平滑和重采样就非常重要。可调节表面复杂度的估计，法线也可以在相同的步骤估计，如果需要的话。

resampling_1.png


网格是一个从点来创建面的通用方法，目前有两种算法：一个非常快的原始点的三角，和较慢的啮合，以及平滑和孔填充。

surface_meshing.png


当有一个简化的表面陈述或边界需要被提取时，创建一个凸或凹外壳是非常必要的。

surface_hull.png


更多信息，请访问http://www.pointclouds.org的教程。

The pcl_range_image library contains two classes for representing and working with range images. A range image (or depth map) is an image whose pixel values represent a distance or depth from the sensor's origin. Range images are a common 3D representation
 and are often generated by stereo or time-of-flight cameras. With knowledge of the camera's intrinsic calibration parameters, a range image can be converted into a point cloud

pcl_range_image库包含两类可表示和使用的深度图像。一个深度图是其像素值代表从传感器开始的距离或深度。深度图像，是一种常见的3D表示，常常通过立体相机和TOF相机生成。通过知道相机的内部标定参数，深度图像可以被转换成一个点云




The pcl_io library contains classes and functions for reading and writing point cloud data and capturing from OpenNI-compatible depth cameras. An introduction to some of these capabilities can be found in the following tutorials.


    * The PCD (Point Cloud Data) file format

    * Reading PointCloud data from PCD files

    * Writing PointCloud data to PCD files

    * The OpenNI Grabber Framework in PCL


pcl_io库包含了点云数据读写的类和函数，并可以通过OpenNI深度兼容相机来捕捉数据。可以在以下的教程进行了解。


    * PCD（点云数据）的文件格式

    *从PCD的文件读点云数据

    *写点云数据的PCD文件

    * OpenNI PCL采集卡框架

The pcl_visualization library was built for the purpose of being able to quickly prototype and visualize the results of algorithms operating on 3D point cloud data. Similar to OpenCV's highgui routines for displaying 2D images and for drawing basic 2D shapes
 on screen, the library offers:


    * methods for rendering and setting visual properties (colors, point sizes, opacity, etc) for any n-D point cloud datasets in pcl::PointCloud<T> format;

      bunny.jpg

    * methods for drawing basic 3D shapes on screen (e.g., cylinders, spheres,lines, polygons, etc) either from sets of points or from parametric equations;

      shapes.jpg

    * a histogram visualization module (PCLHistogramVisualizer) for 2D plots;

      histogram.jpg

    * a multitude of Geometry and Color handler for pcl::PointCloud<T> datasets;

      normals.jpg

      pcs.jpg

    * a pcl::RangeImage visualization module.

      range_image.jpg


The package makes use of the VTK library for 3D rendering and wxwidgets for range image and 2D operations.


For implementing your own visualizers, take a look at the tests and examples accompanying the library.

pcl_visualization库是建立三维点云数据操作后快速原型化和可视化的结果。 和OpenCV中的highgui例程显示2D图像，并在屏幕上绘制的基本的2D形状类似，该库提供了：


    *渲染，并设置为PCL任何n-D 点云数据集的可视属性（颜色，点的大小，不透明度等）：pcl::PointCloud<T> format;

      bunny.jpg

    *从点集或参数方程绘制基本的3D形状（例如，圆柱体，球体，线，多边形等）;

      shapes.jpg

    *2D绘图直方图的可视化模块（PCLHistogramVisualizer）;

      histogram.jpg

    *多种几何形状和颜色处理程序pcl::PointCloud<T> datasets;

      normals.jpg

      pcs.jpg

    * pcl::RangeImage 的可视化模块。

      range_image.jpg


这个包使用了VTK库的3D渲染和wxWidgets的深度图像和2D操作。


为实现自己的可视化效果，看看在测试和库所附的例子。



