
# How can I plot an image (.jpg) in MATLAB in both 2-D and 3-D? - 机器学习的小学生 - CSDN博客


2017年09月16日 16:43:04[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：222个人分类：[Matlab																](https://blog.csdn.net/xuluhui123/article/category/6310922)



**For 2-D plots…**
The function IMAGE is what you’re looking for. Here’s an example:
```python
img = imread(
```
```python
'peppers.png'
```
```python
);
```
```python
%# Load a sample image
```
```python
scatter(
```
```python
rand
```
```python
(
```
```python
1
```
```python
,
```
```python
20
```
```python
)-
```
```python
0.5
```
```python
,
```
```python
rand
```
```python
(
```
```python
1
```
```python
,
```
```python
20
```
```python
)-
```
```python
0.5
```
```python
);
```
```python
%# Plot some random data
```
```python
hold on;
```
```python
%# Add to the plot
```
```python
image(
```
```python
[-
```
```python
0.1
```
```python
0.1
```
```python
]
```
```python
,
```
```python
[
```
```python
0.1
```
```python
-
```
```python
0.1
```
```python
]
```
```python
,img);
```
```python
%# Plot the image
```
![这里写图片描述](https://img-blog.csdn.net/20170916164209525?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFieV9neWw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**For 3-D plots…**
The IMAGE function is no longer appropriate, as the image will not be displayed unless the axis is viewed from directly above (i.e. from along the positive z-axis). In this case you will have to create a surface in 3-D using the SURF function and texture map the image onto it. Here’s an example:
```python
[xSphere,ySphere,zSphere] = sphere(
```
```python
16
```
```python
);          %
```
```python
# Points on a sphere
```
```python
scatter3(xSphere(:),ySphere(:),zSphere(:),
```
```python
'.'
```
```python
);  %
```
```python
# Plot the points
```
```python
axis equal;   %
```
```python
# Make the axes scales match
```
```python
hold on;      %
```
```python
# Add to the plot
```
```python
xlabel(
```
```python
'x'
```
```python
);
ylabel(
```
```python
'y'
```
```python
);
zlabel(
```
```python
'z'
```
```python
);
img = imread(
```
```python
'peppers.png'
```
```python
);     %
```
```python
# Load a sample image
```
```python
xImage = [-
```
```python
0.5
```
```python
0.5
```
```python
; -
```
```python
0.5
```
```python
0.5
```
```python
];   %
```
```python
# The x data for the image corners
```
```python
yImage = [
```
```python
0
```
```python
0
```
```python
;
```
```python
0
```
```python
0
```
```python
];             %
```
```python
# The y data for the image corners
```
```python
zImage = [
```
```python
0.5
```
```python
0.5
```
```python
; -
```
```python
0.5
```
```python
-
```
```python
0.5
```
```python
];   %
```
```python
# The z data for the image corners
```
```python
surf(xImage,yImage,zImage,
```
```python
...
```
```python
%
```
```python
# Plot the surface
```
```python
'CData'
```
```python
,img,
```
```python
...
```
```python
'FaceColor'
```
```python
,
```
```python
'texturemap'
```
```python
);
```
![这里写图片描述](https://img-blog.csdn.net/20170916164230201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcmFieV9neWw=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
Note that this surface is fixed in space, so the image will not always be directly facing the camera as you rotate the axes. If you want the texture-mapped surface to automatically rotate so that it is always perpendicular to the line of sight of the camera, it’s a much more involved process.
参考文献：
[https://stackoverflow.com/questions/3719502/how-can-i-plot-an-image-jpg-in-matlab-in-both-2-d-and-3-d](https://stackoverflow.com/questions/3719502/how-can-i-plot-an-image-jpg-in-matlab-in-both-2-d-and-3-d)

