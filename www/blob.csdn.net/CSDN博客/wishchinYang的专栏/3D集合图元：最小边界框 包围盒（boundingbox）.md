# 3D集合图元：最小边界框/包围盒（boundingbox） - wishchinYang的专栏 - CSDN博客
2013年09月15日 21:02:58[wishchin](https://me.csdn.net/wishchin)阅读数：8312

        对于2D边界框的应用时比较广泛地，它为一个简单匹配建立了很小的计算规则，3D模型的boundingbox则比较困难，计算代价较大。对于PCL库的使用则降低了计算难度，三维数值化降低了建模过程，可以使用简单的边界框规则。
        对于 如何获取最大最小值过程：在载入时去 进行一个 简单 一次交换排序，选取最小最大值... 计算边界框.
        引自天行健，君子以自强不息的 文章；
        原文链接：[http://www.cppblog.com/lovedday/archive/2008/02/23/43122.html](http://www.cppblog.com/lovedday/archive/2008/02/23/43122.html)
[](http://www.cppblog.com/lovedday/archive/2008/02/23/43122.html)
**矩形边界框 / 包围盒**
        另一种常见的用来界定物体的几何图元是矩形边界框，矩形边界框可以是与轴对齐的或是任意方向的。轴对齐矩形边界框有一个限制，就是它的边必须垂直于坐标轴。缩写AABB常用来表示axially aligned bounding box(轴对齐矩形边界框)，OBB用来表示oriented bounding
 box(方向矩形边界框)。轴对齐矩形边界框不仅容易创建，而且易于使用。
        一个3D的AABB就是一个简单的六面体，每一边都平行于一个坐标平面。矩形边界框不一定是立方体，它的长、宽、高可以彼此不同。在图12.10中，画出了一些简单的3D物体和它们的AABB。
![](http://www.cppblog.com/images/cppblog_com/lovedday/6197/o_object_and_aabb.jpg)
**AABB的表达方法**
先介绍AABB的一些重要性质和引用这些值时所用到的记法。AABB内的点满足下列等式：
xmin≤ x ≤ xmax
ymin≤ y ≤ ymax
zmin≤ z ≤ zmax
特别重要的两个点为：
**p**min = [xmin   ymin   zmin]
**p**max = [xmax   ymax   zmax]
中心点**c**为：
**c** = (**p**min + **p**max) /2
"尺寸向量"**s**是从**p**min指向**p**max的向量，包含了矩形边界的长、宽、高：
**s** = **p**max - **p**min
还可以求出矩形边界框的"半径向量"**r**，它是从中心指向**p**max的向量：
**r** = **p**max - **c** = **s**/2
明确地定义一个AABB只需要**p**min、**p**max、**c**、**s**、**r**这5个向量中的两个（除**s**和**r**不能配对外，它们中的任意两个都可配对）。在一些情况下，某些配对形式比其他的会更有用。我们建议用**p**min和**p**max表示一个边界框，因为实际应用中，使用它们的频率远高于**c**、**s**、**r**。当然，由**p**min和**p**max计算其余三个中的任意一个都是很容易的。
在我们的C++代码中，使用下面的类表示AABB，这是一个缩略的代码清单。
```cpp
#ifndef AABB3_H
#define AABB3_H
#include "vector3.h"
class cMatrix4x3;
   
//---------------------------------------------------------------------------
    // Implement a 3D axially aligned bounding box
    //---------------------------------------------------------------------------
 class cAABB3
    {
    public:
        cVector3 min, max;
   
    public:
        // query for dimentions
        cVector3 size() const        { return max - min; }
        float     x_size()            { return max.x - min.x; }
        float     y_size()            { return max.y - min.y; }
        float     z_size()            { return max.z - min.z; }
        cVector3 center() const        { return (min + max) * 0.5f; }
   
        // fetch one of the eight corner points
        cVector3 corner(int i) const;
   
        // "Empty" the box, by setting the values to really large/small numbers.
        void empty();
   
        // add a point to the box
        void add(const cVector3& p);
   
        // add an AABB to the box
        void add(const cAABB3& box);
   
        // return true if the box is empty
        bool is_empty() const;
   
        // return true if the box contains a point
        bool contains(const cVector3& p) const;
   
        // transform the box and compute the new AABB
        void set_to_transformed_box(const cAABB3& box, const cMatrix4x3& m);
   
        // return the clostet point on this box to another point
        cVector3 clostet_point_to(const cVector3& p) const;
    };
   
    #endif
```
**计算AABB**
       计算一个顶点集合的AABB是非常简单的，先将最小值和最大值设为"正负无穷大"或任何比实际中用到的数都大或小得多的数。接着，遍历全部点，并扩展边界框直到它包含所有点为止。
       我们在cAABB类中引入了两个辅助函数，第一个函数负责"清空"AABB
```cpp
//---------------------------------------------------------------------------
    // "Empty" the box, by setting the values to really large/small numbers.
    //---------------------------------------------------------------------------
    void cAABB3::empty() 
    {
        const float big_number = 1e37f;
   
        min.x = min.y = min.z = big_number;
        max.x = max.y = max.z = -big_number;
    }
```
第二个函数将单个点"加"到AABB中，并在必要的时候扩展AABB以包含每个点：
```cpp
//---------------------------------------------------------------------------
    // Add a point to the box
    //---------------------------------------------------------------------------
    void cAABB3::add(const cVector3& p)
    {
        // expand the box as necessary to contain the point
        if(p.x < min.x)        min.x = p.x;
        if(p.x > max.x)        max.x = p.x;
        if(p.y < min.y)        min.y = p.y;
        if(p.y > max.y)        max.y = p.y;
        if(p.z < min.z)        min.z = p.z;
        if(p.z > max.z)        max.z = p.z;
    }
```
现在，从一个点集创建矩形边界框，可以使用下面的代码：
```cpp
Listing 12.1: Computing the AABB for a set of points
   
    // Our list of points
    const int n;
    Vector3 list[n];
   
    // First, empty the box
    AABB3 box;
    box.empty();
   
    // Add each point into the box
    for (int i = 0 ; i < n ; ++i) 
      box.add(list[i]);
取得AABB的顶点：
    //--------------------------------------------------------------------------------------
    // Return one of the 8 corner points.  The points are numbered as follows:
    //
    //            6                                7
    //              ------------------------------
    //             /|                           /|
    //            / |                          / |
    //           /  |                         /  |
    //          /   |                        /   |
    //         /    |                       /    |
    //        /     |                      /     |
    //       /      |                     /      |
    //      /       |                    /       |
    //     /        |                   /        |
    //  2 /         |                3 /         |
    //   /----------------------------/          |
    //   |          |                 |          |
    //   |          |                 |          |      +Y
    //   |        4 |                 |          | 
    //   |          |-----------------|----------|      |
    //   |         /                  |         /  5    |
    //   |        /                   |        /        |       +Z
    //   |       /                    |       /         |
    //   |      /                     |      /          |     /
    //   |     /                      |     /           |    /
    //   |    /                       |    /            |   /
    //   |   /                        |   /             |  /
    //   |  /                         |  /              | /
    //   | /                          | /               |/
    //   |/                           |/                ----------------- +X
    //   ------------------------------
    //  0                              1
    //
    // Bit 0 selects min.x vs. max.x
    // Bit 1 selects min.y vs. max.y
    // Bit 2 selects min.z vs. max.z
    //--------------------------------------------------------------------------------------
    cVector3 cAABB3::corner(int i) const
    {
        assert(i >= 0 && i <= 7);    // make sure index is in range
   
        return cVector3((i & 1) ? max.x : min.x,
                        (i & 2) ? max.y : min.y,
                        (i & 4) ? max.z : min.z);
    }
```
其他的相关函数，具体功能详见注释：
```cpp
//---------------------------------------------------------------------------
    // Add an AABB to the box
    //---------------------------------------------------------------------------
    void cAABB3::add(const cAABB3& box)
    {
        // expand the box as necessary
        if(box.min.x < min.x)    min.x = box.min.x;
        if(box.min.x > max.x)    max.x = box.min.x;
        if(box.min.y < min.y)    min.y = box.min.y;
        if(box.min.y > max.y)    max.y = box.min.y;
        if(box.min.z < min.z)    min.z = box.min.z;
        if(box.min.z > max.z)    max.z = box.min.z;
    }
   
    //---------------------------------------------------------------------------
    // Return true if the box is empty
    //---------------------------------------------------------------------------
    bool cAABB3::is_empty() const
    {
        // check if we're inverted on any axis
        return (min.x > max.x) || (min.y > max.y) || (min.z > max.z);
    }
   
    //---------------------------------------------------------------------------
    // Return true if the box contains a point
    //---------------------------------------------------------------------------
    bool cAABB3::contains(const cVector3& p) const
    {
        // check for overlap on each axis
        return (p.x >= min.x) && (p.x <= max.x) &&
               (p.y >= min.y) && (p.y <= max.y) &&
               (p.z >= min.z) && (p.z <= max.z);
    }
   
    //---------------------------------------------------------------------------
    // return the closest point on this box to another point
    //---------------------------------------------------------------------------
    cVector3 cAABB3::clostet_point_to(const cVector3& p) const
    {
        // "push" p into the box, on each dimension.
   
        cVector3 r;
   
        if(p.x < min.x)
            r.x = min.x;
        else if(p.x > max.x)
            r.x = max.x;
        else
            r.x = p.x;
   
        if(p.y < min.y) 
            r.y = min.y;
        else if(p.y > max.y) 
            r.y = max.y;
        else
            r.y = p.y;
   
        if(p.z < min.z)
            r.z = min.z;
        else if(p.z > max.z)
            r.z = max.z;
        else
            r.z = p.z;
   
        return r;
    }
```
**AABB与边界球**
很多情况下，AABB比边界球更适合于做定界球：
（1）计算一个点集的AABB，在编程上更容易实现，并能在较短的时间内完成。计算边界球则困难得多。
（2）对实际世界里的许多物体，AABB提供了一种"更紧凑"的边界。当然，对于某些物体，边界球更好（设想一个本身就是球形的物体）。在极端情况下，AABB的体积可能仅相当于边界球体积的1/2，大部分时候边界球的体积会比矩形框的体积大得多，比较一下电线杆的边界球和AABB就知道了。图12.11所示为不同物体的AABB与边界球的比较。
![](http://www.cppblog.com/images/cppblog_com/lovedday/6197/o_aabb_and_bound_sphere.jpg)
边界球的根本问题是它的形状只有一个自由度----半径，而AABB却有三个自由度----长、宽、高。因此，它可以调节这些自由度以适应不同物体。对图12.11中的大部分物体，除了右上角的星形体外，AABB都比边界球小。对这颗星，边界球也仅比AABB略小一些。通过图12.11，我们可以注意到AABB对物体的方向很敏感。比较下面两支枪的AABB，图中枪的大小都是相同的，只是方向不同而已；还应注意到在这一情况下边界球大小相同，因为边界球对物体方向不敏感。
**变换AABB**
       当物体在虚拟世界中移动时，它的AABB也需要随之移动。此时我们有两个选择----用变换后的物体来重新计算AABB，或者对AABB做和物体同样的变换。所得到的结果不一定是轴对齐的（如果物体旋转），也不一定是盒状的（如果物体发生了扭曲）。不过，通过"变换后的AABB"进行计算要比通过"经过变换后的物体"计算AABB快得多，因为AABB只有8个顶点。
       通过"变换后的AABB"计算不能只是简单地变换8个顶点，也不能通过转换原**p**min和**p**max来得到新的**p**min和**p**max
 ----这样可能会导致xmin > xmax。为了计算新的AABB，必须先变换8个顶点，再从这8个顶点中计算一个新的AABB。
       根据变换的不同，这种方法可能使新边界框比原边界框大许多。例如，在2D中，45度的旋转会大大增加边界框的尺寸，如图12.12所示：
![](http://www.cppblog.com/images/cppblog_com/lovedday/6197/o_aabb_transformed_box.jpg)
       比较图12.12中原AABB（灰色框）和新AABB（右边较大的方框），它是通过旋转后的AABB计算的，新AABB几乎是原来的两倍。注意，如果从旋转后的物体而不是通过旋转后的AABB来计算新AABB，它的大小将和原来的AABB相同。
       可以利用AABB的结构来加快新的AABB的计算速度，而不必先变换8个顶点，再从这8个顶点中计算新AABB。
让我们简单回顾一下3x3矩阵变换一个3D点的过程：
![](http://www.cppblog.com/images/cppblog_com/lovedday/6197/o_3d_transform.jpg)
       设原边界框为xmin，xmax，ymin...，新边界框计算将得到x'min，x'max，y'min...。现在我们的任务就是想办法加快计算x'min的速度，换句话说，我们希望找到m11x+m21y+m31z的最小值，其中[x,
 y, z]是原8个顶点中的任意一个，我们所要做的就是找出这些点经过变换后谁的x坐标最小。看第一个乘积：m11x，为了最小化乘积，必须决定是用xmin还是xmax来代换其中的x。显然，如果m11>0，用xmin能得到最小化乘积；如果m11<0，则用xmax能得到最小化乘积。比较方便的是，不管xmin和xmax中哪个被用来计算xmin，都可以用另外一个来计算xmax。可以对矩阵9个元素中的每个都应用这个计算过程，
如下列代码所示：
```cpp
//---------------------------------------------------------------------------
    // Transform the box and compute the new AABB.  Remember, this always
    // results in an AABB that is at least as big as the origin, and may be
    // considerably bigger.
    //---------------------------------------------------------------------------
    void cAABB3::set_to_transformed_box(const cAABB3& box, const cMatrix4x3& m)
    {
        // if we're empty, then bail.
        if(box.is_empty())
        {
            empty();
            return;
        }  
        // start with the translation portion
        min = max = get_translation(m);   
        // examine each of the 9 matrix elements and compute the new AABB   
        if(m.m11 > 0.0f)
        {
            min.x += m.m11 * box.min.x;
            max.x += m.m11 * box.max.x;
        }
        else
        {
            min.x += m.m11 * box.max.x;
            max.x += m.m11 * box.min.x;
        }
   
        if(m.m21 > 0.0f)
        {
            min.x += m.m21 * box.min.y; 
            max.x += m.m21 * box.max.y;
        }
        else
        {
            min.x += m.m21 * box.max.y; 
            max.x += m.m21 * box.min.y;
        }
   
        if(m.m31 > 0.0f)
        {
            min.x += m.m31 * box.min.z; 
            max.x += m.m31 * box.max.z;
        }
        else
        {
            min.x += m.m31 * box.max.z; 
            max.x += m.m31 * box.min.z;
        }
   
        if(m.m12 > 0.0f) 
        {
            min.y += m.m12 * box.min.x; 
            max.y += m.m12 * box.max.x;
        }
        else
        {
            min.y += m.m12 * box.max.x; 
            max.y += m.m12 * box.min.x;
        }
   
        if(m.m22 > 0.0f)
        {
            min.y += m.m22 * box.min.y; 
            max.y += m.m22 * box.max.y;
        }
        else
        {
            min.y += m.m22 * box.max.y; 
            max.y += m.m22 * box.min.y;
        }
   
        if(m.m32 > 0.0f)
        {
            min.y += m.m32 * box.min.z; 
            max.y += m.m32 * box.max.z;
        }
        else
        {
            min.y += m.m32 * box.max.z; 
            max.y += m.m32 * box.min.z;
        }
   
        if(m.m13 > 0.0f) 
        {
            min.z += m.m13 * box.min.x; 
            max.z += m.m13 * box.max.x;
        }
        else
        {
            min.z += m.m13 * box.max.x; 
            max.z += m.m13 * box.min.x;
        }
   
        if(m.m23 > 0.0f)
        {
            min.z += m.m23 * box.min.y; 
            max.z += m.m23 * box.max.y;
        }
        else
        {
            min.z += m.m23 * box.max.y; 
            max.z += m.m23 * box.min.y;
        }
   
        if(m.m33 > 0.0f)
        {
            min.z += m.m33 * box.min.z; 
            max.z += m.m33 * box.max.z;
        }
        else
        {
            min.z += m.m33 * box.max.z; 
            max.z += m.m33 * box.min.z;
        }
    }
```
**后记**：
         包围盒作为一种约束，和坐标系有关系，世界坐标系标定了包围盒的精度。
