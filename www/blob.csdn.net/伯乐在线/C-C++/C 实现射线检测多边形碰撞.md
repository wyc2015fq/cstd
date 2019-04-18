# C 实现射线检测多边形碰撞 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [scottcgi](http://www.jobbole.com/members/scottcgi) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
以前，使用旋转分离轴实现过， 矩形旋转碰撞,OBB方向包围盒算法实现 。但这个算法，本身有点复杂，并且在边越多的时候计算量增长的会很快，扩展到3D层面会更加的复杂。而且这个算法碰撞后获取碰撞点的坐标有点繁琐。射线检测算法，是一个比较简单清晰的思路，实现起来复杂度也不高，碰撞点也容易获得，扩展到3D世界依然有效。
要用射线去检测碰撞，之前我们先从一个点开始。如果能够判断一个点是否和多边形碰撞，那么就可以轻易的扩展到，线和多边形，多边形和多边形的碰撞。点与多边形的碰撞是基于这个实现，[PNPOLY – Point Inclusion in Polygon Test](https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html#3D%20Polygons)。
其核心的思路是，判断这个点，和多边形每条边的位置关系。在一个多条边围成的区域，点在一条边的右侧，这个点可能在多边形内部，也可能在外部。但是如果判断完点和每一条边的左右关系，如果在右边的边是奇数个，那么点就在内部，如果是偶数，那么点就在外部。通过这个规则，就可以判断，点和多边形的碰撞关系。有两个注意点，多边行必须是凸多边形，并且如果点落在边上，我们算在左边，这样落在边上是算在内部。
那么，如果判断一个点和一条边的位置关系。这里需要用到一个向量叉积公式。比如，点(x, y)，与线 (x1, y1) (x2, y2) 的位置关系。我们先求出两个向量，(x – x1, y – y1) 和 (x2 – x1, y2 – y1)。对这两个向量做叉积的结果是 (x – x1) * (y2 – y1) – (y – y1) * (x2 – x1)， 如果结果是0，那么点在线上。如果结果大于0，点在线的左边。如果结果小于0，点在线的右边。利用这个公式，我们就能判断点是否在多边形的内部还是外部。代码如下：


```
/** 
 * Test polygon contains point, true inside or false outside 
 * one vertex contains pair of x, y 
 */  
static bool TestPolygonPoint(Array(float)* vertexArr, float x, float y)  
{  
    int    preIndex   = vertexArr->length - 2;  
    bool   inside     = false;  
    float* vertexData = AArray_GetData(vertexArr, float);  
  
    for (int i = 0; i < vertexArr->length; i += 2)  
    {  
        float vertexY = vertexData[i + 1];  
        float preY    = vertexData[preIndex + 1];  
  
        if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))  
        {  
            float vertexX = vertexData[i];  
  
            // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)  
            // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)  
            // if result zero means point (x, y) on vector (preX - vertexX, preY - vertexY)  
            // if result positive means point on left  vector  
            // if result negative means point on right vector  
            if (vertexX + (y - vertexY) / (preY - vertexY) * (vertexData[preIndex] - vertexX) <= x)  
            {  
                inside = !inside;  
            }  
        }  
  
        preIndex = i;  
    }  
  
    return inside;  
}
```
在能判断，点在多边形之后，我们就能够判断，线与多边形，多边形与多边形的关系。就是判断多边形的每一个点，是否在另一个多边形里即可。当然，两个多边形都要用对方的点检测一次。相对来说比较耗时，真实的计算中，可以先用AABB或是圆形碰撞做粗略的检测过滤掉一部分，在做精确的检测。


```
/** 
 * Test polygonA each vertex in polygonB, true inside or false outside 
 * not test through and cross each others 
 */  
static bool TestPolygonPolygon(Array(float)* polygonA, Array(float)* polygonB)  
{  
    bool inside = false;  
  
    for (int i = 0; i < polygonA->length; i += 2)  
    {  
        float x        = AArray_Get(polygonA, i,     float);  
        float y        = AArray_Get(polygonA, i + 1, float);  
        int   preIndex = polygonB->length - 2;  
  
        // test polygonB contains vertex  
        for (int j = 0; j < polygonB->length; j += 2)  
        {  
            float vertexY = AArray_Get(polygonB, j        + 1, float);  
            float preY    = AArray_Get(polygonB, preIndex + 1, float);  
  
            if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))  
            {  
                float vertexX = AArray_Get(polygonB, j, float);  
  
                // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)  
                // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)  
                // if result zero means point (x, y) on vector (preX - vertexX, preY - vertexY)  
                // if result positive means point on left  vector  
                // if result negative means point on right vector  
                if (vertexX + (y - vertexY) / (preY - vertexY) * (AArray_Get(polygonB, preIndex, float) - vertexX) <= x)  
                {  
                    inside = !inside;  
                }  
            }  
  
            preIndex = j;  
        }  
  
        if (inside)  
        {  
            return true;  
        }  
    }  
  
    return inside;  
}
```
在两个多边形，碰撞之前，以上算法是可以计算的。但是存在一种互相穿透的情况，这种检测就会失效，因为可能一个多边形所有的点都在另一个的外面，但是两者确是交织的。这种情况会出现在碰撞过后，继续运动产生的情况。所以刚体的碰撞判断上面的算法即可。
但，这种交织的情况，射线法仍然可以通过某些手段进行判断的。核心的思路就是，计数左边的点，和右边的点，如果两边计数是相等的就是穿透的情况。代码如下：


```
/** 
 * Test polygonA each vertex in polygonB, true inside or false outside 
 * Can test through and cross each others 
 * 
 */  
static bool TestPolygonPolygonFull(Array(float)* polygonA, Array(float)* polygonB)  
{  
    int leftCount  = 0;  
    int rightCount = 0;  
  
    for (int i = 0; i < polygonA->length; i += 2)  
    {  
        float x        = AArray_Get(polygonA, i,     float);  
        float y        = AArray_Get(polygonA, i + 1, float);  
  
        int   preIndex = polygonB->length - 2;  
  
        // test polygonB contains vertex  
        for (int j = 0; j < polygonB->length; j += 2)  
        {  
            float vertexY = AArray_Get(polygonB, j        + 1, float);  
            float preY    = AArray_Get(polygonB, preIndex + 1, float);  
  
            if ((vertexY < y && preY >= y) || (preY < y && vertexY >= y))  
            {  
                float vertexX = AArray_Get(polygonB, j, float);  
  
                // cross product between vector (x - vertexX, y - vertexY) and (preX - vertexX, preY - vertexY)  
                // result is (x - vertexX) * (preY - vertexY) - (y - vertexY) * (preX - vertexX)  
                // if result zero means point (x, y) on vector (preX - vertexX, preY - vertexY)  
                // if result positive means point on left  vector  
                // if result negative means point on right vector  
                if (vertexX + (y - vertexY) / (preY - vertexY) * (AArray_Get(polygonB, preIndex, float) - vertexX) <= x)  
                {  
                    leftCount++;  
                }  
                else  
                {  
                    rightCount++;  
                }  
            }  
  
            preIndex = j;  
        }  
  
        if (leftCount % 2 != 0)  
        {  
            return true;  
        }  
    }  
  
    return leftCount != 0 && leftCount == rightCount;  
}
```
最后，线与线的碰撞，和多边形的检测有些有不同。因为两个都是线的话，就不存在一个封闭的空间，那么久不能用点在多边形内部的规则判断。但，我们仍然使用点与线的位置关系来判断。就是，两条线如果相交，那么各自的两个点，一定是在另一条线的两边的。代码如下：


```
/** 
 * Test one lineA intersect lineB 
 */  
static bool TestLineLine(Array(float)* lineA, Array(float)* lineB)  
{  
    int   flag[2]  = {0, 0};  
    float vertexX1 = AArray_Get(lineB, 0, float);  
    float vertexX2 = AArray_Get(lineB, 2, float);  
    float vertexY1 = AArray_Get(lineB, 1, float);  
    float vertexY2 = AArray_Get(lineB, 3, float);  
  
    for (int i = 0; i < 4; i += 2)  
    {  
        float x = AArray_Get(lineA, i,     float);  
        float y = AArray_Get(lineA, i + 1, float);  
  
        if ((vertexY1 < y && vertexY2 >= y) || (vertexY2 < y && vertexY1 >= y))  
        {  
            // cross product between vector (x - vertexX1, y - vertexY1) and (vertexX2 - vertexX1, vertexY2 - vertexY1)  
            // result is (x - vertexX1) * (vertexY2 - vertexY1) - (y - vertexY1) * (vertexX2 - vertexX1)  
            if (vertexX1 + (y - vertexY1) / (vertexY2 - vertexY1) * (vertexX2 - vertexX1) <= x)  
            {  
                flag[i >> 1] = 1;  
            }  
            else  
            {  
                flag[i >> 1] = 2;  
            }  
        }  
    }  
  
    // test lineA two points both sides of lineB  
    if (flag[0] + flag[1] == 3)  
    {  
        return true;  
    }  
  
  
    flag[0]  = 0;  
    flag[1]  = 0;  
  
    vertexX1 = AArray_Get(lineA, 0, float);  
    vertexX2 = AArray_Get(lineA, 2, float);  
    vertexY1 = AArray_Get(lineA, 1, float);  
    vertexY2 = AArray_Get(lineA, 3, float);  
  
    for (int i = 0; i < 4; i += 2)  
    {  
        float x = AArray_Get(lineB, i,     float);  
        float y = AArray_Get(lineB, i + 1, float);  
  
        if ((vertexY1 < y && vertexY2 >= y) || (vertexY2 < y && vertexY1 >= y))  
        {  
            // cross product between vector (x - vertexX1, y - vertexY1) and (vertexX2 - vertexX1, vertexY2 - vertexY1)  
            // result is (x - vertexX1) * (vertexY2 - vertexY1) - (y - vertexY1) * (vertexX2 - vertexX1)  
            if (vertexX1 + (y - vertexY1) / (vertexY2 - vertexY1) * (vertexX2 - vertexX1) <= x)  
            {  
                flag[i >> 1] = 1;  
            }  
            else  
            {  
                flag[i >> 1] = 2;  
            }  
        }  
    }  
  
    // test lineB two points both sides of lineA  
    return flag[0] + flag[1] == 3;  
}
```
通过，射线算法的判定，不仅可以判断碰撞，还能获得碰撞点的坐标，还能扩展到3D层面。可以就此扩展一个简单的物理碰撞系统了。
