# SVG path 标签简介 - cocoonyang的专栏 - CSDN博客





2018年07月23日 22:06:57[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：634








# Paths

Paths represent the outline of a shape which can be filled, stroked, used as a clipping path, or any combination of the three[1].

# d

d = “path data”. The definition of the outline of a shape.

Pen Command

## M(move to)
|Command|Parameters|Repeatable|Explanation|
|----|----|----|----|
|M (absolute)|x, y|Yes|将画笔移动(Move to)到点(x,y)|
|m (relative)|||Start a new sub-path at the given (x,y) coordinate.|
||||M (uppercase) indicates that absolute coordinates will follow;|
||||m (lowercase) indicates that relative coordinates will follow.|
||||If a moveto is followed by multiple pairs of coordinates,|
||||the subsequent pairs are treated as implicit lineto commands.|
||||Hence, implicit lineto commands will be relative if the moveto is relative,|
||||and absolute if the moveto is absolute.|
||||If a relative moveto (m) appears as the first element of the path,|
||||then it is treated as a pair of absolute coordinates.|
||||In this case, subsequent pairs of coordinates are treated as relative even though|
||||the initial moveto is interpreted as an absolute moveto.|

Line Commands
|Command|Parameters|Repeatable|Explanation|
|----|----|----|----|
|L ( l )|x, y|Yes|从当前点画直线段到点(x,y) Line to|
||||Draw a line from the current point to the given (x,y) coordinate|
||||which becomes the new current point.|
||||L (uppercase) indicates that absolute coordinates will follow;|
||||l (lowercase) indicates that relative coordinates will follow.|
||||A number of coordinates pairs may be specified to draw a polyline.|
||||At the end of the command, the new current point is set to|
||||the final set of coordinates provided.|
|H ( h )|x|Yes|从当前点画长度为x的水平直线段 Horizontal line to|
||||Draws a horizontal line from the current point (cpx, cpy) to (x, cpy).|
||||H (uppercase) indicates that absolute coordinates will follow;|
||||h (lowercase) indicates that relative coordinates will follow.|
||||Multiple x values can be provided (although usually this doesn’t make sense).|
||||At the end of the command, the new current point becomes (x, cpy)|
||||for the final value of x.|
|V ( v )|y|Yes|从当前点画长度为y的垂直直线段 Vertical line to|
||||Draws a vertical line from the current point (cpx, cpy) to (cpx, y).|
||||V (uppercase) indicates that absolute coordinates will follow;|
||||v (lowercase) indicates that relative coordinates will follow.|
||||Multiple y values can be provided (although usually this doesn’t make sense).|
||||At the end of the command, the new current point becomes (cpx, y) for the final value of y.|

Example:

```
<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" 
  "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg width="4cm" height="4cm" viewBox="0 0 400 400"
     xmlns="http://www.w3.org/2000/svg" version="1.1">
  <path d="M 100 100 L 300 100 L 200 300 z"
        fill="red" stroke="blue" stroke-width="3" />
</svg>
```

path标签中路径命令解析：

将画笔移动到坐标为(100, 100)的起始点。 从起始点画蓝色直线段到点(300, 100), 然后继续画蓝色直线段到点(200 , 300), 最后画蓝色直线段到起始点。 三条线段构成一个封闭多边形，并将这个多边形填充红色。
![这里写图片描述](https://img-blog.csdn.net/20180512002030644?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## Bezier Curve Commands
|Command|Parameters|Repeatable|Explanation|
|----|----|----|----|
|C ( c )|x1 y1 x2 y2 x y|Yes|Cubic Bézier curve to|
||||Draws a cubic Bézier curve from the current point to (x,y)|
||||using (x1,y1) as the control point at the beginning of the curve|
||||and (x2,y2) as the control point at the end of the curve.|
||||C (uppercase) indicates that absolute coordinates will follow;|
||||c (lowercase) indicates that relative coordinates will follow.|
||||Multiple sets of coordinates may be specified to draw a polybézier.|
||||At the end of the command, the new current point becomes|
||||the final (x,y) coordinate pair used in the polybézier.|
|S ( s )|x2 y2 x y|Yes|shorthand/smooth cubic Bézier  curve to|
||||Draws a cubic Bézier curve from the current point to (x,y).|
||||The first control point is assumed to be the reflection of|
||||the second control point on the previous command relative to|
||||the current point. (If there is no previous command or if the|
||||previous command was not an C, c, S or s, assume the first|
||||control point is coincident with the current point.) (x2,y2) is|
||||the second control point (i.e., the control point at the end of the curve).|
||||S (uppercase) indicates that absolute coordinates will follow;|
||||s (lowercase) indicates that relative coordinates will follow.|
||||Multiple sets of coordinates may be specified to draw a polybézier.|
||||At the end of the command, the new current point becomes the|
||||final (x,y) coordinate pair used in the polybézier.|
|Q ( q )|x1 y1 x y|Yes|quadratic Bézier curveto|
||||Draws a quadratic Bézier curve from the current point to (x,y)|
||||using (x1,y1) as the control point. Q (uppercase) indicates that|
||||absolute coordinates will follow; q (lowercase) indicates that relative|
||||coordinates will follow. Multiple sets of coordinates may be specified to|
||||draw a polybézier. At the end of the command, the new current point|
||||becomes the final (x,y) coordinate pair used in the polybézier.|
|T ( t )|x y|Yes|Shorthand/smooth quadratic Bézier curveto|
||||Draws a quadratic Bézier curve from the current point to (x,y).|
||||The control point is assumed to be the reflection of the control point on|
||||the previous command relative to the current point. (If there is no previous|
||||command or if the previous command was not a Q, q, T or t, assume|
||||the control point is coincident with the current point.) T (uppercase) indicates|
||||that absolute coordinates will follow;|
||||t (lowercase) indicates that relative coordinates will follow.|
||||At the end of the command, the new current point becomes the final (x,y)|
||||coordinate pair used in the polybézier.|

Example

```
<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" 
  "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd">
<svg width="12cm" height="6cm" viewBox="0 0 1200 600"
     xmlns="http://www.w3.org/2000/svg" version="1.1">
      <path id="Cubil Bezier curve path"   d="M100,200 C100,100 250,100 250,200
        S400,300 400,200" fill="none" stroke="black" stroke-width="5"  />
      <path id="Quadratic Bezier curve path" d="M 400,50 Q 600,300 1000,300"
        fill="none" stroke="red" stroke-width="5"  />
</svg>
```

![这里写图片描述](https://img-blog.csdn.net/20180512001835868?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## Elliptical Arc Curve Command
|Command|Parameters|Repeatable|Explanation|
|----|----|----|----|
|A (absolute)|rx|Yes|画一段连接当前点和点(x,y)的椭圆弧 elliptical arc|
|a (relative)|ry||椭圆的x轴向半径和y轴向半径分别由rx 和ry两个参数设定。|
||x-axis-rotation||连接当前点和点(x,y)的椭圆弧有四种路径, 通过large-arc-flag 和 sweep-flag两个参数选定。|
||large-arc-flag||具体样例详见下面图例。|
||sweep-flag||Draws an elliptical arc from the current point to (x, y).|
||x||The size and orientation of the ellipse are defined by two radii (rx, ry)|
||y||and an x-axis-rotation, which indicates how the ellipse as a whole is rotated|
||||relative to the current coordinate system.|
||||The center (cx, cy) of the ellipse is calculated automatically to satisfy the|
||||constraints imposed by the other parameters. large-arc-flag and sweep-flag|
||||contribute to the automatic calculations and help determine how the arc is drawn.|

Examples:

![这里写图片描述](https://img-blog.csdn.net/20180723215014213?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NvY29vbnlhbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## End Path Command
|Command|Parameters|Repeatable|Explanation|
|----|----|----|----|
|Z ( z )|none|No|Close path|
||||Close the current subpath by drawing a straight line from the current point to current subpath’s initial point.|
||||Since the Z and z commands take no parameters, they have an identical effect.|

> 
[1] [https://www.w3.org/TR/SVG11/paths.html](https://www.w3.org/TR/SVG11/paths.html)

[2] [https://www.dashingd3js.com/svg-paths-and-d3js](https://www.dashingd3js.com/svg-paths-and-d3js)





