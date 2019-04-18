# 【Python】点集凸包Numpy实现（补充版） - YZXnuaa的博客 - CSDN博客
2018年03月23日 19:52:20[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：953
## 1. 矢量减法
设二维矢量 11
 ，22
则矢量减法定义（对应位相减）为： 1212
显然有性质 
如不加说明，下面所有的**点**都看作**矢量**，两点的减法就是矢量相减；
```python
def sub(p, q):
    return p-q
p, q = np.array([1, 2]), np.array([3, 5])
print(sub(p, q))
print(-sub(q, p))
```
- 1
- 2
- 3
- 4
- 5
## 2. 矢量叉积
设矢量1122
（本文以二维为例，可自然推广到三维） 
则矢量叉积定义为： 1221   得到的是一个标量 
显然有性质 
如不加说明，下面所有的点都看作矢量，点的乘法看作矢量叉积；
叉乘的重要性质：
> 
**若 P × Q > 0 , 则P 在Q的顺时针方向   若 P × Q < 0 , 则P 在Q的逆时针方向**（与性质1等价） 
**若 P × Q = 0 , 则P 与Q共线，但可能同向也可能反向**
```python
def cross_prod(p, q):
    assert len(p)==2 and len(q)==2
    return p[0]*q[1]-p[1]*q[0]
p, q = np.array([1, 0]), np.array([0, 1])
                        # p在q的顺时针方向
print(cross_prod(p, q))
                        # 1>0
p, q = np.array([1, 2], [2, 4])         
                        # p, q 同向
print(cross_prod(p, q))
                        # 0
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
## 3. 判断点在线段上
设点为
，线段为12 ，判断点在该线段上的依据是： 
1210 且  在以 12 为对角顶点的矩形内； 
这是叉积性质的直接应用，也即1210，则 1与21
共线；
```python
def is_point_on_line(q, p1, p2):
    return True if cross_prod(q-p1, p2-p1)==0 else False
p1, p2 = np.array([1, 0]), np.array([0, 1])
q = np.array([1/2, 1/2])
print(is_point_on_line(q, p1, p2))
                    # True
q = np.array([1/2, 1/3])
print(is_point_on_line(q, p1, p2))
                    # False
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
## 4. 判断两线段是否相交
我们分两步确定两条线段是否相交：
- 
快速排斥试验
设以线段 12
 为对角线的矩形为， 设以线段 12 为对角线的矩形为，如果和- 
不相交，显然两线段不会相交；
```python
def rect_overlap(r1, r2):
    return not (((r1[1][0] < r2[0][0])|(r1[0][1] > r2[1][1]))
               |((r2[1][0] < r1[0][0])|(r2[0][1] > r1[1][1]))
```
- 1
- 2
- 3
- 
跨立试验
如果两线段相交，则两线段必然相互跨立对方，如图1所示。在图1中，12
跨立12 ，则   矢量 11 和21位于矢量21 的两侧，即112121210- 
 上式可改写成 ( P1 - Q1 ) × ( Q2 - Q1 ) * ( Q2 - Q1 ) × ( P2 - Q1 ) > 0， 
当 ( P1 - Q1 ) × ( Q2 - Q1 ) = 0 时，说明   ( P1 - Q1 ) 和 ( Q2 - Q1 )共线，但是因为已经通过快速排斥试验，所以 P1 一定在线段 Q1Q2上；同理，( Q2 - Q1 ) ×(P2 - Q1 ) = 0 说明 P2 一定在线段 Q1Q2上。   所以判断P1P2跨立Q1Q2的依据是：  ( P1 - Q1 ) × ( Q2 - Q1 ) * ( Q2 - Q1 ) × ( P2 - Q1 ) ≥ 0 同理判断Q1Q2跨立P1P2的依据是：  ( Q1 - P1 ) × ( P2 - P1 ) * ( P2 - P1 ) × ( Q2 - P1 ) ≥ 0至此已经完全解决判断线段是否相交的问题。
```python
def line_intersection(p1, p2, q1, q2):
    return False if not rect_overlap([p1, p2], [q1, q2]) else \             # 不相交直接返回
            (True if cross_prod(p1-q1, q2-q1)*cross_prod(q2-q1, p2-q1) >= 0 else False)
```
- 1
- 2
- 3
## 5. 判断线段和直线是否相交
从标题即可看出，这里的直线和线段是不相同的； 
如果线段 12
和直线 12相交，则 12跨立 12
，根据4的结论： 
11212121\beq0
## 6. 判断点是否在矩形内
只需判断该点的横纵坐标是否夹在矩形的左右边，上下边之间。 
判断线段、折线、多边形是否在矩形中，因为**矩形是个凸集**，所以只要判断所有端点是否都在矩形中即可；
## 7. 判断矩形是否在矩形中
只要把比较左右边界和上下边界即可；
## 8. 判断圆是否在矩形中：
圆在矩形内的**充要条件**： 
圆心在矩形中且圆的半径小于等于圆心到矩形四条边的距离的最小值。
```python
def circle_in_rect(c, r, lb, rt):
                # c: center
                # r: radius
                # lb: left bottom
                # rt: right top
    if ( ((center[0]<lb[0]) | (center[0]>rt[0])) | ((center[1]<lb[1])|(center[1]>rt[1])) ):
                # 圆心是否在矩形内
        return False
    return True r <= min(center[0]-lb[0], rt[0]-center[0], center[1]-lb[1], rt[1]-center[
```
