# 【Scikit-Learn】初试开源机器学习工具 - CD's Coding - CSDN博客





2016年03月17日 19:42:05[糖果天王](https://me.csdn.net/okcd00)阅读数：725








啊好久没用Markdown以外的编辑器了，这次随性一点就用xhEditor吧~

嘛，写的确实也挺随(luan)性(xie)的23333

故事大概讲述了一个少年误打误撞硬是生搬硬凑出一个谱聚类的程序（调试过程略去，惨不忍睹）



```
$ pip install -U scikit-learn
$ wget http://scikit-learn.org/stable/_downloads/plot_spectral_biclustering.py
$ wget http://scikit-learn.org/stable/_downloads/bicluster_newsgroups.py
$ python plot_spectral_biclustering.py
```

哎？报错？



```
Traceback (most recent call last):
  File "plot_spectral_biclustering.py", line 24, in <module>
    from matplotlib import pyplot as plt
```



说啥？——我没有“Matplotlib”？哦……没有就装呗



```
$ wget http://scikit-learn.org/stable/_downloads/plot_spectral_coclustering.py
$ jumbo search matplotlib
$ jumbo install python-matplotlib
$ python plot_spectral_coclustering.py
```




scikit里给定的Spectral clustering函数如下：

【plot_spectral_clustering.py】

效果为，画几个圆圈，马赛克化模糊边界，然后用聚类聚出来



```python
"""
===========================================
Spectral clustering for image segmentation
===========================================

In this example, an image with connected circles is generated and
spectral clustering is used to separate the circles.

In these settings, the :ref:`spectral_clustering` approach solves the problem
know as 'normalized graph cuts': the image is seen as a graph of
connected voxels, and the spectral clustering algorithm amounts to
choosing graph cuts defining regions while minimizing the ratio of the
gradient along the cut, and the volume of the region.

As the algorithm tries to balance the volume (ie balance the region
sizes), if we take circles with different sizes, the segmentation fails.

In addition, as there is no useful information in the intensity of the image,
or its gradient, we choose to perform the spectral clustering on a graph
that is only weakly informed by the gradient. This is close to performing
a Voronoi partition of the graph.

In addition, we use the mask of the objects to restrict the graph to the
outline of the objects. In this example, we are interested in
separating the objects one from the other, and not from the background.
"""
print(__doc__)

# Authors:  Emmanuelle Gouillart <emmanuelle.gouillart@normalesup.org>
#           Gael Varoquaux <gael.varoquaux@normalesup.org>
# License: BSD 3 clause

import numpy as np
import matplotlib.pyplot as plt

from sklearn.feature_extraction import image
from sklearn.cluster import spectral_clustering

###############################################################################
l = 100
x, y = np.indices((l, l))

center1 = (28, 24)
center2 = (40, 50)
center3 = (67, 58)
center4 = (24, 70)

radius1, radius2, radius3, radius4 = 16, 14, 15, 14

circle1 = (x - center1[0]) ** 2 + (y - center1[1]) ** 2 < radius1 ** 2
circle2 = (x - center2[0]) ** 2 + (y - center2[1]) ** 2 < radius2 ** 2
circle3 = (x - center3[0]) ** 2 + (y - center3[1]) ** 2 < radius3 ** 2
circle4 = (x - center4[0]) ** 2 + (y - center4[1]) ** 2 < radius4 ** 2

###############################################################################
# 4 circles
img = circle1 + circle2 + circle3 + circle4
mask = img.astype(bool)
img = img.astype(float)

img += 1 + 0.2 * np.random.randn(*img.shape)

# Convert the image into a graph with the value of the gradient on the
# edges.
graph = image.img_to_graph(img, mask=mask)

# Take a decreasing function of the gradient: we take it weakly
# dependent from the gradient the segmentation is close to a voronoi
graph.data = np.exp(-graph.data / graph.data.std())

# Force the solver to be arpack, since amg is numerically
# unstable on this example
labels = spectral_clustering(graph, n_clusters=4, eigen_solver='arpack')
label_im = -np.ones(mask.shape)
label_im[mask] = labels

plt.matshow(img)
plt.matshow(label_im)

###############################################################################
# 2 circles
img = circle1 + circle2
mask = img.astype(bool)
img = img.astype(float)

img += 1 + 0.2 * np.random.randn(*img.shape)

graph = image.img_to_graph(img, mask=mask)
graph.data = np.exp(-graph.data / graph.data.std())

labels = spectral_clustering(graph, n_clusters=2, eigen_solver='arpack')
label_im = -np.ones(mask.shape)
label_im[mask] = labels

plt.matshow(img)
plt.matshow(label_im)

plt.show()
```




因为画的还真心是挺那么有点容易懂……

解释什么的……来这里看，这里这里——【[传送门](http://scikit-learn.org/stable/modules/clustering.html#spectral-clustering)】




然后呢，要把我现在的一些数据做谱聚类，是一个正定矩阵，使用编辑距离作为距离

数据示例及代码如下：

【AAA】



```python
ABCDEFG 	ABCDEHG 	0.142857142857
ABCDEFG 	ABCDEFGI 	0.125
```


【Name】





```python
ABCDE 	1
ABCDEFG 	2
BBBACFG 	3
```






【Test_SpeClustering.py】



```python
import numpy as np
import matplotlib.pyplot as plt

from scipy.sparse import csr_matrix, coo_matrix
from sklearn.feature_extraction import image
from sklearn.cluster import spectral_clustering

def convert(term_dict):
    ''' 
    Convert a dictionary with elements of form ('d1', 't1'): 12 to a CSR type         matrix.
    The element ('d1', 't1'): 12 becomes entry (0, 0) = 12.
    * Conversion from 1-indexed to 0-indexed.
    * d is row
    * t is column.
    '''
    # Create the appropriate format for the COO format.
    data = []
    row = []
    col = []
    for k, v in term_dict.items():
        r = int(k[0])
        c = int(k[1])
        data.append(v)
        row.append(r-1)
        col.append(c-1)
    # Create the COO-matrix
    return coo_matrix((data,(row,col)))
    
if __name__ == '__main__':  
    Rec = {}
    _Rec = {}
    names = [line.strip() for line in file("./Name")];
    for i in xrange(0,len(names)):
        if names[i].split('\t')[1]:
            Rec[names[i].split('\t')[0]] = int(names[i].split('\t')[1])
            _Rec[int(names[i].split('\t')[1])] = names[i].split('\t')[0]
    
    Mat = {}
    lines = [line.strip() for line in file("./AAA")];
    for i in xrange(0,len(lines)):
        # str1,str2 = lines[i].decode('utf-8'),lines[j].decode('utf-8')
        Stra,Strb,dist = lines[i].split('\t')[0],lines[i].split('\t')[1],float(lines[i].split('\t')[2])
        Mat[(str(Rec[Stra]),str(Rec[Strb]))] = dist
        Mat[(str(Rec[Strb]),str(Rec[Stra]))] = dist

    # Force the solver to be arpack, since amg is numerically
    # unstable on this example
    graph = convert(Mat)
    #for each in graph :
    #    print each
    graph.data = np.exp(-graph.data / graph.data.std())
    for num in [10,20,50,100] :
        labels = spectral_clustering(graph, n_clusters=num, assign_labels="kmeans", eigen_solver='arpack')
        fobj = open("./Data/TestLab_%s.txt" % str(num), "a");
        for i in xrange(0,len(labels)):
            fobj.write( "%s\t%s\n" % (_Rec[i+1], labels[i]) )
        fobj.close();
```







