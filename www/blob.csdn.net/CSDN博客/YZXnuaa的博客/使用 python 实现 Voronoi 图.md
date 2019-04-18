# 使用 python 实现 Voronoi 图 - YZXnuaa的博客 - CSDN博客
2018年05月07日 17:43:22[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：613
个人分类：[基础算法																[Python库](https://blog.csdn.net/YZXnuaa/article/category/7389269)](https://blog.csdn.net/YZXnuaa/article/category/7523136)

- from PIL import Image  
- import random  
- import math  
- 
- def generate_voronoi_diagram(width, height, num_cells):  
-     image = Image.new("RGB", (width, height))  
-     putpixel = image.putpixel  
-     imgx, imgy = image.size  
-     nx = []  
-     ny = []  
-     nr = []  
-     ng = []  
-     nb = []  
- for i in range(num_cells):  
-         nx.append(random.randrange(imgx))  
-         ny.append(random.randrange(imgy))  
-         nr.append(random.randrange(256))  
-         ng.append(random.randrange(256))  
-         nb.append(random.randrange(256))  
- for y in range(imgy):  
- for x in range(imgx):  
-             dmin = math.hypot(imgx-1, imgy-1)  
-             j = -1
- for i in range(num_cells):  
-                 d = math.hypot(nx[i]-x, ny[i]-y)  
- if d < dmin:  
-                     dmin = d  
-                     j = i  
-             putpixel((x, y), (nr[j], ng[j], nb[j]))  
-     image.save("VoronoiDiagram.png", "PNG")  
-         image.show()  
- 
- generate_voronoi_diagram(500, 500, 25)  
![](https://img-blog.csdn.net/20170407203753512?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE4MzQ4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
