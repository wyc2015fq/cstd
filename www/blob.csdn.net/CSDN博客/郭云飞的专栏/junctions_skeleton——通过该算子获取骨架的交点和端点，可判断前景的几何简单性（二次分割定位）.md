
# junctions_skeleton——通过该算子获取骨架的交点和端点，可判断前景的几何简单性（二次分割定位） - 郭云飞的专栏 - CSDN博客


2014年06月09日 14:23:00[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：1266


junctions_skeleton(Operator)Name
junctions_skeleton— Find junctions and end points in a skeleton.
Signature
junctions_skeleton(Region:EndPoints,JuncPoints: : )
Description
junctions_skeletondetects junctions and end points in a skeleton (seeskeleton). The junctions in the
 input regionRegionare output as a region inJuncPoints,
 while the end points are output as a region inEndPoints.
To obtain reasonable results withjunctions_skeletonthe input regionRegionmust
 not contain lines which are more than one pixel wide. Regions obtained byskeletonmeet this condition, while
 regions obtained bymorph_skeletondo not meet this condition in general.
Parallelization
Multithreading type: reentrant (runs in parallel with non-exclusive operators).
Multithreading scope: global (may be called from any thread).
Automatically parallelized on tuple level.
Parameters
Region(input_object)
  region(-array)→object
Input skeletons.
EndPoints(output_object)  region(-array)→object
Extracted end points.
Number of elements:EndPoints == Region
JuncPoints(output_object)  region(-array)→object
Extracted junctions.
Number of elements:JuncPoints == Region
Example
 (HDevelop)
* non-connected branches of a skeletonskeleton(Region,Skeleton)junctions_skeleton(Skeleton,EPoints,JPoints)difference(S,JPoints,Rows)set_system('heighbourhood',4)connection(Rows,Parts)

