# 关系图︱python 关系网络的可视化NetworkX(与Apple.Turicreate深度契合) - 素质云笔记/Recorder... - CSDN博客





2018年01月03日 17:27:59[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4077








> 
Apple.Turicreate模块中本来是有可视化.show()模块，但是4.0版本之后移除了。 感谢apple工程师耐心+ 

  详细推荐了networkX。于是乎摸索了一下，如何用networkx与Apple.Turicreate互动。


![这里写图片描述](https://img-blog.csdn.net/20180103164127309?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**来一个例子开场：**

```python
import networkx as nx
%matplotlib inline
from turicreate import SGraph, Vertex, Edge ,SFrame

def Networkx2Turi(g,direct = 'directed',weight = False):
    # 是否有向    
    if direct == 'directed':
        draw_g = nx.DiGraph()  
    elif direct == 'undirected':
        draw_g = nx.Graph()  
    elif direct == 'multi.directed':
        draw_g = nx.MultiDiGraph()
    elif direct == 'multi.undirected':
        draw_g = nx.MultiGraph() 
    # 加载数据
    if weight:
        edge_list = [tuple(g_list.values()) for g_list in g.edges[['__src_id','__dst_id','weight']]]
        draw_g.add_weighted_edges_from(edge_list)
    else:
        edge_list = [tuple(g_list.values()) for g_list in g.edges[['__src_id','__dst_id']]]
        draw_g.add_edges_from(edge_list)
    return draw_g

# load data
url = 'https://static.turi.com/datasets/bond/bond_vertices.csv'
vertex_data = SFrame.read_csv(url)
url = 'https://static.turi.com/datasets/bond/bond_edges.csv'
edge_data = SFrame.read_csv(url)
csg = SGraph(vertices=SFrame(vertex_data), edges=edge_data, vid_field='name',
           src_field='src', dst_field='dst')
csg.edges['weight'] = range(len(csg.edges))

# draw directed graph
draw_g = Networkx2Turi(csg,direct = 'directed',weight = True)
nx.draw(draw_g, node_color='y', with_labels=True, node_size=500)
```

![这里写图片描述](https://img-blog.csdn.net/20180103164433137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
# 一、函数Networkx2Turi()

自己简单写了一个划算适用的小函数：
`Networkx2Turi(g,direct = 'directed',weight = False)`- 其中g就是apple.turicreate的一个graph内容；
- direct是选择画图的模式，有向（directed）与无向（undirected），还有一个multi模式下的有向与无向（[具体可见地址](https://networkx.github.io/documentation/networkx-1.10/reference/classes.multigraph.html)）
- weight 

代表是否需要加权，如果选择加权模式，需要在SGraph（）.edges中加入权重列。也就是这句：
```
csg.edges['weight'] 

= range(len(csg.edges))
```

.

# 二、函数nx.draw()

这个函数是networkx中的，主函数：
`nx.draw(draw_g, pos,node_color='y', with_labels=True, node_size=500)  `- draw_g，代表networkx中的graph格式，并不能直接用turicreate的graph。Networkx2Turi()就是这么生成的。
- pos代表每个点的位置，一般来说很难定义这个坐标。
- node_color每个点的颜色，此时的y代表黄色；
- with_labels每个顶点是否带文字内容；
- node_size每个点的大小。

.

# 三、更好地绘图

我们想根据每个点的重要性来判定顶点的大小与颜色，来看一下apple.turicreate中如何获得顶点的度：

```python
# 如何要绘制不同点的颜色
# node_color=range(len(csg.vertices))
def increment_degree(src, edge, dst):
    src['degree'] += 1
    dst['degree'] += 1
    return (src, edge, dst)
csg.vertices['degree'] = 0

csg = csg.triple_apply(increment_degree, mutated_fields=['degree'])
csg.vertices
```

![这里写图片描述](https://img-blog.csdn.net/20180103171920415?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**来看看根据每个顶点的degree来调整颜色以及大小。**
- 调整颜色的案例：

```
node_color=[float(v) for v in csg.vertices['degree']]
nx.draw(draw_g, node_color=node_color, with_labels=True, node_size=500)
```

![这里写图片描述](https://img-blog.csdn.net/20180103172121205?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 调整顶点大小的案例：

```python
# 调整每个的大小
node_size= [i * 150 for i in range(len(csg.vertices)) ]
nx.draw(draw_g, node_color=node_color, with_labels=True, node_size=node_size)
```

![这里写图片描述](https://img-blog.csdn.net/20180103172252727?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**顶点是否带标签信息（`with_labels=False`）：**
![这里写图片描述](https://img-blog.csdn.net/20180103172330234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.
# 四、Networkx中几款图

这两款特别好看，笔者摘录。可看官网： 
[https://networkx.github.io/documentation/networkx-1.10/examples/drawing/index.html](https://networkx.github.io/documentation/networkx-1.10/examples/drawing/index.html)

## 4.1 [Knuth Miles](https://networkx.github.io/documentation/networkx-1.10/examples/drawing/knuth_miles.html)

```python
import networkx as nx


def miles_graph():
    """ Return the cites example graph in miles_dat.txt
        from the Stanford GraphBase.
    """
    # open file miles_dat.txt.gz (or miles_dat.txt)
    import gzip
    fh = gzip.open('knuth_miles.txt.gz','r')

    G=nx.Graph()
    G.position={}
    G.population={}

    cities=[]
    for line in fh.readlines():
        line = line.decode()
        if line.startswith("*"): # skip comments
            continue

        numfind=re.compile("^\d+")

        if numfind.match(line): # this line is distances
            dist=line.split()
            for d in dist:
                G.add_edge(city,cities[i],weight=int(d))
                i=i+1
        else: # this line is a city, position, population
            i=1
            (city,coordpop)=line.split("[")
            cities.insert(0,city)
            (coord,pop)=coordpop.split("]")
            (y,x)=coord.split(",")

            G.add_node(city)
            # assign position - flip x axis for matplotlib, shift origin
            G.position[city]=(-int(x)+7500,int(y)-3000)
            G.population[city]=float(pop)/1000.0
    return G

if __name__ == '__main__':
    import networkx as nx
    import re
    import sys

    G=miles_graph()

    print("Loaded miles_dat.txt containing 128 cities.")
    print("digraph has %d nodes with %d edges"\
          %(nx.number_of_nodes(G),nx.number_of_edges(G)))


    # make new graph of cites, edge if less then 300 miles between them
    H=nx.Graph()
    for v in G:
        H.add_node(v)
    for (u,v,d) in G.edges(data=True):
        if d['weight'] < 300:
            H.add_edge(u,v)

    # draw with matplotlib/pylab

    try:
        import matplotlib.pyplot as plt
        plt.figure(figsize=(8,8))
        # with nodes colored by degree sized by population
        node_color=[float(H.degree(v)) for v in H]
        nx.draw(H,G.position,
             node_size=[G.population[v] for v in H],
             node_color=node_color,
             with_labels=False)

        # scale the axes equally
        plt.xlim(-5000,500)
        plt.ylim(-2000,3500)

        plt.savefig("knuth_miles.png")
    except:
        pass
```

![这里写图片描述](https://img-blog.csdn.net/20180103172436159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

.

## 4.2 [Random Geometric Graph](https://networkx.github.io/documentation/networkx-1.10/examples/drawing/random_geometric_graph.html)

```
import networkx as nx
import matplotlib.pyplot as plt

G=nx.random_geometric_graph(200,0.125)
# position is stored as node attribute data for random_geometric_graph
pos=nx.get_node_attributes(G,'pos')

# find node near center (0.5,0.5)
dmin=1
ncenter=0
for n in pos:
    x,y=pos[n]
    d=(x-0.5)**2+(y-0.5)**2
    if d<dmin:
        ncenter=n
        dmin=d

# color by path length from node near center
p=nx.single_source_shortest_path_length(G,ncenter)
# 主函数
plt.figure(figsize=(8,8))
nx.draw_networkx_edges(G,pos,nodelist=[ncenter],alpha=0.4)
nx.draw_networkx_nodes(G,pos,nodelist=p.keys(),
                       node_size=80,
                       node_color=p.values(),
                       cmap=plt.cm.Reds_r)

# 去除背景颜色
#plt.xlim(-0.05,1.05)
#plt.ylim(-0.05,1.05)
#plt.axis('off')
#plt.savefig('random_geometric_graph.png')
#plt.show()
```

![这里写图片描述](https://img-blog.csdn.net/20180103172529900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**公众号“素质云笔记”定期更新博客内容：**
![这里写图片描述](https://img-blog.csdn.net/20180226155348545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







