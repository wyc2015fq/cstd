# Intersection-over-union between two detections - 家家的专栏 - CSDN博客





2016年03月11日 09:56:27[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2191









1) You have two overlapping bounding boxes. You compute the intersection of the boxes, which is the area of the overlap. You compute the union of the overlapping boxes, which is the sum of the areas of the entire boxes minus the area of the overlap. Then you
 divide the intersection by the union. There is a function for that in the Computer Vision System Toolbox called [**bboxOverlapRatio**](http://www.mathworks.com/help/vision/ref/bboxoverlapratio.html).


2) Generally, you don't want to concatenate the color channels. What you want instead, is a 3D histogram, where the dimensions are H, S, and V.





转自：http://stackoverflow.com/questions/28723670/intersection-over-union-between-two-detections



