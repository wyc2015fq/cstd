//A_ A星 算法 C语言 实现代码
// 版权声明：本文为博主原创文章，未经博主允许不得转载。
//
// 关于A*算法，很早就想写点什么，可是貌似天天在忙活着什么，可事实又没有做什么，真是浮躁啊！所以今晚还是来写一下总结吧！
//
// A*算法是很经典的只能启发式搜索算法，关于只能搜索算法和一般的搜索算法(例如DFS，BFS之类)，在语言描述上的区别，我觉得用《代码大全》中的一句话描述的非常好：“驾驶汽车达到某人家，写成算法是：沿167号高速往南行至Puyallup，从XX出口后往山上开4.5英里，在一个杂货店旁边的红绿灯右转，接着在第一个路口左转，从左边的褐色大房子的车道进去就是；而启发式是：找出上一次我们给你寄的信，照着信上地址开车到这个镇，到了那里你问一下我们房子在那里，这里的每一个人都认识我们，肯定会有人愿意帮助你，如果你找不到人，就找一个电话亭打电话给我们，我们出来接你”。听上去还是有点抽象。那么下面我们具体看看A*算法！
//
// 注：本文的图形以及一些思想借助于http://www.policyalmanac.org/games/aStarTutorial.htm，非常感谢经典文章！
//
// 我们要想从绿色的点到红色的点，需要启发式得去找一条路径，到底怎么去找呢，开始没有办法，只能从开始点慢慢尝试！我们需要定义一个OPEN表，OPEN表中放的是什么呢？就是当前考虑的点，及其周边的点需要添加进来，作为可能的路径上的点。这样说可能有点抽象，那么先看下面：
//
// 我们从起始点开始搜索：
//
// 1：从点A开始，并且把它作为待处理点存入一个OPEN表。你的路径可能会通过它包含的方格，也可能不会。基本上，这是一个待检查方格的列表。
// 2：寻找起点周围(邻居)所有可到达或者可通过的方格，如果有障碍物方格。那么就不需要考虑他们，对于其他的吕剧节点也把他们加入OPEN表。这些邻居节点认当前点为父节点。(父节点的保存是必须的！因为当我们找到最后一个点的时候，需要通过父节点回溯，找到所有点直到开始节点！那么就是一个完整的路径！)
// 3：从开启列表中删除点A，把它加入到一个CLOSE列表，列表中保存所有不需要再次检查的方格。
//
// 在这一点，你应该形成如图的结构。在图中，暗绿色方格是你起始方格的中心。它被用浅蓝色描边，以表示它被加入到关闭列表中了。所有的相邻格现在都在开启列表中，它们被用浅绿色描边。每个方格都有一个灰色指针反指他们的父方格，也就是开始的方格。
//
// 结下来我们应该选择哪一个点继续考虑呢！我想大家应该知道所谓的启发函数，所谓权值之类(此处所谓的权值就是路劲的长度)。YES，我们需要OPEN表中权值F最小的那个点！为什么呢，当然是权值越小，越靠近目标点咯！
//
// 对于权值我们设为F，那么F怎么计算到的！我们有两个项！G和H，
//
// G = 从起点A，沿着产生的路径，移动到网格上指定方格的路径耗费。
// H = 从网格上那个方格移动到终点B的预估移动耗费。这经常被称为启发式的。这样叫的原因是因为它只是个猜测。我们没办法事先知道路径的长度。(但是我们需要知道：虽然只是猜测，但是只要是基于一个统一的标准，相对远近的趋势是不变的！这一点是很重要的！ )
//
// 例如：H值的估计采用“曼哈顿”法，也就是当前的点，到目标点，横向和纵向的格子数相加，就是H值！
//
// ( 注意：对于障碍物我们是不考虑是否跳过的问题！即，障碍物也当做正常的一个格子！这也是H值仅仅是预测的而已的原因！即所谓启发式！ )
//
// 那么对于第一幅图，起始点离终点的H值是：横向相差4个格子，纵向相差0个格子，那么H=4+0=4；
//
// 当然也有其他的办法，例如使用直线距离，sqrt( pow( des.x - src.x ) + pow( des.y - src.y ) )也是可以的~
//
//
// 对于G值！在这个例子，令水平或者垂直移动的耗费为10，对角线方向耗费为14。我们取这些值是因为沿对角线的距离是沿水平或垂直移动耗费的的根号2约1.414倍。为了简化，我们用10和14近似。有时候简化是很重要的~
//
// ( 其实距离只要反应了基本的倍数关系就可以了！ )
//
// 对于起始点以及周围点的G值，H值和F值，下图可以很清晰的看到！( 左上角是F，左下角是G，右下角是H )
//
// 对于G值，只要是横向和竖向的邻居，设为+10，斜向的邻居+14就可以了~计算真的很easy吧~呵呵~
//
// 对于H值，就是数格子就是咯~
//
// F = G + H
//
// 注意上面的邻居节点都加入OPEN表了哦~~~ 起点从OPEN中删除，加入CLOSE中~
//
// 接着计算：
//
// 然后从OPEN表中选择一个F值最小的然后考虑其周围邻居，再循环处理！直到终点加入CLOSE中，寻路结束！或者OPEN空了，没找到路径！
//
// ( 至于我们怎么选出最小的那个点呢！？我们使用堆排序处理的，对于选出最小值是很快的~ )
//
// 可以看到，F最小的是起始点右边的那个点，下面框框表示的~
//
// 然后再考虑其邻居：
// 此时对于其邻居有几种可能性！
// 1：邻居已经在CLOSE表中了，那么不需要考虑了~
// 2：邻居是障碍物，不需要考虑了e
// 3：邻居不在OPEN表中，那么将邻居加入OPEN，并将次邻居的父节点赋值为当前节点
// 4：邻居在OPEN中，那么需要看看此邻居点的G值与当前点的(G值+当前点到邻居点的距离(10或者14))的大小，如果从此点走更近(即G值更小)，那么将此点的父节点换成当前点！(注意：对于同一个点，G值下，那么F必须小！因为H是相同的！)
//
// 下面是进一步的图示：
// 那么我们按照上面的思路，知道终点加入CLOSE表！（ 终极图示如下 ）
// 最终我们可以得到路径：( 之前我们说过，由父节点往前回溯就很easy的得到路径了~ )
// 说了这么多，也不知道说的行不行，还是需要总结一下！
// 总结：
// 1：将起始点加入OPEN表中
// 2：循环直到OPEN为空或者终点加入CLOSE表中
// 否则：找到OPEN表中F值最小的节点(使用堆排序得到小值点)，将此点从OPEN删除，加入CLOSE！
// (此时最小点已经取出，那么需要从新排序OPEN表，是的第一个点是最小F的点！)
// 对8个邻居进行处理：
// 若：1：邻居已经在CLOSE表中了，那么不需要考虑了~
// 2：邻居是障碍物，不需要考虑了e
// 3：邻居不在OPEN表中，那么将邻居加入OPEN，并将次邻居的父节点赋值为当前节点
// 4：邻居在OPEN中，那么需要看看此邻居点的G值与当前点的(G值+当前点到邻居点的距
// 离 (10或者14))的大小，如果从此点走更近(即G值更小)，那么将此点的父节点换成当前
// 点！ (注意：对于同一个点，G值下，那么F必须小！因为H是相同的！)
// 注意：当父节点改变后，OPEN表中的最小点可能会变化，那么需要再次排序得到最小点！
// 3：结束，根据退出循环的条件可以知道到底有没有找到路径！所以下面的工作就easy了~
// 基本的原理就是这样的~
// 下面给一个简单的C语言的演示代码，只是为了演示原理，没有注重其他问题，所以大家莫怪啊~
// 注意：数组中1代表起点，2代表终点，0代表可以通过，3代表障碍物
#include <stdio.h>
#include <stdlib.h>
#define STARTNODE 1
#define ENDNODE 2
#define BARRIER 3
typedef struct AStarNode {
  int s_x; // 坐标(最终输出路径需要)
  int s_y;
  int s_g; // 起点到此点的距离( 由g和h可以得到f，此处f省略，f=g+h )
  int s_h; // 启发函数预测的此点到终点的距离
  int s_style;// 结点类型：起始点，终点，障碍物
  struct AStarNode* s_parent; // 父节点
  int s_is_in_closetable; // 是否在close表中
  int s_is_in_opentable; // 是否在open表中
} AStarNode;
AStarNode map_maze[10][10]; // 结点数组
AStarNode* open_table[100]; // open表
AStarNode* close_table[100]; // close表
int open_node_count; // open表中节点数量
int close_node_count; // close表中结点数量
AStarNode* path_stack[100]; // 保存路径的栈
int top = -1; // 栈顶
// 交换两个元素
//
void swap(int idx1, int idx2)
{
  AStarNode* tmp = open_table[idx1];
  open_table[idx1] = open_table[idx2];
  open_table[idx2] = tmp;
}
// 堆调整
void adjust_heap(int nIndex)
{
  int curr = nIndex;
  int child = curr * 2 + 1; // 得到左孩子idx( 下标从0开始，所有做孩子是curr*2+1 )
  int parent = (curr - 1) / 2; // 得到双亲idx
  if (nIndex < 0 || nIndex >= open_node_count) {
    return;
  }
  // 往下调整( 要比较左右孩子和cuur parent )
  while (child < open_node_count) {
    // 小根堆是双亲值小于孩子值
    if (child + 1 < open_node_count && open_table[child]->s_g + open_table[child]->s_h > open_table[child + 1]->s_g + open_table[child + 1]->s_h) {
      ++child; // 判断左右孩子大小
    }
    if (open_table[curr]->s_g + open_table[curr]->s_h <= open_table[child]->s_g + open_table[child]->s_h) {
      break;
    }
    else {
      swap(child, curr); // 交换节点
      curr = child; // 再判断当前孩子节点
      child = curr * 2 + 1; // 再判断左孩子
    }
  }
  if (curr != nIndex) {
    return;
  }
  // 往上调整( 只需要比较cuur child和parent )
  while (curr != 0) {
    if (open_table[curr]->s_g + open_table[curr]->s_h >= open_table[parent]->s_g + open_table[parent]->s_h) {
      break;
    }
    else {
      swap(curr, parent);
      curr = parent;
      parent = (curr - 1) / 2;
    }
  }
}
// 判断邻居点是否可以进入open表
void insert_to_opentable(int x, int y, AStarNode* curr_node, AStarNode* end_node, int w)
{
  int i;
  if (map_maze[x][y].s_style != BARRIER) { // 不是障碍物
    if (!map_maze[x][y].s_is_in_closetable) { // 不在闭表中
      if (map_maze[x][y].s_is_in_opentable) { // 在open表中
        // 需要判断是否是一条更优化的路径
        if (map_maze[x][y].s_g > curr_node->s_g + w) { // 如果更优化
          map_maze[x][y].s_g = curr_node->s_g + w;
          map_maze[x][y].s_parent = curr_node;
          for (i = 0; i < open_node_count; ++i) {
            if (open_table[i]->s_x == map_maze[x][y].s_x && open_table[i]->s_y == map_maze[x][y].s_y) {
              break;
            }
          }
          adjust_heap(i); // 下面调整点
        }
      }
      else { // 不在open中
        map_maze[x][y].s_g = curr_node->s_g + w;
        map_maze[x][y].s_h = abs(end_node->s_x - x) + abs(end_node->s_y - y);
        map_maze[x][y].s_parent = curr_node;
        map_maze[x][y].s_is_in_opentable = 1;
        open_table[open_node_count++] = &(map_maze[x][y]);
      }
    }
  }
}
// 查找邻居
// 对上下左右8个邻居进行查找
void get_neighbors(AStarNode* curr_node, AStarNode* end_node)
{
  int x = curr_node->s_x;
  int y = curr_node->s_y;
  // 下面对于8个邻居进行处理！
  if ((x + 1) >= 0 && (x + 1) < 10 && y >= 0 && y < 10) {
    insert_to_opentable(x + 1, y, curr_node, end_node, 10);
  }
  if ((x - 1) >= 0 && (x - 1) < 10 && y >= 0 && y < 10) {
    insert_to_opentable(x - 1, y, curr_node, end_node, 10);
  }
  if (x >= 0 && x < 10 && (y + 1) >= 0 && (y + 1) < 10) {
    insert_to_opentable(x, y + 1, curr_node, end_node, 10);
  }
  if (x >= 0 && x < 10 && (y - 1) >= 0 && (y - 1) < 10) {
    insert_to_opentable(x, y - 1, curr_node, end_node, 10);
  }
  if ((x + 1) >= 0 && (x + 1) < 10 && (y + 1) >= 0 && (y + 1) < 10) {
    insert_to_opentable(x + 1, y + 1, curr_node, end_node, 14);
  }
  if ((x + 1) >= 0 && (x + 1) < 10 && (y - 1) >= 0 && (y - 1) < 10) {
    insert_to_opentable(x + 1, y - 1, curr_node, end_node, 14);
  }
  if ((x - 1) >= 0 && (x - 1) < 10 && (y + 1) >= 0 && (y + 1) < 10) {
    insert_to_opentable(x - 1, y + 1, curr_node, end_node, 14);
  }
  if ((x - 1) >= 0 && (x - 1) < 10 && (y - 1) >= 0 && (y - 1) < 10) {
    insert_to_opentable(x - 1, y - 1, curr_node, end_node, 14);
  }
}
int test_astar3()
{
  // 地图数组的定义
  //
  AStarNode* start_node; // 起始点
  AStarNode* end_node; // 结束点
  AStarNode* curr_node; // 当前点
  int is_found; // 是否找到路径
  int maze[][10] = { // 仅仅为了好赋值给map_maze
    { 1, 0, 0, 3, 0, 3, 0, 0, 0, 0 },
    { 0, 0, 3, 0, 0, 3, 0, 3, 0, 3 },
    { 3, 0, 0, 0, 0, 3, 3, 3, 0, 3 },
    { 3, 0, 3, 0, 0, 0, 0, 0, 0, 3 },
    { 3, 0, 0, 0, 0, 3, 0, 0, 0, 3 },
    { 3, 0, 0, 3, 0, 0, 0, 3, 0, 3 },
    { 3, 0, 0, 0, 0, 3, 3, 0, 0, 0 },
    { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0 },
    { 3, 3, 3, 0, 0, 3, 0, 3, 0, 3 },
    { 3, 0, 0, 0, 0, 3, 3, 3, 0, 3 },
  };
  int i, j, x;
  // 下面准备点
  //
  for (i = 0; i < 10; ++i) {
    for (j = 0; j < 10; ++j) {
      map_maze[i][j].s_g = 0;
      map_maze[i][j].s_h = 0;
      map_maze[i][j].s_is_in_closetable = 0;
      map_maze[i][j].s_is_in_opentable = 0;
      map_maze[i][j].s_style = maze[i][j];
      map_maze[i][j].s_x = i;
      map_maze[i][j].s_y = j;
      map_maze[i][j].s_parent = NULL;
      if (map_maze[i][j].s_style == STARTNODE) { // 起点
        start_node = &(map_maze[i][j]);
      }
      else if (map_maze[i][j].s_style == ENDNODE) { // 终点
        end_node = &(map_maze[i][j]);
      }
      printf("%d ", maze[i][j]);
    }
    printf("\n");
  }
  // 下面使用A*算法得到路径
  //
  open_table[open_node_count++] = start_node; // 起始点加入open表
  start_node->s_is_in_opentable = 1; // 加入open表
  start_node->s_g = 0;
  start_node->s_h = abs(end_node->s_x - start_node->s_x) + abs(end_node->s_y - start_node->s_y);
  start_node->s_parent = NULL;
  if (start_node->s_x == end_node->s_x && start_node->s_y == end_node->s_y) {
    printf("起点==终点！\n");
    return 0;
  }
  is_found = 0;
  while (1) {
    // for test
    if (0) {
      for (x = 0; x < open_node_count; ++x) {
        printf("(%d,%d):%d ", open_table[x]->s_x, open_table[x]->s_y, open_table[x]->s_g + open_table[x]->s_h);
      }
      printf("\n\n");
    }
    curr_node = open_table[0]; // open表的第一个点一定是f值最小的点(通过堆排序得到的)
    open_table[0] = open_table[--open_node_count]; // 最后一个点放到第一个点，然后进行堆调整
    adjust_heap(0); // 调整堆
    close_table[close_node_count++] = curr_node; // 当前点加入close表
    curr_node->s_is_in_closetable = 1; // 已经在close表中了
    if (curr_node->s_x == end_node->s_x && curr_node->s_y == end_node->s_y) { // 终点在close中，结束
      is_found = 1;
      break;
    }
    get_neighbors(curr_node, end_node); // 对邻居的处理
    if (open_node_count == 0) { // 没有路径到达
      is_found = 0;
      break;
    }
  }
  if (is_found) {
    curr_node = end_node;
    while (curr_node) {
      path_stack[++top] = curr_node;
      curr_node = curr_node->s_parent;
    }
    while (top >= 0) { // 下面是输出路径看看~
      if (top > 0) {
        printf("(%d,%d)-->", path_stack[top]->s_x, path_stack[top--]->s_y);
      }
      else {
        printf("(%d,%d)", path_stack[top]->s_x, path_stack[top--]->s_y);
      }
    }
  }
  else {
    printf("么有找到路径");
  }
  puts("");
  return 0;
}

