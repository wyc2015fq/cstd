# Direct3D学习（五）：基于高度图的地形生成 - 逍遥剑客 - CSDN博客
2007年05月21日 04:35:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：5309
地形生成有许多方法，其中最广泛的就是利用高度图。相信大家对中学学过的地理还有点印象吧？一幅地图不同海拨用不同的颜色表示，即等高线表示法。高度图基于同样的原理，只不过这里的高度值表现为图像中的亮度值。
在高度图中，高度值表现为0~255之间的明暗值，如图所示：
![](http://images.blogcn.com//2007/5/21/4/xoyojank,20070521042100084.JPG)
对应的地形如下：
![](http://images.blogcn.com//2007/5/21/4/xoyojank,20070521042100554.JPG)
**高度图的生成原理**
一般情况下，三维空间中的水平面是xz平面，高度对应y值。所以，可以将高度图的每一个像素对应生成三维空间中的一个顶点，像素的二维坐标值对应三维顶点的(x,z)值，亮度值对应顶点的y值。
当然，如果要将地形居中显示，需要把（x,z）平移一下。注意DX中z轴是指向显示器里面的，所以要取负值。
基本的对应关系如下：
x = pixel.x – image.width/2
z = -(pixel.y – image.height/2)
y = pixel.value/10          //因为255太大，需要适当缩小一下
**地形的绘制**
现在有了一堆顶点了，怎么绘制出来呢？最简单的方法就是画三角形了，如图
![](http://images.blogcn.com//2007/5/21/4/xoyojank,20070521042100880.JPG)绘制出来
![](http://images.blogcn.com//2007/5/21/4/xoyojank,20070521042113755.JPG)
这需要用到索引缓冲，相应算法见程序：
for(int x = 0; x < GetWidth() - 1; x++)
     {
for(int z = 0; z < GetHeight() - 1; z++)
         {
              index._0 = x * GetHeight() + z;
              index._1 = x * GetHeight() + z + 1;
              index._2 = (x + 1) * GetHeight() + z;
              *pIndex++ = index;
              index._0 = (x + 1) * GetHeight() + z;
              index._1 = x * GetHeight() + z + 1;
              index._2 = (x + 1) * GetHeight() + z + 1;
              *pIndex++ = index;
         }
     }
最后，再把地形的纹理帖上，一块世外桃源（荒地？）就出来啦~~~~~~~~~~~~~~
纹理
![](http://images.blogcn.com//2007/5/21/4/xoyojank,20070521042101137.JPG)
最终效果如图：
![](http://images.blogcn.com//2007/5/21/4/xoyojank,20070521042721977.JPG)
![](http://images.blogcn.com//2007/5/21/4/xoyojank,20070521043146401.JPG)
