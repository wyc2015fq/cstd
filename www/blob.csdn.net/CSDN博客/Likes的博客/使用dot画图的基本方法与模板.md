# 使用dot画图的基本方法与模板 - Likes的博客 - CSDN博客
2019年01月02日 19:58:23[Lailikes](https://me.csdn.net/songchuwang1868)阅读数：34
### 1、下载dot并配置环境变量
[http://www.graphviz.org/](http://www.graphviz.org/)
### 2、写.dot文件（常用模板）
```cpp
digraph DAG {
rankdir=BT;//竖直排列
node [shape=box];//矩形节点
edge [fontcolor=darkgreen];//边的颜色
Node0[label="hello\nworld\n"];//节点以及节点中显示的内容
Node1[label="let\ngo!\n"];
Node0 -> Node1[label="lalala..."];//->表示有向边，lable显示边上的内容
}
```
### 3、转换成.png
```bash
dot.exe -Tpng -o test.png test.dot
```
### 4、结果
![](https://img-blog.csdnimg.cn/20190102195658922.png)
