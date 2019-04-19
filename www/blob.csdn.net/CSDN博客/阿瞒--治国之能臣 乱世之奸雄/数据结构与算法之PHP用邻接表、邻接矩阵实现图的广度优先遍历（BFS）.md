# 数据结构与算法之PHP用邻接表、邻接矩阵实现图的广度优先遍历（BFS） - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2019年03月26日 11:55:14[baofenny](https://me.csdn.net/jiaminbao)阅读数：29
个人分类：[数据结构与算法](https://blog.csdn.net/jiaminbao/article/category/8795800)
**一、基本思想**
1）从图中的某个顶点V出发访问并记录；
2）依次访问V的所有邻接顶点；
3）分别从这些邻接点出发，依次访问它们的未被访问过的邻接点，直到图中所有已被访问过的顶点的邻接点都被访问到。
4）重复第3步，直到图中所有顶点都被访问完为止。
**二、图的存储结构**
![](https://images2017.cnblogs.com/blog/953680/201712/953680-20171218165121756-95295455.png)![](https://images2017.cnblogs.com/blog/953680/201712/953680-20171218165256865-1921361649.png)![](https://img-blog.csdnimg.cn/20190326115505892.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ppYW1pbmJhbw==,size_16,color_FFFFFF,t_70)
                     示例图                            图的邻接表存储方式                       图的邻接矩阵存储方式
**三、实现方式**
[伪代码]
1. 初始化队列Q；visited[n] = 0；
2. 访问顶点v；visited[v] = 1；
3. 顶点v入队列Q；
4. while（队列Q非空）  
        v = 队列Q的对头元素出队；
        w = 顶点v的第一个邻接点；
        while（w存在）
            如果w未访问，则访问顶点w；
            visited[w] = 1；
            顶点w入队列Q；
            w = 顶点v的下一个邻接点。
1、邻接表非递归实现
广度优先遍历（BFS）的非递归遍历可以使用队列这个数据结构来实现。写代码前我画了一个非递归遍历入队出队的流程图，如下所示。
![](https://images2017.cnblogs.com/blog/953680/201712/953680-20171218184301037-53523117.png)
<?php
/**
 * 图的广度优先遍历
 * 图的存储结构--邻接表
 */
//实现连通图的广度优先搜索
class Node{
    public $value = null;
    public $next = array();//存储下一个节点位置的数组
    public function __construct($value = null){
        $this->value = $value;
    }
}
class Graph {
    // 记录节点是否已被遍历
    public $visited = [];
    // 图的邻接表数组
    public $graph = [];
    /**
     * 为顶点添加邻接点
     * @param $vertex 顶点v
     * @param $adjvex 顶点v的邻接点
     */
    public function addVertex($vertex, $adjvex)
    {
        $this->graph[$vertex][] = $adjvex;
    }
    /**
     * 非递归
     * @param $v 传入的是第一个需要访问的顶点
     */
    public function breadthFirstSearch($v) {
        // 初始化节点遍历标记
        $vertices = array_keys($this->graph);
        foreach ($vertices as $vertex) {
            $this->visited[$vertex] = 0;
        }
        $this->visited[$v] = 1;     // 设置当前顶点访问过
        $queue[] = $v;
        while (!empty($queue)) {    // 若当前队列不为空
            $current = array_shift($queue);     // 将队对元素出队列
            echo $current . PHP_EOL;            // 打印顶点
            for ($i = 0; $i < count($this->graph[$current]); $i++) { // 查找该顶点的相邻顶点
                $curChildNode = $this->graph[$current][$i];
                if ($this->visited[$curChildNode] == 0) {   // 若是未访问过就处理
                    $queue[] = $curChildNode;               // 将找到的此顶点入队列
                    $this->visited[$curChildNode] = 1;      // 将找到的此顶点标记为已访问
                }
            }
        }
    }
}
// 测试
$vertices = ['v1', 'v2', 'v3', 'v4', 'v5', 'v6', 'v7', 'v8'];
$g = new Graph();
$g->addVertex('v1', 'v2');
$g->addVertex('v1', 'v3');
$g->addVertex('v2', 'v1');
$g->addVertex('v2', 'v4');
$g->addVertex('v2', 'v5');
$g->addVertex('v3', 'v1');
$g->addVertex('v3', 'v6');
$g->addVertex('v3', 'v7');
$g->addVertex('v4', 'v2');
$g->addVertex('v4', 'v8');
$g->addVertex('v5', 'v2');
$g->addVertex('v5', 'v8');
$g->addVertex('v6', 'v3');
$g->addVertex('v6', 'v7');
$g->addVertex('v7', 'v3');
$g->addVertex('v7', 'v6');
$g->addVertex('v8', 'v4');
$g->addVertex('v8', 'v5');
// 非递归
$g->breadthFirstSearch($vertices[0]);
2、邻接矩阵非递归实现
<?php
/**
 * 图的广度优先遍历
 * 图的存储结构--邻接矩阵
 */
class Graph {
    // 存储节点信息
    public $vertices;
    // 存储边信息
    public $arcs;
    // 图的节点数
    public $vexnum;
    // 记录节点是否已被遍历
    public $visited = [];
    // 初始化
    public function __construct($vertices) {
        $this->vertices = $vertices;
        $this->vexnum = count($this->vertices);
        for ($i = 0; $i < $this->vexnum; $i++) {
            for ($j = 0; $j < $this->vexnum; $j++) {
                $this->arcs[$i][$j] = 0;
            }
        }
    }
    // 两个顶点间添加边(无向图)
    public function addEdge($a, $b) {
        if ($a == $b) { // 边的头尾不能为同一节点
            return;
        }
        $this->arcs[$a][$b] = 1;
        $this->arcs[$b][$a] = 1;
    }
    // 非递归
    public function breadthFirstSearch() {
        // 初始化节点遍历标记
        for ($i = 0; $i < $this->vexnum; $i++) {
            $this->visited[$i] = 0;
        }
        $queue = [];
        for ($i = 0; $i < $this->vexnum; $i++) { // 对每一个顶点做循环
            if (!$this->visited[$i]) {      // 若是未访问过就处理
                $this->visited[$i] = 1;     // 设置当前顶点访问过
                echo $this->vertices[$i] . PHP_EOL;   // 打印顶点
                $queue[] = $i;              // 将此顶点入队列
                while (!empty($queue)) {    // 若当前队列不为空
                    $curr = array_shift($queue);    // 将队对元素出队
                    for ($j = 0; $j < $this->vexnum; $j++) {
                        if ($this->arcs[$curr][$j] == 1 && $this->visited[$j] == 0) {
                            $this->visited[$j] = 1; // 将找到的此顶点标记为已访问
                            echo $this->vertices[$j] . PHP_EOL;   // 打印顶点
                            $queue[] = $j;          // 将找到的此顶点入队列
                        }
                    }
                }
            }
        }
    }
}
// 测试
$vertices = ['v1', 'v2', 'v3', 'v4', 'v5', 'v6', 'v7', 'v8'];
$graph = new Graph($vertices);
$graph->addEdge(0, 1); // v1 v2
$graph->addEdge(0, 2); // v1 v3
$graph->addEdge(1, 3); // v2 v4
$graph->addEdge(1, 4); // v2 v5
$graph->addEdge(2, 5); // v3 v6
$graph->addEdge(2, 6); // v3 v7
$graph->addEdge(4, 7); // v5 v8
$graph->addEdge(3, 7); // v4 v8
$graph->breadthFirstSearch();
